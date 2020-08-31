
// COMTOOLDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "COMTOOL.h"
#include "COMTOOLDlg.h"
#include "afxdialogex.h"

#include "TestSettingsDlg.h"
#include "TestResultQueryDlg.h"
#include "Paint_ADO.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define EditBoxWidth 50

int BaudRate[]={300,600,1200,2400,4800,9600,14400,19200,38400,56000,57600,115200,230400,460800,921600};
int ParitySelNum=5;
CString Parity[]={"None","Odd","Even","Mark","Space"};
int DataBits[]={5,6,7,8};
int StopBits[]={1,2};
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCOMTOOLDlg 对话框




CCOMTOOLDlg::CCOMTOOLDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCOMTOOLDlg::IDD, pParent)
	, m_strSnEdit(_T(""))
	, m_strTCCCode(_T(""))
	, m_strTCCShow(_T(""))
{
	//{{AFX_DATA_INIT(CCOMTOOLDlg)
	m_strSend = "";
	m_strReceive = "";
	m_bHexR = FALSE;
	m_bHexS = FALSE;
	m_bAutoSend = TRUE;
	m_nSendPeriod = 1000;
	m_strStatus = "关闭";
	m_strSendBytes = "0";
	m_strReceiveBytes = "0";
	m_strPortNO = "";
	m_nSendBytes=0;
	m_nReceiveBytes=0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	gv_testcase = TEST_CASE_MAX;
	License_max_value=0;
	License_current_value=0;
	License_current_backup_value=0;

	memset(HKey,0,sizeof(HKey));
	memset(LKey,0,sizeof(LKey));
    HKey[0] = '1';
    HKey[1] = '2';
    HKey[2] = '3';
    HKey[3] = '4';
    
	LKey[0] = '8';
	LKey[1] = '7';
	LKey[2] = '6';
	LKey[3] = '5';
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCOMTOOLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCOMTOOLDlg)
	DDX_Control(pDX, IDC_COM, m_PortNO);
	DDX_CBString(pDX, IDC_COM, m_strPortNO);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_SN, m_strSnEdit);
	DDV_MaxChars(pDX, m_strSnEdit, 10);
	DDX_Text(pDX, IDC_EDIT_TCC_SHOW, m_strTCCShow);
}

BEGIN_MESSAGE_MAP(CCOMTOOLDlg, CDialogEx)
	//{{AFX_MSG_MAP(CCOMTOOLDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ABOUT, OnAbout)
	ON_BN_CLICKED(IDC_QUIT, OnQuit)
	ON_BN_CLICKED(IDC_ClearS, OnClearSendEdit)
	ON_BN_CLICKED(IDC_ClearR, OnClearReceiveEdit)
	ON_BN_CLICKED(IDC_BAutoSend, OnBAutoSend)
	ON_BN_CLICKED(IDC_ClearCounter, OnClearCounter)
	ON_MESSAGE(WM_COMM_RXCHAR,OnReceiveChar)
	ON_BN_CLICKED(IDC_Send, OnSend)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BHexS, OnBHexS)
	ON_BN_CLICKED(IDC_BHexR, OnBHexR)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_SETTINGS, &CCOMTOOLDlg::OnFileSettings)
	ON_COMMAND(ID_Menu_TEST_RESULT_QUERY, &CCOMTOOLDlg::OnMenuTestResultQuery)
	ON_EN_CHANGE(IDC_EDIT_SN, &CCOMTOOLDlg::OnEnChangeEditSn)
END_MESSAGE_MAP()


// CCOMTOOLDlg 消息处理程序

BOOL CCOMTOOLDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

    CString Tmp,Tmp1;
	HKey[4] = '5';
    HKey[5] = '6';
    HKey[6] = '7';
    HKey[7] = '8';
    
	LKey[4] = '4';
	LKey[5] = '3';
	LKey[6] = '2';
	LKey[7] = '1';

    CString title = "718生产工具(v111)";        
    SetWindowText(title);

	CString temp;				
	//显示串口设置
	for(int i=1;i<=MaxSerialPortNum-1;i++)
	{
		if(m_SerialPort.InitPort(this,i))
		{
			temp.Format("COM%d",i);		
			m_PortNO.AddString((LPCTSTR)temp);
		}
	}
	if(m_PortNO.GetCount())
	{
		m_SerialPort.InitPort(this,MaxSerialPortNum);
		m_PortNO.SetCurSel(0);
	}

	//加载板级默认配置
	LoadSmallTestToolConfig();

	m_strTCCShow = m_strTCCCode;
	GetDlgItem(IDC_EDIT_TCC_SHOW)->SetWindowText(m_strTCCShow);
	GetDlgItem(IDC_EDIT_SN)->SetFocus(); //.SendMessage(WM_KEYDOWN, VK_END, 0);
	return FALSE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCOMTOOLDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCOMTOOLDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCOMTOOLDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCOMTOOLDlg::OnAbout() 
{
	// TODO: Add your control notification handler code here
	CAboutDlg dlg;
	dlg.DoModal();
}

void CCOMTOOLDlg::OnQuit() 
{
	// TODO: Add your control notification handler code here
	CString temp;
	UpdateData(true);
	OnOpenClosePort(TRUE,FALSE);
	OnOpenClosePort(FALSE,FALSE);
	Sleep(1000);
	PostQuitMessage(0);
}

void CCOMTOOLDlg::OnClearSendEdit() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	m_strSend="";
	UpdateData(false);
}

void CCOMTOOLDlg::OnClearReceiveEdit() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	m_strReceive="";
	UpdateData(false);
}

void CCOMTOOLDlg::OnBAutoSend() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	m_SendPeriodCtrl.EnableWindow(m_bAutoSend);
}

void CCOMTOOLDlg::OnClearCounter() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	m_nSendBytes=0;
	m_nReceiveBytes=0;
	m_strSendBytes="0";
	m_strReceiveBytes="0";
	UpdateData(false);
}

