#pragma once
#include "TemplateFun.h"
#include "Json.h"

//������������CDataBase��id,name,icon,createTime,exKeyVal
class CDataBase
{
public:
	//id
	u64 id;
	//name
	CString name;
	//icon
	CString icon;
	//done time / create the time
	u64 doneTime;
	//state, 0-normal,other denote the different state
	int state;
	//expand key-val
	map<CString, CString > mpKeyVal;

	//temporary variable, not save
	u64 ntmp;
	//JSON key, not save
	vector<CString > vcKey;
public:
	CDataBase();
	~CDataBase();
	void Serialize(CArchive &ar);
	CString GetState();
	//�Ƿ�������ȡ
	bool IsNorm();
};
//��������
class CDataCategory :public CDataBase
{
public:
	//parent ID
	int parentID;
public:
	CDataCategory();
	~CDataCategory();
	void Serialize(CArchive &ar);

};

