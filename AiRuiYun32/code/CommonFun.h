#pragma once
#include <vector>
#include <map>
#include <list>
using namespace std;

//1844,6744,0737,0955,1615
#define u64 unsigned __int64
typedef map<CString, CString>::iterator itStr;
typedef map<u64, CString>::iterator itUStr;
typedef map<u64, u64>::iterator itUU;


const CString SIGN = L"〇";
const CString COMMA = L"，";
const int MINUTE = 60;
const int HOUR = 60 * MINUTE;
const int HALFHOUR = 30 * MINUTE;
const int DAY = 24 * HOUR;
const int WEEK = 7 * DAY;
const int YEAR = 365 * DAY;
const int DEC2 = 100;
const int DEC3 = DEC2 * 10;
const int DEC4 = DEC3 * 10;
const int DEC5 = DEC4 * 10;
const int DEC6 = DEC5 * 10;
const int DEC7 = DEC6 * 10;
const u64 DEC8 = DEC7 * 100;
const u64 DEC9 = DEC8 * 10;
const u64 DEC10 = DEC9 * 10;
const u64 DEC11 = DEC10 * 10;
const u64 DEC12 = DEC11 * 10;
const u64 DEC13 = DEC12 * 10;
const u64 DEC18 = DEC13* DEC5;

//日志类型
enum EnumLogData {
	LOG_COMMU,LOG_MANAGE,LOG_REQUIRE
};

enum EnumDataType
{
	DAT_ERR=-1,
	DAT_USER,DAT_MASTER,DAT_STORE,DAT_WORKEAR, DAT_STORE_PRODUCT,

	DAT_ACTIVITY, DAT_RELEASE, DAT_ORDER,

	CL_USER,CL_DOCTOR, CL_CLINIC, CL_SYSTEM, CL_NUL,
	CL_USER_ANDROID_APP,CL_USER_IOS_APP,
	CL_USER_WEIXIN_APP,CL_USER_WEBSITE,
	CL_DOCTOR_ANDROID_APP,CL_DOCTOR_IOS_APP,
	CL_DOCTOR_PC_APP,CL_DOCTOR_CLINIC_APP,
	CL_DOCTOR_WEBSITE,CL_CLINIC_APP,
	CL_SYSTEM_WEBSITE,CL_SYSTEM_PC


};
//系统所用的各种状态字
enum EnumState
{
	STA_ERR=-1, STA_OK=0, STA_PAUSE, STA_DEL, 
	STA_STOP,	
	STA_DEAD, STA_SYS, STA_CLINIC, STA_STR_ACCOUNT, STA_STR_EMAIL, STA_STR_THIRDACC,
	STA_STR,STA_EMAIL,STA_PHONE,
	STA_ORDER_DONE, STA_ORDER_ADDUSER, STA_ORDER_SUBMIT, STA_ORDER_NOPAY,STA_ORDER_PAY, STA_ORDER_OVERDUE,STA_ORDER_MESSAGE, STA_ORDER_FREEZE,
	STA_RELEASE_SUBMIT, STA_RELEASE_STOP, STA_RELEASE_DEL,
	STA_DIAG_START,STA_DIAG_END, STA_DIAG_RECIPE_PRICE, STA_DIAG_RECIPE_OUTPUT,STA_DIAG_EXPRESS,
	STA_PAY_SAVE,STA_PAY_OK,STA_PAY_REFUND,STA_PAY_DEL,
};
//系统所用的各种操作字, 各操作对应数据的状态信息
enum EnumOperate
{
	OPE_ERR = -1, OPE_OK = 0, 
	OPER_GET, OPER_LIST, OPER_ADD, OPER_DEL, OPER_INLIST, OPER_OUTLIST,
	OPE_SIGN_PHONEVERIFY,OPE_SIGN_PHONECODE,OPE_SIGN_ACCCODE,OPE_SIGN_THIRDACC,
	OPE_PHONE_CODEVERIFY,
	
	
	OPE_ORDER_TIMESEG_LIST, OPE_ORDER_ADDUSER, OPE_ORDER_SUBMIT, OPE_ORDER_SUBMITPAY, OPE_ORDER_DEL,
	OPE_RELEASE_SUBMIT,OPE_RELEASE_DEL,
	OPE_DATA_ADD, OPE_DATA_SAVE, OPE_DATA_MODIFY, OPE_DATA_DEL, OPE_DATA_REFUND
};
//初始化全局容器变量
void InitGlobalVariable();

