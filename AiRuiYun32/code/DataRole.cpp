#include "stdafx.h"
#include "DataRole.h"


CDataRole::CDataRole()
{
	id = sex = age = 0;
	cellPhone = 0;
	nTotalScore = 0;
}

CDataRole::~CDataRole()
{

}

//data serialize
void CDataRole::Serialize(CArchive &ar)
{
	CDataBase::Serialize(ar);
	//
	if (ar.IsStoring()) //save
	{
		ar << nickName;
		ar << birthday;
		ar << sex;
		ar << age;
		ar << IDCard;
		ar << cellPhone;
		ar << otherPhone;
		ar << weixinName;
		ar << weixinOpenID;
		ar << account;
		ar << email;
		ar << code;
		ar << payCode;
		ar << verifyCode;
		ar << thirdAccount;
		ar << thirdType;
		ar << nTotalScore;
	}
	else //read
	{
		ar >> nickName;
		ar >> birthday;
		ar >> sex;
		ar >> age;
		ar >> IDCard;
		ar >> cellPhone;
		ar >> otherPhone;
		ar >> weixinName;
		ar >> weixinOpenID;
		ar >> account;
		ar >> email;
		ar >> code;
		ar >> payCode;
		ar >> verifyCode;
		ar >> thirdAccount;
		ar >> thirdType;
		ar >> nTotalScore;
	}
	TplSerializeContainer(ar, thirdAcc);
	TplSerializeContainer(ar, mpMember);
	TplSerializeContainer(ar, mpBusinessRecords);
	TplSerializeContainer(ar, mpCouponActivty);
	TplSerializeContainer(ar, mpFlag);
	TplSerializeContainer(ar, mpRemark);
	TplSerializeContainer(ar, mpExpressAddr);
	TplSerializeContainer(ar, mpSignTime);
	TplSerializeContainer(ar, mpLog);
}
//验证码验证，验证码1小时后失效,return true is expire
int CDataRole::ExpireVerifyAndLogin()
{
	//sign time, module of each app, 0-phone verify sign
	map<u64, int >::reverse_iterator it, its, ite;
	u64 ntime;
	int num;
	//
	num = 0;
	ntime = GetTimeStamp();
	its = mpSignTime.rbegin();
	ite = mpSignTime.rend();
	for (it = its; it != ite; it++)
	{
		if (it->second == OPE_SIGN_PHONEVERIFY)
		{
			if (ntime - it->first > HOUR)
			{
				verifyCode = L"";
				break;
			}
		}
		if (it->second == OPE_ERR)
		{
			if (ntime - it->first < 24 * HOUR)
			{
				++num;
			}
		}
	}
	//
	return num;
}
//add score
void CDataRole::AddScore(int nScore)
{
	nTotalScore += nScore;
}
//sub score
void CDataRole::SubScore(int nScore)
{
	nTotalScore -= nScore;
	if (nTotalScore < 0) nTotalScore = 0;
}

///////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//user
CDataUser::CDataUser() :CDataRole()
{

}
CDataUser::~CDataUser()
{

}
//
void CDataUser::Serialize(CArchive &ar)
{
	CDataRole::Serialize(ar);
	//
	TplSerializeContainer(ar, mpOrder);
	TplSerializeContainer(ar, mpDoneOrder);
	TplSerializeContainer(ar, mpMyMaster);
	TplSerializeContainer(ar, mpMyStore);
	TplSerializeContainer(ar, mpCouponGetTicket);
	TplSerializeContainer(ar, mpCouponOutTicket);
	TplSerializeContainer(ar, mpSalesGoods);
}
//由积分得到等级
int CDataUser::GetGrade()
{
	if (nTotalScore < DEC3)
	{
		return 1;
	}
	else if (nTotalScore < DEC4)
	{
		return 2;
	}
	else if (nTotalScore < DEC5)
	{
		return 3;
	}
	else if (nTotalScore < DEC6)
	{
		return 4;
	}
	return 5;
}
//得到用户的基本信息，id_name,sex,phone
CString CDataUser::GetBaseInfo()
{
	CString str;
	str.Format(L"%d_%s,%s,%I64d", id, name, LibSex(sex), cellPhone);
	return str;
}

