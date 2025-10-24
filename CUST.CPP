
// cust.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "multisort.h"

void PrintCustomer(long irec,FILE *ptr,DB *df)
{
    ropen(&report,"pcode.rep");
    StdPrintTableFormat(&one, df, &report, 0);
    dclose(&report);
}

void init_accode(DB *df)
{
    df->e->prec  = PrintCustomer;
}
