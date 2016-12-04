#include "stdafx.h"
#include "CommonFun.h"
#include "TemplateFun.h"
#include "md5.h"
//
map<CString, int> glbMpClientType;
//
//��ʼ��ȫ����������
void InitGlobalVariable()
{
	glbMpClientType[L"CL_USER_ANDROID_APP"] = CL_USER_ANDROID_APP;
	glbMpClientType[L"CL_USER_IOS_APP"] = CL_USER_IOS_APP;
	glbMpClientType[L"CL_USER_WEIXIN_APP"] = CL_USER_WEIXIN_APP;
	glbMpClientType[L"CL_USER_WEBSITE"] = CL_USER_WEBSITE;
	glbMpClientType[L"CL_DOCTOR_ANDROID_APP"] = CL_DOCTOR_ANDROID_APP;
	glbMpClientType[L"CL_DOCTOR_IOS_APP"] = CL_DOCTOR_IOS_APP;
	glbMpClientType[L"CL_DOCTOR_PC_APP"] = CL_DOCTOR_PC_APP;
	glbMpClientType[L"CL_DOCTOR_CLINIC_APP"] = CL_DOCTOR_CLINIC_APP;
	glbMpClientType[L"CL_DOCTOR_WEBSITE"] = CL_DOCTOR_WEBSITE;
	glbMpClientType[L"CL_CLINIC_APP"] = CL_CLINIC_APP;
	glbMpClientType[L"CL_SYSTEM_WEBSITE"] = CL_SYSTEM_WEBSITE;
	glbMpClientType[L"CL_SYSTEM_PC"] = CL_SYSTEM_PC;
}


