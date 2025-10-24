// exp_inv.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Tio.h"



void ExportArinvItem(DB *df)
{
	//long Srec,irec,rec;
    int  i;
    double total=0;
	long glrec=0;
    CString str;
    str.Format("SELECT GLCODE,SUM(AMT) FROM IREQ_GLCODE WHERE INVOICE='%s' GROUP BY GLCODE",df->dat[1]);
    DB adoTmp;
	ado(&adoTmp,"adoTmp",NULL,str);
	long Srec = read_sumindex(0,&adoTmp);
	if (Srec){
		for(long rec=1;rec<=Srec; rec++){
			readrec(rec,&adoTmp);
			//append data to aroinv
			lsetblank(&aroinv);
			lset(aroinv.dat[1],df->dat[1]);
			ltodb(rec,&aroinv,Oiv_SQNO);
			lset(aroinv.dat[Oiv_GLCODE],adoTmp.dat[1]);
			glrec= qd(aroinv.dat[Oiv_GLCODE],&glcode);
			if (glrec>0){
				lset(aroinv.dat[Oiv_NAME],glcode.dat[2]);
			}
			else{
				CString strMsg;
				strMsg.Format("Invoice :%s GLCODE NOT FOUND",df->dat[1]);
				waitmsg(strMsg);
			}
//			lset(aroinv.dat[Oiv_AMOUNT],adoTmp.dat[2]);
			dtodb(adoTmp.val(2),&aroinv,Oiv_AMOUNT);
			total += aroinv.val(Oiv_AMOUNT);
			aroinv.AddNew();		
		}	
	}
	dclose(&adoTmp);
	
    
    dtodb(total,&arinv,IV_TOTAL);
	arinv.Update();
}

void TRansferToAR(char *key,DB *df)
{
    long xrec,mrec;

    lset(key,df->dat[1]);
    xrec = arinv.Search(key,1);
    if(xrec==0) {
        lsetblank(&arinv);
		fetch(&arinv,df);
        lset(arinv.dat[1],key);
        lset(arinv.dat[IV_DEBTOR],df->dat[IVWB_ACCODE]);
        lset(arinv.dat[IV_TERMS],df->dat[IVWB_TERMS]);
        lset(arinv.dat[IV_REM],"SALES");
        lset(arinv.dat[IV_TYPE],"IV");
        lset(arinv.dat[IV_CURRENCY],"RM");
        dtodb(1,&arinv,IV_RATE);
        arinv.AddNew();         
        ExportArinvItem(df);
    }
}

void DoExportOtherToArinv(char *main)
{
    long last,rec;
    char key[20];

    strcpy(key,arinv.dat[1]);
    
	CString strSql,strCmd;
	
    strSql.Format("SELECT * FROM [%s] WHERE ",main);
    strCmd.Format("[DATE]>='%s' ",DateStr(two.dat[1]));
    strSql+=strCmd;
    strCmd.Format("AND [DATE]<='%s' ",DateStr(two.dat[2]));
    strSql+=strCmd;
    strCmd.Format("AND [TT]='S' ORDER BY INVOICE");
    strSql+=strCmd;
	DB adoTmp;
	ado(&adoTmp,"adoTmp",NULL,strSql);
	long Srec = read_sumindex(0,&adoTmp);
	if (Srec){
		for(long rec=1;rec<=Srec; rec++){
			readrec(rec,&adoTmp);
			fetch(&arinv,&adoTmp);
			TRansferToAR(key,&adoTmp);
		}	
	}
	dclose(&adoTmp);
    
}

void ExportArinvItem_WB(DB *df)
{
    //long Srec,irec,rec;
    int  i;
    double total=0;
	long glrec=0;
    CString str;
    str.Format("SELECT GLCODE,SUM(AMT)AS AMT FROM DES_GLCODE WHERE INVOICE='%s' GROUP BY GLCODE",df->dat[1]);
    DB adoTmp;
	ado(&adoTmp,"adoTmp",NULL,str);
	long Srec = read_sumindex(0,&adoTmp);
	if (Srec){
		for(long rec=1;rec<=Srec; rec++){
			readrec(rec,&adoTmp);
			//append data to aroinv
			lsetblank(&aroinv);
			lset(aroinv.dat[1],df->dat[1]);
			ltodb(rec,&aroinv,Oiv_SQNO);
			lset(aroinv.dat[Oiv_GLCODE],adoTmp.dat[1]);
			glrec= qd(aroinv.dat[Oiv_GLCODE],&glcode);
			if (glrec>0){
				lset(aroinv.dat[Oiv_NAME],glcode.dat[2]);
			}
			else{
				CString strMsg;
				strMsg.Format("Invoice :%s GLCODE NOT FOUND",df->dat[1]);
				waitmsg(strMsg);
			}
			
//			lset(aroinv.dat[Oiv_AMOUNT],adoTmp.dat[2]);
			dtodb(adoTmp.val(2),&aroinv,Oiv_AMOUNT);
			total += aroinv.val(Oiv_AMOUNT);
			aroinv.AddNew();		
		}	
	}
	dclose(&adoTmp);
	
    
    dtodb(total,&arinv,IV_TOTAL);
	arinv.Update();
}

void TRansferWBToAR(char *key,DB *df)
{
    long xrec,mrec;
	
    lset(key,df->dat[1]);
    xrec = arinv.Search(key,1);
    if(xrec==0) {
        lsetblank(&arinv);
		fetch(&arinv,df);
        lset(arinv.dat[1],key);
        lset(arinv.dat[IV_DEBTOR],df->dat[IVWB_ACCODE]);
        lset(arinv.dat[IV_TERMS],df->dat[IVWB_TERMS]);
        lset(arinv.dat[IV_REM],"SALES");
        lset(arinv.dat[IV_TYPE],"IV");
        lset(arinv.dat[IV_CURRENCY],"RM");
        dtodb(1,&arinv,IV_RATE);
        arinv.AddNew();         
        ExportArinvItem_WB(df);
    }
}

void DoExportWBToArinv(char *main)
{
    long last,rec;
    char key[20];
	
    strcpy(key,arinv.dat[1]);
    
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
			fetch(&arinv,&adoTmp);
			TRansferWBToAR(key,&adoTmp);
		}	
	}
	dclose(&adoTmp);
    
}

void put_exp_inv()
{
    char tmp[80];

    FLASH = 0;
	DoExportWBToArinv("arinv_wb");  
    DoExportOtherToArinv("arinv_oth");    
    waitmsg("Export Completed");
    FLASH = 1;

}

void ExportArinvTransactions()
{
   
    
   CTio t1(IDD_TWO,&two,0,put_exp_inv);
   t1.Status = 1;
   t1.DoModal();



}
