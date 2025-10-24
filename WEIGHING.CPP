// Weighing.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Weighing.h"
#include "GetWt.h"
#include "db_ado.h"
#include "comm.h"
#include "WaitUntilZero.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWeighing dialog


CWeighing::CWeighing(UINT IDD,DB *pdf,int Idx,CWnd* pParent /*=NULL*/)
    : CWb(IDD,pdf,Idx,pParent)
{
	//{{AFX_DATA_INIT(CWeighing)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    vehicle_fld = 0;
    m_mfile     = NULL;
    df          = NULL;
    w1_array = w2_array = NULL;
    WeighingMode = 1;
    LastLight = -1;
}

CWeighing::~CWeighing()
{
}


void CWeighing::DoDataExchange(CDataExchange* pDX)
{
    CWb::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWeighing)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWeighing, CWb)
	//{{AFX_MSG_MAP(CWeighing)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWeighing message handlers

void CWeighing::W1Start(long weight)
{
}

void CWeighing::W2Start(long weight)
{
}

void CWeighing::OnAddedit()
{
	int flag;
    CWb::OnAddedit();
    lsetblank(df);
    df->e->start(df);
    ShowInformation();

    CGetWt p1;
    p1.df = df;
    p1.m_vehicle = df->dat[vehicle_fld];
    flag = p1.DoModal();

	if(flag==IDCANCEL) {
         SaveFlag = CANNOTSAVE;
         OnCancel();
         return;
	}
    lset(df->dat[vehicle_fld],p1.m_vehicle);
    if(isblank(df->dat[vehicle_fld])) {
        if(df->rec<=0 || df->rec> df->t->sumrecord) df->rec = 1;
        df->rec       = ShowField1Table();
		if(df->rec ==0) {
			SaveFlag = CANNOTSAVE;
			OnCancel();
			return;
		}
        ExistRecordNo = ridx(df->rec,df,IdxItem);
	}
    CheckVehicleNo(df->dat[vehicle_fld]);               //vehicle no
    if(isblank(df->dat[vehicle_fld])) return;
    backup(df);
    //ExistRecordNo =  find(df->dat[vehicle_fld], 1l, vehicle_fld, df);
	ExistRecordNo =  df->Search(df->dat[vehicle_fld],vehicle_fld,TRUE);
    lset(df->dat[vehicle_fld],df->bak[vehicle_fld]);
    if(ExistRecordNo==0) {
        NewFlag  = NEWFILE;
        SaveFlag = NEWFILE;
        m_array  = w1_array;
        W1Start(p1.weight);
    }
    else {
        SaveFlag = OLDFILE;
        NewFlag  = OLDFILE;
        ReadTicketNo(m_mfile,1);                       //get ticket no
        lset(df->dat[1],m_mfile->dat[1]);
        m_array  = w2_array;
        W2Start(p1.weight);
    }
    if(m_array[1]==0) {
        ShowInformation();
        OnCancel();
        return;
    }
	CWnd *p = GetDlgItem(IDC_EDIT1+m_array[1]-1);
	if(p) {
		GotoDlgCtrl(p);
	}
    SetFieldEnable(m_array);
    ShowInformation();
    ShowSaveButton();
}


void CWeighing::SaveSecondWeightTicket(DB *fdes,DB *mfile)
{
    long mrec = appnet(mfile);

    fetch(mfile,fdes);
    FindAddInfoTicketNo(mfile);
    AppendIdx(mrec,mfile,1);

	fetch(df,mfile);
	ShowInformation();
}


void CWeighing::OnPrintTicket(DB *mainfile)
{
    FILE *ptr;

    ptr = fopen("lpt1","w");
    if(ptr) {
        setvbuf(ptr,NULL,_IONBF,0);
        (*mainfile->e->prec)(mainfile->rec,ptr,mainfile);
    }
    else {
        MessageBox("Printer I/O error");
    }
    fclose(ptr);

}


void CWeighing::W1Saved()
{
}

void CWeighing::W2Saved()
{
}

void CWeighing::OnDeleteFirstWeightRecord()
{
	char tmp[80];
	long frec =  df->Search(df->dat[vehicle_fld],vehicle_fld);
    if(frec>0){
        CString str;
        str.Format("DELETE FROM [%s] WHERE [%s]= '%s' ",
            df->m_strTable, df->f[vehicle_fld].name, df->dat[vehicle_fld]);
        RunSql(str);
        df->ReOpen();
    }
    else {
        sprintf(tmp,"%s Vehicle not found",df->dat[vehicle_fld]);
        MessageBox(tmp);
    }
}

void CWeighing::WaitUntilZero()
{
	return;  // ccwong 20120305
	if(!GwstWeight) return;

	CWaitUntilZero h;
	h.DoModal();
    GwstWeight = 0;  
}

