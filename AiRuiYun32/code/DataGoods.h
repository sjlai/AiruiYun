#pragma once
#include "database.h"

class CDataProduct :public CDataBase
{
public:
	float price;
	CString unit;
	float discount;
	CString description;
public:
	CDataProduct();
	~CDataProduct();

	void Serialize(CArchive &ar);
	//JSON object
	void JsonObj(CJson &json);
	//Ω‚ŒˆJSON∂‘œÛ
	bool ParseJson(CJson *pJson);
};

