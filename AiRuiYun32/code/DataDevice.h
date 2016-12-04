#pragma once
#include "database.h"
//设备数据及设备链接状态数据

//device bed，主板后面贴上不干胶编码 
class CDataDevice:public CDataBase
{
public:
	//类型,"BED_BOARD","ANDROID_PAD","STORE_BED"
	CString type;
	//function
	CString function;
	//设备编码
	CString barCode;
	//规格
	CString specfication;
	//版本,硬件平板版本
	CString version;
	//device code for manage
	CString manageCode;
	//pad ID
	u64 padID;
	//board ID
	u64 boardID;
	//生产、出厂时间
	u64 produceTime;
	u64 outFactoryTime;
	//设备工作是时间范围
	u64 startTime;
	u64 endTime;

public:
	CDataDevice();
	~CDataDevice();

	void Serialize(CArchive &ar);
	//device JSON information
	void GetDeviceInfo_Json(CJson &jsonStr);
};

//main board ,平板后面贴上不干胶编码 
class CDataPad :public CDataBase
{
public:
	//类型,"APPLE_PAD","ANDROID_PAD"
	CString type;
	//品牌
	CString brand;
	//厂家信息
	CString factory;
	//平板编码信息
	CString barCode;;
	//规格
	CString specfication;
	//版本,平板版本
	CString version;
	//device code for manage
	CString manageCode;
	//购买日期
	u64 buyTime;
	//出单日期
	u64 outTime;
	//设备工作是时间范围
	u64 startTime;
	u64 endTime;

public:
	CDataPad();
	~CDataPad();

	void Serialize(CArchive &ar);
};


//设备链接数据结构
class CDeviceLink:public CDataBase
{
public:
	//<ip,time>
	map<CString,u64> mpIPTime;
	//heart link time, second,30s
	int heartConnectTime;
	//link state, <mtime,state> 0-link, 1-cut off, 2-test, only save 100 link records
	map<u64,int> mpLinkTime;
public:
	CDeviceLink();
	~CDeviceLink();

	void Serialize(CArchive &ar);
};
