// GetWt.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "GetWt.h"
#include "GwEdit.h"
#include "mgrid.h"
#include "AskPassword.h"
#include "comm.h"
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGetWt dialog


CGetWt::CGetWt(CWnd* pParent /*=NULL*/)
	: CDialog(CGetWt::IDD, pParent)
{
    m_WtFlag= TRUE;
	//{{AFX_DATA_INIT(CGetWt)
	//}}AFX_DATA_INIT
    SwCount = 0;
    weight = 0;
    stCount = 0;

    m_Status = m_zero = FALSE;
    hcount = 0;

    if (!m_ok.LoadBitmaps("OKU", "OKD", "OKF"))
	{
		TRACE("Failed to load bitmaps for buttons\n");
		MessageBox("Failed to load bitmaps for buttons\n");
		AfxThrowResourceException();
	}
    if (!m_cancel.LoadBitmaps("CANCELU", "CANCELD", "CANCELF"))
	{
		TRACE("Failed to load bitmaps for buttons\n");
		MessageBox("Failed to load bitmaps for buttons\n");
		AfxThrowResourceException();
	}
	m_vehicle = "";
}

void CGetWt::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGetWt)
	DDX_Control(pDX, IDC_TRAFFIC, m_traffic);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Control(pDX, IDC_TEXTBOX1, m_wt);
	DDX_Text(pDX, IDC_EDIT1, m_vehicle);
	DDX_Control(pDX, IDC_MSFLEXGRID1, m_grid);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGetWt, CDialog)
	//{{AFX_MSG_MAP(CGetWt)
	ON_BN_CLICKED(IDC_GW, OnGw)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGetWt message handlers

BOOL CGetWt::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    m_gwedit.SubclassDlgItem(IDC_EDIT1,this);

    cwTime = atoi(profile.dat[Pro_CWTIME]);
    if(cwTime<100 || cwTime>2500) cwTime = 500;
    SetTimer(1,cwTime,NULL);

    HICON m_hIcon = AfxGetApp()->LoadIcon(IDI_SCALE);
	this->SetIcon(m_hIcon, TRUE);			// Set big icon
    SetGridTitle();
    ShowGridContent();
    lsetblank(df);
    GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_HIDE);

	m_vehicle.TrimRight();
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CGetWt::OnGw()
{
    if( m_Status == FALSE) {
        SetDlgItemText(IDC_EDIT2,"Unstable");
    }
    else {
        SetDlgItemText(IDC_EDIT2,"stable");
        GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_HIDE);
    }
    if( m_zero == FALSE) {
        SetDlgItemText(IDC_EDIT3,"No");
    }
    else {
        SetDlgItemText(IDC_EDIT3,"Yes");
    }

    long oweight = weight = GetCurrenctCommWeight(1);
	if (weight<=0){
		GwstPercent = GwstWeight = 0; // ccwong 20120305
	}
    if(GwstPercent>0) {
        GwstWeight = weight * GwstPercent / 100;
        GwstWeight = radix(GwstWeight);
    }

    if((weight>1000 && GwstWeight>0) || GwstWeight<0) {
        weight -= GwstWeight;
    }

	if(weight < 1000 || oweight < 1000) {
		weight = oweight;
	}

    char tmp[80];
    sprintf(tmp," %5.5ld",weight);
    m_wt.SetText( putnum0(tmp));

    return TRUE;
}

void CGetWt::OnOK() 
{
    GotoDlgCtrl(GetDlgItem(IDC_EDIT1));
    GetDlgItemText(IDC_EDIT1,m_vehicle);
    if(weight==0) {
        OnGw();
    }
    if(dBfldLogic(&profile,Pro_STABLE)) {
        if(m_Status == FALSE) {
            MessageBox("Current weight is unstable now\nplease wait","Error Message");
            return;
        }
    }
    if(dBfldLogic(&profile,Pro_ZERO)) {
        if(m_zero == FALSE) {
            MessageBox("weight no zero");
            return;
        }
    }
    KillTimer(1);
    CDialog::OnOK();
}

void CGetWt::OnCancel() 
{
	KillTimer(1);
	CDialog::OnCancel();
}

