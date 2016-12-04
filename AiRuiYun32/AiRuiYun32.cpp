// AiRuiYun32.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "AiRuiYun32.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "code/Manage.h"
#include "code/Communication.h"

// 唯一的应用程序对象

CWinApp theApp;
CManage *pManage=nullptr;
CRITICAL_SECTION glb_csFile;
using namespace std;

int main()
{
    int nRetCode = 0;

	//CString strRes=L"good 好东西！";
	//int len;

	//char *bf;
	//bf= CStringToChar(strRes);
	//len = strRes.GetLength();
	//len = strlen(bf);
	//////unicode to utf8
	//char *sbuf = UNICODE_to_UTF8(strRes, len);
	//////
	//strRes = UTF8ToUnicode(sbuf);


    HMODULE hModule = ::GetModuleHandle(nullptr);

    if (hModule != nullptr)
    {
        // 初始化 MFC 并在失败时显示错误
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            // TODO: 更改错误代码以符合您的需要
            wprintf(L"错误: MFC 初始化失败\n");
            nRetCode = 1;
        }
        else
        {
            // TODO: 在此处为应用程序的行为编写代码。
			//////////////////////////////////////////////////////////////////////////
			printf("Start...\n");
			InitializeCriticalSection(&glb_csFile);
			pManage = new CManage();
			pManage->Init();
			//通信工作线程
			commu_create_threads();
			//启动监听线程
			_beginthreadex(NULL, 0, StartServer, pManage, 0, NULL);
        }
    }
    else
    {
        // TODO: 更改错误代码以符合您的需要
        wprintf(L"错误: GetModuleHandle 失败\n");
        nRetCode = 1;
    }
	getchar();
	app_close();
	TplRelease(pManage);
	DeleteCriticalSection(&glb_csFile);
    return nRetCode;
}
