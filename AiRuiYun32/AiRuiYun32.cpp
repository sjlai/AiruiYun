// AiRuiYun32.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "AiRuiYun32.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "code/Manage.h"
#include "code/Communication.h"

// Ψһ��Ӧ�ó������

CWinApp theApp;
CManage *pManage=nullptr;
CRITICAL_SECTION glb_csFile;
using namespace std;

int main()
{
    int nRetCode = 0;

	//CString strRes=L"good �ö�����";
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
        // ��ʼ�� MFC ����ʧ��ʱ��ʾ����
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            // TODO: ���Ĵ�������Է���������Ҫ
            wprintf(L"����: MFC ��ʼ��ʧ��\n");
            nRetCode = 1;
        }
        else
        {
            // TODO: �ڴ˴�ΪӦ�ó������Ϊ��д���롣
			//////////////////////////////////////////////////////////////////////////
			printf("Start...\n");
			InitializeCriticalSection(&glb_csFile);
			pManage = new CManage();
			pManage->Init();
			//ͨ�Ź����߳�
			commu_create_threads();
			//���������߳�
			_beginthreadex(NULL, 0, StartServer, pManage, 0, NULL);
        }
    }
    else
    {
        // TODO: ���Ĵ�������Է���������Ҫ
        wprintf(L"����: GetModuleHandle ʧ��\n");
        nRetCode = 1;
    }
	getchar();
	app_close();
	TplRelease(pManage);
	DeleteCriticalSection(&glb_csFile);
    return nRetCode;
}
