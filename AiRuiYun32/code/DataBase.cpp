#include "stdafx.h"
#include "DataBase.h"

//������������CDataBase��id,name,icon,createTime,exKeyVal
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
		return L"ɾ��";
		break;
	case STA_PAUSE:
		return L"��ͣ";
		break;
	case STA_STOP:
		return L"ͣ��";
		break;
	default:
		break;
	}
	return L"";	
}
//�Ƿ�������ȡ
bool CDataBase::IsNorm()
{
	if (state == STA_OK) return true;
	return false;
}
//��������
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