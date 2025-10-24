// Detail.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "month.h"
#include "multisort.h"
#include "PrintJob.h"

enum detgtn {DI_DATE=1,DI_TICKET,DI_DN,DI_ACCODE,DI_NAME,DI_PT,DI_TANK,DI_LORRY,
             DI_CTR,DI_TIMEIN,DI_TIMEOUT,DI_PNET,DI_SNET,DI_WASTAGE,DI_VAR};
void save_detgtn(DB *df,DB *rep)
{
    if(!tscr_flag(df)) return;
    fetch_rec(rep,df);
    compute_weight(df);
    qq(df->dat[CUST],&accode);
    lset(rep->dat[DI_NAME],accode.dat[3]);

    if(m[2]>m[5]) {
        dtodb(double(m[5])/1000,rep,DI_SNET);
        dtodb(double(m[2])/1000,rep,DI_PNET);
    }
    else {
        dtodb(double(m[2])/1000,rep,DI_SNET);
        dtodb(double(m[5])/1000,rep,DI_PNET);
    }
    dtodb(double(m[7])/1000,rep,DI_WASTAGE);
    dtodb(double(m[11])/1000,rep,DI_VAR);
    memwrt(rep);
}

void get_detgtn()
{
    mloop(&tscr,ttt,&report,Date,save_detgtn);
}

void sort_detgtn(int *field,DB *scr,DB *rep)
{
    int  fo[] = { DI_TICKET,DI_DATE,DI_NAME,DI_LORRY,DI_PT,0};
    char *na[]={"Ticket No","Date","A/c Code","Vehicle No","Product",""};

    multisort s2(3,fo,na,rep,field);
    s2.DoModal();
}

void DetailGrossTareNetListingReport(DB *df)
{
    ttt = df;

    ropen(&tscr,"tscr.scr");
    ropen(&report,"detgtn.dbf",TRUE);

    CMonth t2(IDD_TSCR,&tscr);
    t2.CollectData = get_detgtn;
    t2.SortData    = sort_detgtn;
    t2.fldPrinter = Pro_RPRINTER;
    t2.DoModal();

    dclose(&report);
    dclose(&tscr);
}
