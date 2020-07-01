
// COMTOOLDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "COMTOOL.h"
#include "COMTOOLDlg.h"
#include "afxdialogex.h"

#include "TestSettingsDlg.h"
#include "TestResultQueryDlg.h"
#include "Paint_ADO.h"

class CChatSocket;

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
	m_strRfidPortNO = _T("");
}

void CCOMTOOLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCOMTOOLDlg)
	DDX_Control(pDX, IDC_Send, m_SendCtrl);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_SN, m_strSnEdit);
	DDV_MaxChars(pDX, m_strSnEdit, 10);
	DDX_Control(pDX, IDC_COM_RFID, m_RfidPortNO);
	DDX_CBString(pDX, IDC_COM_RFID, m_strRfidPortNO);
	DDV_MaxChars(pDX, m_strRfidPortNO, 10);
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
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BHexS, OnBHexS)
	ON_BN_CLICKED(IDC_BHexR, OnBHexR)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_SETTINGS, &CCOMTOOLDlg::OnFileSettings)
	ON_COMMAND(ID_Menu_TEST_RESULT_QUERY, &CCOMTOOLDlg::OnMenuTestResultQuery)
	ON_EN_CHANGE(IDC_EDIT_SN, &CCOMTOOLDlg::OnEnChangeEditSn)
	ON_BN_CLICKED(IDC_RFID_READ, &CCOMTOOLDlg::OnBnClickedRfidRead)
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
	m_pSocket = new CChatSocket(this);
#if 0
	if (!m_pSocket->Create())
	{
		delete m_pSocket;
		m_pSocket = NULL;
		AfxMessageBox("create socket failed");
		return FALSE;
	}

	if(!m_pSocket->Connect((LPCTSTR)"127.0.0.1",6188))
	{
	    AfxMessageBox("Failed to connect to server\nTry again? ");
		m_pSocket->Close();
	    delete m_pSocket;
		m_pSocket = NULL;
		return FALSE;
	}
