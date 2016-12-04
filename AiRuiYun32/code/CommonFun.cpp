#include "stdafx.h"
#include "CommonFun.h"
#include "TemplateFun.h"
#include "md5.h"
//
map<CString, int> glbMpClientType;
//
//初始化全局容器变量
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


//是否为JSON串
bool LibIsJson(CString str, bool &bArr)
{
	int np;
	bArr = false;
	np = str.Find(L"\":");
	if (str[0] == '[' && str[str.GetLength() - 1] == ']') bArr = true;
	if (np > 0) return true;
	return false;
}
//查找字符中成对符号的位置
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
	//后面的第一对
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
//把Json串解析为一层key，value对象
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
		//remove “ front and back
		key.Trim(); 
		key.Trim( L"\""); 
		val.Trim(); 
		val.Trim(L"\"");
		//add key-val
		mpJson.insert(make_pair(key, val));
	}
	return true;
}
//把JSON数组串分解为数组串
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
//把数据表中多条记录对象JSON串进行数组封装, "vector<{JSON}>"-->"[{},{},...{}]"
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
///把表中1条数据记录的<key,val>封装成1个JSON数据对象{key:val,...}
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
		if (val.GetLength() > 1 && (val[0] == '[' || val[0] == '{')) {//数组数据，不加引号
			str = L"\"" + key + L"\"" + L":" + val;
		}
		else if (strNO != L"NO" && LibCheckIsNum(val)) {//数字数据，不加引号
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
//判断字符串是否是数字
bool LibCheckIsNum(CString str)
{
	if (str.IsEmpty()) return false;
	bool bIsDigit;
	int nPoint;
	int ch;
	bIsDigit = true;
	nPoint = 0;
	int nCount = str.GetLength(); // 获得字符个数
	for (int i = 0; i < nCount; i++)
	{
		//处理负号-
		if (i == 0 && str[0] == '-') continue;
		//
		ch = str.GetAt(i);
		if (ch <= 0 || ch > 255) return false;
		if (0 == isdigit(ch)) // 不是数字就置标志位
		{
			if (str.GetAt(i) == '.') {
				++nPoint;
				continue;
			}
			bIsDigit = false;
			break;// 退出
		}
	}
	if (nPoint > 1) bIsDigit = false;
	return bIsDigit;
}
//读取应用程序的当前路径,include "\\"
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
//创建多级目录
bool LibCreateDir(const CString& szPath)
{
	//判断目录是否创建
	CFileFind fileFind;
	//
	if (fileFind.FindFile(szPath)) return false;
	//---------------------
	CString strDir(szPath);//存放要创建的目录字符串
						   //确保以'\'结尾以创建最后一个目录
	if (strDir.GetAt(strDir.GetLength() - 1) != _T('\\'))
	{
		strDir.AppendChar(_T('\\'));
	}
	vector<CString > vPath;//存放每一层目录字符串
	CString strTemp;//一个临时变量,存放目录字符串
	BOOL bSuccess = false;//成功标志
						  //遍历要创建的字符串
	for (int i = 0; i < strDir.GetLength(); ++i)
	{
		if (strDir.GetAt(i) != _T('\\'))
		{//如果当前字符不是'\\'
			strTemp.AppendChar(strDir.GetAt(i));
		}
		else
		{//如果当前字符是'\\'
			vPath.push_back(strTemp);//将当前层的字符串添加到数组中
			strTemp.AppendChar(_T('\\'));
		}
	}

	//遍历存放目录的数组,创建每层目录
	std::vector<CString>::const_iterator vIter;
	for (vIter = vPath.begin(); vIter != vPath.end(); vIter++)
	{
		//如果CreateDirectory执行成功,返回true,否则返回false
		bSuccess = CreateDirectory(*vIter, NULL) ? true : false;
	}
	if (bSuccess) return true;
	return false;
}
//是否手机号
bool LibIsPhone(u64 phone, CString &errInfo)
{
	if (phone > 13000000000 && phone < 19000000000) {
		return true;
	}
	errInfo = L"手机号位数或格式错误";
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
//读取当前时间 
u64 GetTimeStamp()
{
	time_t t;
	t = time(NULL);
	time_t ii = time(&t);
	u64 nt = (u64)ii;
	return nt;

}
//读取当前年月日20160708
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
//读取当前年月日20160708125959
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
//读取2016-09-09 11:22:30
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
//把时间戳分解为日期和时间两个部分的时间戳
void LibTimeStampTwoPart(u64 ntime, u64 &ymd, u64 &hms)
{
	//北京时间8区处理
	hms = (ntime+8*HOUR) % DAY;
	ymd = (ntime - hms);
}
//把时间戳分解为日期和时间两个部分的十进制
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
//转换时间戳, 2016-07-26 12:33:40
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
//得到当前星期几
int LibGetWeek(u64 ntime)
{
	struct tm lt;
	time_t tt = (time_t)ntime;
	//
	localtime_s(&lt, &tt);
	return lt.tm_wday;
}

//读取当前时分秒231130
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
//产生0-nt的随机数
int LibRandom(int nt)
{
	float rv;
	srand((unsigned)GetTickCount()); //避免相同时刻产生相同的随机数
	rv = 1.0f*rand() / RAND_MAX; //产生0~1.0随机数
	rv = rv*nt;
	return (int)rv;
}
//读取访问来源属性
EnumDataType GetAppTypeSource(CString &str)
{
	int nr = TplGetMpData(glbMpClientType, str);
	return (EnumDataType)nr;
}
//得到访问端类型1--user，2--doctor，3--clinic，4--system
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
//得到访问端类型1--user，2--doctor，3--clinic，4--system
EnumDataType AppType(CString &str)
{
	EnumDataType type=GetAppTypeSource(str);
	return AppType(type);
}
//得到应用端登录区域
CString LibGetRegion(CString ipStr, float lati , float longti)
{
	CString strRegion;
	strRegion = L"成都市";
	return strRegion;
}
//CString char*转换,要释放char *
char * CStringToChar(CString &str,int &len)
{
	len = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	char *ptxtTemp = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, str, -1, ptxtTemp, len, NULL, NULL);
	return ptxtTemp;
}
//create the token, 时间戳+信息串
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
//判断字符串属性，邮件、电话、账号
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
//按隔离符号分解字符串为各单元
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
//邮箱判断
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
//读取目录下所有文件
void LibAllFileInDir(CString path, CString inName, vector<CString > &vcName)
{
	CFileFind finder;
	CString str, filename;
	int np;
	//
	//包含反斜杠判断
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
			//继续读取
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
//判断文件是否存在
bool LibIsFileExited(CString &fileName)
{
	CFileFind find;
	BOOL bExit=find.FindFile(fileName);
	if (bExit) return true;
	return false;
}
//统一生成系统的数据ID，后两位是数据类型，前面是该类型的数据ID
int LibIDCreate(int id, EnumDataType type)
{
	return (id * 100 + type);
}
//放号ID读取医生ID
int LibMasterIDbyReleaseID(u64 releaseID)
{
	return (int)(releaseID % DEC6);
}
//由数据ID读取数据产生的时间戳
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
//由数据ID读取数据产生的年月日
int LibGetYmdByID(int datype, u64 id)
{
	return LibYmdByTimeStamp(LibGetTimeByID(datype,id));
}
//分配放号ID, timestamp(10)+doctorID(6)
u64 LibReleaseID(int doctorID, u64 startTime)
{
	u64 releaseID;
	//
	releaseID = startTime*DEC6 + doctorID;
	return releaseID;
}
//分配约诊ID, timestamp(10) + doctorID(6) + No(3)
u64 LibOrderID(u64 releaseID, int orderID)
{
	return (releaseID *DEC3 + orderID);
}
//分配零售ID,timestamp(10) + idStore(6) + idClinet(3)
u64 LibPayID(int idStore, int idClient)
{
	u64 ntime = GetTimeStamp();
	u64 id;
	id = ntime*DEC9 + idStore*DEC3 + idClient;
	return id;
}
//约诊ID读取放号ID
u64 LibReleaseIDbyOrderID(u64 orderID)
{
	return (orderID/ DEC3);
}
//约诊ID读取医生
int LibMasterIDbyOrderID(u64 orderID)
{
	u64 nt = orderID%DEC9;
	return (int)( nt / DEC3);
}
//约诊ID读取约诊单号
int LibNobyOrderID(u64 orderID)
{
	return orderID % DEC3;
}
//得到性别对应的字符串
CString LibSex(int sex)
{
	if (sex==0)
	{
		return L"女";
	}
	return L"男";
}
//字符串向数字转换
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
//活动时间是否过期
bool LibIsExpire(u64 startTime, u64 endTime, u64 curTime)
{
	if (curTime < startTime) return true;
	if (curTime > endTime) return true;
	return false;
}
//获得本机的IP地址
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
	if ((retVal = getaddrinfo(myname, /*如果是“127.0.0.1”的话，得到的IP地址也会是“127.0.0.1”,而且只有一个地址。
									  传入的hostname 时，会输出本机所以网卡的IP地址，包括虚拟网卡的IP，但是一般来说，第一个IP为物理IP的地址。*/
		port, &aiHints, &aiList)) != 0) {
		printf("getaddrinfo() failed.\n");
	}
	//char *myip;
	for (curr = aiList; curr != NULL; curr = curr->ai_next) {
		//这个函数目前来说还是不要用，毕竟IPV6普及还需要一定的年月，所目前一般还是用inet_ntoa
		inet_ntop(AF_INET, &(((struct sockaddr_in *)(curr->ai_addr))->sin_addr), ipstr, 16);
		/*myip = inet_ntoa(((struct sockaddr_in *)(curr->ai_addr))->sin_addr);*/
	}
	freeaddrinfo(aiList); ///最后一个释放内存空间，别忘记了
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
	DWORD dwUnicodeLen;        //转换后Unicode的长度
	TCHAR *pwText;            //保存Unicode的指针
	CString strUnicode;        //返回值
							   //获得转换后的长度，并分配内存
	dwUnicodeLen = MultiByteToWideChar(CP_UTF8, 0, UTF8, -1, NULL, 0);
	pwText = new TCHAR[dwUnicodeLen];
	if (!pwText)
	{
		return strUnicode;
	}
	//转为Unicode
	MultiByteToWideChar(CP_UTF8, 0, UTF8, -1, pwText, dwUnicodeLen);
	//转为CString
	strUnicode.Format(_T("%s"), pwText);
	//清除内存
	FreeArr(pwText);
	//返回转换好的Unicode字串
	return strUnicode;
}

bool IsTextUTF8(char* str, ULONGLONG length)
{
	int i;
	DWORD nBytes = 0;//UFT8可用1-6个字节编码,ASCII用一个字节
	UCHAR chr;
	BOOL bAllAscii = true; //如果全部都是ASCII, 说明不是UTF-8
	for (i = 0; i < length; i++)
	{
		chr = *(str + i);
		if ((chr & 0x80) != 0) // 判断是否ASCII编码,如果不是,说明有可能是UTF-8,ASCII用7位编码,但用一个字节存,最高位标记为0,o0xxxxxxx
			bAllAscii = false;
		if (nBytes == 0) //如果不是ASCII码,应该是多字节符,计算字节数
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
		else //多字节符的非首字节,应为 10xxxxxx
		{
			if ((chr & 0xC0) != 0x80)
			{
				return false;
			}
			nBytes--;
		}
	}
	if (nBytes > 0) //违返规则
	{
		return false;
	}
	if (bAllAscii) //如果全部都是ASCII, 说明不是UTF-8
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
//读取三级地址
CString LibGetRegion(int idProvince, int idCity, int idCounty)
{
	return L"";
}
