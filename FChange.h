#if !defined(AFX_FCHANGE_H__AFB81D01_1FEF_11D3_8243_C1A84EE0C74A__INCLUDED_)
#define AFX_FCHANGE_H__AFB81D01_1FEF_11D3_8243_C1A84EE0C74A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FChange.h : header file
//
#include "tio.h"
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CFChange dialog

class CFChange : public CTio
{
// Construction
public:
        CFChange(UINT IDD,DB *pdf,int Idx=0,void(*p)()=NULL,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFChange)
	enum { IDD = IDD_FCHANGE };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFChange)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFChange)
		// NOTE: the ClassWizard will add member functions here
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FCHANGE_H__AFB81D01_1FEF_11D3_8243_C1A84EE0C74A__INCLUDED_)
