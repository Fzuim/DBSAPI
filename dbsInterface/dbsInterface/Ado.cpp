/*####################################################################
	Filename: 	ado.cpp
	----------------------------------------------------
	Remarks:	...
	----------------------------------------------------
	Author:		����
	Email:		anyou@sina.com
				anyou@msn.com
	Created:	20/1/2003 19:19
  ####################################################################*/
#include "stdafx.h"
#include <math.h>
#include "ADO.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


/*####################################################################
			------------------------------------------------
						CAdoConnection class
			------------------------------------------------
  ####################################################################*/

CAdoConnection::CAdoConnection()
{
	///���� Connection ����---------------------------
	m_pConnection.CreateInstance("ADODB.Connection");
	ASSERT(m_pConnection != NULL);
}

CAdoConnection::~CAdoConnection()
{
	if (m_pConnection != NULL)
	{
		Release();
	}
}

/*===================================================================
	Name:		���ӵ�����Դ.
	-----------------------------------------------------
	Params:		strConnect: �����ַ���,����������Ϣ.
				lOptions:	��ѡ�������÷����������ӽ���֮��(�첽)��������
						����֮ǰ(ͬ��)���ء�����������ĳ��������
		����					˵�� 
		adConnectUnspecified	(Ĭ��)ͬ����ʽ�����ӡ� 
		adAsyncConnect			�첽��ʽ�����ӡ�Ado�� ConnectComplete ��
							����֪ͨ��ʱ������ӡ� 
=====================================================================*/
BOOL CAdoConnection::Connect(LPCTSTR strConnect, CString& v_cstrError, long lOptions)
{
	ASSERT(m_pConnection != NULL);
	m_strConnect = strConnect;
	try
	{
		// �������ݿ�---------------------------------------------
		return SUCCEEDED(m_pConnection->Open(strConnect, "", "", lOptions));
	}
	catch (_com_error e)
	{
		TRACE(_T(":( �������ݿⷢ���쳣: %s\n"), e.ErrorMessage());
		v_cstrError = e.ErrorMessage();
		return FALSE;
	} 
	catch (...)
	{
		TRACE(_T(":( �������ݿ�ʱ����δ֪����:"));
		v_cstrError = "Unknowable error";
	}
	return FALSE;
}

/*===================================================================
	Name:	�ر�������Դ������.
	-----------------------------------------------------
	Remarks: ʹ�� Close �����ɹر� Connection �����Ա��ͷ����й�����ϵͳ��
		Դ���رն��󲢷ǽ������ڴ���ɾ�������Ը��������������ò����ڴ˺���
		�δ򿪡�Ҫ��������ڴ�����ȫɾ���������Release������
=====================================================================*/
BOOL CAdoConnection::Disconnect()
{
	try
	{
		if (m_pConnection != NULL && m_pConnection->State == adStateOpen) 
		{
			if (SUCCEEDED(m_pConnection->Close()))
			{
				return TRUE;
			}
		}
	}
	catch(_com_error e)
	{
		TRACE(_T(":( �ر����ݿⷢ���쳣: %s\n"), e.ErrorMessage());
		return FALSE;
	} 
	return FALSE;
}

/*===================================================================
	Name:	�ر����Ӳ��ͷŶ���.
	-----------------------------------------------------
	Remarks: �ر����Ӳ��ͷ�connection�������������ϴ���������ȫ���
		��connection����
=====================================================================*/
BOOL CAdoConnection::Release()
{
	try
	{
		Disconnect();
		m_pConnection.Release();
		return TRUE;
	}
	catch(_com_error e)
	{
		TRACE(_T(":( Release���������쳣: %s\n"), e.ErrorMessage());
		return FALSE;
	} 
	return TRUE;
}

/*========================================================================
	Remarks:	�뿼�� CAdoRecordSet �� Cancel ����.
==========================================================================*/
BOOL CAdoConnection::Cancel()
{
	ASSERT(m_pConnection != NULL);
	try
	{
		return m_pConnection->Cancel();
	}
	catch (_com_error e)
	{
		TRACE(_T(":( Cancel�����쳣: %s\n"), e.ErrorMessage());
		return FALSE;
	} 
	return FALSE;
}

_ConnectionPtr CAdoConnection::GetConnection()
{
	return m_pConnection;
}

/// ִ��SQL���
BOOL CAdoConnection::ExecSql(const char *v_szSql)
{
	try
	{
		// ִ�����ݿ����---------------------------------------------
		return SUCCEEDED(m_pConnection->Execute(v_szSql, NULL,adOptionUnspecified));
	}
	catch (_com_error e)
	{
		TRACE(_T(":( ִ�����ݿ���������쳣: %s\n"), e.ErrorMessage());
		return FALSE;
	} 
	catch (...)
	{
		TRACE(_T(":( ִ�����ݿ����ʱ����δ֪����:"));
	}
	return FALSE;
}

/*===================================================================
	Name:	ȡ��������Ĵ�����Ϣ.
	-----------------------------------------------------
	Remarks: �κ��漰 ADO ����Ĳ��������Բ���һ�������ṩ�ߴ��󡣲�����
		��ʱ,���Խ�һ������ Error �������� Connection ����� Errors ����
		�С����� ADO ������������ʱ������� Errors ���ϣ����ҽ��µ� Error 
		�������� Errors �����С� 
			ÿ�� Error ��������ض����ṩ�ߴ��󣬶����� ADO ����ADO ����
		������������ʱ���쳣��������С�
			û�в�������� ADO ������ Errors ����û��Ӱ�졣ʹ�� Clear ��ʽ
		���ֹ���� Errors ���ϡ�
			Errors �����е� Error ���󼯺�������Ӧ�������ʱ���������д���
		�о� Errors ������ָ�������ʹ���������̸���ȷ��ȷ�����������ԭ
		�򼰴�����Դ����ִ���ʵ���ԭ���衣
			ĳЩ���Ժͷ�����������Ϊ Errors �����е� Error ������ʾ�ľ��棬
		��������ֹ�����ִ�С��ڵ��� Recordset �����ϵ� Resync��UpdateBatch
		�� CancelBatch ������Connection �� Open ������������ Recordset ����
		�ϵ� Filter ����ǰ������� Errors �����ϵ� Clear �������������Ϳ���
		�Ķ� Errors ���ϵ� Count �����Բ��Է��صľ��档
=====================================================================*/
CString CAdoConnection::GetLastError(DWORD* v_pdwError)
{
	ASSERT(m_pConnection != NULL);
	CString strErrors = "";
	try
	{
		if (m_pConnection != NULL)
		{
			ErrorsPtr pErrors = m_pConnection->Errors;
			CString strError;
			for (long n = 0; n < pErrors->Count; n++)
			{
				ErrorPtr pError = pErrors->GetItem(n);
				strError.Format(_T("Description: %s\r\n\r\nState: %s, Native: %d, Number=%08x, Source: %s\r\n"),
								(LPCTSTR)pError->Description,
								(LPCTSTR)pError->SQLState,
										 pError->NativeError,
										 pError->Number,
								(LPCTSTR)pError->Source);
				strErrors += strError;

				if( NULL!=v_pdwError )
				{
					*v_pdwError = pError->NativeError;
				}
			}
		}
		return strErrors;
	}
	catch(_com_error e)
	{
		TRACE(_T(":( GetLastError�����쳣: %s\n"), e.ErrorMessage());
		return strErrors;
	} 
	return strErrors;
}

ErrorsPtr CAdoConnection::GetErrors()
{
	ASSERT(m_pConnection != NULL);
	try
	{
		if (m_pConnection != NULL)
		{
			return m_pConnection->Errors;
		}
		return NULL;
	}
	catch(_com_error e)
	{
		TRACE(_T(":( GetLastError�����쳣: %s\n"), e.ErrorMessage());
		return NULL;
	} 
	return NULL;
}

BOOL CAdoConnection::IsOpen()
{
	try
	{
		long dwState = m_pConnection->State;
		return (m_pConnection != NULL && (dwState & adStateOpen));
	}
	catch (_com_error e)
	{
		TRACE(_T(":( IsOpen���������쳣: %s\n"), e.ErrorMessage());
		return FALSE;
	} 
	return FALSE;
}

BOOL CAdoConnection::SetConnectTimeOut(long lTime)
{
	ASSERT(m_pConnection != NULL);
	try
	{
		return SUCCEEDED(m_pConnection->put_ConnectionTimeout(lTime));
	}
	catch (_com_error e)
	{
		TRACE(_T(":( SetConnectTimeOut���������쳣: %s\n"), e.ErrorMessage());
		return FALSE;
	} 
	return FALSE;
}