char sendbuf[1024];
void CCOMTOOLDlg::OnSend() 
{
	// TODO: Add your control notification handler code here
	CString strsend;
	UpdateData(true);
	if(m_strStatus=="关闭")
	{
		AfxMessageBox("请首先打开串口");
		return;
	}


    GetDlgItem(IDC_EDIT1)->GetWindowText(strsend);
	if(m_bNewLine.GetCheck())
	{
		strsend+="\r\n";
	}

	m_SerialPort.WriteToPort(strsend.GetBuffer(0),strsend.GetLength());
	
	return;
#if 0
	KillTimer(1);
	SetTimer(1,m_nSendPeriod,NULL);
    //m_SendCtrl.EnableWindow(FALSE);
#endif

}

#define PRESET_VALUE 0xFFFF
#define POLYNOMIAL  0x8408
unsigned int uiCrc16Cal(unsigned char const  * pucY, unsigned char ucX)
{
	unsigned char ucI, ucJ;
	unsigned short int  uiCrcValue = PRESET_VALUE;

	for (ucI = 0; ucI < ucX; ucI++)
	{
		uiCrcValue = uiCrcValue ^ *(pucY + ucI);
		for (ucJ = 0; ucJ < 8; ucJ++)
		{
			if (uiCrcValue & 0x0001)
			{
				uiCrcValue = (uiCrcValue >> 1) ^ POLYNOMIAL;
			}
			else
			{
				uiCrcValue = (uiCrcValue >> 1);
			}
		}
	}
	return uiCrcValue;
}

///////////////////////////////////////////////////////////////////////////

BYTE gv_recstatus=0;   // 0 不能接收数据，1接收数据
char strRevBuf[1024];
int  g_iRevCounter=0;

void CCOMTOOLDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CString strsend;
    BYTE readlen = 0;
	CString strTmp,Tmp1;
	gv_recstatus = 0;	
	UpdateData(true);
    KillTimer(nIDEvent);
	switch(nIDEvent)
	{
	case TEST_START_SWTCH:
		if (m_retry_count < TEST_STATE_COUNT_MAX){
			OnOpenClosePort(FALSE, TRUE);
			strsend = "ATSTATE=?\r\n";
			m_SerialPort.WriteToPort(strsend.GetBuffer(0), strsend.GetLength());
			m_retry_count++;
			strTmp.Format("查询状态:%d次 \r\n", m_retry_count);
			m_strTestStatus = m_strTestStatus + strTmp;
			GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
			SetTimer(gv_testcase, 5 * 1000, NULL);
		}else{
			gv_testcase = TEST_SUCCEED_WAIT_NEXT;
			GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(m_strTestStatus);
			strTmp = "STATE 状态失败\r\n";
			m_strTestStatus = m_strTestStatus + strTmp;
			GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
			GetDlgItem(IDC_EDIT_SN)->EnableWindow(1);
			GetDlgItem(IDC_EDIT_SN)->SetWindowText("");
		}
		break;
	case TEST_SWTCH_STATE:
		gv_testcase = TEST_SUCCEED_WAIT_NEXT;
			GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(m_strTestStatus);
			strTmp = "STATE 状态失败\r\n";
			m_strTestStatus = m_strTestStatus + strTmp;
			GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
			GetDlgItem(IDC_EDIT_SN)->EnableWindow(1);
			GetDlgItem(IDC_EDIT_SN)->SetWindowText("");
			break;
	case TEST_SN:
		gv_testcase = TEST_SUCCEED_WAIT_NEXT;
			GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(m_strTestStatus);
			strTmp = "SN:"+m_strSnEdit+" 写失败\r\n";
			m_strTestStatus = m_strTestStatus+strTmp;
			GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
			GetDlgItem(IDC_EDIT_SN)->EnableWindow(1);
			GetDlgItem(IDC_EDIT_SN)->SetWindowText("");
			break;
	case TEST_WAIT_ALARM:
			//OnOpenClosePort(FALSE, TRUE);
			gv_testcase = TEST_ALARM;
			break;
	case TEST_ALARM:
		gv_testcase = TEST_SUCCEED_WAIT_NEXT;
			GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(m_strTestStatus);
			strTmp = "ALARM:" + m_strAlarm + " 失败\r\n";
			m_strTestStatus = m_strTestStatus + strTmp;
			GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
			GetDlgItem(IDC_EDIT_SN)->EnableWindow(1);
			GetDlgItem(IDC_EDIT_SN)->SetWindowText("");
			break;
	case TEST_LORA_RF_1278:
		gv_testcase = TEST_SUCCEED_WAIT_NEXT;
			GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(m_strTestStatus);
			strTmp = "1278 RF:失败\r\n";
			m_strTestStatus = m_strTestStatus + strTmp;
			GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
			GetDlgItem(IDC_EDIT_SN)->EnableWindow(1);
			GetDlgItem(IDC_EDIT_SN)->SetWindowText("");
			break;
	case TEST_LORA_RF_1301:
		gv_testcase = TEST_SUCCEED_WAIT_NEXT;
			GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(m_strTestStatus);
			strTmp = "1301 RF:失败\r\n";
			m_strTestStatus = m_strTestStatus + strTmp;
			GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
			GetDlgItem(IDC_EDIT_SN)->EnableWindow(1);
			GetDlgItem(IDC_EDIT_SN)->SetWindowText("");
			break;
	case TEST_ADC_TEMP:
			gv_testcase = TEST_SUCCEED_WAIT_NEXT;
			GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(m_strTestStatus);
			strTmp = "ADC TEMP 失败\r\n";
			m_strTestStatus = m_strTestStatus + strTmp;
			GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
			GetDlgItem(IDC_EDIT_SN)->EnableWindow(1);
			GetDlgItem(IDC_EDIT_SN)->SetWindowText("");
			break;
	case TEST_LTE:
			gv_testcase = TEST_SUCCEED_WAIT_NEXT;
			GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(m_strTestStatus);
			strTmp = "LTE 失败\r\n";
			m_strTestStatus = m_strTestStatus + strTmp;
			GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
			GetDlgItem(IDC_EDIT_SN)->EnableWindow(1);
			GetDlgItem(IDC_EDIT_SN)->SetWindowText("");
			break;
	case TEST_AT_STATE:
			if (m_retry_count < TEST_STATE_COUNT_MAX){
				strsend = "ATSTATE=1\r\n";
				m_retry_count++;
				m_SerialPort.WriteToPort(strsend.GetBuffer(0), strsend.GetLength());
				strTmp.Format("设置状态:%d次 \r\n", m_retry_count);
				m_strTestStatus = m_strTestStatus + strTmp;
				GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
				SetTimer(gv_testcase, 5 * 1000, NULL);
			}
			else{
				gv_testcase = TEST_SUCCEED_WAIT_NEXT;
				GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(m_strTestStatus);
				strTmp = "STATE 设置状态失败\r\n";
				m_strTestStatus = m_strTestStatus + strTmp;
				GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
				GetDlgItem(IDC_EDIT_SN)->EnableWindow(1);
				GetDlgItem(IDC_EDIT_SN)->SetWindowText("");
			}
			break;
	case TEST_WAIT_RESET:
			gv_testcase = TEST_SUCCEED_WAIT_NEXT;
			GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(m_strTestStatus);
			strTmp = "STATE 失败\r\n";
			m_strTestStatus = m_strTestStatus + strTmp;
			GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
			GetDlgItem(IDC_EDIT_SN)->EnableWindow(1);
			GetDlgItem(IDC_EDIT_SN)->SetWindowText("");
			break;
	case TEST_SUCCEED_WAIT_NEXT:
			KillTimer(nIDEvent);
			OnOpenClosePort(FALSE,FALSE);
			OnOpenClosePort(TRUE,FALSE);
			GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(m_strTestStatus);
			strTmp = "测试成功,\r\n等待扫描新设备.\r\n";
			m_strTestStatus = m_strTestStatus + strTmp;
			GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
			GetDlgItem(IDC_EDIT_SN)->EnableWindow(1);
			GetDlgItem(IDC_EDIT_SN)->SetWindowText("");
			break;
	default:
			break;
	}
    //GetDlgItem(IDC_EDIT_SN)->EnableWindow(1);
	GetDlgItem(IDC_EDIT_SN)->SetFocus();
	UpdateData(false);
	CDialog::OnTimer(nIDEvent);
}
char gv_Label[13];

