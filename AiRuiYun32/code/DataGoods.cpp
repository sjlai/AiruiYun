#include "stdafx.h"
#include "DataGoods.h"


CDataProduct::CDataProduct()
{
	price = 0.f;
	discount = 1.0f;
	vcKey.push_back(L"dataID");
	vcKey.push_back(L"name");
	vcKey.push_back(L"price");
	vcKey.push_back(L"unit");
	vcKey.push_back(L"discount");
	vcKey.push_back(L"description");

}


CDataProduct::~CDataProduct()
{
}

void CDataProduct::Serialize(CArchive &ar)
{
	CDataBase::Serialize(ar);
	//
	if (ar.IsStoring())
	{
		ar << price;
		ar << discount;
		ar << unit;
		ar << description;
	} 
	else
	{
		ar >> price;
		ar >> discount;
		ar >> unit;
		ar >> description;
	}
}

//JSON object
void CDataProduct::JsonObj(CJson &json)
{
	int i;
	//
	json.Clear();
	i = 0;
	json.Set(vcKey[i++], id);
	json.Set(vcKey[i++], name);
	json.Set(vcKey[i++], price);
	json.Set(vcKey[i++], unit);
	json.Set(vcKey[i++], discount);
	json.Set(vcKey[i++], description);
}

//½âÎöJSON¶ÔÏó
bool CDataProduct::ParseJson(CJson *pJson)
{
	bool bOK;
	int i;
	//
	bOK = true;
	i = 0;
	bOK &= pJson->Get(vcKey[i++], id);
	bOK &= pJson->Get(vcKey[i++], name);
	bOK &= pJson->Get(vcKey[i++], price);
	bOK &= pJson->Get(vcKey[i++], unit);
	bOK &= pJson->Get(vcKey[i++], discount);
	bOK &= pJson->Get(vcKey[i++], description);
	return bOK;
}
