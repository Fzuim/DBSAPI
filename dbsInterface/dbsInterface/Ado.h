/*########################################################################
	Filename: 	ado.h
	----------------------------------------------------
		-- -- 湖南城市学院 信息工程系 000641班
		-- -- 成真
		-- -- anyou@sina.com
		-- -- anyou@msn.com
		-- -- 20/1/2003 19:08
	----------------------------------------------------
	【ado封装类修正版】:
		主要修正了智能指针没有及时释放的bug, 删除oleinit()函数，因为觉得不妥。
		增加了记录集存取函数.

	【注意事项】:
		1. 必须在下面指明msado15.dll的全路径,一般在"C:\Program Files\Common
		   Files\System\ADO\"目录下.
		2. 程序必须要初始化COM环境,可以调用CoInitialize(0)来初始化; 
		   用CoUninitialize()释放;
		3. 在使用记录集对象前必须先调用CAdoConnection::Connect()连接数据库,
		   连接后可给多个CAdoRecordSet对象使用, 用完后请关闭数据库.
		5. 打开记录集前, 先调用CAdoRecordSet::SetAdoConnection()设置连接对
		   象, 用完后请关闭记录集.
		6. 在编译过程中不用理会下面的编译警告:
		   warning: unary minus operator applied to unsigned type, result
		   still unsigned
		   如果不想此警告出现,可以在StdAfx.h文件中加入这样一行代码以禁止此
		   警告: 	#pragma warning(disable:4146) 
  ########################################################################*/
#if !defined(_ANYOU_COOL_ADOCONNETION_H)
#define _ANYOU_COOL_ADOCONNETION_H

#if _MSC_VER > 1000
#pragma once
#endif 
 
//注意，编译时候很容易因为这两个导入库导致编译失败。具体解决办法：到C:\Program Files\CommonFiles\System\ADO目录下将msado15.dll拷贝到程序目录下即可
//note by huangdq
#import "msado15.dll" no_namespace rename("EOF","adoEOF"),rename("BOF","adoBOF")
//#import "msado20.tlb" no_namespace rename("EOF","adoEOF"),rename("BOF","adoBOF")
#import "MSJRO.DLL" 

/*########################################################################
			------------------------------------------------
						CAdoConnection class
			------------------------------------------------
  ########################################################################*/
class CAdoConnection
{
//构建-----------------------------------------------
public:
	CAdoConnection();
	virtual ~CAdoConnection();

//属性-----------------------------------------------
public:
	BOOL SetConnectTimeOut(long lTime);
	BOOL IsOpen();
	_ConnectionPtr GetConnection();
	
//操作-----------------------------------------------
public:
	BOOL ExecSql( const char* v_szSql );
	ErrorsPtr GetErrors();
	BOOL Connect(LPCTSTR strConnect, CString& v_cstrError, long lOptions = adConnectUnspecified);
	BOOL Disconnect();

	BOOL Cancel();

	long BeginTrans();
	BOOL RollbackTrans();
	BOOL CommitTrans();

	_RecordsetPtr OpenSchema(SchemaEnum QueryType);
	CString GetLastError(DWORD* v_pdwError=NULL);
	_ConnectionPtr	m_pConnection;

//数据-----------------------------------------------
protected:
	BOOL Release();
	CString			m_strConnect;
};

/*########################################################################
			------------------------------------------------
						CAdoRecordSet class
			------------------------------------------------
  ########################################################################*/

class CAdoRecordSet 
{
//构建 ------------------------------------------------
public:
	CAdoRecordSet();
	CAdoRecordSet(CAdoConnection *pConnection);
	virtual ~CAdoRecordSet();
	HRESULT Open(LPCTSTR strSQL, long lOption = adCmdText, CursorTypeEnum CursorType = adOpenStatic, LockTypeEnum LockType = adLockOptimistic);
	void SetAdoConnection(CAdoConnection *pConnection);
	BOOL Close();
	
//属性 ------------------------------------------------	
public:
	BOOL IsEOF();
	BOOL IsBOF();
	
