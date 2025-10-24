#if !defined(AFX_SCR_UPDATE_H__FA7AA741_3D27_42D0_98A2_FACC2C21C156__INCLUDED_)
#define AFX_SCR_UPDATE_H__FA7AA741_3D27_42D0_98A2_FACC2C21C156__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Scr_Update.h : header file
//
#include "tio.h"
/////////////////////////////////////////////////////////////////////////////
// CScr_Update dialog

class CScr_Update : public CTio
{
// Construction
public:
	CScr_Update(UINT IDD,DB *pdf,int Idx,void(*p)(),CWnd* pParent = NULL);   // standard constructor
	virtual void OnSetWild();		

// Dialog Data
	//{{AFX_DATA(CScr_Update)
	enum { IDD = IDD_SCR_UPDATE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScr_Update)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CScr_Update)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCR_UPDATE_H__FA7AA741_3D27_42D0_98A2_FACC2C21C156__INCLUDED_)
