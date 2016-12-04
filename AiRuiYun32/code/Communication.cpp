#include "stdafx.h"
#include "Communication.h"
#include "CommonFun.h"
#include "Manage.h"


extern CManage *pManage;

struct StruThreadInitData
{
	int idThread;
};

struct StruThreadRequire
{
	CString strRequire;
	bool bUTF8;
	SOCKET clientSocket;
	CString ipConnect;
};
//端口号
int g_port = 6680;
//是否退出程序
bool g_bExitSystem = false;
//静态分配处理线程
const int g_NTHTREAD = 2;
HANDLE g_HandelEvent[g_NTHTREAD], g_hThread[g_NTHTREAD];
//消息链表
list<StruThreadRequire *> g_ListRequire[g_NTHTREAD];
//临界保护区
CRITICAL_SECTION g_CriSecCommu[g_NTHTREAD];

//////////////////////////////////////////////////////////////////////////
//退出服务端监听
void commu_exit_server()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	//initial socket
	wVersionRequested = MAKEWORD(1, 1);
	//
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		return ;
	}
	if (LOBYTE(wsaData.wVersion) != 1 ||
		HIBYTE(wsaData.wVersion) != 1) {
		WSACleanup();
		return ;
	}
	SOCKET sockClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//
	char chIP[20];
	GetIP(chIP);
	//
	SOCKADDR_IN addrSrv;
	struct in_addr s_in; // IPv4地址结构体
	//addrSrv.sin_addr.S_un.S_addr = inet_addr("202.115.12.73");
	inet_pton(AF_INET, chIP, (void *)&s_in);
	addrSrv.sin_addr.S_un.S_addr = s_in.S_un.S_addr;
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(g_port);
	int nc = connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	if (nc < 0)
	{
		return ;
	}
	//send the require and recieve the results
	string strRequire = "exit";
	int ns = (int)strRequire.length();
	send(sockClient, strRequire.c_str(), ns, 0);
	//
	closesocket(sockClient);
	WSACleanup();
}
//处理连接请求的线程
unsigned int __stdcall commu_thread_require(LPVOID lParam)
{
	StruThreadInitData *pInitData = (StruThreadInitData *)lParam;
	//
	int idt,len;
	CString strRes,ipConnect;
	StruThreadRequire *pData;
	//
	idt = pInitData->idThread;
	while (true)
	{
		//
		WaitForSingleObject(g_HandelEvent[idt], INFINITE);//等待
		//处理接收的消息
		while (true)
		{
			if (g_ListRequire[idt].size() == 0) break;
			pData=g_ListRequire[idt].back();
			//get results from the manage
			strRes = pManage->Proc(pData->strRequire);
			//send data
			if (pData->bUTF8)
			{
				char *sbuf=UNICODE_to_UTF8(strRes, len);
				send(pData->clientSocket, sbuf, len, 0);
				FreeArr(sbuf);
			} 
			else
			{
				char *sbuf = CStringToChar(strRes,len);
				send(pData->clientSocket, sbuf, len, 0);
				FreeArr(sbuf);
			}
			//
			closesocket(pData->clientSocket);
			//
			delete pData;
			pData = nullptr;
			//数据修改，临界包含
			EnterCriticalSection(&g_CriSecCommu[idt]);
			g_ListRequire[idt].pop_back();
			LeaveCriticalSection(&g_CriSecCommu[idt]);
		}
		//
		if(g_bExitSystem) break;
	}
	delete pInitData;
	pInitData = nullptr;
	return 1;
}
//创建处理线程
void commu_create_threads()
{
	int i;
	//创建线程对应的事件
	for (i = 0; i < g_NTHTREAD;i++)
	{
		g_HandelEvent[i]= CreateEvent(NULL, TRUE, TRUE, NULL);
		ResetEvent(g_HandelEvent[i]);
		//初始化临界保护区
		InitializeCriticalSection(&g_CriSecCommu[i]);
	}
	//创建处理线程
	for (i = 0; i < g_NTHTREAD;i++)
	{
		//创建 计算线程
		StruThreadInitData *pData = new StruThreadInitData;
		pData->idThread = i;
		g_hThread[i]=(HANDLE)_beginthreadex(NULL, 0, commu_thread_require, pData, 0, NULL);
	}
}
//关闭程序退出
void app_close()
{
	int i;
	g_bExitSystem = true;
	//关闭事件
	for (i = 0; i < g_NTHTREAD; i++)
	{
		SetEvent(g_HandelEvent[i]);
	}
	//向自己发送指令，中断服务器请求
	commu_exit_server();
}
//释放通信事件和线程
void commu_release()
{
	int i;
	//等待线程结束
	WaitForMultipleObjects(g_NTHTREAD, g_hThread, TRUE, INFINITE);//当两个线程都返回时，结束 
	//关闭事件
	for (i = 0; i < g_NTHTREAD; i++)
	{
		CloseHandle(g_HandelEvent[i]);
		//释放临界保护区
//		DeleteCriticalSection(&g_CriSecCommu[i]);
	}
}
//
void commu_proc_require(StruThreadRequire *pData)
{
	//判断消息列表，把请求加到消息最少列表（线程）中，并启动线程处理请求
	int i,imin,ns,idt;
	//判断ip链接是否非法，非法退出链接
	if (!pManage->GetCtrlDevice()->ConnectLink(pData->ipConnect))
	{
		TplRelease(pData);
		return;
	}
	//
	imin = 1000000000;
	for (i = 0; i < g_NTHTREAD;i++)
	{
		ns = (int)g_ListRequire[i].size();
		if ( ns == 0) {
			idt = i;
			break;
		}
		//	
		if (ns < imin) {
			imin = ns;
			idt=i;
		}
	}
	//数据修改，临界包含
	EnterCriticalSection(&g_CriSecCommu[idt]);
	g_ListRequire[idt].push_front(pData);
	LeaveCriticalSection(&g_CriSecCommu[idt]);
	//
	SetEvent(g_HandelEvent[idt]);
}
//服务端接口
unsigned int __stdcall StartServer(LPVOID lParam)
{
	CManage *pManage = (CManage*)lParam;
	//
	WSADATA wsaData;
	//Winsock 的版本, 建议用1.1 ,兼容性好
	WORD wVersionRequested = MAKEWORD(1, 1);
	int nResult = WSAStartup(wVersionRequested, &wsaData);
	if (nResult != 0)
	{
		AfxMessageBox(L"Socket server Startup error!");
		return 1;
	}
	//socket------------------------------------------------
	//接口对象
	SOCKET aSocket;
	//寻址相关结构
	sockaddr_in serverSockaddr;
	memset(&serverSockaddr, 0, sizeof(serverSockaddr));
	aSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (aSocket == INVALID_SOCKET)
	{
		AfxMessageBox(L"Socket object create error!");
		return 1;
	}
	//注意，该处非常重要，取值的正确与否决定关闭scoket后端口是否能正常释放
	BOOL bOptVal = TRUE;
	int bOptLen = sizeof(BOOL);
	//设置 socket 选项, SOL_SOCKET 和 SO_REUSEADDR 一起使用, 并且后面的参数如上, 关闭scoket后端口便能正常释放
	setsockopt(aSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&bOptVal, bOptLen);
	//设置超时300ms
	INT nNetTimeout=300;
	setsockopt(aSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));

	//寻址相关结构
	sockaddr_in aSockaddr;
	memset(&aSockaddr, 0, sizeof(aSockaddr));
	aSockaddr.sin_family = AF_INET;
	aSockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	aSockaddr.sin_port = htons((u_short)g_port);
	//绑定: 注意参数的类型转换
	if (bind(aSocket, (sockaddr *)&aSockaddr, sizeof(aSockaddr)) == SOCKET_ERROR)
	{
		AfxMessageBox(L"Socket Bind error!");
		return 1;
	}
	//监听
	if (listen(aSocket, 10) == SOCKET_ERROR)
	{
		AfxMessageBox(L"Socket listen error");
		return 1;
	}
	//最大接收4个字节的指令长度
	const int ns = 1024 * 4;
	char szRecvMsg[ns];
	//
	CString logStr;
	while (1)
	{
		if(g_bExitSystem) break;
		//接收外部连接, 一个连接一个线程处理
		SOCKET clientSocket = accept(aSocket, (sockaddr *)&serverSockaddr, 0);
		if (clientSocket == INVALID_SOCKET) continue;
		//如果链接后，没有发送信息，造成死等状态，链接后最多等待300ms
		memset(szRecvMsg, 0, ns);
		int n = recv(clientSocket, szRecvMsg, ns, 0);
		if (n < 0) {
			if(clientSocket>0) closesocket(clientSocket);
			continue;
		}
		//
		//n = send(clientSocket, szRecvMsg, n, 0);
		//
		szRecvMsg[n] = '\0';
		if (n < 10) {
			logStr = L"非正常接收数据（小于5个字节）！ ";
			if(strcmp(szRecvMsg,"exit")) logStr = L"关闭系统通信模块！";
			pManage->Log(LOG_COMMU,logStr);
			closesocket(clientSocket);
			continue;
		}
		//thread for the new connect
		StruThreadRequire *pData = new StruThreadRequire;
		//判断是否UTF8，进行转换为Unicode码
		if (IsTextUTF8(szRecvMsg, n))
		{
			pData->bUTF8 = true;
			pData->strRequire = UTF8ToUnicode(szRecvMsg);
		}
		else {
			pData->bUTF8 = false;
			pData->strRequire = szRecvMsg;
		}
		pData->strRequire.Trim();
		pData->clientSocket = clientSocket;
		//保存链接IP
		pData->ipConnect = GetConnectIP(pData->clientSocket);
		////////////
		if (1) //dispaly debug information
		{
			char *buf1, *buf2;
			buf1 = CStringToChar(GetDateTime(), n);
			buf2 = CStringToChar(pData->strRequire, n);
			printf("%s %s\n", buf1, buf2);
			FreeArr(buf1);
			FreeArr(buf2);
		}
		//////////////
		commu_proc_require(pData);
		//
	}
	//错误
	logStr = L"服务器接收退出线程！";
	pManage->Log(LOG_COMMU,logStr);
	//关闭
	closesocket(aSocket);
	//当你使用完Winsock接口后，要调用下面的函数对其占用的资源进行释放
	WSACleanup();
	//释放通信模块
	commu_release();
	return 0;
}
