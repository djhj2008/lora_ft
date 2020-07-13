
// COMTOOLDlg.cpp : ʵ���ļ�
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
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CCOMTOOLDlg �Ի���




CCOMTOOLDlg::CCOMTOOLDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCOMTOOLDlg::IDD, pParent)
	, m_strSnEdit(_T(""))
{
	//{{AFX_DATA_INIT(CCOMTOOLDlg)
	m_strSend = "";
	m_strReceive = "";
	m_bHexR = FALSE;
	m_bHexS = FALSE;
	m_bAutoSend = TRUE;
	m_nSendPeriod = 1000;
	m_strStatus = "�ر�";
	m_strSendBytes = "0";
	m_strReceiveBytes = "0";
	m_strPortNO = "";
	m_nSendBytes=0;
	m_nReceiveBytes=0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	gv_testcase = TEST_CASE_MAX;
	m_retry_count = TEST_STEP_COUNT_MAX;
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
	DDX_Control(pDX, IDC_CHECK2, m_bNewLine);
	DDX_Control(pDX, IDC_Send, m_SendCtrl);
	DDX_Control(pDX, IDC_COM, m_PortNO);
	DDX_CBString(pDX, IDC_COM, m_strPortNO);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_SN, m_strSnEdit);
	DDV_MaxChars(pDX, m_strSnEdit, 10);
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
	ON_EN_CHANGE(IDC_EDIT2, &CCOMTOOLDlg::OnEnChangeEdit2)
END_MESSAGE_MAP()


// CCOMTOOLDlg ��Ϣ�������

