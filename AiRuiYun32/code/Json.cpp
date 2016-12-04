#include "stdafx.h"
#include "Json.h"


CJson::CJson()
{
	type = JSON_NUL;
}
CJson::CJson(CString &str)
{
	type = JSON_NUL;
	Parse(str);
}
//get the data objects
void CJson::KeyData(CString key, CJson &res)
{
	//first key-val
	map<CString, CString >::iterator it, ite;
	ite = keyVal.end();
	it = keyVal.find(key);
	if (it != ite) {
		res.type = JSON_VAL;
		res.strJson = it->second;
		return ;
	}
	//second key-arr
	map<CString, vector<CJson > >::iterator itv, itve;
	itve = keyArr.end();
	itv = keyArr.find(key);
	if (itv != itve) {
		res.type = JSON_ARR;
		res.keyArr[key] = itv->second;
		return ;
	}
	//third key-obj
	map<CString, CJson >::iterator ito, itoe;
	itoe = keyObj.end();
	ito = keyObj.find(key);
	if (ito != itoe) {
		res.type = JSON_OBJ;
		res.keyObj[key] = ito->second;
		return ;
	}
	res.type = JSON_NUL;
}
//get the data
CJson *CJson::GetDataObj(CString kstr)
{
	//third key-obj
	map<CString, CJson >::iterator ito, itoe;
	itoe = keyObj.end();
	ito = keyObj.find(kstr);
	if (ito != itoe) return &ito->second;
	return nullptr;
}
//get key-val
bool CJson::Get(CString key, CString &val)
{
	val = L"";
	CJson obj;
	KeyData(key,obj);
	switch (obj.type)
	{
	case JSON_NUL:
	case JSON_ARR:
	case JSON_OBJ:
		return false;
		break;
	}
	val = obj.strJson;
	return true;
}
//
bool CJson::Get(CString key, int &val)
{
	val = 0;
	CJson obj;
	KeyData(key,obj);
	switch (obj.type)
	{
	case JSON_NUL:
	case JSON_ARR:
	case JSON_OBJ:
		return false;
		break;
	}
	val = _wtoi(obj.strJson);
	return true;
}
//
bool CJson::Get(CString key, u64 &val)
{
	val = 0;
	CJson obj;
	KeyData(key,obj);
	switch (obj.type)
	{
	case JSON_NUL:
	case JSON_ARR:
	case JSON_OBJ:
		return false;
		break;
	}
	val = _wcstoui64(obj.strJson, nullptr, 10);
	return true;
}
//
bool CJson::Get(CString key, float &val)
{
	val = 0.f;
	CJson obj;
	KeyData(key,obj);
	switch (obj.type)
	{
	case JSON_NUL:
	case JSON_ARR:
	case JSON_OBJ:
		return false;
		break;
	}
	val = (float)_wtof(obj.strJson);
	return true;
}
//parse Json
bool CJson::Parse(CString &str)
{
	if (str.IsEmpty()) return false;
	if (type != JSON_NUL) return false;
	//
	map<CString, CString >::iterator it, its, ite;
	CString val, key;
	bool bJson, bArr;
	vector<CString > vcJson;
	map<CString, CString > mpkv;
	//
	strJson = str;
	bJson = LibParseJsonOneData(str, mpkv);
	if (!bJson) return false;
	//判断处理keyval串是否子Json对象
	its = mpkv.begin();
	ite = mpkv.end();
	for (it = its; it != ite; it++)
	{
		key = it->first;
		val = it->second;
		bJson = LibIsJson(val, bArr);
		if (!bJson) {
			val.Trim(L"\"");
			keyVal[key] = val;
			continue;
		}
		//如果是数组，分别对每个数组进行分解
		if (bArr)
		{
			vector<CJson > vcArr;
			//
			bJson = LibParseArrayJsonToVc(val, vcJson);
			if (!bJson) return false;
			int i, n;
			n = (int)vcJson.size();
			for (i = 0; i < n; i++)
			{
				CJson dataSub;
				dataSub.Parse(vcJson[i]);
				vcArr.push_back(dataSub);
			}
			keyArr.insert(make_pair(key, vcArr));
			//				
		}
		else //obj
		{
			CJson data;
			bJson = data.Parse(val);
			if (!bJson) return false;
			keyObj.insert(make_pair(key, data));
		}
	}
	type = JSON_VAL;
	return true;
}
//create Json
CString CJson::Json()
{
	map<CString, CString > mpkv;
	vector<CString > vcJson;
	//
	int i, n, nsize;
	CString key, val, resStr;
	//first key-val
	mpkv = keyVal;
	//second key-array
	map<CString, vector<CJson > >::iterator itv, itvs, itve;
	itvs = keyArr.begin();
	itve = keyArr.end();
	nsize = (int)keyArr.size();
	for (itv = itvs; itv != itve; itv++)
	{
		key = itv->first;
		n = (int)itv->second.size();
		vcJson.clear();
		for (i = 0; i < n; i++)
		{
			resStr = itv->second[i].Json();
			vcJson.push_back(resStr);
		}
		val = LibJson(vcJson);
		mpkv[key] = val;
	}
	if (type==JSON_ARR && nsize == 1) return val;
	//third object
	map<CString, CJson >::iterator it, its, ite;
	its = keyObj.begin();
	ite = keyObj.end();
	for (it = its; it != ite; it++)
	{
		key = it->first;
		val = it->second.Json();
		mpkv[key] = val;
	}
	//
	strJson = LibJson(mpkv);
	return strJson;
}
void CJson::Clear()
{
	strJson = L"";
	type = JSON_NUL;
	keyVal.clear();
	keyArr.clear();
	keyObj.clear();
}
//设置对象数据
void CJson::Set(CString key, CString val)
{
	keyVal[key] = val;
}
void CJson::Set(CString key, int val)
{
	CString str;
	str.Format(L"%d", val);
	keyVal[key] = str;
}
void CJson::Set(CString key, u64 val)
{
	CString str;
	str.Format(L"%I64u", val);
	keyVal[key] = str;
}
void CJson::Set(CString key, float val)
{
	CString str;
	str.Format(L"%f", val);
	keyVal[key] = str;
}
//判断JSON对象关键字是否存在
bool CJson::IsKey(vector<CString > &vcKey)
{
	int i, n;
	map<CString, CString >::iterator it, ite;
	map<CString, vector<CJson >  >::iterator itv, itve;
	map<CString, CJson >::iterator ito, itoe;
	//
	ite = keyVal.end();
	itve = keyArr.end();
	itoe = keyObj.end();
	//
	n = (int)vcKey.size();
	for (i = 0; i < n; i++)
	{
		//first part
		it = keyVal.find(vcKey[i]);
		if (it != ite) {
			continue;
		}
		//second part
		itv = keyArr.find(vcKey[i]);
		if (itv != itve) {
			continue;
		}
		//third part
		ito = keyObj.find(vcKey[i]);
		if (ito != itoe) {
			continue;
		}
		return false;
	}
	return true;
}
//获取数组对象数据,key-val
void CJson::GetMapDataByArr(CString key, vector<map<CString, CString > > &vcmpData)
{
	vcmpData.clear();
	//
	map<CString, vector<CJson >  >::iterator it, ite;
	int i, N;
	CJson *pObj;
	//
	ite = keyArr.end();
	it = keyArr.find(key);
	if (it == ite) return;
	//
	N = (int)it->second.size();
	for (i = 0; i < N; i++)
	{
		pObj = &it->second[i];
		vcmpData.push_back(pObj->keyVal);
	}
}
//设置数组数据,key-Arr
void CJson::SetArrObj(CString key, CJson &data)
{
	map<CString, vector<CJson >  >::iterator it, its, ite;
	//
	type = JSON_ARR;
	ite = keyArr.end();
	it = keyArr.find(key);
	if (it == ite)
	{
		vector<CJson > vcData;
		vcData.push_back(data);
		keyArr[key] = vcData;
	}
	else
	{
		it->second.push_back(data);
	}
}