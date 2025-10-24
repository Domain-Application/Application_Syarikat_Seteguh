// chkvar.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Daily.h"
#include "multisort.h"
#include <io.h>

enum chkvar {DI_DATE=1,DI_TICKET,DI_ACCODE,DI_NAME,DI_LORRY,
             DI_FIRST,DI_SECOND,DI_W1,DI_W2,DI_Z1,DI_Z2,DI_D1,DI_D2};

void save_chkvar(DB *df,DB *report)
{
    long wt;

    fetch_rec(report,df);
    qd(report->dat[DI_ACCODE],&accode);
    lset(report->dat[DI_NAME],accode.dat[2]);
    if(atol(report->dat[DI_W2]) - atol(report->dat[DI_W1])) {
        wt = labs(atol(report->dat[DI_W2]) - atol(report->dat[DI_W1])) -
             labs(atol(report->dat[DI_FIRST]) - atol(report->dat[DI_SECOND]));
        ltodb(wt,report,DI_D1);
    }
    if (labs(atol(report->dat[DI_Z2]) - atol(report->dat[DI_Z1]))) {
        wt = labs(atol(report->dat[DI_Z2]) - atol(report->dat[DI_Z1])) -
             labs(atol(report->dat[DI_FIRST]) - atol(report->dat[DI_SECOND]));
        ltodb(wt,report,DI_D2);
    }
    memwrt(report);
}

DB *ptrvar;

void put_var()
{
    int  field[12],fo[] = { DI_TICKET,DI_ACCODE,DI_LORRY,0};
    char *nddws[] = {"Ticket No","A/C Code","Vehicle No",""};

        if(mloop(&daily,ptrvar,&report,DP_DATE,save_chkvar)) {
            multisort s2(3,fo,nddws,&report,field);
            s2.DoModal();
            sort(&report,field);
            stdplan(&daily,&report);
        }
}
void VarianceDetailReport(DB *df)
{
    ropen(&daily,"daily.scr");
    ropen(&report,"chkvar.rep");
    ptrvar = df;
    CDaily t1(IDD_DAILY,&daily,0,put_var);
    t1.DoModal();
    dclose(&report);
    dclose(&daily);
}
