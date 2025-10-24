// GwEdit.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "comm.h"
#include "GwEdit.h"
#include "GetWt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGwEdit

CGwEdit::CGwEdit()
{
    flag = 0;
    OldChar = 0;
}

CGwEdit::~CGwEdit()
{
}


BEGIN_MESSAGE_MAP(CGwEdit, CEdit)
	//{{AFX_MSG_MAP(CGwEdit)
	ON_WM_GETDLGCODE()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGwEdit message handlers

UINT CGwEdit::OnGetDlgCode() 
{
	return CEdit::OnGetDlgCode() | DLGC_WANTALLKEYS;
}

void CGwEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CGwEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
 	if(nChar>=1 && nChar<=10) {
		if (nChar!=8 || nFlags==35){
			GwstPercent = atol(gst.dat[nChar]);           
		}		GwstWeight = 0;
    }
    else if(nChar>=11 && nChar<=26) {
        if (nChar!=13 || nFlags==50){
			GwstWeight = atol(gst.dat[nChar]);      
			GwstPercent = 0; 
		}
		
    }
    else if(nChar==32 && OldChar==32) {
        GwstPercent = GwstWeight = 0;
    }

    OldChar = nChar;
	if(nChar==32 || nChar==22) return;
	
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CGwEdit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    CGetWt *p;

    p = (CGetWt *) GetParent();
    if(nChar==VK_RETURN && flag==1) {
         p->OnOK();
	}
    else if(nChar==VK_ESCAPE) {
        p->OnCancel();
	}
	CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
    flag = 1;
}