char * formatSn(int sn)
{
	static char snbuf[20]={0};
	char tmpbuf[10];
	int factoryID;
	int pgID;
	char factoryidbuf[5+1]={0};
	char pgidbuf[4+1]={0};

	pgID = sn & 0x1fff;
	factoryID = (sn>>13)&0x7ffff;

	memcpy(factoryidbuf,"00000",5);
	memset(tmpbuf,0,sizeof(tmpbuf));
	sprintf(tmpbuf, "%d",factoryID);
	memcpy(factoryidbuf+(5-strlen(tmpbuf)),tmpbuf,strlen(tmpbuf));

	memcpy(pgidbuf,"0000",4);
	memset(tmpbuf,0,sizeof(tmpbuf));
	sprintf(tmpbuf, "%d",pgID);
	memcpy(pgidbuf+(4-strlen(tmpbuf)),tmpbuf,strlen(tmpbuf));

	memset(snbuf,0,sizeof(snbuf));
	sprintf(snbuf, "%s%s",factoryidbuf,pgidbuf);
	return snbuf;
}
LRESULT CCOMTOOLDlg::OnReceiveChar(UINT ch, LONG port)
{
	int pos1=-1,pos2=-1,pos3=-1,pos4=-1,tx_rssi,rx_rssi;
	CString strsend,strTmp,str_tx_rssi,str_rx_rssi;
	char tmpbuf[128];
    GetDlgItem(IDC_EDIT2)->GetWindowText(m_strReceive);
	{
		m_strReceive += (char)ch;
	}
    GetDlgItem(IDC_EDIT2)->SetWindowText(m_strReceive);
	// here handle recieve data
	switch(gv_testcase)
	{
	case TEST_START_SWTCH:
		if (m_strReceive.Find("+MJSTAT:") >= 0){
			pos1 = m_strReceive.Find("+MJSTAT:", 0);
			if (pos1 >= 0){
				pos2 = m_strReceive.Find("\r\n", pos1 + 8);
				if (pos2 > 0){
					CString at_state = m_strReceive.Mid(pos1 + 8, pos2 - (pos1 + 8));
					int state = atoi(at_state);
					m_strReceive = "";
					KillTimer(gv_testcase);
					strTmp.Format("STATE:%d\r\n", state);
					m_strTestStatus = m_strTestStatus + strTmp;
					GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
					GetDlgItem(IDC_EDIT2)->SetWindowText("");
					if (state == 0){
						gv_testcase = TEST_SN;
						strsend = "ATESETSN=" + m_strSnEdit + "\r\n";
						m_SerialPort.WriteToPort(strsend.GetBuffer(0), strsend.GetLength());
						SetTimer(gv_testcase, 5 * 1000, NULL);
					}
					else{
						gv_testcase = TEST_SWTCH_STATE;
						strsend = "ATSTATE=0\r\n";
						m_SerialPort.WriteToPort(strsend.GetBuffer(0), strsend.GetLength());
						SetTimer(gv_testcase, 5 * 1000, NULL);
					}
				}
			}
		}
		break;
	case TEST_SWTCH_STATE:
		if (m_strReceive.Find("+STATOK") >= 0){
			m_strReceive = "";
			GetDlgItem(IDC_EDIT2)->SetWindowText(m_strReceive);
			KillTimer(gv_testcase);
			GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(m_strTestStatus);

			strTmp = "STATE:0 设置成功\r\n";
			m_strTestStatus = m_strTestStatus + strTmp;
			GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
			GetDlgItem(IDC_EDIT2)->SetWindowText("");

			gv_testcase = TEST_START_SWTCH;
			m_retry_count = 0;
			SetTimer(gv_testcase, 10 * 1000, NULL);
		}
		break;
	case TEST_SN:
		//AT+SNOK
		if(m_strReceive.Find("+SNOK")>=0)
		{
		   m_strReceive= "";
		   GetDlgItem(IDC_EDIT2)->SetWindowText(m_strReceive);
		   KillTimer(gv_testcase);
           GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(m_strTestStatus);

		   UpdateTestResult(m_strSnEdit, TEST_SN, m_strSnEdit);
		   strTmp = "SN:"+m_strSnEdit+"写成功\r\n";
		   m_strTestStatus = m_strTestStatus+strTmp;
           GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
           GetDlgItem(IDC_EDIT2)->SetWindowText("");

		   gv_testcase = TEST_NET_CONFIG;
		   strsend.Format("ATNETCONFIG=%s\r\n", m_strUrl);
		   m_SerialPort.WriteToPort(strsend.GetBuffer(0), strsend.GetLength());
		   SetTimer(gv_testcase, 5 * 1000, NULL);
		}
		break;
	case TEST_NET_CONFIG:
		if (m_strReceive.Find("+NETOK") >= 0)
		{
			m_strReceive = "";
			GetDlgItem(IDC_EDIT2)->SetWindowText(m_strReceive);
			KillTimer(gv_testcase);
			GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(m_strTestStatus);

			UpdateTestResult(m_strSnEdit, TEST_NET_CONFIG, "1");
			strTmp = "URL:" + m_strUrl + "写成功\r\n";
			m_strTestStatus = m_strTestStatus + strTmp;
			GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
			GetDlgItem(IDC_EDIT2)->SetWindowText("");
			gv_testcase = TEST_TCC_CODE;
			strsend.Format("ATESETTCC=%s\r\n", m_strTCCCode);

			m_SerialPort.WriteToPort(strsend.GetBuffer(0), strsend.GetLength());
			SetTimer(gv_testcase, 5 * 1000, NULL);
		}

		break;
	case TEST_TCC_CODE:
		if (m_strReceive.Find("+TCCOK") >= 0)
		{
			m_strReceive = "";
			GetDlgItem(IDC_EDIT2)->SetWindowText(m_strReceive);
			KillTimer(gv_testcase);
			GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(m_strTestStatus);

			UpdateTestResult(m_strSnEdit, TEST_TCC_CODE, "1");
			strTmp = "TCC:" + m_strTCCCode + "写成功\r\n";
			m_strTestStatus = m_strTestStatus + strTmp;
			GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
			GetDlgItem(IDC_EDIT2)->SetWindowText("");
			gv_testcase = TEST_WAIT_ALARM;
			strsend.Format("ATALARM=%s\r\n", m_strAlarm);
			strTmp = "ALARM:" + m_strAlarm + "等待唤醒.\r\n";
			m_strTestStatus = m_strTestStatus + strTmp;
			GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
			m_SerialPort.WriteToPort(strsend.GetBuffer(0), strsend.GetLength());
			SetTimer(gv_testcase, 10 * 1000, NULL);
		}
		break;
	case TEST_ALARM:
		if (m_strReceive.Find("+ALAOK") >= 0)
		{
			m_strReceive = "";
			GetDlgItem(IDC_EDIT2)->SetWindowText(m_strReceive);
			KillTimer(gv_testcase);
			GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(m_strTestStatus);

			UpdateTestResult(m_strSnEdit, TEST_ALARM, "1");
			strTmp = "ALARM:" + m_strUrl + "设置成功\r\n";
			m_strTestStatus = m_strTestStatus + strTmp;
			GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
			GetDlgItem(IDC_EDIT2)->SetWindowText("");
			gv_testcase = TEST_LORA_RF_1278;
			strsend.Format("AT1278RF=%s\r\n", m_strRate);
			m_SerialPort.WriteToPort(strsend.GetBuffer(0), strsend.GetLength());

			strTmp = "RF1278:" + m_strRate + "开始测试.\r\n";
			m_strTestStatus = m_strTestStatus + strTmp;
			GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);

			SetTimer(gv_testcase, 5 * 1000, NULL);
		}
		break;
	case TEST_LORA_RF_1278:
		if (m_strReceive.Find("AT+RFOK 1278 RSSI") >= 0)
		{
			pos1 = m_strReceive.Find("AT+RFOK 1278 RSSI TX:", 0);
			if (pos1 >= 0){
				pos2 = m_strReceive.Find("RX:", pos1 + 21);
				if (pos2 > 0){
					pos3 = m_strReceive.Find("\r\n", pos2 + 3);
					if (pos3 >= 0){
						CString tx_1278 = m_strReceive.Mid(pos1 + 21, pos2 - (pos1 + 21));
						CString rx_1278 = m_strReceive.Mid(pos2 + 3, pos3 - (pos2 + 3));

						int i_tx1278 = atoi(tx_1278);
						int i_rx1278 = atoi(rx_1278);

						m_strReceive = "";
						GetDlgItem(IDC_EDIT2)->SetWindowText(m_strReceive);
						KillTimer(gv_testcase);
						GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(m_strTestStatus);

						if (i_tx1278 >= atoi(m_str1278tx) && i_rx1278 >= atoi(m_str1278rx)){
							UpdateTestResult(m_strSnEdit, TEST_LORA_RF_1278, "1");
							strTmp = "RF1278:TX:" + tx_1278 + " RX:" + rx_1278 + "测试成功\r\n";
							m_strTestStatus = m_strTestStatus + strTmp;
							GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
							GetDlgItem(IDC_EDIT2)->SetWindowText("");

							if (m_check1301){
								gv_testcase = TEST_LORA_RF_1301;
								strsend.Format("AT1301RF=%s\r\n", m_strRate);
								m_SerialPort.WriteToPort(strsend.GetBuffer(0), strsend.GetLength());

								strTmp = "RF1301:" + m_strRate + "开始测试.\r\n";
								m_strTestStatus = m_strTestStatus + strTmp;
								GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
							}
							else{
								gv_testcase = TEST_ADC_TEMP;
								strsend = "ATLTEADC=?\r\n";
								m_SerialPort.WriteToPort(strsend.GetBuffer(0), strsend.GetLength());

								strTmp = "ADC TEMP:" + m_strRate + "开始测试.\r\n";
								m_strTestStatus = m_strTestStatus + strTmp;
								GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
							}

							SetTimer(gv_testcase, 15 * 1000, NULL);
						}
						else{
							UpdateTestResult(m_strSnEdit, TEST_LORA_RF_1278, "2");
							strTmp = "RF1278:TX:" + tx_1278 + " RX:" + rx_1278 + "超出范围\r\n";
							m_strTestStatus = m_strTestStatus + strTmp;
							GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
							GetDlgItem(IDC_EDIT2)->SetWindowText("");
							SetTimer(gv_testcase, 1 * 1000, NULL);
						}

					}
				}

			}
		}
		else  if(m_strReceive.Find("+R1278ERR") >= 0){
			SetTimer(gv_testcase, 1 * 1000, NULL);
		}
		break;
	case TEST_LORA_RF_1301:
		if (m_strReceive.Find("AT+RFOK 1301 RSSI") >= 0)
		{
			pos1 = m_strReceive.Find("AT+RFOK 1301 RSSI TX:", 0);
			if (pos1 >= 0){
				pos2 = m_strReceive.Find("RX:", pos1 + 21);
				if (pos2 > 0){
					pos3 = m_strReceive.Find("index", pos2 + 3);
					if (pos3 >= 0){
						CString tx_1301 = m_strReceive.Mid(pos1 + 21, pos2 - (pos1 + 21));
						CString rx_1301 = m_strReceive.Mid(pos2 + 3, pos3 - (pos2 + 3));

						int i_tx1301 = atoi(tx_1301);
						int i_rx1301 = atoi(rx_1301);

						m_strReceive = "";
						GetDlgItem(IDC_EDIT2)->SetWindowText(m_strReceive);
						KillTimer(gv_testcase);
						GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(m_strTestStatus);


						if (i_tx1301 >= atoi(m_str1301tx) && i_rx1301 >= atoi(m_str1301rx)){
							UpdateTestResult(m_strSnEdit, TEST_LORA_RF_1301, "1");
							strTmp = "RF1301:TX:" + tx_1301 + " RX:" + rx_1301 + "测试成功\r\n";
							m_strTestStatus = m_strTestStatus + strTmp;
							GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
							GetDlgItem(IDC_EDIT2)->SetWindowText("");

							gv_testcase = TEST_ADC_TEMP;
							strsend = "ATLTEADC=?\r\n";
							m_SerialPort.WriteToPort(strsend.GetBuffer(0), strsend.GetLength());

							strTmp = "ADC TEMP:" + m_strRate + "开始测试.\r\n";
							m_strTestStatus = m_strTestStatus + strTmp;
							GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);

							SetTimer(gv_testcase, 5 * 1000, NULL);
						}
						else{
							UpdateTestResult(m_strSnEdit, TEST_LORA_RF_1301, "2");
							strTmp = "RF1301:TX:" + tx_1301 + " RX:" + rx_1301 + "超出范围\r\n";
							m_strTestStatus = m_strTestStatus + strTmp;
							GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
							GetDlgItem(IDC_EDIT2)->SetWindowText("");
							SetTimer(gv_testcase, 1 * 1000, NULL);
						}
					}
				}
			}
		}
		else  if (m_strReceive.Find("+R1301ERR") >= 0){
			SetTimer(gv_testcase, 1 * 1000, NULL);
		}
		break;
	case TEST_ADC_TEMP:
		if (m_strReceive.Find("+MJADC:") >= 0){
			pos1 = m_strReceive.Find("+MJADC:", 0);
			if (pos1 >= 0){
				pos2 = m_strReceive.Find("\r\n", pos1 + 7);
				if (pos2 > 0){
					CString adc_str = m_strReceive.Mid(pos1 + 7, pos2 - (pos1 + 7));
					int adc_int = atoi(adc_str);
					m_strReceive = "";

					GetDlgItem(IDC_EDIT2)->SetWindowText(m_strReceive);
					KillTimer(gv_testcase);
					GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(m_strTestStatus);
					UpdateTestResult(m_strSnEdit, TEST_ADC_TEMP, adc_str);
					strTmp.Format("ADC TEMP:%d\r\n", adc_int);
					m_strTestStatus = m_strTestStatus + strTmp;
					GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
					GetDlgItem(IDC_EDIT2)->SetWindowText("");

					if (adc_int<atoi(m_strAdcMin) || adc_int>atoi(m_strAdcMax)){
						
					}
					else{
						gv_testcase = TEST_LTE;
						strsend = "ATLTEDATA=1\r\n";
						m_SerialPort.WriteToPort(strsend.GetBuffer(0), strsend.GetLength());

						strTmp = "LTE:" + m_strRate + "开始测试.\r\n";
						m_strTestStatus = m_strTestStatus + strTmp;
						GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);

						//gv_testcase = TEST_AT_STATE;
						//strsend = "ATSTATE=1\r\n";
						//m_SerialPort.WriteToPort(strsend.GetBuffer(0), strsend.GetLength());

						//strTmp = "设置AT模式:1\r\n";
						//m_strTestStatus = m_strTestStatus + strTmp;
						//GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
					}
					SetTimer(gv_testcase, 20 * 1000, NULL);
				}
			}
		}
		break;
	case TEST_LTE:
		if (m_strReceive.Find("+LTEOK") >= 0){
			m_strReceive = "";
			GetDlgItem(IDC_EDIT2)->SetWindowText(m_strReceive);
			KillTimer(gv_testcase);
			GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(m_strTestStatus);

			UpdateTestResult(m_strSnEdit, TEST_LTE, "1");
			UpdateTestResult(m_strSnEdit, TEST_SUCCEED_WAIT_NEXT, "1");
			strTmp = "LTE:" + m_strUrl + "测试成功\r\n";
			m_strTestStatus = m_strTestStatus + strTmp;
			GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
			GetDlgItem(IDC_EDIT2)->SetWindowText("");

			gv_testcase = TEST_SUCCEED_WAIT_NEXT;
			//strsend = "ATSTATE=1\r\n";
			//m_SerialPort.WriteToPort(strsend.GetBuffer(0), strsend.GetLength());

			//m_retry_count=0;
			//strTmp = "设置AT模式:1\r\n";
			//m_strTestStatus = m_strTestStatus + strTmp;
			//GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
			SetTimer(gv_testcase, 2 * 1000, NULL);
		}
		break;
	case TEST_AT_STATE:
	case TEST_WAIT_RESET:
		if (m_strReceive.Find("+STATOK") >= 0){
			m_strReceive = "";
			GetDlgItem(IDC_EDIT2)->SetWindowText(m_strReceive);
			KillTimer(gv_testcase);
			GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(m_strTestStatus);

			strTmp = "STATE:1 测试成功\r\n";
			m_strTestStatus = m_strTestStatus + strTmp;
			GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
			GetDlgItem(IDC_EDIT2)->SetWindowText("");

			gv_testcase = TEST_SUCCEED_WAIT_NEXT;
			SetTimer(gv_testcase, 5 * 1000, NULL);
		}
		break;
		/*
	case TEST_SUCCEED_WAIT_NEXT:
	    if(m_strReceive.GetLength()>1024)
	    {
		   m_strReceive= "";
		   GetDlgItem(IDC_EDIT2)->SetWindowText(m_strReceive);
	    }
		break;
		*/
	default:
		break;
	}
	return 0;
}

