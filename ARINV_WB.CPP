// arinv_wb.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "arinv_wb.h"
#include "db_ado.h"
#include "openrs.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

DB invwb(IDD_ACCODE,6);
/////////////////////////////////////////////////////////////////////////////
// CArinvWb dialog

CArinvWb::CArinvWb(UINT IDD,DB *pdf, int Idx,CWnd* pParent /*=NULL*/)
	: CParent(IDD,pdf,Idx,pParent)
{
    //{{AFX_DATA_INIT(CArinvWb)
	//}}AFX_DATA_INIT
    ExtraButton1 = IDC_BUTTON1;
}

void CArinvWb::OnCreate_Einv_UI()
{
    pPage[1] = NULL;

    CRect rcDlgs;
    GetDlgItem(IDC_EINV_FUNCTIONS)->GetWindowRect(rcDlgs);
    pPage[1] = new CEINV_Functions;
    VERIFY(pPage[1]->Create(CEINV_Functions::IDD, this));
    pPage[1]->df = df;
    pPage[1]->sub = sub;
    pPage[1]->MoveWindow(rcDlgs);
    pPage[1]->ShowWindow(SW_SHOW);
    ScreenToClient(rcDlgs);
}

BOOL CArinvWb::OnInitDialog()
{
    OnCreate_Einv_UI();
    CParent::OnInitDialog();
	return TRUE;
}

void CArinvWb::DoDataExchange(CDataExchange* pDX)
{
	CParent::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CArinvWb)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CArinvWb, CParent)
    //{{AFX_MSG_MAP(CArinvWb)
	ON_BN_CLICKED(IDC_BUTTON1, OnCreateNewInvoice)
	ON_BN_CLICKED(IDC_Unlock, OnUnlock)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CArinvWb::IsCannotEdit()
{
	/*
    char invno[20];
    strcpy(invno,arinv.dat[1]);
    lset(invno,df->dat[1]);
	long xrec =0;
	if(!stricmp(df->name,"arinv_wb")) {
		xrec = arinv.Search(invno,1); //acc\ar\invoice
	}
	else
	{
		xrec = apinv.Search(invno,1); //acc\ap\invoice
	}
    
	if(xrec>0) {
		waitmsg("This invoice already post to ACC\nModification Not Allow");
		return TRUE;
    }
	*/
    return FALSE;
}

void CArinvWb::ShowInformation()
{   
    if (pPage[1])
    {
        pPage[1]->ShowInformation();
    }
    ReFreshSubFile();
    CParent::ShowInformation();    
}

void CArinvWb::ShowInitFrame()
{
    fg.Initial(&m_grid,sub,2);

	fg.SetTitle("No",500,1);
    fg.SetTitle("Ticket No",1500,1);
    fg.SetTitle("Date",1200,1);
    fg.SetTitle("Vehicle No",1500,1);    
    fg.SetTitle("DN No",1200,1);
    fg.SetTitle("Qty(kg)",1200,7);
    fg.SetTitle("Amount",1200,7);
    fg.SetTitle("Classification Code",1650,1);
}

void CArinvWb::ReplaceSubGrid(long rec,long irec,CMSFlexGrid *m_grid,DB *sub,int IdxItem)
{
	fg.show(rec);
	long xrec = 0;	
    if(!stricmp(sub->name,"apoinv_wb")) {
		qq(sub->dat[VIWB_DO],&receive);
        fg.SetText(sub,0,VIWB_DO,-1);
        fg.SetText(&receive,Date,LORRY,DN,-1);
        fg.SetText(sub,VIWB_QTY);
        fg.SetText(sub,VIWB_AMOUNT);
        fg.SetText(sub,GetDbFieldNo(sub,"CLASSIFICATION"));
    }
	else{
        qq(sub->dat[VIWB_DO],&despatch);
        fg.SetText(sub,0,VIWB_DO,-1);
        fg.SetText(&despatch,Date,LORRY,DN,-1);
        fg.SetText(sub,VIWB_QTY);
        fg.SetText(sub,VIWB_AMOUNT);
        fg.SetText(sub,GetDbFieldNo(sub,"CLASSIFICATION"));
    }
}

void CArinvWb::SetAttribute(int flag,int fld)
{
    if(flag) {
		if(atoi(user.dat[UR_LEVEL])>=99){
	        GetDlgItem(IDC_Unlock)->ShowWindow(SW_SHOW);
		}
		else{
			GetDlgItem(IDC_Unlock)->ShowWindow(SW_HIDE);
		}
    }
    else {
        GetDlgItem(IDC_Unlock)->ShowWindow(SW_HIDE);
    }
    CParent::SetAttribute(flag,fld);
}

void SalesInvoiceWb()
{
    Purchase_Flag = FALSE;
	
    CArinvWb p1(IDD_ARINV_WB,&arinv_wb,1);
    p1.sub       = &aroinv_wb;
    p1.IDD_SUB   = IDD_AROINV_WB;
	//    p1.Status = 0x00ff;	//Not Allow to delete
	p1.m_WinPrint = TRUE;
    p1.DoModal();
	
}

void PurchaseInvoiceWb()
{
    Purchase_Flag = TRUE;
	
    CArinvWb p1(IDD_APINV_WB,&apinv_wb,1);
    p1.sub       = &apoinv_wb;
    p1.IDD_SUB   = IDD_APOINV_WB;
	//    p1.Status = 0x00ff; //Not Allow to delete
	p1.m_WinPrint = TRUE;
    p1.DoModal();
	//	SetAttribute(TRUE,TRUE);
}