#endif

    CString Tmp,Tmp1;
	HKey[4] = '5';
    HKey[5] = '6';
    HKey[6] = '7';
    HKey[7] = '8';
    
	LKey[4] = '4';
	LKey[5] = '3';
	LKey[6] = '2';
	LKey[7] = '1';

    CString title = "С����װRFID����(03)";        
    SetWindowText(title);

	CString temp;				
	//��ʾ��������

	for(int i=1;i<=MaxSerialPortNum-1;i++)
	{
		if(m_RfidPort.InitPort(this,i))
		{
			temp.Format("COM%d",i);		
			m_RfidPortNO.AddString((LPCTSTR)temp);
		}
	}
	if(m_RfidPortNO.GetCount())
	{
		m_RfidPort.InitPort(this,MaxSerialPortNum);
		m_RfidPortNO.SetCurSel(0);
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
	OnOpenClosePort(FALSE);
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
char gv_Label[13];
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
	case TEST_RFID_WRITE_SN:
		{
			unsigned short int uiCrcValue = 0;
			char cmdLen;
			char cmdAddr;
			char cmd;
			char cmd_buf[1024 + 5];
			int Length = 0;
			CString m_strSnWrite;

			m_strSnWrite = m_strSnEdit+"FFF";
			memset(gv_Label,0,sizeof(gv_Label));
			Length = m_strSnWrite.GetLength()/2;
			gv_Label[0] = Length;
			for(int i=0;i<Length;i++)
			{
				gv_Label[i+1]+=CombineHexChar(m_strSnWrite.GetAt(i*2),m_strSnWrite.GetAt(i*2+1));
			}
	    //	m_strSnEdit 


#if 0
			gv_Label[0]  = 0x06;
			gv_Label[1]  = 0x00;
			gv_Label[2]  = 0x00;
			gv_Label[3]  = 0x20;
			gv_Label[4]  = 0x28;
			gv_Label[5]  = 0x3f;
			gv_Label[6]  = 0xff;
#endif
			memset(strRevBuf, 0, sizeof(strRevBuf));
			g_iRevCounter = 0;

			cmdLen = 0x00;
			cmdAddr = 0x00;
			cmdLen++;
			cmd = 0x04;

			cmd_buf[cmdLen++] = cmdAddr;
			cmd_buf[cmdLen++] = cmd;
			//WNum	ENum	EPC	Mem	WordPtr	Wdt	Pwd	MaskAdr	MaskLen
			char WNum = 0x04;
			//ENum
			//char ENum = 0x06;
			char ENum = 0x03;
			//cmd_buf[cmdLen++] = WNum;
			cmd_buf[cmdLen++] = ENum;

			char accessPwd[4];
			accessPwd[0] = 0x00;
			accessPwd[1] = 0x00;
			accessPwd[2] = 0x00;
			accessPwd[3] = 0x00;
			memcpy(cmd_buf + cmdLen, accessPwd, 4); // Pwd
			cmdLen = cmdLen + 4;
			//EPC
			memcpy(cmd_buf + cmdLen, gv_Label + 1, ENum * 2);    // EPC 
			cmdLen = cmdLen + ENum * 2;
			cmd_buf[0] = cmdLen + 1;
			uiCrcValue = uiCrc16Cal((unsigned char *)cmd_buf, cmdLen);
			memcpy(cmd_buf + cmdLen, &uiCrcValue, 2);
			m_RfidPort.WriteToPort(cmd_buf, cmdLen + 2);
			m_WriteCounter++;
			if(m_WriteCounter>10)
			{
				CString strStatus;
				GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(strStatus);
				strStatus += "дRFID��ʱ\r\n";
				GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(strStatus);
                UpdateTestResult(m_strSnEdit,TEST_RFID_WRITE_SN,"2");
				gv_testcase = TEST_RFID_VIRIFY_SN_OVER;
			}

		    SetTimer(gv_testcase,1*500,NULL);
		}
		break;
	case TEST_RFID_FOREVER_READ_SN:
		 SetTimer(gv_testcase,1*500,NULL);
		//break;
	case TEST_RFID_VIRIFY_SN:
		{
			unsigned short int uiCrcValue = 0;
			char cmdLen;
			char cmdAddr;
			char cmd;
			static char cmd_buf[1024 + 5];	
			UpdateData(true);
			memset(strRevBuf, 0, sizeof(strRevBuf));
			g_iRevCounter = 0;
		    //GetDlgItem(IDC_EDIT2)->SetWindowText(m_strReceive);
			memset(cmd_buf, 0, sizeof(cmd_buf));
			cmdLen = 0x00;
			cmdAddr = 0xFF;
			cmdLen++;
			//cmd = 0x01;
			cmd = 0x0f;
			cmdLen++;
			cmd_buf[1] = cmdAddr;
			cmd_buf[2] = cmd;
			cmd_buf[0] = cmdLen + 2;
			uiCrcValue = uiCrc16Cal((unsigned char *)cmd_buf, cmdLen+1);
			memcpy(cmd_buf + cmdLen + 1, &uiCrcValue, 2);
			m_RfidPort.WriteToPort(cmd_buf, cmdLen+3);
		}
		break;
	case TEST_RFID_VIRIFY_SN_OVER:
		{
			OnOpenClosePort(FALSE);
			GetDlgItem(IDC_EDIT_SN)->SetWindowText("");
			GetDlgItem(IDC_EDIT_SN)->EnableWindow(1);
		}
		KillTimer(nIDEvent);
		break;
	default:
		break;
	}
    //GetDlgItem(IDC_EDIT_SN)->EnableWindow(1);
	GetDlgItem(IDC_EDIT_SN)->SetFocus();
	UpdateData(false);
	CDialog::OnTimer(nIDEvent);
}

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
	CString strsend,strTmp;

	m_strReceive += DevideHexChar(ch);
	strRevBuf[g_iRevCounter++] = ch;
	if ((strRevBuf[0] + 1) == g_iRevCounter)
	{
		char RspLen;
		char RspAddr;
		char Rsp;
		char RspStatus;
		static int try_write_epc_counter=0;
		g_iRevCounter = strRevBuf[0];
		// here handle recieve data
		RspLen = strRevBuf[0];
		RspAddr = strRevBuf[1];
		Rsp = strRevBuf[2];
		RspStatus = strRevBuf[3];
		switch (Rsp)
		{
		case 0x01: // ��ѯ��ǩ
		case 0x0f: //ѯ�鵥�ű�ǩ
			if ((RspStatus == 0x01) || (RspStatus == 0x02))
			{
				char Num = strRevBuf[4];
				int pos = 5;
				int EpcLen =0;
				unsigned char EpcBuf[15]={0};
				memset(gv_Label, 0, sizeof(gv_Label));
				//E20020170525000000001384
				for (int i = 0; i < Num; i++)
				{
					CString strTemp="";
					CString strStatus="";
					CString strTmp1;
					GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(strTemp);
					EpcLen = strRevBuf[pos++];
					memset(gv_Label,0,sizeof(gv_Label));
					memcpy(gv_Label, &strRevBuf[pos], EpcLen);
					strTmp1 = "";
					for(int i = 0; i<EpcLen;i++)
					{
						strTmp1 += DevideHexChar(gv_Label[i]);
					}
					strStatus = "Read  Sn : "+ strTmp1.Left(9)+ "\r\n" +strTemp;
					if(gv_testcase==TEST_RFID_VIRIFY_SN)
					{
						KillTimer(gv_testcase);
						strTmp1= strTmp1.Left(m_strSnEdit.GetLength());
						strTemp = strTmp1;
                        gv_testcase = TEST_RFID_VIRIFY_SN_OVER;
						if(strTmp1.Compare(m_strSnEdit)!=0)
						{
							strStatus += "RFIDд����һ�£�ʧ�ܣ�����\r\n";
							UpdateTestResult(m_strSnEdit,TEST_RFID_WRITE_SN,"2");
						}
						else
						{
                            strStatus += "RFIDдSN�ɹ�������\r\n";
							UpdateTestResult(m_strSnEdit,TEST_RFID_WRITE_SN,"3");
						}
		                SetTimer(gv_testcase,1*500,NULL);
					}
					GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(strStatus);
				}
			}
			else
			{
					if(gv_testcase==TEST_RFID_VIRIFY_SN)
					{
						try_write_epc_counter++;
						if(try_write_epc_counter>10)
						{
							CString strTemp;
					        GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(strTemp);
							UpdateTestResult(m_strSnEdit,TEST_RFID_WRITE_SN,"2");
							strTemp += "RFID��SNʧ��!!!!\r\n";
					        try_write_epc_counter = 0;
						    gv_testcase = TEST_RFID_VIRIFY_SN_OVER;
						    SetTimer(gv_testcase,1*500,NULL);
							GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(strTemp);
							try_write_epc_counter = 0;
						}
						SetTimer(gv_testcase,1*500,NULL);
					}
			}
			memset(strRevBuf, 0, sizeof(strRevBuf));
			g_iRevCounter = 0;
			break;
		case 0x02: //������
			if (RspStatus == 0x00)
			{
				char readData[128];
				CString strTemp ;
				memset(readData, 0, sizeof(readData));
				memcpy(readData, strRevBuf + 4, strRevBuf[0] - 5);		
				GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(strTemp);	
				strTemp += "readData:";
				for (int m = 0; m < strRevBuf[0] - 5; m++)
						strTemp += DevideHexChar(strRevBuf[m+4]);
				GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(strTemp);
			}
			else
			{
				GetDlgItem(IDC_EDIT_STATUS)->SetWindowText("������ʧ��");
			}
			break;
		case 0x03: //д����
			if (RspStatus == 0x00)
			{
				CString strTemp;
				GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(strTemp);
				strTemp += "д���ݳɹ�";
				GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(strTemp);
			}
			else
			{
				CString strTemp;
				GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(strTemp);
				strTemp += "д����ʧ��";
				GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(strTemp);
			}
		case 0x04: //дEPC
			KillTimer(gv_testcase);
			if (RspStatus == 0x00)
			{
				gv_testcase = TEST_RFID_VIRIFY_SN;
                SetTimer(gv_testcase,1*500,NULL);
			}
			else
			{
				CString strTemp;
				GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(strTemp);
 
				if(try_write_epc_counter>10)
				{
					strTemp += "дEPCʧ��";
					try_write_epc_counter = 0;
                    UpdateTestResult(m_strSnEdit,TEST_RFID_WRITE_SN,"2");
					KillTimer(gv_testcase);
					gv_testcase = TEST_RFID_VIRIFY_SN_OVER;
					SetTimer(gv_testcase,1*500,NULL);
				}
				else
				{
	                try_write_epc_counter++;
					SetTimer(gv_testcase,1*500,NULL);
				}
				GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(strTemp);
			}
		case 0x05:
			break;
		case 0x06:
			if (RspStatus == 0x00)
			{
				CString strTemp;
				GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(strTemp);
				strTemp += "�趨�洢����д����״̬�ɹ�";
				GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(strTemp);
			}
			else
			{
				CString strTemp;
				GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(strTemp);
				strTemp += "�趨�洢����д����״̬ʧ��";
				GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(strTemp);
			}
			break;
		case 0x07:
			if (RspStatus == 0x00)
			{
				CString strTemp;
				GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(strTemp);
				strTemp += "������ɹ�";
				GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(strTemp);
			}
			else
			{
				CString strTemp;
				GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(strTemp);
				strTemp += "�����ʧ��";
				GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(strTemp);
			}
			break;
		case 0x08:
			if (RspStatus == 0x00)
			{
				CString strTemp;
				GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(strTemp);
				strTemp += "����EPC�Ŷ��趨�ɹ�";
				GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(strTemp);
			}
			else
			{
				CString strTemp;
				GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(strTemp);
				strTemp += "����EPC�Ŷ��趨ʧ��";
				GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(strTemp);
			}
			break;
		case 0x09:
			if (RspStatus == 0x00)
			{
				CString strTemp;
				GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(strTemp);
				strTemp += "��EPC�Ŷ��趨�ɹ�";
				GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(strTemp);
			}
			else
			{
				CString strTemp;
				GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(strTemp);
				strTemp += "��EPC�Ŷ��趨ʧ��";
				GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(strTemp);
			}
			break;
		case 0x0A:
			if (RspStatus == 0x00)
			{
				CString strTemp;
				GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(strTemp);
				strTemp += "�����ɹ�";
				GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(strTemp);
			}
			else if (RspStatus == (char)0x0E)
			{
				CString strTemp;
				GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(strTemp);
				strTemp += "�޶�д�����������";
				GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(strTemp);
			}
			else if (RspStatus == (char)0xfb)
			{
				CString strTemp;
				strTemp += "�޿��õĵ��ӱ�ǩ";
				GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(strTemp);
			}
			else
			{
				CString strTemp;
				GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(strTemp);
				strTemp += "����ʧ��";
				GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(strTemp);
			}
			break;
		case 0x0B:
			if (RspStatus == 0x00)
			{
				char ReadPro = strRevBuf[4];
				CString strTemp;										
				if (RspStatus == (char)0x00)
				{
					strTemp += "���ӱ�ǩû�б�����Ϊ������";
				}
				else if (RspStatus ==(char)0x01)
				{
					strTemp += "���ӱ�ǩ������Ϊ������";
				}					
				GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(strTemp);
			}
			else if (RspStatus == (char)0xfb)
			{
				CString strTemp;
				strTemp += "�޿��õĵ��ӱ�ǩ";
				GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(strTemp);
			}
			else
			{
				CString strTemp;
				GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(strTemp);
				strTemp += "���Ա�ǩ�Ƿ����ö�����ʧ��";
				GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(strTemp);
			}
			break;
		case 0x10:
			if (RspStatus == 0x00)
			{
				CString strTemp;
				GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(strTemp);
				strTemp += "д�ɹ�";
				GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(strTemp);
			}		
			else if (RspStatus == (char)0xfb)
			{
				CString strTemp;
				strTemp += "�޿��õĵ��ӱ�ǩ";
				GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(strTemp);
			}
			else
			{
				CString strTemp;
				GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(strTemp);
				strTemp += "дʧ��";
				GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(strTemp);
			}
			break;
			break;
		default:
				break;
		}
	}
	m_strReceive += " ";
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