//�Ƿ�ΪJSON��
bool LibIsJson(CString str, bool &bArr)
{
	int np;
	bArr = false;
	np = str.Find(L"\":");
	if (str[0] == '[' && str[str.GetLength() - 1] == ']') bArr = true;
	if (np > 0) return true;
	return false;
}
//�����ַ��гɶԷ��ŵ�λ��
void LibFindFisrtPairPos(CString sign, CString str, int &ns, int &ne)
{
	WCHAR lsign, rsign, ch;
	int nLeft, nRight;
	//
	nLeft = nRight = 0;
	ns = ne = 0;
	if (sign == L"[") {
		lsign = '[';
		rsign = ']';
	}
	else if (sign == L"{") {
		lsign = '{';
		rsign = '}';
	}
	//
	//	ns=str.Find(lsign);	
	//	ne=str.ReverseFind(rsign);
	//����ĵ�һ��
	int nlen = str.GetLength();
	for (int i = 0; i < nlen; i++)
	{
		ch = str.GetAt(i);
		if (ch == lsign) {
			if (nLeft == 0) {
				ns = i;
			}
			++nLeft;
		}
		if (ch == rsign) {
			ne = i;
			++nRight;
		}
		if (nLeft > 0 && nLeft == nRight) break;
	}
}
//��Json������Ϊһ��key��value����
bool LibParseJsonOneData(CString str, map<CString, CString > &mpJson)
{
	mpJson.clear();
	//judge the Json tString format
	/////////////////////////
	int np;
	int ns, ne;
	CString key, val;
	//erase the bracket of object or array
	LibFindFisrtPairPos(L"{", str, ns, ne);
	if (ns == 0) str.SetAt(ns, ' ');
	if (ne == str.GetLength() - 1) str.SetAt(ne, ' ');
	//
	while (true)
	{
		str.Trim();
		if (str[0] == ',') {
			str.SetAt(0, ' ');
			str.Trim();
		}
		//find the ":
		np = str.Find(L"\":");
		if (np < 0) break;
		//key
		key = str.Left(np + 1);
		str = str.Right(str.GetLength() - np - 2);
		str.Trim();
		//value
		if (str[0] == '[') //array
		{
			LibFindFisrtPairPos(L"[", str, ns, ne);
			if (ne > ns) {
				val = str.Left(ne + 1);
				str = str.Right(str.GetLength() - ne - 1);
			}
			else {
				return false;
			}
		}
		else if (str[0] == '{') //object
		{
			LibFindFisrtPairPos(L"{", str, ns, ne);
			if (ne > ns) {
				val = str.Left(ne + 1);
				str = str.Right(str.GetLength() - ne - 1);
			}
			else {
				return false;
			}
		}
		else {
			np = str.Find(L",\"");
			if (np > 0) {
				val = str.Left(np);
				str = str.Right(str.GetLength() - np - 1);
			}
			else {
				val = str;
				str = L"";
			}
		}
		//remove �� front and back
		key.Trim(); 
		key.Trim( L"\""); 
		val.Trim(); 
		val.Trim(L"\"");
		//add key-val
		mpJson.insert(make_pair(key, val));
	}
	return true;
}
//��JSON���鴮�ֽ�Ϊ���鴮
bool LibParseArrayJsonToVc(CString str, vector<CString > &vcJson)
{
	CString tmp;
	int ns, ne;
	//
	str.Trim();
	vcJson.clear();
	if (str.IsEmpty()) return false;
	//
	LibFindFisrtPairPos(L"[", str, ns, ne);
	if (ns == 0) str.SetAt(ns, ' ');
	if (ne == str.GetLength() - 1) str.SetAt(ne, ' ');
	//
	while (1) {
		str.Trim();
		if (str.IsEmpty()) break;
		LibFindFisrtPairPos(L"{", str, ns, ne);
		if (ne == 0)
		{
			vcJson.push_back(str);
			break;
		}
		else {
			tmp = str.Mid(ns, ne - ns + 1);
			vcJson.push_back(tmp);
			str = str.Right(str.GetLength() - ne - 2);
		}
	}
	return true;
}
//�����ݱ��ж�����¼����JSON�����������װ, "vector<{JSON}>"-->"[{},{},...{}]"
CString LibJson(vector<CString > &vcStr)
{
	CString str;
	int i, n;
	//
	str = L"";
	n = (int)vcStr.size();
	if (n < 1) return L"[]";
	for (i = 0; i < n; i++)
	{
		str = str + vcStr[i] + L",";
	}
	str.Trim(L",");
	str = L"[" + str + L"]";
	return str;
}
///�ѱ���1�����ݼ�¼��<key,val>��װ��1��JSON���ݶ���{key:val,...}
CString LibJson(map<CString, CString > &mpdata)
{
	map<CString, CString >::iterator it, its, ite;
	CString str, key, val, strJson, strNO;
	//
	strJson = L"";
	its = mpdata.begin();
	ite = mpdata.end();
	for (it = its; it != ite; it++)
	{
		key = it->first;
		strNO = key.Right(2);
		val = it->second;
		if (val.GetLength() > 1 && (val[0] == '[' || val[0] == '{')) {//�������ݣ���������
			str = L"\"" + key + L"\"" + L":" + val;
		}
		else if (strNO != L"NO" && LibCheckIsNum(val)) {//�������ݣ���������
			str = L"\"" + key + L"\"" + L":" + val;
		}
		else if (val.IsEmpty()) {
			str = L"\"" + key + L"\"" + L":" + L"\"" + L"\"";
		}
		else {
			str = L"\"" + key + L"\"" + L":" + L"\"" + val + L"\"";
		}
		strJson += str + L",";
	}
	if (!strJson.IsEmpty())
	{
		strJson = strJson.Left(strJson.GetLength() - 1);
	}
	strJson = L"{" + strJson + L"}";
	return strJson;
}
//�ж��ַ����Ƿ�������
bool LibCheckIsNum(CString str)
{
	if (str.IsEmpty()) return false;
	bool bIsDigit;
	int nPoint;
	int ch;
	bIsDigit = true;
	nPoint = 0;
	int nCount = str.GetLength(); // ����ַ�����
	for (int i = 0; i < nCount; i++)
	{
		//������-
		if (i == 0 && str[0] == '-') continue;
		//
		ch = str.GetAt(i);
		if (ch <= 0 || ch > 255) return false;
		if (0 == isdigit(ch)) // �������־��ñ�־λ
		{
			if (str.GetAt(i) == '.') {
				++nPoint;
				continue;
			}
			bIsDigit = false;
			break;// �˳�
		}
	}
	if (nPoint > 1) bIsDigit = false;
	return bIsDigit;
}
//��ȡӦ�ó���ĵ�ǰ·��,include "\\"
CString LibGetAppDir()
{
	static CString pathStr = L"";
	//
	if (!pathStr.IsEmpty()) return pathStr;
	//
	TCHAR   path[MAX_PATH];
	::GetCurrentDirectory(MAX_PATH, path);
	pathStr = path;
	pathStr.Trim();
	return (pathStr+L"\\");
}
//�����༶Ŀ¼
bool LibCreateDir(const CString& szPath)
{
	//�ж�Ŀ¼�Ƿ񴴽�
	CFileFind fileFind;
	//
	if (fileFind.FindFile(szPath)) return false;
	//---------------------
	CString strDir(szPath);//���Ҫ������Ŀ¼�ַ���
						   //ȷ����'\'��β�Դ������һ��Ŀ¼
	if (strDir.GetAt(strDir.GetLength() - 1) != _T('\\'))
	{
		strDir.AppendChar(_T('\\'));
	}
	vector<CString > vPath;//���ÿһ��Ŀ¼�ַ���
	CString strTemp;//һ����ʱ����,���Ŀ¼�ַ���
	BOOL bSuccess = false;//�ɹ���־
						  //����Ҫ�������ַ���
	for (int i = 0; i < strDir.GetLength(); ++i)
	{
		if (strDir.GetAt(i) != _T('\\'))
		{//�����ǰ�ַ�����'\\'
			strTemp.AppendChar(strDir.GetAt(i));
		}
		else
		{//�����ǰ�ַ���'\\'
			vPath.push_back(strTemp);//����ǰ����ַ�����ӵ�������
			strTemp.AppendChar(_T('\\'));
		}
	}

	//�������Ŀ¼������,����ÿ��Ŀ¼
	std::vector<CString>::const_iterator vIter;
	for (vIter = vPath.begin(); vIter != vPath.end(); vIter++)
	{
		//���CreateDirectoryִ�гɹ�,����true,���򷵻�false
		bSuccess = CreateDirectory(*vIter, NULL) ? true : false;
	}
	if (bSuccess) return true;
	return false;
}
//�Ƿ��ֻ���
bool LibIsPhone(u64 phone, CString &errInfo)
{
	if (phone > 13000000000 && phone < 19000000000) {
		return true;
	}
	errInfo = L"�ֻ���λ�����ʽ����";
	return false;
}
u64 LibIsPhone(CString &str)
{
	u64 phone;
	bool bPhone;
	phone=_wcstoui64(str, nullptr, 10);
	bPhone = LibIsPhone(phone, str);
	if (!bPhone) return 0;
	return phone;
}
//��ȡ��ǰʱ�� 
u64 GetTimeStamp()
{
	time_t t;
	t = time(NULL);
	time_t ii = time(&t);
	u64 nt = (u64)ii;
	return nt;

}
//��ȡ��ǰ������20160708
int GetTimeYMD()
{
	struct tm lt;
	time_t t;
	//
	t = time(NULL);
	time_t ii = time(&t);
	localtime_s(&lt, &ii);
	int nDate = (lt.tm_year + 1900) * 10000 + (lt.tm_mon + 1) * 100 + lt.tm_mday;
	return nDate;
}
//��ȡ��ǰ������20160708125959
u64 GetTimeAll()
{
	time_t t;
	struct tm lt;
	//
	t = time(NULL);
	time_t tt = time(&t);
	//
	localtime_s(&lt, &tt);
	int hms = lt.tm_hour * 10000 + lt.tm_min * 100 + lt.tm_sec;
	int ymd = (lt.tm_year + 1900) * 10000 + (lt.tm_mon + 1) * 100 + lt.tm_mday;
	u64 ntt =  ymd;
	ntt = ntt*DEC6 + hms;
	return ntt;
}
//��ȡ2016-09-09 11:22:30
CString GetDateTime()
{
	struct tm lt;
	time_t t;
	//
	t = time(NULL);
	time_t ii = time(&t);
	localtime_s(&lt, &ii);
	int nDate = (lt.tm_year + 1900) * 10000 + (lt.tm_mon + 1) * 100 + lt.tm_mday;
	CString str;
	char buf[20];
	sprintf_s(buf,"%04d-%02d-%02d %02d:%02d:%02d", (lt.tm_year + 1900), (lt.tm_mon + 1), lt.tm_mday, lt.tm_hour, lt.tm_min, lt.tm_sec);
	str=buf;
	return str;
}
//��ʱ����ֽ�Ϊ���ں�ʱ���������ֵ�ʱ���
void LibTimeStampTwoPart(u64 ntime, u64 &ymd, u64 &hms)
{
	//����ʱ��8������
	hms = (ntime+8*HOUR) % DAY;
	ymd = (ntime - hms);
}
//��ʱ����ֽ�Ϊ���ں�ʱ���������ֵ�ʮ����
void LibTimeStampYMD_HMS(u64 ntime, int &ymd, int &hms)
{
	struct tm lt;
	time_t tt = (time_t)ntime;
	//
	localtime_s(&lt, &tt);
	hms = lt.tm_hour * 10000 + lt.tm_min * 100 + lt.tm_sec;
	ymd = (lt.tm_year + 1900) * 10000 + (lt.tm_mon + 1) * 100 + lt.tm_mday;
}
int LibYmdByTimeStamp(u64 ntime)
{
	if (ntime < 1) return 0;
	//
	struct tm lt;
	time_t tt = (time_t)ntime;
	//
	localtime_s(&lt, &tt);
	int ymd = (lt.tm_year + 1900) * 10000 + (lt.tm_mon + 1) * 100 + lt.tm_mday;
	return ymd;
}
//ת��ʱ���, 2016-07-26 12:33:40
CString GetTimeStrByTimeStamp(u64 nt)
{
	struct tm lt;
	time_t tt = (time_t)nt;
	//
	localtime_s(&lt, &tt);
	//
	CString str;
	str.Format(L"%04d-%02d-%02d %02d:%02d:%02d",
		(lt.tm_year + 1900), (lt.tm_mon + 1), lt.tm_mday, lt.tm_hour, lt.tm_min, lt.tm_sec);
	return str;
}
//�õ���ǰ���ڼ�
int LibGetWeek(u64 ntime)
{
	struct tm lt;
	time_t tt = (time_t)ntime;
	//
	localtime_s(&lt, &tt);
	return lt.tm_wday;
}

