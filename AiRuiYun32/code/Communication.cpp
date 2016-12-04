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
//�˿ں�
int g_port = 6680;
//�Ƿ��˳�����
bool g_bExitSystem = false;
//��̬���䴦���߳�
const int g_NTHTREAD = 2;
HANDLE g_HandelEvent[g_NTHTREAD], g_hThread[g_NTHTREAD];
//��Ϣ����
list<StruThreadRequire *> g_ListRequire[g_NTHTREAD];
//�ٽ籣����
CRITICAL_SECTION g_CriSecCommu[g_NTHTREAD];

//////////////////////////////////////////////////////////////////////////
//�˳�����˼���
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
	struct in_addr s_in; // IPv4��ַ�ṹ��
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
//��������������߳�
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
		WaitForSingleObject(g_HandelEvent[idt], INFINITE);//�ȴ�
		//������յ���Ϣ
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
			//�����޸ģ��ٽ����
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
//���������߳�
void commu_create_threads()
{
	int i;
	//�����̶߳�Ӧ���¼�
	for (i = 0; i < g_NTHTREAD;i++)
	{
		g_HandelEvent[i]= CreateEvent(NULL, TRUE, TRUE, NULL);
		ResetEvent(g_HandelEvent[i]);
		//��ʼ���ٽ籣����
		InitializeCriticalSection(&g_CriSecCommu[i]);
	}
	//���������߳�
	for (i = 0; i < g_NTHTREAD;i++)
	{
		//���� �����߳�
		StruThreadInitData *pData = new StruThreadInitData;
		pData->idThread = i;
		g_hThread[i]=(HANDLE)_beginthreadex(NULL, 0, commu_thread_require, pData, 0, NULL);
	}
}
//�رճ����˳�
void app_close()
{
	int i;
	g_bExitSystem = true;
	//�ر��¼�
	for (i = 0; i < g_NTHTREAD; i++)
	{
		SetEvent(g_HandelEvent[i]);
	}
	//���Լ�����ָ��жϷ���������
	commu_exit_server();
}
//�ͷ�ͨ���¼����߳�
void commu_release()
{
	int i;
	//�ȴ��߳̽���
	WaitForMultipleObjects(g_NTHTREAD, g_hThread, TRUE, INFINITE);//�������̶߳�����ʱ������ 
	//�ر��¼�
	for (i = 0; i < g_NTHTREAD; i++)
	{
		CloseHandle(g_HandelEvent[i]);
		//�ͷ��ٽ籣����
//		DeleteCriticalSection(&g_CriSecCommu[i]);
	}
}
//
void commu_proc_require(StruThreadRequire *pData)
{
	//�ж���Ϣ�б�������ӵ���Ϣ�����б��̣߳��У��������̴߳�������
	int i,imin,ns,idt;
	//�ж�ip�����Ƿ�Ƿ����Ƿ��˳�����
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
	//�����޸ģ��ٽ����
	EnterCriticalSection(&g_CriSecCommu[idt]);
	g_ListRequire[idt].push_front(pData);
	LeaveCriticalSection(&g_CriSecCommu[idt]);
	//
	SetEvent(g_HandelEvent[idt]);
}
//����˽ӿ�
unsigned int __stdcall StartServer(LPVOID lParam)
{
	CManage *pManage = (CManage*)lParam;
	//
	WSADATA wsaData;
	//Winsock �İ汾, ������1.1 ,�����Ժ�
	WORD wVersionRequested = MAKEWORD(1, 1);
	int nResult = WSAStartup(wVersionRequested, &wsaData);
	if (nResult != 0)
	{
		AfxMessageBox(L"Socket server Startup error!");
		return 1;
	}
	//socket------------------------------------------------
	//�ӿڶ���
	SOCKET aSocket;
	//Ѱַ��ؽṹ
	sockaddr_in serverSockaddr;
	memset(&serverSockaddr, 0, sizeof(serverSockaddr));
	aSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (aSocket == INVALID_SOCKET)
	{
		AfxMessageBox(L"Socket object create error!");
		return 1;
	}
	//ע�⣬�ô��ǳ���Ҫ��ȡֵ����ȷ�������ر�scoket��˿��Ƿ��������ͷ�
	BOOL bOptVal = TRUE;
	int bOptLen = sizeof(BOOL);
	//���� socket ѡ��, SOL_SOCKET �� SO_REUSEADDR һ��ʹ��, ���Һ���Ĳ�������, �ر�scoket��˿ڱ��������ͷ�
	setsockopt(aSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&bOptVal, bOptLen);
	//���ó�ʱ300ms
	INT nNetTimeout=300;
	setsockopt(aSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));

	//Ѱַ��ؽṹ
	sockaddr_in aSockaddr;
	memset(&aSockaddr, 0, sizeof(aSockaddr));
	aSockaddr.sin_family = AF_INET;
	aSockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	aSockaddr.sin_port = htons((u_short)g_port);
	//��: ע�����������ת��
	if (bind(aSocket, (sockaddr *)&aSockaddr, sizeof(aSockaddr)) == SOCKET_ERROR)
	{
		AfxMessageBox(L"Socket Bind error!");
		return 1;
	}
	//����
	if (listen(aSocket, 10) == SOCKET_ERROR)
	{
		AfxMessageBox(L"Socket listen error");
		return 1;
	}
	//������4���ֽڵ�ָ���
	const int ns = 1024 * 4;
	char szRecvMsg[ns];
	//
	CString logStr;
	while (1)
	{
		if(g_bExitSystem) break;
		//�����ⲿ����, һ������һ���̴߳���
		SOCKET clientSocket = accept(aSocket, (sockaddr *)&serverSockaddr, 0);
		if (clientSocket == INVALID_SOCKET) continue;
		//������Ӻ�û�з�����Ϣ���������״̬�����Ӻ����ȴ�300ms
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
			logStr = L"�������������ݣ�С��5���ֽڣ��� ";
			if(strcmp(szRecvMsg,"exit")) logStr = L"�ر�ϵͳͨ��ģ�飡";
			pManage->Log(LOG_COMMU,logStr);
			closesocket(clientSocket);
			continue;
		}
		//thread for the new connect
		StruThreadRequire *pData = new StruThreadRequire;
		//�ж��Ƿ�UTF8������ת��ΪUnicode��
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
		//��������IP
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
	//����
	logStr = L"�����������˳��̣߳�";
	pManage->Log(LOG_COMMU,logStr);
	//�ر�
	closesocket(aSocket);
	//����ʹ����Winsock�ӿں�Ҫ��������ĺ�������ռ�õ���Դ�����ͷ�
	WSACleanup();
	//�ͷ�ͨ��ģ��
	commu_release();
	return 0;
}
