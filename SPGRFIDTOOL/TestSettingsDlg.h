#pragma once


// CTestSettingsDlg dialog

class CTestSettingsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTestSettingsDlg)

public:
	CTestSettingsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTestSettingsDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_SETTINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSave();
	CString m_strTestFreq;
	CString m_strWorkFreq;
	CString m_strAdcSampleMaxValue;
	CString m_strAdcSampleMinValue;
	CString m_strShutDownCurrentMaxValue;
	CString m_strTCCCode;
	CString m_strFactoryCode;
	void SaveSmallTestToolConfig(void);
};
