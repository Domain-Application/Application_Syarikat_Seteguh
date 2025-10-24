// BatchScr.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "BatchScr.h"
#include "Wb.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern DB *main;
/////////////////////////////////////////////////////////////////////////////
// CBatchScr dialog


CBatchScr::CBatchScr(UINT IDD,DB *pdf,int Idx,void(*p)(),CWnd* pParent /*=NULL*/)
: CTio(IDD,pdf,Idx,p,pParent)
{
	//{{AFX_DATA_INIT(CBatchScr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CBatchScr::DoDataExchange(CDataExchange* pDX)
{
	CTio::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBatchScr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBatchScr, CTio)
	//{{AFX_MSG_MAP(CBatchScr)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBatchScr message handlers
enum batch { S_D1=1,S_D2,S_ACCODE,S_PORLA_AC,S_PT,S_TERM};

void AdminOneTicketDb(DB *df)
{
    if(df->ID==0) {
        AfxMessageBox("Database not open");
        return;
    }
    CWb p1(df->ID,df,df->f[1].idx & 0x01);
	p1.OneRecordOnly = TRUE;
	p1.LptNo       = 1;
    p1.DoModal();
}

void CBatchScr::OnSetWild()
{
    CTio::OnSetWild();	
	lset(report.dat[S_PT],"01");
	
}

BOOL isInvoiceExisted(DB *df)
{
	char txt[100];
	long xrec = 0;
	if(!stricmp(df->name,"despatch")) {
		xrec = aroinv_wb.Search(df->dat[1],VIWB_DO,TRUE);
		strcpy(txt,aroinv_wb.dat[1]);
	}
	else{
		xrec = apoinv_wb.Search(df->dat[1],VIWB_DO,TRUE);
		strcpy(txt,apoinv_wb.dat[1]);
	}
	if(xrec){
		CString str;
		str.Format("Ticket %s is issued to Invoice:%s\nNot allow modify.",df->dat[1],trim(txt));
		AfxMessageBox(str);
		return TRUE;
	}
	return FALSE;
}

void Batch_modify()
{		
	CString cmd,filter,str;
    cmd.Format("Select Ticket From [%s] ",main->name);    
    filter.Format("where [date] >= '%s' AND ",DateStr(report.dat[IvS_DATE1]));	
	str.Format("[date] <= '%s' ",DateStr(report.dat[IvS_DATE2]));
	filter += str;
	long Xrec = qd(report.dat[S_ACCODE],&accode);
	if (Xrec>0){
		str.Format("and [ACCODE] = '%s' ",report.dat[S_ACCODE]);
        filter += str;
	}
	Xrec = qd(report.dat[S_PORLA_AC],&accode);
	if (Xrec>0){
		str.Format("and [ACCODE9] = '%s' ",report.dat[S_PORLA_AC]);
        filter += str;
	}
    Xrec = qd(report.dat[S_PT],&prodtype);
	if (Xrec>0){
		str.Format("AND [PT] = '%s' ",report.dat[S_PT] );
        filter += str;
	}
    cmd += filter;
    DB adoTmp;
	long crec=0;
	ado(&adoTmp,"adoTmp",NULL,cmd);	
	long Srec = read_sumindex(0,&adoTmp);
	if(Srec>0) {
		for(long rec=1; rec<=Srec; rec++){
			readrec(rec,&adoTmp);			
			main->Search(adoTmp.dat[1],1,TRUE);				
			crec = qd(main->dat[ACCODE],&accode);
			if(crec>0){
				if(!wildcmp(accode.dat[CU_TERM],report.dat[S_TERM])){
					if (!isInvoiceExisted(main)){
						AdminOneTicketDb(main);
					}
				}
			}
		}
	}
	dclose(&adoTmp);
	waitmsg("Update	Completed");
}

void Batch_Transaction(DB *df)
{
	dopen(&report,"batch.scr");
	main = df;
	CBatchScr t1(IDD_SCR_BATCH,&report,0,Batch_modify);   
	t1.Status = 1; //Process Button
    t1.DoModal();
    dclose(&report);
}