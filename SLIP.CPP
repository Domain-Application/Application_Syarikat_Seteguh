// slip1.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "multisort.h"
#include "AskPassword.h"
#include "PrintProgress.h"

/*

enum slip {Sl_CAPTION=1,Sl_COADD1,Sl_COADD2,Sl_COADD3,Sl_CONO,
     Sl_TICKET,Sl_LORRY,Sl_TRAILER,Sl_DATE,
     Sl_ACCODE,Sl_ANAME,Sl_ADD1,Sl_ADD2,Sl_ADD3,Sl_PORLA_NO,Sl_EDATE,
     Sl_TRANSP,Sl_TNAME,
     Sl_DRIVER1,Sl_D1NAME,Sl_D1IC,
     Sl_DRIVER2,Sl_D2NAME,Sl_D2IC,
     Sl_FIRST,Sl_SECOND,Sl_WASTAGE,Sl_NET,Sl_SW,Sl_VAR,Sl_STATUS,
     Sl_PT,Sl_PTNAME,Sl_AVG_B_WT,
     Sl_DN,Sl_DN2,Sl_PL3,Sl_GSNO,Sl_CTR,Sl_TIMEIN,Sl_TIMEOUT,
     Sl_UNRIPE,Sl_LONGTALK,Sl_UNDERRIPE,Sl_OLDBUNCHES,Sl_RIPE,
     Sl_WETBUNCHES,Sl_OVERRIPE,Sl_DIRTSTONE,Sl_ROTTEN,Sl_LOOSEFRUIT,Sl_EMPTY,
     Sl_FRESH,Sl_UNFRESH,Sl_ROTTEN2,Sl_DIRT,Sl_WATER,Sl_REM,
     Sl_W1,Sl_W2,Sl_Z1,Sl_Z2,Sl_USER,Sl_USERNAME,
     Sl_USER1,Sl_USER2,Sl_DATE_IN,Sl_LOCNAME,Sl_RATE,Sl_TFEE,Sl_PRICE,Sl_AMT,
	 S2_D3NAME,S2_D4NAME,S2_D5NAME,
};
*/

DB add;
char TmpTco[500];


char *Z[] = {
"\nMM/s  :E%sF                     PA/C CODE : E%sFM",
"\n      %s",
"\nName :%s Position: %s",
""};
enum slip2 {S2_TICKET=1,S2_LORRY,S2_DATE,S2_AGENT,S2_COMM0,S2_ALLOW0,
S2_ACCODE,S2_ACCODE9,S2_LOCATE,S2_DRIVER1,S2_COMM1,S2_ALLOW1,
S2_DRIVER2,S2_COMM2,S2_ALLOW2,S2_DRIVER3,S2_COMM3,S2_ALLOW3,S2_DRIVER4,
S2_COMM4,S2_ALLOW4,S2_DRIVER5,S2_COMM5,S2_ALLOW5,S2_PT,S2_PTNAME,
S2_DN,S2_CASHBILL,S2_MUDA1,S2_MUDA2,S2_BUSUK1,S2_BUSUK2,
S2_HUJAN1,S2_HUJAN2,S2_FIRST,S2_SECOND,S2_WASTAGE,S2_NET,S2_RATE,
S2_TFEE,S2_HFEE,S2_PRICE,S2_AMT,S2_SUBSIDY,S2_SAMT,
S2_TIMEIN,S2_TIMEOUT,S2_DURATION,S2_REM,S2_UPDATE,
S2_W1,S2_W2,S2_Z1,S2_Z2,S2_PORLA,S2_EDATE,S2_LOCATION,
S2_LESS,S2_FFBAMT,S2_GROSSAMT,S2_totalAMT,S2_totalSAMT};

void PrintEmployeeList(FILE *ptr, DB *df,int DRIVER1)
{
    long rec = qd(df->dat[DRIVER1],&driver);
    if(rec>0){
        qd(driver.dat[DR_POSITION], &position);
        fprintf(ptr,Z[2], driver.dat[2] , position.dat[2]);
    }
}

