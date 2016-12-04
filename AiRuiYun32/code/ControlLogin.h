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
	//平板登录
	bool LoginPad(u64 idt, CString &code, CString &resMsg);
	//店面登录
	bool LoginStore(CString &name, CString &code, CString &resMsg);
public:
	//设备管理员登录
	bool PAD_DeviceSign(CJson *pJsonData,CString &resMsg);

	CControlLogin();
	~CControlLogin();
};

