// menu.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "menu.h"

DB renewal;
void Put_renewal()
{
    long rec,Srec;

    Srec = read_sumindex(1,&accode);
    fetch_set(&renewal,&accode);
    memstart(&renewal);
    for(rec=1;rec<=Srec;rec++) {
        ridx(rec,&accode,1);
        if(isblank(accode.dat[CU_EXPIRY]) || strcmp(accode.dat[CU_EXPIRY],two.dat[1]) >=0 && strcmp(accode.dat[CU_EXPIRY],two.dat[2])<=0) {
            fetch_rec(&renewal,&accode);
            memwrt(&renewal);
        }
    }
    memend(&renewal);
    if(renewal.t->sumrecord>0){
        stdplan(&two,&renewal);
    }
}
/*
void Renew()
{
    KB   key;

    dopen(&renewal,"renew.rep");
    setwild(&two);
    rset(two.dat[3],"55");
    key.code = keymon(&two,SubTitle);
    if(key.ch[1] == F1) {
        Put_renewal();
    }
    dclose(&renewal);
} */

void CheckPorlaLicenseExpiredDate()
{
	/*
    dopen(&renewal,"renew.rep");
    rset(two.dat[3],"55");
    lset(two.dat[1],"19900101");
    lset(two.dat[2],today);
    plusday(two.dat[2],90);
    Put_renewal();
    dclose(&renewal);
	*/
	int flag = AfxMessageBox("Check Porla Expiry Date?",MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2);
	if ( flag == IDYES ){
		PeriodCrystalRpt("renew.rpt","Porla Expiried");
	}
}
