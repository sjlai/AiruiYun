#include "stdafx.h"
#include "ControlData.h"
#include "Manage.h"

extern CManage *pManage;


//��ȡ���澭Ӫ�����б�
bool CControlData::DAT_Store_Products_List(CJson *pJsonData, CString &resMsg)
{
	bool bOK;
	//require parameters
	int pageID, pageNum;
	u64 dataID;
	bOK = pJsonData->Get(L"pageID", pageID);
	if (!bOK) return false;
	bOK = pJsonData->Get(L"pageNum", pageNum);
	if (!bOK) return false;
	bOK = pJsonData->Get(L"storeID", dataID);
	if (!bOK) return false;

	//get the data object
	CControlRole *pRole;
	CDataStore *pData;
	pRole = pManage->GetCtrlRole();
	pData = (CDataStore *)pRole->GetData(dataID, DAT_STORE);
	if (!TmplGetDataState(pData, resMsg)) return false;

	//get the page list data
	int totalNum, nCurNum;
	CString str;
	str = TmplGetPageData<CDataStore, CDataProduct>(pageID, pageNum, pData, totalNum, nCurNum);

	//get the result
	CJson resJson;
	resJson.Set(L"dataList", str);
	resJson.Set(L"totalNum", totalNum);
	resJson.Set(L"pageID", pageID);
	resJson.Set(L"pageNum", nCurNum);
	resMsg = resJson.Json();
	return true;
}
//ɾ�����澭Ӫ���ݣ���ʾ״̬Ϊɾ����
bool CControlData::DAT_Store_Products_Del(CJson *pJsonData, CString &resMsg)
{
	bool bOK;
	//require parameters
	u64 dataID, storeID;
	CString description;
	bOK = pJsonData->Get(L"dataID", dataID);
	if (!bOK) return false;
	bOK = pJsonData->Get(L"storeID", storeID);
	if (!bOK) return false;
	bOK = pJsonData->Get(L"description", description);
	if (!bOK) return false;

	//get the data object
	CControlRole *pRole;
	CDataStore *pData;
	pRole = pManage->GetCtrlRole();
	pData = (CDataStore *)pRole->GetData(storeID, DAT_STORE);
	if (!TmplGetDataState(pData, resMsg)) return false;

	//get the sub data
	CDataProduct *pProduct = pData->GetProduct(dataID);
	if (!TmplGetDataState(pProduct, resMsg)) return false;

	//get the result
	bOK = DataObjDelOper(resMsg, DAT_STORE, pProduct, description, false);

	//save the store data alone
	DataObjSaveRecord(DAT_STORE, pData, L"ɾ����Ӫ��Ʒ���ݣ�" + description);
	//
	return bOK;
}
//��ӵ�����Ʒ����
bool CControlData::DAT_Store_Products_Add(CJson *pJsonData, CString &resMsg)
{
	CJson *pObj;
	u64 storeID;
	bool bOK;
	//
	bOK = pJsonData->Get(L"storeID", storeID);
	if (!bOK) return false;
	pObj = pJsonData->GetDataObj(L"dataObj");
	if (!pObj) return false;
	//
	//get the control object
	CControlRole *pRole;
	pRole = pManage->GetCtrlRole();
	//get the data object	
	CDataStore *pStore = (CDataStore *)pRole->GetData(storeID, DAT_STORE);
	if (!pStore) {
		resMsg = L"���治����";
		return false;
	}
	if (!pStore->IsNorm()) {
		resMsg = L"�������ݲ��ܲ�����";
		return false;
	}
	//product data
	CDataProduct *pProduct = new CDataProduct;
	bOK = pProduct->ParseJson(pObj);
	if (!bOK) {
		resMsg = L"�������ݸ�ʽ����";
		TplRelease(pProduct);
		return false;
	}
	//���游�����ID
	pProduct->ntmp = pStore->id;
	bOK = DataObjAddOper(resMsg, DAT_STORE_PRODUCT, (CDataBase *&)pProduct, L"��Ӫ��Ʒ�������", false);
	if (bOK) pRole->DataFile_Store(true, pStore->id, true);
	return bOK;
}
