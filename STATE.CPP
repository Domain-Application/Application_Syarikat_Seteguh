// Master.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "month.h"
#include "PrintJob.h"

char inv_key[200];
enum state {ST_DATE=2,ST_LOCATE,ST_PT,ST_TICKET,ST_LORRY,ST_NET,
            ST_RATE,ST_PRICE,ST_GROSS,ST_TFEE,ST_HFEE,ST_NP,ST_DEBIT,
            ST_NO,ST_ACCODE3};
enum sto   {So_NO=2,So_DATE,So_PT,So_TICKET,So_DEBIT,So_CREDIT,So_BAL};

int  receive_no,report_no;
char receive_key[10];
char StatementRefNo[80];

void SaveFfbPrice()
{
}

void put_state(DB *df)
{
    if(strcmp(receive_key,receive.dat[UPDATE])){
        SaveFfbPrice();
    }
    fetch_rec(&state,&receive);
    backup(&accode);
    qd(receive.dat[LOCATE],&locate);
    lset(state.dat[ST_LOCATE],locate.dat[2]);

    double net_price = (df->val(PRICE) - df->val(TFEE) - df->val(HFEE));
    dtodb(net_price,&state,ST_NP);

    double amount = receive.val(AMT);
    dtodb(amount,&state,ST_DEBIT);

    double weight = df->val(NET) / 1000;
    double amt    = weight * df->val(PRICE);
    dtodb(amt,&state,ST_GROSS);
    double hfee   = weight * df->val(HFEE);
    dtodb(hfee,&state,ST_HFEE);
    double tfee   = weight * df->val(TFEE);
    dtodb(tfee,&state,ST_TFEE);

    lset(state.dat[ST_PT],prodtype.dat[2]);
    lset(state.dat[ST_DATE], mdate(df->dat[Date]));
    lset(state.dat[1], inv_key);
    lset(state.dat[ST_NO], StatementRefNo);
    memwrt(&state);

    if(receive.val(SAMT)) {
        fetch_rec(&state,&receive);
        backup(&accode);
        dtodb(0,&state,ST_NET);
        lset(state.dat[ST_LOCATE],"***SUBSIDY***");
        rset(state.dat[ST_PRICE],receive.dat[SUBSIDY]);

        double amount = receive.val(SAMT);
        dtodb(amount,&state,ST_DEBIT);
        lset(state.dat[ST_PT],prodtype.dat[2]);
        lset(state.dat[ST_DATE], mdate(df->dat[Date]));
        lset(state.dat[1], inv_key);
        memwrt(&state);
    }
    restore(&accode);
}

int rep_state()
{
    int  flag=0,kk=0;

    long rec   = dseek(tscr.dat[St_D1],&receive,Date,SF);
    long last  = dseek(tscr.dat[St_D2],&receive,Date,SL);
    fetch_set(&state,&receive);
    for( ; rec<=last && rec>0; rec++) {
        ridx(rec,&receive,Date);
        if(!strcmp(receive.dat[ACCODE],inv_key)) {
            if(strcmp(receive.dat[PT],"01")>=0 && strcmp(receive.dat[PT],"03")<=0){
                qq(receive.dat[PT],&prodtype);
                if(kk==0) {
                    SaveFfbPrice();
                    kk = 1;
                }
                put_state(&receive);
            }
            else {
                flag++;
            }
        }
    }
    return flag;
}

void put_despatch(DB *df)
{
    fetch_rec(&state,&despatch);
    lset(state.dat[ST_TICKET],despatch.dat[DN]);
    double amount = despatch.val(AMT) * -1;
    double price = despatch.val(PRICE) - despatch.val(TFEE) - despatch.val(HFEE);
    dtodb(price,&state,ST_NP);
    dtodb(amount,&state,ST_DEBIT);
    qd(despatch.dat[LOCATE],&locate);
    lset(state.dat[ST_LOCATE],locate.dat[2]);
    lset(state.dat[ST_PT],prodtype.dat[2]);
    lset(state.dat[ST_DATE], mdate(df->dat[Date]));
    lset(state.dat[1], inv_key);
    memwrt(&state);

    amount = despatch.val( SAMT ) * -1;
    if(amount==0) return;

    fetch_rec(&state,&despatch);
    lset(state.dat[ST_TICKET],despatch.dat[DN]);
    dtodb(amount,&state,ST_DEBIT);
    dtodb(0,&state,ST_NET);
    lset(state.dat[ST_LOCATE],"***SUBSIDY***");
    rset(state.dat[ST_PRICE],despatch.dat[SUBSIDY]);
    lset(state.dat[ST_PT],prodtype.dat[2]);
    lset(state.dat[ST_DATE], mdate(df->dat[Date]));
    lset(state.dat[1], inv_key);
    memwrt(&state);
}