CString CCOMTOOLDlg::ChangeCharstr2Hexstr(CString Charstr)
{
	CString Hexstr="";
	Charstr.MakeUpper();
	HexStringFilter(Charstr);
	int Length=Charstr.GetLength();
	if(Length%2)
		Charstr.Delete(Length-1);
	Length=Charstr.GetLength();
	for(int i=0;i<Length/2;i++)
	{
		Hexstr+=CombineHexChar(Charstr.GetAt(i*2),Charstr.GetAt(i*2+1));
	}
	return Hexstr;
}

void CCOMTOOLDlg::OnBHexS() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
}

void CCOMTOOLDlg::OnBHexR() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
}

void CCOMTOOLDlg::HexStringFilter(CString &str)
{
	BOOL bOK;
	for(int i=0;i<str.GetLength();)
	{
		bOK=((str.GetAt(i)>='0')&&(str.GetAt(i)<='9'))||
			((str.GetAt(i)>='A')&&(str.GetAt(i)<='F'))||
			((str.GetAt(i)>='a')&&(str.GetAt(i)<='f'));
		if(!bOK)
			str.Delete(i);
		else i++;	
	}
}
char CCOMTOOLDlg::CombineHexChar(char CharH,char CharL)
{
	char result;
	CString temp;
	if(CharH>='0'&&CharH<='9')			result=(CharH-'0');
	else if(CharH>='a'&&CharH<='f')		result=(CharH-'a'+10);
	else if(CharH>='A'&&CharH<='F')		result=(CharH-'A'+10);
	else								result=0;
	result<<=4;	
	if(CharL>='0'&&CharL<='9')			result+=(CharL-'0');
	else if(CharL>='a'&&CharL<='f')		result+=(CharL-'a'+10);
	else if(CharL>='A'&&CharL<='F')		result+=(CharL-'A'+10);
	else								result+=0;
	return result;
}


