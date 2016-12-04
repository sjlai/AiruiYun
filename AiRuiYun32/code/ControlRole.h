#pragma once
#include "DataRole.h"
//

typedef map<u64, CDataStore*>::iterator itStore;

class CControlRole
{
private:
	//user
	map<u64, CDataUser *> m_mpUser;
	//doctor
	map<u64, CDataMaster *> m_mpMaster;
	//clinic
	map<u64, CDataStore*> m_mpStore;
	//worker
	map<u64, CDataWorker*> m_mpWorker;
	//temporary data container
private:
	//////temporary container <other Key,accountID>
	//user
	map<u64, u64 > mpUserPhoneID;
	map<CString, u64 > mpUserAccountID, mpUserEmailID, mpUserThirdAccID;
	//doctor
	map<u64, u64 > mpMasterPhoneID;
	map<CString, u64 > mpMasterAccountID, mpMasterEmailID, mpMasterThirdAccID;
	//worker
	map<u64, u64 > mpWorkerPhoneID;
	map<CString, u64 > mpWorkerAccountID, mpWorkerEmailID, mpWorkerThirdAccID;
	//clinic
	map<u64, u64 > mpStorePhoneID;
	map<CString, u64 > mpStoreAccountID, mpStoreEmailID, mpStoreThirdAccID;



private:
	void DataFile_User(bool bSave);
	void DataFile_Master(bool bSave);
	void DataFile_Worker(bool bSave);
public:
	void DataFile_Store(bool bSave,u64 idt=0,bool bSaveTmp=false);


public:
	//data file
	void DataFile(bool bSave);
	//read the data
	CDataBase *GetData(u64 id, EnumDataType type=DAT_USER);
	//add the data
	bool AddData(CDataBase *pRole, EnumDataType type=DAT_USER,bool bSaveTmp=false);
	//delete the data
	bool DelData(u64 id, EnumDataType type=DAT_USER);
	//
	//read the data for login
	CDataRole *GetDataLogin(CString &roleInfo, EnumDataType type = DAT_USER);
	//
	void GetData(itStore &its, itStore &ite, int &num);

public:
	CControlRole();
	~CControlRole();
};