/*========================================================================
    Name:		������Դ��ȡ���ݿ���Ϣ.
	-----------------------------------------------------
	Params:		QueryType  ��Ҫ���е�ģʽ��ѯ����,����Ϊ�������ⳣ��.
	adSchemaAsserts				CONSTRAINT_CATALOG
								CONSTRAINT_SCHEMA
								CONSTRAINT_NAME 
	adSchemaCatalogs			CATALOG_NAME 
	adSchemaCharacterSets		CHARACTER_SET_CATALOG
								CHARACTER_SET_SCHEMA
								CHARACTER_SET_NAME 
	adSchemaCheckConstraints	CONSTRAINT_CATALOG
								CONSTRAINT_SCHEMA
								CONSTRAINT_NAME 
	adSchemaCollations			 COLLATION_CATALOG
								COLLATION_SCHEMA
								COLLATION_NAME 
	adSchemaColumnDomainUsage	DOMAIN_CATALOG
								DOMAIN_SCHEMA
								DOMAIN_NAME
								COLUMN_NAME 
	adSchemaColumnPrivileges	TABLE_CATALOG
								TABLE_SCHEMA
								TABLE_NAME
								COLUMN_NAME
								GRANTOR
								GRANTEE 
	adSchemaColumns				TABLE_CATALOG
								TABLE_SCHEMA
								TABLE_NAME
								COLUMN_NAME 
	adSchemaConstraintColumnUsage TABLE_CATALOG
								TABLE_SCHEMA
								TABLE_NAME
								COLUMN_NAME 
	adSchemaConstraintTableUsage TABLE_CATALOG
								TABLE_SCHEMA
								TABLE_NAME 
	adSchemaForeignKeys			PK_TABLE_CATALOG
								PK_TABLE_SCHEMA
								PK_TABLE_NAME
								FK_TABLE_CATALOG
								FK_TABLE_SCHEMA
								FK_TABLE_NAME 
	adSchemaIndexes				TABLE_CATALOG
								TABLE_SCHEMA
								INDEX_NAME
								TYPE
								TABLE_NAME 
	adSchemaKeyColumnUsage		CONSTRAINT_CATALOG
								CONSTRAINT_SCHEMA
								CONSTRAINT_NAME
								TABLE_CATALOG
								TABLE_SCHEMA
								TABLE_NAME
								COLUMN_NAME 
	adSchemaPrimaryKeys			PK_TABLE_CATALOG
								PK_TABLE_SCHEMA
								PK_TABLE_NAME 
	adSchemaProcedureColumns	PROCEDURE_CATALOG
								PROCEDURE_SCHEMA
								PROCEDURE_NAME
								COLUMN_NAME 
	adSchemaProcedureParameters PROCEDURE_CATALOG
								PROCEDURE_SCHEMA
								PROCEDURE_NAME
								PARAMTER_NAME 
	adSchemaProcedures			PROCEDURE_CATALOG
								PROCEDURE_SCHEMA
								PROCEDURE_NAME
								PROCEDURE_TYPE 
	adSchemaProviderSpecific	 �μ�˵�� 
	adSchemaProviderTypes		DATA_TYPE
								BEST_MATCH 
	adSchemaReferentialConstraints CONSTRAINT_CATALOG
								CONSTRAINT_SCHEMA
								CONSTRAINT_NAME 
	adSchemaSchemata			CATALOG_NAME
								SCHEMA_NAME
								SCHEMA_OWNER 
	adSchemaSQLLanguages		<��> 
	adSchemaStatistics			TABLE_CATALOG
								TABLE_SCHEMA
								TABLE_NAME 
	adSchemaTableConstraints	CONSTRAINT_CATALOG
								CONSTRAINT_SCHEMA
								CONSTRAINT_NAME
								TABLE_CATALOG
								TABLE_SCHEMA
								TABLE_NAME
								CONSTRAINT_TYPE 
	adSchemaTablePrivileges		TABLE_CATALOG
								TABLE_SCHEMA
								TABLE_NAME
								GRANTOR
								GRANTEE 
	adSchemaTables				TABLE_CATALOG
								TABLE_SCHEMA
								TABLE_NAME
								TABLE_TYPE 
	adSchemaTranslations		TRANSLATION_CATALOG
								TRANSLATION_SCHEMA
								TRANSLATION_NAME 
	adSchemaUsagePrivileges		OBJECT_CATALOG
								OBJECT_SCHEMA
								OBJECT_NAME
								OBJECT_TYPE
								GRANTOR
								GRANTEE 
	adSchemaViewColumnUsage		VIEW_CATALOG
								VIEW_SCHEMA
								VIEW_NAME 
	adSchemaViewTableUsage		VIEW_CATALOG
								VIEW_SCHEMA
								VIEW_NAME 
	adSchemaViews				TABLE_CATALOG
								TABLE_SCHEMA
								TABLE_NAME 
	-----------------------------------------------------
	returns:	���ذ������ݿ���Ϣ�� Recordset ����. Recordset ����ֻ������̬
			�α��.
	-----------------------------------------------------
	Remarks:	OpenSchema��������������Դ�йص���Ϣ��������ڷ������ϵı�
			�Լ����е��е���Ϣ, �������ݽ����ο�, �Ӿ��������Դ���ܻ��в�ͬ��
==========================================================================*/
_RecordsetPtr CAdoConnection::OpenSchema(SchemaEnum QueryType)
{
	ASSERT(m_pConnection != NULL);
	try
	{
		return m_pConnection->OpenSchema(QueryType, vtMissing, vtMissing);
	}
	catch(_com_error e)
	{
		TRACE(_T(":( OpenSchema���������쳣: %s\n"), e.ErrorMessage());
		return NULL;
	} 
	return NULL;
}

/*########################################################################
			  ------------------------------------------------
									������
			  ------------------------------------------------
	BeginTrans		- ��ʼ������
	CommitTrans		- �����κθ��Ĳ�������ǰ������Ҳ��������������
	RollbackTrans	- ȡ����ǰ�������������κθ��Ĳ�����������Ҳ��������
					������

		һ�������� BeginTrans �������ڵ��� CommitTrans �� RollbackTrans ��
	������֮ǰ�����ݿ⽫���������ύ�������κθ��ġ�
		����֧��Ƕ����������ݿ���˵�����Ѵ򿪵������е��� BeginTrans ����
	����ʼ�µ�Ƕ�����񡣷���ֵ��ָʾǶ�ײ�Σ�����ֵΪ 1 ��ʾ�Ѵ򿪶�������
	(�����񲻱���һ��������Ƕ��)������ֵΪ 2 ��ʾ�Ѵ򿪵ڶ�������(Ƕ��
	�ڶ��������е�����)���������ơ����� CommitTrans �� RollbackTrans ֻӰ
	�����´򿪵������ڴ����κθ��߲�����֮ǰ����رջ�ؾ�ǰ����
		���� CommitTrans ���������������ϴ򿪵������������ĸ��Ĳ���������
	���� RollbackTrans������ԭ�������������ĸ��Ĳ�����������δ������
	ʱ���������κ�һ�ַ���������������
		ȡ���� Connection ����� Attributes ���ԣ����� CommitTrans �� 
	RollbackTrans �����������Զ�������������� Attributes ��������Ϊ 
	adXactCommitRetaining�����ݿ��� CommitTrans ���ú���Զ�����������
	��� Attributes ��������Ϊ adXactAbortRetaining�����ݿ��ڵ��� 
	RollbackTrans ֮���Զ�����������
  ########################################################################*/
long CAdoConnection::BeginTrans()
{
	ASSERT(m_pConnection != NULL);
	try
	{
		return m_pConnection->BeginTrans();
	}
	catch (_com_error e)
	{
		TRACE(_T(":( BeginTrans�����쳣: %s\n"), e.ErrorMessage());
		return -1;
	} 
	return -1;
}

BOOL CAdoConnection::CommitTrans()
{
	ASSERT(m_pConnection != NULL);
	try
	{
		return SUCCEEDED(m_pConnection->CommitTrans());
	}
	catch (_com_error e)
	{
		TRACE(_T(":( CommitTrans�����쳣: %s\n"), e.ErrorMessage());
		return FALSE;
	} 
	return FALSE;
}

BOOL CAdoConnection::RollbackTrans()
{
	ASSERT(m_pConnection != NULL);
	try
	{
		return SUCCEEDED(m_pConnection->RollbackTrans());
	}
	catch (_com_error e)
	{
		TRACE(_T(":( RollbackTrans�����쳣: %s\n"), e.ErrorMessage());
		return FALSE;
	} 
	return FALSE;
}

/*########################################################################
			------------------------------------------------
					 CAdoRecordSet class ���캯��
			------------------------------------------------
  ########################################################################*/

CAdoRecordSet::CAdoRecordSet()
{
	m_pConnection = NULL;
	m_pRecordset.CreateInstance("ADODB.Recordset");
	ASSERT(m_pRecordset != NULL);
}

CAdoRecordSet::CAdoRecordSet(CAdoConnection *pConnection)
{
	m_pConnection = pConnection;
	ASSERT(m_pConnection != NULL);
	m_pRecordset.CreateInstance("ADODB.Recordset");
	ASSERT(m_pRecordset != NULL);
}

CAdoRecordSet::~CAdoRecordSet()
{
	Release();
}