int rep_despatch()
{
    int  flag=0,kk=0;

    long rec  = dseek(tscr.dat[St_D1],&despatch,Date,SF);
    long last = dseek(tscr.dat[St_D2],&despatch,Date,SL);
    fetch_set(&state,&despatch);
    for( ; rec<=last && rec>0; rec++) {
        ridx(rec,&despatch,Date);
        if(!strcmp(despatch.dat[ACCODE],inv_key)) {
            if(strcmp(despatch.dat[PT],"01")>=0 && strcmp(despatch.dat[PT],"03")<=0){
                qq(despatch.dat[PT],&prodtype);
                if(kk==0) {
                    SaveFfbPrice();
                    kk = 1;
                }
                put_despatch(&despatch);
            }
            else {
                flag++;
            }
        }
    }
    return flag;
}

void put_payment()
{
    double total = payment.val( PY_TOTAL );

    lsetblank(&sto);
    lset(sto.dat[So_TICKET],payment.dat[PY_REFNO]);
    lset(sto.dat[So_DATE],payment.dat[PY_DATE]);
    if(total>=0){
        lset(sto.dat[So_NO],"2");
        dtodb(total,&sto,So_CREDIT);
    }
    else {
        lset(sto.dat[So_NO],"1");
        dtodb(total*-1,&sto,So_DEBIT);
    }
    lset(sto.dat[So_PT],payment.dat[PY_PARTICULAR]);
    lset(sto.dat[1], inv_key);
    memwrt(&sto);
}

void rep_payment()
{
    long rec,first,last;
    int  field[] = { PY_DATE, PY_TOTAL , 0 };

    first = dseek(tscr.dat[St_D1],&payment,PY_DATE,SF);
    last  = dseek(tscr.dat[St_D2],&payment,PY_DATE,SL);
    if(!first) return;
    for(rec=first; rec<=last  && rec>0; rec++) {
        ridx(rec,&payment,PY_DATE);
        if(!strcmp(payment.dat[PY_ACCODE],inv_key)) {
            put_payment();
        }
    }
}

void put_receipt()
{
    double total = receipt.val( PY_TOTAL );

    lsetblank(&sto);
    lset(sto.dat[So_TICKET],receipt.dat[PY_REFNO]);
    lset(sto.dat[So_DATE],receipt.dat[PY_DATE]);
    if(total>=0){
        lset(sto.dat[So_NO],"1");
        dtodb(total,&sto,So_DEBIT);
    }
    else {
        lset(sto.dat[So_NO],"2");
        dtodb(total*-1,&sto,So_CREDIT);
    }
    lset(sto.dat[So_PT],receipt.dat[PY_PARTICULAR]);
    lset(sto.dat[1], inv_key);
    memwrt(&sto);
}

void rep_receipt()
{
    long rec,first,last;
    int  field[] = { PY_DATE, PY_TOTAL , 0 };

    first = dseek(tscr.dat[St_D1],&receipt,PY_DATE,SF);
    last  = dseek(tscr.dat[St_D2],&receipt,PY_DATE,SL);
    if(!first) return;
    for(rec=first; rec<=last && rec>0; rec++) {
        ridx(rec,&receipt,PY_DATE);
        if(!strcmp(receipt.dat[PY_ACCODE],inv_key)) {
            put_receipt();
        }
    }
}

void put_ireq()
{
    double total= ireq.val( IQ_BAL );

    lsetblank(&sto);
    lset(sto.dat[So_TICKET],ireq.dat[IQ_REFNO]);
    lset(sto.dat[So_DATE],ireq.dat[IQ_DATE]);
    if(total>=0){
        dtodb(total,&sto,So_CREDIT);
        lset(sto.dat[So_NO],"2");
    }
    else {
        lset(sto.dat[So_NO],"1");
        dtodb(total*-1,&sto,So_DEBIT);
    }
    lset(sto.dat[So_PT],ireq.dat[IQ_PARTICULAR]);
    lset(sto.dat[1], inv_key);
    memwrt(&sto);

}

