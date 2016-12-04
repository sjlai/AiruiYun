#pragma once
#include "DataBase.h"
#include "DataGoods.h"

//��ɫ����
class CDataRole:public CDataBase
{
public:
	//nick name����΢�ź��ǳ�
	CString nickName;
	//birthday
	int birthday;
	//sex, 0-female, 1- male
	int sex;
	//age
	int age;
	//identity cardidentification
	CString IDCard;
	//cell-phone number���ֻ��ţ�ΪΨһ��¼���ж�
	u64 cellPhone;
	//�����ֻ����绰��ϵ��ʽ�����Ÿ���
	CString otherPhone;
	//΢�Ź��ں�/΢�ź�/
	CString weixinName;
	//΢��OpenID
	CString weixinOpenID;
	//account��ƽ̨�˺�/����������¼���˺�ID
	CString account;
	//email
	CString email;
	//code
	CString code;
	//pay code ֧���˺�
	CString payCode;
	//cell-phone number verify code
	CString verifyCode;
	//third account
	CString thirdAccount;
	//third type
	CString thirdType;
	//total score
	int nTotalScore;
	//<thirdType,name#thirdAccount>,�󶨵ĵ�������¼�˺�ID���󶨵�ϵͳ�˻�
	map<CString, CString> thirdAcc;
	//member,chain member, <time,userID��name��sex��age��phone>
	map<u64, CString > mpMember;
	//<dataID, state> ,business transaction serial number records for money
	map<u64, int> mpBusinessRecords;
	//�Żݻ�������������Ʒ�� <time+state(2), couponNo> , state=1 fee, =2 recipe, =3 retail
	map<u64, u64> mpCouponActivty;
	//<dataID, state> flag���ͳ�/�յ�����, state=1������state=2���ա�
	map<u64, int> mpFlag;
	//<dataID, state> remark������/�õ�����, state=1������state=2���ա�
	map<u64, int> mpRemark;
	//express address <time,data>
	map<u64, CString > mpExpressAddr;
	//<time, state> ��¼/������֤��sign time, module of each app, -1-assign failer;phone verify sign;account sign;phone verify operation
	map<u64, int > mpSignTime;
	//��־��¼, <time+model ID(3), logstr>���ڴ��б���7�յ�¼��¼����7��-���浽����Ϊ��λ����־�ļ���
	map<u64, CString > mpLog;
public:
	CDataRole();
	~CDataRole();
	//data serialize
	void Serialize(CArchive &ar);

public:
	//��֤����֤����֤��1Сʱ��ʧЧ,return number of login failure
	int ExpireVerifyAndLogin();
	//add score
	void AddScore(int nScore);
	//sub score
	void SubScore(int nScore);
};

////�û�
class CDataUser :public CDataRole
{
public:
	//order diagnosis, <orderID,state>
	map<u64, int > mpOrder;
	//done diagnosis, <orderID,state>
	map<u64, int > mpDoneOrder;
	//my doctor,�ղؼ�ʦ <doctorID,time>
	map<u64, u64 > mpMyMaster;
	//my clinic �ղط��񳡵�, <clinicID, time>
	map<u64, u64 > mpMyStore;
	///////����Żݼ�¼
	//��ȡ���Ż݄�����<couponNo,time>
	map<u64, u64> mpCouponGetTicket;
	//���ѳ�ȥ���Ż݄�����<couponNo,time>
	map<u64, u64> mpCouponOutTicket;
	//���ѻ�õ��Ż���Ʒ����<goodsNo,time>
	map<u64, u64> mpSalesGoods;

public:
	CDataUser();
	~CDataUser();
	void Serialize(CArchive &ar);
	//�ɻ��ֵõ��ȼ�, �û�����Ĭ��10Ԫ��100Ԫ��1000Ԫ��10000Ԫ4���ȼ�
	int GetGrade();
	//�õ��û��Ļ�����Ϣ��id_name,sex,phone
	CString GetBaseInfo();
};