void SetTransactionType(DB *df, DB *accode)
{
	if (!strcmp(df->name, "ARINV_WB")) 
 	{
		lset(df->dat[GetDbFieldNo(df, "einv_tt")], "IV");
	} 
	else if(!strcmp(df->name, "APINV_WB") && accode->Boolean(GetDbFieldNo(accode, "SELFBILL")))
    {
        lset(df->dat[GetDbFieldNo(df, "einv_tt")], "IV");
    } 
    else
	{
		lset(df->dat[GetDbFieldNo(df, "einv_tt")], "S");
	}
}

int tell_aroinv_wb(UINT nChar,int &id,int &fld,DB *df,int sq)
{
    switch(fld) {
    case VIWB_DO:
        id = DO;
        break;
	default:
		break;
    }

    return 2;
}

int start_arinv_wb(DB *df)
{
    df->FldDefault(2);
	lset(df->dat[IVWB_STATUS],"U");
    return 1;
}

int arinv_wb_initial(DB *df,int *fld)                //initial setting
{
    if(atoi(user.dat[UR_LEVEL])<9) {
        fld[0] = IVWB_DATE;
        fld[2] = 0;
    }
    return 1;
}

int tell_arinv_wb(UINT nChar,int &id,int &fld,DB *df,int sq)
{
	switch(fld){
	case IVWB_ACCODE:
		long Srec,irec;
		Srec=irec=0;
		Srec = interval(&irec,df->dat[1],df->e->chain,1);
		if(Srec){
            CString strMsg;
            strMsg.Format("Invoice Detail Existed.Not Allow to Edit this Field");
            waitmsg(strMsg);
            id = DO;
		}
		if(sq){
			qd(df->dat[fld],&accode);
			lset(df->dat[IVWB_TERMS],accode.dat[CU_TERMS]);			
		}
        SetTransactionType(df, &accode);
		break;
	}
	return 2;
}

void init_arinv_wb(DB *df)
{

    df->e->start   = start_arinv_wb;
    df->e->initial = arinv_wb_initial;
	df->e->tell	   = tell_arinv_wb;
}

void init_aroinv_wb(DB *df)
{
    df->e->tell = tell_aroinv_wb;
}


void CArinvWb::OnCreateNewInvoice() 
{
    CString strTable = (Purchase_Flag) ? "dsinv" : "dpinv";

	strSql.Format("select * from [%s] where [accode]='%s' and [date] <= %s and [DO] IS NULL",
	strTable,df->dat[IVWB_ACCODE], DbText(df,IVWB_DATE,df->dat[IVWB_DATE]));


    for(;;) {
        ado(&invwb,strTable,NULL,strSql);
        long irec = viewidx(&invwb);
        if(irec==0){
            dclose(&invwb);
            break;
        }
        if(SaveMainInformation()==FALSE){
            dclose(&invwb);
            break;
        }
        lset(sub->dat[1], df->dat[1]);
        sub->Find_SequenceNo();
        lset(sub->dat[VIWB_DO], invwb.dat[1]);
        rset(sub->dat[VIWB_QTY], invwb.dat[5]);
        rset(sub->dat[VIWB_AMOUNT], invwb.dat[6]);
        irec = qd(invwb.dat[GetDbFieldNo(&invwb,"PT")], &prodtype);
        int fld = 0;
        if(!strcmp(strTable, "dsinv"))
        {
            fld = GetDbFieldNo(&prodtype,"CLASSIFICATION");
        }
        else
        {
            fld = GetDbFieldNo(&prodtype,"CLASSIFICATION1");
        }
        if(irec > 0)
        {
            lset(sub->dat[GetDbFieldNo(sub,"CLASSIFICATION")], prodtype.dat[fld]);
        }
        sub->AddNew();
        ReFreshSubFile();
        ShowInformation();
        dclose(&invwb);
    }
}

void CArinvWb::OnUnlock()
{	

	ShowInformation();
	CDialog::OnCancel();
}

BEGIN_EVENTSINK_MAP(CArinvWb, CParent)
    //{{AFX_EVENTSINK_MAP(CArinvWb)
	ON_EVENT(CArinvWb, IDC_MSFLEXGRID1, -601 /* DblClick */, OnDblClickMsflexgrid1, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

//extern void TicketData(DB *df);
void CArinvWb::OnDblClickMsflexgrid1() 
{
	OnDblClickMsflexgrid(&m_grid,sub);
	/*
	if(GetKeyState(VK_CONTROL)<0){
		OnDblClickMsflexgrid(&m_grid,sub);
	}
	else{
		rec = m_grid.GetRow() - 1;
		ridx(rec+irec,sub,IdxItem);
		if(Purchase_Flag && receive.Search(sub->dat[VIWB_DO],TICKET,TRUE)){
			app->UpdateTableLastTransaction(&receive,FALSE);
			app->UpdateBrowseLastTransaction(&receive,FALSE);
			TicketData(&receive);
		}
		else{
			despatch.Search(sub->dat[VIWB_DO],TICKET,TRUE);
			app->UpdateTableLastTransaction(&despatch,FALSE);
			app->UpdateBrowseLastTransaction(&despatch,FALSE);
			TicketData(&despatch);
		}
	}
	*/
}