/*========================================================================
	Params:		
		- strSQL:		���� Command ����ı�����,SQL ���,����,�洢���̵�
					�û�־� Recordset �ļ���.
		- CursorType:   ��ѡ.CursorTypeEnum ֵ, ȷ����RecordsetʱӦ
					��ʹ�õ��α�����.��Ϊ���г���֮һ.
		����				˵�� 
		-----------------------------------------------
		adOpenForwardOnly	�򿪽���ǰ�����α�. 
		adOpenKeyset		�򿪼��������α�. 
		adOpenDynamic		�򿪶�̬�����α�. 
		adOpenStatic		�򿪾�̬�����α�. 
		-----------------------------------------------
		- LockType:     ��ѡ,ȷ����RecordsetʱӦ��ʹ�õ���������(����)��
					LockTypeEnumֵ,��Ϊ���г���֮һ.
		����				˵�� 
		-----------------------------------------------
		adLockReadOnly		ֻ�� �� ���ܸı�����. 
		adLockPessimistic	����ʽ���� - ͨ��ͨ���ڱ༭ʱ������������Դ�ļ�¼. 
		adLockOptimistic	����ʽ���� - ֻ�ڵ��� Update ����ʱ��������¼. 
		adLockBatchOptimistic ����ʽ������ - ����������ģʽ(����������ģʽ
						���). 
		-----------------------------------------------
		- lOption		��ѡ. ������ֵ, ����ָʾ strSQL ���������Ϳ�Ϊ����
					����֮һ.
		����				˵�� 
		-------------------------------------------------
		adCmdText			ָʾstrSQLΪ�����ı�, ����ͨ��SQL���. 
		adCmdTable			ָʾADO����SQL��ѯ�Ա����strSQL�������ı��з�
						��������. 
		adCmdTableDirect	ָʾ�����ĸ�����strSQL�������ı��з���������. 
		adCmdStoredProc		ָʾstrSQLΪ�洢����. 
		adCmdUnknown		ָʾstrSQL�����е���������Ϊδ֪. 
		adCmdFile			ָʾӦ����strSQL���������ļ��лָ�����(�����)
						Recordset. 
		adAsyncExecute		ָʾӦ�첽ִ��strSQL. 
		adAsyncFetch		ָʾ����ȡ Initial Fetch Size ������ָ���ĳ�ʼ
						������,Ӧ���첽��ȡ����ʣ�����.������������δ��
						ȡ,��Ҫ���߳̽�������ֱ�������¿���. 
		adAsyncFetchNonBlocking ָʾ��Ҫ�߳�����ȡ�ڼ��δ����. ���������
						������δ��ȡ,��ǰ���Զ��Ƶ��ļ�ĩβ. 
==========================================================================*/
HRESULT CAdoRecordSet::Open(LPCTSTR strSQL, long lOption, CursorTypeEnum CursorType, LockTypeEnum LockType)
{
	ASSERT(m_pConnection != NULL);
	ASSERT(m_pRecordset != NULL);

	try
	{
		if (m_pConnection == NULL)
		{
			return -1;
		}
		Close();
		m_pRecordset->Open(_bstr_t(strSQL), _variant_t((IDispatch*)m_pConnection->GetConnection(), true),
							CursorType, LockType, lOption);
		if (m_pRecordset == NULL) 
		{
			return -1;
		}
		return (m_pRecordset->adoEOF) ? 0 : 1;
	}
	catch (_com_error e)
	{
		TRACE(_T(":( �򿪼�¼�������쳣: %s\n"), e.ErrorMessage());
		return -1;
	}
}

/*===================================================================
	Name:		�رմ򿪵Ķ����κ���ض���.
	-----------------------------------------------------
	Remarks:	ʹ��Close�����ɹر�Recordset�����Ա��ͷ����й�����ϵͳ��Դ.
		�رն��󲢷ǽ������ڴ���ɾ��, ���Ը��������������ò����ڴ˺��ٴδ�
		��.Ҫ��������ڴ�����ȫɾ��, �ɽ������������Ϊ Nothing.
		    ʹ��Close�����ر�Recordset�����ͬʱ, ���ͷŹ��������ݺͿ�����
		��ͨ�����ض�Recordset��������ݽ��еĶ�������.���ɵ���Open������
		�´򿪾�����ͬ���Ի����޸����Ե�Recordset.��Recordset����رպ�,��
		���κ���Ҫ��α�ķ�������������.
			���������������ģʽ�½��б༭,����Close��������������, Ӧ����
		����Update��CancelUpdat ����.������������ڼ�ر�Recordset����, ��
		���ϴ�UpdateBatch���������������޸Ľ�ȫ����ʧ.
			���ʹ��Clone���������Ѵ򿪵�Recordset����ĸ���,�ر�ԭʼRecordset
		���丱������Ӱ���κ���������.
			Ҫ��������ڴ�����ȫɾ���������Release������
=====================================================================*/
BOOL CAdoRecordSet::Close()
{
	try
	{
		if (m_pRecordset != NULL && m_pRecordset->State != adStateClosed)
		{
			return SUCCEEDED(m_pRecordset->Close());
		}
		return TRUE;
	}
	catch (const _com_error& e)
	{
		TRACE(_T(":( �رռ�¼�������쳣: %s\n"), e.ErrorMessage());
		return FALSE;
	}

	return TRUE;
}

/*===================================================================
	Name:	�ر����Ӳ��ͷŶ���.
	-----------------------------------------------------
	Remarks: �ر����Ӳ��ͷ�CAdoRecordSet�������������ϴ���������ȫ���
		��CAdoRecordSet����
=====================================================================*/
BOOL CAdoRecordSet::Release()
{
	try
	{
		if (m_pRecordset != NULL)
		{
			Close();
			m_pRecordset.Release();
		}
		return TRUE;
	}
	catch (const _com_error& e)
	{
		TRACE(_T(":( Release���������쳣: %s\n"), e.ErrorMessage());
		return FALSE;
	}
	return TRUE;
}

/*########################################################################
			  ------------------------------------------------
							   ��¼�����²���
			  ------------------------------------------------
  ########################################################################*/

/*===================================================================
	Remarks:	����µļ�¼.��AddNew()����, Ȼ����PutCollect()����ÿ
				�е�ֵ, �����Update()�������ݿ�.
=====================================================================*/
BOOL CAdoRecordSet::AddNew()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (m_pRecordset != NULL) 
		{
			return SUCCEEDED(m_pRecordset->AddNew());
		}
	}
	catch (_com_error e)
	{
		TRACE(_T(":( AddNew�����쳣: %s\n"), e.ErrorMessage());
		return FALSE;
	} 
	return	FALSE;
}

/*===================================================================
	Remarks:	�ڵ���AddNew()�ȷ����󣬵��ô˷�����ɸ��»��޸�.
=====================================================================*/
BOOL CAdoRecordSet::Update()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (m_pRecordset != NULL) 
		{
			return SUCCEEDED(m_pRecordset->Update());
		}
	}
	catch (_com_error e)
	{
		TRACE(_T(":( Update�����쳣: %s\n"), e.ErrorMessage());
		return FALSE;
	} 
	return	FALSE;
}

/*========================================================================
	Name:		ȡ���ڵ��� Update ����ǰ�Ե�ǰ��¼���¼�¼�������κθ���.
	-----------------------------------------------------
	Remarks:	ʹ�� CancelUpdate ������ȡ���Ե�ǰ��¼�������κθ��Ļ�
	��������ӵļ�¼.�ڵ��� Update �������޷������Ե�ǰ��¼���¼�¼
	�����ĸ���,���Ǹ����ǿ����� RollbackTrans �����ؾ�������һ����,
	�����ǿ����� CancelBatch ����ȡ���������µ�һ����.
	����ڵ��� CancelUpdate ����ʱ����¼�¼,����� AddNew ֮ǰ�ĵ�ǰ
	��¼���ٴγ�Ϊ��ǰ��¼.
	�����δ���ĵ�ǰ��¼������¼�¼,���� CancelUpdate ��������������.
==========================================================================*/
BOOL CAdoRecordSet::CancelUpdate()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (m_pRecordset != NULL) 
		{
			return SUCCEEDED(m_pRecordset->CancelUpdate());
		}
	}
	catch (_com_error e)
	{
		TRACE(_T(":( CancelUpdate�����쳣: %s\n"), e.ErrorMessage());
		return FALSE;
	} 
	return FALSE;
}

/*========================================================================
	Name:		ȡ�������������.
	-----------------------------------------------------
	Params:		AffectRecords   ��ѡ�� AffectEnum ֵ,����CancelBatch 
		������Ӱ���¼����Ŀ,��Ϊ���г���֮һ��
		����			˵�� 
		-------------------------------------------------
		AdAffectCurrent ��ȡ����ǰ��¼�Ĺ������. 
		AdAffectGroup	�����㵱ǰ Filter �������õļ�¼ȡ���������.
						ʹ�ø�ѡ��ʱ,���뽫 Filter ��������Ϊ�Ϸ���Ԥ
						���峣��֮һ. 
		AdAffectAll		Ĭ��ֵ.ȡ�� Recordset ���������м�¼�Ĺ����
						��,�����ɵ�ǰ Filter �������������ص��κμ�¼. 
==========================================================================*/
BOOL CAdoRecordSet::CancelBatch(AffectEnum AffectRecords)
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (m_pRecordset != NULL) 
		{
			return SUCCEEDED(m_pRecordset->CancelBatch(AffectRecords));
		}
	}
	catch(_com_error e)
	{
		TRACE(_T(":( CancelBatch�����쳣: %s\n"), e.ErrorMessage());
		return FALSE;
	} 
	return FALSE;
}

/*========================================================================
	Params:		AffectRecords:  AffectEnumֵ, ȷ��Delete������Ӱ��ļ�¼��
		Ŀ,��ֵ���������г���֮һ.
		����				˵�� 
		-------------------------------------------------
		AdAffectCurrent		Ĭ��.��ɾ����ǰ��¼. 
		AdAffectGroup		ɾ�����㵱ǰFilter�������õļ�¼.Ҫʹ�ø�ѡ��,
						���뽫 Filter ��������Ϊ��Ч��Ԥ���峣��֮һ. 
		adAffectAll			ɾ�����м�¼. 
		adAffectAllChapters ɾ�������Ӽ���¼. 
==========================================================================*/
BOOL CAdoRecordSet::Delete(AffectEnum AffectRecords)
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (m_pRecordset != NULL) 
		{
			return SUCCEEDED(m_pRecordset->Delete(AffectRecords));
		}
	}
	catch(_com_error e)
	{
		TRACE(_T(":( Delete�����쳣: %s\n"), e.ErrorMessage());
		return FALSE;
	} 
	return	FALSE;
}

