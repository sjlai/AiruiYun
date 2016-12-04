#include "stdafx.h"
#include "Manage.h"

bool glb_bRun = false;

CManage::CManage()
{
	m_mpAPI[L"PAD_DeviceSign"] = PAD_DeviceSign;
	m_mpAPI[L"DAT_Store"] = DAT_Store;
	m_mpAPI[L"DAT_Store_Products"] = DAT_Store_Products;
	//
	m_pathName = LibGetAppDir();
	m_pCtrlRole = new CControlRole;
	m_pCtrlDevice = new CControlDevice;
	m_pCtrlLogin = new CControlLogin;
	m_pCtrlData = new CControlData;
}


CManage::~CManage()
{
	TplRelease(m_pCtrlRole);
	TplRelease(m_pCtrlDevice);
	TplRelease(m_pCtrlLogin);
	TplRelease(m_pCtrlData);
}

//data file
void CManage::DataFile(bool bSave)
{
	m_pCtrlRole->DataFile(bSave);
	m_pCtrlDevice->DataFile(bSave);
}

void CManage::Log(EnumLogData type, CString logStr)
{

}

//Require process
CString CManage::Proc(CString &strRequire)
{
	CString resStr,keyStr;
	EnumAPI apiType;
	bool bSuc;
	//
	bSuc = false;
	resStr = L"ÇëÇó¸ñÊ½´íÎó£¡";
	//json object
	CJson *pDataJson;
	CJson inJson(strRequire);
	//key string
	inJson.Get(L"require", keyStr);
	apiType =(EnumAPI)TplGetMpData(m_mpAPI, keyStr);
	//require
	pDataJson=inJson.GetDataObj();
	if (apiType > 1000 && apiType < 2000) //PAD
	{
		bSuc = ProcPad(apiType, pDataJson, resStr);
	}
	else if (apiType > 2000 && apiType < 3000) //PC system
	{
	}
	else if (apiType > 3000 && apiType < 4000) //APP system
	{
	}
	else if (apiType > 5000 && apiType < 6000) //manage
	{
		bSuc = ProcManage(apiType, pDataJson, resStr);
	}
	//construct the result string
	resStr=ProcLast(bSuc, resStr);
	return resStr;
}

//initial
void CManage::Init()
{
	//read the data
	DataFile(false);
	glb_bRun = true;
}

//get the current work directory
CString CManage::GetPathName()
{
	return m_pathName;
}

//get the control pointer
CControlDevice *CManage::GetCtrlDevice()
{
	return m_pCtrlDevice;
}
CControlRole *CManage::GetCtrlRole()
{
	return m_pCtrlRole;
}

//////////////////////////////////////////////////////////////////////////
//construct last result string
CString CManage::ProcLast(bool bSuc, CString &reStr)
{
	CString str;
	if (bSuc)
	{
		if (reStr.GetLength()>10)
		{
			str.Format(L"{\"errorCode\":0,\"message\":\"\",\"data\":%s}", reStr);
		}
		else {
			str.Format(L"{\"errorCode\":0,\"message\":\"%s\",\"data\":{}}", reStr);
		}
	}
	else {
		str.Format(L"{\"errorCode\":-1,\"message\":\"%s\",\"data\":{}}", reStr);
	}
	return str;
}

//process the API for PAD
bool CManage::ProcPad(EnumAPI type, CJson *pJsonData, CString &reStr)
{
	bool bSuc;
	//
	bSuc = false;
	switch (type)
	{
	case PAD_DeviceSign:
		bSuc=m_pCtrlLogin->PAD_DeviceSign(pJsonData, reStr);
		break;
	default:
		break;
	}

	return bSuc;
}
//process the API for manage system
bool CManage::ProcManage(EnumAPI type, CJson *pJsonData, CString &reStr)
{
	bool bSuc;
	//
	bSuc = false;
	switch (type)
	{
	case DAT_Store:
		bSuc = m_pCtrlData->DAT_Store(pJsonData, reStr);
		break;
	case DAT_Store_Products:
		bSuc = m_pCtrlData->DAT_Store_Products(pJsonData, reStr);
		break;
	default:
		break;
	}

	return bSuc;
}