////��ʦ
class CDataMaster :public CDataRole
{
public:
	//���֤ͼƬ
	CString IDCardIcon;
	//ִҵҽʦ�ʸ�֤ͼƬ
	CString businessCertiPic;
	//ҽ��֤��ͼƬ
	CString doctorCertiPic;
	//ִҵҽʦ�ʸ�֤��
	CString businessCertiCode;
	//ҽ��֤���
	CString doctorCertiCode;
	//ҽ������, ����ҽʦ,������ҽʦ,����ҽʦ, סԺҽʦ
	CString gradeMaster;
	//ҽ��ְ��, ����, ������, ��ʦ, ����
	CString titleMaster;
	//ѧ��, ��ר,��ר,��ѧ,�о���,��ʿ,��ʿ��
	CString education;
	//�������
	CString honour;
	//���˽���
	CString introduction;
	//���˼��(50������)
	CString briefIntro;
	//�ó�����
	CString skillTreat;
	//������˾
	int idCompany;
	//�ó���Ŀ
	map<int, CString>  mpSubjects;
	//��֢ <id,name>
	map<int, CString>  mpSymptom;
	//���� <id,name>
	map<int, CString>  mpDisease;
	//��ע����, <clinicID, time>
	map<u64, u64 > mpStore;
	//release records, <releaseID, state> ,STA_RELEASE_SUBMIT
	map<u64, int > mpRelease;
	//user order diagnosis, <orderID, time>
	map<u64, u64 > mpUserOrder;
	//done diagnosis <orderID, time>
	map<u64, u64 > mpDoneDiag;
	//my user, <userID, time>
	map<u64, u64 > mpMyUser;
	//�����Ļ���clinic <clinicID,time>
	map<u64, u64 > mpPersonStore;
	//////////////////////////////////////////////////////////////////////////
	////������������////////////////////////////
	//�Ż݄����� <couponNo,time>
	map<u64, u64 > mpCouponData;
	//��������� <activivityID,time>
	map<int, u64 > mpActivityData;
	/////����Żݻ����
	//��ȡ���Ż݄�����<couponNo,time>
	map<u64, u64> mpCouponGetTicket;
	//���ֵ��Ż݄�����<couponNo,time>
	map<u64, u64> mpCouponOutTicket;
	//Ҫ��ɵ���Ʒ��Ʒ����<goodsNo,time>
	map<u64, u64> mpSalesGoods;
	///////���򰬲���Ʒ�ļ�¼<transID,time>
	////�����ֵ��¼<chargeID,time>
	map<u64, u64 > mpChargeMoney;
	////�빫˾���ּ�¼<withDrawID,time>
	map<u64, u64 > mpWithDrawMoney;

public:
	CDataMaster();
	~CDataMaster();
	void Serialize(CArchive &ar);
	//////////////////////////////////////////////////////////////////////////
	//�õ�ҽ��ͷ����Ϣ
	CString GetTitle();
};
////������Ա��������Ա
class CDataWorker :public CDataRole
{
public:
	//������Ա����
	CString codeBar;
	// Ȩ��ģ��
	map<int, CString > mpAuthority;
	//������λ
	map<int, CString > mpPosition;
	//////
	//��������Ʒ����<goodsNo,time>
	map<u64, u64> mpSalesGoods;
public:
	CDataWorker();
	~CDataWorker();
	void Serialize(CArchive &ar);
};
////������ɫ
class CDataStore :public CDataRole
{
public:
	//full name
	CString fullName;
	//����ľ�γ��
	float longtitude; //����
	float latitude;  //γ��
	//��������������, ʡ���С���
	int idProvince;
	int idCity;
	int idCounty;
	//��д��׺��ַ�������أ��ֵ�
	CString briefAddr;
	//������˾,Ĭ��Ϊ0����ʾ�ܹ�˾
	int idCompany;
	//manager
	CString managerName;
	u64 managerPhone;
	CString managerIDCard;
	CString managerIDCardIcon;
	//Ӫҵ֤ͼƬ
	CString businessCertiPic;
	//Ӫҵ֤��
	CString businessCertiCode;
	//��Ӫ��Χ
	CString businessRange;
	//��Ӫʱ�䣬ʱ��
	u64 businessStartTime, businessEndTime;
	//��Ȩϵͳ����ʱ�䷶Χ
	u64 authorityStartTime, authorityEndTime;
	//����绰��
	CString businessTel;
	//ҽ�����֤ͼƬ
	CString permitCertiPic;
	//ҽ�����֤��
	CString permitCertiCode;
	//��������-δ������
	CString gradeStore;
	//�������
	CString honour;
	//��ϸ���ܣ�������html��ַ
	CString introduction;
	//��Ҫ���(50������)
	CString briefIntro;
	//�ó�����(50������)
	CString skillTreat;
	//clinic ����ͼƬ����ַ <id+description, webAdr>
	map<CString, CString > mpAdPic;
	//�ó���Ŀ
	map<int, CString>  mpSubjects;
	//��֢ <id,name>
	map<int, CString>  mpSymptom;
	//���� <id,name>
	map<int, CString>  mpDisease;
	//����˽��ҽ��  <doctorID, time>
	map<u64, u64 > mpPersonMaster;
	//�ⲿ�ӵ�ҽ��, <doctorID, time> 
	map<u64, u64 > mpMaster;
	//release records , <releaseID, time>
	map<u64, u64> mpRelease;
	//user order diagnosis  <orderID, time>
	map<u64, u64 > mpUserOrder;
	//done diagnosis <orderID, time>
	map<u64, u64 > mpDoneDiag;
	//my user,<userID,time>
	map<u64, u64 > mpMyUser;
	//�豸���� <deviceID, time >
	map<u64, u64 > mpDeviceBed;
	//����� <houseID, name#address >
	map<int, CString > mpHouse;
	//������Ա, ����������Ա<workerID, time+state(2)>
	map<int, u64 > mpWorker;
	//���澭Ӫ���ݹ���
	map<u64, CDataProduct *> mpProducts;
	////������������////////////////////////////
	//�������� <couponID,time>
	map<int, u64 > mpCouponData;
	//��������� <activivityID,time>
	map<int, u64 > mpActivityData;
	/////����Żݻ����
	//��ȡ���Ż݄�����<couponNo,time>
	map<u64, u64> mpCouponGetTicket;
	//���ֵ��Ż݄�����<couponNo,time>
	map<u64, u64> mpCouponOutTicket;
	//Ҫ��ɵ���Ʒ��Ʒ����<goodsNo,time>
	map<u64, u64> mpCouponGetGoods;
	////���򰬲���Ʒ�ļ�¼<transID,time>
	////�����ֵ��¼<chargeID,time>
	map<u64, u64 > mpChargeMoney;
	////�빫˾���ּ�¼<withDrawID,time>
	map<u64, u64 > mpWithDrawMoney;


public:
	CDataStore();
	~CDataStore();
	void Serialize(CArchive &ar);
	//�õ���������ƺ͵�ַ
	void GetHouseInfo(int nhID, CString &nm, CString &adr);

private:
	//��ȡ������������JSON���ݴ�
	CString JsonArrAdPic();
	//��JSON������Ϊ�����������
	bool AddJsonToAdPicArr(CString key,CJson *pJson);

	//JSON process
public:
	//���������JSON��
	CString JsonObj();
	//�������
	void JsonObj(CJson &json);
	//����Json���ݶ���
	bool ParseJson(CJson *pJson);
	//��ȡ��Ӫ����
	void GetData(map<u64, CDataProduct *>::iterator &its, map<u64, CDataProduct *>::iterator &ite, int &totalNum);
	//get product
	CDataProduct *GetProduct(u64 idt);
};

