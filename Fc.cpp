// fc.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "FChange.h"
#include "multisort.h"
#include <io.h>

DB   tvo;
DB   FChange;
enum FchangeRep {FchR_DATE=1,FchR_TIME,FchR_USER,FchR_KEY,FchR_FIELDNAME,
FchR_NEW_DATA,FchR_OLD_DATA,FchR_FIELDNO};


void save_FChange(DB *df,DB *rep)
{
    char tmp[80];
	
    strcpy(tmp,tvo.dat[Fch_KEY]);
    lset(tmp,df->dat[Fch_KEY]);
    if ( !wildcmp(tmp,tvo.dat[Fch_KEY])) {
        if ( !wildcmp(df->dat[Fch_USER],tvo.dat[Fch_USER])) {
            fetch_rec(rep,df);
            memwrt(rep);
        }
    }
}

void SetFileChangeFieldName(DB *rep,DB *df)
{
    long rec,last = rep->t->sumrecord;
    int  i;

    for(rec=1; rec<=last; rec++) {
        readrec(rec,rep);
        i  = atoi(rep->dat[FchR_FIELDNO]);
        if(i>0) {
            lset(rep->dat[FchR_FIELDNAME],df->f[i].name);
        }
        fastwrt(rec,rep);
    }
}


void put_fc()
{
    char tmp[40];

    if(!IsFcExist(ttt,tmp,"fc")) return;
    dopen(&FChange,tmp);
    restore(&user);
    mloop(&tvo,&FChange,&report,1,save_FChange);
    if(report.t->sumrecord>0){
        SetFileChangeFieldName(&report,ttt);
    }
    else {
        lsetblank(&report);
        writerec(1,&report);
    }
    dclose(&FChange);
}

void sort_fc(int *field,DB *scr,DB *rep)
{
	int  fo[] = { FchR_DATE,FchR_TIME,FchR_USER,FchR_KEY,FchR_FIELDNAME,0};
    char *nmrws[]={"Date","Time","User No","Ref. No","Field Name",""};
	
    multisort s2(3,fo,nmrws,&report,field);
    s2.DoModal();

}

void do_fc(int *field,DB *df,DB *rep,int fldPrinter)
{
//    PrintJobType = 1;
    sort(&report,field);
    CrystalReport(&tvo,&report,field,Pro_RPRINTER);
//    stdplan(&tvo,&report);
}

void FileChangeReport(DB *df)
{

    dopen(&tvo,"wbfc.scr",TRUE);
    dopen(&report,"fchange.dbf",TRUE);

    ttt = df;
    backup(&user);
    CFChange t1(IDD_FCHANGE,&tvo);
    t1.CollectData = put_fc;
    t1.SortData    = sort_fc;
//    t1.PrintReport = do_fc;
    t1.DoModal();

    dclose(&report);
    dclose(&tvo);
}

void put_fc_site()
{
    char tmp[40];
	    
	sprintf(tmp,"%ssite.fc",FileDir);
    if(IsFileExisted(tmp)==FALSE) return;
    sprintf(tmp,"site.fc");
	
    dopen(&FChange,tmp);
    restore(&user);
    mloop(&tvo,&FChange,&report,1,save_FChange);
    if(report.t->sumrecord>0){
        SetFileChangeFieldName(&report,ttt);
    }
    else {
        lsetblank(&report);
        writerec(1,&report);
    }
    dclose(&FChange);
}

void SiteFileChangeReport(DB *df)
{
	
    dopen(&tvo,"wbfc.scr",TRUE);
    dopen(&report,"fchange.dbf",TRUE);
	
    ttt = df;
    backup(&user);
    CFChange t1(IDD_FCHANGE,&tvo);
    t1.CollectData = put_fc_site;
    t1.SortData    = sort_fc;
	//    t1.PrintReport = do_fc;
    t1.DoModal();
	
    dclose(&report);
    dclose(&tvo);
}
