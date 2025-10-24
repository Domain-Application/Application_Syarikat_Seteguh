// Payment.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Payment.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPayment dialog


CPayment::CPayment(UINT IDD,DB *pdf,int Idx,CWnd* pParent /*=NULL*/)
    : CStaff(IDD,pdf,Idx,pParent)
{
    //{{AFX_DATA_INIT(CPayment)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}


void CPayment::DoDataExchange(CDataExchange* pDX)
{
    CStaff::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPayment)
        // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPayment, CStaff)
    //{{AFX_MSG_MAP(CPayment)
    ON_WM_DESTROY()
    ON_WM_TIMER()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPayment message handlers
void CPayment::ShowInformation()
{
    CStaff::ShowInformation();
}


BOOL CPayment::OnInitDialog()
{
    CStaff::OnInitDialog();

    // TODO: Add extra initialization here

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void CPayment::OnDestroy()
{
    CStaff::OnDestroy();

    // TODO: Add your message handler code here

}

void CPayment::OnTimer(UINT nIDEvent)
{
    // TODO: Add your message handler code here and/or call default

    CStaff::OnTimer(nIDEvent);
}

int tell_payment(UINT nChar,int &id,int &fld,DB *df,int sq)
{
    
	switch(fld) {
    case PY_PARTICULAR:
		if (isblank(df->dat[fld])){
			long Srec = viewidx(&particul);
			if (Srec>0){
				lset(df->dat[fld],particul.dat[2]);
			}
		}
		break;
    }
    return 2;
}

int payment_save(DB *df,int idi)
{
	long irec = qd(df->dat[PY_GLCODE],&glcode);
    if(irec==0) {
        waitmsg("Please assign 'GLCODE A/C'");
        return 0;
    }
    irec = qd(df->dat[PY_ACCODE],&accode);
    if(irec==0) {
        waitmsg("Please assign 'A/C Code'");
        return 0;
    }
    return 1;
}
	
void init_payment(DB *df)
{
    df->e->tell      = tell_payment;
    df->e->save      = payment_save;
    
}

void AdminDbPayment(DB *df)
{
    if(df->ID==0) {                           
        AfxMessageBox("Database not open");   
        return;                               
    }                                         
    CPayment p1(df->ID,df,df->f[1].idx & 0x01);
    p1.DoModal();                             
}