//��ȡ��ǰʱ����231130
int GetTimeHMS()
{
	struct tm lt;
	time_t t;
	//
	t = time(NULL);
	time_t ii = time(&t);
	localtime_s(&lt, &ii);
	int nDate = lt.tm_hour * 10000 + lt.tm_min * 100 + lt.tm_sec;
	return nDate;
}
//����0-nt�������
int LibRandom(int nt)
{
	float rv;
	srand((unsigned)GetTickCount()); //������ͬʱ�̲�����ͬ�������
	rv = 1.0f*rand() / RAND_MAX; //����0~1.0�����
	rv = rv*nt;
	return (int)rv;
}
//��ȡ������Դ����
EnumDataType GetAppTypeSource(CString &str)
{
	int nr = TplGetMpData(glbMpClientType, str);
	return (EnumDataType)nr;
}
//�õ����ʶ�����1--user��2--doctor��3--clinic��4--system
EnumDataType AppType(EnumDataType src) {
	switch (src)
	{
	case CL_USER_ANDROID_APP:
	case CL_USER_IOS_APP:
	case CL_USER_WEIXIN_APP:
	case CL_USER_WEBSITE:
		return CL_USER;
		break;
	case CL_DOCTOR_ANDROID_APP:
	case CL_DOCTOR_IOS_APP:
	case CL_DOCTOR_PC_APP:
	case CL_DOCTOR_CLINIC_APP:
	case CL_DOCTOR_WEBSITE:
		return CL_DOCTOR;
		break;
	case CL_CLINIC_APP:
		return CL_CLINIC;
		break;
	case CL_SYSTEM_WEBSITE:
	case CL_SYSTEM_PC:
		return CL_SYSTEM;
		break;
	}
	return CL_NUL;
}
//�õ����ʶ�����1--user��2--doctor��3--clinic��4--system
EnumDataType AppType(CString &str)
{
	EnumDataType type=GetAppTypeSource(str);
	return AppType(type);
}
//�õ�Ӧ�ö˵�¼����
CString LibGetRegion(CString ipStr, float lati , float longti)
{
	CString strRegion;
	strRegion = L"�ɶ���";
	return strRegion;
}
//CString char*ת��,Ҫ�ͷ�char *
char * CStringToChar(CString &str,int &len)
{
	len = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	char *ptxtTemp = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, str, -1, ptxtTemp, len, NULL, NULL);
	return ptxtTemp;
}
//create the token, ʱ���+��Ϣ��
CString LibToken(CString strInfo)
{
	CString strToken;
	u64 ntime = GetTickCount64();
	strToken.Format(L"%I64u_%s", ntime, strInfo);
	MD5 md5;
	md5.update(strToken);
	strToken = md5.toString();
	return strToken;
}
//�ж��ַ������ԣ��ʼ����绰���˺�
EnumState LibStrPropery(CString &str, u64 &phone)
{
	bool bOK;
	//
	phone = LibIsPhone(str);
	if (phone > 0) return STA_PHONE;
	bOK = LibIsMail(str);
	if (bOK) return STA_EMAIL;
	return STA_STR;
}
//��������ŷֽ��ַ���Ϊ����Ԫ
void LibParseStrUnit(CString strInfo, CString sepator, vector<CString > &vcRes, bool bDelSep )
{
	int np;
	CString str1, str2;
	//
	vcRes.clear();
	while (true)
	{
		if (bDelSep) strInfo.Trim();
		//first character "{[(", then ",",last " "
		np = strInfo.Find(sepator);
		if (!bDelSep)
		{
			if (np == 0) np = strInfo.Find(sepator, 1);
			if (np > 0) {
				str1 = strInfo.Left(np);
				str1.Trim();
				vcRes.push_back(str1);
				strInfo = strInfo.Right(strInfo.GetLength() - np);
			}
			else {
				strInfo.Trim();
				vcRes.push_back(strInfo);
				break;
			}
		}
		else {
			if (np > 0) {
				str1 = strInfo.Left(np);
				str1.Trim();
				if (!str1.IsEmpty())
				{
					vcRes.push_back(str1);
				}
				strInfo = strInfo.Right(strInfo.GetLength() - np - sepator.GetLength());
			}
			else {
				if (strInfo.GetLength() > 0)
				{
					vcRes.push_back(strInfo);
				}
				break;
			}
		}
	}
}
//�����ж�
bool LibIsMail(CString &mail)
{
	vector<CString > vcUnit, vc1;
	int npos;
	npos = mail.Find(L"@");
	if (npos <= 0) return false;
	LibParseStrUnit(mail, L"@", vcUnit);
	if (vcUnit.size() != 2) return false;
	//
	npos = vcUnit[1].Find(L".");
	if (npos <= 0) return false;
	LibParseStrUnit(vcUnit[1], L".", vc1);
	if (vc1.size() < 1) return false;
	//
	return true;
}
//��ȡĿ¼�������ļ�
void LibAllFileInDir(CString path, CString inName, vector<CString > &vcName)
{
	CFileFind finder;
	CString str, filename;
	int np;
	//
	//������б���ж�
	if (path.Right(1) == L"\\") {
		filename = path + L"*.*";
	}
	else {
		path = path + L"\\";
		filename = path + L"*.*";
	}
	BOOL working = finder.FindFile(filename);
	while (working) {
		working = finder.FindNextFile();
		if (finder.IsDots())   continue;
		if (finder.IsDirectory()) {
			//������ȡ
			LibAllFileInDir(finder.GetFileName(), inName, vcName);
		}
		else {
			CString name = finder.GetFileName();
			if (name.Find(inName)>= 0)
			{
				np = name.Find(L".");
				if (np < 0) continue;
				vcName.push_back(name);
			}
		}
	}
}
//�ж��ļ��Ƿ����
bool LibIsFileExited(CString &fileName)
{
	CFileFind find;
	BOOL bExit=find.FindFile(fileName);
	if (bExit) return true;
	return false;
}
//ͳһ����ϵͳ������ID������λ���������ͣ�ǰ���Ǹ����͵�����ID
int LibIDCreate(int id, EnumDataType type)
{
	return (id * 100 + type);
}
//�ź�ID��ȡҽ��ID
int LibMasterIDbyReleaseID(u64 releaseID)
{
	return (int)(releaseID % DEC6);
}
//������ID��ȡ���ݲ�����ʱ���
u64 LibGetTimeByID(int datype, u64 id)
{
	u64 ntime=0;
	switch (datype)
	{
	case DAT_ACTIVITY:
		ntime = id / DEC8;
		break;
	case DAT_RELEASE:
		ntime = id / DEC6;
		break;
	case DAT_ORDER:
		ntime = id / DEC9;
		break;
	}
	return ntime;
}
//������ID��ȡ���ݲ�����������
int LibGetYmdByID(int datype, u64 id)
{
	return LibYmdByTimeStamp(LibGetTimeByID(datype,id));
}
//����ź�ID, timestamp(10)+doctorID(6)
u64 LibReleaseID(int doctorID, u64 startTime)
{
	u64 releaseID;
	//
	releaseID = startTime*DEC6 + doctorID;
	return releaseID;
}
//����Լ��ID, timestamp(10) + doctorID(6) + No(3)
u64 LibOrderID(u64 releaseID, int orderID)
{
	return (releaseID *DEC3 + orderID);
}
//��������ID,timestamp(10) + idStore(6) + idClinet(3)
u64 LibPayID(int idStore, int idClient)
{
	u64 ntime = GetTimeStamp();
	u64 id;
	id = ntime*DEC9 + idStore*DEC3 + idClient;
	return id;
}
//Լ��ID��ȡ�ź�ID
u64 LibReleaseIDbyOrderID(u64 orderID)
{
	return (orderID/ DEC3);
}
//Լ��ID��ȡҽ��
int LibMasterIDbyOrderID(u64 orderID)
{
	u64 nt = orderID%DEC9;
	return (int)( nt / DEC3);
}
//Լ��ID��ȡԼ�ﵥ��
int LibNobyOrderID(u64 orderID)
{
	return orderID % DEC3;
}
//�õ��Ա��Ӧ���ַ���
CString LibSex(int sex)
{
	if (sex==0)
	{
		return L"Ů";
	}
	return L"��";
}
//�ַ���������ת��
void LibStrNum(CString str, int &num)
{
	num = _wtoi(str);
}
void LibStrNum(CString str, u64 &num)
{
	num = _wcstoui64(str, nullptr, 10);
}
void LibStrNum(CString str, float &num)
{
	num = wcstof(str, nullptr);
}

