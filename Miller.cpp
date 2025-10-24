// Miller.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Miller.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMiller dialog
extern BOOL IsWholeWildString(char *str);

CMiller::CMiller(UINT IDD,DB *pdf,int Idx,CWnd* pParent /*=NULL*/)
: CStaff(IDD,pdf,Idx,pParent)
{
	//{{AFX_DATA_INIT(CMiller)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMiller::DoDataExchange(CDataExchange* pDX)
{
	CStaff::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMiller)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMiller, CStaff)
	//{{AFX_MSG_MAP(CMiller)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMiller message handlers
BOOL IsTicketAlreadyBilled(DB *df)
{
	
	long xrec = 0;
	if(!stricmp(df->name,"despatch")) {
		xrec = aroinv_wb.Search(df->dat[1],VIWB_DO,TRUE);		
	}
	else{
		xrec = apoinv_wb.Search(df->dat[1],VIWB_DO,TRUE);		
	}
	if(xrec){		
		return TRUE;
	}
	return FALSE;
	
}

void SetPrice(DB *df, DB *main)
{
	if (IsTicketAlreadyBilled(main)) return; // 28/05/2008 OOI
    lset(main->dat[UPDATE],df->dat[1]);              //Save Update No
    double weight = main->val(NET) / 1000;
    double price  = 0;
	
	
    if(df->dat[ML_TERM][0]=='T'){
        price = df->val(ML_PRICE);
    }
	else if(df->dat[ML_TERM][0]=='P'){
        double rate   = main->val(RATE);
        price  = df->val(ML_PRICE) * rate;
    }
	
	if(strcmp(main->dat[PT],"01   ")>=0 && strcmp(main->dat[PT],"03   ")<=0){
	}
	else{
		price = df->val(ML_PRICE);
	}
	
	double tfee=main->val(TFEE);
	double hfee=main->val(HFEE);
	/* All based on ticket 3/11/2010 chee
	if (isblank(main->dat[HFEE]) && isblank(main->dat[TFEE])){
		tfee = accode.val(CU_TFEE);
		dtodb(tfee,main,TFEE);
		hfee = accode.val(CU_HFEE);
		dtodb(hfee,main,HFEE);
	}
	*/
    dtodb(price,main,PRICE);
    price = main->val(PRICE);
    price -= (tfee+hfee);
	if(price <=0) price = 0;
    double amount = weight * price;
    dtodb(amount,main,AMT);
    main->Update();
}

void upp_pm(DB *df,DB *main)
{
	
    CString cmd,filter,str;
    cmd.Format("Select Ticket From [%s] ",main->name);    
    filter.Format("where [date] >= '%s' AND ",DateStr(df->dat[ML_DATE1]));	
	str.Format("[date] <= '%s' ",DateStr(df->dat[ML_DATE2]));
	filter += str;
    if(!isblank(df->dat[ML_ACCODE])) {
        str.Format("and [ACCODE] = '%s' ",df->dat[ML_ACCODE] );
        filter += str;
    }
    if(!isblank(df->dat[ML_PRODUCT])) {
        str.Format("AND [PT] = '%s' ",df->dat[ML_PRODUCT] );
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
				if(!strcmp(accode.dat[CU_TERM],df->dat[ML_TERM])){
					if(!wildcmp(accode.dat[CU_GROUP],df->dat[ML_GROUP])){
						SetPrice(df,main);
					}
				}
			}
		}
	}
	dclose(&adoTmp);
	
}

void CMiller::after_save(int ExistFlag)
{
    if ( choice_menu("Posting Data",FAIL) == OK ){
        upp_pm(df,main);
        AfxMessageBox("Update Completed");
    }
}

int miller_save(DB *df,int flagi)
{
    if(atof(df->dat[ML_PRICE])<=0){
        waitmsg("Unit Price <= 0");
        return 0;
    }
    long rec = qd(df->dat[ML_PRODUCT],&prodtype);
    if(rec==0){
        waitmsg("Please key in product");
        return 0;
    }
	if (isblank(df->dat[ML_TERM])){
		waitmsg("Please specify term");
		return 0;
	}
    return 1;
}

int start_miller(DB *df)
{
	
    dbasedate(df->dat[ML_DATE1]);
    lset(df->dat[ML_DATE2],df->dat[ML_DATE1]);
    lastday(df->dat[ML_DATE2]);	
	lset(df->dat[ML_PRODUCT],"01");
    return 1;
}

int tell_miller(UINT nChar,int &id,int &fld,DB *df,int sq)
{
	switch(fld) {
	case ML_DATE2:
        lset(df->dat[fld],df->dat[ML_DATE1]);
        lastday(df->dat[fld]);
        break;
	}
    return 2;
}

int UpdateUnitPrice(DB *df,DB *main)
{
    CMiller p1(df->ID,df,df->f[1].idx & 0x01);
	p1.main = main;
    p1.m_WinPrint = TRUE;
    p1.DoModal();
    return 1;
}

void init_miller(DB *df)
{
    df->e->tell      = tell_miller;
    df->e->save      = miller_save;
    df->e->start     = start_miller;
    //df->e->AdminDb   = MILLER_AdminDb;
}
