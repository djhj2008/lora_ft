// Paint_ADO.cpp: implementation of the Paint_ADO class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ComTool.h"
#include "Paint_ADO.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Paint_ADO::Paint_ADO()
{

}

Paint_ADO::~Paint_ADO()
{

}
void Paint_ADO::OnInitADOConn()
{
	::CoInitialize(NULL);
	try
	{
		m_pConnection.CreateInstance("ADODB.Connection");  //�������Ӷ���ʵ��
		_bstr_t strConnect="DRIVER={Microsoft Access Driver (*.mdb)};\
		uid=;pwd=;DBQ=SPgDataBase.mdb;";
	   m_pConnection->Open(strConnect,"","",adModeUnknown); //�����ݿ�
	}
	catch (_com_error * e)
	{
		AfxMessageBox(e->Description());
	}


}	
//!�򿪼�¼��
_RecordsetPtr&  Paint_ADO::OpenRecordset(CString sql)
{
	//!SQL��䲻��Ϊ��
	ASSERT(!sql.IsEmpty());
	try
	{
		//������¼������ʵ��
		m_pRecordset.CreateInstance(__uuidof(Recordset));
		//ִ��SQL�õ���¼��
		m_pRecordset->Open(_bstr_t(sql),m_pConnection.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
	}
	catch (_com_error * e)
	{
		AfxMessageBox(e->Description());
	}
	return m_pRecordset;									
}	
//!�رռ�¼��
void Paint_ADO::CloseRecordset()
{
	if(m_pRecordset->GetState() ==adStateOpen)
	{
		m_pRecordset->Close();
	}
}
//!�ر����ݿ�����
void Paint_ADO::CloseConn()
{
	m_pConnection->Close();
	//�ͷ�com����
	::CoInitialize(NULL);
}	
//!��ü�¼��
UINT Paint_ADO::GetRecordCount(_RecordsetPtr pRecordset)
{
	int nCount = 0;											//���������¼���ı���
	try{
		pRecordset->MoveFirst();								//����¼��ָ���ƶ�����һ����¼
	}
	catch(...)												//��׽���ܳ��ֵĴ���
	{
		return 0;											//��������ʱ����0
	}
	if(pRecordset->EndOfFile)									//�жϼ�¼�����Ƿ�û�м�¼
		return 0;											//�޼�¼ʱ����0
	while (!pRecordset->EndOfFile)								//����¼��ָ��û��ָ�����ʱ
	{
		pRecordset->MoveNext();								//����¼��ָ���ƶ�����һ����¼
		nCount = nCount + 1;								//��¼�����ı�����1
	}
	pRecordset->MoveFirst();									//����¼��ָ���ƶ�����һ����¼
	return nCount;											//���ؼ�¼��
}	