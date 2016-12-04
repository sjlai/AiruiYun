#include "stdafx.h"
#include "ControlLogin.h"
#include "Manage.h"
#include "md5.h"

extern CManage *pManage;
MD5 md5;

CControlLogin::CControlLogin()
{
}


CControlLogin::~CControlLogin()
{
}
//ƽ���¼
bool CControlLogin::LoginPad(u64 idt, CString &code, CString &resMsg)
{
	CDataDevice *pData = pManage->GetCtrlDevice()->GetDevice(idt);
	if (!pData) {
		resMsg = L"û�е�ǰ�豸��";
		return false;
	}
	if (pData->manageCode != code) {
		resMsg = L"�豸�������";
		return false;
	}
	return true;
}
//�����¼
bool CControlLogin::LoginStore(CString &name, CString &code, CString &resMsg)
{
	CDataStore *pData=(CDataStore *)pManage->GetCtrlRole()->GetDataLogin(name, DAT_STORE);
	if (!pData) {
		resMsg = L"û�и��û���";
		return false;
	}
	//
	md5.reset();
	md5.update(code);
	CString md5code = md5.toString();
	//
	if (pData->code!=md5code)
	{
		resMsg = L"�������";
		return false;
	}
	return true;
}
//��¼��type{PAD,USER,STORE, MANAGE (worker)},account,code
bool CControlLogin::PAD_DeviceSign(CJson *pJsonData, CString &resMsg)
{
	CString account, code;
	u64 idv;
	bool bOK;
	//
	bOK = Login(pJsonData, account, code);
	if (!bOK) return false;
	idv = _wcstoi64(account, nullptr, 10);
	CDataDevice *pData = pManage->GetCtrlDevice()->GetDevice(idv);
	if (!pData) {
		resMsg = L"û�е�ǰ�豸��";
		return false;
	}
	if (pData->manageCode != code) {
		resMsg = L"�豸�������";
		return false;
	}
	//get the device information data
	//result
	CJson resJson;
	pData->GetDeviceInfo_Json(resJson);
	resMsg = resJson.Json();
	return true;
}
//
bool CControlLogin::Login(CJson *pJsonData, CString &account,CString &code)
{
	bool bOK;
	//
	bOK = pJsonData->Get(L"account", account);
	if (!bOK) return false;
	bOK = pJsonData->Get(L"code", code);
	if (!bOK) return false;
	//////////////////////////////////////////////////////////////////////////
	return true;
}

