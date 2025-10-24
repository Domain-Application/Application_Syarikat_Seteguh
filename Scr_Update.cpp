// Scr_Update.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Scr_Update.h"
#include "ShortFigure.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScr_Update dialog
extern BOOL IsTicketAlreadyBilled(DB *df);
DB *main;

CScr_Update::CScr_Update(UINT IDD,DB *pdf,int Idx,void(*p)(),CWnd* pParent /*=NULL*/)
: CTio(IDD,pdf,Idx,p,pParent)
{
	//{{AFX_DATA_INIT(CScr_Update)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CScr_Update::DoDataExchange(CDataExchange* pDX)
{
	CTio::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScr_Update)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScr_Update, CTio)
	//{{AFX_MSG_MAP(CScr_Update)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScr_Update message handlers
void CScr_Update::OnSetWild()
{
    CTio::OnSetWild();
	accode.MoveFirst();
	lset(report.dat[IvS_ACCODE1],accode.dat[1]);
	accode.MoveLast();
	lset(report.dat[IvS_ACCODE2],accode.dat[1]);
	lset(report.dat[IvS_PRODUCT],"01");
	lset(report.dat[IvS_TERM],"P");
	dbasedate(report.dat[IvS_TODAY]);
}




void Make_Percent()
{
	long xrec = qd(report.dat[IvS_ACCODE1],&accode);
	if (xrec<=0){
		waitmsg("A/C Code From Not Found");
		return;
	}
	xrec = qd(report.dat[IvS_ACCODE2],&accode);
	if (xrec<=0){
		waitmsg("A/C Code To Not Found");
		return;
	}
	xrec = qd(report.dat[IvS_PRODUCT],&prodtype);
	if (xrec<=0){
		waitmsg("Product Not Found");
		return;
	}
	xrec = qd(report.dat[IvS_TERM],&term);
	if (xrec<=0){
		waitmsg("TERM Not Found");
		return;
	}
    //ShortFigureEnter();
	CShortFigure p1(IDD_OER_PERCENT);
	p1.m_value = 18;
	int flag = p1.DoModal();
	
	if(flag==IDCANCEL) {
		return;
	}
    double percent = p1.m_value;//atof(ShortFigure);
	
	CString cmd,filter,str;
    cmd.Format("Select Ticket From [%s] ",main->name);    
    filter.Format("where [date] >= '%s' AND ",DateStr(report.dat[IvS_DATE1]));	
	str.Format("[date] <= '%s' ",DateStr(report.dat[IvS_DATE2]));
	filter += str;
    if(!isblank(report.dat[IvS_ACCODE1]) && !isblank(report.dat[IvS_ACCODE2])) {
        str.Format("and [ACCODE] BETWEEN '%s' AND '%s' ",report.dat[IvS_ACCODE1],report.dat[IvS_ACCODE2]);
        filter += str;
    }
    if(!isblank(report.dat[IvS_PRODUCT])) {
        str.Format("AND [PT] = '%s' ",report.dat[IvS_PRODUCT] );
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
			if (main->val(RATE)>0) continue;
			crec = qd(main->dat[ACCODE],&accode);
			if(crec>0){
				if(!strcmp(accode.dat[CU_TERM],report.dat[IvS_TERM])){
					if(!wildcmp(accode.dat[CU_GROUP],report.dat[IvS_GROUP])){
						if (!IsTicketAlreadyBilled(main)){
							dtodb(percent,main,RATE);
							main->Update();
						}
					}
				}
			}
		}
	}
	dclose(&adoTmp);
	waitmsg("Update	Completed");
}

void SetPercent(DB *df)
{
	
    dopen(&report,"invoice.scr");
	main = df;
	CScr_Update t1(IDD_SCR_UPDATE,&report,0,Make_Percent);   
	t1.Status = 1; //Process Button
    t1.DoModal();
    dclose(&report);
}

void Make_Subsidy()
{
	long xrec = qd(report.dat[IvS_ACCODE1],&accode);
	if (xrec<=0){
		waitmsg("A/C Code From Not Found");
		return;
	}
	xrec = qd(report.dat[IvS_ACCODE2],&accode);
	if (xrec<=0){
		waitmsg("A/C Code To Not Found");
		return;
	}
	xrec = qd(report.dat[IvS_PRODUCT],&prodtype);
	if (xrec<=0){
		waitmsg("Product Not Found");
		return;
	}
	xrec = qd(report.dat[IvS_TERM],&term);
	if (xrec<=0){
		waitmsg("TERM Not Found");
		return;
	}
    //ShortFigureEnter();
	CShortFigure p1(IDD_SET_SUBSIDY);
	p1.m_value = 5;
	int flag = p1.DoModal();
	
	if(flag==IDCANCEL) {
		return;
	}
    double percent = p1.m_value;//atof(ShortFigure);
	
	CString cmd,filter,str;
    cmd.Format("Select Ticket From [%s] ",main->name);    
    filter.Format("where [date] >= '%s' AND ",DateStr(report.dat[IvS_DATE1]));	
	str.Format("[date] <= '%s' ",DateStr(report.dat[IvS_DATE2]));
	filter += str;
    if(!isblank(report.dat[IvS_ACCODE1]) && !isblank(report.dat[IvS_ACCODE2])) {
        str.Format("and [ACCODE] BETWEEN '%s' AND '%s' ",report.dat[IvS_ACCODE1],report.dat[IvS_ACCODE2]);
        filter += str;
    }
    if(!isblank(report.dat[IvS_PRODUCT])) {
        str.Format("AND [PT] = '%s' ",report.dat[IvS_PRODUCT] );
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
			if (main->val(SAMT)>0) continue;
			crec = qd(main->dat[ACCODE],&accode);
			if(crec>0){
				if(!strcmp(accode.dat[CU_TERM],report.dat[IvS_TERM])){
					if(!wildcmp(accode.dat[CU_GROUP],report.dat[IvS_GROUP])){
						if (!IsTicketAlreadyBilled(main)){
							dtodb(percent,main,SUBSIDY);
							double weight = atof(main->dat[NET])/1000;
							double amt = weight * main->val(SUBSIDY);
							dtodb(amt,main,SAMT);
							main->Update();
						}
					}
				}
			}
		}
	}
	dclose(&adoTmp);
	waitmsg("Update	Completed");
}

void SetSubsidy(DB *df)
{
	
    dopen(&report,"invoice.scr");
	main = df;
	CScr_Update t1(IDD_SCR_UPDATE,&report,0,Make_Subsidy);   
	t1.Status = 1; //Process Button
    t1.DoModal();
    dclose(&report);
}