//////////////////////////////////////////////////////////////////////////
//doctor
CDataMaster::CDataMaster() :CDataRole()
{
	idCompany = 0; //总公司
}
CDataMaster::~CDataMaster()
{

}
//
void CDataMaster::Serialize(CArchive &ar)
{
	CDataRole::Serialize(ar);
	//
	if (ar.IsStoring()) //save
	{
		ar << IDCardIcon;
		ar << businessCertiPic;
		ar << doctorCertiPic;
		ar << businessCertiCode;
		ar << doctorCertiCode;
		ar << gradeMaster;
		ar << titleMaster;
		ar << education;
		ar << honour;
		ar << introduction;
		ar << briefIntro;
		ar << skillTreat;
		ar << idCompany;
	}
	else
	{
		ar >> IDCardIcon;
		ar >> businessCertiPic;
		ar >> doctorCertiPic;
		ar >> businessCertiCode;
		ar >> doctorCertiCode;
		ar >> gradeMaster;
		ar >> titleMaster;
		ar >> education;
		ar >> honour;
		ar >> introduction;
		ar >> briefIntro;
		ar >> skillTreat;
		ar >> idCompany;
	}
	TplSerializeContainer(ar, mpSubjects);
	TplSerializeContainer(ar, mpSymptom);
	TplSerializeContainer(ar, mpDisease);
	TplSerializeContainer(ar, mpStore);
	TplSerializeContainer(ar, mpRelease);
	TplSerializeContainer(ar, mpUserOrder);
	TplSerializeContainer(ar, mpDoneDiag);
	TplSerializeContainer(ar, mpMyUser);
	TplSerializeContainer(ar, mpPersonStore);
	//
	TplSerializeContainer(ar, mpCouponData);
	TplSerializeContainer(ar, mpActivityData);
	TplSerializeContainer(ar, mpCouponGetTicket);
	TplSerializeContainer(ar, mpCouponOutTicket);
	TplSerializeContainer(ar, mpSalesGoods);
	//
	TplSerializeContainer(ar, mpChargeMoney);
	TplSerializeContainer(ar, mpWithDrawMoney);

}
//得到医生头衔信息
CString CDataMaster::GetTitle()
{
	CString str;
	int nt = 0;
	if (!gradeMaster.IsEmpty()) {
		str = str + gradeMaster; ++nt;
	}
	if (!titleMaster.IsEmpty()) {
		str = str + titleMaster; ++nt;
	}
	if (nt < 2 && !education.IsEmpty()) str = str + education;
	if (str.IsEmpty()) str = L"医师";
	return str;
}

