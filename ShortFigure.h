#if !defined(AFX_SHORTFIGURE_H__A394F34B_25B7_4702_9FD4_D537ECA2A370__INCLUDED_)
#define AFX_SHORTFIGURE_H__A394F34B_25B7_4702_9FD4_D537ECA2A370__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShortFigure.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CShortFigure dialog

class CShortFigure : public CDialog
{
// Construction
public:
	CShortFigure(UINT IDD,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CShortFigure)
	enum { IDD = IDD_OER_PERCENT };
	double	m_value;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShortFigure)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CShortFigure)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHORTFIGURE_H__A394F34B_25B7_4702_9FD4_D537ECA2A370__INCLUDED_)