CString CCOMTOOLDlg::DevideHexChar(char HexChar)
{
	CString result="";
	int temp=(HexChar&0xF0)>>4;
	if(temp<10)
		result+=(char)(temp+'0');
	else 
		result+=(char)(temp+'A'-10);
	temp=HexChar&0x0F;
	if(temp<10)
		result+=(char)(temp+'0');
	else 
		result+=(char)(temp+'A'-10);

	return result;
}



void CCOMTOOLDlg::OnButton2() 
{
	// TODO: Add your control notification handler code here
	m_strReceive="";
    GetDlgItem(IDC_EDIT2)->SetWindowText(m_strReceive);	
}

void CCOMTOOLDlg::OnFileSettings()
{
	// TODO: Add your command handler code here
	CTestSettingsDlg dlg;
	dlg.m_strTCCCode = m_strTCCCode;
	dlg.m_strUrl = m_strUrl;
	dlg.m_strRate = m_strRate;
	dlg.m_strAlarm = m_strAlarm;
	dlg.m_strAdcMax = m_strAdcMax;
	dlg.m_strAdcMin = m_strAdcMin;
	dlg.m_check1301 = m_check1301;
	dlg.m_str1278rx = m_str1278rx;
	dlg.m_str1278tx = m_str1278tx;
	dlg.m_str1301rx = m_str1301rx;
	dlg.m_str1301tx = m_str1301tx;

	dlg.DoModal();
    LoadSmallTestToolConfig();
}