/*########################################################################
			  ------------------------------------------------
								��¼����������
			  ------------------------------------------------
    Remarks:	
		ʹ��MoveFirst��������ǰ��¼λ���ƶ���Recordse�еĵ�һ����¼.
		ʹ��MoveLast��������ǰ��¼λ���ƶ���Recordset�е����һ����¼.
	Recordset�������֧����ǩ��������ƶ�;������ø÷�������������.

		ʹ��MoveNext��������ǰ��¼��ǰ�ƶ�һ����¼(��Recordset�ĵײ�).���
	���һ����¼�ǵ�ǰ��¼���ҵ���MoveNext����,��ADO����ǰ��¼���õ�
	Recordset(EOFΪTrue)��β��¼֮��.��EOF�����Ѿ�ΪTrueʱ��ͼ��ǰ�ƶ�
	����������.
		ʹ��MovePrevious��������ǰ��¼λ������ƶ�һ����¼(���¼���Ķ���).
	Recordset �������֧����ǩ������α��ƶ�;���򷽷����ý���������.
	����׼�¼�ǵ�ǰ��¼���ҵ���MovePrevious����,��ADO����ǰ��¼������
	Recordset(BOFΪTrue)���׼�¼֮ǰ.��BOF����ΪTrueʱ����ƶ���������
	��. ���Recordset����֧����ǩ������α��ƶ�,��MovePrevious������
	��������.
		�����¼���ǽ���ǰ��,�����û�ϣ��֧����ǰ��������,�����ʹ��
	CacheSize ���Դ�����¼����,ͨ�� Move����֧������α��ƶ�.����
	�����¼�Ǽ��ص��ڴ��е�,����Ӧ���ⲻ��Ҫ�ػ���̫���¼.���Ե���
	����ǰ Recordset ����� MoveFirst ����;��������ʹ�ṩ������ִ����
	�� Recordset ���������.
  ########################################################################*/

BOOL CAdoRecordSet::MoveFirst()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (m_pRecordset != NULL) 
		{
			return SUCCEEDED(m_pRecordset->MoveFirst());
		}
	}
	catch (_com_error e)
	{
		TRACE(_T(":( MoveFirst�����쳣: %s\n"), e.ErrorMessage());
		return FALSE;
	} 
	return	FALSE;
}

BOOL CAdoRecordSet::MoveLast()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (m_pRecordset != NULL) 
		{
			return SUCCEEDED(m_pRecordset->MoveLast());
		}
	}
	catch (_com_error e)
	{
		TRACE(_T(":( MoveLast�����쳣: %s\n"), e.ErrorMessage());
		return FALSE;
	} 
	return	FALSE;
}

BOOL CAdoRecordSet::MovePrevious()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (m_pRecordset != NULL) 
		{
			return SUCCEEDED(m_pRecordset->MovePrevious());
		}
	}
	catch (_com_error e)
	{
		TRACE(_T(":( MovePrevious�����쳣: %s\n"), e.ErrorMessage());
		return FALSE;
	}
	return FALSE;	
}

BOOL CAdoRecordSet::MoveNext()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (m_pRecordset != NULL) 
		{
			return SUCCEEDED(m_pRecordset->MoveNext());
		}
	}
	catch (_com_error e)
	{
		TRACE(_T(":( MoveNext�����쳣: %s\n"), e.ErrorMessage());
		return FALSE;
	}
	return FALSE;
}

/*========================================================================
	Name:		�ƶ� Recordset �����е�ǰ��¼��λ�á�
    ----------------------------------------------------------
	Params:		
		- lRecords    �����ų����ͱ��ʽ��ָ����ǰ��¼λ���ƶ��ļ�¼����
		- Start    ��ѡ���ַ���������ͣ����ڼ�����ǩ��Ҳ��Ϊ���� BookmarkEnum ֵ֮һ��
		����				˵�� 
		adBookmarkCurrent	Ĭ�ϡ��ӵ�ǰ��¼��ʼ�� 
		adBookmarkFirst		���׼�¼��ʼ�� 
		AdBookmarkLast		��β��¼��ʼ�� 
==========================================================================*/
BOOL CAdoRecordSet::Move(long lRecords, long Start)
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (m_pRecordset != NULL) 
		{
			return SUCCEEDED(m_pRecordset->Move(lRecords, _variant_t(Start)));
		}
	}
	catch (_com_error e)
	{
		TRACE(_T(":( Move�����쳣: %s\n"), e.ErrorMessage());
		return FALSE;
	} 
	return	FALSE;
}

/*########################################################################
			  ------------------------------------------------
								  ��¼������
			  ------------------------------------------------
  ########################################################################*/

/*========================================================================
	Name:		�����п�Ӧ�ö���,˵�������״̬�Ǵ򿪻��ǹر�.��ִ��
	�첽������ Recordset ����,˵����ǰ�Ķ���״̬�����ӡ�ִ�л��ǻ�ȡ.
	-----------------------------------------------------
	returns:	�������г���֮һ�ĳ�����ֵ.
		����				˵�� 
		----------------------------------
		adStateClosed		ָʾ�����ǹرյ�. 
		adStateOpen			ָʾ�����Ǵ򿪵�. 
		adStateConnecting	ָʾ Recordset ������������. 
		adStateExecuting	ָʾ Recordset ��������ִ������. 
		adStateFetching		ָʾ Recordset ����������ڱ���ȡ. 
	-----------------------------------------------------
	Remarks:		������ʱʹ�� State ����ȷ��ָ������ĵ�ǰ״̬.��
	������ֻ����.Recordset ����� State ���Կ��������ֵ.����,���
	����ִ�����,�����Խ��� adStateOpen �� adStateExecuting �����ֵ.
==========================================================================*/
long CAdoRecordSet::GetState()
{
	ASSERT(m_pRecordset != NULL);
	long lState = 0;
	try
	{
		if (SUCCEEDED(m_pRecordset->get_State(&lState)))
		{
			return lState;
		}
	}
	catch (_com_error e)
	{
		TRACE(_T(":( GetState�����쳣: %s\n"), e.ErrorMessage());
		return NULL;
	} 
	return NULL;
}

/*========================================================================
	Name:		ָʾ�й������»��������������ĵ�ǰ��¼��״̬.
	-----------------------------------------------------
	returns:	��������һ������ RecordStatusEnum ֵ֮��.
		����						˵�� 
		-------------------------------------------------
		adRecOK						�ɹ��ظ��¼�¼. 
		adRecNew					��¼���½���. 
		adRecModified				��¼���޸�. 
		adRecDeleted				��¼��ɾ��. 
		adRecUnmodified				��¼û���޸�. 
		adRecInvalid				������ǩ��Ч,��¼û�б���. 
		adRecMultipleChanges		����Ӱ������¼,��˼�¼δ������. 
		adRecPendingChanges			���ڼ�¼���ù���Ĳ���,���δ������. 
		adRecCanceled				���ڲ�����ȡ��,δ�����¼. 
		adRecCantRelease			�������м�¼����,û�б����¼�¼. 
		adRecConcurrencyViolation	���ڿ���ʽ������ʹ����,��¼δ������. 
		adRecIntegrityViolation		�����û�Υ��������Լ��,��¼δ������. 
		adRecMaxChangesExceeded		���ڴ��ڹ���������,��¼δ������. 
		adRecObjectOpen				������򿪵Ĵ�������ͻ,��¼δ������. 
		adRecOutOfMemory			���ڼ�����ڴ治��,��¼δ������. 
		adRecPermissionDenied		�����û�û���㹻��Ȩ��,��¼δ������. 
		adRecSchemaViolation		���ڼ�¼Υ���������ݿ�Ľṹ,���δ������. 
		adRecDBDeleted				��¼�Ѿ�������Դ��ɾ��. 
	-----------------------------------------------------
	Remarks:	ʹ�� Status ���Բ鿴���������б��޸ĵļ�¼����Щ���ı�
	����.Ҳ��ʹ�� Status ���Բ鿴��������ʱʧ�ܼ�¼��״̬.����,���� 
	Recordset ����� Resync��UpdateBatch �� CancelBatch ����,�������� 
	Recordset ����� Filter ����Ϊ��ǩ����.ʹ�ø�����,�ɼ��ָ����¼
	Ϊ��ʧ�ܲ���������.
==========================================================================*/
long CAdoRecordSet::GetStatus()
{
	ASSERT(m_pRecordset != NULL);
	long lStatus = 0;
	try
	{
		if (SUCCEEDED(m_pRecordset->get_Status(&lStatus)))
		{
			return lStatus;
		}
	}
	catch (_com_error e)
	{
		TRACE(_T(":( GetStatus�����쳣: %s\n"), e.ErrorMessage());
		return NULL;
	} 
	return NULL;
}

/*========================================================================
	Name:		��ȡ��ǰ��¼���м�¼��Ŀ
==========================================================================*/
long CAdoRecordSet::GetRecordCount()
{
	ASSERT(m_pRecordset != NULL);
	long lcount;
	try
	{
		m_pRecordset->get_RecordCount(&lcount);
		return lcount;
	}
	catch(_com_error e)
	{
		TRACE(_T(":( GetRecordCount�����쳣: %s\n"), e.ErrorMessage());
		return NULL;
	} 
}

/*========================================================================
	Name:		��ȡ��ǰ��¼�����ֶ���Ŀ
==========================================================================*/
long CAdoRecordSet::GetFieldsCount()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		return GetFields()->Count;
	}
	catch(_com_error e)
	{
		TRACE(_T(":( GetFieldsCount�����쳣: %s\n"), e.ErrorMessage());
		return -1;
	} 
}

/*========================================================================
	Name:		ָ���Ƿ����ڼ�¼��ͷ
==========================================================================*/
BOOL CAdoRecordSet::IsBOF()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		return m_pRecordset->adoBOF;
	}
	catch(_com_error e)
	{
		TRACE(_T(":( IsBOF�����쳣: %s\n"), e.ErrorMessage());
		return FALSE;
	} 
	return FALSE;
}

