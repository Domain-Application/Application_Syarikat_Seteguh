//{{AFX_INCLUDES()
#include "msflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_AUTOINVOICE_H__03DBF920_A638_4F6E_A36E_20FDE3B1DCD4__INCLUDED_)
#define AFX_AUTOINVOICE_H__03DBF920_A638_4F6E_A36E_20FDE3B1DCD4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AutoInvoice.h : header file
//

extern double compute_qty(DB *df);
#include "mgrid.h"
/////////////////////////////////////////////////////////////////////////////
// CAutoInvoice dialog

class CAutoInvoice : public CDialog
{
// Construction
public:
    void SetGridTitle();
    void SetActiveStatus();
	void DoPoInvoice();
	BOOL IsInvoiceExist();
	CString str;
	CString d1,d2;
    char szDate1[20];
    BOOL OnInitDialog();
    MGRID fg;
	void DoAutoGenerateInvoice();
	CAutoInvoice(CWnd* pParent = NULL);   // standard constructor
    void FindLastDate();
	DB *df,*invmain,*invsub;

// Dialog Data
	//{{AFX_DATA(CAutoInvoice)
	enum { IDD = IDD_AUTOINV };
	CProgressCtrl	m_progress;
	COleDateTime	m_d1;
	COleDateTime	m_d2;
	CString	m_nextInvoiceNo;
	CMSFlexGrid	m_grid;
	COleDateTime	m_invdate;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoInvoice)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAutoInvoice)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOINVOICE_H__03DBF920_A638_4F6E_A36E_20FDE3B1DCD4__INCLUDED_)
