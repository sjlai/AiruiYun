#pragma once
#include "CommonFun.h"
#include "afxmt.h"

//状态枚举类型，（暂停状态--可恢复，删除状态--不可恢复，这两个状态不能正常工作）
template<class T> 
void FreeArr(T *&arr)
{
	if (!arr) return;
	delete[] arr;
	arr = nullptr;
}
//get maximum id
template<class T1, class T2>
T1 TplMaxID(map<T1, T2 > &mpData,int nStart=1)
{
	T1 id;
	if (mpData.size() == 0) {
		return nStart;
	}
	//
	map<T1, T2 >::reverse_iterator it;
	it = mpData.rbegin();
	id = it->first;
	++id;
	return id;
}
//////////////////////////////////////////////////////////////////////////
template<class T1, class T2>
void TplRelease(map<T1, T2* > &mpData)
{
	map<T1, T2* >::iterator it,its, ite;
	//
	its = mpData.begin();
	ite = mpData.end();
	for (it = its; it != ite;it++)
	{
		delete it->second;
		it->second = nullptr;
	}
	mpData.clear();
}
template<class T1>
void TplRelease(T1 *&pData)
{
	if (!pData) return;
	delete pData;
	pData = nullptr;
}
//////////////////////////////////////////////////////////////////////////
//serialize for container data
template<class T>
void TplSerializeContainer(CArchive &ar,vector<T> &vcData)
{
	int i, n;
	T data;
	if (ar.IsStoring()) //save
	{
		n = (int)vcData.size();
		ar << n;
		for (i = 0; i < n;i++)
		{
			ar << vcData[i];
		}
	} 
	else //read
	{
		vcData.clear();
		ar >> n;
		for (i = 0; i < n;i++)
		{
			ar >> data;
			vcData.push_back(data);
		}
	}
}
template<class T1,class T2>
void TplSerializeContainer(CArchive &ar, map<T1, T2> &mpData)
{
	int i, n;
	T1 key;
	T2 val;
	if (ar.IsStoring()) //save
	{
		n = (int)mpData.size();
		ar << n;
		//
		map<T1, T2>::iterator it, its, ite;
		its = mpData.begin();
		ite = mpData.end();
		for (it = its; it != ite;it++)
		{
			ar << it->first;
			ar << it->second;
		}
	}
	else //read
	{
		mpData.clear();
		ar >> n;
		for (i = 0; i < n; i++)
		{
			ar >> key;
			ar >> val;
			mpData[key] = val;
		}
	}
}
//serialize the map-objects
template<class T1, class T2>
void TplSerializeMapData(CArchive &ar, map<T1, T2*> &mpData)
{
	int i, n;
	T1 key;
	if (ar.IsStoring()) //save
	{
		n = (int)mpData.size();
		ar << n;
		//
		map<T1, T2*>::iterator it, its, ite;
		its = mpData.begin();
		ite = mpData.end();
		for (it = its; it != ite; it++)
		{
			ar << it->first;
			it->second->Serialize(ar);
		}
	}
	else //read
	{
		TplRelease(mpData);
		//
		ar >> n;
		for (i = 0; i < n; i++)
		{
			T2 *val=new T2();
			ar >> key;
			val->Serialize(ar);
			mpData[key] = val;
		}
	}
}
//get map data
template<class T1, class T2>
T2 TplGetMpData(map<T1, T2 > &mpData, T1 &key)
{
	map<T1, T2 >::iterator it, ite;
	//
	ite = mpData.end();
	it = mpData.find(key);
	if (it != ite) return it->second;
	return 0;
}
template<class T1, class T2>
bool TplGetMpData(map<T1, T2 > &mpData, T1 &key,T2 &val)
{
	map<T1, T2 >::iterator it, ite;
	//
	ite = mpData.end();
	it = mpData.find(key);
	if (it != ite) {
		val = it->second;
		return true;
	}
	return false;
}
//get data
template<class T1, class T2>
T2 *TplGetData(map<T1, T2* > &mpData, T1 &key)
{
	map<T1, T2* >::iterator it, ite;
	//
	ite = mpData.end();
	it = mpData.find(key);
	if (it == ite) return nullptr;
	return it->second;
}
//del data
template<class T1, class T2>
bool TplDelData(map<T1, T2* > &mpData, T1 &key,bool bDel=false)
{
	T2 *pData;
	//
	pData = TplGetData(mpData, key);
	if (!pData) return false;
	if (bDel)
	{
		map<T1, T2* >::iterator it;
		it = mpData.find(key);
		delete it->second;
		it->second = nullptr;
		mpData.erase(it);
	} 
	else
	{
		pData->state = STA_DEL;
	}
	return true;
}
//add new data, return true-add, false-modify
template<class T1,class T2>
bool TplAddData(map<T1,T2* > &mpData, T2 *data,int nStart=1)
{
	T1 key;
	//
	key = data->id;
	if (key != 0) { //modify
		map<T1, T2* >::iterator it, ite;
		ite = mpData.end();
		it = mpData.find(key);
		if (it == ite) return false;
		//delete old
		delete it->second;
		it->second = nullptr;
		mpData.erase(it);
		//add new
		mpData[key] = data;
		return true;
	}
	//add new
	key = TplMaxID(mpData,nStart);
	data->id = key;
	mpData[key] = data;
	return true;
}
//mapdata to one file, pathName include the "\\",name not include expand name
template<class T1, class T2>
void TplFileData(CString pathName,CString name, map<T1, T2* > &mpData, bool bSave)
{
	CFile file;
	byte *pBuffer;
	UINT nLen;
	CMemFile memFile;
	CString fileName;
	//
	fileName = pathName + name + L".dat";
	if (bSave) //save
	{
		LibCreateDir(pathName);
		//
		CArchive ar(&memFile, CArchive::store);
		TplSerializeMapData(ar, mpData);
		ar.Close();
		//
		nLen = (UINT)(memFile.GetLength());
		pBuffer = new byte[nLen];
		pBuffer = (byte*)memFile.Detach();
		//
		file.Open(fileName, CFile::modeCreate | CFile::modeWrite);
		file.Write(pBuffer, nLen);
		file.Close();
		//
		FreeArr(pBuffer);
	}
	else //read
	{
		CFileFind find;
		if (!find.FindFile(fileName)) return;
		//
		TplRelease(mpData);
		//
		file.Open(fileName, CFile::modeRead);
		nLen = (UINT)(file.GetLength());
		pBuffer = new byte[nLen];
		file.Read(pBuffer, nLen);
		file.Close();
		//
		memFile.Attach((unsigned char*)pBuffer, nLen);
		memFile.SeekToBegin();
		//
		CArchive ar(&memFile, CArchive::load);
		TplSerializeMapData(ar,mpData);
		ar.Close();
		//
		pBuffer = (byte*)memFile.Detach();
		//
		FreeArr(pBuffer);
	}
}
//mapdata to one file, pathName include the "\\",name not include expand name
template<class T1, class T2>
void TplFileData(CString pathName, CString name, map<T1, T2> &mpData, bool bSave)
{
	CFile file;
	byte *pBuffer;
	UINT nLen;
	CMemFile memFile;
	CString fileName;
	//
	fileName = pathName + name + L".dat";
	if (bSave) //save
	{
		LibCreateDir(pathName);
		//
		CArchive ar(&memFile, CArchive::store);
		TplSerializeContainer(ar, mpData);
		ar.Close();
		//
		nLen = (UINT)(memFile.GetLength());
		pBuffer = new byte[nLen];
		pBuffer = (byte*)memFile.Detach();
		//
		file.Open(fileName, CFile::modeCreate | CFile::modeWrite);
		file.Write(pBuffer, nLen);
		file.Close();
		//
		FreeArr(pBuffer);
	}
	else //read
	{
		CFileFind find;
		if (!find.FindFile(fileName)) return;
		//
		mpData.clear();
		//
		file.Open(fileName, CFile::modeRead);
		nLen = (UINT)(file.GetLength());
		pBuffer = new byte[nLen];
		file.Read(pBuffer, nLen);
		file.Close();
		//
		memFile.Attach((unsigned char*)pBuffer, nLen);
		memFile.SeekToBegin();
		//
		CArchive ar(&memFile, CArchive::load);
		TplSerializeContainer(ar, mpData);
		ar.Close();
		//
		pBuffer = (byte*)memFile.Detach();
		FreeArr(pBuffer);
	}
}
//save one data to one a file
template<class T1 ,class T2 >
bool TplOneDataOneFile(CString &pathName,CString &name,T1 &key, T2 *&data, bool bSave,bool bSaveTmp=false)
{
	CMemFile memFile;
	CFile file;
	CString fileName,hisFileName,newPath;
	int nLen;
	byte *pBuffer;
	//
	try
	{
		if (bSave)
		{
			if (!data) return false;
			fileName.Format(L"%s%s_%05I64u.dat", pathName, name, key);
			CArchive ar(&memFile, CArchive::store);
			data->Serialize(ar);
			ar.Close();
			//
			nLen = (UINT)(memFile.GetLength());
			pBuffer = new byte[nLen];
			pBuffer = (byte*)memFile.Detach();
			//
			file.Open(fileName, CFile::modeCreate | CFile::modeWrite);
			file.Write(pBuffer, nLen);
			file.Close();
			//
			FreeArr(pBuffer);
			if (bSaveTmp)
			{
				//备份历史记录，history\filename_time
				newPath = pathName + L"history\\";
				LibCreateDir(newPath);
				hisFileName.Format(L"%s%s_%05I64u_%I64u.dat", newPath, name, key, GetTimeAll());
				//
				CopyFile(fileName,hisFileName,FALSE);
			}
		}
		else
		{
			//
			if(data != nullptr ) return false;
			fileName.Format(L"%s%s", pathName, name);
			if (!LibIsFileExited(fileName)) return false;
			file.Open(fileName, CFile::modeRead);
			nLen = (UINT)(file.GetLength());
			pBuffer = new byte[nLen];
			file.Read(pBuffer, nLen);
			file.Close();
			//
			memFile.Attach((unsigned char*)pBuffer, nLen);
			memFile.SeekToBegin();
			//
			data = new T2();
			//
			CArchive ar(&memFile, CArchive::load);
			data->Serialize(ar);
			ar.Close();
			//
			key = data->id;
			//
			pBuffer = (byte*)memFile.Detach();
			FreeArr(pBuffer);
		}
	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		if (!bSave && data) {
			delete data;
			data = nullptr;
		}
		return false;
	}
	catch (CFileException* e)
	{
		e->ReportError();
		if (!bSave && data) {
			delete data;
			data = nullptr;
		}
		return false;
	}
	catch (CException* e)
	{
		e->ReportError();
		if (!bSave && data) {
			delete data;
			data = nullptr;
		}
		return false;
	}
	return true;
}