//save the vector data to the file
void LibFileAppendInfo(CString fileName, vector<CString > &vcData)
{
	int i, n;
	//
	CFileFind find;
	CStdioFile file;
	//
	if (find.FindFile(fileName))
	{
		file.Open(fileName, CFile::modeWrite);
	}
	else { //create new file
		file.Open(fileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite);
	}
	file.Seek(0, CFile::end);
	//
	n = (int)vcData.size();
	for (i = 0; i < n; i++)
	{
		file.WriteString(vcData[i]);
	}
	file.Close();
}
//�ʱ���Ƿ����
bool LibIsExpire(u64 startTime, u64 endTime, u64 curTime)
{
	if (curTime < startTime) return true;
	if (curTime > endTime) return true;
	return false;
}
//��ñ�����IP��ַ
void GetIP(char ipstr[])
{
	// Declare and initialize variables.
	char* port = "8000";
	struct addrinfo aiHints;
	struct addrinfo *aiList = NULL;
	struct addrinfo *curr = NULL;
	int retVal;
	char  myname[100];
	//
	gethostname(myname, 100);
	//--------------------------------
	// Setup the hints address info structure
	// which is passed to the getaddrinfo() function
	memset(&aiHints, 0, sizeof(aiHints));
	aiHints.ai_family = AF_INET;
	aiHints.ai_socktype = SOCK_DGRAM;
	aiHints.ai_protocol = IPPROTO_UDP;
	//--------------------------------
	// Call getaddrinfo(). If the call succeeds,
	// the aiList variable will hold a linked list
	// of addrinfo structures containing response
	// information about the host
	if ((retVal = getaddrinfo(myname, /*����ǡ�127.0.0.1���Ļ����õ���IP��ַҲ���ǡ�127.0.0.1��,����ֻ��һ����ַ��
									  �����hostname ʱ���������������������IP��ַ����������������IP������һ����˵����һ��IPΪ����IP�ĵ�ַ��*/
		port, &aiHints, &aiList)) != 0) {
		printf("getaddrinfo() failed.\n");
	}
	//char *myip;
	for (curr = aiList; curr != NULL; curr = curr->ai_next) {
		//�������Ŀǰ��˵���ǲ�Ҫ�ã��Ͼ�IPV6�ռ�����Ҫһ�������£���Ŀǰһ�㻹����inet_ntoa
		inet_ntop(AF_INET, &(((struct sockaddr_in *)(curr->ai_addr))->sin_addr), ipstr, 16);
		/*myip = inet_ntoa(((struct sockaddr_in *)(curr->ai_addr))->sin_addr);*/
	}
	freeaddrinfo(aiList); ///���һ���ͷ��ڴ�ռ䣬��������
}
CString GetIP()
{
	char chIP[20];
	GetIP(chIP);
	CString ipStr(chIP);
	return ipStr;
}
//get the connect socket IP
CString GetConnectIP(SOCKET &serverSocket)
{
	//GET IP
	char szPeerAddress[16];
	SOCKADDR_IN addr_conn;
	int nSize = sizeof(addr_conn);
	CString strIP;
	//
	getpeername(serverSocket, (SOCKADDR *)&addr_conn, &nSize);
	memset((void *)szPeerAddress, 0, sizeof(szPeerAddress));
	//
	inet_ntop(AF_INET, &(addr_conn.sin_addr), szPeerAddress, 16);
	//
	strIP = szPeerAddress;
	return strIP;
}
//
char *UNICODE_to_UTF8(const CString &sendStr, int &len)
{
	len = WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)sendStr, -1, NULL, 0, NULL, NULL);
	char *szUtf8 = new char[len + 1];
	WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)sendStr, -1, szUtf8, len, NULL, NULL);
	szUtf8[len] = '\0';
	return szUtf8;
}


