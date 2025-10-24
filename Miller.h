#if !defined(AFX_MILLER_H__2A7BBB63_C28C_4129_989C_125D86FBE75D__INCLUDED_)
#define AFX_MILLER_H__2A7BBB63_C28C_4129_989C_125D86FBE75D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Miller.h : header file
//
#include "staff.h"
/////////////////////////////////////////////////////////////////////////////
// CMiller dialog

class CMiller : public CStaff
{
// Construction
public:
	CMiller(UINT IDD,DB *pdf,int Idx,CWnd* pParent = NULL);   // standard constructor
	virtual void after_save(int ExistFlag);		
	DB *main;

// Dialog Data
	//{{AFX_DATA(CMiller)
	enum { IDD = IDD_MILLER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMiller)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMiller)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MILLER_H__2A7BBB63_C28C_4129_989C_125D86FBE75D__INCLUDED_)