void CCOMTOOLDlg::OnMenuTestResultQuery()
{
	// TODO: Add your command handler code here
	int mm=0;
	CTestResultQueryDlg dlg;
	dlg.DoModal();
}

/*
小板板级测试流程:
盒子激光电子标签和二维码同时存在，当二维码丢失时，需要根据激光电子标签重新打印二维码标签
1. 扫描二维码标签,输入SN，写入SN
2. LORA测试收发:小板发送数据到测试基站，基站收到后再把接收到的数据发送回来。如果小板发送数据和接收到的数据一致，为成功。（只能测试功能一个，不能测试信号性能)
3. 测试ADC,ADC为一个固定值的电阻，读出来的值应该为一个定值（变化幅度很小范围）才能算通过。（需要设置范围参考）
4. RTC测试，设置闹钟，关机后能起来为成功
5. 关机电流(TBD,需要程控电源可编程，有API函数可以控制)
6. 电压检测，比较开关(TBD,需要程控电源可编程，有API函数可以控制)
7. 以上测试都成功后，写入状态ATW,进入校准状态。写入状态ATW后，打超声波，然后可以放入水中恒温箱中进入组装校准过程
*/
/*
2018.12.12
1. 测试ADC值3组
2. 写默认值3组
3. 写默认工作频率
4. 测试完成后，进入采样状态(校准过程省略掉，使用默认值)
*/
void CCOMTOOLDlg::OnEnChangeEditSn()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CString strsend;
	CString temp;
    UpdateData(true);
	if(m_strSnEdit.GetLength()<9)
	{
		return ;
	}
	//m_strSnEdit
    //检查都为数字，不为数字为非法字符
	//检查并写数据库
	//INSERT INTO SmallBoard_Info VALUES ('000000006', 0,0,0,0)
	// write sn
	if(AddNewSPgBoardItem(m_strSnEdit))
	{
		return ;
	}

	OnOpenClosePort(FALSE, FALSE);
	temp=m_strPortNO;
    temp.Delete(0,3);
	m_iPortNo = atoi(temp);
	
	//
	//GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(m_strTestStatus);
	//m_strTestStatus = m_strTestStatus+"*********************************\r\n";

	m_strTestStatus = "扫描SN:"+m_strSnEdit+"\r\n";
	GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
	
