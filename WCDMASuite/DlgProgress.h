#if !defined(AFX_DLGPROGRESS_H__230CE8B3_ED88_4CF7_92E3_510C0CE6618A__INCLUDED_)
#define AFX_DLGPROGRESS_H__230CE8B3_ED88_4CF7_92E3_510C0CE6618A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgProgress.h : header file
//
#include "BmpBkDialog.h"


/////////////////////////////////////////////////////////////////////////////
// CDlgProgress dialog
//��ʼ������
void InitProgress(int nMax = NULL);

//��ʾ����
void ShowProgress(int nProgress, CString strInfo = "");

//��������
void EndProgress();

class CDlgProgress : public CDialog
{
// Construction
public:
	CDlgProgress(CWnd* pParent = NULL);   // standard constructor
	void SetMaxProgress(int nMax);
	//���õ�ǰ������Ϣ
	void SetProgress(int nProgress, const CString& strInfo = "");
	CBrush  m_whiteBk ;
// Dialog Data
	//{{AFX_DATA(CDlgProgress)
	enum { IDD = IDD_PROGRESS };
	CStatic	m_ctrlProgress2;
	CProgressCtrl	m_ctrlProgress;
	CString	m_strInfo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgProgress)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgProgress)
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPROGRESS_H__230CE8B3_ED88_4CF7_92E3_510C0CE6618A__INCLUDED_)