//////////////////////////////////////////////////////////////////////////
//worker
CDataWorker::CDataWorker() :CDataRole()
{
}
CDataWorker::~CDataWorker()
{

}
//
void CDataWorker::Serialize(CArchive &ar)
{
	CDataRole::Serialize(ar);
	//
	if (ar.IsStoring())
	{
		ar << codeBar;
	} 
	else
	{
		ar >> codeBar;
	}
	TplSerializeContainer(ar, mpAuthority);
	TplSerializeContainer(ar, mpPosition);
	TplSerializeContainer(ar, mpSalesGoods);
}
//////////////////////////////////////////////////////////////////////////
//clinic
CDataStore::CDataStore() :CDataRole()
{
	longtitude = latitude = 0.f;
	idProvince = idCity = idCounty = 0;
	idCompany = 0;
	managerPhone = businessStartTime = businessEndTime = authorityStartTime = authorityEndTime = 0;
	//
	vcKey.push_back(L"dataID");
	vcKey.push_back(L"name");
	vcKey.push_back(L"address");
	vcKey.push_back(L"briefIntro");
	vcKey.push_back(L"introduction");
	vcKey.push_back(L"longitude");
	vcKey.push_back(L"latitude");
	vcKey.push_back(L"storeIcon");
	vcKey.push_back(L"storeAdPic");
	vcKey.push_back(L"managerName");
	vcKey.push_back(L"managerPhone");
	vcKey.push_back(L"managerIDCard");
	vcKey.push_back(L"managerIDCardIcon");
	vcKey.push_back(L"businessRange");
	vcKey.push_back(L"businessStartTime");
	vcKey.push_back(L"businessEndTime");
	vcKey.push_back(L"businessTel");
	vcKey.push_back(L"authorityStartTime");
	vcKey.push_back(L"authorityEndTime");
	vcKey.push_back(L"state");
	//
}
CDataStore::~CDataStore()
{
	TplRelease(mpProducts);
}
void CDataStore::Serialize(CArchive &ar)
{
	CDataRole::Serialize(ar);
	//
	if (ar.IsStoring()) //save
	{
		ar << fullName;
		ar << longtitude;
		ar << latitude;
		ar << idProvince;
		ar << idCity;
		ar << idCounty;
		ar << briefAddr;
		ar << idCompany;
		ar << managerName;
		ar << managerPhone;
		ar << managerIDCard;
		ar << managerIDCardIcon;
		ar << businessCertiPic;
		ar << businessCertiCode;
		ar << businessRange;
		ar << businessStartTime;
		ar << businessEndTime;
		ar << businessTel;
		ar << authorityStartTime;
		ar << authorityEndTime;
		ar << permitCertiPic;
		ar << permitCertiCode;
		ar << gradeStore;
		ar << honour;
		ar << introduction;
		ar << briefIntro;
		ar << skillTreat;
	}
	else //read
	{
		ar >> fullName;
		ar >> longtitude;
		ar >> latitude;
		ar >> idProvince;
		ar >> idCity;
		ar >> idCounty;
		ar >> briefAddr;
		ar >> idCompany;
		ar >> managerName;
		ar >> managerPhone;
		ar >> managerIDCard;
		ar >> managerIDCardIcon;
		ar >> businessCertiPic;
		ar >> businessCertiCode;
		ar >> businessRange;
		ar >> businessStartTime;
		ar >> businessEndTime;
		ar >> businessTel;
		ar >> authorityStartTime;
		ar >> authorityEndTime;
		ar >> permitCertiPic;
		ar >> permitCertiCode;
		ar >> gradeStore;
		ar >> honour;
		ar >> introduction;
		ar >> briefIntro;
		ar >> skillTreat;
	}
	//
	TplSerializeContainer(ar, mpAdPic);
	TplSerializeContainer(ar, mpSubjects);
	TplSerializeContainer(ar, mpSymptom);
	TplSerializeContainer(ar, mpDisease);
	TplSerializeContainer(ar, mpPersonMaster);
	TplSerializeContainer(ar, mpMaster);
	TplSerializeContainer(ar, mpRelease);
	TplSerializeContainer(ar, mpUserOrder);
	TplSerializeContainer(ar, mpDoneDiag);
	TplSerializeContainer(ar, mpMyUser);
	TplSerializeContainer(ar, mpHouse);
	TplSerializeContainer(ar, mpWorker);
	TplSerializeContainer(ar, mpCouponData);
	TplSerializeContainer(ar, mpActivityData);
	TplSerializeContainer(ar, mpCouponGetTicket);
	TplSerializeContainer(ar, mpCouponOutTicket);
	TplSerializeContainer(ar, mpCouponGetGoods);
	//
	TplSerializeContainer(ar, mpChargeMoney);
	TplSerializeContainer(ar, mpWithDrawMoney);
	//
	TplSerializeMapData(ar, mpProducts);
}
//得到房间的名称和地址
void CDataStore::GetHouseInfo(int nhID, CString &nm, CString &adr)
{
	nm = adr = L"";
	map<int, CString >::iterator it, ite;
	//
	it = mpHouse.find(nhID);
	ite = mpHouse.end();
	if (it == ite) return;
	//
	vector<CString> vcUn;
	LibParseStrUnit(it->second, L"#", vcUn);
	if (vcUn.size() > 0) nm = vcUn[0];
	if (vcUn.size() > 1) adr = vcUn[1];
}

