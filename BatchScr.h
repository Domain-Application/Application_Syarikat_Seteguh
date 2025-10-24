#if !defined(AFX_BATCHSCR_H__A538043A_1F13_4330_AD40_B13C374A9444__INCLUDED_)
#define AFX_BATCHSCR_H__A538043A_1F13_4330_AD40_B13C374A9444__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BatchScr.h : header file
//
#include "tio.h"
/////////////////////////////////////////////////////////////////////////////
// CBatchScr dialog

class CBatchScr : public CTio
{
// Construction
public:
	CBatchScr(UINT IDD,DB *pdf,int Idx,void(*p)(),CWnd* pParent = NULL);   // standard constructor
	virtual void OnSetWild();

// Dialog Data
	//{{AFX_DATA(CBatchScr)
	enum { IDD = IDD_SCR_BATCH };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBatchScr)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBatchScr)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BATCHSCR_H__A538043A_1F13_4330_AD40_B13C374A9444__INCLUDED_)
