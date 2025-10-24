#if !defined(AFX_WRCV_H__5BAD42C1_12AD_11D3_B36C_9E8348FB682E__INCLUDED_)
#define AFX_WRCV_H__5BAD42C1_12AD_11D3_B36C_9E8348FB682E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Wrcv.h : header file
//
#include "weighing.h"
/////////////////////////////////////////////////////////////////////////////
// CWrcv dialog

class CWrcv : public CWeighing
{
// Construction
public:
    CWrcv(UINT IDD,DB *pdf,int Idx,CWnd* pParent = NULL);   // standard constructor
    virtual void W1Start(long weight);
    virtual void W2Start(long weight);
    virtual void W2Saved();
	virtual void ShowInformation();

// Dialog Data
	//{{AFX_DATA(CWrcv)
	enum { IDD = IDD_ACCODE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWrcv)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWrcv)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WRCV_H__5BAD42C1_12AD_11D3_B36C_9E8348FB682E__INCLUDED_)
