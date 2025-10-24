// ShortFigure.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "ShortFigure.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShortFigure dialog


CShortFigure::CShortFigure(UINT IDD,CWnd* pParent /*=NULL*/)
	: CDialog(IDD, pParent)
{
	//{{AFX_DATA_INIT(CShortFigure)
	m_value = 0.0;
	//}}AFX_DATA_INIT
}


void CShortFigure::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShortFigure)
	DDX_Text(pDX, IDC_EDIT1, m_value);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CShortFigure, CDialog)
	//{{AFX_MSG_MAP(CShortFigure)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShortFigure message handlers
