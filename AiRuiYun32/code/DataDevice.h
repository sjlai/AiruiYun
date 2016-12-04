#pragma once
#include "database.h"
//�豸���ݼ��豸����״̬����

//device bed������������ϲ��ɽ����� 
class CDataDevice:public CDataBase
{
public:
	//����,"BED_BOARD","ANDROID_PAD","STORE_BED"
	CString type;
	//function
	CString function;
	//�豸����
	CString barCode;
	//���
	CString specfication;
	//�汾,Ӳ��ƽ��汾
	CString version;
	//device code for manage
	CString manageCode;
	//pad ID
	u64 padID;
	//board ID
	u64 boardID;
	//����������ʱ��
	u64 produceTime;
	u64 outFactoryTime;
	//�豸������ʱ�䷶Χ
	u64 startTime;
	u64 endTime;

public:
	CDataDevice();
	~CDataDevice();

	void Serialize(CArchive &ar);
	//device JSON information
	void GetDeviceInfo_Json(CJson &jsonStr);
};

//main board ,ƽ��������ϲ��ɽ����� 
class CDataPad :public CDataBase
{
public:
	//����,"APPLE_PAD","ANDROID_PAD"
	CString type;
	//Ʒ��
	CString brand;
	//������Ϣ
	CString factory;
	//ƽ�������Ϣ
	CString barCode;;
	//���
	CString specfication;
	//�汾,ƽ��汾
	CString version;
	//device code for manage
	CString manageCode;
	//��������
	u64 buyTime;
	//��������
	u64 outTime;
	//�豸������ʱ�䷶Χ
	u64 startTime;
	u64 endTime;

public:
	CDataPad();
	~CDataPad();

	void Serialize(CArchive &ar);
};


//�豸�������ݽṹ
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