/*========================================================================
	Name:		ָ���Ƿ����ڼ�¼��β
==========================================================================*/
BOOL CAdoRecordSet::IsEOF()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		return m_pRecordset->adoEOF;
	}
	catch (_com_error e)
	{
		TRACE(_T(":( IsEOF�����쳣: %s\n"), e.ErrorMessage());
		return FALSE;
	}
}

void CAdoRecordSet::SetAdoConnection(CAdoConnection *pConnection)
{
	m_pConnection = pConnection;
}

/*===================================================================
	Remarks:	Recordset ������� Field ������ɵ� Fields ����.ÿ�� 
	Field �����Ӧ Recordset ���е�һ��.
=====================================================================*/
Fields* CAdoRecordSet::GetFields()
{
	ASSERT(m_pRecordset != NULL);
	FieldsPtr pFields;
	try
	{
		if (SUCCEEDED(m_pRecordset->get_Fields(&pFields)))
		{
			return pFields;
		}
	}
	catch (_com_error e)
	{
		TRACE(_T(":( GetFields�����쳣: %s\n"), e.ErrorMessage());
		return NULL;
	} 
	return NULL;
}

/*========================================================================
	Remarks:	ȡ��ָ�����ֶε��ֶ���.
==========================================================================*/
CString CAdoRecordSet::GetFieldName(long lIndex)
{
	ASSERT(m_pRecordset != NULL);
	CString strFieldName;
	try
	{
		FieldPtr field = GetFields()->GetItem(_variant_t(lIndex));
		strFieldName = LPCTSTR(_bstr_t(field->GetName()));
		return strFieldName;
		field->GetType();
	}
	catch (_com_error e)
	{
		TRACE(_T(":( GetFieldName�����쳣: %s\n"), e.ErrorMessage());
		return strFieldName;
	}
}

/*========================================================================
	returns:	���� Field ����Attributes ����Ϊֻ������ֵ����Ϊ��������һ������ FieldAttributeEnum ֵ�ĺ͡�
	  ����				˵�� 
	  adFldMayDefer			ָʾ�ֶα��ӳ٣�������ӵ��������¼������Դ����
						�ֶ�ֵ��������ʽ������Щ�ֶ�ʱ�Ž��м����� 
	  adFldUpdatable		ָʾ����д����ֶΡ� 
	  adFldUnknownUpdatable ָʾ�ṩ���޷�ȷ���Ƿ����д����ֶΡ� 
	  adFldFixed			ָʾ���ֶΰ����������ݡ� 
	  adFldIsNullable		ָʾ���ֶν��� Null ֵ�� 
	  adFldMayBeNull		ָʾ���ԴӸ��ֶζ�ȡ Null ֵ�� 
	  adFldLong				ָʾ���ֶ�Ϊ���������ֶΡ���ָʾ����ʹ�� AppendChunk �� GetChunk ������ 
	  adFldRowID			ָʾ�ֶΰ����־õ��б�ʶ�����ñ�ʶ���޷���д��
						���ҳ��˶��н��б�ʶ(���¼�š�Ψһ��ʶ����)�ⲻ
						�����������ֵ�� 
	  adFldRowVersion		ָʾ���ֶΰ����������ٸ��µ�ĳ��ʱ������ڱ�ǡ� 
	  adFldCacheDeferred	ָʾ�ṩ�߻������ֶ�ֵ������������Ի���Ķ�ȡ�� 
    ----------------------------------------------------------
	Remarks:	ȡ�� Field ����һ���������ԡ�
==========================================================================*/
long CAdoRecordSet::GetFieldAttributes(long lIndex)
{
	ASSERT(m_pRecordset != NULL);
	long lAttributes;
	try
	{
		FieldPtr field = GetFields()->GetItem(_variant_t(lIndex));
		field->get_Attributes(&lAttributes);
		return lAttributes;
	}
	catch (_com_error e)
	{
		TRACE(_T(":( GetFieldAttributes�����쳣: %s\n"), e.ErrorMessage());
		return -1;
	}
}
/*========================================================================
	Name:		ָʾ Field ����������ĳ��ȡ�
    ----------------------------------------------------------
	returns:	����ĳ���ֶζ���ĳ���(���ֽ���)�ĳ�����ֵ��
    ----------------------------------------------------------
	Remarks:	ʹ�� DefinedSize ���Կ�ȷ�� Field ���������������
==========================================================================*/

long CAdoRecordSet::GetFieldDefineSize(long lIndex)
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		FieldPtr field = GetFields()->GetItem(_variant_t(lIndex));
		return field->GetDefinedSize();
	}
	catch (_com_error e)
	{
		TRACE(_T(":( GetDefineSize�����쳣: %s\n"), e.ErrorMessage());
		return -1;
	}
}

/*========================================================================
	Name:	ȡ���ֶε�ֵ��ʵ�ʳ��ȡ�
    ----------------------------------------------------------
	returns:	���س�����ֵ��ĳЩ�ṩ���������ø������Ա�Ϊ BLOB ����Ԥ��
			�ռ䣬�ڴ������Ĭ��ֵΪ 0��
    ----------------------------------------------------------
	Remarks:	ʹ�� ActualSize ���Կɷ��� Field ����ֵ��ʵ�ʳ���.��������
			�ֶ�,ActualSize ����Ϊֻ������� ADO �޷�ȷ�� Field ����ֵ��ʵ
			�ʳ��ȣ�ActualSize ���Խ����� adUnknown��
				�����·�����ʾ��ActualSize ��  DefinedSize ����������ͬ��
			adVarChar ������������󳤶�Ϊ 50 ���ַ��� Field ���󽫷���Ϊ 
			50 �� DefinedSize ����ֵ�����Ƿ��ص� ActualSize ����ֵ�ǵ�ǰ��
			¼���ֶ��д洢�����ݵĳ��ȡ�
==========================================================================*/
long CAdoRecordSet::GetFieldActualSize(long lIndex)
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		FieldPtr field = GetFields()->GetItem(_variant_t(lIndex));
		return field->GetActualSize();
	}
	catch (_com_error e)
	{
		TRACE(_T(":( GetFieldActualSize�����쳣: %s\n"), e.ErrorMessage());
		return -1;
	}	
}

/*========================================================================
	returns:	��������ֵ֮һ����Ӧ�� OLE DB ���ͱ�ʶ�����±��˵�����������и�����
	  ����				˵�� 
	  ---------------------------------------------------------
	  adArray			����������һ������߼� OR ��ָʾ���������������͵�
					��ȫ���� (DBTYPE_ARRAY)�� 
	  adBigInt			8 �ֽڴ����ŵ����� (DBTYPE_I8)�� 
	  adBinary			������ֵ (DBTYPE_BYTES)�� 
	  adBoolean			������ֵ (DBTYPE_BOOL)�� 
	  adByRef			����������һ������߼� OR ��ָʾ������������������
					�ݵ�ָ�� (DBTYPE_BYREF)�� 
	  adBSTR			�Կս�β���ַ��� (Unicode) (DBTYPE_BSTR)�� 
	  adChar			�ַ���ֵ (DBTYPE_STR)�� 
	  adCurrency		����ֵ (DBTYPE_CY)���������ֵ�С����λ�ù̶���С��
					���Ҳ�����λ���֡���ֵ����Ϊ 8 �ֽڷ�ΧΪ10,000 �Ĵ���
					������ֵ�� 
	  adDate			����ֵ (DBTYPE_DATE)�����ڰ�˫��������ֵ�����棬��
					��ȫ����ʾ�� 1899 �� 12 �� 30 ��ʼ����������С��������
					һ�쵱�е�Ƭ��ʱ�䡣 
	  adDBDate			����ֵ (yyyymmdd) (DBTYPE_DBDATE)�� 
	  adDBTime			ʱ��ֵ (hhmmss) (DBTYPE_DBTIME)�� 
	  adDBTimeStamp		ʱ��� (yyyymmddhhmmss �� 10 �ڷ�֮һ��С��)(DBTYPE_DBTIMESTAMP). 
	  adDecimal			���й̶����Ⱥͷ�Χ�ľ�ȷ����ֵ (DBTYPE_DECIMAL)�� 
	  adDouble			˫���ȸ���ֵ (DBTYPE_R8)�� 
	  adEmpty			δָ��ֵ (DBTYPE_EMPTY)�� 
	  adError			32 - λ������� (DBTYPE_ERROR)�� 
	  adGUID			ȫ��Ψһ�ı�ʶ�� (GUID) (DBTYPE_GUID)�� 
	  adIDispatch		OLE ������ Idispatch �ӿڵ�ָ�� (DBTYPE_IDISPATCH)�� 
	  adInteger			4 �ֽڵĴ��������� (DBTYPE_I4)�� 
	  adIUnknown		OLE ������ IUnknown �ӿڵ�ָ�� (DBTYPE_IUNKNOWN)��
	  adLongVarBinary	��������ֵ�� 
	  adLongVarChar		���ַ���ֵ�� 
	  adLongVarWChar	�Կս�β�ĳ��ַ���ֵ�� 
	  adNumeric			���й̶����Ⱥͷ�Χ�ľ�ȷ����ֵ (DBTYPE_NUMERIC)�� 
	  adSingle			�����ȸ���ֵ (DBTYPE_R4)�� 
	  adSmallInt		2 �ֽڴ��������� (DBTYPE_I2)�� 
	  adTinyInt			1 �ֽڴ��������� (DBTYPE_I1)�� 
	  adUnsignedBigInt	8 �ֽڲ����������� (DBTYPE_UI8)�� 
	  adUnsignedInt		4 �ֽڲ����������� (DBTYPE_UI4)�� 
	  adUnsignedSmallInt 2 �ֽڲ����������� (DBTYPE_UI2)�� 
	  adUnsignedTinyInt 1 �ֽڲ����������� (DBTYPE_UI1)�� 
	  adUserDefined		�û�����ı��� (DBTYPE_UDT)�� 
	  adVarBinary		������ֵ�� 
	  adVarChar			�ַ���ֵ�� 
	  adVariant			�Զ������� (DBTYPE_VARIANT)�� 
	  adVector			����������һ������߼� OR �У�ָʾ������ DBVECTOR 
					�ṹ(�� OLE DB ����)���ýṹ����Ԫ�صļ������������� 
					(DBTYPE_VECTOR) ���ݵ�ָ�롣 
	  adVarWChar		�Կս�β�� Unicode �ַ����� 
	  adWChar			�Կս�β�� Unicode �ַ��� (DBTYPE_WSTR)�� 
    ----------------------------------------------------------
	Remarks:	����ָ���е��������͡�
==========================================================================*/
DWORD CAdoRecordSet::GetFieldType(long lIndex)
{
	ASSERT(m_pRecordset != NULL);
	try 
	{
		FieldPtr field = GetFields()->GetItem(_variant_t(lIndex));
		return field->GetType();
	}
	catch (_com_error e)
	{
		TRACE(_T(":( GetField�����쳣: %s\n"), e.ErrorMessage());
		return NULL;
	}	
}

