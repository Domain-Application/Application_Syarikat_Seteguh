// ToCash.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "tio.h"

void DoPostingCashBillTicket()
{
	DB adoTmp;
	CString str,str1;
	str.Format("Select TICKET from receive where [date]>='%s' and ",DateStr(two.dat[1]));
	str1.Format("[date]<='%s' AND CASHBILL_FLAG ='Y' order by ticket",DateStr(two.dat[2]));
	str+=str1;
	
	ado(&adoTmp,"adoTmp",NULL,str);	
	long Srec = read_sumindex(0,&adoTmp);
	if(Srec>0) {
		for(long rec=1; rec<=Srec; rec++){
			readrec(rec,&adoTmp);			
			receive.Search(adoTmp.dat[1],1,TRUE);			
			fetch(&cashbill,&receive);			
			cashbill.AddNew();
			receive.Delete(adoTmp.dat[1],1);
		}
	}
	dclose(&adoTmp);
}

void PostingToCashBill()
{
    
    CTio t1(IDD_TWO,&two,0,DoPostingCashBillTicket);    
	t1.Status = 1; //Process Button
    t1.DoModal();
    
}