#if 0
	gv_testcase = TEST_SN;
	strsend = "ATS"+m_strSnEdit+"\r\n";
	m_SerialPort.WriteToPort(strsend.GetBuffer(0),strsend.GetLength());
	SetTimer(gv_testcase,2*1000,NULL);
    GetDlgItem(IDC_EDIT_SN)->EnableWindow(0);
#else
	OnOpenClosePort(FALSE, TRUE);
    GetDlgItem(IDC_EDIT_SN)->EnableWindow(0);
	m_retry_count = 0;
	gv_testcase = TEST_START_SWTCH;
	strsend = "ATSTATE=?\r\n";
	m_SerialPort.WriteToPort(strsend.GetBuffer(0),strsend.GetLength());
	m_retry_count++;
	m_strTestStatus.Format("查询状态:%d次 \r\n", m_retry_count);
	GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);

	SetTimer(gv_testcase,2*1000,NULL);
#endif
}


void CCOMTOOLDlg::LoadSmallTestToolConfig(void)
{
    CString strPath;
	CString strTmp="1";
    char buf[1000]; 
    GetCurrentDirectory(1000,buf);//得到当前工作路径  
	strPath = buf;
	strPath += "\\SmallBJ.ini";

	::GetPrivateProfileString(_T("SmallBJConfig"), _T("NET_URL"), _T("iot.xunrun.com.cn"), m_strUrl.GetBuffer(MAX_PATH), MAX_PATH, strPath);
	::GetPrivateProfileString(_T("SmallBJConfig"), _T("TCC"), _T("2640423000"), m_strTCCCode.GetBuffer(MAX_PATH), MAX_PATH, strPath);
	::GetPrivateProfileString(_T("SmallBJConfig"), _T("RATE"), _T("430"), m_strRate.GetBuffer(MAX_PATH), MAX_PATH, strPath);
	::GetPrivateProfileString(_T("SmallBJConfig"), _T("ALARM"), _T("5"), m_strAlarm.GetBuffer(MAX_PATH), MAX_PATH, strPath);
	::GetPrivateProfileString(_T("SmallBJConfig"), _T("ADC_MIN"), _T("10"), m_strAdcMin.GetBuffer(MAX_PATH), MAX_PATH, strPath);
	::GetPrivateProfileString(_T("SmallBJConfig"), _T("ADC_MAX"), _T("40"), m_strAdcMax.GetBuffer(MAX_PATH), MAX_PATH, strPath);
	::GetPrivateProfileString(_T("SmallBJConfig"), _T("RX_1278"), _T("-100"), m_str1278rx.GetBuffer(MAX_PATH), MAX_PATH, strPath);
	::GetPrivateProfileString(_T("SmallBJConfig"), _T("TX_1278"), _T("-100"), m_str1278tx.GetBuffer(MAX_PATH), MAX_PATH, strPath);
	::GetPrivateProfileString(_T("SmallBJConfig"), _T("RX_1301"), _T("-100"), m_str1301rx.GetBuffer(MAX_PATH), MAX_PATH, strPath);
	::GetPrivateProfileString(_T("SmallBJConfig"), _T("TX_1301"), _T("-100"), m_str1301tx.GetBuffer(MAX_PATH), MAX_PATH, strPath);

	::GetPrivateProfileString(_T("SmallBJConfig"), _T("CHECK_1301"), _T("1"), strTmp.GetBuffer(MAX_PATH), MAX_PATH, strPath);
	m_check1301 = atoi(strTmp);

	m_strTCCShow = m_strTCCCode;
	GetDlgItem(IDC_EDIT_TCC_SHOW)->SetWindowText(m_strTCCShow);
}


int CCOMTOOLDlg::AddNewSPgBoardItem(CString m_strSn)
{
	CTime time = CTime::GetCurrentTime();
	CString date = time.Format("%Y-%m-%d %H:%M:%S");
	Paint_ADO m_Ado;
	//!连接数据库
	m_Ado.OnInitADOConn();
	//!设置查询字符串
	//INSERT INTO SmallBoard_Info VALUES ('00000001', 0,0,0,0)
	//CString SQL = "select * from Patient_Info order by Patient_ID desc";	
	//CString m_ExeSQL = "INSERT INTO SmallBoard_Info VALUES ('"+m_strSn+"',0,0,0,0,0,'"+date+"')";
	CString m_ExeSQL = "INSERT INTO SmallBoard_Info VALUES ('"+m_strSn+"',0,0,0,0,0,0,0,0,'"+date+"')";
	try
	{
	    m_Ado.m_pRecordset = m_Ado.OpenRecordset(m_ExeSQL);
		m_Ado.CloseRecordset();
		m_Ado.CloseConn();
	}
	catch (...)
	{
		//MessageBox("操作失败!","提示!",MB_ICONWARNING | MB_OK);
		//检查是否存在
		CString m_ExeSQL = "select * from SmallBoard_Info WHERE SBI_SN='"+m_strSn+"'";
		try
		{
			m_Ado.m_pRecordset = m_Ado.OpenRecordset(m_ExeSQL);
			if(m_Ado.GetRecordCount(m_Ado.m_pRecordset)==1)
			{
				CString strsn = (char*)(_bstr_t)m_Ado.m_pRecordset->GetCollect("SBI_SN");
				int atw_state = m_Ado.m_pRecordset->GetCollect("SBI_ATW_STATE");
				CString strTime = (char*)(_bstr_t)m_Ado.m_pRecordset->GetCollect("SBI_TIME");
				if(atw_state==1) // 所有测试都已经成功，重复扫描，测试不往下进行。如果需要重新测试，先从数据库里把这条记录删除
				{
					m_Ado.CloseRecordset();
					m_Ado.CloseConn();
					MessageBox("这个板子已经成功测试过!!!","提示!",MB_ICONWARNING | MB_OK);
					return 1;
				}
			}
			m_Ado.CloseRecordset();
			m_Ado.CloseConn();
		}
		catch(...)
		{
		}
	}
	return 0;
}