DWORD CAdoRecordSet::GetFieldType(LPCTSTR strfield)
{
	ASSERT(m_pRecordset != NULL);
	try 
	{
		FieldPtr field = GetFields()->GetItem(strfield);
		return field->GetType();
	}
	catch (_com_error e)
	{
		TRACE(_T(":( GetField�����쳣: %s\n"), e.ErrorMessage());
		return NULL;
	}	
}
/*========================================================================
	Name:	ȡ��ָ���е��ֶζ����ָ��.	
==========================================================================*/
FieldPtr CAdoRecordSet::GetField(long lIndex)
{
	try
	{
		return GetFields()->GetItem(_variant_t(lIndex));
	}
	catch (_com_error e)
	{
		TRACE(_T(":( GetField�����쳣: %s\n"), e.ErrorMessage());
		return NULL;
	}
}

const _RecordsetPtr& CAdoRecordSet::GetRecordset()
{
	return m_pRecordset;
}

CString CAdoRecordSet::GetLastError()
{
	ASSERT(m_pConnection != NULL);
	return m_pConnection->GetLastError();
}

/*========================================================================
	Name:		ȡ��ִ�й�����첽 Execute �� Open �����ĵ���.
	-----------------------------------------------------
	Remarks:	ʹ�� Cancel ������ִֹ���첽 Execute �� Open ��������(��ͨ
		�� adAsyncConnect��adAsyncExecute �� adAsyncFetch �������õķ���).
		�������ͼ��ֹ�ķ�����û��ʹ�� adAsyncExecute,�� Cancel ����������
		ʱ����.
			�±���ʾ���ض����Ͷ�����ʹ�� Cancel ����ʱ����ֹ������.
		�������Ϊ		����ֹ�Ը÷�������һ���첽���� 
		-------------------------------------------------
		Command			Execute 
		Connection		Execute �� Open 
		Recordset		Open 
==========================================================================*/
BOOL CAdoRecordSet::Cancel()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		return m_pRecordset->Cancel();
	}
	catch (_com_error e)
	{
		TRACE(_T(":( Cancel�����쳣: %s\n"), e.ErrorMessage());
		return FALSE;
	} 
	return FALSE;
}

_RecordsetPtr CAdoRecordSet::operator =(_RecordsetPtr &pRecordSet)
{
	Release();
	m_pRecordset = pRecordSet;
	return m_pRecordset;
}

/*########################################################################
			  ------------------------------------------------
								   ��������
			  ------------------------------------------------
  ########################################################################*/

/*========================================================================
	Remarks:	ȡ�õ�ǰ��¼��ָ���ֶε�ֵ��ת�����ַ���
==========================================================================*/
BOOL CAdoRecordSet::GetValueString(CString& strValue, long lFieldIndex)
{
	ASSERT(m_pRecordset != NULL);
	if (lFieldIndex < 0 || lFieldIndex >= GetFieldsCount())
	{
		return FALSE;
	}
	try
	{
		if (m_pRecordset == NULL || m_pRecordset->State == adStateClosed)
		{
			//MessageBox(NULL,"���ݿ�����Ѿ��Ͽ�,\r\n���������ӡ�Ȼ������.","��ʾ",MB_ICONINFORMATION);
			return FALSE;
		} 
		if (m_pRecordset->adoEOF)
		{
			return FALSE;
		}
		_variant_t value = m_pRecordset->GetCollect(_variant_t(lFieldIndex));
		strValue = Format(value);
		return TRUE;
	}
	catch (_com_error e)
	{
		TRACE(_T(":( �ֶη���ʧ��: %s\n"), e.ErrorMessage());
		return FALSE;
	}

	return FALSE;
}

BOOL CAdoRecordSet::GetValueString(CString &strValue, CString strFieldName)
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (m_pRecordset == NULL || m_pRecordset->State == adStateClosed)
		{
			//MessageBox(NULL,"���ݿ�����Ѿ��Ͽ�,\r\n���������ӡ�Ȼ������.","��ʾ",MB_ICONINFORMATION);
			return FALSE;
		} 
		if (m_pRecordset->adoEOF)
		{
			return FALSE;
		}
		_variant_t value = m_pRecordset->GetCollect(_variant_t(LPCTSTR(strFieldName)));
		strValue = Format(value);
		return TRUE;	
	}
	catch (_com_error e)
	{
		TRACE(_T(":( �ֶη���ʧ��: %s\n"), e.ErrorMessage());
		return FALSE;
	}

	return FALSE/**/;
}

/*========================================================================
	Remarks:	��_variant_t�ͱ�����ʽ����CString���ַ���.
==========================================================================*/
CString CAdoRecordSet::Format(_variant_t var)
{
	DATE dt;
	COleDateTime da;
	CString strValue;
	
	switch (var.vt)
	{
	case VT_BSTR://�ַ���
	case VT_LPSTR://�ַ���
	case VT_LPWSTR://�ַ���
		strValue = (LPCTSTR)(_bstr_t)var;
		break;
	case VT_I2://������
		strValue.Format("%d", var.iVal);
		break;
	case VT_UI2://�޷��Ŷ�����
		strValue.Format("%d", var.uiVal);
		break;
	case VT_VOID://
	case VT_INT://����
	case VT_I4://����
		strValue.Format("%d", var.intVal);
		break;
	case VT_I8://������
		strValue.Format("%d", var.lVal);
		break;
	case VT_UINT://�޷�������
	case VT_UI4://�޷�������
		strValue.Format("%d", var.uintVal);
		break;
	case VT_UI8://�޷��ų�����
		strValue.Format("%d", var.ulVal);
		break;
	case VT_R4://������
		strValue.Format("%.4f", var.fltVal);
		break;
	case VT_R8://˫������
		strValue.Format("%.8f", var.dblVal);
		break;
	case VT_DECIMAL: //С��
		{
			double val = var.decVal.Lo32;
			val *= (var.decVal.sign == 128)? -1 : 1;
			val /= pow(10, var.decVal.scale * 1.0); 
			strValue.Format("%.4f", val);
		}
		break;
	case VT_BOOL://������
		strValue = var.lVal ? "TRUE" : "FALSE";
		break;
	case VT_DATE: //������
		dt = var.date;
		da = COleDateTime(dt); 
		strValue = da.Format("%Y-%m-%d %H:%M:%S");
		break;
	case VT_NULL://NULLֵ
		//strValue = "NULL";
		strValue = "";
		break;
	case VT_EMPTY://��
		//strValue = "EMPTY";
		strValue = "";
		break;
	case VT_UNKNOWN://δ֪����
	default:
		strValue = "UN_KNOW";
		break;
	}
	return strValue;
}


BOOL CAdoRecordSet::SetCacheSize(const long &lCacheSize)
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (m_pRecordset != NULL && !(GetState() & adStateExecuting))
		{
			return SUCCEEDED(m_pRecordset->put_CacheSize(lCacheSize));
		}
	}
	catch (const _com_error& e)
	{
		TRACE(_T(":( SetCacheSize���������쳣: %s\n"), e.ErrorMessage());
		return FALSE;
	}
	return FALSE;
}