	long GetState();
	long GetStatus();
	
	long GetRecordCount();
	long GetFieldsCount();

	CString GetLastError();

	CString GetFieldName(long lIndex);
	DWORD GetFieldType(long lIndex);
	DWORD GetFieldType(LPCTSTR strfield);
	
	long GetFieldAttributes(long lIndex);
	long GetFieldDefineSize(long lIndex);
	long GetFieldActualSize(long lIndex);

	Fields* GetFields();
	FieldPtr GetField(long lIndex);
	const _RecordsetPtr& GetRecordset();
	
//记录集更新操作 --------------------------------------
public:
	BOOL AddNew();
	BOOL Update();
	BOOL CancelUpdate();
	BOOL CancelBatch(AffectEnum AffectRecords = adAffectAll);
	BOOL Delete(AffectEnum AffectRecords = adAffectCurrent);

//记录集导航操作 --------------------------------------
public:
	BOOL MoveFirst();
	BOOL MovePrevious();
	BOOL MoveNext();
	BOOL MoveLast();
	BOOL Move(long lRecords, long Start = adBookmarkCurrent);
	
//其他方法 --------------------------------------------
public:
	BOOL IsOpen();
	BOOL PutCollect(long index, const _variant_t &value);
	BOOL PutCollect(long index, const LPCSTR &value);
	BOOL PutCollect(long index, const double &value);
	BOOL PutCollect(long index, const float &value);
	BOOL PutCollect(long index, const long &value);
	BOOL PutCollect(long index, const int &value);
	BOOL PutCollect(long index, const short &value);
	BOOL PutCollect(long index, const BYTE &value);

	BOOL PutCollect(LPCSTR strFieldName, _variant_t &value);
	BOOL PutCollect(LPCSTR strFieldName, const LPCSTR &value);
	BOOL PutCollect(LPCSTR strFieldName, const double &value);
	BOOL PutCollect(LPCSTR strFieldName, const float &value);
	BOOL PutCollect(LPCSTR strFieldName, const long &value);
	BOOL PutCollect(LPCSTR strFieldName, const int &value);
	BOOL PutCollect(LPCSTR strFieldName, const short &value);
	BOOL PutCollect(LPCSTR strFieldName, const BYTE &value);
	
	BOOL GetCollect(long index, CString &value);
	BOOL GetCollect(long index, double &value);
	BOOL GetCollect(long index, float &value);
	BOOL GetCollect(long index, long &value);
	BOOL GetCollect(long index, int& value);
	BOOL GetCollect(long index, short &value);
	
	BOOL GetCollect(LPCSTR strFieldName, BYTE &value);

	BOOL GetCollect(LPCSTR strFieldName, double &value);
	BOOL GetCollect(LPCSTR strFieldName, float &value);
	BOOL GetCollect(LPCSTR strFieldName, long &value);
	BOOL GetCollect(LPCSTR strFieldName, int& value);
	BOOL GetCollect(LPCSTR strFieldName, short &value);
	BOOL GetCollect(long index, BYTE &value);

	BOOL Supports(CursorOptionEnum CursorOptions = adAddNew);
	BOOL SetCacheSize(const long& lCacheSize);
	BOOL Cancel();
	BOOL GetValueString(CString &strValue, CString strFieldName);
	BOOL GetValueString(CString &strValue, long lFieldIndex);

	_RecordsetPtr operator =(_RecordsetPtr &pRecordSet);
	CString Format(_variant_t var);
	
//成员变量 --------------------------------------------
protected:
	BOOL Release();
	
	CAdoConnection     *m_pConnection;
	_RecordsetPtr		m_pRecordset;
	CString				m_strSQL;
};

#endif // !defined(_ANYOU_COOL_ADOCONNETION_H)










































