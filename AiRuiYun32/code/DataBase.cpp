#include "stdafx.h"
#include "DataBase.h"

//基础管理数据CDataBase，id,name,icon,createTime,exKeyVal
CDataBase::CDataBase()
{
	id = 0;
	doneTime = GetTimeStamp();
	state = STA_OK;
}
CDataBase::~CDataBase()
{
	vcKey.clear();
}
void CDataBase::Serialize(CArchive &ar)
{
	if (ar.IsStoring()) //save
	{
		ar << id;
		ar << name;
		ar << icon;
		ar << doneTime;
		ar << state;
	}
	else //read
	{
		ar >> id;
		ar >> name;
		ar >> icon;
		ar >> doneTime;
		ar >> state;
	}
	TplSerializeContainer(ar, mpKeyVal);
}
CString CDataBase::GetState()
{
	switch (state)
	{
	case STA_DEL:
		return L"删除";
		break;
	case STA_PAUSE:
		return L"暂停";
		break;
	case STA_STOP:
		return L"停用";
		break;
	default:
		break;
	}
	return L"";	
}
//是否正常读取
bool CDataBase::IsNorm()
{
	if (state == STA_OK) return true;
	return false;
}
//分类数据
CDataCategory::CDataCategory()
{
	parentID = 0;
}
CDataCategory::~CDataCategory()
{

}
void CDataCategory::Serialize(CArchive &ar)
{
	CDataBase::Serialize(ar);
	if (ar.IsStoring())
	{
		ar << parentID;
	}
	else
	{
		ar >> parentID;
	}
}