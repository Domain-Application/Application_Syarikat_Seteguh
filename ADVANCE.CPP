// Advance.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Advance.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdvance dialog


CAdvance::CAdvance(UINT IDD,DB *pdf,int Idx,CWnd* pParent /*=NULL*/)
    : CStaff(IDD,pdf,Idx,pParent)
{
    //{{AFX_DATA_INIT(CAdvance)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}


void CAdvance::DoDataExchange(CDataExchange* pDX)
{
    CStaff::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAdvance)
        // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAdvance, CStaff)
    //{{AFX_MSG_MAP(CAdvance)
    ON_WM_DESTROY()
    ON_WM_TIMER()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdvance message handlers
void CAdvance::ShowInformation()
{
    CStaff::ShowInformation();
}


BOOL CAdvance::OnInitDialog()
{
    CStaff::OnInitDialog();

    // TODO: Add extra initialization here

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void CAdvance::OnDestroy()
{
    CStaff::OnDestroy();

    // TODO: Add your message handler code here

}

void CAdvance::OnTimer(UINT nIDEvent)
{
    // TODO: Add your message handler code here and/or call default

    CStaff::OnTimer(nIDEvent);
}


void AdminDbAdvance(DB *df)
{
    if(df->ID==0) {                           
        AfxMessageBox("Database not open");   
        return;                               
    }                                         
    CAdvance p1(df->ID,df,df->f[1].idx & 0x01);
    p1.DoModal();                             
}