extern void PrintTicketSlip(FILE *ptr,char *tl,DB *df);
void prec(long irec,FILE *ptr,DB *df)
{

	dopen(&add,"add.scr");
	TmpTco[0] = 0;
    int  i,t1,t2,w1,w2,viewonly=0;
    char time[20],Dn[20];

	#ifdef _DEBUG
       viewonly = 1;
	#endif //_DEBUG

    if(viewonly) {
        ptr = topen("slip.txt");
    }
	else{
		ptr = fopen("lpt1","w");
	}
	
	setvbuf(ptr,NULL,_IONBF,0);
    PrintTicketSlip(ptr,title,df);
    long rec = qd(df->dat[LOCATE],&locate);

    if ( atol(df->dat[SECOND]) > atol(df->dat[FIRST]) ){
        w1 = SECOND;  t1 = TIMEOUT;
        w2 = FIRST;   t2 = TIMEIN;
    }
    else {
        w2 = SECOND;  t2 = TIMEOUT;
        w1 = FIRST;   t1 = TIMEIN;
    }
    char Wt[40], szTmp[40];
    space_str(szTmp, 30);
    char chMode = df->name[0];
    if(chMode=='R') {
         mset(szTmp, "Receiving Ticket");
         fprintf(ptr,"\n%10.10s%E%sF\n", "", szTmp);
    }
    else if(chMode=='D') {
         mset(szTmp, "Despatch Ticket");
         fprintf(ptr,"\n%10.10s%E%sF\n", "", szTmp);
    }
    else if(chMode=='C') {
         mset(szTmp, "Cash Bill");
         mset(szTmp, "Receiving Ticket");
         fprintf(ptr,"\n%10.10s%E%sF\n", "", szTmp);
    }
    lsetblank(&add);
    for (i=0;Z[i][0];i++) {
        switch(i) {
        case 0:
            qd(df->dat[ACCODE],&accode);
            fprintf(ptr,Z[i],accode.dat[2],accode.dat[1]);
            break;
        case 1:
            qd(df->dat[ACCODE],&accode);
            ShortAddress(TmpTco,&accode,CU_ADD1);
            split(&add,1, 2, TmpTco);
            if(!isblank(add.dat[1])) {
                fprintf(ptr,Z[i],add.dat[1]);
            }
            if(!isblank(add.dat[2])) {
                fprintf(ptr,Z[i],add.dat[2]);
            }
            break;
        case 2:
            for(int i=0; i<5; i++) {
                PrintEmployeeList(ptr,df,DRIVER1+i*3);
            }
            break;
        }
    }

    if(df->name[0]=='C') {
        dopen(&report,"slip0.rep");
    }
    else if(df->val(AMT)==0) {
        dopen(&report,"slip2.rep");
    }
    else {
        if(df->val(LESS)==0) {
            dopen(&report,"slip3.rep");
        }
        else {
            dopen(&report,"slip1.rep");
        }
    }
    fetch(&report, df);
    qd(df->dat[LOCATE],&locate);
    lset(report.dat[S2_LOCATION], locate.dat[2]);
    qd(df->dat[PT],&prodtype);
    lset(report.dat[S2_PTNAME], prodtype.dat[2]);
    qd(df->dat[ACCODE],&accode);
    lset(report.dat[S2_PORLA], accode.dat[CU_POLAR]);
    lset(report.dat[S2_EDATE], accode.dat[CU_EXPIRY]);
    lset(report.dat[S2_TIMEIN], df->dat[t1]);
    lset(report.dat[S2_TIMEOUT], df->dat[t2]);
    ltodb( __max( atol(df->dat[FIRST]), atol(df->dat[SECOND])), &report, S2_FIRST);
    ltodb( __min( atol(df->dat[FIRST]), atol(df->dat[SECOND])), &report, S2_SECOND);
    dtime(time,df->dat[TIMEIN],df->dat[TIMEOUT]);
    lset(report.dat[S2_DURATION], time);
    dtodb( df->val(MUDA2) /1000,  &report, S2_MUDA2);
    dtodb( df->val(BUSUK2) /1000,  &report, S2_BUSUK2);
    dtodb( df->val(HUJAN2) /1000,  &report, S2_HUJAN2);
    double ffbamt = df->val(AMT) + df->val(LESS);
    dtodb(ffbamt,&report,S2_FFBAMT);
    ffbamt = df->val(NET) /1000 * df->val(TFEE);
    dtodb(ffbamt,&report,S2_TFEE);
    ffbamt = df->val(NET) /1000 * df->val(HFEE);
    dtodb(ffbamt,&report,S2_HFEE);
    ffbamt = df->val(NET) /1000 * df->val(PRICE);
    dtodb(ffbamt,&report,S2_GROSSAMT);
    ffbamt = report.val(S2_SAMT) + report.val(S2_FFBAMT);
    dtodb(ffbamt,&report,S2_totalAMT);
    ffbamt = report.val(S2_SAMT) + report.val(S2_FFBAMT) - df->val(LESS);
    dtodb(ffbamt,&report,S2_totalSAMT);

    mprint(ptr,&report);
    dclose(&report);
    fprintf(ptr,"\n");
    if(viewonly) {
      fclose(ptr);
      junction("slip.txt",0);
    }
	dclose(&add);
}

char text[200];
void prec_slip(long irec,FILE *ptr,DB *df)
{
	if (profile.dat[Pro_TEXTMODE][0]=='T' || profile.dat[Pro_TEXTMODE][0]=='Y'){
        prec(irec,ptr,df);
	}
	else{
		df->m_PrintOneFormOnly = TRUE;   
#ifdef _DEBUG
		df->m_PreviewCrystalReport = TRUE;
		df->m_Direct = TRUE;
#else
		df->m_PreviewCrystalReport = FALSE;
		df->m_Direct = TRUE;
#endif //_DEBUG
		char filename[280];
		sprintf(filename,"%s.rpt",df->name);
		df->PrintOneFormOnly(filename);		
	}
}


void pslip1(long irec,FILE *ptr,DB *df)
{
    CPrintProgress  p1;
    p1.irec = irec;
    p1.ptr  = ptr;
    p1.df   = df;
    p1.prec = prec_slip;
    p1.DoModal();
}