void CGetWt::OnTimer(UINT nIDEvent) 
{
    int flag = TRUE;

    if(m_WtFlag == TRUE) {
        m_WtFlag = FALSE;
        flag = OnGw();
        m_WtFlag = TRUE;
    }
	CDialog::OnTimer(nIDEvent);
    if(flag==FALSE) {
        OnCancel();
    }
    if(hcount>11) hcount = 0;
    m_Status = FALSE;

    long vw = atol(profile.dat[Pro_VW]);
    if( labs(weight - hwt[0]) <= vw) {
        SwCount++;
        if(SwCount>=2) {
            m_Status = TRUE;
        }
    }
    else {
        SwCount = 0;
    }
    if(weight==0) {
        m_zero = TRUE;
    }
    hwt[0] = weight;

    flag = TRUE;

    if(dBfldLogic(&profile,Pro_STABLE)) {
        if(m_Status == FALSE) {
            flag = FALSE;
        }
    }
    if(dBfldLogic(&profile,Pro_ZERO)) {
        if(m_zero == FALSE) {
            flag = FALSE;
        }
    }
    if(weight < 500) {
        flag = FALSE;
    }
    if(flag == TRUE) {
        HICON m_hIcon = AfxGetApp()->LoadIcon(IDI_TRAFFIC_GREEN);
        m_traffic.SetIcon(m_hIcon);
	}
    else {
        HICON m_hIcon = AfxGetApp()->LoadIcon(IDI_TRAFFIC_RED);
        m_traffic.SetIcon(m_hIcon);
    }
    if(weight < 500) return;

    stCount++;
    if(stCount == (1000 / cwTime * atol(profile.dat[Pro_STTIME])) ) {
        GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_SHOW);
    }
}

BEGIN_EVENTSINK_MAP(CGetWt, CDialog)
    //{{AFX_EVENTSINK_MAP(CGetWt)
	ON_EVENT(CGetWt, IDC_MSFLEXGRID1, -600 /* Click */, OnClickMsflexgrid1, VTS_NONE)
	ON_EVENT(CGetWt, IDC_MSFLEXGRID1, -601 /* DblClick */, OnDblClickMsflexgrid1, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CGetWt::OnClickMsflexgrid1() 
{
    long mrec,last = read_sumindex(0,df);
	int  row = m_grid.GetRow();

	if(row==0) return;
    int no = row;
    if(no<=last){
        mrec = ridx(no,df,0);
        SetDlgItemText(IDC_EDIT1,df->dat[LORRY]);
	}
	else {
		MessageBox("Out of range");
	}
    GotoDlgCtrl(GetDlgItem(CtrlId(1)));
}

void CGetWt::SetGridTitle()
{
    MGRID g(&m_grid,df,0,2);

    g.ratio = float(1.2);
    g.SetTitle("No",400,7);
    g.SetTitle("Vehicle No",900);
    g.SetTitle("Date",900,4);
    g.SetTitle("Time In",700,4);
    g.SetTitle("A/C Code",750);
    g.SetTitle("Product",750);
    g.SetTitle("Wt.In(kg)",750,7);
}

void CGetWt::SetActiveStatus(long rec)
{
    MGRID g(&m_grid,df,rec);

    g.SetText(df,0,LORRY,Date,TIMEIN,ACCODE,PT,FIRST,-1);
}

void CGetWt::ShowGridContent()
{
    long srec = read_sumindex(0,df);

    m_grid.SetRows(srec+1);
    for(long rec=1; rec<=srec; rec++) {
        readrec(rec,df);
        m_grid.SetRow(rec);
        SetActiveStatus(rec);
    }
    CString str;
    str.Format("%ld",srec);
    SetDlgItemText(IDC_TOTAL_VEHICLE_IN_YARD,str);
}

void CGetWt::OnDblClickMsflexgrid1() 
{
    OnClickMsflexgrid1();
    OnOK();
}

void CGetWt::OnButton1() 
{
    long wt = weight;
    CString str;
    str.Format("Please ask supervisor to confirm this weight :%ld.\r\nNeed Power ",
        wt);

    int flag = UserPasswordLoginAccepted(df, 9 ,str);
    GotoDlgCtrl(GetDlgItem(IDC_EDIT1));
    if(!flag) return;

    KillTimer(1);
    weight = wt;
    CDialog::OnOK();
}
