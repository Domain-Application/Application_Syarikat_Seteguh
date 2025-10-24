// Salary.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Salary.h"
#include "db_ado.h"
#include "openrs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSalary dialog


CSalary::CSalary(UINT IDD,DB *pdf,int Idx,CWnd* pParent /*=NULL*/)
    : CStaff(IDD,pdf,Idx,pParent)
{
    //{{AFX_DATA_INIT(CSalary)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}


void CSalary::DoDataExchange(CDataExchange* pDX)
{
    CStaff::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CSalary)
        // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSalary, CStaff)
    //{{AFX_MSG_MAP(CSalary)
    ON_WM_DESTROY()
    ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, OnComputeSalary)
	ON_BN_CLICKED(IDC_BUTTON2, OnViewSalaryStatement)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSalary message handlers
void CSalary::SetAttribute(int flag,int fld)
{
    if(flag) {
		GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_HIDE);
    }
	else{
		GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_SHOW);
	}
    CStaff::SetAttribute(flag,fld);
}
void CSalary::ShowInformation()
{
    CStaff::ShowInformation();
}


BOOL CSalary::OnInitDialog()
{
    CStaff::OnInitDialog();

    // TODO: Add extra initialization here

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void CSalary::OnDestroy()
{
    CStaff::OnDestroy();

    // TODO: Add your message handler code here

}

void CSalary::OnTimer(UINT nIDEvent)
{
    // TODO: Add your message handler code here and/or call default

    CStaff::OnTimer(nIDEvent);
}


void AdminDbSalary(DB *df)
{
    if(df->ID==0) {                           
        AfxMessageBox("Database not open");   
        return;                               
    }                                         
    CSalary p1(df->ID,df,df->f[1].idx & 0x01);
    p1.DoModal();                             
}


void ComputeAdvance(DB *df)
{
    double amt = 0;
	CString str,str2;
	str.Format("Select sum(total) from advance where DRIVER='%s' AND [date]>='%s' and ",df->dat[Sl_DRIVER],DateStr(two.dat[1]));
	str2.Format("[DATE]<='%s'",DateStr(two.dat[2]));
	str+=str2;
	amt = SqlValue(str);

	str.Format("Select sum(total) from salary where DRIVER='%s' AND [date]>'%s' and ",df->dat[Sl_DRIVER],DateStr(two.dat[1]));
	str2.Format("[DATE]<='%s'",DateStr(two.dat[2]));
	str+=str2;
	amt -= SqlValue(str);

/*
    for(rec=0; rec<Srec; rec++) {
        ridx(rec+irec, &advance, PY_ACCODE);
        if( strcmp( advance.dat[PY_DATE], daily.dat[2]) <=0 ) {
            amt += advance.val(PY_TOTAL);
        }
    }

    dopen(&tpo,"salary");
    Srec = interval(&irec, df->dat[Sl_DRIVER], &tpo, Sl_DRIVER);
    for(rec=0; rec<Srec; rec++) {
        ridx(rec+irec, &tpo, Sl_DRIVER);
        if( strcmp( tpo.dat[Sl_DATE], daily.dat[2]) < 0 ) {
            amt -= tpo.val(Sl_NET_EARN);
        }
    }
    dclose(&tpo); */

    dtodb(amt , df, Sl_ADVANCE);
}

void ComputeSalaryTotal(DB *df)
{
    double amt = df->val(Sl_BASIC) + df->val(Sl_ALLOWANCE) + df->val(Sl_COMM) -
             df->val(Sl_DEDUCTION) + df->val(Sl_ADDITION)
             + df->val(Sl_A1)+ df->val(Sl_A2)+ df->val(Sl_A3);
    dtodb(amt, df, Sl_NET_SALARY);

    double amt2 = df->val(Sl_NET_SALARY) + df->val(Sl_EPF_BOSS) + df->val(Sl_SOCSO_B);
    dtodb(amt2 , df, Sl_TOTAL);
    double net = df->val(Sl_NET_SALARY) - df->val(Sl_EPF_STAFF) - df->val(Sl_SOCSO_S);
    dtodb(net , df, Sl_NET_EARN);
    double balance = df->val(Sl_NET_EARN) - df->val(Sl_ADVANCE);
    dtodb(balance , df, Sl_BALANCE);
}

void ComputeCommissionAllowance(DB *df)
{
	CString str,str2;
	str.Format("Select sum(comm_amt),sum(allow1) from rep_rcomm where DRIVER1='%s' AND [date]>='%s' and ",df->dat[Sl_DRIVER],DateStr(two.dat[1]));
	str2.Format("[DATE]<='%s'",DateStr(two.dat[2]));
	str+=str2;
	
	double comm  = SqlValue(str);
    dtodb(comm,df,Sl_COMM);
    double allow = SqlValue(str,2);
    dtodb(allow,df,Sl_ALLOWANCE);
}

void Epf( double *Salary )
{
    long   WEPF,COEPF;
    double Earning;
	
    if ( Salary[0]<10 ) {
        Salary[1] = Salary[2] = 0;
    } else {
        Earning = Salary[0]*100;
        WEPF = COEPF = (long) Earning;
        if ( WEPF<500001L )
            WEPF = ( (WEPF%2000)==0 ) ? WEPF : ( ((WEPF/2000)*2000)+2000 );
        else
            WEPF = ( (WEPF%10000)==0 ) ? WEPF : ( ((WEPF/10000)*10000)+10000 );
        WEPF = (long) (WEPF / 100 * atof(epf_rate.dat[EPFRATE_STAFF]));
        WEPF = ( (WEPF%100)==0 ) ? WEPF : ( ((WEPF/100)*100)+100 );
		
        if ( COEPF<500001L )
            COEPF = ( (COEPF%2000)==0 ) ? COEPF : ( ((COEPF/2000)*2000)+2000 );
        else
            COEPF = ( (COEPF%10000)==0 ) ? COEPF : ( ((COEPF/10000)*10000)+10000 );
        COEPF = (long) (COEPF / 100 * atof(epf_rate.dat[EPFRATE_BOSS]));
        COEPF = ( (COEPF%100)==0 ) ? COEPF : ( ((COEPF/100)*100)+100 );
		
        Salary[1] = (double) WEPF/100;
        Salary[2] = (double)COEPF/100;
    }
}

void Socso( double *salary )
{
    long rec;
	
    for(rec=1; rec<=socso.t->sumrecord; rec++){
        readrec(rec,&socso);
        if(salary[0]>=atof(socso.dat[SOCSO_BSALARY]) && salary[0]<=atof(socso.dat[SOCSO_ESALARY])){
            salary[1] = atof(socso.dat[SOCSO_EMPLOYEE]);
            salary[2] = atof(socso.dat[SOCSO_EMPLOYER]);
            break;
        }
    }
}

void ComputeEpfSocso(DB *df)
{
	double amt[10];
	memset(amt,0,sizeof(amt));
    backup(&two);
    lset(two.dat[1], df->dat[Sl_DATE]);
    lset(two.dat[2], df->dat[Sl_DATE]);
    firstday(two.dat[1]);
    lastday(two.dat[2]);
	qd(df->dat[Sl_DRIVER], &driver);
    double bs = driver.val(DR_SALARY);
    dtodb(bs, df, Sl_BASICPAY);
	
    if(driver.dat[DR_TYPE][0]=='D' || driver.dat[DR_TYPE][0]=='d') {
        if(df->val(Sl_RATE)==0) {
            dtodb(26, df, Sl_RATE);
        }
    }
    else {
        dtodb(1, df, Sl_RATE);
    }
    double basic_salary = bs * df->val(Sl_RATE);
    dtodb(basic_salary, df, Sl_BASIC);
    ComputeCommissionAllowance(df);
    ComputeAdvance(df);
	

    amt[0] = df->val(Sl_BASIC) + df->val(Sl_ALLOWANCE) + df->val(Sl_COMM) -
		df->val(Sl_DEDUCTION) + df->val(Sl_ADDITION)
		+ df->val(Sl_A1)+ df->val(Sl_A2)+ df->val(Sl_A3);
    dtodb(amt[0], df, Sl_NET_SALARY);
	
    amt[0] = df->val(Sl_BASIC);
    if(driver.dat[DR_EPF_COMM][0]=='Y') {
        amt[0] = df->val(Sl_BASIC) + df->val(Sl_COMM);
    }
    if(!isblank(driver.dat[DR_EPF])) {
        Epf( amt );
    }
    dtodb(amt[2], df, Sl_EPF_BOSS);
    dtodb(amt[1], df, Sl_EPF_STAFF);
    amt[1] = amt[2] = 0;
	/*
    if(driver.dat[DR_EPF_COMM][0]=='Y') {
        amt[0] = df->val(Sl_BASIC) + df->val(Sl_COMM);
    }
	*/
    amt[0] = df->val(Sl_BASIC);
    if(driver.dat[DR_SOCSO_COMM][0]=='Y') {
        amt[0] = df->val(Sl_BASIC) + df->val(Sl_COMM);
    }
    if(!isblank(driver.dat[DR_SOCSO])) {
        Socso( amt );
    }
    dtodb(amt[1], df, Sl_SOCSO_S);
    dtodb(amt[2], df, Sl_SOCSO_B);
	
    ComputeSalaryTotal(df);
	restore(&two);
}


int tell_salary(UINT nChar,int &flag,int &fld,DB *df,int sq)
{
    char text[40];
    double bs;

    switch(fld) {
    case Sl_COMM:
    case Sl_ALLOWANCE:
    case Sl_ADVANCE:
    case Sl_TOTAL:
    case Sl_NET_EARN:
    case Sl_BALANCE:
    case Sl_NET_SALARY:
        flag = DO;
        break;
    case Sl_DRIVER:
        if(sq){
             ComputeEpfSocso(df);
        }
        break;
    default:
        break;
    }
    bs = df->val(Sl_BASICPAY) * df->val(Sl_RATE);
    dtodb(bs, df, Sl_BASIC);
	ComputeSalaryTotal(df);
    return 2;
}

int salary_initial(DB *df,int *fld)                //initial setting
{
    fld[0] = Sl_BASIC;
    return 1;
}

void init_salary(DB *df)
{
    df->e->tell    = tell_salary;
    df->e->initial = salary_initial;
}

void CSalary::OnComputeSalary() 
{
	ComputeEpfSocso(df);
	ShowInformation();
}

DB salaryflow(IDD_SALARY,6);
void ViewSalaryFlow(DB *df)
{
    CString strSql;
	
    strSql.Format("select * from [salaryflow] where [driver]=%s order by [date],[sqno],[refno]",
		DbText(df,Sl_DRIVER));
	
    salaryflow.DefaultOrder = FALSE;
	
    salaryflow.rep_field = 6;//(UserLevel()<9)  ? 6 : 10;
    dopen(&salaryflow,"salflow.rep");
	
    OpenRs  p1(strSql);
    p1.ToDBF(&salaryflow);
    AutoDebitCreditBalance(&salaryflow,4);
    //AutoDebitCreditBalance(&salaryflow,8);
    viewidx(&salaryflow);
    dclose(&salaryflow);
}

void CSalary::OnViewSalaryStatement() 
{
	ViewSalaryFlow(df);
}
