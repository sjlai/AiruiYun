#pragma once
#include "json.h"
//

class CControlLogin
{
private:
	//<token, userID  >
	map<CString, u64> m_mpTokenUser,m_mpTokenStore,m_mpTokenDevice; 
	//

	bool Login(CJson *pJsonData, CString &account,CString &code);
	//ƽ���¼
	bool LoginPad(u64 idt, CString &code, CString &resMsg);
	//�����¼
	bool LoginStore(CString &name, CString &code, CString &resMsg);
public:
	//�豸����Ա��¼
	bool PAD_DeviceSign(CJson *pJsonData,CString &resMsg);

	CControlLogin();
	~CControlLogin();
};

