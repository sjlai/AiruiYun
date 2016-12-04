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


const CString SIGN = L"��";
const CString COMMA = L"��";
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

//��־����
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
//ϵͳ���õĸ���״̬��
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
//ϵͳ���õĸ��ֲ�����, ��������Ӧ���ݵ�״̬��Ϣ
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
//��ʼ��ȫ����������
void InitGlobalVariable();

//�Ƿ�ΪJSON��
bool LibIsJson(CString str, bool &bArr);
//�����ַ��гɶԷ��ŵ�λ��
void LibFindFisrtPairPos(CString sign, CString str, int &ns, int &ne);
//��Json������Ϊһ��key��value����
bool LibParseJsonOneData(CString str, map<CString, CString > &mpJson);
//��JSON���鴮�ֽ�Ϊ���鴮
bool LibParseArrayJsonToVc(CString str, vector<CString > &vcJson);
//�����ݱ��ж�����¼����JSON�����������װ, "vector<{JSON}>"-->"[{},{},...{}]"
CString LibJson(vector<CString > &vcStr);
///�ѱ���1�����ݼ�¼��<key,val>��װ��1��JSON���ݶ���{key:val,...}
CString LibJson(map<CString, CString > &mpdata);
//�ж��ַ����Ƿ�������
bool LibCheckIsNum(CString str);
//��ȡӦ�ó���ĵ�ǰ·��,include "\\"
CString LibGetAppDir();
//�����༶Ŀ¼
bool LibCreateDir(const CString& szPath);
//�Ƿ��ֻ���
bool LibIsPhone(u64 phone,CString &errInfo);
u64 LibIsPhone(CString &str);
//��ȡ��ǰʱ��
u64 GetTimeStamp();
//��ȡ��ǰ������20160708
int GetTimeYMD();
//��ȡ��ǰ������20160708125959
u64 GetTimeAll();
//��ȡ2016-09-09 11:22:30
CString GetDateTime();
//��ȡ��ǰʱ����231130
int GetTimeHMS();
//ת��ʱ���, 2016-07-26 12:33:40
CString GetTimeStrByTimeStamp(u64 nt);
//��ʱ����ֽ�Ϊ���ں�ʱ���������ֵ�ʱ���
void LibTimeStampTwoPart(u64 ntime,u64 &ymd,u64 &hms);
//��ʱ����ֽ�Ϊ���ں�ʱ���������ֵ�ʮ����,�����÷ֽ���ʱ����ʱ������ֽ�
void LibTimeStampYMD_HMS(u64 ntime, int &ymd, int &hms);
int LibYmdByTimeStamp(u64 ntime);
//�õ���ǰ���ڼ�
int LibGetWeek(u64 ntime);
//����0-nt�������
int LibRandom(int nt);
//��ȡ������Դ����
EnumDataType GetAppTypeSource(CString &str);
//�õ����ʶ�����1--user��2--doctor��3--clinic��4--system
EnumDataType AppType(EnumDataType src);
//�õ����ʶ�����1--user��2--doctor��3--clinic��4--system
EnumDataType AppType(CString &str);
//�õ�Ӧ�ö˵�¼����
CString LibGetRegion(CString ipStr, float lati = -1.f, float longti = -1.f);
//CString char*ת��,�����ͷ�char *
char * CStringToChar(CString &str,int &len);
//create the token, ʱ���+��Ϣ��
CString LibToken(CString strInfo);
//�ж��ַ������ԣ��ʼ����绰���˺�
EnumState LibStrPropery(CString &str,u64 &phone);
//��������ŷֽ��ַ���Ϊ����Ԫ
void LibParseStrUnit(CString strInfo, CString sepator, vector<CString > &vcRes, bool bDelSep = true);
//�����ж�
bool LibIsMail(CString &mail);
//��ȡĿ¼�������ļ�
void LibAllFileInDir(CString path, CString inName, vector<CString > &vcName);
//�ж��ļ��Ƿ����
bool LibIsFileExited(CString &fileName);
////ͳһ����ϵͳ������ID������λ���������ͣ�ǰ���Ǹ����͵�����ID
int LibIDCreate(int id,EnumDataType type);
//����ź�ID
u64 LibReleaseID(int doctorID, u64 startTime);
//�ź�ID��ȡҽ��ID
int LibMasterIDbyReleaseID(u64 releaseID);
//����Լ��ID
u64 LibOrderID(u64 releaseID, int orderID);
//��������ID
u64 LibPayID(int idStore,int idClinet);
//Լ��ID��ȡ�ź�ID
u64 LibReleaseIDbyOrderID(u64 orderID);
//������ID��ȡ���ݲ�����ʱ���
u64 LibGetTimeByID(int datype,u64 id);
//������ID��ȡ���ݲ�����������
int LibGetYmdByID(int datype,u64 id);
//Լ��ID��ȡҽ��
int LibMasterIDbyOrderID(u64 orderID);
//Լ��ID��ȡԼ�ﵥ��
int LibNobyOrderID(u64 orderID);
//�õ��Ա��Ӧ���ַ���
CString LibSex(int sex);
//�ַ���������ת��
void LibStrNum(CString str,int &num);
void LibStrNum(CString str, u64 &num);
void LibStrNum(CString str, float &num);
//save the vector data to the file
void LibFileAppendInfo(CString fileName, vector<CString > &vcData);
//�ʱ���Ƿ����
bool LibIsExpire(u64 startTime, u64 endTime, u64 curTime);
//��ñ�����IP��ַ
CString GetIP();
void GetIP(char IPChar[]);
//get the connect socket IP
CString GetConnectIP(SOCKET &serverSocket);
//
char *UNICODE_to_UTF8(const CString &sendStr, int &len);

CString UTF8ToUnicode(char* UTF8);

bool IsTextUTF8(char* str, ULONGLONG length);

EnumOperate LibGetOperateState(CString &str);
//��ȡ������ַ
CString LibGetRegion(int idProvince,int idCity,int idCounty);
