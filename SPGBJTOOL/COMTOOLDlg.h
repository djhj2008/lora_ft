
// COMTOOLDlg.h : 头文件
//

#include "SerialPort.h"	// Added by ClassView
#pragma once

#define TEST_STEP_COUNT_MAX 5
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
	TEST_START_SWTCH,
	TEST_SN=10,
	TEST_STEP,
	TEST_STEP_START,
	TEST_STEP_COUNT,
	TEST_RTC,
	TEST_ADC_CALI,
	TEST_ADC_SAMPLE,
	TEST_LORA_RXTX,
	TEST_ATW_STATE,
	TEST_ADC_SAMPLE_STATE,
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
	TCHAR HKey[20];
	TCHAR LKey[20];
	////////////////////////////////////////////////////////////
	CString strEndata;
	CString DevideHexChar(char HexChar);
	char CombineHexChar(char CharH,char CharL);
	void HexStringFilter(CString &str);
	CString ChangeCharstr2Hexstr(CString Charstr);
	int m_nReceiveBytes;
	int m_nSendBytes;
	CSerialPort m_SerialPort;
	CSerialPort m_RfidPort;
	CCOMTOOLDlg(CWnd* pParent = NULL);	// 标准构造函数
// Dialog Data
	//{{AFX_DATA(CCOMTOOLDlg)
	enum { IDD = IDD_COMTOOL_DIALOG };
	CButton	m_bNewLine;
	CButton	m_SendCtrl;

	CEdit	m_SendPeriodCtrl;
	CString	m_strSend;
	CString	m_strReceive;
	BOOL	m_bHexR;
	BOOL	m_bHexS;
	BOOL	m_bAutoSend;
	long	m_nSendPeriod;
	CString	m_strStatus;
	CString	m_strSendBytes;
	CString	m_strReceiveBytes;
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
	afx_msg void OnClearSendEdit();
	afx_msg void OnClearReceiveEdit();
	afx_msg void OnBAutoSend();
	afx_msg void OnClearCounter();
	afx_msg LRESULT OnReceiveChar(UINT ch, LONG port);
	afx_msg void OnSend();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBHexS();
	afx_msg void OnBHexR();
	afx_msg void OnButton2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CString m_strSnEdit;
	unsigned int nSN;
	int nFactoryID;
	int nPgID;
public:
	TESTCASE gv_testcase;
	CString  m_strTestStatus;
	afx_msg void OnFileSettings();
	afx_msg void OnMenuTestResultQuery();
	afx_msg void OnEnChangeEditSn();

public:
	CString m_strTestFreq;
	CString m_strWorkFreq;
	CString m_strAdcSampleMaxValue;
	CString m_strAdcSampleMinValue;
	CString	m_strTxrssi;
	CString m_strRxrssi;
	BOOL	m_checkStep;
	CString m_strAdcCaliValue;

	void LoadSmallTestToolConfig(void);
	afx_msg void OnEnChangeEdit2();
	int AddNewSPgBoardItem(CString m_strSn);
	int UpdateTestResult(CString m_strSn, TESTCASE m_icase,CString m_strResult);
    int UpdateADCResult(CString m_strSn,CString m_strADC1,CString m_strADC2,CString m_strADC3);

	BOOL m_bATWConfig;
	CComboBox	m_PortNO;
	CString	m_strPortNO;
	int  m_iPortNo;
	int  m_retry_count;
	void OnOpenClosePort(BOOL bRfidPort,int openflag); // openflag : (0,close;1,open)
};