//是否为JSON串
bool LibIsJson(CString str, bool &bArr);
//查找字符中成对符号的位置
void LibFindFisrtPairPos(CString sign, CString str, int &ns, int &ne);
//把Json串解析为一层key，value对象
bool LibParseJsonOneData(CString str, map<CString, CString > &mpJson);
//把JSON数组串分解为数组串
bool LibParseArrayJsonToVc(CString str, vector<CString > &vcJson);
//把数据表中多条记录对象JSON串进行数组封装, "vector<{JSON}>"-->"[{},{},...{}]"
CString LibJson(vector<CString > &vcStr);
///把表中1条数据记录的<key,val>封装成1个JSON数据对象{key:val,...}
CString LibJson(map<CString, CString > &mpdata);
//判断字符串是否是数字
bool LibCheckIsNum(CString str);
//读取应用程序的当前路径,include "\\"
CString LibGetAppDir();
//创建多级目录
bool LibCreateDir(const CString& szPath);
//是否手机号
bool LibIsPhone(u64 phone,CString &errInfo);
u64 LibIsPhone(CString &str);
//读取当前时间
u64 GetTimeStamp();
//读取当前年月日20160708
int GetTimeYMD();
//读取当前年月日20160708125959
u64 GetTimeAll();
//读取2016-09-09 11:22:30
CString GetDateTime();
//读取当前时分秒231130
int GetTimeHMS();
//转换时间戳, 2016-07-26 12:33:40
CString GetTimeStrByTimeStamp(u64 nt);
//把时间戳分解为日期和时间两个部分的时间戳
void LibTimeStampTwoPart(u64 ntime,u64 &ymd,u64 &hms);
//把时间戳分解为日期和时间两个部分的十进制,不能用分解后的时分秒时间戳来分解
void LibTimeStampYMD_HMS(u64 ntime, int &ymd, int &hms);
int LibYmdByTimeStamp(u64 ntime);
//得到当前星期几
int LibGetWeek(u64 ntime);
//产生0-nt的随机数
int LibRandom(int nt);
//读取访问来源属性
EnumDataType GetAppTypeSource(CString &str);
//得到访问端类型1--user，2--doctor，3--clinic，4--system
EnumDataType AppType(EnumDataType src);
//得到访问端类型1--user，2--doctor，3--clinic，4--system
EnumDataType AppType(CString &str);
//得到应用端登录区域
CString LibGetRegion(CString ipStr, float lati = -1.f, float longti = -1.f);
//CString char*转换,不用释放char *
char * CStringToChar(CString &str,int &len);
//create the token, 时间戳+信息串
CString LibToken(CString strInfo);
//判断字符串属性，邮件、电话、账号
EnumState LibStrPropery(CString &str,u64 &phone);
//按隔离符号分解字符串为各单元
void LibParseStrUnit(CString strInfo, CString sepator, vector<CString > &vcRes, bool bDelSep = true);
//邮箱判断
bool LibIsMail(CString &mail);
//读取目录下所有文件
void LibAllFileInDir(CString path, CString inName, vector<CString > &vcName);
//判断文件是否存在
bool LibIsFileExited(CString &fileName);
////统一生成系统的数据ID，后两位是数据类型，前面是该类型的数据ID
int LibIDCreate(int id,EnumDataType type);
//分配放号ID
u64 LibReleaseID(int doctorID, u64 startTime);
//放号ID读取医生ID
int LibMasterIDbyReleaseID(u64 releaseID);
//分配约诊ID
u64 LibOrderID(u64 releaseID, int orderID);
//分配零售ID
u64 LibPayID(int idStore,int idClinet);
//约诊ID读取放号ID
u64 LibReleaseIDbyOrderID(u64 orderID);
//由数据ID读取数据产生的时间戳
u64 LibGetTimeByID(int datype,u64 id);
//由数据ID读取数据产生的年月日
int LibGetYmdByID(int datype,u64 id);
//约诊ID读取医生
int LibMasterIDbyOrderID(u64 orderID);
//约诊ID读取约诊单号
int LibNobyOrderID(u64 orderID);
//得到性别对应的字符串
CString LibSex(int sex);
//字符串向数字转换
void LibStrNum(CString str,int &num);
void LibStrNum(CString str, u64 &num);
void LibStrNum(CString str, float &num);
//save the vector data to the file
void LibFileAppendInfo(CString fileName, vector<CString > &vcData);
//活动时间是否过期
bool LibIsExpire(u64 startTime, u64 endTime, u64 curTime);
//获得本机的IP地址
CString GetIP();
void GetIP(char IPChar[]);
//get the connect socket IP
CString GetConnectIP(SOCKET &serverSocket);
//
char *UNICODE_to_UTF8(const CString &sendStr, int &len);

CString UTF8ToUnicode(char* UTF8);

bool IsTextUTF8(char* str, ULONGLONG length);

EnumOperate LibGetOperateState(CString &str);
//读取三级地址
CString LibGetRegion(int idProvince,int idCity,int idCounty);
