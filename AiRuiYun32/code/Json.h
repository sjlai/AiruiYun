#pragma once
#include "TemplateFun.h"

enum EnumJsonType
{
	JSON_NUL = 0, JSON_VAL, JSON_ARR, JSON_OBJ
};
//通用Json对象数据结构
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
	//设置对象数据
	void Set(CString key, CString val);
	void Set(CString key, int val);
	void Set(CString key, u64 val);
	void Set(CString key, float val);

	/////对外服务接口
	//判断JSON对象关键字是否存在,return true 表示对应上
	bool IsKey(vector<CString > &vcKey);
	//获取数组对象数据,key-val
	void GetMapDataByArr(CString key, vector<map<CString, CString > > &vcmpData);
	//设置数组数据,key-Arr
	void SetArrObj(CString key, CJson &data);
};