void CCOMTOOLDlg::OnFileSettings()
{
	// TODO: Add your command handler code here
	CTestSettingsDlg dlg;
	dlg.m_strTestFreq = m_strTestFreq;
    dlg.m_strWorkFreq = m_strWorkFreq;
	dlg.m_strAdcSampleMaxValue = m_strAdcSampleMaxValue;
	dlg.m_strAdcSampleMinValue = m_strAdcSampleMinValue;
	dlg.m_strShutDownCurrentMaxValue = m_strShutDownCurrentMaxValue;
	dlg.m_strTCCCode = m_strTCCCode;
	dlg.m_strFactoryCode = m_strFactoryCode;
	//UpdateData(true);

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
	CString strStatus;
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

    temp=m_strRfidPortNO;
    temp.Delete(0,3);
	m_iRfPortNo = atoi(temp);

	OnOpenClosePort(FALSE);
	OnOpenClosePort(FALSE);
	//
	//GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(m_strTestStatus);
	//m_strTestStatus = m_strTestStatus+"*********************************\r\n";
    GetDlgItem(IDC_EDIT_STATUS)->SetWindowText("");
    GetDlgItem(IDC_EDIT_SN)->EnableWindow(0);

    gv_testcase = TEST_RFID_WRITE_SN;
	OnOpenClosePort(TRUE);

	GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(strStatus);
	strStatus = strStatus + "Write Sn : "+ m_strSnEdit+ "\r\n";
	GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(strStatus);

	m_WriteCounter = 0;

	SetTimer(gv_testcase,1*500,NULL);

}


