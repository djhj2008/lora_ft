
// COMTOOLDlg.h : ͷ�ļ�
//

#include "SerialPort.h"	// Added by ClassView
#include "ChatSocket.h"
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
	TEST_RFID_WRITE_SN=1,
	TEST_RFID_VIRIFY_SN,
	TEST_RFID_FOREVER_READ_SN,
	TEST_RFID_VIRIFY_SN_OVER,
	TEST_RFID_ONCE_READ_SN,
	TEST_RFID_READ_USER,
	TEST_RFID_WRITE_USER,
	TEST_RFID_USER_OVER,
	TEST_CASE_MAX=0xFF,
};

// CCOMTOOLDlg �Ի���
class CCOMTOOLDlg : public CDialogEx
{
// ����
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
	CSerialPort m_RfidPort;
	CCOMTOOLDlg(CWnd* pParent = NULL);	// ��׼���캯��
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
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBHexS();
	afx_msg void OnBHexR();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CString m_strSnEdit;
	unsigned int nSN;
	int nFactoryID;
	int nPgID;
public:
	TESTCASE gv_testcase;
	TESTCASE gv_testNextcase;
	CString  m_strTestStatus;
	afx_msg void OnFileSettings();
	afx_msg void OnMenuTestResultQuery();
	afx_msg void OnEnChangeEditSn();

public:
	CString m_strTCCCode;
	CString m_strFYCMin;
	CString m_strFYMMax;
	CString m_strPsn;
	CChatSocket* m_pSocket;

	void LoadSmallTestToolConfig(void);
	afx_msg void OnEnChangeEdit2();
	int AddNewSPgBoardItem(CString m_strSn,bool flag);
	int UpdateTestResult(CString m_strSn, TESTCASE m_icase,CString m_strResult);
	int UpdateFYMResult(CString m_strSn, CString m_strFYCode);
	void UpdateTestPic(int ret);

	CComboBox	m_PortNO;
	int  m_iPortNo;

	CComboBox m_RfidPortNO;
	CString m_strRfidPortNO;
	CStatic m_retPicture;

	int  m_iRfPortNo;
	int  m_WriteCounter;
	void OnOpenClosePort(int openflag); // openflag : (0,close;1,open)
	afx_msg void OnBnClickedRfidRead();
    void ProcessPendingRead();
    void ProcessSocketClose();
	afx_msg void OnBnUserRead();
	afx_msg void OnBnUserWrite();
	afx_msg void OnBnClickedSend();
	CString m_strSnRead;
	afx_msg void OnmenuSetting();
};
