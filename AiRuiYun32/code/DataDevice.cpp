#include "stdafx.h"
#include "DataDevice.h"


CDataDevice::CDataDevice()
{
	boardID = padID = 0;
	produceTime = outFactoryTime = 0;
	startTime = endTime = 0;
}

CDataDevice::~CDataDevice()
{
}

void CDataDevice::Serialize(CArchive &ar)
{
	CDataBase::Serialize(ar);
	//
	if (ar.IsStoring())
	{
		ar << type;
		ar << function;
		ar << barCode;
		ar << specfication;
		ar << version;
		ar << manageCode;
		ar << padID;
		ar << boardID;
		ar << produceTime;
		ar << outFactoryTime;
		ar << startTime;
		ar << endTime;
	} 
	else
	{
		ar >> type;
		ar >> function;
		ar >> barCode;
		ar >> specfication;
		ar >> version;
		ar >> manageCode;
		ar >> padID;
		ar >> boardID;
		ar >> produceTime;
		ar >> outFactoryTime;
		ar >> startTime;
		ar >> endTime;
	}
}
//device JSON information
void CDataDevice::GetDeviceInfo_Json(CJson &jsonStr)
{
	jsonStr.Clear();
	jsonStr.Set(L"deviceID", id);
	jsonStr.Set(L"type", type);
	jsonStr.Set(L"function", function);
	jsonStr.Set(L"specfication", specfication);
	jsonStr.Set(L"padID", padID);
	jsonStr.Set(L"boardID", boardID);
	jsonStr.Set(L"produceTime", produceTime);
	jsonStr.Set(L"outputTime", outFactoryTime);
	jsonStr.Set(L"startTime", startTime);
	jsonStr.Set(L"endTime", endTime);
}
//////////////////////////////////////////////////////////////////////////
CDataPad::CDataPad()
{
	buyTime = outTime = 0;
	startTime = endTime = 0;
}
CDataPad::~CDataPad()
{

}

void CDataPad::Serialize(CArchive &ar)
{
	CDataBase::Serialize(ar);
	if (ar.IsStoring())
	{
		ar << type;
		ar << brand;
		ar << factory;
		ar << barCode;
		ar << specfication;
		ar << version;
		ar << manageCode;
		ar << buyTime;
		ar << outTime;
		ar << startTime;
		ar << endTime;
	} 
	else
	{
		ar >> type;
		ar >> brand;
		ar >> factory;
		ar >> barCode;
		ar >> specfication;
		ar >> version;
		ar >> manageCode;
		ar >> buyTime;
		ar >> outTime;
		ar >> startTime;
		ar >> endTime;
	}
}


//////////////////////////////////////////////////////////////////////////
CDeviceLink::CDeviceLink()
{

}
CDeviceLink::~CDeviceLink()
{

}

void CDeviceLink::Serialize(CArchive &ar)
{
	CDataBase::Serialize(ar);
	//
	if (ar.IsStoring())
	{
		ar << heartConnectTime;
	} 
	else
	{
		ar >> heartConnectTime;
	}
	TplSerializeContainer(ar, mpIPTime);
	TplSerializeContainer(ar, mpLinkTime);
}
