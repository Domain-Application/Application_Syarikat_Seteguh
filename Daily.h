#if !defined(AFX_DAILY_H__4E911361_18C9_11D3_8243_B50274934C4B__INCLUDED_)
#define AFX_DAILY_H__4E911361_18C9_11D3_8243_B50274934C4B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Daily.h : header file
//
#include "tio.h"
/////////////////////////////////////////////////////////////////////////////
// CDaily dialog

class CDaily : public CTio
{
// Construction
public:
        CDaily(UINT IDD,DB *pdf,int Idx,void(*p)(),CWnd* pParent = NULL);   // standard constructor
        virtual void show_tio();

// Dialog Data
	//{{AFX_DATA(CDaily)
	enum { IDD = IDD_DAILY };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDaily)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDaily)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DAILY_H__4E911361_18C9_11D3_8243_B50274934C4B__INCLUDED_)
