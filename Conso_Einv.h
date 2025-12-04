#if !defined(AFX_CONSO_EINV_H__0D9F5AB1_3A08_44D0_9749_99613D6AA7D8__INCLUDED_)
#define AFX_CONSO_EINV_H__0D9F5AB1_3A08_44D0_9749_99613D6AA7D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Conso_Einv.h : header file
//
#include "parent.h"
/////////////////////////////////////////////////////////////////////////////
// CConso_Einv dialog

class CConso_Einv : public CParent
{
// Construction
public:
	CConso_Einv(UINT IDD,DB *pdf,int Idx,CWnd* pParent = NULL);   // standard constructor
	virtual void ShowInitFrame();
	virtual void ShowInitFrame1();
    virtual void ReplaceSubGrid(long rec,long irec,CMSFlexGrid *m_grid,DB *sub,int IdxItem);
    virtual void ShowInformation();

// Dialog Data
	//{{AFX_DATA(CConso_Einv)
	enum { IDD = IDD_CONSO_EINV };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConso_Einv)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConso_Einv)
	virtual BOOL OnInitDialog();
	afx_msg void OnLock();
	afx_msg void OnUnLock();
	afx_msg void OnImportData();
	afx_msg void OnGetQRCODE();
	afx_msg void OnSubmitEinv();
	afx_msg void OnRefreshmsg();
	afx_msg void OnImportRangeInv();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONSO_EINV_H__0D9F5AB1_3A08_44D0_9749_99613D6AA7D8__INCLUDED_)
