// AutoInvoice.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "db_ado.h"
#include "AutoInvoice.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAutoInvoice dialog


CAutoInvoice::CAutoInvoice(CWnd* pParent /*=NULL*/)
	: CDialog(CAutoInvoice::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAutoInvoice)
	m_d1 = COleDateTime::GetCurrentTime();
	m_d2 = COleDateTime::GetCurrentTime();
	m_nextInvoiceNo = _T("");
	m_invdate = COleDateTime::GetCurrentTime();
	//}}AFX_DATA_INIT
}


void CAutoInvoice::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAutoInvoice)
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_d1);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER2, m_d2);
	DDX_Text(pDX, IDC_EDIT1, m_nextInvoiceNo);
	DDX_Control(pDX, IDC_MSFLEXGRID1, m_grid);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER3, m_invdate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAutoInvoice, CDialog)
	//{{AFX_MSG_MAP(CAutoInvoice)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoInvoice message handlers

void CAutoInvoice::OnOK() 
{
    GetDlgItem(IDOK)->ShowWindow(SW_HIDE);

	UpdateData();
	
	lset(two.dat[1], m_d1);
	lset(two.dat[2], m_d2);
	d1 = DbText(&two, 1);
	d2 = DbText(&two, 2);

	DoAutoGenerateInvoice();


//	CDialog::OnOK();
}

void CAutoInvoice::DoPoInvoice()
{
    long Srec = accode.Search(report.dat[1], 1, TRUE);
    if(Srec==0) return;
	
    //FindLastDate();
	
    lsetblank(invmain);
    FindAddInfoTicketNo(invmain);
    lset(invmain->dat[IVWB_DATE], m_invdate);  
    lset(invmain->dat[IVWB_ACCODE], accode.dat[1]);
    lset(invmain->dat[IVWB_TERMS], accode.dat[CU_TERMS]);        
	//if (Purchase_Flag){
	lset(invmain->dat[IVWB_REMARKS],"F.F.B TOTAL");        
	//}
	CString strTable = (Purchase_Flag) ? "dsinv" : "dpinv";
	CString str;
	str.Format("Select [ticket],[qty],[amount] from [%s] where accode='%s' and [date] between %s and %s and [DO] IS NULL and [amount] > 0 order by [ticket]",
		strTable,accode.dat[1],   d1, d2);
	DB adoTmp;
	ado(&adoTmp, "", NULL, str);
	long xrec = adoTmp.t->sumrecord;	
	if (xrec>0){
		invmain->AddNew();
	}
    for(long rec=1; rec<=xrec; rec++) {
        adoTmp.read(rec);
		lsetblank(invsub);
		lset(invsub->dat[1],invmain->dat[1]);
		lset(invsub->dat[VIWB_DO],adoTmp.dat[1]);
		rset(invsub->dat[VIWB_QTY],adoTmp.dat[2]);
		rset(invsub->dat[VIWB_AMOUNT],adoTmp.dat[3]);
		invsub->AddNew();
    }
	dclose(&adoTmp);
    SetActiveStatus();
}

void CAutoInvoice::DoAutoGenerateInvoice()
{
	CString strTable = (Purchase_Flag) ? "dsinv" : "dpinv";
	CString str;
    str.Format("Select [accode] from [%s] where [date] between %s and %s and [DO] IS NULL group by [accode]",
     strTable,   d1, d2);

	ado(&report, "", NULL, str);
	long xrec = report.t->sumrecord;
	m_progress.SetRange32(1, xrec);
    for(long rec=1; rec<=xrec; rec++) {
        report.read(rec);
		DoPoInvoice();
    }
	dclose(&report);

}

BOOL CAutoInvoice::IsInvoiceExist()
{
	return FALSE;
}


void DoAutoInvoice(DB *df,DB *invmain,DB *invsub)
{
	CAutoInvoice h1;
	h1.df = df;
	h1.invmain = invmain;
	h1.invsub = invsub;
	h1.DoModal();

}

void CAutoInvoice::FindLastDate()
{
	/*
    strcpy(szDate1, two.dat[1]);

    long xrec = po.Search(report.dat[2], 1, TRUE);

    if(xrec>0) {
        str.Format("Select max([period2]) from invoice where (po = '%s' OR PO='*') and accode = '%s' and ([period1] between %s and %s or [period2] between %s and %s)",
            po.dat[1], accode.dat[1], d1, d2 ,d1, d2);
    }
    else {
        str.Format("Select max([period2]) from invoice where accode = '%s' and ([period1] between %s and %s or [period2] between %s and %s)",
            accode.dat[1], d1, d2 ,d1, d2);
    }

    CString text = SqlText(str,1);
	if(text.GetLength()>0) {
		lset(szDate1, text);
		plusday(szDate1, 1);
	}

    int k = 0;
	*/
}


BOOL CAutoInvoice::OnInitDialog()
{

    CDialog::OnInitDialog();

    invmain->t->set |= 0x01;
    FindAddInfoTicketNo(invmain);
    m_nextInvoiceNo = invmain->dat[1];
    UpdateData(FALSE);
    SetGridTitle();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAutoInvoice::SetGridTitle()
{
    MGRID g(&m_grid,invmain,0,1);
    g.ratio = float(1.20);
    g.SetTitle(invmain,0,-1);
    g.SetTitle("Invoice",1000,1);
    g.SetTitle("AC Code",800,1);
    g.SetTitle("Date",800,4);
    g.SetTitle("Amount (RM)",1000,7);
    m_grid.SetRows(1);
}

void CAutoInvoice::SetActiveStatus()
{
    long rec = m_grid.GetRows();

    m_grid.SetRows(rec+1);
    m_grid.SetRow(rec);
    MGRID g(&m_grid,invmain,rec);
    g.SetText(invmain,0,1,IVWB_ACCODE,IVWB_DATE,-1);
	CString str;
	str.Format("Select sum(amount) from [%s] where invoice = '%s'",invsub->name,invmain->dat[1]);
	double amt = SqlValue(str);
	char txtAmt[15];
	dtoa(amt,txtAmt,13,2);
	g.SetText(putnum(txtAmt));
}
