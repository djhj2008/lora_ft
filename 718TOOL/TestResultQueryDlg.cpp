// TestResultQueryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "COMTOOL.h"
#include "TestResultQueryDlg.h"
#include "afxdialogex.h"


// CTestResultQueryDlg dialog

IMPLEMENT_DYNAMIC(CTestResultQueryDlg, CDialogEx)

CTestResultQueryDlg::CTestResultQueryDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestResultQueryDlg::IDD, pParent)
	, m_strSN(_T(""))
	, m_strRTC(_T(""))
	, m_strADC(_T(""))
	, m_strLORA(_T(""))
	, m_strATW(_T(""))
	, m_strTime(_T(""))
{

	m_strRFEPC = _T("");
}

CTestResultQueryDlg::~CTestResultQueryDlg()
{
}

void CTestResultQueryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Text(pDX, IDC_EDIT_SN, m_strSN);
	DDX_Text(pDX, IDC_EDIT_RTC, m_strRTC);
	DDX_Text(pDX, IDC_EDIT_ADC, m_strADC);
	DDX_Text(pDX, IDC_EDIT_LORA, m_strLORA);
	DDX_Text(pDX, IDC_EDIT_ATW, m_strATW);
	DDX_Text(pDX, IDC_EDIT_TIME, m_strTime);
	DDX_Text(pDX, IDC_EDIT_RFEPC, m_strRFEPC);
}


BEGIN_MESSAGE_MAP(CTestResultQueryDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &CTestResultQueryDlg::OnBnClickedButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_PREVIOUS, &CTestResultQueryDlg::OnBnClickedButtonPrevious)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CTestResultQueryDlg::OnNMClickList1)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CTestResultQueryDlg::OnBnClickedButtonDel)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CTestResultQueryDlg::OnLvnItemchangedList1)
	ON_BN_CLICKED(IDC_SEARCH_BUTTON, &CTestResultQueryDlg::OnBnClickedSearchButton)
	ON_BN_CLICKED(IDC_LIST_ALL, &CTestResultQueryDlg::OnBnClickedListAll)
END_MESSAGE_MAP()


// CTestResultQueryDlg message handlers


BOOL CTestResultQueryDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_List.InsertColumn(0,"SN",LVCFMT_LEFT,102,0);
	m_List.InsertColumn(1,"NET",LVCFMT_LEFT,102,1);
	m_List.InsertColumn(2,"TCC",LVCFMT_LEFT,102,2);
	m_List.InsertColumn(3,"RTC",LVCFMT_LEFT,102,3);
	m_List.InsertColumn(4,"ADC",LVCFMT_LEFT,102,4);
	m_List.InsertColumn(5,"LORA1278",LVCFMT_LEFT,102,5);
	m_List.InsertColumn(6,"LORA1301",LVCFMT_LEFT,200,6);
	m_List.InsertColumn(7,"LTE",LVCFMT_LEFT,102,7);
	m_List.InsertColumn(8,"STATE",LVCFMT_LEFT,102,8);
	m_List.InsertColumn(9,"TIME", LVCFMT_LEFT, 102, 9);
	//!连接数据库
	RunAdoSQL("select * from SmallBoard_Info order by SBI_SN asc"); //desc
	AddToList(1);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