void CCOMTOOLDlg::LoadSmallTestToolConfig(void)
{
    CString strPath;
	CString strTmp="1";
    char buf[1000]; 
    GetCurrentDirectory(1000,buf);//�õ���ǰ����·��  
	strPath = buf;
	strPath += "\\SmallBJ.ini";
#if 1
    ::GetPrivateProfileString(_T("SmallBJConfig"), _T("TestFreq"), _T("437000000"), m_strTestFreq.GetBuffer(MAX_PATH),MAX_PATH, strPath);
    ::GetPrivateProfileString(_T("SmallBJConfig"), _T("WorkFreq"), _T("2"), m_strWorkFreq.GetBuffer(MAX_PATH),MAX_PATH, strPath);
    ::GetPrivateProfileString(_T("SmallBJConfig"), _T("AdcSampleMaxValue"), _T("4048384"),m_strAdcSampleMaxValue.GetBuffer(MAX_PATH),MAX_PATH,strPath);  
    ::GetPrivateProfileString(_T("SmallBJConfig"), _T("AdcSampleMinValue"), _T("1000000"),m_strAdcSampleMinValue.GetBuffer(MAX_PATH),MAX_PATH, strPath);  
 // ::GetPrivateProfileString(_T("SmallBJConfig"), _T("ShutDownCurrentMaxValue"), _T("ShutDownCurrentMaxValue"),m_strShutDownCurrentMaxValue.GetBuffer(MAX_PATH),MAX_PATH, strPath);

	::GetPrivateProfileString(_T("SmallBJConfig"), _T("TCCCode"), _T("086100102"),m_strTCCCode.GetBuffer(MAX_PATH),MAX_PATH, strPath);  
    ::GetPrivateProfileString(_T("SmallBJConfig"), _T("FactoryCode"), _T("00001"),m_strFactoryCode.GetBuffer(MAX_PATH),MAX_PATH, strPath);

    ::GetPrivateProfileString(_T("SmallBJConfig"), _T("RFIDSaveSN"), _T("0"),strTmp.GetBuffer(MAX_PATH),MAX_PATH, strPath);

	GetDlgItem(IDC_COM_RFID)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_RFID_READ)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_RFPORT)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_RFID_READ)->ShowWindow(SW_SHOW);

