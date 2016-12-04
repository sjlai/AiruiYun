#pragma once
#include "DataDevice.h"
#include "DataRole.h"
//管理店面、计算机、网络设备、设备床、电路板等基础数据

template<class TUp, class TData>
CString TmplGetPageData(int pageID, int pageNum, TUp *pFather, int &totalNum, int &nCurNum)
{
	map<u64, TData *>::iterator it, its, ite;
	TData *pData;
	CString str;
	int n, ns, ne;
	CJson resJson;
	//
	pFather->GetData(its, ite, totalNum);
	//
	ns = (pageID - 1)*pageNum;
	ne = pageID*pageNum;
	str = L"pageData";
	nCurNum = 0;
	for (n = 0, it = its; it != ite; it++, n++)
	{
		pData = it->second;
		//非正常数据不能访问，计数修正
		if (!pData->IsNorm()) {
			--n;
			--totalNum;
			continue;
		}
		if (n < ns || n >= ne) continue;
		//
		CJson json;
		pData->JsonObj(json);
		resJson.SetArrObj(str, json);
		++nCurNum;
	}
	//list Data
	str = resJson.Json();
	return str;
}
//
template<class T>
bool TmplGetDataState(T *pData, CString &resMsg)
{
	if (!pData) {
		resMsg = L"该数据不存在！";
		return false;
	}
	if (pData->state == STA_DEL) {
		resMsg = L"该数据已删除！";
		return false;
	}
	if (pData->state == STA_PAUSE) {
		resMsg = L"该数据暂停使用！";
		return false;
	}
	return true;
}
//
template<class T>
bool TmplParseObj(CJson *pObjJson, T *&pData, CString &resMsg)
{
	pData = new T;
	bool bOK = pData->ParseJson(pObjJson);
	if (!bOK) {
		resMsg = L"dataObj中的关键字出错！";
		TplRelease(pData);
		return false;
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////

class CControlData
{

public:
	CControlData();
	~CControlData();
public:
	//店面管理接口
	bool DAT_Store(CJson *pJsonData, CString &resMsg);
	//店面经验产品数据管理接口
	bool DAT_Store_Products(CJson *pJsonData, CString &resMsg);

public:
	//数据通用操作（修改、删除）处理模块，判断操作并保存数据、记录
	//data temporary save a history records file
	void DataObjSaveRecord(EnumDataType type, CDataBase *pBase, CString description);
	//数据删除操作处理模块
	bool DataObjDelOper(CString &resMsg, EnumDataType type,CDataBase *pData, CString description=L"",bool bSave=true);
	//数据添加操作处理模块
	bool DataObjAddOper(CString &resMsg, EnumDataType type,CDataBase *&pData, CString description=L"",bool bSave=true);




private:
	///////////////////////
	bool DAT_Store_Get(CJson *pJsonData, CString &resMsg);
	bool DAT_Store_List(CJson *pJsonData, CString &resMsg);
	//删除店面数据（表示状态为删除）
	bool DAT_Store_Del(CJson *pJsonData, CString &resMsg);
	//添加店面数据
	bool DAT_Store_Add(CJson *pJsonData, CString &resMsg);
	///////////////////////
	bool DAT_Store_Products_List(CJson *pJsonData, CString &resMsg);
	//删除店面数据（表示状态为删除）
	bool DAT_Store_Products_Del(CJson *pJsonData, CString &resMsg);
	//添加店面数据
	bool DAT_Store_Products_Add(CJson *pJsonData, CString &resMsg);




};

