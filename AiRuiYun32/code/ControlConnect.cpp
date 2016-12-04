#include "stdafx.h"
#include "ControlConnect.h"
#include "Manage.h"

extern CManage *pManage;

CControlDevice::CControlDevice()
{
	//defaule
	CDataDevice *pData = new CDataDevice();
	pData->type = L"���ܰ��Ļ�";
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
	//���浽һ���ļ�
	path = pManage->GetPathName() + L"BedDevice\\";
	TplFileData(path, L"BedDevice", m_mpDevice, bSave);
	//���浽�����ļ�
	path = pManage->GetPathName() + L"DeviceConnect\\";
	TplMulFileData(path, L"Connect", m_mpLink, bSave);
	//���浽һ���ļ�
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
//���ӵ�IP�Ƿ�������������⹥����Ƿ��������������
bool CControlDevice::ConnectLink(CString &ipStr)
{
	return true;
	//
	u64 idDevice=TplGetMpData(m_mpIPDeviceID, ipStr);
	//��ʾ��IP�״�����
	if (0 == idDevice) return true;
	//����豸״̬����90����ʾ�豸�����Ǵ���
	int state = idDevice % 100;
	if (state > 90) return false;
	//
	u64 idDev = idDevice / 100;
	//
	//���û���豸���ӵ��ƶˣ���ʾ�Ƿ�����
	CDeviceLink *pLink = GetLink(idDev);
	if (!pLink) return false;
	u64 ct=GetTickCount64();
	//�����豸���Ӽ�¼
	pLink->mpIPTime[ipStr] = ct;
	return true;
	//�������IP��ʱ�����Ƶ������һ���Ӵ���100�����ӣ���ʾ���⹥��state=99�����θ�����
	if (pLink->mpLinkTime.size()>100) //ֻ�����2000�ε����Ӽ�¼
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
		if ((ed-st)<60*1000) //һ����
		{
			//���⹥������ͣ����
			m_mpIPDeviceID[ipStr] = idDev * 100 + 99;
			return false;
		}
	}
	pLink->mpLinkTime[ct] = 0;
	return true;
}

