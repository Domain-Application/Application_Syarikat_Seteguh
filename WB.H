#if !defined(AFX_WB_H__8B172581_7B26_11D3_B370_00001CD5F6A7__INCLUDED_)
#define AFX_WB_H__8B172581_7B26_11D3_B370_00001CD5F6A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Wb.h : header file
//
#include "staff.h"
/////////////////////////////////////////////////////////////////////////////
// CWb dialog

class CWb : public CStaff
{
// Construction
public:
    CWb(UINT IDD,DB *pdf,int Idx,CWnd* pParent = NULL);   // standard constructor
    void SetOneTimeScreen();
    void ShowInformation();
	virtual void after_save(int ExistFlag);
	virtual BOOL IsCannotEdit();

// Dialog Data
	//{{AFX_DATA(CWb)
    enum { IDD = IDD_RECEIVE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWb)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWb)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WB_H__8B172581_7B26_11D3_B370_00001CD5F6A7__INCLUDED_)
