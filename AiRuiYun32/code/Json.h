#pragma once
#include "TemplateFun.h"

enum EnumJsonType
{
	JSON_NUL = 0, JSON_VAL, JSON_ARR, JSON_OBJ
};
//ͨ��Json�������ݽṹ
class CJson
{
public:
	//json type
	EnumJsonType type;
	//JSon string data
	CString strJson;
	//key-data
	map<CString, CString > keyVal;
	//key-array
	map<CString, vector<CJson >  > keyArr;
	//key-object
	map<CString, CJson > keyObj;
	/////////////////////////////////////////////////
public:
	CJson();
	CJson(CString &str);
	//parse Json
	bool Parse(CString &str);
	//get Json string by objection
	CString Json();
	//
	void Clear();
	//Get data by key
	void KeyData(CString key, CJson &res);
	//get the data
	CJson *GetDataObj(CString kstr=L"data");
	//get key-val
	bool Get(CString key, CString &val);
	bool Get(CString key, int &val);
	bool Get(CString key, u64 &val);
	bool Get(CString key, float &val);
	//////////////////////////////////////////////////////////////////////////
	//���ö�������
	void Set(CString key, CString val);
	void Set(CString key, int val);
	void Set(CString key, u64 val);
	void Set(CString key, float val);

	/////�������ӿ�
	//�ж�JSON����ؼ����Ƿ����,return true ��ʾ��Ӧ��
	bool IsKey(vector<CString > &vcKey);
	//��ȡ�����������,key-val
	void GetMapDataByArr(CString key, vector<map<CString, CString > > &vcmpData);
	//������������,key-Arr
	void SetArrObj(CString key, CJson &data);
};

