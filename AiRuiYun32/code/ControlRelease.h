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
	//放号数据
	map<u64, CDataRelease* > m_mpRelease;
	//当前按地区放号管理 <regionID, map<releaseID, doctorID> >
	map<int, map<u64,int > > mpRegionRelease;

private:
	//objects
	CControlRoler *m_pCtrlRole;

private:
	//inner variable
	CString m_strPathName,m_strReleaseName; 
private:
	///内部函数
	//判断医生放号是否冲突
	bool IsConflictDoctorRelease(int doctorID, u64 startTime, u64 endTime);
	//判断诊所房间放号是否冲突
	bool IsConflictClinicRelease(int clinicID, int houseID, u64 startTime, u64 endTime);
	//自动分配诊所放号的房间号
	int HouseIDAutoRelease(int clinicID, u64 startTime, u64 endTime);
	//放号数据JSON对象
	bool ObjJsonRelease(u64 releaseID, CObjJson &obj);
	//放号日志
	void LogReleaseOperate(u64 releaseID, EnumOperate opeTpe, CString &strInfo);
	//从文件中读取数据对象
	CDataRelease *ReadRelease(u64 releaseID);
	//添加放号数据, return false is not add because it is existed
	bool AddRelease(CDataRelease *pRelease);
	//删除放号数据
	bool DelRelease(u64 releaseID);

	//对外服务接口
public:
	//set the other control pointer
	void SetCtrlPt(CControlRoler *pRole);
	///放号操作
	//get the doctor release
	CDataRelease *GetRelease(u64 id);
	//读取放号数据对应的对象指针,return true-OK
	bool ReleaseObjPt(u64 releaseID, CDataRelease *&pRelease, CDataClinic *&pClinic, CDataDoctor *&pDoctor);
	//保存数据文件
	void DataFile(bool bSave);
	//save or read one data
	u64 SaveRelease(CDataRelease *pRelease,bool bOtherOpe=false);

	///应用端的请求接口
public:
	//doctor release diagnosis
	bool ReleaseDiagnosis(CObjJson *pObj, CString &strRes);


};

