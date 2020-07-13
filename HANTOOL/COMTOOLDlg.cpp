
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
{
	m_strReceive = "";

	m_strStatus = "关闭";

	m_strPortNO = "";

	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	gv_testcase = TEST_RECVICE_STR;
	License_max_value=0;
	License_current_value=0;
	License_current_backup_value=0;

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCOMTOOLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COM, m_PortNO);
	DDX_CBString(pDX, IDC_COM, m_strPortNO);
	DDX_Control(pDX, IDC_PIC, m_retPicture);
}

BEGIN_MESSAGE_MAP(CCOMTOOLDlg, CDialogEx)
	//{{AFX_MSG_MAP(CCOMTOOLDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ABOUT, OnAbout)
	ON_BN_CLICKED(IDC_QUIT, OnQuit)
	ON_BN_CLICKED(IDC_ClearR, OnClearReceiveEdit)

	ON_MESSAGE(WM_COMM_RXCHAR,OnReceiveChar)
	ON_WM_TIMER()

	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_Send, OnSend)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_SETTINGS, &CCOMTOOLDlg::OnFileSettings)
	ON_COMMAND(ID_Menu_TEST_RESULT_QUERY, &CCOMTOOLDlg::OnMenuTestResultQuery)

	ON_WM_CTLCOLOR()
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

    CString title = "整机测试工具(v301)";        
    SetWindowText(title);

	//设置静态文本字体大小

	m_editFont.CreatePointFont(160, _T("宋体"));
	CEdit* pEdt = (CEdit*)GetDlgItem(IDC_EDIT_STATUS);
	CEdit* pEdt1 = (CEdit*)GetDlgItem(IDC_EDIT1);
	pEdt->SetFont(&m_editFont);
	pEdt1->SetFont(&m_editFont);

	/*
	int ret=UpdateADCResult("000050014","22","33","31","-38","-80");
	if (ret == -1)
	{

	}
	else
	{
		CString temp_str = "成功 ADC:22,33,44";
		GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(temp_str);
		GetDlgItem(IDC_EDIT1)->SetWindowText("SN:000050014");
	}
	*/
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


	LoadSmallTestToolConfig();
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
	OnOpenClosePort(FALSE);
	OnOpenClosePort(FALSE);
	Sleep(1000);
	PostQuitMessage(0);
}


void CCOMTOOLDlg::OnClearReceiveEdit() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	m_strReceive="";
	UpdateData(false);
}



