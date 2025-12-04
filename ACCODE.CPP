//
// Accode.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "mgrid.h"
#include <afxctl.h>
#include "picture.h"
#include "accode.h"
#include "einv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Accode

void ReplaceAccodeGrid(long rec,CMSFlexGrid *grid,DB *df,int IdxItem)
{
    MGRID g(grid,df,rec);
    g.SetText(df,0,1,2,CU_TEL,CU_FAX,-1);
}

void SetAccodeGridTitle(CMSFlexGrid *grid,DB *df,int IdxItem)
{
    MGRID g(grid,df,0,5);
    g.SetTitle(0);

    g.SetTitle("Code ID",1350);
    g.SetTitle("Name",3500);
    g.SetTitle("Tel. No",1250,7);
    g.SetTitle("Fax. No",1250,7);

    grid->SetCol(3);
    CPictureHolder pict;
    pict.CreateFromIcon(IDI_TEL);
    CPicture picture(pict.GetPictureDispatch());
    grid->SetRefCellPicture(picture);
    grid->SetRowHeight(0, 550);

    grid->SetCol(4);
    pict.CreateFromIcon(IDI_FAX);
    CPicture picture2(pict.GetPictureDispatch());
    grid->SetRefCellPicture(picture2);
    grid->SetRowHeight(0, 550);
}

int tell_accode(UINT nChar,int &id,int &fld,DB *df,int sq)
{
    CString str;
	
    switch(fld){
    case CU_SNAME:
        GetShortName(df,fld);
		return 2;
        break;
	}
	return 1;
}

CAccode::CAccode(UINT IDD,DB *pdf,int Idx,CWnd* pParent /*=NULL*/)
    : CStaff(IDD,pdf,Idx,pParent)
{
    df->m_CannotAdd = TRUE;
}


void CAccode::DoDataExchange(CDataExchange* pDX)
{
    CStaff::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAccode, CStaff)
    ON_BN_CLICKED(IDC_BUTTON1, OnEinv)
END_MESSAGE_MAP()

void AccodeEntry()
{     
    CAccode p1(IDD_ACCODE,&accode,1);
    p1.DoModal();
}

int Accode_AdminDb(DB *df,int flag)
{
    AccodeEntry();
    return 1;
}


void init_accode(DB *df)
{
    df->e->rep       = ReplaceAccodeGrid;
    df->e->GridTitle = SetAccodeGridTitle;
	df->e->tell		 = tell_accode;
    df->e->AdminDb   = Accode_AdminDb;
}

int tell_einv_view(UINT nChar,int &id,int &fld,DB *df,int sq)
{
    switch(fld) {
    case 10:
        if(sq)
        {
            qd(df->dat[GetDbFieldNo(df, "POSTCODE")], &postcode);
            lset(df->dat[GetDbFieldNo(df, "MALAYSIA_STATE")], postcode.dat[GetDbFieldNo(&postcode, "MALAYSIA_STATE")]);
            lset(df->dat[GetDbFieldNo(df, "MALAYSIA_CITY")], postcode.dat[GetDbFieldNo(&postcode, "CITY_NAME")]);
        }
        break;
	default:
		break;
    }
    return 2;
}

void init_einv_view(DB *df)
{
	df->e->tell = tell_einv_view;
}

void CAccode::OnEinv() 
{
    if(SaveFlag == NEWFILE){
        long nrec = getdelrec(df, FALSE);
        save_entry(NONEXIST,IdxItem,nrec,df);
		SaveFlag = OLDFILE;
		ShowInformation();
    }
	if( SaveFlag == CANNOTSAVE) return;
    CString strSQL,dbName;
	dbName.Format("%s_einv", df->name);
    DB adoTmp;
    strSQL.Format("select * from %s where %s = '%s'", dbName, df->fieldName[1], df->dat[1]); //Note: create a view where contains code, field from TIN UNTIL EINV_EMAIL
    ado(&adoTmp, dbName, init_einv_view, strSQL);
    long rec = read_sumindex(0, &adoTmp);
    if(rec <= 0)
    {
		dclose(&adoTmp);
        return;
    }
    readrec(1, &adoTmp);
    CEinv p;
    p.df = &adoTmp;
    p.isFFBProgram = TRUE;
    UINT ID = p.DoModal();
    if(ID == IDOK)
    {
        fetch(df, &adoTmp);
    }
    dclose(&adoTmp);
	ShowInformation();
}