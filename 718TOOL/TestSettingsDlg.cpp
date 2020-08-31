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
	, m_strAlarm(_T(""))
	, m_strRate(_T(""))
	, m_strUrl(_T(""))
	, m_strTCCCode(_T(""))
	, m_strAdcMax(_T(""))
	, m_strAdcMin(_T(""))
	, m_check1301(FALSE)
	, m_str1278rx(_T(""))
	, m_str1278tx(_T(""))
	, m_str1301rx(_T(""))
	, m_str1301tx(_T(""))
{

}

CTestSettingsDlg::~CTestSettingsDlg()
{
}

void CTestSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_ALARM, m_strAlarm);
	DDX_Text(pDX, IDC_EDIT_RATE, m_strRate);
	DDX_Text(pDX, IDC_EDIT_TEST_URL, m_strUrl);
	DDX_Text(pDX, IDC_EDIT_TCC_CODE, m_strTCCCode);
	DDX_Text(pDX, IDC_EDIT_ADC_MAX, m_strAdcMax);
	DDX_Text(pDX, IDC_EDIT_ADC_MIN, m_strAdcMin);
	DDX_Check(pDX, IDC_CHECK_1301, m_check1301);
	DDX_Text(pDX, IDC_EDIT_1278_RX, m_str1278rx);
	DDX_Text(pDX, IDC_EDIT_1278_TX, m_str1278tx);
	DDX_Text(pDX, IDC_EDIT_1301_RX, m_str1301rx);
	DDX_Text(pDX, IDC_EDIT_1301_TX, m_str1301tx);
}

BEGIN_MESSAGE_MAP(CTestSettingsDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CTestSettingsDlg::OnBnClickedButtonSave)
	ON_EN_CHANGE(IDC_EDIT_TEST_URL, &CTestSettingsDlg::OnEnChangeEditTestUrl)
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
    ::WritePrivateProfileString(_T("SmallBJConfig"), _T("NET_URL"), m_strUrl, strPath);
    ::WritePrivateProfileString(_T("SmallBJConfig"), _T("TCC"), m_strTCCCode, strPath);
    ::WritePrivateProfileString(_T("SmallBJConfig"), _T("RATE"), m_strRate, strPath);  
    ::WritePrivateProfileString(_T("SmallBJConfig"), _T("ALARM"), m_strAlarm, strPath);
	::WritePrivateProfileString(_T("SmallBJConfig"), _T("ADC_MAX"), m_strAdcMax, strPath);
	::WritePrivateProfileString(_T("SmallBJConfig"), _T("ADC_MIN"), m_strAdcMin, strPath);
	::WritePrivateProfileString(_T("SmallBJConfig"), _T("RX_1278"), m_str1278rx, strPath);
	::WritePrivateProfileString(_T("SmallBJConfig"), _T("TX_1278"), m_str1278tx, strPath);
	::WritePrivateProfileString(_T("SmallBJConfig"), _T("RX_1301"), m_str1301rx, strPath);
	::WritePrivateProfileString(_T("SmallBJConfig"), _T("TX_1301"), m_str1301tx, strPath);

	if (m_check1301)
		::WritePrivateProfileString(_T("SmallBJConfig"), _T("CHECK_1301"), "1", strPath);
	else
		::WritePrivateProfileString(_T("SmallBJConfig"), _T("CHECK_1301"), "0", strPath);
}



void CTestSettingsDlg::OnEnChangeEditTestUrl()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
