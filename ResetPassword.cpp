// ResetPassword.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "ResetPassword.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResetPassword dialog


CResetPassword::CResetPassword(CWnd* pParent /*=NULL*/)
	: CDialog(CResetPassword::IDD, pParent)
{
	//{{AFX_DATA_INIT(CResetPassword)
	m_password = _T("");
	//}}AFX_DATA_INIT
}

void CResetPassword::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CResetPassword)
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	DDX_Text(pDX, IDC_EDIT1, m_password);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CResetPassword, CDialog)
	//{{AFX_MSG_MAP(CResetPassword)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResetPassword message handlers

void CResetPassword::RunSql(CString str)
{
    m_progress.StepIt();
    ::RunSql(str);
}

void CResetPassword::OnOK() 
{
    UpdateData();


	CString str;
    str.Format("FFB-%s",today);
    if(m_password.Compare(str)) {
        MessageBox("Incorret password");
        return;
    }

    m_progress.SetRange(1,20);

	str = "delete from [Fwb]";
	RunSql(str);

    str = "delete from [wb]";
	RunSql(str);

    str = "delete from [pr]";
	RunSql(str);

    str = "delete from [fpr]";
	RunSql(str);

	str = "delete from [RECEIVE]";
	RunSql(str);

    str = "delete from [first]";
	RunSql(str);

    str = "delete from [FDES]";
	RunSql(str);

    str = "delete from [DESPATCH]";
	RunSql(str);

	str = "delete from [cashbill]";
	RunSql(str);

	str = "delete from [reqitem]";
	RunSql(str);
	
    str = "delete from [ireq]";
	RunSql(str);

	str = "delete from [initem]";
	RunSql(str);
	
    str = "delete from [ientry]";
	RunSql(str);

	str = "delete from [aroinv_wb]";
	RunSql(str);
	
    str = "delete from [arinv_wb]";
	RunSql(str);
	
	str = "delete from [apoinv_wb]";
	RunSql(str);
	
    str = "delete from [apinv_wb]";
	RunSql(str);

	str = "delete from [receipt]";
	RunSql(str);

	str = "delete from [payment]";
	RunSql(str);

	str = "delete from [miller]";
	RunSql(str);

	str = "delete from [PARTICUL]";
	RunSql(str);

	str = "delete from [SALARY]";
	RunSql(str);

	str = "delete from [advance]";
	RunSql(str);
	
	str = "delete from [up_des]";
	RunSql(str);
/*
    str = "DELETE FROM [pt]";
	RunSql(str);
*/

	str = "DELETE FROM [ACCODE]";
	RunSql(str);
	
    str = "DELETE FROM [driver]";
	RunSql(str);

    str = "DELETE FROM [transp]";
	RunSql(str);

    str = "DELETE FROM [lorry]";
	RunSql(str);

	str = "DELETE FROM [locate]";
	RunSql(str);

	str = "DELETE FROM [group]";
	RunSql(str);

	str = "DELETE FROM [agent]";
	RunSql(str);

    str = "truncate TABLE [login]";
	RunSql(str);

	str = "TRUNCATE TABLE [logsqlcmd]";
	RunSql(str);

	str = "truncate TABLE [logerr]";
	RunSql(str);
	
    str = "update [ticketno] set [next no] = 1";
	RunSql(str);


    MessageBox("Completed");

	CDialog::OnOK();
}

void CleanDatabase()
{
    CResetPassword h1;

    h1.DoModal();
}

void CResetPassword::OnButton1() 
{
    UpdateData();

	CString str;

    m_progress.SetRange(1,20);

    str = "delete from [rptlog]";
	RunSql(str);

    MessageBox("Completed");

	CDialog::OnOK();
	
}