#define  TIMELIMIT 200
void CCOMTOOLDlg::OnSend() 
{
	// TODO: Add your control notification handler code here
	int ret = 0;
	//对多次单击进行时间间隔处理
	if (GetTickCount() - dwStartOk < TIMELIMIT)
	{
		dwStartOk = ::GetTickCount();
		return;
	}
	dwStartOk = ::GetTickCount();

	//OnReceiveChar(0x65,1);
	//OnReceiveChar(0x0d,1);
	//OnReceiveChar(0x0a,1);
	/*
	HWND hWnd = ::FindWindowW(L"Notepad", NULL); //搜索记事本程序主窗口句柄 
	HWND hWndc = ::GetWindow(hWnd, GW_CHILD); //获得记事本客户区句柄（该窗口是记事本主窗口的子窗口，即那个白色的可编辑区域） 
	if (hWndc){
		::SendMessage(hWndc, WM_CHAR, '0', NULL);//发送按键消息
		::SendMessage(hWndc, WM_CHAR, '0', NULL);//发送按键消息
		::SendMessage(hWndc, WM_CHAR, '0', NULL);//发送按键消息
		::SendMessage(hWndc, WM_CHAR, '3', NULL);//发送按键消息
		::SendMessage(hWndc, WM_CHAR, '0', NULL);//发送按键消息
		::SendMessage(hWndc, WM_CHAR, '0', NULL);//发送按键消息
		::SendMessage(hWndc, WM_CHAR, '2', NULL);//发送按键消息
		::SendMessage(hWndc, WM_CHAR, '8', NULL);//发送按键消息
		::SendMessage(hWndc, WM_CHAR, '1', NULL);//发送按键消息
	}
	*/

	if (m_strStatus == "关闭")
	{
		ret = OnOpenClosePort(TRUE);
		if (ret == 0) {
			GetDlgItem(IDC_Send)->SetWindowText("关闭");
			m_strStatus = "开始";
			GetDlgItem(IDC_COM)->EnableWindow(FALSE);

		}
		else {
			AfxMessageBox("该串口已经被其他应用程序所占用!\n请选择其它的串口");
		}

	}
	else {
		GetDlgItem(IDC_Send)->SetWindowText("开始");
		m_strStatus = "关闭";
		ret = OnOpenClosePort(FALSE);
		GetDlgItem(IDC_COM)->EnableWindow(TRUE);
	}

	//OnOpenClosePort(TRUE);
	return;
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
	case TEST_ADC_SAMPLE:
		  //UpdateTestResult(m_strSnEdit,TEST_ADC_SAMPLE,"2");
		  //GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(m_strTestStatus);
		  //m_strTestStatus = m_strTestStatus+"ADC采样测试失败\r\n";
          //GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strTestStatus);
		  break;
	case TEST_SUCCEED_WAIT_NEXT:
		{
			//KillTimer(nIDEvent);
			//OnOpenClosePort(FALSE);
            //OnOpenClosePort(FALSE);
		}
		break;
	default:
		break;
	}
    //GetDlgItem(IDC_EDIT_SN)->EnableWindow(1);
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
	int pos1, pos2, pos3, pos4, pos5, pos6, pos7, pos8, pos9, pos10;
	CString strsend,strTmp;
    GetDlgItem(IDC_EDIT2)->GetWindowText(m_strReceive);
	if (gv_testcase == TEST_WAIT_CMD) {
		if (ch == 0x0a)
		{
			gv_testcase = TEST_ADC_SAMPLE;
		}
		else 
		{
			gv_testcase = TEST_RECVICE_STR;
		}

	}
	if (ch == 0x0d)
	{
		gv_testcase = TEST_WAIT_CMD;
	}
			
	
	m_strReceive += (char)ch;
	m_strCMD += (char)ch;
	
    GetDlgItem(IDC_EDIT2)->SetWindowText(m_strReceive);
	//m_strCMD = "PGSN:000050003;Temp:22,26,21;Rssi:-58;sRssi:-68\r\n\r\n";
	switch(gv_testcase)
	{
	case TEST_ADC_SAMPLE:
		//AT+TEMP:7115b0,7066f6
		pos1 = m_strCMD.Find("PGSN:");
		if(pos1>=0)
		{
		  GetDlgItem(IDC_EDIT2)->SetWindowText(m_strCMD);
		  pos2 = m_strCMD.Find(";",pos1);
		  if(pos2>=0)
		  {
				pos3 = m_strCMD.Find("Temp:",pos2+1);
				if(pos3>=0)
				{
					 pos4 = m_strCMD.Find(",",pos3+1);
					 pos5 = m_strCMD.Find(",", pos4+1);
					 pos6 = m_strCMD.Find(";", pos5+1);
					 pos7 = m_strCMD.Find("Rssi:", pos6 + 1);
					 pos8 = m_strCMD.Find(";", pos7 + 1);
					 pos9 = m_strCMD.Find("sRssi:", pos8 + 1);
					 pos10 = m_strCMD.Find("\r\n", pos9 + 6);
					 if(pos4>=0)
					 {
						 CString str_sn, str_temp1, str_temp2, str_temp3, str_rssi, str_rssi2;
						 int adc1, adc2, adc3, rssi, rssi2;

						str_sn = m_strCMD.Mid(pos1+5,pos2-(pos1+5));
						str_temp1 = m_strCMD.Mid(pos3+5,pos4-(pos3+5));
						str_temp2 = m_strCMD.Mid(pos4+1,pos5-pos4-1);
						str_temp3 = m_strCMD.Mid(pos5+1,pos6-pos5-1);
						str_rssi = m_strCMD.Mid(pos7+5, pos8-(pos7+5));
						str_rssi2 = m_strCMD.Mid(pos9 + 6, pos10 - (pos9 + 6));
						adc1 = atoi(str_temp1);
						adc2 = atoi(str_temp2);
						adc3 = atoi(str_temp3);
						rssi = atoi(str_rssi);
						rssi2 = atoi(str_rssi2);
						//GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strCMD);
						//GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(m_strTestStatus);
						CString cur_time; //获取系统时间 　　

						CTime tm; tm = CTime::GetCurrentTime();

						cur_time = tm.Format("%Y-%m-%d %X");

						strTmp.Format("%s \r\n温度值:%d;%d;%d Rssi:%d sRssi:%d", cur_time, adc1, adc2, adc3, rssi, rssi2);

						m_strCMD.Empty();
						gv_testcase = TEST_RECVICE_STR;
						KillTimer(gv_testcase);

						int find_sn = UpdateADCResult(str_sn, str_temp1, str_temp2, str_temp3, str_rssi,str_rssi2);

						if (find_sn == -1) 
						{
							break;
						}

						m_strReceive.Empty();
						m_strResult = "";

						if(((adc1<atoi(m_strAdcSampleMaxValue))&&(adc1>atoi(m_strAdcSampleMinValue)))&&
							((adc2<atoi(m_strAdcSampleMaxValue))&&(adc2>atoi(m_strAdcSampleMinValue)))&&
							(rssi >= atoi(m_strRssiValue)) && (rssi2 >= atoi(m_strRssiValue2)))
						{
							UpdateTestResult(str_sn,TEST_ADC_SAMPLE,"1");
							UpdateTestPic(TRUE);
							strTmp = "成功:"+ strTmp+"\r\n";
							m_strResult = m_strTestStatus+strTmp+ m_strResult;
							GetDlgItem(IDC_EDIT1)->SetWindowText("SN:"+str_sn);
							GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strResult);
							//SetTimer(gv_testcase,20*1000,NULL);
						}
						else
						{
							UpdateTestResult(str_sn,TEST_ADC_SAMPLE,"2");
							UpdateTestPic(FALSE);
							strTmp = "失败:"+ strTmp +"\r\n";
							m_strResult = m_strTestStatus+strTmp+ m_strResult;
							GetDlgItem(IDC_EDIT1)->SetWindowText("SN:" + str_sn);
							GetDlgItem(IDC_EDIT_STATUS)->SetWindowText(m_strResult);
							//GetDlgItem(IDC_EDIT2)->SetWindowText("");
						}
					}
				}
		  }

		}
		else {
			m_strCMD.Empty();
			m_strReceive.Empty();
			gv_testcase = TEST_RECVICE_STR;
		}
		break;
	case TEST_RECVICE_STR:
	    if(m_strCMD.GetLength()>1024)
	    {
			m_strCMD.Empty();
			m_strReceive.Empty();
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
	dlg.m_strAdcSampleMaxValue = m_strAdcSampleMaxValue;
	dlg.m_strAdcSampleMinValue = m_strAdcSampleMinValue;
	dlg.m_strRssiValue = m_strRssiValue;
	dlg.m_strRssiValue2 = m_strRssiValue2;
	//UpdateData(true);
	//if(m_bRfidSaveSn)
	//OnOpenClosePort(TRUE,FALSE);
	//OnOpenClosePort(FALSE,FALSE);

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





void CCOMTOOLDlg::LoadSmallTestToolConfig(void)
{
    CString strPath;
	CString strTmp="1";
    char buf[1000]; 
    GetCurrentDirectory(1000,buf);//得到当前工作路径  
	strPath = buf;
	strPath += "\\SmallBJ.ini";

    ::GetPrivateProfileString(_T("SmallBJConfig"), _T("AdcSampleMaxValue"), _T("10"),m_strAdcSampleMaxValue.GetBuffer(MAX_PATH),MAX_PATH,strPath);  
    ::GetPrivateProfileString(_T("SmallBJConfig"), _T("AdcSampleMinValue"), _T("40"),m_strAdcSampleMinValue.GetBuffer(MAX_PATH),MAX_PATH, strPath); 
	::GetPrivateProfileString(_T("SmallBJConfig"), _T("RssiValue"), _T("-70"), m_strRssiValue.GetBuffer(MAX_PATH), MAX_PATH, strPath);
	::GetPrivateProfileString(_T("SmallBJConfig"), _T("RssiValue2"), _T("-70"), m_strRssiValue2.GetBuffer(MAX_PATH), MAX_PATH, strPath);

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
	CString m_ExeSQL = "INSERT INTO SmallBoard_Info VALUES ('"+m_strSn+"',0,0,0,0,0,'"+date+"'"+",0,0,0"")";
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
				int rtc = m_Ado.m_pRecordset->GetCollect("SBI_RTC");
				int adc = m_Ado.m_pRecordset->GetCollect("SBI_ADC");
				int lora_rt = m_Ado.m_pRecordset->GetCollect("SBI_LORA_RT");
				int atw_state = m_Ado.m_pRecordset->GetCollect("SBI_ATW_STATE");
				CString strTime = (char*)(_bstr_t)m_Ado.m_pRecordset->GetCollect("SBI_TIME");
				if((rtc==1)&&(adc==1)&&(lora_rt==1)&&(atw_state==1)) // 所有测试都已经成功，重复扫描，测试不往下进行。如果需要重新测试，先从数据库里把这条记录删除
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
		break;
	case TEST_ADC_SAMPLE:
		strField = "SBI_ADC" ;
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


int CCOMTOOLDlg::UpdateADCResult(CString m_strSn,CString m_strADC1,CString m_strADC2,CString m_strADC3,CString m_strRssi,CString m_strRssi2)
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
	CString m_ExeSQL = "INSERT INTO SmallBoard_Info VALUES ('" + m_strSn + "',0,0,0,0,0,0,'" + date + "'" + ",0,0,0"")";
	try
	{
		m_Ado.m_pRecordset = m_Ado.OpenRecordset(m_ExeSQL);
		m_Ado.CloseRecordset();
		m_Ado.CloseConn();
	}
	catch (...)
	{
		//检查是否存在
		CString m_ExeSQL = "select * from SmallBoard_Info WHERE SBI_SN='" + m_strSn + "'";
		try
		{
			m_Ado.m_pRecordset = m_Ado.OpenRecordset(m_ExeSQL);
			if (m_Ado.GetRecordCount(m_Ado.m_pRecordset) == 1)
			{
				return -1;
			}
			m_Ado.CloseRecordset();
			m_Ado.CloseConn();
		}
		catch (...)
		{
		}
	}

	m_Ado.OnInitADOConn();
	CString m_ExeSQL2 = "UPDATE SmallBoard_Info SET SBI_LORA_RT =" + m_strRssi + " , SBI_LORA_RT2 = " + m_strRssi2 + " , ADC1 = " + m_strADC1 + ", ADC2 = " + m_strADC2 + ", ADC3 = " + m_strADC3 + " WHERE SBI_SN='" + m_strSn + "'";
	try
	{
	    m_Ado.m_pRecordset = m_Ado.OpenRecordset(m_ExeSQL2);
		m_Ado.CloseRecordset();
		m_Ado.CloseConn();
	}
	catch (...)
	{
		//MessageBox("测试结果保存失败!","提示!",MB_ICONWARNING | MB_OK);
		//检查是否存在
	}
	return 0;
}

void CCOMTOOLDlg::UpdateTestPic(BOOL ret) 
{
	// TODO: Add your control notification handler code here   
	CBitmap bitmap;  // CBitmap对象，用于加载位图   
	HBITMAP hBmp;    // 保存CBitmap加载的位图的句柄   
	if (ret)
	{
		bitmap.LoadBitmap(IDB_BITMAP2);  // 将位图IDB_BITMAP1加载到bitmap   
	}
	else 
	{
		bitmap.LoadBitmap(IDB_BITMAP3);  // 将位图IDB_BITMAP1加载到bitmap   
	}
	hBmp = (HBITMAP)bitmap.GetSafeHandle();  // 获取bitmap加载位图的句柄   
	m_retPicture.SetBitmap(hBmp);    // 设置图片控件m_jzmPicture的位图图片为
}


int CCOMTOOLDlg::OnOpenClosePort(int openflag)
{
	static int SelPortNO = MaxSerialPortNum;
	
	CString temp;
	UpdateData(true);

	if(m_PortNO.GetCount()==0)
	{
		return -1;
	}

	if(openflag==0)
	{
		m_SerialPort.InitPort(this,SelPortNO);
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

		//if(m_SerialPort.InitPort(this,SelPortNO,128000,SelParity,SelDataBits,SelStopBits,EV_RXCHAR|EV_CTS,512))		
        if(m_SerialPort.InitPort(this,SelPortNO,115200,SelParity,SelDataBits,SelStopBits,EV_RXCHAR|EV_CTS,512))	
		{
			m_SerialPort.StartMonitoring();
			return 0;
		}
		else
		{
			return 1;
			//AfxMessageBox("该串口已经被其他应用程序所占用!\n请选择其它的串口");

		}
	}
}


HBRUSH CCOMTOOLDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_EDIT1:
		pDC->SetTextColor(RGB(0, 0, 255));//设置他的文本显示
		break;
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
