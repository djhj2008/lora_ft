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
		连接对象指针，在使用_ConnectionPtr智能指针时之前，需要导入动态链接库
		导入方法为：在stdAfx.h中加上
		#import "c:\Program Files\Common Files\System\ADO\msado15.dll" \
		rename_namespace("ADOCG") rename("EOF", "EndOfFile") rename("BOF", "BeginOfFile")
		using namespace ADOCG;
	*/
	_ConnectionPtr m_pConnection;
	//记录集对象指针			
	_RecordsetPtr  m_pRecordset;					
public:
	Paint_ADO();
	virtual ~Paint_ADO();
	//连接数据库
	void OnInitADOConn();	
	//打开记录集
	_RecordsetPtr&  OpenRecordset(CString sql);	
	//关闭记录集
	void CloseRecordset();
	//关闭数据库连接
	void CloseConn();	
	//获得记录数
	UINT GetRecordCount(_RecordsetPtr pRecordset);
	int index;
	int Count;

};

#endif // !defined(AFX_PAINT_ADO_H__F6154592_EAA5_4985_BBF7_BAF884DAF8A3__INCLUDED_)