void ComputeCommission(DB *df)
{
	qd(df->dat[ACCODE],&accode);
    double hComm = accode.val(CU_HCOMM);
    double tComm = accode.val(CU_TCOMM);
    double dComm = accode.val(CU_DCOMM);
    for(int i=0; i<5 ; i++) {
        int fld = DRIVER1+i*3;
		if (df->val(fld+1)>0) continue;
        long xrec = qd(df->dat[fld],&driver);
        if(driver.dat[DR_POSITION][0] == 'H') {
            dtodb(hComm , df, fld+1);
        }
        else if(driver.dat[DR_POSITION][0] == 'D') {
            dtodb(dComm , df, fld+1);
        }
        else if(driver.dat[DR_POSITION][0] == 'T') {
            dtodb(tComm , df, fld+1);
        }
    }
/*    if(companyNo==4 || companyNo==5 || companyNo==9) return;
	
    int iHarvest=0, iDriver=0, iTransp=0;
    for(int i=0; i<5 ; i++) {
        int fld = DRIVER1+i*3;
        long xrec = qd(df->dat[fld],&driver);
        if(driver.dat[DR_POSITION][0] == 'H') {
            iHarvest++;
        }
        else if(driver.dat[DR_POSITION][0] == 'D') {
            iDriver++;
        }
        else if(driver.dat[DR_POSITION][0] == 'T') {
            iTransp++;
        }
    }
    qd(df->dat[ACCODE],&accode);
    double hComm = accode.val(CU_HCOMM);
    double tComm = accode.val(CU_TCOMM);
    double dComm = accode.val(CU_DCOMM);
    for(i=0; i<5 ; i++) {
        int fld = DRIVER1+i*3;
        long xrec = qd(df->dat[fld],&driver);
        if(driver.dat[DR_POSITION][0] == 'H') {
            dtodb(hComm / iHarvest, df, fld+1);
        }
        else if(driver.dat[DR_POSITION][0] == 'D') {
            dtodb(dComm / iDriver, df, fld+1);
        }
        else if(driver.dat[DR_POSITION][0] == 'T') {
            dtodb(tComm / iTransp, df, fld+1);
        }
    }
	*/
}


void CWeighing::OnSave()
{
    int  flag;

	if(SaveFlag == CANNOTSAVE) {
		MessageBox("Can not save");
	}
	else if(SaveFlag == NEWFILE){
		ReplaceRecord();
        flag = (*df->e->save_first_wb)(df,0);
        if(flag==0) return;
        waitflock(df);
        CountTicketNo(df,1);
        save_entry(NONEXIST,IdxItem,0,df);      //TRUE: NO NEED LOCK AGAIN
        W1Saved();
        unflock(df);
        if(df->e->prec != unprint) {
        }
        WaitUntilZero();
        OnAddedit();
    }
	else if(SaveFlag == OLDFILE){
		ReplaceRecord();
        flag = (*df->e->save)(df,0);
        if(flag==0) return;
        waitflock(df);
        waitflock(m_mfile);
		ComputeCommission(df);
		if (strncmp(m_mfile->name,"RECEIVE",7)==0){
			if (df->val(AMT)>0){
				//int flag = AfxMessageBox("Cash Bill?",MB_YESNO | MB_ICONQUESTION);
				//if ( flag == IDYES ){					
				if(df->dat[CASHBILL][0]=='Y') {
					waitflock(&cashbill);
					SaveSecondWeightTicket(df,&cashbill);
					W2Saved();
					OnDeleteFirstWeightRecord();
					unflock(df);
					unflock(m_mfile);
					unflock(&cashbill);
					if(cashbill.e->prec != unprint) {
						OnPrintTicket(&cashbill);
					}
					//GwstWeight = 0; //ccwong 20120305
					OnAddedit();
					return;
				}
			}
			
		}
        SaveSecondWeightTicket(df,m_mfile);
        W2Saved();
        OnDeleteFirstWeightRecord();
        unflock(df);
        unflock(m_mfile);
        if(m_mfile->e->prec != unprint) {
            OnPrintTicket(m_mfile);
        }
        //GwstWeight = 0;  //ccwong 20120305
        OnAddedit();
	}
}

BOOL CWeighing::OnInitDialog() 
{
    if(vehicle_fld==0) {
        MessageBox("Please assign vehicle field no");
        CDialog::OnCancel();
        return TRUE;  // return TRUE unless you set the focus to a control
    }
    if(m_mfile == NULL) {
        MessageBox("Please assign ticket DB pointer");
        CDialog::OnCancel();
        return TRUE;  // return TRUE unless you set the focus to a control
    }
    if(df == NULL) {
        MessageBox("Please assign first weight DB pointer");
        CDialog::OnCancel();
        return TRUE;
    }
    if( w1_array == NULL ) {
        MessageBox("Please assign first weight array");
        CDialog::OnCancel();
        return TRUE;  // return TRUE unless you set the focus to a control
    }
    if( w2_array == NULL ) {
        MessageBox("Please assign second weight array");
        CDialog::OnCancel();
        return TRUE;  // return TRUE unless you set the focus to a control
    }
    HICON m_hIcon = AfxGetApp()->LoadIcon(IDI_WB);
    this->SetIcon(m_hIcon, TRUE);         // Set big icon

    CWb::OnInitDialog();

    PostMessage(WM_COMMAND,ID_ADDEDIT);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWeighing::OnDirectSave()
{
    OnSave();
}

void CWeighing::SetOneTimeScreen()
{
}

void CWeighing::OnNew()
{
     OnAddedit();
}

int CWeighing::GetRecord()
{
    OnAddedit();
    return 1;
}
