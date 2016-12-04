#pragma once
#include "DataDevice.h"
#include "DataRole.h"
//������桢������������豸���豸������·��Ȼ�������

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
		//���������ݲ��ܷ��ʣ���������
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
		resMsg = L"�����ݲ����ڣ�";
		return false;
	}
	if (pData->state == STA_DEL) {
		resMsg = L"��������ɾ����";
		return false;
	}
	if (pData->state == STA_PAUSE) {
		resMsg = L"��������ͣʹ�ã�";
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
		resMsg = L"dataObj�еĹؼ��ֳ���";
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
	//�������ӿ�
	bool DAT_Store(CJson *pJsonData, CString &resMsg);
	//���澭���Ʒ���ݹ���ӿ�
	bool DAT_Store_Products(CJson *pJsonData, CString &resMsg);

public:
	//����ͨ�ò������޸ġ�ɾ��������ģ�飬�жϲ������������ݡ���¼
	//data temporary save a history records file
	void DataObjSaveRecord(EnumDataType type, CDataBase *pBase, CString description);
	//����ɾ����������ģ��
	bool DataObjDelOper(CString &resMsg, EnumDataType type,CDataBase *pData, CString description=L"",bool bSave=true);
	//������Ӳ�������ģ��
	bool DataObjAddOper(CString &resMsg, EnumDataType type,CDataBase *&pData, CString description=L"",bool bSave=true);




private:
	///////////////////////
	bool DAT_Store_Get(CJson *pJsonData, CString &resMsg);
	bool DAT_Store_List(CJson *pJsonData, CString &resMsg);
	//ɾ���������ݣ���ʾ״̬Ϊɾ����
	bool DAT_Store_Del(CJson *pJsonData, CString &resMsg);
	//��ӵ�������
	bool DAT_Store_Add(CJson *pJsonData, CString &resMsg);
	///////////////////////
	bool DAT_Store_Products_List(CJson *pJsonData, CString &resMsg);
	//ɾ���������ݣ���ʾ״̬Ϊɾ����
	bool DAT_Store_Products_Del(CJson *pJsonData, CString &resMsg);
	//��ӵ�������
	bool DAT_Store_Products_Add(CJson *pJsonData, CString &resMsg);




};

