//{{AFX_INCLUDES()
#include "mdctext.h"
#include "gwedit.h"
#include "msflexgrid.h"

//}}AFX_INCLUDES
#if !defined(AFX_GETWT_H__3BE40621_945B_11D2_B1F7_D42340A919EF__INCLUDED_)
#define AFX_GETWT_H__3BE40621_945B_11D2_B1F7_D42340A919EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GetWt.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGetWt dialog

class CGetWt : public CDialog
{
// Construction
public:
	CGetWt(CWnd* pParent = NULL);   // standard constructor
    DB   *df;
    void SetGridTitle();
    void SetActiveStatus(long rec);
    void ShowGridContent();
    long    weight,SwCount,stCount,cwTime;
    long    hcount,hwt[12];
    virtual void OnCancel();
    virtual void OnOK();
    BOOL    m_WtFlag,m_Status,m_zero;
    HANDLE hComm;
    COMMPROP cp;
    COMMTIMEOUTS to;
    COMMCONFIG cc;

// Dialog Data
	//{{AFX_DATA(CGetWt)
	enum { IDD = IDD_GW };
	CStatic	m_traffic;
    CBitmapButton m_ok;
    CBitmapButton m_cancel;
	CGwEdit	m_gwedit;
    CMdcText m_wt;
	CString	m_vehicle;
	CMSFlexGrid	m_grid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGetWt)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGetWt)
	virtual BOOL OnInitDialog();
    afx_msg int OnGw();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClickMsflexgrid1();
	afx_msg void OnDblClickMsflexgrid1();
	afx_msg void OnButton1();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GETWT_H__3BE40621_945B_11D2_B1F7_D42340A919EF__INCLUDED_)
