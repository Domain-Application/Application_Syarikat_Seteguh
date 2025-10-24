// Wrcv.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Wrcv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWrcv dialog


CWrcv::CWrcv(UINT IDD,DB *pdf,int Idx,CWnd* pParent /*=NULL*/)
    : CWeighing(IDD,pdf,Idx,pParent)
{
	//{{AFX_DATA_INIT(CWrcv)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CWrcv::DoDataExchange(CDataExchange* pDX)
{
    CWeighing::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWrcv)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWrcv, CWeighing)
	//{{AFX_MSG_MAP(CWrcv)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
void CWrcv::ShowInformation()
{
	char str[10];
	
	CStaff::ShowInformation();
    compute_weight(df);
	
	double weight   = df->val(NET) / 1000;
	double price = (df->val(PRICE) - df->val(TFEE) - df->val(HFEE));
	if(price <=0) price = 0;
	double amt      = weight * price;	
    dtoa(amt,str,9,2);
    SetDlgItemText(IDC_SUBTOTAL,putnum(str));
	
	int  j;
	int fld=0;
	for(j=0; j<=4; j+=1){
		fld = DRIVER1+j*3;
		qd(df->dat[fld],&driver);
		SetDlgItemText(IDC_POSITION1+j,driver.dat[DR_POSITION]);
	}
	
}
void CWrcv::W1Start(long weight)
{
    dbasedate(df->dat[Date]);
    _strtime(df->dat[TIMEIN]);
    ltodb(weight,df,FIRST);
    ltodb(weight + GwstWeight,df,W1);
    ltodb(weight,df,Z1);
	long Srec = qd(df->dat[LORRY],&lorry);
	if (Srec>0){
		SetVehicleDefault(LORRY,df);
	}
}

void CWrcv::W2Start(long weight)
{
    ltodb(weight,df,SECOND);
    dbasedate(df->dat[Date]);
    _strtime(df->dat[TIMEOUT]);
    ltodb(weight + GwstWeight,df,W2);
    ltodb(weight,df,Z2);
}

void CWrcv::W2Saved()
{
}

/////////////////////////////////////////////////////////////////////////////
// CWrcv message handlers
int  FirstBasicArray[] = { 0, ACCODE,AGENT,COMM0,ALLOW0,PORLA_AC,LOCATE,
        DRIVER1,COMM1,ALLOW1,DRIVER2,COMM2,ALLOW2,
        DRIVER3,COMM3,ALLOW3,DRIVER4,COMM4,ALLOW4,
        DRIVER5,COMM5,ALLOW5,
        PT,DN,FIRST,RATE,TFEE,HFEE,PRICE,WFEE,
        MUDA1,MUDA2,BUSUK1,BUSUK2,HUJAN2,
        TIMEIN,Date,0};
int  BasicArray[] = { 0,SECOND,WASTAGE,NET,RATE,TFEE,HFEE,PRICE,LESS,SUBSIDY,WFEE,
         PT,DN,CASHBILL,MUDA1,MUDA2,BUSUK1,BUSUK2,HUJAN2,REM,0};
int  FirstArray[80], Array[80];
void SetWrcvFields()
{
}

void WeighingProcess(DB *main,DB *first)
{
    memset(FirstArray,0,sizeof(FirstArray));
    memset(Array,0,sizeof(Array));
    int i,j,fld;
    for(i=j=1; fld = FirstBasicArray[i]; i++) {
        if(fld==0) break;
        if(UserLevel()<99) {
            if(fld==FIRST || fld==SECOND) continue;
        }
        FirstArray[j++] = fld;
    }
    for(i=j=1; fld = BasicArray[i]; i++) {
        if(fld==0) break;
        if(UserLevel()<99) {
            if(fld==FIRST || fld==SECOND) continue;
        }
        Array[j++] = fld;
    }

    CWrcv p1(first->ID,first,0);

    SetWrcvFields();
    p1.df          = first;
    p1.m_mfile     = main;
    p1.vehicle_fld = LORRY;
    p1.w1_array    = FirstArray;
    p1.w2_array    = Array;
    p1.LastField   = WFEE;
//    p1.m_WinPrint  = TRUE;
    p1.DoModal();
}
