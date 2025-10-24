// Daily.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Daily.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDaily dialog


CDaily::CDaily(UINT IDD,DB *pdf,int Idx,void(*p)(),CWnd* pParent /*=NULL*/)
    : CTio(IDD,pdf,Idx,p,pParent)
{
	//{{AFX_DATA_INIT(CDaily)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDaily::DoDataExchange(CDataExchange* pDX)
{
    CTio::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDaily)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


void CDaily::show_tio()
{
    long rec;

    TmpTio[0] = 0;
    rec    = qd(tscr.dat[Da_PT],&prodtype);
    if(rec>0){
        strcpy(TmpTio,"Product : ");
        strcat(TmpTio,prodtype.dat[2]);
    }
    rec    = qd(tscr.dat[Da_ACCODE],&accode);
    if(rec>0){
        strcat(TmpTio,"\nA/C Code: ");
        strcat(TmpTio,accode.dat[2]);
    }
    if(tscr.dat[Da_CTR][0]!='*') {
        strcat(TmpTio,"\nContract No: ");
        strcat(TmpTio,tscr.dat[Da_CTR]);
    }
    _PL[1] = TmpTio;
    _PL[11] = tscr.dat[Da_TIME1];
    _PL[12] = tscr.dat[Da_TIME2];
}

BEGIN_MESSAGE_MAP(CDaily, CTio)
	//{{AFX_MSG_MAP(CDaily)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDaily message handlers

int tscr_flag(DB *df)
{
    if(!strcmp(df->dat[DATE],tscr.dat[Da_D1])) {
        if(strcmp(df->dat[TIMEOUT],tscr.dat[Da_TIME1])<0) return 0;
    }
    else if(!strcmp(df->dat[DATE],tscr.dat[Da_D2])) {
        if(strcmp(df->dat[TIMEOUT],tscr.dat[Da_TIME2])>0) return 0;
    }

    if(!strcmp(df->dat[DATE],tscr.dat[Da_D1]) && !strcmp(df->dat[DATE],tscr.dat[Da_D2])) {
        if(strcmp(df->dat[TIMEOUT],tscr.dat[Da_TIME1])<0) return 0;
        if(strcmp(df->dat[TIMEOUT],tscr.dat[Da_TIME2])>0) return 0;
    }
    return 1;
}
