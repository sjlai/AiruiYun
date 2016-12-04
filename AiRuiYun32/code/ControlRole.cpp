#include "stdafx.h"
#include "ControlRole.h"
#include "Manage.h"

extern CManage *pManage;

CControlRole::CControlRole()
{
	CDataUser *pUser;
	for (int i = 0; i < 0; i++)
	{
		pUser = new CDataUser;
		pUser->id = i;
		pUser->name.Format(L"客户%03d", i);
		pUser->age = i + 1;
		m_mpUser.insert(make_pair(i, pUser));
	}
}


CControlRole::~CControlRole()
{
	TplRelease(m_mpUser);
	TplRelease(m_mpMaster);
	TplRelease(m_mpStore);
	TplRelease(m_mpWorker);
}
//////////////////////////////////////////////////////////////////////////
//private for data file
void CControlRole::DataFile_User(bool bSave)
{
	CString path;
	path = pManage->GetPathName() + L"UserData\\";
	TplMulFileData(path, L"User", m_mpUser, bSave);
	//read
	if (!bSave)
	{
		map<u64, CDataUser *>::iterator it, its, ite;
		CDataUser *pRole;
		u64 idt;
		//
		mpUserPhoneID.clear();
		mpUserAccountID.clear();
		mpUserEmailID.clear();
		mpUserThirdAccID.clear();
		//
		its = m_mpUser.begin();
		ite = m_mpUser.end();
		for (it = its; it != ite; it++)
		{
			pRole = it->second;
			idt = pRole->id;
			if (pRole->cellPhone > 0) mpUserPhoneID[pRole->cellPhone] = idt;
			if (!pRole->account.IsEmpty()) mpUserAccountID[pRole->account] = idt;
			if (!pRole->email.IsEmpty()) mpUserEmailID[pRole->email] = idt;
			if (!pRole->thirdAccount.IsEmpty()) mpUserThirdAccID[pRole->thirdAccount] = idt;
		}
	}
}
void CControlRole::DataFile_Master(bool bSave)
{
	CString path;
	path = pManage->GetPathName() + L"MasterData\\";
	TplMulFileData(path, L"Master", m_mpMaster, bSave);
	//read
	if (!bSave)
	{
		map<u64, CDataMaster* >::iterator it, its, ite;
		CDataMaster *pRole;
		u64 idt;
		//
		mpMasterPhoneID.clear();
		mpMasterAccountID.clear();
		mpMasterEmailID.clear();
		mpMasterThirdAccID.clear();
		//
		its = m_mpMaster.begin();
		ite = m_mpMaster.end();
		for (it = its; it != ite; it++)
		{
			pRole = it->second;
			idt = pRole->id;
			if (pRole->cellPhone > 0) mpMasterPhoneID[pRole->cellPhone] = idt;
			if (!pRole->account.IsEmpty()) mpMasterAccountID[pRole->account] = idt;
			if (!pRole->email.IsEmpty()) mpMasterEmailID[pRole->email] = idt;
			if (!pRole->thirdAccount.IsEmpty()) mpMasterThirdAccID[pRole->thirdAccount] = idt;
		}
	}
}
void CControlRole::DataFile_Store(bool bSave,u64 idt,bool bSaveTmp)
{
	CString path;
	path = pManage->GetPathName() + L"StoreData\\";
	//单文件保存
	TplMulFileData(path, L"Store", m_mpStore, bSave, idt, bSaveTmp);
	//read
	if (!bSave)
	{
		map<u64, CDataStore* >::iterator it, its, ite;
		CDataStore *pRole;
		//单文件读取
		if (idt>0)
		{
			pRole =(CDataStore *)GetData(idt,DAT_STORE);
			if (!pRole) return ;
			if (pRole->cellPhone > 0) mpStorePhoneID[pRole->cellPhone] = idt;
			if (!pRole->account.IsEmpty()) mpStoreAccountID[pRole->account] = idt;
			if (!pRole->email.IsEmpty()) mpStoreEmailID[pRole->email] = idt;
			if (!pRole->thirdAccount.IsEmpty()) mpStoreThirdAccID[pRole->thirdAccount] = idt;
		} 
		else
		{
			mpStorePhoneID.clear();
			mpStoreAccountID.clear();
			mpStoreEmailID.clear();
			mpStoreThirdAccID.clear();
			//
			its = m_mpStore.begin();
			ite = m_mpStore.end();
			for (it = its; it != ite; it++)
			{
				pRole = it->second;
				idt = pRole->id;
				if (pRole->cellPhone > 0) mpStorePhoneID[pRole->cellPhone] = idt;
				if (!pRole->account.IsEmpty()) mpStoreAccountID[pRole->account] = idt;
				if (!pRole->email.IsEmpty()) mpStoreEmailID[pRole->email] = idt;
				if (!pRole->thirdAccount.IsEmpty()) mpStoreThirdAccID[pRole->thirdAccount] = idt;
			}	
		}

	}
}
void CControlRole::DataFile_Worker(bool bSave)
{
	CString path;
	path = pManage->GetPathName() + L"WorkerData\\";
	TplMulFileData(path, L"Worker", m_mpWorker, bSave);
	//read
	if (!bSave)
	{
		map<u64, CDataWorker* >::iterator it, its, ite;
		CDataWorker *pRole;
		u64 idt;
		//
		mpWorkerPhoneID.clear();
		mpWorkerAccountID.clear();
		mpWorkerEmailID.clear();
		mpWorkerThirdAccID.clear();
		//
		its = m_mpWorker.begin();
		ite = m_mpWorker.end();
		for (it = its; it != ite; it++)
		{
			pRole = it->second;
			idt = pRole->id;
			if (pRole->cellPhone > 0) mpWorkerPhoneID[pRole->cellPhone] = idt;
			if (!pRole->account.IsEmpty()) mpWorkerAccountID[pRole->account] = idt;
			if (!pRole->email.IsEmpty()) mpWorkerEmailID[pRole->email] = idt;
			if (!pRole->thirdAccount.IsEmpty()) mpWorkerThirdAccID[pRole->thirdAccount] = idt;
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//data file
void CControlRole::DataFile(bool bSave)
{
	//一个角色一个数据文件
	//for user
	DataFile_User(bSave);
	////for doctor
	DataFile_Master(bSave);
	////for clinic
	DataFile_Store(bSave);
	////for worker
	DataFile_Worker(bSave);

}


//////////////////////////////////////////////////////////////////////////
//read the data
CDataBase *CControlRole::GetData(u64 id, EnumDataType type)
{
	if (type==DAT_USER)
	{
		CDataUser *pRole = TplGetData(m_mpUser, id);
		return pRole;
	}else if (type==DAT_MASTER)
	{
		CDataMaster *pRole = TplGetData(m_mpMaster, id);
		return pRole;
	}
	else if (type == DAT_STORE)
	{
		CDataStore *pRole = TplGetData(m_mpStore, id);
		return pRole;
	}
	else if (type == DAT_WORKEAR)
	{
		CDataWorker *pRole = TplGetData(m_mpWorker, id);
		return pRole;
	}
	return nullptr;
}
void CControlRole::	GetData(itStore &its, itStore &ite,int &num)
{
	its = m_mpStore.begin();
	ite = m_mpStore.end();
	num = (int)m_mpStore.size();
}
//add the data, auto save the file
bool CControlRole::AddData(CDataBase *pRole, EnumDataType type,bool bSaveTmp)
{
	bool bState;
	bState = false;
	if (type == DAT_USER)
	{
		CDataUser *pRoleObj = (CDataUser*)pRole;
		bState=TplAddData(m_mpUser, pRoleObj,1001);
	}
	else if (type == DAT_MASTER)
	{
		CDataMaster *pRoleObj = (CDataMaster*)pRole;
		bState=TplAddData(m_mpMaster, pRoleObj,101);
	}
	else if (type == DAT_STORE)
	{
		CDataStore *pRoleObj = (CDataStore*)pRole;
		bState = TplAddData(m_mpStore, pRoleObj,1001);
	}
	else if (type == DAT_WORKEAR)
	{
		CDataWorker *pRoleObj = (CDataWorker*)pRole;
		bState = TplAddData(m_mpWorker, pRoleObj);
	}
	return bState;
}

//delete the data
bool CControlRole::DelData(u64 id, EnumDataType type)
{
	bool bState;
	bState = false;
	if (type == DAT_USER)
	{
		bState = TplDelData(m_mpUser, id);
	}
	else if (type == DAT_MASTER)
	{
		bState = TplDelData(m_mpMaster, id);
	}
	else if (type == DAT_STORE)
	{
		bState = TplDelData(m_mpStore, id);
	}
	else if (type == DAT_WORKEAR)
	{
		bState = TplDelData(m_mpWorker, id);
	}
	return bState;

}


//read the data for login
CDataRole *CControlRole::GetDataLogin(CString &roleInfo, EnumDataType type)
{
	CString phoneMsg;
	u64 phone,idt;
	//
	phoneMsg = roleInfo;
	phone = LibIsPhone(phoneMsg);
	if (DAT_STORE==type)
	{
		//phone
		if (phone > 0) 
		{
			idt = TplGetMpData(mpStorePhoneID, phone);
		}
		//mail
		else if(LibIsMail(roleInfo))
		{
			idt = TplGetMpData(mpStoreEmailID, roleInfo);
		}
		//account
		else{
			idt = TplGetMpData(mpStoreAccountID, roleInfo);			
		}
		//account
		CDataStore *pStore = TplGetData(m_mpStore, idt);
		return pStore;
	} 
	else
	{
	}
	return nullptr;
}
