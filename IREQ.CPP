// Ireq.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "db_ado.h"
#include "Ireq.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIreq dialog


CIreq::CIreq(UINT IDD,DB *pdf,int Idx,CWnd* pParent /*=NULL*/)
    : CParent(IDD,pdf,Idx,pParent)
{
    //{{AFX_DATA_INIT(CIreq)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}


void CIreq::DoDataExchange(CDataExchange* pDX)
{
    CParent::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CIreq)
        // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIreq, CParent)
    //{{AFX_MSG_MAP(CIreq)
    ON_WM_DESTROY()
    ON_WM_TIMER()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIreq message handlers
void CIreq::ShowInformation()
{
    CParent::ShowInformation();
}


BOOL CIreq::OnInitDialog()
{
    CParent::OnInitDialog();

    // TODO: Add extra initialization here

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void CIreq::OnDestroy()
{
    CParent::OnDestroy();

    // TODO: Add your message handler code here

}

void CIreq::OnTimer(UINT nIDEvent)
{
    // TODO: Add your message handler code here and/or call default

    CParent::OnTimer(nIDEvent);
}

void CIreq::ShowDetail()
{
    CParent::ShowDetail();
}

void CIreq::OnDetail()
{
    CParent::OnDetail();

	CString s;
	s.Format("select sum(AMOUNT) from [%s] where refno = %s", 
		sub->name, DbText(sub,1));
	double total= SqlValue(s);
	dtodb(total, df, IQ_BAL);
	ShowInformation();
}

void CIreq::ShowInitFrame(CMSFlexGrid *grid,DB *sub)
{
    fg.Initial(grid,sub,2,grid_ratio);
    fg.SetTitle("No",400);
    fg.SetTitle("D/N No",1000);
    fg.SetTitle("Vehicle No",1200);
    fg.SetTitle("Item Code",1000);
    fg.SetTitle("Description",4800);
    fg.SetTitle("Quantity",1000,7);
    fg.SetTitle("Unit",800);
    fg.SetTitle("U/Price",1000,7);
    fg.SetTitle("Amount",1000,7);
    fg.SetTitle("Driver",1500);
    fg.SetTitle("Comm.",1000,7);
}

void CIreq::ReplaceSubGrid(long rec,long irec,CMSFlexGrid *m_grid,DB *sub,int IdxItem)
{
    fg.show(rec);
    fg.SetText(sub,NULL,PI_DN,PI_LORRY,PI_PT,PI_NAME,PI_QTY,
         PI_UNIT,PI_PRICE,PI_AMOUNT,PI_DRIVER,PI_NETCOMM,-1);
}


void AdminDbIreq(DB *df,DB *sub)
{
    if(df->ID==0) {
        AfxMessageBox("Database not open");
        return;
    }
    CIreq p1(df->ID,df,df->f[1].idx & 0x01);
    p1.sub = sub;
    p1.DoModal();
}



int tell_reqitem(UINT nChar,int &flag,int &fld,DB *df,int sq)
{
	long xrec;
	
	switch(fld) {
    case PI_ITEM:
        if(sq) {
            lset(df->dat[PI_NAME],prodtype.dat[2]);
            lset(df->dat[PI_UNIT],prodtype.dat[3]);
            if(df->name[0]=='r' || df->name[0]=='R') {
                rset(df->dat[PI_PRICE],prodtype.dat[PT_SPRICE]);
            }
        }
        break;
    case PI_AMOUNT:
    case PI_NETCOMM:
        flag = DO;
    case PI_COMM:
        xrec = qd(df->dat[PI_DRIVER], &driver);
        if(xrec==0) flag = DO;
        break;
    }
    double total = df->val(PI_QTY) * df->val(PI_PRICE);
    dtodb(total,df,PI_AMOUNT);
    total = df->val(PI_QTY) * df->val(PI_COMM);
    dtodb(total,df,PI_NETCOMM);
    return 2;
}

void init_reqitem(DB *df)
{
    df->e->tell    = tell_reqitem;
}

int ireq_save(DB *df,int x)
{
    BOOL flag = dBfldLogic(df,IQ_CASHBILL);
    if(flag) {
    }
    return 1;
}

int start_ireq(DB *df)
{
    dbasedate(df->dat[IQ_DATE]);
    lset(df->dat[IQ_ISSUED],"S");
    return 1;
}

int ireq_initial(DB *df,int *fld)                //initial setting
{
    fld[0] = IQ_BAL;
    fld[2] = 0;
	return 1;
}

void init_ireq(DB *df)
{
    df->e->start = start_ireq;
    df->e->initial = ireq_initial;
    df->e->save = ireq_save;
}

int start_ientry(DB *df)
{
    dbasedate(df->dat[IQ_DATE]);
    lset(df->dat[IQ_ISSUED],"P");
    return 1;
}

void init_ientry(DB *df)
{
    df->e->start = start_ientry;
    df->e->initial = ireq_initial;
    df->e->save = ireq_save;
}
