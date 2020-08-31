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
		m_pConnection.CreateInstance("ADODB.Connection");  //创建连接对象实例
		_bstr_t strConnect="DRIVER={Microsoft Access Driver (*.mdb)};\
		uid=;pwd=;DBQ=SPgDataBase.mdb;";
	   m_pConnection->Open(strConnect,"","",adModeUnknown); //打开数据库
	}
	catch (_com_error * e)
	{
		AfxMessageBox(e->Description());
	}


}	
//!打开记录集
_RecordsetPtr&  Paint_ADO::OpenRecordset(CString sql)
{
	//!SQL语句不能为空
	ASSERT(!sql.IsEmpty());
	try
	{
		//创建记录集对象实例
		m_pRecordset.CreateInstance(__uuidof(Recordset));
		//执行SQL得到记录集
		m_pRecordset->Open(_bstr_t(sql),m_pConnection.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
	}
	catch (_com_error * e)
	{
		AfxMessageBox(e->Description());
	}
	return m_pRecordset;									
}	
//!关闭记录集
void Paint_ADO::CloseRecordset()
{
	if(m_pRecordset->GetState() ==adStateOpen)
	{
		m_pRecordset->Close();
	}
}
//!关闭数据库连接
void Paint_ADO::CloseConn()
{
	m_pConnection->Close();
	//释放com环境
	::CoInitialize(NULL);
}	
//!获得记录数
UINT Paint_ADO::GetRecordCount(_RecordsetPtr pRecordset)
{
	int nCount = 0;											//声明保存记录数的变量
	try{
		pRecordset->MoveFirst();								//将记录集指针移动到第一条记录
	}
	catch(...)												//捕捉可能出现的错误
	{
		return 0;											//产生错误时返回0
	}
	if(pRecordset->EndOfFile)									//判断记录集中是否没有记录
		return 0;											//无记录时返回0
	while (!pRecordset->EndOfFile)								//当记录集指针没有指向最后时
	{
		pRecordset->MoveNext();								//将记录集指针移动到下一条记录
		nCount = nCount + 1;								//记录个数的变量加1
	}
	pRecordset->MoveFirst();									//将记录集指针移动到第一条记录
	return nCount;											//返回记录数
}	