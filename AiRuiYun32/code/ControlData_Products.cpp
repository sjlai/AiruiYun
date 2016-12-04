#include "stdafx.h"
#include "ControlData.h"
#include "Manage.h"

extern CManage *pManage;


//获取店面经营数据列表
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
//删除店面经营数据（表示状态为删除）
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
	DataObjSaveRecord(DAT_STORE, pData, L"删除经营产品数据：" + description);
	//
	return bOK;
}
//添加店面商品数据
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
		resMsg = L"店面不存在";
		return false;
	}
	if (!pStore->IsNorm()) {
		resMsg = L"店面数据不能操作！";
		return false;
	}
	//product data
	CDataProduct *pProduct = new CDataProduct;
	bOK = pProduct->ParseJson(pObj);
	if (!bOK) {
		resMsg = L"对象数据格式错误！";
		TplRelease(pProduct);
		return false;
	}
	//保存父类对象ID
	pProduct->ntmp = pStore->id;
	bOK = DataObjAddOper(resMsg, DAT_STORE_PRODUCT, (CDataBase *&)pProduct, L"经营产品数据添加", false);
	if (bOK) pRole->DataFile_Store(true, pStore->id, true);
	return bOK;
}
