// TestSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "COMTOOL.h"
#include "TestSettingsDlg.h"
#include "afxdialogex.h"


// CTestSettingsDlg dialog

IMPLEMENT_DYNAMIC(CTestSettingsDlg, CDialogEx)

CTestSettingsDlg::CTestSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestSettingsDlg::IDD, pParent)
	, m_strTCCCode(_T(""))
	, m_strPsn(_T(""))
	, m_strFYCMin(_T(""))
	, m_strFYMMax(_T(""))
{

}

CTestSettingsDlg::~CTestSettingsDlg()
{
}

void CTestSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_FYCODE_START, m_strFYCMin);
	DDX_Text(pDX, IDC_EDIT_FYCODE_END, m_strFYMMax);
	DDX_Text(pDX, IDC_EDIT_PSN, m_strPsn);
	DDX_Text(pDX, IDC_EDIT_TCC_CODE, m_strTCCCode);
	DDV_MaxChars(pDX, m_strTCCCode, 7);
}


BEGIN_MESSAGE_MAP(CTestSettingsDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CTestSettingsDlg::OnBnClickedButtonSave)
END_MESSAGE_MAP()


// CTestSettingsDlg message handlers


void CTestSettingsDlg::OnBnClickedButtonSave()
{
	// TODO: Add your control notification handler code here
	SaveSmallTestToolConfig();
}


void CTestSettingsDlg::SaveSmallTestToolConfig(void)
{
    CString strPath;
    char buf[1000]; 
    GetCurrentDirectory(1000,buf);  //得到当前工作路径  
	strPath = buf;
	strPath += "\\SmallBJ.ini";
	UpdateData(true);

    ::WritePrivateProfileString(_T("SmallBJConfig"), _T("FYMCode"), m_strTCCCode, strPath);
	::WritePrivateProfileString(_T("SmallBJConfig"), _T("FactoryCode"), m_strPsn, strPath);
	::WritePrivateProfileString(_T("SmallBJConfig"), _T("FYMMin"), m_strFYCMin, strPath);
	::WritePrivateProfileString(_T("SmallBJConfig"), _T("FYMMax"), m_strFYMMax, strPath);
//	if(m_bRfidSaveSn)
//        ::WritePrivateProfileString(_T("SmallBJConfig"), _T("RFIDSaveSN"), "1", strPath);
//	else
//		::WritePrivateProfileString(_T("SmallBJConfig"), _T("RFIDSaveSN"), "0", strPath);
}
