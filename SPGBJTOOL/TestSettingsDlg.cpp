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
	, m_strTestFreq(_T(""))
    , m_strWorkFreq(_T(""))
	, m_strAdcSampleMaxValue(_T(""))
	, m_strAdcSampleMinValue(_T(""))
	, m_strRxrssi(_T(""))
	, m_strTxrssi(_T(""))
	, m_checkStep(FALSE)
{

}

CTestSettingsDlg::~CTestSettingsDlg()
{
}

void CTestSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TEST_FREQ_SETTING, m_strTestFreq);
	DDV_MaxChars(pDX, m_strTestFreq, 9);

	DDX_Text(pDX, IDC_EDIT_WORK_FREQ_SETTING, m_strWorkFreq);
	DDV_MaxChars(pDX, m_strWorkFreq, 9);

	DDX_Text(pDX, IDC_EDIT_ADC_SAMPLE_VALUE_MAX, m_strAdcSampleMaxValue);
	DDX_Text(pDX, IDC_EDIT_ADC_SAMPLE_VALUE_MIN, m_strAdcSampleMinValue);

	DDX_Text(pDX, IDC_EDIT_RX_RSSI, m_strRxrssi);
	DDX_Text(pDX, IDC_EDIT_TX_RSSI, m_strTxrssi);
	DDX_Check(pDX, IDC_CHECK_STEP, m_checkStep);
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
    ::WritePrivateProfileString(_T("SmallBJConfig"), _T("TestFreq"), m_strTestFreq, strPath);
    ::WritePrivateProfileString(_T("SmallBJConfig"), _T("WorkFreq"), m_strWorkFreq, strPath);
    ::WritePrivateProfileString(_T("SmallBJConfig"), _T("AdcSampleMaxValue"), m_strAdcSampleMaxValue, strPath);  
    ::WritePrivateProfileString(_T("SmallBJConfig"), _T("AdcSampleMinValue"), m_strAdcSampleMinValue, strPath);
	::WritePrivateProfileString(_T("SmallBJConfig"), _T("TXRSSI"), m_strTxrssi, strPath);
	::WritePrivateProfileString(_T("SmallBJConfig"), _T("RXRSSI"), m_strRxrssi, strPath);
	if (m_checkStep)
		::WritePrivateProfileString(_T("SmallBJConfig"), _T("TestStep"), "1", strPath);
	else
		::WritePrivateProfileString(_T("SmallBJConfig"), _T("TestStep"), "0", strPath);
}

