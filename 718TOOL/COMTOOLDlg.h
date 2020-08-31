
// COMTOOLDlg.h : 头文件
//

#include "SerialPort.h"	// Added by ClassView
#pragma once

#define TEST_STATE_COUNT_MAX 10
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
	TEST_SWTCH_STATE,
	TEST_SN=10,
	TEST_NET_CONFIG,
	TEST_TCC_CODE,
	TEST_WAIT_ALARM,
	TEST_ALARM,
	TEST_LORA_RF_1278,
	TEST_LORA_RF_1301,
	TEST_ADC_TEMP,
	TEST_LTE,
	TEST_AT_STATE,
	TEST_WAIT_RESET,
	TEST_SUCCEED_WAIT_NEXT,
	TEST_CASE_MAX
};

enum RECVCASE
{
	RECV_WAIT_CMD = 0,
	RECV_RECVICE_STR,
	RECV_EXEC_FUN,
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
	CString m_strCMD;
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
	TESTCASE pre_gv_testcase;
	RECVCASE gv_recvcase;
	CString  m_strTestStatus;
	afx_msg void OnFileSettings();
	afx_msg void OnMenuTestResultQuery();
	afx_msg void OnEnChangeEditSn();

public:

	void LoadSmallTestToolConfig(void);
	int AddNewSPgBoardItem(CString m_strSn);
	int UpdateTestResult(CString m_strSn, TESTCASE m_icase,CString m_strResult);
    int UpdateADCResult(CString m_strSn,CString m_strADC1,CString m_strADC2,CString m_strADC3);

	BOOL m_bATWConfig;
	CComboBox	m_PortNO;
	CString	m_strPortNO;
	int  m_iPortNo;
	int  m_retry_count;
	void OnOpenClosePort(BOOL bRfidPort,int openflag); // openflag : (0,close;1,open)
	int OnOpenClosePort(int openflag); // openflag : (0,close;1,open)
	CString m_strTCCCode;
	CString m_strUrl;
	CString m_strRate;
	CString m_strAlarm;
	CString m_strTCCShow;
	CString m_strAdcMax;
	CString m_strAdcMin;
	BOOL m_check1301;
	CString m_str1278rx;
	CString m_str1278tx;
	CString m_str1301rx;
	CString m_str1301tx;
};