#else
    ::GetPrivateProfileString(("SmallBJConfig"), ("TestFreq"), ("434000000"), m_strTestFreq.GetBuffer(MAX_PATH),MAX_PATH, strPath);  
    ::GetPrivateProfileString(("SmallBJConfig"), ("AdcSampleMaxValue"), ("2294612"),m_strAdcSampleMaxValue.GetBuffer(MAX_PATH),MAX_PATH,strPath);  
    ::GetPrivateProfileString(("SmallBJConfig"), ("AdcSampleMinValue"), ("1802342"),m_strAdcSampleMinValue.GetBuffer(MAX_PATH),MAX_PATH, strPath);  
  //  ::GetPrivateProfileString(("SmallBJConfig"), ("ShutDownCurrentMaxValue"), ("ShutDownCurrentMaxValue"),m_strShutDownCurrentMaxValue.GetBuffer(MAX_PATH),MAX_PATH, strPath);
#endif
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
	//CString m_ExeSQL = "INSERT INTO SmallBoard_Info VALUES ('"+m_strSn+"',0,0,0,0,0,'"+date+"'"+",0,0,0"")";

	CString m_ExeSQL = "INSERT INTO SmallBoard_Info VALUES ('"+m_strSn+"','"+date+"'"+",0"")";

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
	case TEST_RFID_WRITE_SN:
		 strField = "SBI_RFID_STATE" ;
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

