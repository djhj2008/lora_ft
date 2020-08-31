// Paint_ADO.h: interface for the Paint_ADO class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PAINT_ADO_H__F6154592_EAA5_4985_BBF7_BAF884DAF8A3__INCLUDED_)
#define AFX_PAINT_ADO_H__F6154592_EAA5_4985_BBF7_BAF884DAF8A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Paint_ADO  
{
public:
	/*
		���Ӷ���ָ�룬��ʹ��_ConnectionPtr����ָ��ʱ֮ǰ����Ҫ���붯̬���ӿ�
		���뷽��Ϊ����stdAfx.h�м���
		#import "c:\Program Files\Common Files\System\ADO\msado15.dll" \
		rename_namespace("ADOCG") rename("EOF", "EndOfFile") rename("BOF", "BeginOfFile")
		using namespace ADOCG;
	*/
	_ConnectionPtr m_pConnection;
	//��¼������ָ��			
	_RecordsetPtr  m_pRecordset;					
public:
	Paint_ADO();
	virtual ~Paint_ADO();
	//�������ݿ�
	void OnInitADOConn();	
	//�򿪼�¼��
	_RecordsetPtr&  OpenRecordset(CString sql);	
	//�رռ�¼��
	void CloseRecordset();
	//�ر����ݿ�����
	void CloseConn();	
	//��ü�¼��
	UINT GetRecordCount(_RecordsetPtr pRecordset);
	int index;
	int Count;

};

#endif // !defined(AFX_PAINT_ADO_H__F6154592_EAA5_4985_BBF7_BAF884DAF8A3__INCLUDED_)
