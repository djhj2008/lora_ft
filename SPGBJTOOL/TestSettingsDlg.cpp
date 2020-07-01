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
	, m_strShutDownCurrentMaxValue(_T(""))
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
	DDV_MaxChars(pDX, m_strAdcSampleMaxValue, 30);
	DDX_Text(pDX, IDC_EDIT_ADC_SAMPLE_VALUE_MIN, m_strAdcSampleMinValue);
	DDV_MaxChars(pDX, m_strAdcSampleMinValue, 30);
	DDX_Text(pDX, IDC_EDIT_SHUTDOWN_CURRENT_VALUE_MAX, m_strShutDownCurrentMaxValue);
	DDV_MaxChars(pDX, m_strShutDownCurrentMaxValue, 30);
	DDX_Check(pDX, IDC_CHECK_RFID_SN, m_bRfidSaveSn);
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
#if 1
    ::WritePrivateProfileString(_T("SmallBJConfig"), _T("TestFreq"), m_strTestFreq, strPath);
    ::WritePrivateProfileString(_T("SmallBJConfig"), _T("WorkFreq"), m_strWorkFreq, strPath);
    ::WritePrivateProfileString(_T("SmallBJConfig"), _T("AdcSampleMaxValue"), m_strAdcSampleMaxValue, strPath);  
    ::WritePrivateProfileString(_T("SmallBJConfig"), _T("AdcSampleMinValue"), m_strAdcSampleMinValue, strPath);  
    ::WritePrivateProfileString(_T("SmallBJConfig"), _T("ShutDownCurrentMax"), m_strShutDownCurrentMaxValue, strPath);
	if(m_bRfidSaveSn)
        ::WritePrivateProfileString(_T("SmallBJConfig"), _T("RFIDSaveSN"), "1", strPath);
	else
		::WritePrivateProfileString(_T("SmallBJConfig"), _T("RFIDSaveSN"), "0", strPath);
#else

	memset(buf,0,sizeof(buf));
	sprintf(buf,":%s",m_strTestFreq);

   ::WritePrivateProfileString(("SmallBJConfig"), ("TestFreq"), m_strTestFreq, strPath);  
    ::WritePrivateProfileString(("SmallBJConfig"), ("AdcSampleMaxValue"), m_strAdcSampleMaxValue, strPath);  
    ::WritePrivateProfileString(("SmallBJConfig"), ("AdcSampleMinValue"), m_strAdcSampleMinValue, strPath);  
    ::WritePrivateProfileString(("SmallBJConfig"), ("ShutDownCurrentMax"), m_strShutDownCurrentMaxValue, strPath);
#endif
}
