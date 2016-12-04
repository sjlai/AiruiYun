#pragma once
#include "ControlRole.h"
#include "ControlConnect.h"
#include "ControlLogin.h"
#include "ControlData.h"

//////////////////////////////////////////////////////////////////////////
enum EnumAPI
{
	API_ERR = -1, API_NUL = 0,
	PAD_DeviceSign = 1001,
	DAT_Store = 5001, DAT_Store_Products
};
//
class CManage
{
public:
	CManage();
	~CManage();
private:
	//control data
	CControlRole *m_pCtrlRole;
	CControlDevice *m_pCtrlDevice;
	CControlLogin *m_pCtrlLogin;
	CControlData *m_pCtrlData;

private:
	CString m_pathName;

	//API string
	map<CString, int> m_mpAPI;
private:
	//data file
	void DataFile(bool bSave);

	//construct last result string
	CString ProcLast(bool bSuc,CString &reStr);
	//process the API for control Pad
	bool ProcPad(EnumAPI type, CJson *pJsonData,CString &reStr);
	//process the API for manage system
	bool ProcManage(EnumAPI type, CJson *pJsonData,CString &reStr);


public:
	//initial
	void Init();
	//get the control pointer
	CControlDevice *GetCtrlDevice();
	CControlRole *GetCtrlRole();
//LOG
	void Log(EnumLogData type,CString logStr);

//Require process
	CString Proc(CString &strRequire);
	//get the current work directory
	CString GetPathName();
};

