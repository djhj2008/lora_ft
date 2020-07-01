// ChatSocket.cpp : implementation file
//

#include "stdafx.h"
#include "ComTool.h"
#include "ChatSocket.h"
#include "COMTOOLDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChatSocket

CChatSocket::CChatSocket()
{
}

CChatSocket::~CChatSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CChatSocket, CSocket)
	//{{AFX_MSG_MAP(CChatSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CChatSocket member functions
CChatSocket::CChatSocket(CCOMTOOLDlg* pDlg)
{
	m_pDlg = pDlg;
}

//֪ͨ�ͻ��˿��Խ���������
void CChatSocket::OnReceive(int nErrorCode) 
{
	CSocket::OnReceive(nErrorCode);
	//�����Ի���������
	if(m_pDlg)
		m_pDlg->ProcessPendingRead();
}


void CChatSocket::OnClose(int nErrorCode) 
{
	CSocket::OnClose(nErrorCode);
	//�����Ի���������
	if(m_pDlg)
		m_pDlg->ProcessSocketClose();
}