void rep_ireq()
{
    long rec,first,last;

    first = dseek(tscr.dat[St_D1],&ireq,IQ_DATE,SF);
    last  = dseek(tscr.dat[St_D2],&ireq,IQ_DATE,SL);
    for(rec=first; rec<=last && rec>0; rec++) {
        ridx(rec,&ireq,IQ_DATE);
        if(!strcmp(ireq.dat[IQ_ACCODE],inv_key)) {
            put_ireq();
        }
    }
}

void put_ientry()
{
    double total= ientry.val( IQ_BAL );

    lsetblank(&sto);
    lset(sto.dat[So_TICKET],ientry.dat[IQ_REFNO]);
    lset(sto.dat[So_DATE],ientry.dat[IQ_DATE]);
    if(total>=0){
        dtodb(total,&sto,So_DEBIT);
        lset(sto.dat[So_NO],"1");
    }
    else {
        lset(sto.dat[So_NO],"2");
        dtodb(total*-1,&sto,So_CREDIT);
    }
    lset(sto.dat[So_PT],ientry.dat[IQ_PARTICULAR]);
    lset(sto.dat[1], inv_key);
    memwrt(&sto);
}

void rep_ientry()
{
    long rec,first,last;

    first = dseek(tscr.dat[St_D1],&ientry,IQ_DATE,SF);
    last  = dseek(tscr.dat[St_D2],&ientry,IQ_DATE,SL);
    for(rec=first; rec<=last && rec>0; rec++) {
        ridx(rec,&ientry,IQ_DATE);
        if(!strcmp(ientry.dat[IQ_ACCODE],inv_key)) {
            put_ientry();
        }
    }
}

double TallyBF(DB *df,int fldDate, int fldKey, int fldAmount)
{
    double total = 0;
    long rec  = dseek(inv_key,df,fldKey,SF);
    long last = dseek(inv_key,df,fldKey,SL);
    for( ;rec<=last && rec>0; rec++){
        ridx(rec,df,fldKey);
        if( strcmp(df->dat[fldDate],tscr.dat[1])>=0 ) continue;
        total += df->val(fldAmount);
    }
    return total;
}

void rep_bf()
{
    double total = accode.val(CU_BALANCE);

    total += TallyBF(&receive,Date, ACCODE, AMT);
    total += TallyBF(&receive,Date, ACCODE, SAMT);
    total -= TallyBF(&despatch,Date, ACCODE, AMT);
    total -= TallyBF(&despatch,Date, ACCODE, SAMT);

    total -= TallyBF(&ireq,IQ_DATE, IQ_ACCODE, IQ_BAL);

    total += TallyBF(&ientry,IQ_DATE, IQ_ACCODE, IQ_BAL);

    total -= TallyBF(&payment,PY_DATE, PY_ACCODE, PY_TOTAL);
    total += TallyBF(&receipt,PY_DATE, PY_ACCODE, PY_TOTAL);

    if(total){
        lsetblank(&sto);
        lset(sto.dat[So_DATE],tscr.dat[1]);
        plusday(sto.dat[So_DATE],-1);
        lset(sto.dat[So_PT],"Balance of last Month");
        lset(sto.dat[So_NO],"0");
        if(total>=0){
            dtodb(total,&sto,So_DEBIT);
        }
        else {
            dtodb(total*-1,&sto,So_CREDIT);
        }
        lset(sto.dat[1], inv_key);
        memwrt(&sto);
    }
}

void rep_receive()
{
    long   last  = read_sumindex(0, &state);
    double total = sum( 0, &state, ST_DEBIT , 1, last);

    lsetblank(&sto);
    lset(sto.dat[So_DATE],tscr.dat[St_D2]);
    lset(sto.dat[So_PT],"F.F.B. TOTAL");
    if(total>=0){
        lset(sto.dat[So_NO],"1");
        dtodb(total,&sto,So_DEBIT);
    }
    else {
        lset(sto.dat[So_NO],"2");
        dtodb(total*-1,&sto,So_CREDIT);
    }
    lset(sto.dat[1], inv_key);
    memwrt(&sto);
}

void put_other()
{
    lsetblank(&sto);

    fetch(&sto,&receive);
    qd(receive.dat[PT],&prodtype);
    lset(sto.dat[So_PT],prodtype.dat[2]);

    lset(sto.dat[1], inv_key);
    memwrt(&sto);
}