//单个对象的JSON串
CString CDataStore::JsonObj()
{
	CJson json;
	//
	JsonObj(json);
	//
	return json.Json();
}
//数组对象
void CDataStore::JsonObj(CJson &json)
{
	CJson jsonSub;
	int i;
	//
	json.Clear();
	i = 0;
	json.Set(vcKey[i++], id);
	json.Set(vcKey[i++], name);
	json.Set(vcKey[i++], briefAddr);
	json.Set(vcKey[i++], briefIntro);
	json.Set(vcKey[i++], introduction);
	json.Set(vcKey[i++], longtitude);
	json.Set(vcKey[i++], latitude);
	json.Set(vcKey[i++], icon);
	json.Set(vcKey[i++], JsonArrAdPic());
	json.Set(vcKey[i++], managerName);
	json.Set(vcKey[i++], managerPhone);
	json.Set(vcKey[i++], managerIDCard);
	json.Set(vcKey[i++], managerIDCardIcon);
	json.Set(vcKey[i++], businessRange);
	json.Set(vcKey[i++], businessStartTime);
	json.Set(vcKey[i++], businessEndTime);
	json.Set(vcKey[i++], businessTel);
	json.Set(vcKey[i++], authorityStartTime);
	json.Set(vcKey[i++], authorityEndTime);
	json.Set(vcKey[i++], GetState());
	//
}
//解析Json数据对象
bool CDataStore::ParseJson(CJson *pJson)
{
	bool bOK;
	int i;
	CString storeAdPic;
	//
	bOK = true;
	i = 0;
	bOK &= pJson->Get(vcKey[i++], id);
	bOK &= pJson->Get(vcKey[i++], name);
	bOK &= pJson->Get(vcKey[i++], briefAddr);
	bOK &= pJson->Get(vcKey[i++], briefIntro);
	bOK &= pJson->Get(vcKey[i++], introduction);
	bOK &= pJson->Get(vcKey[i++], longtitude);
	bOK &= pJson->Get(vcKey[i++], latitude);
	bOK &= pJson->Get(vcKey[i++], icon);
	bOK &= AddJsonToAdPicArr(vcKey[i++],pJson);
	bOK &= pJson->Get(vcKey[i++], managerName);
	bOK &= pJson->Get(vcKey[i++], managerPhone);
	bOK &= pJson->Get(vcKey[i++], managerIDCard);
	bOK &= pJson->Get(vcKey[i++], managerIDCardIcon);
	bOK &= pJson->Get(vcKey[i++], businessRange);
	bOK &= pJson->Get(vcKey[i++], businessStartTime);
	bOK &= pJson->Get(vcKey[i++], businessEndTime);
	bOK &= pJson->Get(vcKey[i++], businessTel);
	bOK &= pJson->Get(vcKey[i++], authorityStartTime);
	bOK &= pJson->Get(vcKey[i++], authorityEndTime);
	return bOK;
}

//读取经营数据
void CDataStore::GetData(map<u64, CDataProduct *>::iterator &its, map<u64, CDataProduct *>::iterator &ite, int &totalNum)
{
	its = mpProducts.begin();
	ite = mpProducts.end();
	totalNum =(int) mpProducts.size();
}
//get product
CDataProduct *CDataStore::GetProduct(u64 idt)
{
	return TplGetData(mpProducts, idt);
}


//////////////////////////////////////////////////////////////////////////
//宣传图片，网址 <id+description,webAdr>
CString CDataStore::JsonArrAdPic()
{
	map<CString, CString >::iterator its, it, ite; 
	//
	CJson json,jsonArr;
	int npos;
	u64 idt;
	CString key;
	//
	its = mpAdPic.begin();
	ite = mpAdPic.end();
	for (it = its; it != ite;it++)
	{
		key = it->first;
		//
		npos = key.Find(L" ");
		LibStrNum(key.Left(npos),idt);
		key = key.Right(key.GetLength() - npos);
		//
		json.Clear();
		json.Set(L"dataID", idt);
		json.Set(L"pic", it->second); //http
		json.Set(L"title", key);
		//
		jsonArr.SetArrObj(it->first, json);
	}
	return jsonArr.Json();
}
//把JSON串解析为广告宣传数组
bool CDataStore::AddJsonToAdPicArr(CString keyStr, CJson *pJson)
{
	//
	vector<map<CString, CString > > vcmpData;
	CString key, val, strKey;
	int i, N;
	bool bOK;
	//
	pJson->GetMapDataByArr(keyStr, vcmpData);
	//
	mpAdPic.clear();
	N = (int)vcmpData.size();
	for (i = 0; i < N;i++)
	{
		key = L"dataID";
		bOK =TplGetMpData(vcmpData[i],key,val);
		if (!bOK) return false;
		strKey = val + L" ";
		key = L"title";
		bOK = TplGetMpData(vcmpData[i], key, val);
		if (!bOK) return false;
		val.Trim();
		strKey = strKey + val;
		//
		key = L"pic";
		bOK = TplGetMpData(vcmpData[i], key, val);
		if (!bOK) return false;
		mpAdPic[strKey] = val;
	}
	return true;
}