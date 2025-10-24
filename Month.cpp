// Month.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Month.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMonth dialog


CMonth::CMonth(UINT IDD,DB *pdf,int Idx ,void(*p)() ,CWnd* pParent /*=NULL*/)
    : CTio(IDD,pdf,Idx,p,pParent)
{
	//{{AFX_DATA_INIT(CMonth)
	m_flag = FALSE;
	//}}AFX_DATA_INIT
    bRangerSelect = FALSE;
}


void CMonth::DoDataExchange(CDataExchange* pDX)
{
    CTio::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMonth)
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	DDX_Check(pDX, IDC_CHECK1, m_flag);
	//}}AFX_DATA_MAP
}

void CMonth::OnSetWild()
{
    setwild(df);
    ridx(1,&accode,1);
    lset(df->dat[St_ACCODE1], accode.dat[1]);
    long last = read_sumindex(1,&accode);
    ridx(last,&accode,1);
    lset(df->dat[St_ACCODE2], accode.dat[1]);
}

void SetLogoMemo(int date,DB *scr)
{
    char str[256],tmp[0x2001],d1[12],d2[12],text[256];
    int  FFlag;
    strcpy(d1,mdate(scr->dat[1]));
    strcpy(d2,mdate(scr->dat[2]));

    qmove(logo.e->memo,0x200);
    sprintf(tmp,"Report Condition:\nDate period: %s %s to %s %s\n",d1,scr->dat[3],d2,scr->dat[4]);
    for(uint i=date; i<= scr->sumfield; i++) {
        if(!strcmp(scr->f[i].name,"PAGE")) continue;
        strcpy(str,DbFieldName(scr,i));
        strcat(str,":");
        FFlag = scr->FieldFlag[i];
        if(FFlag==0) {
            strcat(str,scr->dat[i]);
        }
        else if(FFlag==1) {
            sprintf(text,"range in (from %s to %s)",scr->from[i],scr->to[i]);
            strcat(str,text);
        }
        else if(FFlag==2) {
            sprintf(text,"range out (from %s to %s)",scr->from[i],scr->to[i]);
            strcat(str,text);
        }
        strcat(str,"\n");
        strcat(tmp,str);
    }

    qwrt(logo.e->memo,tmp,strlen(tmp));
	char ch=26;
	qwrt(logo.e->memo,(char *)&ch,1);
	qwrt(logo.e->memo,(char *)&ch,1);
}

void CMonth::show_tio()
{
    long rec;

    TmpTio[0] = 0;
    app->SetStdCompanyLogo();
    lset(logo.dat[Logo_DATE1],tscr.dat[1]);
    lset(logo.dat[Logo_DATE2],tscr.dat[2]);
    lset(logo.dat[Logo_SELECT1],TmpTio);
    lset(logo.dat[Logo_RTITLE],SubTitle);
    lset(logo.dat[Logo_USER],user.dat[1]);
    lset(logo.dat[Logo_USERNAME],user.dat[2]);
    writerec(1,&logo);
}


BEGIN_MESSAGE_MAP(CMonth, CTio)
	//{{AFX_MSG_MAP(CMonth)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

extern void get_master();
/////////////////////////////////////////////////////////////////////////////
// CMonth message handlers

