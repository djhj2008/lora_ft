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

	CString m_strFYCMin;
	CString m_strFYMMax;
	CString m_strPsn;
	CString m_strTCCCode;
};