BOOL CCOMTOOLDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

    CString Tmp,Tmp1;
	HKey[4] = '5';
    HKey[5] = '6';
    HKey[6] = '7';
    HKey[7] = '8';
    
	LKey[4] = '4';
	LKey[5] = '3';
	LKey[6] = '2';
	LKey[7] = '1';

    CString title = "�����������(v105)";        
    SetWindowText(title);

	CString temp;				
	//��ʾ��������
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

	//���ذ弶Ĭ������
	LoadSmallTestToolConfig();
	GetDlgItem(IDC_EDIT_SN)->SetFocus(); //.SendMessage(WM_KEYDOWN, VK_END, 0);
	return FALSE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCOMTOOLDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
	if(m_strStatus=="�ر�")
	{
		AfxMessageBox("�����ȴ򿪴���");
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

BYTE gv_recstatus=0;   // 0 ���ܽ������ݣ�1��������
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
		{
			KillTimer(nIDEvent);
			OnOpenClosePort(FALSE,TRUE);
	        gv_testcase = TEST_SN;
	        strsend = "ATS"+m_strSnEdit+"\r\n";
	        m_SerialPort.WriteToPort(strsend.GetBuffer(0),strsend.GetLength());
	        SetTimer(gv_testcase,2*1000,NULL);
		}
		break;
	case TEST_SN:
		  GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(m_strTestStatus);
		  strTmp = "SN:"+m_strSnEdit+"дʧ��\r\n";
		  m_strTestStatus = m_strTestStatus+strTmp;
          GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
          GetDlgItem(IDC_EDIT_SN)->EnableWindow(1);
          GetDlgItem(IDC_EDIT_SN)->SetWindowText("");
		  KillTimer(TEST_SN);
		  break;
	case TEST_STEP:
			GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(m_strTestStatus);
			strTmp = "STEP:����ʧ��\r\n";
			m_strTestStatus = m_strTestStatus + strTmp;
			GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
			GetDlgItem(IDC_EDIT_SN)->EnableWindow(1);
			GetDlgItem(IDC_EDIT_SN)->SetWindowText("");
			KillTimer(TEST_STEP);
			break;
	case TEST_STEP_START:
			GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(m_strTestStatus);
			strTmp = "STEP:�ȴ���ȡ����\r\n";
			m_strTestStatus = m_strTestStatus + strTmp;
			GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
			GetDlgItem(IDC_EDIT_SN)->EnableWindow(1);
			GetDlgItem(IDC_EDIT_SN)->SetWindowText("");
			KillTimer(TEST_STEP_START);
			m_retry_count = TEST_STEP_COUNT_MAX;
			gv_testcase = TEST_STEP_COUNT;
			strsend = "ATB\r\n";
			m_SerialPort.WriteToPort(strsend.GetBuffer(0), strsend.GetLength());
			SetTimer(gv_testcase, 2 * 1000, NULL);
			break;
	case TEST_STEP_COUNT:
			GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(m_strTestStatus);
			strTmp = "STEP:��ȡ����ʧ��\r\n";
			m_strTestStatus = m_strTestStatus + strTmp;
			GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
			GetDlgItem(IDC_EDIT_SN)->EnableWindow(1);
			GetDlgItem(IDC_EDIT_SN)->SetWindowText("");
			KillTimer(TEST_STEP_COUNT);
			break;
	case TEST_RTC:
		  UpdateTestResult(m_strSnEdit,TEST_RTC,"2");
		  GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(m_strTestStatus);
		  m_strTestStatus = m_strTestStatus+"RTC����ʧ��\r\n";
          GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
		  break;
	case TEST_ADC_SAMPLE:
		  UpdateTestResult(m_strSnEdit,TEST_ADC_SAMPLE,"2");
		  GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(m_strTestStatus);
		  m_strTestStatus = m_strTestStatus+"ADC��������ʧ��\r\n";
          GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
		  break;
	case TEST_LORA_RXTX:
		  UpdateTestResult(m_strSnEdit,TEST_LORA_RXTX,"4");
		  GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(m_strTestStatus);
		  strTmp =m_strTestFreq.GetBuffer();
          strTmp = "Lora(Ƶ��"+strTmp+")�շ�����ʧ��\r\n";
		  m_strTestStatus = m_strTestStatus+strTmp;
          GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
		  break;
	case TEST_ATW_STATE:
	case TEST_ADC_SAMPLE_STATE:
		  UpdateTestResult(m_strSnEdit,TEST_ATW_STATE,"2");
		  GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(m_strTestStatus);
		  m_strTestStatus = m_strTestStatus+"�������״̬ʧ��!!!\r\n";
          GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
		break;
	case TEST_SUCCEED_WAIT_NEXT:
		{
			KillTimer(nIDEvent);
			OnOpenClosePort(FALSE,FALSE);
            OnOpenClosePort(TRUE,FALSE);
		}
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
    GetDlgItem(IDC_EDIT2)->GetWindowText(m_strReceive);
	{
		m_strReceive += (char)ch;
	}
    GetDlgItem(IDC_EDIT2)->SetWindowText(m_strReceive);
	// here handle recieve data
	switch(gv_testcase)
	{
	case TEST_SN:
		//AT+SNOK
		if(m_strReceive.Find("AT+SNOK")>=0)
		{
		   m_strReceive= "";
		   GetDlgItem(IDC_EDIT2)->SetWindowText(m_strReceive);
		   KillTimer(gv_testcase);
           GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(m_strTestStatus);

		   strTmp = "SN:"+m_strSnEdit+"д�ɹ�\r\n";
		   m_strTestStatus = m_strTestStatus+strTmp;
           GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
           GetDlgItem(IDC_EDIT2)->SetWindowText("");
           //gv_testcase = TEST_RTC;
	       //strsend = "ATR5\r\n";
	       //m_SerialPort.WriteToPort(strsend.GetBuffer(0),strsend.GetLength());
	       //SetTimer(gv_testcase,10*1000,NULL);
		   gv_testcase = TEST_STEP;
		   if (m_checkStep){
			   strsend = "ATK1\r\n";
		   }
		   else{
			   strsend = "ATK0\r\n";
		   }
		   m_SerialPort.WriteToPort(strsend.GetBuffer(0), strsend.GetLength());
		   SetTimer(gv_testcase, 5 * 1000, NULL);
		}
		break;
	case TEST_STEP:
		if (m_strReceive.Find("ATKOK") >= 0){
			m_strReceive = "";
			GetDlgItem(IDC_EDIT2)->SetWindowText(m_strReceive);
			KillTimer(gv_testcase);
			GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(m_strTestStatus);

			strTmp = "�Ʋ���:���óɹ�\r\n";
			m_strTestStatus = m_strTestStatus + strTmp;
			GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
			GetDlgItem(IDC_EDIT2)->SetWindowText("");

			if (m_checkStep){
				gv_testcase = TEST_STEP_START;
				strsend = "ATP1\r\n";
				m_SerialPort.WriteToPort(strsend.GetBuffer(0), strsend.GetLength());
				SetTimer(gv_testcase, 2 * 1000, NULL);
			}
			else{
				gv_testcase = TEST_RTC;
				strsend = "ATR5\r\n";
				m_SerialPort.WriteToPort(strsend.GetBuffer(0), strsend.GetLength());
				SetTimer(gv_testcase, 10 * 1000, NULL);
			}
		}
		else if (m_strReceive.Find("ATKERR") >= 0){
			m_strReceive = "";
			GetDlgItem(IDC_EDIT2)->SetWindowText(m_strReceive);
			KillTimer(gv_testcase);
			GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(m_strTestStatus);

			strTmp = "�Ʋ���:���óɹ�\r\n";
			m_strTestStatus = m_strTestStatus + strTmp;
			GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
			GetDlgItem(IDC_EDIT2)->SetWindowText("");
		}
		break;
	case TEST_STEP_COUNT:
		if (m_strReceive.Find("ATBOK") >= 0){
			pos1 = m_strReceive.Find("ATBOK");
			pos2 = m_strReceive.Find("\r\n",pos1+6);
			if (pos2 >= 0 ){
				CString str_count = m_strReceive.Mid(pos1 + 6, pos2 - (pos1 + 1));

				m_strReceive = "";
				GetDlgItem(IDC_EDIT2)->SetWindowText(m_strReceive);
				KillTimer(gv_testcase);
				GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(m_strTestStatus);

				UpdateTestResult(m_strSnEdit, TEST_STEP_COUNT, str_count);
				strTmp = "�Ʋ���:" + str_count + "��ȡ�����ɹ�\r\n";
				m_strTestStatus = m_strTestStatus + strTmp;
				GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
				GetDlgItem(IDC_EDIT2)->SetWindowText("");

				strsend = "ATP0\r\n";
				m_SerialPort.WriteToPort(strsend.GetBuffer(0), strsend.GetLength());

				gv_testcase = TEST_RTC;
				strsend = "ATR5\r\n";
				m_SerialPort.WriteToPort(strsend.GetBuffer(0), strsend.GetLength());
				SetTimer(gv_testcase, 10 * 1000, NULL);
			}
		}
		else if (m_strReceive.Find("ATBERR") >= 0){
			m_strReceive = "";
			GetDlgItem(IDC_EDIT2)->SetWindowText(m_strReceive);
			KillTimer(gv_testcase);
			GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(m_strTestStatus);

			if (m_retry_count > 0){
				CString str_retry_count = "";
				str_retry_count.Format("%d", m_retry_count);
				strTmp = "STEP:" + str_retry_count + "������\r\n";
				m_strTestStatus = m_strTestStatus + strTmp;
				GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
				GetDlgItem(IDC_EDIT2)->SetWindowText("");
				Sleep(500);
				strsend = "ATB\r\n";
				m_SerialPort.WriteToPort(strsend.GetBuffer(0), strsend.GetLength());
				SetTimer(gv_testcase, 2 * 1000, NULL);
			}
			else{
				strTmp = "�Ʋ���:��ȡ����ʧ��\r\n";
				m_strTestStatus = m_strTestStatus + strTmp;
				GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
				GetDlgItem(IDC_EDIT2)->SetWindowText("");
			}
			m_retry_count--;
		}
		break;
	case TEST_RTC:
		//RTCOK
		if(m_strReceive.Find("RTCOK")>=0)
		{
		   UpdateTestResult(m_strSnEdit,TEST_RTC,"1");
		   m_strReceive= "";
		   GetDlgItem(IDC_EDIT2)->SetWindowText(m_strReceive);
		   KillTimer(gv_testcase);
           GetDlgItem(IDC_EDIT2)->SetWindowText("");
           GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(m_strTestStatus);
		   m_strTestStatus = m_strTestStatus+"RTC���Գɹ�\r\n";
           GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
		
           GetDlgItem(IDC_EDIT2)->SetWindowText("");
           gv_testcase = TEST_ADC_SAMPLE;
		   strsend = "ATA\r\n";
	       m_SerialPort.WriteToPort(strsend.GetBuffer(0),strsend.GetLength());
		   SetTimer(gv_testcase,10*1000,NULL);
		}

		break;
	case TEST_ADC_SAMPLE:
		//AT+TEMP:7115b0,7066f6
		pos1 = m_strReceive.Find("AT+TEMP:");
		if(pos1>=0)
		{
		  pos2 = m_strReceive.Find(",",pos1);
		  if(pos2>=0)
		  {
			    //pos3 = m_strReceive.Find("\r\n",pos2);
                pos3 = m_strReceive.Find(",",pos2+1);
				if(pos3>=0)
				{
					 pos4 = m_strReceive.Find("\r\n",pos3+1);
		             if(pos4>=0)
					 {
						CString str_adc1,str_adc2,str_adc3;
						int adc1,adc2,adc3;

						str_adc1 = m_strReceive.Mid(pos1+8,pos2-(pos1+8));
						str_adc2 = m_strReceive.Mid(pos2+1,pos3-pos2-1);
						str_adc3 = m_strReceive.Mid(pos3+1,pos4-pos3-1);
						adc1 = atoi(str_adc1);
						adc2 = atoi(str_adc2);
                        adc3 = atoi(str_adc3);
						GetDlgItem(IDC_EDIT2)->SetWindowText("");
						GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(m_strTestStatus);
						strTmp.Format("ADC����ֵ:%d;%d;%d",adc1,adc2,adc3);

						UpdateADCResult(m_strSnEdit,str_adc1,str_adc2,str_adc3);
						m_strReceive= "";
						GetDlgItem(IDC_EDIT2)->SetWindowText(m_strReceive);
						KillTimer(gv_testcase);
						if(((adc1<atoi(m_strAdcSampleMaxValue))&&(adc1>atoi(m_strAdcSampleMinValue)))&&((adc2<atoi(m_strAdcSampleMaxValue))&&(adc2>atoi(m_strAdcSampleMinValue)))&&((adc3<atoi(m_strAdcSampleMaxValue))&&(adc3>atoi(m_strAdcSampleMinValue))))
						{
							UpdateTestResult(m_strSnEdit,TEST_ADC_SAMPLE,"1");
							strTmp = strTmp+"�����ɹ�\r\n";
							m_strTestStatus = m_strTestStatus+strTmp;
							GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
							gv_testcase = TEST_LORA_RXTX;
							strTmp = m_strTestFreq.GetBuffer();
							strsend = "ATF" + strTmp + "\r\n";
							m_SerialPort.WriteToPort(strsend.GetBuffer(0),strsend.GetLength());
							SetTimer(gv_testcase,20*1000,NULL);
						}
						else
						{
							UpdateTestResult(m_strSnEdit,TEST_ADC_SAMPLE,"2");
							strTmp = strTmp+"������Χ\r\n";
							m_strTestStatus = m_strTestStatus+strTmp;
							GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
							GetDlgItem(IDC_EDIT2)->SetWindowText("");
							GetDlgItem(IDC_EDIT_SN)->EnableWindow(1);
							GetDlgItem(IDC_EDIT_SN)->SetWindowText("");
							GetDlgItem(IDC_EDIT_SN)->SetFocus();
						}
				    }
				}
		  }
		}
		break;
	case TEST_LORA_RXTX:
		if(m_strReceive.Find("AT+RFOK")>=0)
		{
			pos1 = m_strReceive.Find("AT+RFOK");
			if (pos1 >= 0){
				pos2 = m_strReceive.Find(":",pos1+strlen("AT+RFOK"));
				if (pos2 > 0){
					pos3 = m_strReceive.Find("\r\n", pos2);
				}
			}
			if (pos3 >= 0){
				str_rx_rssi = m_strReceive.Mid(pos1 + strlen("AT+RFOK") + 1, pos2 - (pos1 + strlen("AT+RFOK") + 1));
				str_tx_rssi = m_strReceive.Mid(pos2 + 1, pos3 - (pos2 + 1));

				int i_rx_rssi = atoi(str_rx_rssi);
				int i_tx_rssi = atoi(str_tx_rssi);

				if (i_rx_rssi < atoi(m_strRxrssi) || i_tx_rssi < atoi(m_strTxrssi)){
					if (i_rx_rssi < atoi(m_strRxrssi)){
						UpdateTestResult(m_strSnEdit, TEST_LORA_RXTX, "3");
					}
					else{
						UpdateTestResult(m_strSnEdit, TEST_LORA_RXTX, "2");
					}
					m_strReceive = "";
					GetDlgItem(IDC_EDIT2)->SetWindowText(m_strReceive);
					KillTimer(gv_testcase);
					strTmp = strTmp + "LORA RSSI:" + " RX:" + str_rx_rssi + ",TX:" + str_tx_rssi + ",������Χ\r\n";
					m_strTestStatus = m_strTestStatus + strTmp;
					GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
					GetDlgItem(IDC_EDIT2)->SetWindowText("");
					GetDlgItem(IDC_EDIT_SN)->EnableWindow(1);
					GetDlgItem(IDC_EDIT_SN)->SetWindowText("");
					GetDlgItem(IDC_EDIT_SN)->SetFocus();
				}
				else{
					UpdateTestResult(m_strSnEdit, TEST_LORA_RXTX, "1");
					m_strReceive = "";
					GetDlgItem(IDC_EDIT2)->SetWindowText(m_strReceive);
					KillTimer(gv_testcase);
					GetDlgItem(IDC_EDIT2)->SetWindowText("");
					GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(m_strTestStatus);
					strTmp = m_strTestFreq.GetBuffer();
					strTmp = "LoraƵ��:" + strTmp + " RX:" + str_rx_rssi + "TX:" + str_tx_rssi + ",�շ����Գɹ�\r\n";
					m_strTestStatus = m_strTestStatus + strTmp;
					GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);

					if (m_bATWConfig)
					{
						gv_testcase = TEST_ATW_STATE;
						strsend = "ATW\r\n"; 
						m_SerialPort.WriteToPort(strsend.GetBuffer(0), strsend.GetLength());
						SetTimer(gv_testcase, 20 * 1000, NULL);
					}
					else
					{
						CTime time = CTime::GetCurrentTime();
						CString date = time.Format("%Y-%m-%d %H:%M:%S");
						UpdateTestResult(m_strSnEdit, TEST_ATW_STATE, "0");
						UpdateTestResult(m_strSnEdit, TEST_ADC_SAMPLE_STATE, date);
						// ���²��Խ��ʱ��
						m_strReceive = "";
						GetDlgItem(IDC_EDIT2)->SetWindowText(m_strReceive);
						KillTimer(gv_testcase);
						GetDlgItem(IDC_EDIT_SN)->EnableWindow(1);
						GetDlgItem(IDC_EDIT_SN)->SetWindowText("");
						GetDlgItem(IDC_EDIT_SN)->SetFocus();

						strTmp = "���Գɹ����ȴ���һ�β���!!!\r\n";
						m_strTestStatus = m_strTestStatus + strTmp;
						GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
						gv_testcase = TEST_SUCCEED_WAIT_NEXT;
						SetTimer(gv_testcase, 1 * 1000, NULL);
					}
				}

			}
		}
		else if(m_strReceive.Find("AT+RFERR1")>=0) //AT+RFERR1  // LORA���ͳ�ʱ
		{
			UpdateTestResult(m_strSnEdit,TEST_LORA_RXTX,"2");
		    m_strReceive= "";
		    GetDlgItem(IDC_EDIT2)->SetWindowText(m_strReceive);
			KillTimer(gv_testcase);
			strTmp = strTmp+"LORA���ͳ�ʱ\r\n";
			m_strTestStatus = m_strTestStatus+strTmp;
            GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
			GetDlgItem(IDC_EDIT2)->SetWindowText("");
			GetDlgItem(IDC_EDIT_SN)->EnableWindow(1);
            GetDlgItem(IDC_EDIT_SN)->SetWindowText("");
		    GetDlgItem(IDC_EDIT_SN)->SetFocus();
		}
		else if(m_strReceive.Find("AT+RFERR2")>=0) //AT+RFERR2  // LORA���ճ�ʱ
		{
		    UpdateTestResult(m_strSnEdit,TEST_LORA_RXTX,"3");
		    m_strReceive= "";
		    GetDlgItem(IDC_EDIT2)->SetWindowText(m_strReceive);
			KillTimer(gv_testcase);
			strTmp = strTmp+"LORA���ճ�ʱ\r\n";
			m_strTestStatus = m_strTestStatus+strTmp;
            GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
			GetDlgItem(IDC_EDIT2)->SetWindowText("");
			GetDlgItem(IDC_EDIT_SN)->EnableWindow(1);
            GetDlgItem(IDC_EDIT_SN)->SetWindowText("");
		    GetDlgItem(IDC_EDIT_SN)->SetFocus();
		}
		break;
	case TEST_ATW_STATE:
		if(m_strReceive.Find("ATWOK")>=0)
		{
		   UpdateTestResult(m_strSnEdit,TEST_ATW_STATE,"2");
		   m_strReceive= "";
		   GetDlgItem(IDC_EDIT2)->SetWindowText(m_strReceive);
		   KillTimer(gv_testcase);
		   strsend = "ATR5\r\n";
		   m_SerialPort.WriteToPort(strsend.GetBuffer(0),strsend.GetLength());
		   Sleep(2000);
	       gv_testcase = TEST_ADC_SAMPLE_STATE;
		   SetTimer(gv_testcase,20*1000,NULL);
		}
		break;
	case TEST_ADC_SAMPLE_STATE:
		if(m_strReceive.Find("SAMPLE_STATE_OK")>=0)
		{
	       CTime time = CTime::GetCurrentTime();
	       CString date = time.Format("%Y-%m-%d %H:%M:%S");
		   UpdateTestResult(m_strSnEdit,TEST_ATW_STATE,"1");
		   UpdateTestResult(m_strSnEdit,TEST_ADC_SAMPLE_STATE,date);
		   // ���²��Խ��ʱ��

		   m_strReceive= "";
		   GetDlgItem(IDC_EDIT2)->SetWindowText(m_strReceive);
		   KillTimer(gv_testcase);
           GetDlgItem(IDC_EDIT_SN)->EnableWindow(1);
           GetDlgItem(IDC_EDIT_SN)->SetWindowText("");
		   GetDlgItem(IDC_EDIT_SN)->SetFocus();
           strTmp = "�Ѿ��������״̬!!!\r\n";
		   m_strTestStatus = m_strTestStatus+strTmp;
           GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
		   gv_testcase = TEST_SUCCEED_WAIT_NEXT;
		   SetTimer(gv_testcase,1*1000,NULL);
		}
		break;
	case TEST_SUCCEED_WAIT_NEXT:
	    if(m_strReceive.GetLength()>1024)
	    {
		   m_strReceive= "";
		   GetDlgItem(IDC_EDIT2)->SetWindowText(m_strReceive);
	    }
		break;
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
	dlg.m_strTestFreq = m_strTestFreq;
    dlg.m_strWorkFreq = m_strWorkFreq;
	dlg.m_strAdcSampleMaxValue = m_strAdcSampleMaxValue;
	dlg.m_strAdcSampleMinValue = m_strAdcSampleMinValue;
	dlg.m_strRxrssi = m_strRxrssi;
	dlg.m_strTxrssi = m_strTxrssi;
	dlg.m_checkStep = m_checkStep;

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
С��弶��������:
���Ӽ�����ӱ�ǩ�Ͷ�ά��ͬʱ���ڣ�����ά�붪ʧʱ����Ҫ���ݼ�����ӱ�ǩ���´�ӡ��ά���ǩ
1. ɨ���ά���ǩ,����SN��д��SN
2. LORA�����շ�:С�巢�����ݵ����Ի�վ����վ�յ����ٰѽ��յ������ݷ��ͻ��������С�巢�����ݺͽ��յ�������һ�£�Ϊ�ɹ�����ֻ�ܲ��Թ���һ�������ܲ����ź�����)
3. ����ADC,ADCΪһ���̶�ֵ�ĵ��裬��������ֵӦ��Ϊһ����ֵ���仯���Ⱥ�С��Χ��������ͨ��������Ҫ���÷�Χ�ο���
4. RTC���ԣ��������ӣ��ػ���������Ϊ�ɹ�
5. �ػ�����(TBD,��Ҫ�̿ص�Դ�ɱ�̣���API�������Կ���)
6. ��ѹ��⣬�ȽϿ���(TBD,��Ҫ�̿ص�Դ�ɱ�̣���API�������Կ���)
7. ���ϲ��Զ��ɹ���д��״̬ATW,����У׼״̬��д��״̬ATW�󣬴�������Ȼ����Է���ˮ�к������н�����װУ׼����
*/
/*
2018.12.12
1. ����ADCֵ3��
2. дĬ��ֵ3��
3. дĬ�Ϲ���Ƶ��
4. ������ɺ󣬽������״̬(У׼����ʡ�Ե���ʹ��Ĭ��ֵ)
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
    //��鶼Ϊ���֣���Ϊ����Ϊ�Ƿ��ַ�
	//��鲢д���ݿ�
	//INSERT INTO SmallBoard_Info VALUES ('000000006', 0,0,0,0)
	// write sn
	if(AddNewSPgBoardItem(m_strSnEdit))
	{
		return ;
	}

	OnOpenClosePort(FALSE, FALSE);
	OnOpenClosePort(TRUE, FALSE);


	temp=m_strPortNO;
    temp.Delete(0,3);
	m_iPortNo = atoi(temp);
	
	//
	//GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(m_strTestStatus);
	//m_strTestStatus = m_strTestStatus+"*********************************\r\n";
    GetDlgItem(IDC_EDIT_STATUS)->SetWindowText("");
    GetDlgItem(IDC_EDIT2)->SetWindowText("");
	{
		CString strFactoryID;
		CString strPGID;
		CString strTmp;
		CString strTmp1;

		//memcpy(FactoryID,m_strSnEdit.GetBuffer(0),5);
		strFactoryID = m_strSnEdit.Mid(0,5);
		nFactoryID = atoi(strFactoryID);
		nFactoryID &= 0x7ffff;
		strPGID = m_strSnEdit.Mid(5,4);
		nPgID = atoi(strPGID);
		nPgID &= 0x1fff;
		nSN =  (nFactoryID<<13)+ nPgID;

		strTmp = "FactoryID:"+strFactoryID+"\r\n";
		strTmp = strTmp + "PGID:"+strPGID+"\r\n";
		strTmp1.Format("ת�����nSN:%x\r\n",nSN);
		m_strTestStatus = strTmp+strTmp1;
		GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
	}
#if 0
	gv_testcase = TEST_SN;
	strsend = "ATS"+m_strSnEdit+"\r\n";
	m_SerialPort.WriteToPort(strsend.GetBuffer(0),strsend.GetLength());
	SetTimer(gv_testcase,2*1000,NULL);
    GetDlgItem(IDC_EDIT_SN)->EnableWindow(0);
#else
    
    GetDlgItem(IDC_EDIT_SN)->EnableWindow(0);

	gv_testcase = TEST_START_SWTCH;
	//strsend = "ATS"+m_strSnEdit+"\r\n";
	//m_SerialPort.WriteToPort(strsend.GetBuffer(0),strsend.GetLength());
	
	SetTimer(gv_testcase,1*1000,NULL);
#endif
}


void CCOMTOOLDlg::LoadSmallTestToolConfig(void)
{
    CString strPath;
	CString strTmp="1";
    char buf[1000]; 
    GetCurrentDirectory(1000,buf);//�õ���ǰ����·��  
	strPath = buf;
	strPath += "\\SmallBJ.ini";

    ::GetPrivateProfileString(_T("SmallBJConfig"), _T("TestFreq"), _T("440000000"), m_strTestFreq.GetBuffer(MAX_PATH),MAX_PATH, strPath);
    ::GetPrivateProfileString(_T("SmallBJConfig"), _T("WorkFreq"), _T("433000000"), m_strWorkFreq.GetBuffer(MAX_PATH),MAX_PATH, strPath);
    ::GetPrivateProfileString(_T("SmallBJConfig"), _T("AdcSampleMaxValue"), _T("2294612"),m_strAdcSampleMaxValue.GetBuffer(MAX_PATH),MAX_PATH,strPath);  
    ::GetPrivateProfileString(_T("SmallBJConfig"), _T("AdcSampleMinValue"), _T("1802342"),m_strAdcSampleMinValue.GetBuffer(MAX_PATH),MAX_PATH, strPath);  
	::GetPrivateProfileString(_T("SmallBJConfig"), _T("TXRSSI"), _T("-80"), m_strTxrssi.GetBuffer(MAX_PATH), MAX_PATH, strPath);
	::GetPrivateProfileString(_T("SmallBJConfig"), _T("RXRSSI"), _T("-80"), m_strRxrssi.GetBuffer(MAX_PATH), MAX_PATH, strPath);

	::GetPrivateProfileString(_T("SmallBJConfig"), _T("ATW"), _T("1"),strTmp.GetBuffer(MAX_PATH),MAX_PATH, strPath);
	m_bATWConfig = atoi(strTmp);

    ::GetPrivateProfileString(_T("SmallBJConfig"), _T("TestStep"), _T("1"),strTmp.GetBuffer(MAX_PATH),MAX_PATH, strPath);
	m_checkStep = atoi(strTmp);

	if (m_checkStep) {
		GetDlgItem(IDC_EDIT_STEP)->SetWindowText("����");
	}
	else {
		GetDlgItem(IDC_EDIT_STEP)->SetWindowText("�ر�");
	}
}

void CCOMTOOLDlg::OnEnChangeEdit2()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


int CCOMTOOLDlg::AddNewSPgBoardItem(CString m_strSn)
{
	CTime time = CTime::GetCurrentTime();
	CString date = time.Format("%Y-%m-%d %H:%M:%S");
	Paint_ADO m_Ado;
	//!�������ݿ�
	m_Ado.OnInitADOConn();
	//!���ò�ѯ�ַ���
	//INSERT INTO SmallBoard_Info VALUES ('00000001', 0,0,0,0)
	//CString SQL = "select * from Patient_Info order by Patient_ID desc";	
	//CString m_ExeSQL = "INSERT INTO SmallBoard_Info VALUES ('"+m_strSn+"',0,0,0,0,0,'"+date+"')";
	CString m_ExeSQL = "INSERT INTO SmallBoard_Info VALUES ('"+m_strSn+"',0,0,0,0,0,'"+date+"'"+",0,0,0"")";
	try
	{
	    m_Ado.m_pRecordset = m_Ado.OpenRecordset(m_ExeSQL);
		m_Ado.CloseRecordset();
		m_Ado.CloseConn();
	}
	catch (...)
	{
		//MessageBox("����ʧ��!","��ʾ!",MB_ICONWARNING | MB_OK);
		//����Ƿ����
		CString m_ExeSQL = "select * from SmallBoard_Info WHERE SBI_SN='"+m_strSn+"'";
		try
		{
			m_Ado.m_pRecordset = m_Ado.OpenRecordset(m_ExeSQL);
			if(m_Ado.GetRecordCount(m_Ado.m_pRecordset)==1)
			{
				CString strsn = (char*)(_bstr_t)m_Ado.m_pRecordset->GetCollect("SBI_SN");
				int rtc = m_Ado.m_pRecordset->GetCollect("SBI_RTC");
				int adc = m_Ado.m_pRecordset->GetCollect("SBI_ADC");
				int lora_rt = m_Ado.m_pRecordset->GetCollect("SBI_LORA_RT");
				int atw_state = m_Ado.m_pRecordset->GetCollect("SBI_ATW_STATE");
				CString strTime = (char*)(_bstr_t)m_Ado.m_pRecordset->GetCollect("SBI_TIME");
				if((rtc==1)&&(adc==1)&&(lora_rt==1)&&(atw_state==1)) // ���в��Զ��Ѿ��ɹ����ظ�ɨ�裬���Բ����½��С������Ҫ���²��ԣ��ȴ����ݿ����������¼ɾ��
				{
					m_Ado.CloseRecordset();
					m_Ado.CloseConn();
					MessageBox("��������Ѿ��ɹ����Թ�!!!","��ʾ!",MB_ICONWARNING | MB_OK);
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
	//!�������ݿ�
	m_Ado.OnInitADOConn();
	//!���ò�ѯ�ַ���
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
	case TEST_RTC:
		strField = "SBI_RTC" ;
		break;
	case TEST_ADC_SAMPLE:
		strField = "SBI_ADC" ;
		break;
	case TEST_LORA_RXTX:
		strField = "SBI_LORA_RT" ;
		break;
	case TEST_STEP_COUNT:
		 strField = "SBI_STEP" ;
		break;
	case TEST_ATW_STATE:
		strField = "SBI_ATW_STATE" ;
		break;
	case TEST_ADC_SAMPLE_STATE:
		strField = "SBI_TIME";
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
		MessageBox("���Խ������ʧ��!","��ʾ!",MB_ICONWARNING | MB_OK);
		//����Ƿ����
	}
	return 0;
}


int CCOMTOOLDlg::UpdateADCResult(CString m_strSn,CString m_strADC1,CString m_strADC2,CString m_strADC3)
{
	Paint_ADO m_Ado;
	//!�������ݿ�
	m_Ado.OnInitADOConn();
	//!���ò�ѯ�ַ���
	//INSERT INTO SmallBoard_Info VALUES ('00000001', 0,0,0,0)
	//CString SQL = "select * from Patient_Info order by Patient_ID desc";	
	// UPDATE Patient_Info SET Patient_Age=46 WHERE Patient_ID=12
	CString strField;

	//SET Patient_Age=46
	// UPDATE Patient_Info SET Patient_Age=46 WHERE Patient_ID=12
/*
	update   table1
set name='����2',name1='����2',name2='����2'
where id ='2'
*/

	CString m_ExeSQL = "UPDATE SmallBoard_Info SET ADC1 = " + m_strADC1 + ", ADC2 = " + m_strADC2 +   ", ADC3 = " + m_strADC3 + " WHERE SBI_SN='"+ m_strSn+"'";
	try
	{
	    m_Ado.m_pRecordset = m_Ado.OpenRecordset(m_ExeSQL);
		m_Ado.CloseRecordset();
		m_Ado.CloseConn();
	}
	catch (...)
	{
		MessageBox("���Խ������ʧ��!","��ʾ!",MB_ICONWARNING | MB_OK);
		//����Ƿ����
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
			//AfxMessageBox("���ڹر�ʧ��!\n");
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
				AfxMessageBox("�ô����Ѿ�������Ӧ�ó�����ռ��!\n��ѡ�������Ĵ���");
		}
	}
}
