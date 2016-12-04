#pragma once
#include "DataBase.h"
#include "DataGoods.h"

//角色父类
class CDataRole:public CDataBase
{
public:
	//nick name，含微信号昵称
	CString nickName;
	//birthday
	int birthday;
	//sex, 0-female, 1- male
	int sex;
	//age
	int age;
	//identity cardidentification
	CString IDCard;
	//cell-phone number，手机号，为唯一登录名判断
	u64 cellPhone;
	//其他手机、电话联系方式，逗号隔离
	CString otherPhone;
	//微信公众号/微信号/
	CString weixinName;
	//微信OpenID
	CString weixinOpenID;
	//account，平台账号/含第三方登录的账号ID
	CString account;
	//email
	CString email;
	//code
	CString code;
	//pay code 支付账号
	CString payCode;
	//cell-phone number verify code
	CString verifyCode;
	//third account
	CString thirdAccount;
	//third type
	CString thirdType;
	//total score
	int nTotalScore;
	//<thirdType,name#thirdAccount>,绑定的第三方登录账号ID，绑定到系统账户
	map<CString, CString> thirdAcc;
	//member,chain member, <time,userID〇name〇sex〇age〇phone>
	map<u64, CString > mpMember;
	//<dataID, state> ,business transaction serial number records for money
	map<u64, int> mpBusinessRecords;
	//优惠活动（送诊金劵、礼品） <time+state(2), couponNo> , state=1 fee, =2 recipe, =3 retail
	map<u64, u64> mpCouponActivty;
	//<dataID, state> flag，送出/收到锦旗, state=1，发；state=2，收。
	map<u64, int> mpFlag;
	//<dataID, state> remark，发出/得到评论, state=1，发；state=2，收。
	map<u64, int> mpRemark;
	//express address <time,data>
	map<u64, CString > mpExpressAddr;
	//<time, state> 登录/工作验证码sign time, module of each app, -1-assign failer;phone verify sign;account sign;phone verify operation
	map<u64, int > mpSignTime;
	//日志记录, <time+model ID(3), logstr>，内存中保存7日登录记录，超7日-保存到以月为单位的日志文件中
	map<u64, CString > mpLog;
public:
	CDataRole();
	~CDataRole();
	//data serialize
	void Serialize(CArchive &ar);

public:
	//验证码验证，验证码1小时后失效,return number of login failure
	int ExpireVerifyAndLogin();
	//add score
	void AddScore(int nScore);
	//sub score
	void SubScore(int nScore);
};

////用户
class CDataUser :public CDataRole
{
public:
	//order diagnosis, <orderID,state>
	map<u64, int > mpOrder;
	//done diagnosis, <orderID,state>
	map<u64, int > mpDoneOrder;
	//my doctor,收藏技师 <doctorID,time>
	map<u64, u64 > mpMyMaster;
	//my clinic 收藏服务场地, <clinicID, time>
	map<u64, u64 > mpMyStore;
	///////获得优惠记录
	//获取的优惠劵数据<couponNo,time>
	map<u64, u64> mpCouponGetTicket;
	//消费出去的优惠劵数据<couponNo,time>
	map<u64, u64> mpCouponOutTicket;
	//消费获得的优惠商品数据<goodsNo,time>
	map<u64, u64> mpSalesGoods;

public:
	CDataUser();
	~CDataUser();
	void Serialize(CArchive &ar);
	//由积分得到等级, 用户积分默认10元，100元，1000元，10000元4个等级
	int GetGrade();
	//得到用户的基本信息，id_name,sex,phone
	CString GetBaseInfo();
};