void rep_other()
{
    long rec  = dseek(tscr.dat[St_D1],&receive,Date,SF);
    long last = dseek(tscr.dat[St_D2],&receive,Date,SL);
    fetch_set(&sto,&receive);
    readrec(1,&prodtype);                      //Read first record
    for( ; rec<=last  && rec>0; rec++) {
        ridx(rec,&receive,Date);
        if(!strcmp(receive.dat[ACCODE],inv_key)) {
            if(strcmp(receive.dat[PT],"01")>=0 && strcmp(receive.dat[PT],"03")<=0){
            }
            else {
                qq(receive.dat[PT],&prodtype);
                put_other();
            }
        }
    }
}

void modulate()
{
    int  field[] = { 1, 2, 3, 0};
    double balance=0;

    sort(&sto,field);
    for(long rec=1; rec<=sto.t->sumrecord; rec++){
        readrec(rec,&sto);
        balance += sto.val( So_DEBIT ) - sto.val( So_CREDIT );
        dtodb(balance,&sto,So_BAL);
        fastwrt(rec,&sto);
    }
}

void DoStatement(BOOL print_flag)
{
    int field[] = { ST_DATE,ST_TICKET, 0};

    strcpy(inv_key,accode.dat[1]);
    tado(&state,"state");
    tado(&sto,"sto");
    memstart(&sto);
    memstart(&state);
    rep_state();
    rep_despatch();

    rep_bf();
    rep_payment();
    rep_receipt();
    rep_ireq();
    rep_ientry();
    rep_other();                //Other Products

    memend(&state);
    if(state.t->sumrecord>0){
        rep_receive();
    }
    memend(&sto);

    modulate();

	long s1 = state.t->sumrecord;
	long s2 = sto.t->sumrecord;
    dclose(&sto);
    dclose(&state);

	if(s1==0 && s2<2) return;

    PrintRPT_withScr(&tscr, "state.rpt", print_flag);
}

void CMonth::OnPrint()
{
    UpdateData();
    ReplaceRecord();
    show_tio();

    if(strcmp(tscr.dat[St_ACCODE1],tscr.dat[St_ACCODE2])>0){
        swabstr(tscr.dat[St_ACCODE2],tscr.dat[St_ACCODE1]);
    }

    if(report_no) {
    }

    long rec  = dseek(tscr.dat[St_ACCODE1],&accode,1,SF);
    long last = dseek(tscr.dat[St_ACCODE2],&accode,1,SL);

    m_progress.SetRange32(rec, last);
    for(int i=0; rec<=last  && rec>0; rec++) {
        ridx(rec,&accode,1);
        m_progress.SetPos(rec);
        if(wildcmp(accode.dat[CU_TERM],tscr.dat[St_TERM])) continue;
        if(wildcmp(accode.dat[CU_GROUP],tscr.dat[St_GROUP]))  continue;
        if(wildcmp(accode.dat[CU_BANK_AC],tscr.dat[St_BANK]))  continue;

        sprintf(StatementRefNo,"%2.2s/%4.4s-%s",&tscr.dat[2][4],&tscr.dat[2][0],accode.dat[1]);
        lset(strun_no.dat[1],StatementRefNo);
        lset(strun_no.dat[2],tscr.dat[1]);
        lset(strun_no.dat[3],tscr.dat[2]);
        long xrec = dseek(strun_no.dat[1],&strun_no,1,MF);
        if(xrec==0) {
            long mrec = getdelrec(&strun_no);
            AppendIdx(mrec,&strun_no,1);
        }
        _PL[48] = StatementRefNo;
        DoStatement(m_flag);
    }
    CDialog::OnOK();
}

void ViewStatmentOfAccountByRunningNo()
{
    long xrec = viewidx(&strun_no);
    if(xrec==0) return;


    dopen(&tscr,"tso.scr");
    lset(tscr.dat[1], strun_no.dat[2]);
    lset(tscr.dat[2], strun_no.dat[3]);

    lset(accode.dat[1] , &strun_no.dat[1][8]);
    strcpy(StatementRefNo , strun_no.dat[1]);
    xrec = qd(accode.dat[1], &accode);
    if(xrec>0) {
        DoStatement(TRUE);
    }

    dclose(&tscr);
}

void StatementOfAccount()
{
    report_no = 0;
    dopen(&tscr,"tso.scr");

    CMonth t2(IDD_TSO,&tscr);
    t2.DoModal();

    dclose(&tscr);
}
