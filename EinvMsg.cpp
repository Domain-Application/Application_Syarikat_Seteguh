// EinvMsg.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "EinvMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEinvMsg dialog


CEinvMsg::CEinvMsg(CWnd* pParent /*=NULL*/)
	: CDialog(CEinvMsg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEinvMsg)
	m_DATE = COleDateTime::GetCurrentTime();
	m_DOCMSG = _T("");
	m_MSG = _T("");
	m_REFNO = _T("");
	//}}AFX_DATA_INIT
}


void CEinvMsg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEinvMsg)
	DDX_Text(pDX, IDC_DATE, m_DATE);
	DDX_Text(pDX, IDC_DOC_MSG, m_DOCMSG);
	DDX_Text(pDX, IDC_MSG, m_MSG);
	DDX_Text(pDX, IDC_REFNO, m_REFNO);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEinvMsg, CDialog)
	//{{AFX_MSG_MAP(CEinvMsg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEinvMsg message handlers
