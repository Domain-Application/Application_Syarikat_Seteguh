#if !defined(AFX_SALARY_H__79BD2FB0_67E7_4871_8330_07406B8CEF44__INCLUDED_)
#define AFX_SALARY_H__79BD2FB0_67E7_4871_8330_07406B8CEF44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Salary.h : header file
//

#include "staff.h"
extern void PSlip1(long irec,FILE *ptr,DB *df);
enum salary {Sl_REFNO=1,Sl_DATE,Sl_DRIVER,Sl_BASICPAY,Sl_RATE,Sl_BASIC,Sl_ALLOWANCE,
             Sl_COMM, Sl_DEDUCTION,Sl_ADDITION,
             Sl_N1,Sl_A1,Sl_N2,Sl_A2,Sl_N3,Sl_A3,
             Sl_NET_SALARY,
             Sl_EPF_BOSS,Sl_EPF_STAFF,Sl_SOCSO_B,Sl_SOCSO_S,
             Sl_TOTAL,Sl_NET_EARN,Sl_ADVANCE,Sl_BALANCE,Sl_REMARKS};


/////////////////////////////////////////////////////////////////////////////
// CSalary dialog

class CSalary : public CStaff
{
// Construction
public:
    CSalary(UINT IDD,DB *pdf,int Idx,CWnd* pParent = NULL);   // standard constructor
    virtual void ShowInformation();
	void SetAttribute(int flag,int fld);		

// Dialog Data
    //{{AFX_DATA(CSalary)
    enum { IDD = IDD_SALARY };
        // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSalary)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CSalary)
    virtual BOOL OnInitDialog();
    afx_msg void OnDestroy();
    afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnComputeSalary();
	afx_msg void OnViewSalaryStatement();
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


#endif // !defined(AFX_SALARY_H__79BD2FB0_67E7_4871_8330_07406B8CEF44__INCLUDED_)