////技师
class CDataMaster :public CDataRole
{
public:
	//身份证图片
	CString IDCardIcon;
	//执业医师资格证图片
	CString businessCertiPic;
	//医生证书图片
	CString doctorCertiPic;
	//执业医师资格证号
	CString businessCertiCode;
	//医生证书号
	CString doctorCertiCode;
	//医生级别, 主任医师,副主任医师,主治医师, 住院医师
	CString gradeMaster;
	//医生职称, 教授, 副教授, 讲师, 助教
	CString titleMaster;
	//学历, 中专,大专,大学,研究生,博士,博士后
	CString education;
	//社会荣誉
	CString honour;
	//个人介绍
	CString introduction;
	//个人简介(50字以内)
	CString briefIntro;
	//擅长治疗
	CString skillTreat;
	//所属公司
	int idCompany;
	//擅长科目
	map<int, CString>  mpSubjects;
	//病症 <id,name>
	map<int, CString>  mpSymptom;
	//疾病 <id,name>
	map<int, CString>  mpDisease;
	//关注诊所, <clinicID, time>
	map<u64, u64 > mpStore;
	//release records, <releaseID, state> ,STA_RELEASE_SUBMIT
	map<u64, int > mpRelease;
	//user order diagnosis, <orderID, time>
	map<u64, u64 > mpUserOrder;
	//done diagnosis <orderID, time>
	map<u64, u64 > mpDoneDiag;
	//my user, <userID, time>
	map<u64, u64 > mpMyUser;
	//所属的机构clinic <clinicID,time>
	map<u64, u64 > mpPersonStore;
	//////////////////////////////////////////////////////////////////////////
	////发布促销数据////////////////////////////
	//优惠劵数据 <couponNo,time>
	map<u64, u64 > mpCouponData;
	//促销活动数据 <activivityID,time>
	map<int, u64 > mpActivityData;
	/////获得优惠活动数据
	//获取的优惠劵数据<couponNo,time>
	map<u64, u64> mpCouponGetTicket;
	//兑现的优惠劵数据<couponNo,time>
	map<u64, u64> mpCouponOutTicket;
	//要提成的商品商品数据<goodsNo,time>
	map<u64, u64> mpSalesGoods;
	///////购买艾草商品的记录<transID,time>
	////店面充值记录<chargeID,time>
	map<u64, u64 > mpChargeMoney;
	////与公司提现记录<withDrawID,time>
	map<u64, u64 > mpWithDrawMoney;

public:
	CDataMaster();
	~CDataMaster();
	void Serialize(CArchive &ar);
	//////////////////////////////////////////////////////////////////////////
	//得到医生头衔信息
	CString GetTitle();
};
////工作人员、管理人员
class CDataWorker :public CDataRole
{
public:
	//工作人员编码
	CString codeBar;
	// 权限模块
	map<int, CString > mpAuthority;
	//工作岗位
	map<int, CString > mpPosition;
	//////
	//促销的商品数据<goodsNo,time>
	map<u64, u64> mpSalesGoods;
public:
	CDataWorker();
	~CDataWorker();
	void Serialize(CArchive &ar);
};
////诊所角色
class CDataStore :public CDataRole
{
public:
	//full name
	CString fullName;
	//具体的经纬度
	float longtitude; //经度
	float latitude;  //纬度
	//所属的三级地区, 省、市、县
	int idProvince;
	int idCity;
	int idCounty;
	//简写后缀地址，区（县）街道
	CString briefAddr;
	//所属公司,默认为0，表示总公司
	int idCompany;
	//manager
	CString managerName;
	u64 managerPhone;
	CString managerIDCard;
	CString managerIDCardIcon;
	//营业证图片
	CString businessCertiPic;
	//营业证号
	CString businessCertiCode;
	//经营范围
	CString businessRange;
	//经营时间，时分
	u64 businessStartTime, businessEndTime;
	//授权系统工作时间范围
	u64 authorityStartTime, authorityEndTime;
	//服务电话，
	CString businessTel;
	//医疗许可证图片
	CString permitCertiPic;
	//医疗许可证号
	CString permitCertiCode;
	//诊所级别-未来扩充
	CString gradeStore;
	//社会荣誉
	CString honour;
	//详细介绍，可以是html地址
	CString introduction;
	//简要简介(50字以内)
	CString briefIntro;
	//擅长治疗(50字以内)
	CString skillTreat;
	//clinic 宣传图片，网址 <id+description, webAdr>
	map<CString, CString > mpAdPic;
	//擅长科目
	map<int, CString>  mpSubjects;
	//病症 <id,name>
	map<int, CString>  mpSymptom;
	//疾病 <id,name>
	map<int, CString>  mpDisease;
	//诊所私有医生  <doctorID, time>
	map<u64, u64 > mpPersonMaster;
	//外部接单医生, <doctorID, time> 
	map<u64, u64 > mpMaster;
	//release records , <releaseID, time>
	map<u64, u64> mpRelease;
	//user order diagnosis  <orderID, time>
	map<u64, u64 > mpUserOrder;
	//done diagnosis <orderID, time>
	map<u64, u64 > mpDoneDiag;
	//my user,<userID,time>
	map<u64, u64 > mpMyUser;
	//设备床号 <deviceID, time >
	map<u64, u64 > mpDeviceBed;
	//房间号 <houseID, name#address >
	map<int, CString > mpHouse;
	//工作人员, 包括促销人员<workerID, time+state(2)>
	map<int, u64 > mpWorker;
	//店面经营数据管理
	map<u64, CDataProduct *> mpProducts;
	////发布促销数据////////////////////////////
	//促销数据 <couponID,time>
	map<int, u64 > mpCouponData;
	//促销活动数据 <activivityID,time>
	map<int, u64 > mpActivityData;
	/////获得优惠活动数据
	//获取的优惠劵数据<couponNo,time>
	map<u64, u64> mpCouponGetTicket;
	//兑现的优惠劵数据<couponNo,time>
	map<u64, u64> mpCouponOutTicket;
	//要提成的商品商品数据<goodsNo,time>
	map<u64, u64> mpCouponGetGoods;
	////购买艾草商品的记录<transID,time>
	////店面充值记录<chargeID,time>
	map<u64, u64 > mpChargeMoney;
	////与公司提现记录<withDrawID,time>
	map<u64, u64 > mpWithDrawMoney;


public:
	CDataStore();
	~CDataStore();
	void Serialize(CArchive &ar);
	//得到房间的名称和地址
	void GetHouseInfo(int nhID, CString &nm, CString &adr);

private:
	//读取广告宣传数组的JSON数据串
	CString JsonArrAdPic();
	//把JSON串解析为广告宣传数组
	bool AddJsonToAdPicArr(CString key,CJson *pJson);

	//JSON process
public:
	//单个对象的JSON串
	CString JsonObj();
	//数组对象
	void JsonObj(CJson &json);
	//解析Json数据对象
	bool ParseJson(CJson *pJson);
	//读取经营数据
	void GetData(map<u64, CDataProduct *>::iterator &its, map<u64, CDataProduct *>::iterator &ite, int &totalNum);
	//get product
	CDataProduct *GetProduct(u64 idt);
};

