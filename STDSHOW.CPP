#include "stdafx.h"
#include "menu.h"

int  AmtField,Y_Caret;
double ItemAmt;
void stdshow(BinC *wc,DB *data,int flag)
{
	long irec;

//    wc->wi->last = (int) interval(&irec,wc->mf->dat[1], data,1);
    viewscr(wc->wi,irec,wc->Frame, data,wc->rep);
    ItemAmt = sum(1, data, AmtField,irec, (long) wc->wi->last);
    ptotal(wc->wi->x+wc->wi->No+2,Y_Caret+2,"Total :",11,2,ItemAmt);
}

double CreditAmt,DebitAmt;
void stdshow9(BinC *wc,DB *data,int flag)
{
	long irec;

    wc->wi->last = (int) interval(&irec,wc->mf->dat[1], data,1);
    viewscr(wc->wi,irec,wc->Frame, data,wc->rep);
    DebitAmt = sum(1, data, Gi_DEBIT,irec, (long) wc->wi->last);
    ptotal(wc->wi->x+wc->wi->No+2,56,"",11,2,DebitAmt);
    CreditAmt = sum(1, data, Gi_CREDIT,irec, (long) wc->wi->last);
    ptotal(wc->wi->x+wc->wi->No+2,69,"",11,2,CreditAmt);
}