int CCOMTOOLDlg::UpdateTestResult(CString m_strSn, TESTCASE m_icase, CString m_strResult)
{
	Paint_ADO m_Ado;
	//!连接数据库
	m_Ado.OnInitADOConn();
	//!设置查询字符串
	//INSERT INTO SmallBoard_Info VALUES ('00000001', 0,0,0,0)
	//CString SQL = "select * from Patient_Info order by Patient_ID desc";	
	// UPDATE Patient_Info SET Patient_Age=46 WHERE Patient_ID=12
	CString strField;
    CString m_strFieldResult;
	switch(m_icase)
	{
	case TEST_SN:
		strField = "SBI_SN" ;
		break;
	case TEST_NET_CONFIG:
		strField = "SBI_NET";
		break;
	case TEST_TCC_CODE:
		strField = "SBI_TCC";
		break;
	case TEST_ALARM:
		strField = "SBI_RTC";
		break;
	case TEST_LORA_RF_1278:
		strField = "SBI_LORA_1278";
		break;
	case TEST_LORA_RF_1301:
		strField = "SBI_LORA_1301";
		break;
	case TEST_ADC_TEMP:
		strField = "SBI_ADC";
		break;
	case TEST_LTE:
		strField = "SBI_LTE";
		break;
	case TEST_SUCCEED_WAIT_NEXT:
		strField = "SBI_ATW_STATE";
		break;
	default:
		break;
	}
	//m_strFieldResult = m_strResult.GetBuffer();
	m_strFieldResult = m_strResult;
	//m_strFieldResult;
	//SET Patient_Age=46
	// UPDATE Patient_Info SET Patient_Age=46 WHERE Patient_ID=12
	//CString m_ExeSQL = "UPDATE SmallBoard_Info SET " + strField +"='" + m_strResult +"' WHERE SBI_SN='"+ m_strSn+"'";
    CString m_ExeSQL = "UPDATE SmallBoard_Info SET " + strField +"='" + m_strFieldResult + "' WHERE SBI_SN='"+ m_strSn+"'";
	try
	{
	    m_Ado.m_pRecordset = m_Ado.OpenRecordset(m_ExeSQL);
		m_Ado.CloseRecordset();
		m_Ado.CloseConn();
	}
	catch (...)
	{
		MessageBox("测试结果保存失败!","提示!",MB_ICONWARNING | MB_OK);
		//检查是否存在
	}
	return 0;
}


int CCOMTOOLDlg::UpdateADCResult(CString m_strSn,CString m_strADC1,CString m_strADC2,CString m_strADC3)
{
	Paint_ADO m_Ado;
	//!连接数据库
	m_Ado.OnInitADOConn();

	CString strField;

	CString m_ExeSQL = "UPDATE SmallBoard_Info SET ADC1 = " + m_strADC1 + ", ADC2 = " + m_strADC2 +   ", ADC3 = " + m_strADC3 + " WHERE SBI_SN='"+ m_strSn+"'";
	try
	{
	    m_Ado.m_pRecordset = m_Ado.OpenRecordset(m_ExeSQL);
		m_Ado.CloseRecordset();
		m_Ado.CloseConn();
	}
	catch (...)
	{
		MessageBox("测试结果保存失败!","提示!",MB_ICONWARNING | MB_OK);
		//检查是否存在
	}
	return 0;
}

void CCOMTOOLDlg::OnOpenClosePort(BOOL bRfidPort,int openflag)
{
	static int SelPortNO = MaxSerialPortNum;
	int ret;
	CString temp;
	UpdateData(true);

	if(m_PortNO.GetCount()==0)
	{
		return;
	}

	if(openflag==0)
	{
		ret = m_SerialPort.InitPort(this, SelPortNO);
		if (ret == -1) {
			//AfxMessageBox("串口关闭失败!\n");
		}
	}
	else 
	{	
		int SelBaudRate,SelDataBits,SelStopBits;
		char SelParity;
		UpdateData(true);
	    temp=m_strPortNO;
		temp.Delete(0,3);
		SelPortNO=atoi(temp);
		//SelBaudRate=115200;
	    //SelBaudRate=38400;
		SelDataBits=8;
		SelParity='N';
		SelStopBits=1;
		{
			//if(m_SerialPort.InitPort(this,SelPortNO,128000,SelParity,SelDataBits,SelStopBits,EV_RXCHAR|EV_CTS,512))		
            if(m_SerialPort.InitPort(this,SelPortNO,115200,SelParity,SelDataBits,SelStopBits,EV_RXCHAR|EV_CTS,512))	
			{
				m_SerialPort.StartMonitoring();
			}
			else 
				AfxMessageBox("该串口已经被其他应用程序所占用!\n请选择其它的串口");
		}
	}
}

int CCOMTOOLDlg::OnOpenClosePort(int openflag)
{
	static int SelPortNO = MaxSerialPortNum;
	int ret;
	CString temp;
	UpdateData(true);

	if (m_PortNO.GetCount() == 0)
	{
		return -1;
	}

	if (openflag == 0)
	{
		ret = m_SerialPort.InitPort(this, SelPortNO);
		if (ret == -1) {
			//AfxMessageBox("串口关闭失败!\n");
			return -2;
		}
	}
	else
	{
		int SelBaudRate, SelDataBits, SelStopBits;
		char SelParity;
		UpdateData(true);
		temp = m_strPortNO;
		temp.Delete(0, 3);
		SelPortNO = atoi(temp);
		//SelBaudRate=115200;
		//SelBaudRate=38400;
		SelDataBits = 8;
		SelParity = 'N';
		SelStopBits = 1;
		{
			//if(m_SerialPort.InitPort(this,SelPortNO,128000,SelParity,SelDataBits,SelStopBits,EV_RXCHAR|EV_CTS,512))		
			if (m_SerialPort.InitPort(this, SelPortNO, 115200, SelParity, SelDataBits, SelStopBits, EV_RXCHAR | EV_CTS, 512))
			{
				m_SerialPort.StartMonitoring();
			}
			else{
				return 1;
			}
		}
	}
	return 0;
}