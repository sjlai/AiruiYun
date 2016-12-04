#pragma once
#include "DataDevice.h"

class CControlDevice
{
private:
	//平板电脑数据
	map<u64, CDataPad* > m_mpPad;

	//设备床数据（含）
	map<u64, CDataDevice* > m_mpDevice;

	//设备链接
	map<u64, CDeviceLink* > m_mpLink;
	//state==99 , error, illegel link
	//链接IP的信息 <ip, deviceID+state(2)> quick search
	map<CString, u64> m_mpIPDeviceID;
public:
	CControlDevice();
	~CControlDevice();
	//data file
	void DataFile(bool bSave);

	//read the data
	CDataDevice *GetDevice(u64 id);
	//add the data
	bool AddDevice(CDataDevice *pData);
	//delete the data
	bool DelDevice(u64 id);
	//read the data
	CDeviceLink *GetLink(u64 id);
	//add the data
	bool AddLink(CDeviceLink *pData);
	//delete the data
	bool DelLink(u64 id);

	//链接的IP是否正常，如果恶意攻击或非法请求，列入黑名单
	bool ConnectLink(CString &ipStr);
};

