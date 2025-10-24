#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <io.h>
#include <dos.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <ctype.h>
#include <math.h>
#include <fcntl.h>
#include <sys\types.h>
#include <sys\stat.h>
#include "module.h"

enum detail {DI_DATE=1,DI_TICKET,DI_LORRY,DI_ACCODE,DI_NAME,DI_PT,
             DI_DN,DI_GSNO,DI_TODAY,DI_NET};
void MonthRep(DB *df,DB *rep)
{
    if(tscr_flag(df)==0) return;
    fetch_rec(rep,df);
    qd(df->dat[CUST],&accode);
    lset(rep->dat[DI_NAME],accode.dat[2]);
    if(!strcmp(df->dat[DATE],tscr.dat[2])) {
        rset(rep->dat[DI_TODAY], df->dat[NET]);
    }
    memwrt(rep);
}

void solve_state(FILE *ptr,int line,FM *fm,char *text)
{
    replace(text,"%s",mdate(tscr.dat[2]));
    image(fm,ptr,text);
}   

void DetailListingReport(DB *df)
{
    KB   key;
    int  field[12],fo[] = { DI_TICKET,DI_DATE,DI_ACCODE,DI_LORRY,DI_PT,DI_DATE,0};
    char *ndetail[]={"Ticket No","Date","A/c Code","Vehicle No","Product","Date",""};

    dopen(&tscr,"tscr.scr");
    dopen(&report,"detail.rep");
    key.code = keytscr();
    if (key.ch[1] == F1) {
        if(tscr.dat[Da_STATUS][0]=='P') {
            strcpy(SubTitle,"Purchase Detail Report");
        }
        else {
            strcpy(SubTitle,"Sales Detail Report");
        }
        _PL[2] = UserSortMethod(3,fo,ndetail,&report,field);
        mloop(&tscr,df,&report,DATE,MonthRep);
        if(report.t->sumrecord>0) {
            sort(&report,field);
            SquReport(&report,field);
            FM fm;
            plan(&fm,&tscr,&report,solve_state);
        }
    }
    dclose(&report);
    dclose(&tscr);
}