#define LIST_MAXITEM  50
//http://www.time.ac.cn/timeflash.asp?user=flash // 时间查询
void CTestResultQueryDlg::AddToList(int flag)   // flag 1: 向后查询; 0: 向后查询
{
	int count =0;
    m_List.DeleteAllItems();
	if(m_Ado.Count==0)
	{
		return;
	}
	if(flag==1)
	{
		while(count<LIST_MAXITEM)
		{
			if(m_Ado.m_pRecordset->BeginOfFile)
			{
				GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(0);
				m_Ado.m_pRecordset->MoveFirst();
			}
			m_List.InsertItem(0,"");
			m_List.SetItemText(0, 0, (char*)(_bstr_t)m_Ado.m_pRecordset->GetCollect("SBI_SN"));
			m_List.SetItemText(0, 1, (char*)(_bstr_t)m_Ado.m_pRecordset->GetCollect("SBI_NET"));
			m_List.SetItemText(0, 2, (char*)(_bstr_t)m_Ado.m_pRecordset->GetCollect("SBI_TCC"));
			m_List.SetItemText(0, 3, (char*)(_bstr_t)m_Ado.m_pRecordset->GetCollect("SBI_RTC"));
			m_List.SetItemText(0, 4, (char*)(_bstr_t)m_Ado.m_pRecordset->GetCollect("SBI_ADC"));
			m_List.SetItemText(0, 5, (char*)(_bstr_t)m_Ado.m_pRecordset->GetCollect("SBI_LORA_1278"));
			m_List.SetItemText(0, 6, (char*)(_bstr_t)m_Ado.m_pRecordset->GetCollect("SBI_LORA_1301"));
			m_List.SetItemText(0, 7, (char*)(_bstr_t)m_Ado.m_pRecordset->GetCollect("SBI_LTE"));
			m_List.SetItemText(0, 8, (char*)(_bstr_t)m_Ado.m_pRecordset->GetCollect("SBI_ATW_STATE"));
			m_List.SetItemText(0, 9, (char*)(_bstr_t)m_Ado.m_pRecordset->GetCollect("SBI_TIME"));

			//!将记录集指针移动到下一条记录
			m_Ado.m_pRecordset->MoveNext();
			m_Ado.index++;
			count++;
			if(m_Ado.m_pRecordset->EndOfFile)
			{
				GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(0);
				break;
			}
		}
	}
	else
	{
		while(count<LIST_MAXITEM)
		{
			//!将记录集指针移动到上一条记录
			if(m_Ado.m_pRecordset->BeginOfFile)
			{
				GetDlgItem(IDC_BUTTON_PREVIOUS)->EnableWindow(0);
				break;
			}
			m_Ado.m_pRecordset->MovePrevious();
			if(m_Ado.m_pRecordset->BeginOfFile)
			{
				GetDlgItem(IDC_BUTTON_PREVIOUS)->EnableWindow(0);
				break;
			}
			count++;
			m_Ado.index--;
			m_List.InsertItem(0,"");
			m_List.SetItemText(0, 0, (char*)(_bstr_t)m_Ado.m_pRecordset->GetCollect("SBI_SN"));
			m_List.SetItemText(0, 1, (char*)(_bstr_t)m_Ado.m_pRecordset->GetCollect("SBI_NET"));
			m_List.SetItemText(0, 2, (char*)(_bstr_t)m_Ado.m_pRecordset->GetCollect("SBI_TCC"));
			m_List.SetItemText(0, 3, (char*)(_bstr_t)m_Ado.m_pRecordset->GetCollect("SBI_RTC"));
			m_List.SetItemText(0, 4, (char*)(_bstr_t)m_Ado.m_pRecordset->GetCollect("SBI_ADC"));
			m_List.SetItemText(0, 5, (char*)(_bstr_t)m_Ado.m_pRecordset->GetCollect("SBI_LORA_1278"));
			m_List.SetItemText(0, 6, (char*)(_bstr_t)m_Ado.m_pRecordset->GetCollect("SBI_LORA_1301"));
			m_List.SetItemText(0, 7, (char*)(_bstr_t)m_Ado.m_pRecordset->GetCollect("SBI_LTE"));
			m_List.SetItemText(0, 8, (char*)(_bstr_t)m_Ado.m_pRecordset->GetCollect("SBI_ATW_STATE"));
			m_List.SetItemText(0, 9, (char*)(_bstr_t)m_Ado.m_pRecordset->GetCollect("SBI_TIME"));
		}
	}

#if 1
    if(m_Ado.index==0)
	{
      GetDlgItem(IDC_BUTTON_PREVIOUS)->EnableWindow(0);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_PREVIOUS)->EnableWindow(1);
	}

	if((m_Ado.Count==m_Ado.index))
	{
      GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(0);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(1);
	}
#endif
}