CString UTF8ToUnicode(char* UTF8)
{
	DWORD dwUnicodeLen;        //ת����Unicode�ĳ���
	TCHAR *pwText;            //����Unicode��ָ��
	CString strUnicode;        //����ֵ
							   //���ת����ĳ��ȣ��������ڴ�
	dwUnicodeLen = MultiByteToWideChar(CP_UTF8, 0, UTF8, -1, NULL, 0);
	pwText = new TCHAR[dwUnicodeLen];
	if (!pwText)
	{
		return strUnicode;
	}
	//תΪUnicode
	MultiByteToWideChar(CP_UTF8, 0, UTF8, -1, pwText, dwUnicodeLen);
	//תΪCString
	strUnicode.Format(_T("%s"), pwText);
	//����ڴ�
	FreeArr(pwText);
	//����ת���õ�Unicode�ִ�
	return strUnicode;
}

bool IsTextUTF8(char* str, ULONGLONG length)
{
	int i;
	DWORD nBytes = 0;//UFT8����1-6���ֽڱ���,ASCII��һ���ֽ�
	UCHAR chr;
	BOOL bAllAscii = true; //���ȫ������ASCII, ˵������UTF-8
	for (i = 0; i < length; i++)
	{
		chr = *(str + i);
		if ((chr & 0x80) != 0) // �ж��Ƿ�ASCII����,�������,˵���п�����UTF-8,ASCII��7λ����,����һ���ֽڴ�,���λ���Ϊ0,o0xxxxxxx
			bAllAscii = false;
		if (nBytes == 0) //�������ASCII��,Ӧ���Ƕ��ֽڷ�,�����ֽ���
		{
			if (chr >= 0x80)
			{
				if (chr >= 0xFC && chr <= 0xFD)
					nBytes = 6;
				else if (chr >= 0xF8)
					nBytes = 5;
				else if (chr >= 0xF0)
					nBytes = 4;
				else if (chr >= 0xE0)
					nBytes = 3;
				else if (chr >= 0xC0)
					nBytes = 2;
				else
				{
					return false;
				}
				nBytes--;
			}
		}
		else //���ֽڷ��ķ����ֽ�,ӦΪ 10xxxxxx
		{
			if ((chr & 0xC0) != 0x80)
			{
				return false;
			}
			nBytes--;
		}
	}
	if (nBytes > 0) //Υ������
	{
		return false;
	}
	if (bAllAscii) //���ȫ������ASCII, ˵������UTF-8
	{
		return false;
	}
	return true;
}

EnumOperate LibGetOperateState(CString &str)
{
	if (str==L"GET")
	{
		return OPER_GET;
	}else if (str == L"LIST")
	{
		return OPER_LIST;
	}
	else if (str == L"ADD")
	{
		return OPER_ADD;
	}
	else if (str == L"DEL")
	{
		return OPER_DEL;
	}
	else if (str == L"IN_LIST")
	{
		return OPER_INLIST;
	}
	else if (str == L"OUT_LIST")
	{
		return OPER_OUTLIST;
	}
	return OPE_ERR;
}
//��ȡ������ַ
CString LibGetRegion(int idProvince, int idCity, int idCounty)
{
	return L"";
}
