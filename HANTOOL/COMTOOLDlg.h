
// COMTOOLDlg.h : 头文件
//

#include "SerialPort.h"	// Added by ClassView
#pragma once

enum EPSTOREPos
{
	PROJECT_ID=0,
	License_Max=100,
    License_current=200,
    License_current_backup=210,
    License_Logo=480
};

enum TESTCASE
{
	TEST_RECVICE_STR=0,
	TEST_WAIT_CMD,
	TEST_ADC_SAMPLE,
	TEST_SUCCEED_WAIT_NEXT,

	TEST_CASE_MAX
};

// CCOMTOOLDlg 对话框
class CCOMTOOLDlg : public CDialogEx
{
// 构造
public:
	DWORD License_max_value;
	DWORD License_current_value;
	DWORD License_current_backup_value;

	TCHAR m_DevicePath[260];
	CFont m_editFont;
	////////////////////////////////////////////////////////////
	CString DevideHexChar(char HexChar);
	char CombineHexChar(char CharH,char CharL);
	void HexStringFilter(CString &str);
	CString ChangeCharstr2Hexstr(CString Charstr);

	CSerialPort m_SerialPort;

	CCOMTOOLDlg(CWnd* pParent = NULL);	// 标准构造函数
// Dialog Data
	//{{AFX_DATA(CCOMTOOLDlg)
	enum { IDD = IDD_COMTOOL_DIALOG };

	CString	m_strReceive;
	CString	m_strCMD;

	CString	m_strStatus;
	CString	m_strResult;

;	int		dwStartOk;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCOMTOOLDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	//{{AFX_MSG(CCOMTOOLDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnAbout();
	afx_msg void OnQuit();

	afx_msg void OnClearReceiveEdit();
	afx_msg LRESULT OnReceiveChar(UINT ch, LONG port);
	afx_msg void OnSend();
	afx_msg void OnTimer(UINT nIDEvent);

	afx_msg void OnButton2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
//	CString m_strSnEdit;
//	unsigned int nSN;
	int nFactoryID;
	int nPgID;
public:
	TESTCASE gv_testcase;
	CString  m_strTestStatus;
	afx_msg void OnFileSettings();
	afx_msg void OnMenuTestResultQuery();

public:
	CString m_strAdcSampleMaxValue;
	CString m_strAdcSampleMinValue;
	CString m_strRssiValue;
	CString m_strRssiValue2;

	void LoadSmallTestToolConfig(void);
	int AddNewSPgBoardItem(CString m_strSn);
	int UpdateTestResult(CString m_strSn, TESTCASE m_icase,CString m_strResult);
	int UpdateADCResult(CString m_strSn, CString m_strADC1, CString m_strADC2, CString m_strADC3, CString m_strRssi, CString m_strRssi2);

	void UpdateTestPic(BOOL ret);

	CComboBox	m_PortNO;
	CString	m_strPortNO;
	int  m_iPortNo;


	int OnOpenClosePort(int openflag); // openflag : (0,close;1,open)

	CStatic m_retPicture;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