int CTestResultQueryDlg::RunAdoSQL(CString m_strSQL)
{
	CString strTmp;
	int pages = 0;
	if(m_Ado.m_pRecordset)
	{
		m_Ado.CloseRecordset();
		m_Ado.CloseConn();
	}
	try{
		m_Ado.OnInitADOConn();
		m_Ado.m_pRecordset = m_Ado.OpenRecordset(m_strSQL);//打开记录集
		m_Ado.Count = m_Ado.GetRecordCount(m_Ado.m_pRecordset);
	}
	catch (_com_error& e)
	{
	}
	m_Ado.index = 0;
	GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(0);
	pages = m_Ado.Count/LIST_MAXITEM +1;
	if(m_Ado.Count==0)
		pages=0;
	strTmp.Format("总共%d页",pages);
	GetDlgItem(IDC_EDIT_PAGE_STATUS)->SetWindowText(strTmp);
	return 0;
}

BOOL CTestResultQueryDlg::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	m_Ado.CloseRecordset();
	//!断开数据库连接
	m_Ado.CloseConn();
	return CDialogEx::DestroyWindow();
}


void CTestResultQueryDlg::OnBnClickedButtonNext()
{
	// TODO: Add your control notification handler code here
	AddToList(1);
}


void CTestResultQueryDlg::OnBnClickedButtonPrevious()
{
	// TODO: Add your control notification handler code here
	AddToList(0);
}


void CTestResultQueryDlg::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	int m_pos = m_List.GetSelectionMark();
	m_strSN = m_List.GetItemText(m_pos,0);

	//m_strWorkFreq = m_List.GetItemText(m_pos,1); // work Feq

	m_strRTC = m_List.GetItemText(m_pos,3); //RTC
	if(m_strRTC=="0")
	{
	  m_strRTC = "未检测";
	}
	else if(m_strRTC=="1")
	{
		m_strRTC = "测试通过";
	}
	else if(m_strRTC=="2")
	{
		m_strRTC = "超时检测失败";
	}

	m_strADC  = m_List.GetItemText(m_pos,4); //ADC
	if(m_strADC=="0")
	{
	  m_strADC = "未检测";
	}
	else if(m_strADC=="1")
	{
		m_strADC = "测试通过";
	}
	else if(m_strADC=="2")
	{
		m_strRTC = "采样值超出范围";
	}
	else if(m_strADC=="3")
	{
		m_strRTC = "超时检测失败";
	}

	m_strLORA  = m_List.GetItemText(m_pos,5);//LORA

	if(m_strLORA=="0")
	{
	  m_strLORA = "未检测";
	}
	else if(m_strLORA=="1")
	{
		m_strLORA = "测试通过";
	}
	else if(m_strLORA=="2")
	{
		m_strLORA = "检测失败:LORA发送超时";
	}
	else if(m_strLORA=="3")
	{
		m_strLORA = "检测失败:LORA接收超时";
	}
	else if(m_strLORA=="4")
	{
		m_strLORA = "检测失败:结果返回超时";
	}

	m_strATW  = m_List.GetItemText(m_pos,8);//ATW
	if(m_strATW=="0")
	{
	  m_strATW = "状态未切换";
	}
	else if(m_strATW=="1")
	{
		m_strATW = "ATWOK";
	}
	m_strTime = m_List.GetItemText(m_pos,9);
	GetDlgItem(IDC_BUTTON_DEL)->EnableWindow(1);
	UpdateData(FALSE);
}


void CTestResultQueryDlg::OnBnClickedButtonDel()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BUTTON_DEL)->EnableWindow(0);
	CString sn;
	GetDlgItem(IDC_EDIT_SN)->GetWindowText(sn);
	CString strExe = "delete from SmallBoard_Info where SBI_SN='" + sn + "'";
	RunAdoSQL(strExe);
	// update 数据库及显示信息
	strExe = "select * from SmallBoard_Info order by SBI_SN asc";
	RunAdoSQL(strExe); //desc
	AddToList(1);
}


void CTestResultQueryDlg::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CTestResultQueryDlg::OnBnClickedSearchButton()
{
	// TODO: Add your control notification handler code here
	CString sn;
	GetDlgItem(IDC_EDIT_SN)->GetWindowText(sn);
	CString strExe = "select * from SmallBoard_Info where SBI_SN='" + sn + "'";
	RunAdoSQL(strExe); //desc
	AddToList(1);
}

void CTestResultQueryDlg::OnBnClickedListAll()
{
	// TODO: Add your control notification handler code here
	RunAdoSQL("select * from SmallBoard_Info order by SBI_SN asc"); //desc
	AddToList(1);
}