/*========================================================================
	Name:	ȷ��ָ���� Recordset �����Ƿ�֧���ض����͵Ĺ��ܡ�	
    ----------------------------------------------------------
	Params:	CursorOptions   �����ͣ�����һ���������� CursorOptionEnum ֵ��
	����				˵�� 
	adAddNew			��ʹ�� AddNew ��������¼�¼�� 
	adApproxPosition	�ɶ�ȡ������ AbsolutePosition �� AbsolutePage �����ԡ� 
	adBookmark			��ʹ�� Bookmark ���Ի�ö��ض���¼�ķ��ʡ� 
	adDelete			����ʹ�� Delete ����ɾ����¼�� 
	AdHoldRecords		���Լ��������¼���߸�����һ������λ�ö������ύ���й���ĸ��ġ� 
	AdMovePrevious		��ʹ�� MoveFirst �� MovePrevious �������Լ� Move ��
					GetRows ��������ǰ��¼λ������ƶ�������ʹ����ǩ�� 
	AdResync			ͨ�� Resync ������ʹ���ڻ��������ݿ��пɼ������ݸ����αꡣ 
	AdUpdate			��ʹ�� Update �����޸����е����ݡ� 
	AdUpdateBatch		����ʹ��������(UpdateBatch �� CancelBatch ����)�������鴫����ṩ�ߡ� 
	AdIndex				����ʹ�� Index �������������� 
	AdSeek				����ʹ�� Seek ������λ Recordset �е��С� 
    ----------------------------------------------------------
	returns:	���ز�����ֵ��ָʾ�Ƿ�֧�� CursorOptions ��������ʶ�����й��ܡ�
    ----------------------------------------------------------
	Remarks:	ʹ�� Supports ����ȷ�� Recordset ������֧�ֵĹ������͡����
			Recordset ����֧������Ӧ������ CursorOptions �еĹ��ܣ���ô Supports
			�������� True�����򷵻� False��
	ע��   ���� Supports �����ɶԸ����Ĺ��ܷ��� True���������ܱ�֤�ṩ�߿�
	��ʹ���������л����¾���Ч��Supports ����ֻ�����ṩ���Ƿ�֧��ָ���Ĺ���
	(�ٶ�����ĳЩ����)�����磬Supports ��������ָʾ Recordset ����֧�ָ���
	(��ʹ�α���ڶ����ĺϲ�)��������ĳЩ����Ȼ�޷����¡�
==========================================================================*/
BOOL CAdoRecordSet::Supports(CursorOptionEnum CursorOptions)
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (m_pRecordset != NULL)
		{
			BOOL bSupports = m_pRecordset->Supports(CursorOptions);
			return bSupports;
		}
	}
	catch (const _com_error& e)
	{
		TRACE(_T(":( Supports���������쳣: %s\n"), e.ErrorMessage());
		return FALSE;
	}
	return FALSE;
}

/*########################################################################
			  ------------------------------------------------
								  ���ݴ�ȡ
			  ------------------------------------------------
  ########################################################################*/

BOOL CAdoRecordSet::PutCollect(long index, const _variant_t &value)
{
	ASSERT(m_pRecordset != NULL);
	ASSERT(index < GetFieldsCount());
	try
	{
		if (m_pRecordset != NULL && index < GetFieldsCount()) 
		{
			m_pRecordset->PutCollect(_variant_t(index), value);
		}
	}
	catch (_com_error e)
	{
		return FALSE;
	} 
	return	FALSE;
}

BOOL CAdoRecordSet::PutCollect(LPCSTR strFieldName, _variant_t &value)
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (m_pRecordset != NULL) 
		{
			m_pRecordset->put_Collect(_variant_t(strFieldName), value);
		}
	}
	catch (_com_error e)
	{
		return FALSE;
	} 
	return	FALSE;
}

BOOL CAdoRecordSet::PutCollect(long index, const LPCTSTR &value)
{
	ASSERT(m_pRecordset != NULL);
	#ifdef _DEBUG
	if (! (GetFieldType(index) == adVarChar
		|| GetFieldType(index) == adChar
		|| GetFieldType(index) == adLongVarChar))
	//AfxMessageBox("��Ҫ�洢���ֶ���������������Ͳ���");
	#endif

	ASSERT(GetFieldType(index) == adVarChar
		|| GetFieldType(index) == adChar
		|| GetFieldType(index) == adLongVarChar);
	
	return PutCollect(index, _variant_t(value));
}

BOOL CAdoRecordSet::PutCollect(long index, const BYTE &value)
{
	ASSERT(m_pRecordset != NULL);
	#ifdef _DEBUG
	if (GetFieldType(index) != adUnsignedTinyInt)
	//AfxMessageBox("��Ҫ�洢���ֶ���������������Ͳ���");
	#endif
	ASSERT(GetFieldType(index) == adUnsignedTinyInt);

	return PutCollect(index, _variant_t(value));
}

BOOL CAdoRecordSet::PutCollect(long index, const short &value)
{
	ASSERT(m_pRecordset != NULL);
	#ifdef _DEBUG
	if (GetFieldType(index) != adSmallInt)
	//AfxMessageBox("��Ҫ�洢���ֶ���������������Ͳ���");
	#endif
	ASSERT(GetFieldType(index) == adSmallInt);
	
	return PutCollect(index, _variant_t(value));
}

BOOL CAdoRecordSet::PutCollect(long index, const int &value)
{
	ASSERT(m_pRecordset != NULL);
	#ifdef _DEBUG
	if (GetFieldType(index) != adInteger)
	//AfxMessageBox("��Ҫ�洢���ֶ���������������Ͳ���");
	#endif
	ASSERT(GetFieldType(index) == adInteger);
	
	return PutCollect(index, _variant_t(long(value)));
}

BOOL CAdoRecordSet::PutCollect(long index, const long &value)
{
	ASSERT(m_pRecordset != NULL);
	#ifdef _DEBUG
	if (GetFieldType(index) != adBigInt)
	//AfxMessageBox("��Ҫ�洢���ֶ���������������Ͳ���");
	#endif
	ASSERT(GetFieldType(index) == adBigInt);
	
	return PutCollect(index, _variant_t(value));
}

BOOL CAdoRecordSet::PutCollect(long index, const float &value)
{
	ASSERT(m_pRecordset != NULL);
	#ifdef _DEBUG
	if (GetFieldType(index) != adSingle)
	//AfxMessageBox("��Ҫ�洢���ֶ���������������Ͳ���");
	#endif
	ASSERT(GetFieldType(index) == adSingle);
	
	return PutCollect(index, _variant_t(value));
}

BOOL CAdoRecordSet::PutCollect(long index, const double &value)
{
	ASSERT(m_pRecordset != NULL);
	#ifdef _DEBUG
	if (GetFieldType(index) != adDouble)
	//AfxMessageBox("��Ҫ�洢���ֶ���������������Ͳ���");
	#endif
	ASSERT(GetFieldType(index) == adDouble);

	return PutCollect(index, _variant_t(value));
}

BOOL CAdoRecordSet::PutCollect(LPCTSTR strFieldName, const LPCTSTR &value)
{
	ASSERT(m_pRecordset != NULL);
	#ifdef _DEBUG
	if (! (GetFieldType(strFieldName) == adVarChar
		|| GetFieldType(strFieldName) == adChar
		|| GetFieldType(strFieldName) == adLongVarChar))
	//AfxMessageBox("��Ҫ�洢���ֶ���������������Ͳ���");
	#endif
	ASSERT(GetFieldType(strFieldName) == adVarChar
		|| GetFieldType(strFieldName) == adChar
		|| GetFieldType(strFieldName) == adLongVarChar);

	_variant_t  val(value);
	
	return PutCollect(strFieldName, val);
}

BOOL CAdoRecordSet::PutCollect(LPCTSTR strFieldName, const BYTE &value)
{
	ASSERT(m_pRecordset != NULL);
	#ifdef _DEBUG
	if (GetFieldType(strFieldName) != adUnsignedTinyInt)
	//AfxMessageBox("��Ҫ�洢���ֶ���������������Ͳ���");
	#endif
	ASSERT(GetFieldType(strFieldName) == adUnsignedTinyInt);

	 _variant_t  val(value);
	
	return PutCollect(strFieldName, val);
}

BOOL CAdoRecordSet::PutCollect(LPCTSTR strFieldName, const short &value)
{
	ASSERT(m_pRecordset != NULL);
	#ifdef _DEBUG
	if (GetFieldType(strFieldName) != adSmallInt)
	//AfxMessageBox("��Ҫ�洢���ֶ���������������Ͳ���");
	#endif
	ASSERT(GetFieldType(strFieldName) == adSmallInt);


	_variant_t  val(value);

	return PutCollect(strFieldName, val);
}

BOOL CAdoRecordSet::PutCollect(LPCTSTR strFieldName, const int &value)
{
	ASSERT(m_pRecordset != NULL);
	
	#ifdef _DEBUG
	if (GetFieldType(strFieldName) != adInteger)
	//AfxMessageBox("��Ҫ�洢���ֶ���������������Ͳ���");
	#endif
	ASSERT(GetFieldType(strFieldName) == adInteger);

	long nValue(value);
	_variant_t  val(nValue);

	return PutCollect(strFieldName, val);
}

BOOL CAdoRecordSet::PutCollect(LPCTSTR strFieldName, const long &value)
{
	ASSERT(m_pRecordset != NULL);
	#ifdef _DEBUG
	if (GetFieldType(strFieldName) != adBigInt)
	//AfxMessageBox("��Ҫ�洢���ֶ���������������Ͳ���");
	#endif
	ASSERT(GetFieldType(strFieldName) == adBigInt);

	_variant_t val(value);
	
	return PutCollect(strFieldName, val);
}

BOOL CAdoRecordSet::PutCollect(LPCTSTR strFieldName, const float &value)
{
	ASSERT(m_pRecordset != NULL);
	#ifdef _DEBUG
	if (GetFieldType(strFieldName) != adSingle)
	//AfxMessageBox("��Ҫ�洢���ֶ���������������Ͳ���");
	#endif
	ASSERT(GetFieldType(strFieldName) == adSingle);

	_variant_t val(value);

	return PutCollect(strFieldName, val);
}

BOOL CAdoRecordSet::PutCollect(LPCTSTR strFieldName, const double &value)
{
	ASSERT(m_pRecordset != NULL);
	#ifdef _DEBUG
	if (GetFieldType(strFieldName) != adDouble)
	//AfxMessageBox("��Ҫ�洢���ֶ���������������Ͳ���");
	#endif
	ASSERT(GetFieldType(strFieldName) == adDouble);
	
	_variant_t val(value);
	return PutCollect(strFieldName, val);
}

/*########################################################################
			  ------------------------------------------------
							       ���ݴ�ȡ
			  ------------------------------------------------
  ########################################################################*/

