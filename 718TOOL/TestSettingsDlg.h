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

	void SaveSmallTestToolConfig(void);

	CString m_strAlarm;
	CString m_strRate;
	CString m_strUrl;
	CString m_strTCCCode;
	CString m_strAdcMax;
	CString m_strAdcMin;
	BOOL m_check1301;
	afx_msg void OnEnChangeEditTestUrl();
	CString m_str1278rx;
	CString m_str1278tx;
	CString m_str1301rx;
	CString m_str1301tx;
};
