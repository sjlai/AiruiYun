#include "stdafx.h"
#include "ControlData.h"
#include "Manage.h"

extern CManage *pManage;
//////////////////////////////////////////////////////////////////////////

CControlData::CControlData()
{
}


CControlData::~CControlData()
{
}

//�������ӿ�
bool CControlData::DAT_Store(CJson *pJsonData, CString &resMsg)
{
	bool bOK;
	EnumOperate opeState;
	CString operate; //ADD, GET, DELETE
	//
	bOK = pJsonData->Get(L"operateType", operate);
	if (!bOK) return false;
	opeState = LibGetOperateState(operate);
	if (opeState==OPER_GET)
	{
		return DAT_Store_Get(pJsonData, resMsg);
	}else if (opeState==OPER_LIST)
	{
		return DAT_Store_List(pJsonData, resMsg);
	}
	else if (opeState == OPER_DEL)
	{
		return DAT_Store_Del(pJsonData, resMsg);
	}
	else if (opeState == OPER_ADD)
	{
		return DAT_Store_Add(pJsonData, resMsg);
	}
	return true;
}
//���澭���Ʒ���ݹ���ӿ�
bool CControlData::DAT_Store_Products(CJson *pJsonData, CString &resMsg)
{
	bool bOK;
	EnumOperate opeState;
	CString operate; //ADD, GET, DELETE
					 //
	bOK = pJsonData->Get(L"operateType", operate);
	if (!bOK) return false;
	opeState = LibGetOperateState(operate);
	if (opeState == OPER_LIST)
	{
		return DAT_Store_Products_List(pJsonData, resMsg);
	}
	else if (opeState == OPER_DEL)
	{
		return DAT_Store_Products_Del(pJsonData, resMsg);
	}
	else if (opeState == OPER_ADD)
	{
		return DAT_Store_Products_Add(pJsonData, resMsg);
	}
	return true;
}

//data temporary save a history records file
void CControlData::DataObjSaveRecord(EnumDataType type, CDataBase *pData, CString description)
{
	CControlRole *pCtrlRole;
	//
	pCtrlRole = pManage->GetCtrlRole();
	//
	pData->mpKeyVal[L"recordReason"] = description;
	pData->mpKeyVal[L"recordTime"] = GetDateTime();
	//
	switch (type)
	{
	case DAT_USER:
		break;
	case DAT_MASTER:
		break;
	case DAT_STORE:
		pCtrlRole->DataFile_Store(true, pData->id, true);
		break;
	case DAT_WORKEAR:
		break;
	case DAT_ACTIVITY:
		break;
	default:
		break;
	}
}
//����ɾ����������ģ��
bool CControlData::DataObjDelOper(CString &resMsg, EnumDataType dataType, CDataBase *pData, CString description,bool bSave)
{
	//judge the state of object
	if (!TmplGetDataState(pData, resMsg)) return false;
	//
	pData->state = STA_DEL;
	//���������޸ļ�¼
	if(bSave) DataObjSaveRecord(dataType, pData, description);
	//
	resMsg = L"ɾ���ɹ�";
	return true;
}
//������Ӳ�������ģ��
bool CControlData::DataObjAddOper(CString &resMsg, EnumDataType dataType, CDataBase *&pData, CString description, bool bSave)
{
	bool bOK;
	CString str;
	CControlRole *pCtrlRole = pManage->GetCtrlRole();
	//�ж��ܷ��޸�
	bOK = true;
	CDataBase *pExitData=nullptr;
	CDataStore *pStore=nullptr;
	bool bModify = pData->id > 0 ? true : false;
	if (pData->id>0)
	{
		if (dataType==DAT_STORE)
		{
			pExitData = pCtrlRole->GetData(pData->id, dataType);
		}
		else if (dataType == DAT_STORE_PRODUCT)
		{

			pStore =(CDataStore *) pCtrlRole->GetData( pData->ntmp,DAT_STORE);
			pExitData=TplGetData(pStore->mpProducts, pData->id);			
		}
		else {
			resMsg = L"error. no dataType!";
			TplRelease(pData);
			return false;
		}
		//judge the state of object
		if (!TmplGetDataState(pExitData, resMsg)) return false;
	}
	else {
		if (dataType == DAT_STORE_PRODUCT)
		{
			pStore = (CDataStore *)pCtrlRole->GetData(pData->ntmp, DAT_STORE);
		}
	}
	//add the data
	if (dataType == DAT_STORE_PRODUCT) {
		if (pStore) bOK = TplAddData(pStore->mpProducts, (CDataProduct *)pData);
	}
	else {
		bOK = pCtrlRole->AddData(pData, dataType);
	}
	if (!bOK) {
		resMsg = L"û�е�ǰID���ݣ��޸�ʧ�ܣ�";
		TplRelease(pData);
		return false;
	}
	if (bModify)
	{
		resMsg = L"�޸ĳɹ�";
		str =  L"�޸�.";
	}
	else
	{
		resMsg = L"��ӳɹ�";
		str = L"�½�.";
	}
	//���������޸ļ�¼
	if(bSave) DataObjSaveRecord(dataType, pData, str + description);
	return true;
}

