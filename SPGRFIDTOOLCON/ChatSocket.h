#if !defined(AFX_CHATSOCKET_H__5FA83441_8E03_4606_A25B_3250573B2943__INCLUDED_)
#define AFX_CHATSOCKET_H__5FA83441_8E03_4606_A25B_3250573B2943__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChatSocket.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CChatSocket command target
class CCOMTOOLDlg;
class CChatSocket : public CSocket
{
// Attributes
public:
	CChatSocket (CCOMTOOLDlg* pDlg);   // �½��Ĺ��캯��
// Operations
public:
	CChatSocket();
	virtual ~CChatSocket();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChatSocket)
	public:
	virtual void OnReceive(int nErrorCode);    // ����
	virtual void OnClose(int nErrorCode);      // �ر�

	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CChatSocket)
	// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
	CCOMTOOLDlg * m_pDlg;               // ��Ա����
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHATSOCKET_H__5FA83441_8E03_4606_A25B_3250573B2943__INCLUDED_)
