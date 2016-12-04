#pragma once
#include "ControlOperator.h"
#include "DataDiagnosis.h"

//
class CControlRelease
{
public:
	CControlRelease();
	~CControlRelease();

private:
	//�ź�����
	map<u64, CDataRelease* > m_mpRelease;
	//��ǰ�������źŹ��� <regionID, map<releaseID, doctorID> >
	map<int, map<u64,int > > mpRegionRelease;

private:
	//objects
	CControlRoler *m_pCtrlRole;

private:
	//inner variable
	CString m_strPathName,m_strReleaseName; 
private:
	///�ڲ�����
	//�ж�ҽ���ź��Ƿ��ͻ
	bool IsConflictDoctorRelease(int doctorID, u64 startTime, u64 endTime);
	//�ж���������ź��Ƿ��ͻ
	bool IsConflictClinicRelease(int clinicID, int houseID, u64 startTime, u64 endTime);
	//�Զ����������źŵķ����
	int HouseIDAutoRelease(int clinicID, u64 startTime, u64 endTime);
	//�ź�����JSON����
	bool ObjJsonRelease(u64 releaseID, CObjJson &obj);
	//�ź���־
	void LogReleaseOperate(u64 releaseID, EnumOperate opeTpe, CString &strInfo);
	//���ļ��ж�ȡ���ݶ���
	CDataRelease *ReadRelease(u64 releaseID);
	//��ӷź�����, return false is not add because it is existed
	bool AddRelease(CDataRelease *pRelease);
	//ɾ���ź�����
	bool DelRelease(u64 releaseID);

	//�������ӿ�
public:
	//set the other control pointer
	void SetCtrlPt(CControlRoler *pRole);
	///�źŲ���
	//get the doctor release
	CDataRelease *GetRelease(u64 id);
	//��ȡ�ź����ݶ�Ӧ�Ķ���ָ��,return true-OK
	bool ReleaseObjPt(u64 releaseID, CDataRelease *&pRelease, CDataClinic *&pClinic, CDataDoctor *&pDoctor);
	//���������ļ�
	void DataFile(bool bSave);
	//save or read one data
	u64 SaveRelease(CDataRelease *pRelease,bool bOtherOpe=false);

	///Ӧ�ö˵�����ӿ�
public:
	//doctor release diagnosis
	bool ReleaseDiagnosis(CObjJson *pObj, CString &strRes);


};

