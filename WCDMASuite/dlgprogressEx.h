#ifndef AFX_DLGPROGRESS_2A99F42552C34768A07411708CF67EEE
#define AFX_DLGPROGRESS_2A99F42552C34768A07411708CF67EEE

#include "Thread.h"
#include "Resource.h"
#include "GlobalDataCenter.h"
//#include "MyButton.h"

class CDlgProgressEx;

/**
*	�����߳���.
*	������ CDlgProgressEx ����ϣ���ɶ��߳�
*	������ʾ������
*/	
class CPrgressThread: public CThread
{
public:
	CPrgressThread()
		:m_bCancel(false),
		 m_pUI(0)
	{
	}

	void Cancel(){m_bCancel = true;}
	void SetUI(CDlgProgressEx* pDlg)
	{
		m_pUI = pDlg;
	}
protected:
	virtual void Go();
	virtual void DoWork() = 0;
protected:
	CDlgProgressEx* m_pUI;	// CDlgProgressEx �������
	bool m_bCancel;			// �Ƿ��û�ѡ���� cancel
};

/**
*	���ȶԻ����ࡣ
*	�öԻ�����ģ̬��ʽ��ʾ���ڲ�����һ���߳�
*/
class CDlgProgressEx : public CDialog
{
public:
	//���캯��
	CDlgProgressEx(
		CPrgressThread& thread, //�̶߳�������ã��ڶԻ����ʼ��ʱ����
		CWnd* pParent = NULL,	//parent window
		bool canCancel = false);//�Ƿ��ܹ�cancel
		CBrush  m_whiteBk ;
	//���ý���
	void SetProgress(
		int nProgress,					//��ǰ����
		int nMax,						//�ܽ���
		const CString& strInfo = "");	//������Ϣ

		
private:
	void CloseDlg();

// Dialog Data
	//{{AFX_DATA(CDlgProgress)
	enum { IDD = IDD_PROGRESS };
	CProgressCtrl	m_ctrlProgress;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgProgress)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_VIRTUAL
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgProgress)

protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgProgress)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	bool			m_bCanCancel;
	CColorBtn			m_tbnCancel;
	CPrgressThread& m_Thread;

	friend class CPrgressThread;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPROGRESS_H__E82ED040_F058_11D8_9081_0010A40A18A6__INCLUDED_)