BOOL CAdoRecordSet::GetCollect(long index,  BYTE &value)
{
	ASSERT(m_pRecordset != NULL);
	#ifdef _DEBUG
	if (GetFieldType(index) != adUnsignedTinyInt)
	//AfxMessageBox("��Ҫ��ȡ���ֶ���������������Ͳ���");
	#endif
	ASSERT(GetFieldType(index) == adUnsignedTinyInt);

	try
	{
		if (m_pRecordset != NULL) 
		{
			_variant_t result = m_pRecordset->GetCollect(_variant_t(index));
			value = result.bVal;
			return TRUE;
		}
	}
	catch (_com_error e)
	{
		return FALSE;
	} 	
	return FALSE;
}

BOOL CAdoRecordSet::GetCollect(long index,  short &value)
{
	ASSERT(m_pRecordset != NULL);
	ASSERT(m_pRecordset != NULL);
	#ifdef _DEBUG
	if (GetFieldType(index) != adSmallInt)
	//AfxMessageBox("��Ҫ��ȡ���ֶ���������������Ͳ���");
	#endif
	ASSERT(GetFieldType(index) == adSmallInt);

	try
	{
		if (m_pRecordset != NULL) 
		{
			_variant_t result = m_pRecordset->GetCollect(_variant_t(index));
			value = result.iVal;
			return TRUE;
		}
	}
	catch (_com_error e)
	{
		return FALSE;
	} 	
	return FALSE;
}

BOOL CAdoRecordSet::GetCollect(long index,  int &value)
{
	ASSERT(m_pRecordset != NULL);
	#ifdef _DEBUG
	if (GetFieldType(index) != adInteger)
	//AfxMessageBox("��Ҫ��ȡ���ֶ���������������Ͳ���");
	#endif
	ASSERT(GetFieldType(index) == adInteger);

	try
	{
		if (m_pRecordset != NULL) 
		{
			_variant_t result = m_pRecordset->GetCollect(_variant_t(index));
			value = result.intVal;
			return TRUE;
		}
	}
	catch (_com_error e)
	{
		return FALSE;
	} 	
	return FALSE;
}

BOOL CAdoRecordSet::GetCollect(long index,  long &value)
{
	ASSERT(m_pRecordset != NULL);
	#ifdef _DEBUG
	if (GetFieldType(index) != adBigInt)
	//AfxMessageBox("��Ҫ��ȡ���ֶ���������������Ͳ���");
	#endif
	ASSERT(GetFieldType(index) == adBigInt);

	try
	{
		if (m_pRecordset != NULL) 
		{
			_variant_t result = m_pRecordset->GetCollect(_variant_t(index));
			value = result.lVal;
			return TRUE;
		}
	}
	catch (_com_error e)
	{
		return FALSE;
	} 	
	return FALSE;
}

BOOL CAdoRecordSet::GetCollect(long index,  float &value)
{
	ASSERT(m_pRecordset != NULL);
	#ifdef _DEBUG
	if (GetFieldType(index) != adSingle)
	//AfxMessageBox("��Ҫ��ȡ���ֶ���������������Ͳ���");
	#endif
	ASSERT(GetFieldType(index) == adSingle);

	try
	{
		if (m_pRecordset != NULL) 
		{
			_variant_t result = m_pRecordset->GetCollect(_variant_t(index));
			value = result.fltVal;
			return TRUE;
		}
	}
	catch (_com_error e)
	{
		return FALSE;
	} 	
	return FALSE;
}

BOOL CAdoRecordSet::GetCollect(long index,  double &value)
{
	ASSERT(m_pRecordset != NULL);
	#ifdef _DEBUG
	if (GetFieldType(index) != adDouble)
	//AfxMessageBox("��Ҫ��ȡ���ֶ���������������Ͳ���");
	#endif
	ASSERT(GetFieldType(index) == adDouble);

	try
	{
		if (m_pRecordset != NULL) 
		{
			_variant_t result = m_pRecordset->GetCollect(_variant_t(index));
			value = result.dblVal;
			return TRUE;
		}
	}
	catch (_com_error e)
	{
		return FALSE;
	} 	
	return FALSE;
}

BOOL CAdoRecordSet::GetCollect(LPCSTR strFieldName,  BYTE &value)
{
	ASSERT(m_pRecordset != NULL);
	#ifdef _DEBUG
	if (GetFieldType(strFieldName) != adUnsignedTinyInt)
	//AfxMessageBox("��Ҫ��ȡ���ֶ���������������Ͳ���");
	#endif
	ASSERT(GetFieldType(strFieldName) == adUnsignedTinyInt);

	try
	{
		if (m_pRecordset != NULL) 
		{
			_variant_t result = m_pRecordset->GetCollect(_variant_t(strFieldName));
			value = result.bVal;
			return TRUE;
		}
	}
	catch (_com_error e)
	{
		return FALSE;
	} 	
	return FALSE;
}

BOOL CAdoRecordSet::GetCollect(LPCSTR strFieldName,  short &value)
{
	ASSERT(m_pRecordset != NULL);
	#ifdef _DEBUG
	if (GetFieldType(strFieldName) != adSmallInt)
	//AfxMessageBox("��Ҫ��ȡ���ֶ���������������Ͳ���");
	#endif
	ASSERT(GetFieldType(strFieldName) == adSmallInt);

	try
	{
		if (m_pRecordset != NULL) 
		{
			_variant_t result = m_pRecordset->GetCollect(_variant_t(strFieldName));
			value = result.iVal;
			return TRUE;
		}
	}
	catch (_com_error e)
	{
		return FALSE;
	} 	
	return FALSE;
}

BOOL CAdoRecordSet::GetCollect(LPCSTR strFieldName,  int &value)
{
	ASSERT(m_pRecordset != NULL);
	#ifdef _DEBUG
	if (GetFieldType(strFieldName) != adInteger)
	//AfxMessageBox("��Ҫ��ȡ���ֶ���������������Ͳ���");
	#endif
	ASSERT(GetFieldType(strFieldName) == adInteger);

	try
	{
		if (m_pRecordset != NULL) 
		{
			_variant_t result = m_pRecordset->GetCollect(_variant_t(strFieldName));
			value = result.intVal;
			return TRUE;
		}
	}
	catch (_com_error e)
	{
		return FALSE;
	} 	
	return FALSE;
}

BOOL CAdoRecordSet::GetCollect(LPCSTR strFieldName,  long &value)
{
	ASSERT(m_pRecordset != NULL);
	#ifdef _DEBUG
	if (GetFieldType(strFieldName) != adBigInt)
	//AfxMessageBox("��Ҫ��ȡ���ֶ���������������Ͳ���");
	#endif
	ASSERT(GetFieldType(strFieldName) == adBigInt);

	try
	{
		if (m_pRecordset != NULL) 
		{
			_variant_t result = m_pRecordset->GetCollect(_variant_t(strFieldName));
			value = result.lVal;
			return TRUE;
		}
	}
	catch (_com_error e)
	{
		return FALSE;
	} 	
	return FALSE;
}

BOOL CAdoRecordSet::GetCollect(LPCSTR strFieldName,  float &value)
{
	ASSERT(m_pRecordset != NULL);
	#ifdef _DEBUG
	if (GetFieldType(strFieldName) != adSingle)
	//AfxMessageBox("��Ҫ��ȡ���ֶ���������������Ͳ���");
	#endif
	ASSERT(GetFieldType(strFieldName) == adSingle);

	try
	{
		if (m_pRecordset != NULL) 
		{
			_variant_t result = m_pRecordset->GetCollect(_variant_t(strFieldName));
			value = result.fltVal;
			return TRUE;
		}
	}
	catch (_com_error e)
	{
		return FALSE;
	} 	
	return FALSE;
}

BOOL CAdoRecordSet::GetCollect(LPCSTR strFieldName,  double &value)
{
	ASSERT(m_pRecordset != NULL);
	#ifdef _DEBUG
	if (GetFieldType(strFieldName) != adDouble)
	//AfxMessageBox("��Ҫ��ȡ���ֶ���������������Ͳ���");
	#endif
	ASSERT(GetFieldType(strFieldName) == adDouble);

	try
	{
		if (m_pRecordset != NULL) 
		{
			_variant_t result = m_pRecordset->GetCollect(_variant_t(strFieldName));
			value = result.dblVal;
			return TRUE;
		}
	}
	catch (_com_error e)
	{
		return FALSE;
	} 	
	return FALSE;
}

BOOL CAdoRecordSet::GetCollect(long index,  CString &value)
{
	ASSERT(m_pRecordset != NULL);
	#ifdef _DEBUG
	if (! (GetFieldType(index) == adVarChar
		|| GetFieldType(index) == adChar
		|| GetFieldType(index) == adLongVarChar))
	//AfxMessageBox("��Ҫ��ȡ���ֶ���������������Ͳ���");
	#endif

	ASSERT(GetFieldType(index) == adVarChar
		|| GetFieldType(index) == adChar
		|| GetFieldType(index) == adLongVarChar);
	
	try
	{
		if (m_pRecordset != NULL) 
		{
			_variant_t result = m_pRecordset->GetCollect(_variant_t(index));
			value = (LPCTSTR)(_bstr_t)result;
			return TRUE;
		}
	}
	catch (_com_error e)
	{
		return FALSE;
	} 	
	return FALSE;
}

BOOL CAdoRecordSet::IsOpen()
{
	try
	{
		return (m_pRecordset != NULL && (GetState() & adStateOpen));
	}
	catch (_com_error e)
	{
		TRACE(_T(":( IsOpen���������쳣: %s\n"), e.ErrorMessage());
		return FALSE;
	} 
	return FALSE;
}
