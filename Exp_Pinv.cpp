// exp_inv.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Tio.h"

DB   exp_inv;
int  exp_invFlag;
enum exp_inv {Epi_GLCODE=1,Epi_AMOUNT};

void ExportApinvItem(DB *df)
{
	//long Srec,irec,rec;
    int  i;
    double total=0;
	long glrec=0;
    CString str;
    str.Format("SELECT GLCODE,SUM(AMT) FROM IENTRY_GLCODE WHERE INVOICE='%s' GROUP BY GLCODE",df->dat[1]);
    DB adoTmp;
	ado(&adoTmp,"adoTmp",NULL,str);
	long Srec = read_sumindex(0,&adoTmp);
	if (Srec){
		for(long rec=1;rec<=Srec; rec++){
			readrec(rec,&adoTmp);
			//append data to apoinv
			lsetblank(&apoinv);
			lset(apoinv.dat[1],df->dat[1]);
			ltodb(rec,&apoinv,Oiv_SQNO);
			lset(apoinv.dat[Oiv_GLCODE],adoTmp.dat[1]);
			glrec= qd(apoinv.dat[Oiv_GLCODE],&glcode);
			if (glrec>0){
				lset(apoinv.dat[Oiv_NAME],glcode.dat[2]);
			}
			else{
				CString strMsg;
				strMsg.Format("Purchase Invoice :%s GLCODE NOT FOUND",df->dat[1]);
				waitmsg(strMsg);
			}
			
			
			//rset(apoinv.dat[Oiv_AMOUNT],adoTmp.dat[2]);
			dtodb(adoTmp.val(2),&apoinv,Oiv_AMOUNT);
			total += apoinv.val(Oiv_AMOUNT);
			apoinv.AddNew();		
		}	
	}
	dclose(&adoTmp);
	
    
    dtodb(total,&apinv,IV_TOTAL);
	apinv.Update();
}

void TRansferToAP(char *key,DB *df)
{
    long xrec,mrec;

    lset(key,df->dat[1]);
    xrec = apinv.Search(key,1);
    if(xrec==0) {
        lsetblank(&apinv);
		fetch(&apinv,df);
        lset(apinv.dat[1],key);
        lset(apinv.dat[IV_DEBTOR],df->dat[IVWB_ACCODE]);
        lset(apinv.dat[IV_TERMS],df->dat[IVWB_TERMS]);
        lset(apinv.dat[IV_REM],"PURCHASE");
        lset(apinv.dat[IV_TYPE],"IV");
        lset(apinv.dat[IV_CURRENCY],"RM");
        dtodb(1,&apinv,IV_RATE);
        apinv.AddNew();         
        ExportApinvItem(df);
    }
}

void DoExportOtherToApinv(char *main)
{
    long last,rec;
    char key[20];

    strcpy(key,apinv.dat[1]);
    
	CString strSql,strCmd;
	
    strSql.Format("SELECT * FROM [%s] WHERE ",main);
    strCmd.Format("[DATE]>='%s' ",DateStr(two.dat[1]));
    strSql+=strCmd;
    strCmd.Format("AND [DATE]<='%s' ",DateStr(two.dat[2]));
    strSql+=strCmd;
    strCmd.Format("AND [TT]='P' ORDER BY REFNO");
    strSql+=strCmd;
	DB adoTmp;
	ado(&adoTmp,"adoTmp",NULL,strSql);
	long Srec = read_sumindex(0,&adoTmp);
	if (Srec){
		for(long rec=1;rec<=Srec; rec++){
			readrec(rec,&adoTmp);
			fetch(&apinv,&adoTmp);
			TRansferToAP(key,&adoTmp);
		}	
	}
	dclose(&adoTmp);
    
}

void ExportApinvItem_WB(DB *df)
{
    //long Srec,irec,rec;
    int  i;
    double total=0;
	long glrec=0;
    CString str;
    str.Format("SELECT GLCODE,SUM(AMT) FROM RCV_GLCODE WHERE INVOICE='%s' GROUP BY GLCODE",df->dat[1]);
    DB adoTmp;
	ado(&adoTmp,"adoTmp",NULL,str);
	long Srec = read_sumindex(0,&adoTmp);
	if (Srec){
		for(long rec=1;rec<=Srec; rec++){
			readrec(rec,&adoTmp);
			//append data to apoinv
			lsetblank(&apoinv);
			lset(apoinv.dat[1],df->dat[1]);
			ltodb(rec,&apoinv,Oiv_SQNO);
			lset(apoinv.dat[Oiv_GLCODE],adoTmp.dat[1]);
			glrec= qd(apoinv.dat[Oiv_GLCODE],&glcode);
			if (glrec>0){
				lset(apoinv.dat[Oiv_NAME],glcode.dat[2]);
			}
			else{
				CString strMsg;
				strMsg.Format("Purchase Invoice :%s GLCODE NOT FOUND",df->dat[1]);
				waitmsg(strMsg);
			}
			//rset(apoinv.dat[Oiv_AMOUNT],adoTmp.dat[2]);
			dtodb(adoTmp.val(2),&apoinv,Oiv_AMOUNT);
			total += apoinv.val(Oiv_AMOUNT);
			apoinv.AddNew();		
		}	
	}
	dclose(&adoTmp);
	
    
    dtodb(total,&apinv,IV_TOTAL);
	apinv.Update();
}

void TRansferWBToAP(char *key,DB *df)
{
    long xrec,mrec;
	
    lset(key,df->dat[1]);
    xrec = apinv.Search(key,1);
    if(xrec==0) {
        lsetblank(&apinv);
		fetch(&apinv,df);
        lset(apinv.dat[1],key);
        lset(apinv.dat[IV_DEBTOR],df->dat[IVWB_ACCODE]);
        lset(apinv.dat[IV_TERMS],df->dat[IVWB_TERMS]);
        lset(apinv.dat[IV_REM],"SALES");
        lset(apinv.dat[IV_TYPE],"IV");
        lset(apinv.dat[IV_CURRENCY],"RM");
        dtodb(1,&apinv,IV_RATE);
        apinv.AddNew();         
        ExportApinvItem_WB(df);
    }
}

void DoExportWBToApinv(char *main)
{
    long last,rec;
    char key[20];
	
    strcpy(key,apinv.dat[1]);
    
	CString strSql,strCmd;
	
    strSql.Format("SELECT * FROM [%s] WHERE ",main);
    strCmd.Format("[DATE]>='%s' ",DateStr(two.dat[1]));
    strSql+=strCmd;
    strCmd.Format("AND [DATE]<='%s' ",DateStr(two.dat[2]));
    strSql+=strCmd;
    strCmd.Format(" ORDER BY INVOICE");
    strSql+=strCmd;
	DB adoTmp;
	ado(&adoTmp,"adoTmp",NULL,strSql);
	long Srec = read_sumindex(0,&adoTmp);
	if (Srec){
		for(long rec=1;rec<=Srec; rec++){
			readrec(rec,&adoTmp);
			fetch(&apinv,&adoTmp);
			TRansferWBToAP(key,&adoTmp);
		}	
	}
	dclose(&adoTmp);
    
}

void put_exp_pinv()
{
    char tmp[80];

    FLASH = 0;
	DoExportWBToApinv("apinv_wb");  
    DoExportOtherToApinv("ientry");    
    waitmsg("Export Completed");
    FLASH = 1;

}

void ExportApinvTransactions()
{
    
    
   CTio t1(IDD_TWO,&two,0,put_exp_pinv);
   t1.Status = 1;
   t1.DoModal();



}