extern bool glb_bRun;
extern CRITICAL_SECTION glb_csFile;
//mapdata to each file, pathName include the "\\",name not include expand name
//idt>0 表示单文件操作
template<class T1, class T2>
void TplMulFileData(CString pathName, CString name, map<T1, T2* > &mpData, bool bSave, u64 idt = 0,bool bSaveTmp=false)
{
	T1 key;
	T2 *pData = nullptr;
	//文件操作排队操作，减少磁盘问题
	if(glb_bRun) EnterCriticalSection(&glb_csFile);
	if (bSave) //save
	{
		LibCreateDir(pathName);
		//单文件保存
		if (idt>0)
		{
			pData = TplGetData(mpData, idt);
			if(pData) TplOneDataOneFile(pathName, name, idt, pData, bSave, bSaveTmp);
		}
		else {
			map<T1, T2* >::iterator it, its, ite;
			its = mpData.begin();
			ite = mpData.end();
			for (it = its; it != ite;it++)
			{
				key = it->first;
				TplOneDataOneFile(pathName, name, key, it->second, bSave, bSaveTmp);
			}
		}

	}
	else //read
	{
		//read all the specified filename in the directory
		vector<CString > vcFileName;
		int i, N;
		CString fileName;
		////单文件读取
		if (idt>0)
		{
			fileName.Format(L"%s_%05I64u.dat", name, key);
			TplOneDataOneFile(pathName, fileName, key, pData, bSave);
			//add data
			if(pData) mpData[key] = pData;
		}
		else {
			LibAllFileInDir(pathName, name, vcFileName);
			N = (int)vcFileName.size();
			if (N < 1) return;
			TplRelease(mpData);
			//
			for (i = 0; i < N;i++)
			{
				T2 *data=nullptr;
				TplOneDataOneFile(pathName, vcFileName[i], key, data, bSave);
				//add data
				if(data) mpData[key] = data;
			}
		}
	}
	if (glb_bRun) LeaveCriticalSection(&glb_csFile);
}
//mapdata to each file in one day category
template<class T1, class T2>
void TplMulFileDataInDay(EnumDataType dataType,  CString &pathName, CString &name, map<T1, T2* > &mpData, bool bSave)
{
	T1 key;
	int ymd;
	CString subPath;
	//
	if (bSave) //save
	{
		//
		map<T1, T2* >::iterator it, its, ite;
		its = mpData.begin();
		ite = mpData.end();
		for (it = its; it != ite; it++)
		{
			key = it->first;
			ymd = LibGetYmdByID(dataType, key);
			subPath.Format(L"%s%04d\\", pathName, ymd);
			LibCreateDir(subPath);
			TplOneDataOneFile(subPath, name, key, it->second, bSave);
		}
	}
	else //read the current day file
	{
		//read all the specified filename in the directory and sub-directory
		vector<CString > vcFileName;
		int i, N;
		//
		ymd = GetTimeYMD();
		subPath.Format(L"%s%04d\\",pathName, ymd);
		//
		LibAllFileInDir(subPath, name, vcFileName);
		N = (int)vcFileName.size();
		if (N < 1) return;
		TplRelease(mpData);
		//
		for (i = 0; i < N; i++)
		{
			T2 *data=nullptr;
			TplOneDataOneFile(pathName, vcFileName[i], key, data, bSave);
			//add data
			mpData[key] = data;
		}
	}
}


