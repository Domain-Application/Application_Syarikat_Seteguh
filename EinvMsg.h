#if !defined(AFX_EINVMSG_H__692F910E_F81E_4499_BF6F_BD2BC05A9E9B__INCLUDED_)
#define AFX_EINVMSG_H__692F910E_F81E_4499_BF6F_BD2BC05A9E9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EinvMsg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEinvMsg dialog

class CEinvMsg : public CDialog
{
// Construction
public:
	CEinvMsg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEinvMsg)
	enum { IDD = IDD_EINVMSG };
	COleDateTime	m_DATE;
	CString	m_DOCMSG;
	CString	m_MSG;
	CString	m_REFNO;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEinvMsg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEinvMsg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EINVMSG_H__692F910E_F81E_4499_BF6F_BD2BC05A9E9B__INCLUDED_)
