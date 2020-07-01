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
	, m_strAdcSampleMaxValue(_T(""))
	, m_strAdcSampleMinValue(_T(""))
	, m_strRssiValue(_T(""))
{

}

CTestSettingsDlg::~CTestSettingsDlg()
{
}

void CTestSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);


	DDX_Text(pDX, IDC_EDIT_ADC_SAMPLE_VALUE_MAX, m_strAdcSampleMaxValue);
	DDV_MaxChars(pDX, m_strAdcSampleMaxValue, 30);
	DDX_Text(pDX, IDC_EDIT_ADC_SAMPLE_VALUE_MIN, m_strAdcSampleMinValue);
	DDV_MaxChars(pDX, m_strAdcSampleMinValue, 30);
	DDX_Text(pDX, IDC_EDIT_RSSI_VALUE, m_strRssiValue);
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

    ::WritePrivateProfileString(_T("SmallBJConfig"), _T("AdcSampleMaxValue"), m_strAdcSampleMaxValue, strPath);  
    ::WritePrivateProfileString(_T("SmallBJConfig"), _T("AdcSampleMinValue"), m_strAdcSampleMinValue, strPath);
	::WritePrivateProfileString(_T("SmallBJConfig"), _T("RssiValue"), m_strRssiValue, strPath);

}
