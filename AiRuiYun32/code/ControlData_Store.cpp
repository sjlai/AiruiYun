#include "stdafx.h"
#include "ControlData.h"
#include "Manage.h"

extern CManage *pManage;
//////////////////////////////////////////////////////////////////////////

//��ȡ����������
bool CControlData::DAT_Store_Get(CJson *pJsonData, CString &resMsg)
{
	bool bOK;
	//require parameter 
	u64 dataID;
	bOK = pJsonData->Get(L"dataID", dataID);
	if (!bOK) return false;

	//get the data object
	CDataStore *pData;
	CControlRole *pRole;
	pRole = pManage->GetCtrlRole();
	pData = (CDataStore *)pRole->GetData(dataID, DAT_STORE);
	if (!TmplGetDataState(pData, resMsg)) return false;

	//get the result
	resMsg = pData->JsonObj();
	return true;
}
//��ȡ�����������б�
bool CControlData::DAT_Store_List(CJson *pJsonData, CString &resMsg)
{
	bool bOK;
	//require parameter 
	int pageID, pageNum;
	bOK = pJsonData->Get(L"pageID", pageID);
	if (!bOK) return false;
	bOK = pJsonData->Get(L"pageNum", pageNum);
	if (!bOK) return false;

	//get the data object
	CControlRole *pRole;
	pRole = pManage->GetCtrlRole();

	//get the list data
	CString str;
	int totalNum, nCurNum;
	str = TmplGetPageData<CControlRole, CDataStore>(pageID, pageNum, pRole, totalNum, nCurNum);

	//get the result
	CJson resJson;
	resJson.Clear();
	resJson.Set(L"dataList", str);
	resJson.Set(L"totalNum", totalNum);
	resJson.Set(L"pageID", pageID);
	resJson.Set(L"pageNum", nCurNum);
	resMsg = resJson.Json();
	return true;
}
////ɾ���������ݣ���ʾ״̬Ϊɾ����
bool CControlData::DAT_Store_Del(CJson *pJsonData, CString &resMsg)
{
	bool bOK;
	//require parameter 
	u64 dataID;
	CString description;
	bOK = pJsonData->Get(L"dataID", dataID);
	if (!bOK) return false;
	bOK = pJsonData->Get(L"description", description);
	if (!bOK) return false;

	//get the data object
	CControlRole *pRole;
	CDataStore *pData;
	pRole = pManage->GetCtrlRole();
	pData = (CDataStore *)pRole->GetData(dataID, DAT_STORE);
	if (!TmplGetDataState(pData, resMsg)) return false;

	//get the result
	return DataObjDelOper(resMsg, DAT_STORE, pData, description);
}
//��ӵ�������
bool CControlData::DAT_Store_Add(CJson *pJsonData, CString &resMsg)
{
	bool bOK;
	//require parameter 
	CJson *pObj;
	pObj = pJsonData->GetDataObj(L"dataObj");
	if (!pObj) return false;

	//get the data object
	CControlRole *pRole;
	CDataStore *pData;
	pRole = pManage->GetCtrlRole();
	if (!TmplParseObj(pObj, pData, resMsg)) return false;

	//get the result
	bOK = DataObjAddOper(resMsg, DAT_STORE, (CDataBase *&)pData);
	return bOK;
}
