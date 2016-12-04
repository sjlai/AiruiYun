#include "stdafx.h"
#include "ControlConnect.h"
#include "Manage.h"

extern CManage *pManage;

CControlDevice::CControlDevice()
{
	//defaule
	CDataDevice *pData = new CDataDevice();
	pData->type = L"智能艾灸机";
	pData->id = 1001;
	pData->manageCode = L"12345";
	m_mpDevice[pData->id] = pData;
}


CControlDevice::~CControlDevice()
{
	TplRelease(m_mpDevice);
	TplRelease(m_mpLink);
}

//data file
void CControlDevice::DataFile(bool bSave)
{
	CString path;
	//保存到一个文件
	path = pManage->GetPathName() + L"BedDevice\\";
	TplFileData(path, L"BedDevice", m_mpDevice, bSave);
	//保存到各个文件
	path = pManage->GetPathName() + L"DeviceConnect\\";
	TplMulFileData(path, L"Connect", m_mpLink, bSave);
	//保存到一个文件
	path = pManage->GetPathName() + L"PadData\\";
	TplFileData(path, L"Pad", m_mpPad, bSave);

}

//read the data
CDataDevice *CControlDevice::GetDevice(u64 id)
{
	return TplGetData(m_mpDevice, id);
}
//add the data
bool CControlDevice::AddDevice(CDataDevice *pData)
{
	return TplAddData(m_mpDevice, pData,1000);
}
//delete the data
bool CControlDevice::DelDevice(u64 id)
{
	return TplDelData(m_mpDevice, id);
}
//////////////////////////
//read the data
CDeviceLink *CControlDevice::GetLink(u64 id)
{
	return TplGetData(m_mpLink, id);
}
//add the data
bool CControlDevice::AddLink(CDeviceLink *pData)
{
	return TplAddData(m_mpLink, pData, 1000);
}
//delete the data
bool CControlDevice::DelLink(u64 id)
{
	return TplDelData(m_mpLink, id);
}
//链接的IP是否正常，如果恶意攻击或非法请求，列入黑名单
bool CControlDevice::ConnectLink(CString &ipStr)
{
	return true;
	//
	u64 idDevice=TplGetMpData(m_mpIPDeviceID, ipStr);
	//表示该IP首次链接
	if (0 == idDevice) return true;
	//如果设备状态大于90，表示设备链接是错误
	int state = idDevice % 100;
	if (state > 90) return false;
	//
	u64 idDev = idDevice / 100;
	//
	//如果没有设备链接到云端，表示非法链接
	CDeviceLink *pLink = GetLink(idDev);
	if (!pLink) return false;
	u64 ct=GetTickCount64();
	//保存设备链接记录
	pLink->mpIPTime[ipStr] = ct;
	return true;
	//如果链接IP的时间过于频繁，及一分钟大于100次链接，表示恶意攻击state=99，屏蔽该链接
	if (pLink->mpLinkTime.size()>100) //只保存近2000次的链接记录
	{
		u64 st, ed;
		map<u64, int>::reverse_iterator itr;
		itr = pLink->mpLinkTime.rbegin();
		ed = itr->first;
		for (int k = 0; k < 100;k++)
		{
			++itr;
		}
		st = itr->first;
		if ((ed-st)<60*1000) //一分钟
		{
			//恶意攻击，暂停服务
			m_mpIPDeviceID[ipStr] = idDev * 100 + 99;
			return false;
		}
	}
	pLink->mpLinkTime[ct] = 0;
	return true;
}

