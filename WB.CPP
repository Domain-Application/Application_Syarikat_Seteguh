// Wb.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Wb.h"
#include "mgrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWb dialog


CWb::CWb(UINT IDD,DB *pdf,int Idx,CWnd* pParent /*=NULL*/)
    : CStaff(IDD,pdf,Idx,pParent)
{
	//{{AFX_DATA_INIT(CWb)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CWb::DoDataExchange(CDataExchange* pDX)
{
    CStaff::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWb)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

double m[15];
void compute_weight(DB *df)
{
    memset(m,0,sizeof(m));
    double weight1 = atof(df->dat[PGROSS]);
    double weight2 = atof(df->dat[PTARE]);
    if(weight1>weight2) {
        m[2] = weight1; m[5] = weight2;
    }
    else {
        m[5] = weight2; m[2] = weight1;
    }
    m[7] = df->val(WASTAGE);
    m[1] = m[2];
    m[4] = m[5];
    m[10] = m[11] = fabs(m[5]-m[2]) - m[7];

    m[8] = m[10] - m[6];
	//14/3/2008 CHEE follow BUKIT SIPUT
    if (weight2){
		dtodb(m[11], df, NET);
	}
	else{
		dtodb(0, df, NET);
	}
}

BOOL CWb::IsCannotEdit()
{
	char txt[100];
	long xrec = 0;
	if(!stricmp(df->name,"despatch")) {
		xrec = aroinv_wb.Search(df->dat[1],VIWB_DO,TRUE);
		strcpy(txt,aroinv_wb.dat[1]);
	}
	else{
		xrec = apoinv_wb.Search(df->dat[1],VIWB_DO,TRUE);
		strcpy(txt,apoinv_wb.dat[1]);
	}
	if(xrec){
		CString str;
		str.Format("Ticket is issued to Invoice:%s\nNot allow modify.",trim(txt));
		AfxMessageBox(str);
		return TRUE;
	}
	return FALSE;
		
}


void CWb::after_save(int ExistFlag)
{
    CStaff::after_save(ExistFlag);
	long Srec = qd(df->dat[MILL],&accode);
	if (Srec>0){
		Srec = despatch.Search(df->dat[1],1);
		if (Srec<=0){
			lsetblank(&despatch);
			fetch(&despatch,df);
			lset(despatch.dat[ACCODE],df->dat[MILL]);
			qd(despatch.dat[ACCODE],&accode);
			lset(despatch.dat[PORLA_AC],accode.dat[CU_PORLA_AC]);
			lset(despatch.dat[MILL],df->dat[ACCODE]);
			int  j;
			int fld=0;
			for(j=0; j<=4; j+=1){
				fld = DRIVER1+j*3;
				lset(despatch.dat[fld],"");
				dtodb(0,&despatch,fld+1);
				dtodb(0,&despatch,fld+2);								
			}   
			despatch.AddNew();
		}
	}
}

void CWb::SetOneTimeScreen()
{
}

void CWb::ShowInformation()
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

BEGIN_MESSAGE_MAP(CWb, CStaff)
	//{{AFX_MSG_MAP(CWb)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWb message handlers

void ReplaceReceiveGrid(long rec,CMSFlexGrid *grid,DB *df,int IdxItem)
{
    MGRID g(grid,df,rec);

    g.SetText(df,0,1,Date,ACCODE,LORRY,PT,DRIVER1,-1);
    compute_weight(df);
    g.SetText(m[11],6,0);
}

void SetReceiveGridTitle(CMSFlexGrid *grid,DB *df,int IdxItem)
{
    MGRID g(grid,df,0,8);

    g.ratio = float(1.0);
    g.SetTitle(df,0,1,Date,ACCODE,LORRY,PT,DRIVER1,-1);
    g.SetTitle("Net Weight",900,7);
}

int WB_initial(DB *df,int *fld)                //initial setting
{
    fld[0] = 0;
	/*
    if(atoi(user.dat[UR_LEVEL])<5) {
        fld[0] = PGROSS;
        fld[1] = PTARE;
    }
    fld[2] = 0;
	*/
	return 1;
}



void SetDefaultAc(DB *df)
{
	lset(df->dat[AGENT],accode.dat[CU_AGENT]);
	lset(df->dat[PORLA_AC],accode.dat[CU_PORLA_AC]);
	lset(df->dat[LOCATE],accode.dat[CU_LOCATE]);
	lset(df->dat[PT],"01");
	rset(df->dat[COMM0],accode.dat[CU_COMM0]);
	rset(df->dat[ALLOW0],accode.dat[CU_ALLOW0]);
	rset(df->dat[TFEE],accode.dat[CU_TFEE]);
	rset(df->dat[HFEE],accode.dat[CU_HFEE]);
	rset(df->dat[SUBSIDY],accode.dat[CU_SUBSIDY]);
	rset(df->dat[CASHBILL],accode.dat[CU_CASHBILL]);
}

void SetVehicleDefault(int i,DB *df)
{
    char text[20];
    int  j;
	
    for(j=0; j<=2; j+=1){
        if(lorry.dat[LO_FLAG1+j*2][0]=='Y'){
            lset(df->dat[DRIVER1+j*3],lorry.dat[LO_FLAG1+j*2+1]);
        }
    }    
    lset(df->dat[ACCODE],lorry.dat[LO_ACCODE]);
	lset(df->dat[PT],lorry.dat[LO_PT]);
    long Srec = qd(df->dat[ACCODE],&accode);
	if (Srec>0){
		SetDefaultAc(df);
	}
    
}

int tell_receive(UINT nChar,int &id,int &fld,DB *df,int sq)
{
	long Srec=0;
	long xrec = 0;
    compute_weight(df);
	switch(fld) {
	case MUDA1:
	case MUDA2:
	case BUSUK1:
	case BUSUK2:
	case HUJAN2:
        if (profile.dat[Pro_GRADING][0]=='F' || profile.dat[Pro_GRADING][0]=='N'){
            id = DO;            
        }
		if (sq){
			double weight = df->val(MUDA2)  + df->val(BUSUK2) + df->val(HUJAN2);
            dtodb(weight,df,WASTAGE);
            compute_weight(df);            
		}
        break;
	case WASTAGE:
        if (profile.dat[Pro_GRADING][0]=='T' || profile.dat[Pro_GRADING][0]=='Y'){
            id = DO;
            double weight = df->val(MUDA2)  + df->val(BUSUK2) + df->val(HUJAN2);
            dtodb(weight,df,fld);
            compute_weight(df);            
        }
        break;
    case LORRY:
		if (sq){
			SetVehicleDefault(fld,df);
		}        
        break;
    case AMT:
    case SAMT:
	case NET:
        id = DO;
		break;        
	case ACCODE:
		if (sq){
			qd(df->dat[fld],&accode);
			SetDefaultAc(df);
		}		
		break;
    case SECOND:
    case FIRST:
		/*if(atol(user.dat[UR_LEVEL])<99) {
			if (!isblank(df->dat[fld])){
				id = DO;
			}    
        }*/		
        break;
	case COMM1:
    case ALLOW1:
        xrec = qd(df->dat[DRIVER1],&driver);
        if(xrec==0) id = DO;
		/*
		if (driver.dat[DR_POSITION][0]=='D'){
			if (isblank(df->dat[COMM1])){
				qd(df->dat[ACCODE],&accode);
				rset(df->dat[COMM1],accode.dat[CU_DCOMM]);
			}
		}
		*/
        break;
    case DRIVER2:
        xrec = qd(df->dat[DRIVER1],&driver);
        if(xrec==0) id = DO;
        break;
    case COMM2:
    case ALLOW2:
        xrec = qd(df->dat[DRIVER1],&driver);
        if(xrec==0) id = DO;
        xrec = qd(df->dat[DRIVER2],&driver);
        if(xrec==0) id = DO;
        break;
    case DRIVER3:
        xrec = qd(df->dat[DRIVER1],&driver);
        if(xrec==0) id = DO;
        xrec = qd(df->dat[DRIVER2],&driver);
        if(xrec==0) id = DO;
        break;
    case COMM3:
    case ALLOW3:
        xrec = qd(df->dat[DRIVER3],&driver);
        if(xrec==0) id = DO;
        break;
    case DRIVER4:
        xrec = qd(df->dat[DRIVER1],&driver);
        if(xrec==0) id = DO;
        xrec = qd(df->dat[DRIVER2],&driver);
        if(xrec==0) id = DO;
        xrec = qd(df->dat[DRIVER3],&driver);
        if(xrec==0) id = DO;
        break;
    case COMM4:
    case ALLOW4:
        xrec = qd(df->dat[DRIVER4],&driver);
        if(xrec==0) id = DO;
        break;
	case DRIVER5:
        xrec = qd(df->dat[DRIVER1],&driver);
        if(xrec==0) id = DO;
        xrec = qd(df->dat[DRIVER2],&driver);
        if(xrec==0) id = DO;
        xrec = qd(df->dat[DRIVER3],&driver);
        if(xrec==0) id = DO;
		xrec = qd(df->dat[DRIVER4],&driver);
        if(xrec==0) id = DO;
        break;
    case COMM5:
    case ALLOW5:
        xrec = qd(df->dat[DRIVER5],&driver);
        if(xrec==0) id = DO;
        break;
	case TIMEIN:
    case TIMEOUT:
        if(isblank(df->dat[fld])){
            _strtime(df->dat[fld]);
        }
        break;
    }
	//dtodb(0,df,AMT);
	//if (df->val(SECOND)>0) {
		double weight   = df->val(NET) / 1000;
		double price = (df->val(PRICE) - df->val(TFEE) - df->val(HFEE));
		if(price <=0) price = 0;
		double amt      = weight * price;
		amt     -= df->val(LESS);
		dtodb(amt,df,AMT);
		
		amt      = weight * df->val(SUBSIDY);
		dtodb(amt,df,SAMT);
	//}
	
    return 2;
}

int start_receive(DB *df)
{
    dbasedate(df->dat[Date]);
    _strtime(df->dat[TIMEIN]);
    return 1;
}

int receive_save_first_wb(DB *df,int idi)
{

    return 1;
}

int receive_save(DB *df,int idi)
{
    compute_weight(df);
    if(m[2]==0 && m[5]==0){
        waitmsg("Zero weight can not save");
        return 0;
    }
    long rec = qd(df->dat[PT],&prodtype);
    if(rec==0) {
        waitmsg("Please key in 'Product'");
        return 0;
    }
    rec = qd(df->dat[CUST],&accode);
    if(rec==0) {
        waitmsg("Please key in Customer/Supplier");
        return 0;
    }
	if(df->dat[CASHBILL][0]=='Y') {
        if(atof(df->dat[AMT])==0) {
            waitmsg("Please key in U/Price");
            return 0;
        }
    }
    return 1;
}


void SetWBGridRows(CMSFlexGrid *grid,DB *df)
{
    grid->SetRows(15);
}

void init_wb(DB *df)
{
    df->e->SetGridRows   = SetWBGridRows;
    df->e->rep       = ReplaceReceiveGrid;
    df->e->GridTitle = SetReceiveGridTitle;
    df->e->prec      = pslip1;
    df->e->tell      = tell_receive;
    df->e->start     = start_receive;
    df->e->save      = receive_save;
    df->e->initial   = WB_initial;
}

void init_first(DB *df)
{
    df->e->rep       = ReplaceReceiveGrid;
    df->e->GridTitle = SetReceiveGridTitle;
    df->e->prec = unprint;
    df->e->tell          = tell_receive;
    df->e->start         = start_receive;
    df->e->save_first_wb = receive_save_first_wb;
    df->e->save          = receive_save;
    df->e->initial       = WB_initial;
}
