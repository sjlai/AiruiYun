#pragma once
#include "DataDevice.h"

class CControlDevice
{
private:
	//ƽ���������
	map<u64, CDataPad* > m_mpPad;

	//�豸�����ݣ�����
	map<u64, CDataDevice* > m_mpDevice;

	//�豸����
	map<u64, CDeviceLink* > m_mpLink;
	//state==99 , error, illegel link
	//����IP����Ϣ <ip, deviceID+state(2)> quick search
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

	//���ӵ�IP�Ƿ�������������⹥����Ƿ��������������
	bool ConnectLink(CString &ipStr);
};