//空对象错误信息提示, true-error information
template<class T>
bool LibOjbErrInfo(T *pObj,CString tipObj, CString &strInfo)
{
	if (!pObj) {
		strInfo = tipObj+L"数据不存在！";
		return false;
	}
	return true;
}
//判断容器中是否存在数据
template<class T>
bool TplIsExit(T key, vector<T> &vcData)
{
	vector<T>::iterator it, ite;
	it = std::find(vcData.begin(), vcData.end(), key);
	ite = vcData.end();
	if (it == ite) return false;
	return true;
}
//map数据对象生成JSON数组[{key:**,val:**},...]
template<class T1,class T2>
CString LibMpDataJson(map<T1, T2> &mpData, CString key, CString val)
{
	map<T1, T2>::iterator it, its, ite;
	CString strArr,strRes;
	//
	its = mpData.begin();
	ite = mpData.end();
	for (it = its; it != ite;it++)
	{
		strArr.Format(L"{\"%s\":%I64u,\"%s\":%s},", key, it->first, val, it->second);
		strRes = strRes + strArr;
	}
	strRes.TrimRight(L",");
	return (L"["+strRes+L"]");
}
/////////////////////////////
//容器数据添加
template<class T1, class T2>
void TplMpDataNoRepeatSecondAdd(map<T1, T2 > &mpData, T1 key,T2 &data)
{
	map<T1, T2 >::iterator it, its, ite;
	//
	its = mpData.begin();
	ite = mpData.end();
	for (it = its; it != ite;it++)
	{
		if (it->second == data) return;
	}
	mpData[key] = data;
}
template<class T>
void TplVcDataAdd(vector<T > &vcData, T data)
{
	vector<T>::iterator it, ite;
	//
	it = std::find(vcData.begin(), vcData.end(), data);
	ite = vcData.end();
	if (it == ite) vcData.push_back(data);
}
//容器数据删除
template<class T>
void TplVcDataDel(vector<T > &vcData, T data)
{
	vector<T>::iterator it, ite;
	//
	it = std::find(vcData.begin(), vcData.end(), data);
	ite = vcData.end();
	if (it != ite) vcData.erase(it);
}
template<class T1,class T2>
void TplMpDataDel(map<T1, T2 > &mpData, T1 key)
{
	vector<T>::iterator it, ite;
	//
	it = mpData.find(key);
	ite = mpData.end();
	if (it != ite) mpData.erase(it);
}
//删除map<T1, map<T2, T3> > 容器vector唯一数据
template<class T1, class T2, class T3>
void TplMpvcDataDel(map<T1 , map<T2 , T3 > > &mpvcData, T1 key1, T2 key2)
{
	map<T1 , map<T2 , T3 > >::iterator it, ite;
	//
	it = mpvcData.find(key1);
	ite = mpvcData.end();
	if (it == ite) return;
	//
	map<T2, T3 >::iterator ita, itae;
	itae = it->second.end();
	ita = it->second.find(key2);
	if (ita == itae) return;
	it->second.erase(ita);
}
//处理操作过期时间数据
template<class T1,class T2>
void TplOverDueMpData(u64 cutTime,map<T1, T2 *> &mpData)
{
	map<T1, T2 *>::iterator it, its, ite;
	T2 *pData;
	//
	its = mpData.begin();
	ite = mpData.end();
	for (it = its; it != ite;)
	{
		pData = it->second;
		if (pData->doneTime<cutTime)
		{
			delete pData;
			pData = nullptr;
			mpData.erase(it++);
		}
		else {
			it++;
		}
	}
}
//处理操作过期时间数据
template<class T1, class T2>
void TplOverDueMpDataKey(int dataType,u64 cutTime, map<T1, T2 *> &mpData)
{
	map<T1, T2 *>::iterator it, its, ite;
	T1 key;
	T2 *pData;
	u64 nt;
	//
	its = mpData.begin();
	ite = mpData.end();
	for (it = its; it != ite;)
	{
		key = it->first;
		nt = LibGetTimeByID(dataType,key);
		if (nt < cutTime)
		{
			pData = it->second;
			delete pData;
			pData = nullptr;
			mpData.erase(it++);
		}
		else {
			it++;
		}
	}
}