void CCOMTOOLDlg::OnOpenClosePort(int openflag)
{
	static int SelPortNO = MaxSerialPortNum;
	
	CString temp;
	UpdateData(true);

	if(m_RfidPortNO.GetCount()==0)
	{
		return;
	}

	if(openflag==0)
	{
		   m_RfidPort.InitPort(this,SelPortNO);
	}
	else 
	{	
		int SelDataBits,SelStopBits;
		char SelParity;
		UpdateData(true);
	    temp=m_strRfidPortNO;
		temp.Delete(0,3);
		SelPortNO=atoi(temp);
		//SelBaudRate=115200;
	    //SelBaudRate=38400;
		SelDataBits=8;
		SelParity='N';
		SelStopBits=1;

		if(m_RfidPort.InitPort(this,SelPortNO,57600,SelParity,SelDataBits,SelStopBits,EV_RXCHAR|EV_CTS,512))
		{
			m_RfidPort.StartMonitoring();
		}
		else
			AfxMessageBox("�ô����Ѿ�������Ӧ�ó�����ռ��!\n��ѡ�������Ĵ���");

	}
}

void CCOMTOOLDlg::OnBnClickedRfidRead()
{
	// TODO: Add your control notification handler code here
	static int readrfid_tag = 0;
	if(readrfid_tag==0)
	{
		readrfid_tag = 1;
        GetDlgItem(IDC_EDIT_SN)->EnableWindow(0);
		gv_testcase = TEST_RFID_FOREVER_READ_SN;
		OnOpenClosePort(TRUE);
		SetTimer(gv_testcase,1*500,NULL);
		GetDlgItem(IDC_RFID_READ)->SetWindowText("RFIDֹͣ��ȡSN");
	}
	else
	{
		readrfid_tag = 0;
        GetDlgItem(IDC_EDIT_SN)->EnableWindow(1);
		gv_testcase = TEST_RFID_FOREVER_READ_SN;
		OnOpenClosePort(FALSE);
		KillTimer(gv_testcase);
        GetDlgItem(IDC_RFID_READ)->SetWindowText("RFID��ȡSN");
	}
}

#define BUFFER_SIZE  128
void CCOMTOOLDlg::ProcessPendingRead()
{
	int x=5;

	//���建����
	char buffer[BUFFER_SIZE];

	//��������
	int nReceived = m_pSocket->Receive(buffer,BUFFER_SIZE,0);
	buffer[nReceived] = 0;
	
	//���������б������ʾ����
	CString str;
	str.Format("%s",buffer);
//	m_ctrlMsgs.AddString(str);
//	MessageBox(str,"aa");
//	m_ctrlMsgs+=str;
	UpdateData(FALSE);

}

void CCOMTOOLDlg::ProcessSocketClose()
{
	  if(m_pSocket)
	  {
		  delete m_pSocket;
		  m_pSocket = NULL;
	  }
}