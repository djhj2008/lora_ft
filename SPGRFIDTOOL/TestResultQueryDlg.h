#pragma once
#include "afxcmn.h"
#include "Paint_ADO.h"

// CTestResultQueryDlg dialog

class CTestResultQueryDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTestResultQueryDlg)

public:
	CTestResultQueryDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTestResultQueryDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_TESTRESULT_QUERY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_List;
	Paint_ADO m_Ado;
	void AddToList(int flag);
	int RunAdoSQL(CString m_strSQL);
	virtual BOOL DestroyWindow();
	afx_msg void OnBnClickedButtonNext();
	afx_msg void OnBnClickedButtonPrevious();
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	CString m_strSN;
	CString m_strTime;
	CString m_strATW;
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	
	afx_msg void OnBnClickedSearchButton();
	afx_msg void OnBnClickedListAll();
};
