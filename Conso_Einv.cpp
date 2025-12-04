// Conso_Einv.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Conso_Einv.h"
#include "EinvMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConso_Einv dialog


CConso_Einv::CConso_Einv(UINT IDD,DB *pdf,int Idx,CWnd* pParent /*=NULL*/)
: CParent(IDD,pdf,Idx,pParent)
{
	//{{AFX_DATA_INIT(CConso_Einv)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CConso_Einv::DoDataExchange(CDataExchange* pDX)
{
	CParent::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConso_Einv)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConso_Einv, CParent)
	//{{AFX_MSG_MAP(CConso_Einv)
	ON_BN_CLICKED(IDC_LOCK, OnLock)
	ON_BN_CLICKED(IDC_LOCK2, OnUnLock)
	ON_BN_CLICKED(IDC_BUTTON1, OnImportData)
	ON_BN_CLICKED(IDC_QRCODE, OnGetQRCODE)
	ON_BN_CLICKED(IDC_BUTTONEINVOICE, OnSubmitEinv)
	ON_BN_CLICKED(IDC_REFRESHMSG, OnRefreshmsg)
	ON_BN_CLICKED(IDC_BUTTON2, OnImportRangeInv)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConso_Einv message handlers

BOOL CConso_Einv::OnInitDialog()
{
    CString checkstr;

    CParent::OnInitDialog();

	checkstr.Format("SELECT count(*) FROM sys.objects WHERE  object_id = OBJECT_ID(N'[dbo].[dt_ConsoSumImport]') AND type IN ( N'FN', N'IF', N'TF', N'FS', N'FT',N'P' )");
	if (SqlValue(checkstr)==0)
	{
		GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_HIDE);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



void CConso_Einv::ShowInformation()
{
	CString cmdstr,apistr;

    cmdstr.Format("SELECT SUM(ISNULL(AMT,0)+ISNULL(GST,0)) FROM CONSO_EINV_SUB WHERE REFNO='%s'",df->dat[1]);

	SetDlgItemText(IDC_NETAMT,SqlText(cmdstr));

	apistr.Format("SELECT TOP 1 ISNULL(DOMAIN_REMARK,'NEW') FROM EINV_API WHERE REFNO='%s' ORDER BY PKKEY DESC ",df->dat[1]);
	SetDlgItemText(IDC_EINVSTATUS,SqlText(apistr)); 

    CParent::ShowInformation();

}

void CConso_Einv::ShowInitFrame()
{
    ShowInitFrame1();
}

void CConso_Einv::ShowInitFrame1()
{
    fg.Initial(&m_grid,sub,12);

	fg.SetTitle("No",500,1);
	fg.SetTitle("Invoice",1800,1);
    fg.SetTitle("Amt Before Tax",1400,7);
	fg.SetTitle("Non Tax Amount",1400,7);
	fg.SetTitle("Taxable Amount",1400,7);
    fg.SetTitle("Taxcode",800,1);
    fg.SetTitle("T%",500,7);
	fg.SetTitle("Tax Amt",800,7);
	fg.SetTitle("Exempt.Amt",1200,7);
	fg.SetTitle("Branch",900);
	fg.SetTitle("Path",800);
	fg.SetTitle("Net Amt",1300,7);
	

}

void CConso_Einv::ReplaceSubGrid(long rec,long irec,CMSFlexGrid *m_grid,DB *sub,int IdxItem)
{
	CString cmdstr;
	double net_total=0;
	char txt_total[1024];

	fg.show(rec);
    fg.SetText(sub,0,CEINVS_INVOICE,CEINVS_AMT,CEINVS_NONTAX_AMT,CEINVS_TAXABLE_AMT,CEINVS_TAXCODE,
		CEINVS_PERCEN,CEINVS_GST,CEINVS_EXEMPT_AMT,CEINVS_PCCODE,CEINVS_PATH,-1);

	net_total=sub->val(CEINVS_AMT)+sub->val(CEINVS_GST);
	dtoa(net_total,txt_total,12,2);
	fg.SetText(txt_total);

}



int start_ceinv(DB *df)
{
    dbasedate(one.dat[1]);
	plusmon(one.dat[1], -1);
    lset(two.dat[1],one.dat[1]);
	lset(two.dat[2],one.dat[1]);
	firstday(two.dat[1]);
	lastday(two.dat[2]);
	

	dbasedate(df->dat[CEINV_DATE]);
	lset(df->dat[CEINV_CLASS],"004");
	lset(df->dat[CEINV_D1],two.dat[1]);
	lset(df->dat[CEINV_D2],two.dat[2]);
	lset(df->dat[CEINV_CURRENCY],"MYR");
	rset(df->dat[CEINV_RATE],"1.00");

	
    return 1;
}

int tell_ceinv (UINT nChar,int &id,int &fld,DB *df,int sq)
{
	
    switch(fld) {
    case CEINV_DATE:
		if (sq){

	
		}
		break;
    }
	return 2;
}



void init_ceinv(DB *df)
{
	df->e->start     = start_ceinv;
	df->e->tell      = tell_ceinv;

}



void Conso_Einv()
{


	CConso_Einv p1(IDD_CONSO_EINV,&conso_einv,1);
	p1.sub		 = &conso_einv_sub;	
    p1.IDD_SUB   = IDD_CONSO_EINV_SUB;
    p1.DoModal();

 
}




void CConso_Einv::OnLock() 
{
	// TODO: Add your control notification handler code here
	
}

void CConso_Einv::OnUnLock() 
{
	if(UserLevel()<9)
	{
		AfxMessageBox("At least Level 9 user!");
		return;
	}
    
	CString cmdstr;
	int	fld= GetDbFieldNo(df,"REC_STATUS");
	
	if (df->dat[fld][0]=='1')
	{
		cmdstr.Format("UPDATE [%s] set REC_STATUS=NULL WHERE [%s]='%s'",
			df->name,df->fieldName[1],df->dat[1]);
		df->RunSql(cmdstr);
		
        AfxMessageBox("Unlocked!");
	}
    else
	{
		cmdstr.Format("UPDATE [%s] set REC_STATUS=1 WHERE [%s]='%s'",
			df->name,df->fieldName[1],df->dat[1]);
		df->RunSql(cmdstr);
		
		AfxMessageBox("Locked!");
	}
	
	
	
	cmdstr.Format("select * from [%s] where [%s]='%s'",
		df->name,df->fieldName[1], df->dat[1]);
    df->ReOpen(cmdstr,FALSE);
	df->read(1);
    Status = 0;	
}

void CConso_Einv::OnImportData() 
{
    if(SaveMainInformation()==FALSE) return;

	UpdateData();
	
	DB tmpdb;
    CString str,str1,str2,cmdstr,checkstr;
	


	checkstr.Format("SELECT COUNT(*) FROM CONSO_EINV_SUB WHERE REFNO='%s'",df->dat[1]);
	
	if (SqlValue(checkstr)>100)
	{
		AfxMessageBox("Sorry over 100 invoices already!");
		return;
	}


	
	if(AfxMessageBox("Confirm to import invoices?",MB_ICONQUESTION | MB_YESNO)==IDYES) 
	{
		
		if (!isblank(df->dat[CEINV_PCCODE]))
		{
		cmdstr.Format("SELECT REFNO,[DATE],PCCODE,AMT,NON_TAX_AMT,TAXABLE_AMT,GST,EXEMPT_AMT,TAXCODE,PERCEN,[PATH] FROM VIEW_CONSO_EINV_LIST where [DATE] between '%s' and '%s' and ISNULL(PCCODE,'')=ISNULL('%s','') and EINV_TYPE='%s' ",
		           df->dat[CEINV_D1],df->dat[CEINV_D2],df->dat[CEINV_PCCODE],df->dat[CEINV_TYPE]);
        }
		else
		{
			cmdstr.Format("SELECT REFNO,[DATE],PCCODE,AMT,NON_TAX_AMT,TAXABLE_AMT,GST,EXEMPT_AMT,TAXCODE,PERCEN,[PATH] FROM VIEW_CONSO_EINV_LIST where [DATE] between '%s' and '%s' and EINV_TYPE='%s' ",
				df->dat[CEINV_D1],df->dat[CEINV_D2],df->dat[CEINV_TYPE]);
        }			

		ado(&tmpdb,"",NULL,cmdstr);
		
		
		long last = tmpdb.GetRecordCount();

		if (last>100)
			last=100;
		

		if(last==0){
			AfxMessageBox("No Data!");
            dclose(&tmpdb);
			return;
		}

		for (int rec = 1;rec <= last;rec++)
		{
			readrec(rec,&tmpdb);
			lsetblank(sub);
		
		lset(sub->dat[1],df->dat[1]);	
		lset(sub->dat[CEINVS_INVOICE],tmpdb.dat[1]);
		lset(sub->dat[CEINVS_PCCODE],tmpdb.dat[3]);
		rset(sub->dat[CEINVS_AMT],tmpdb.dat[4]);
		rset(sub->dat[CEINVS_NONTAX_AMT],tmpdb.dat[5]);
		rset(sub->dat[CEINVS_TAXABLE_AMT],tmpdb.dat[6]);
		rset(sub->dat[CEINVS_GST],tmpdb.dat[7]);
		rset(sub->dat[CEINVS_EXEMPT_AMT],tmpdb.dat[8]);
		lset(sub->dat[CEINVS_TAXCODE],tmpdb.dat[9]);
		rset(sub->dat[CEINVS_PERCEN],tmpdb.dat[10]);
		lset(sub->dat[CEINVS_PATH],tmpdb.dat[11]);
	
		
		sub->AddNew();
		}
        dclose(&tmpdb);
		
		ReFreshSubFile();
		ShowInformation();
		
		AfxMessageBox("Done!");
	}
	else
		AfxMessageBox("Abort!");
}

void CConso_Einv::OnGetQRCODE() 
{
	CString cmdstr;
	char szJpg[1024], szPath[1024];
	
	
	UpdateData();
	
	char pathbuffer[_MAX_PATH], curdir[_MAX_PATH];
	char *lpFilePart;
	CString tpath = "";
	CString str, strpath,hpstr;

	if (isblank(df->dat[CEINV_URL]))
	{
	hpstr.Format("SELECT TOP 1 IRB_URL FROM [EINV_API] WHERE REFNO='%s' and CANCEL_USER IS NULL ",
		df->dat[1]);
	}
	else
	{	
	hpstr.Format("SELECT TOP 1 EINV_URL FROM [%s] WHERE REFNO='%s'",
		df->name,df->dat[1]);
	}
	
	UINT flag = GetCurrentDirectory(1000, szJpg);
	UINT xflag = GetCurrentDirectory(1000, szPath);
	
	cmdstr.Format("\\\QRCODE\\\QR.exe ");
	strpath.Format("""%s""", SqlText(hpstr));
	
	//strpath.Replace("[",""""");
	
	strcat(szJpg, cmdstr + strpath);
	strcat(szPath, cmdstr);
	
	
	
	BOOL iflag = IsFileExisted(szPath);
	if (iflag) {
		WinExec(szJpg, SW_SHOW);
	}
	
}

void CConso_Einv::OnSubmitEinv() 
{
 CString cmdstr,str,str2,checkstr;	



	
    cmdstr.Format("SELECT count(*) from EINV_API where refno='%s' and CANCEL_USER IS NULL and REJECT_USER IS NULL",df->dat[1]);
	
	if (SqlValue(cmdstr)>0)
	{
        if ( AfxMessageBox("You woule like to CANCEL this E-Invoice? Please be patience submit may take 1 to 2 mins!",MB_ICONQUESTION | MB_YESNO) == IDYES ) 
		{
			
			cmdstr.Format("EXEC DBO.dt_export_einv_api '%s','%s','CANCEL','%s' ",
				df->dat[1],df->name,user.dat[1]);
			//RunSql(cmdstr);
		}
		
		
	}
    else
	{
		if ( AfxMessageBox("You woule like to submit this NEW E-Invoice? Please be patience submit may take 1 to 2 mins!",MB_ICONQUESTION | MB_YESNO) == IDYES ) 
		{
			cmdstr.Format("EXEC DBO.dt_export_einv_api '%s','%s','NEW','%s' ",
				df->dat[1],df->name,user.dat[1]);
			RunSql(cmdstr);
		}
		
		
	}


      cmdstr.Format("EXEC DBO.dt_RunEinvApi '%s','1'",
					df->dat[1]);
				RunSql(cmdstr);



	ReFreshSubFile();
	ShowInformation();

	
	AfxMessageBox("Done!");	

	
}

void CConso_Einv::OnRefreshmsg() 
{
    CString cmdstr;
	

	cmdstr.Format("EXEC DBO.dt_RunEinvApi '%s','2'",
		df->dat[1]);
				RunSql(cmdstr);
	
	cmdstr.Format("SELECT count(*) from EINV_API where refno='%s' and CANCEL_USER IS NULL and REJECT_USER IS NULL and (ISNULL(API_FLAG,0)=0 OR ISNULL(DOCUMENT_FLAG,0)=0) and ISNULL(DOMAIN_REMARK,'')<>'SUBMITTED' and ISNULL(DOMAIN_REMARK,'')<>'VALID' ",df->dat[1]);
	
	if (SqlValue(cmdstr)>0)
	{
		if ( AfxMessageBox("Do you want to reset data for submission! *Only Invalid Status*",MB_ICONQUESTION | MB_YESNO) == IDYES ) 
		{
			cmdstr.Format("DELETE FROM EINV_API WHERE REFNO='%s' and CANCEL_USER IS NULL and REJECT_USER IS NULL and (ISNULL(API_FLAG,0)=1 and ISNULL(DOCUMENT_FLAG,0)=0)   ",
				df->dat[1]);
			RunSql(cmdstr);
			
			cmdstr.Format("EXEC DBO.dt_export_einv_api '%s','%s','NEW','%s' ",
				df->dat[1],df->name,user.dat[1]);
			RunSql(cmdstr);
			
			ShowInformation();
			
		}
		
		if ( AfxMessageBox("You would like to try to submit this E-Invoice? Please be patience submit may take 1 to 2 mins!",MB_ICONQUESTION | MB_YESNO) == IDYES ) 
		{
			
            cmdstr.Format("UPDATE EINV_API SET API_DATE=NULL WHERE refno='%s' and CANCEL_USER IS NULL and REJECT_USER IS NULL and (ISNULL(API_FLAG,0)=0 OR ISNULL(DOCUMENT_ID,'')='') ",
				df->dat[1]);
			RunSql(cmdstr);
			
			cmdstr.Format("EXEC DBO.dt_RunEinvApi '%s','1'",
				df->dat[1]);
			RunSql(cmdstr);
			
			AfxMessageBox("Done!");	
			
		}
		else
		{
			
			if ( AfxMessageBox("Do you want to delete submission data of this invoice in Domain API?",MB_ICONQUESTION | MB_YESNO) == IDYES ) 
			{
				
				cmdstr.Format("DELETE FROM EINV_API WHERE REFNO='%s' and CANCEL_USER IS NULL and REJECT_USER IS NULL and (ISNULL(API_FLAG,0)=0 OR ISNULL(DOCUMENT_ID,'')='')   ",
					df->dat[1]);
				RunSql(cmdstr);
				
				
				AfxMessageBox("Deleted!");	
				
			}
			
		}
        
	}
	
	
	cmdstr.Format("EXEC DBO.dt_RunEinvApi '%s','2'",
		df->dat[1]);
				RunSql(cmdstr);
				
				
				ReFreshSubFile();
				ShowInformation();
				
				CString strSql;
				DB tmpdb(IDD_BROWSE,5);
				
				if ( AfxMessageBox("Do you want to view this invoice API Message?",MB_ICONQUESTION | MB_YESNO) == IDYES ) 
				{
					
					strSql.Format("select REFNO,API_DATE,API_FLAG,MSG,DOCUMENT_MSG from EINV_API where refno='%s' ",
						df->dat[1]);
					
					ado(&tmpdb, "", NULL, strSql);
					
					//long xrec = viewidx(&tmpdb);
					long xrec = tmpdb.GetRecordCount();
					
					
					if(xrec==0)
					{
						dclose(&tmpdb);
						return;
					}
					
					
					readrec(1,&tmpdb);
					
					CEinvMsg p1;
					
					p1.m_REFNO= tmpdb.dat[1];
					//p1.m_DATE = tmpdb.dat[2];
					p1.m_MSG  = tmpdb.dat[4];
					p1.m_DOCMSG= tmpdb.dat[5];
					
					p1.DoModal();
					
					
					
					dclose(&tmpdb);
				}	
}

void CConso_Einv::OnImportRangeInv() 
{
    if(SaveMainInformation()==FALSE) return;
	
	UpdateData();
	
	DB tmpdb;
    CString str,str1,str2,cmdstr,checkstr;
	
	
	
	checkstr.Format("SELECT COUNT(*) FROM CONSO_EINV_SUB WHERE REFNO='%s'",df->dat[1]);
	
	if (SqlValue(checkstr)>100)
	{
		AfxMessageBox("Sorry over 100 invoices already!");
		return;
	}
	

	
	
	if(AfxMessageBox("Confirm to import invoices by range type?",MB_ICONQUESTION | MB_YESNO)==IDYES) 
	{
		cmdstr.Format("EXEC dbo.dt_ConsoSumImport '%s','%s','%s','%s','%s' ",
			df->dat[1],df->dat[CEINV_PCCODE],df->dat[CEINV_TYPE],df->dat[CEINV_D1],df->dat[CEINV_D2]);

		df->RunSql(cmdstr);

		ReFreshSubFile();
		ShowInformation();
		
		AfxMessageBox("Done!");
		
	}
	else
		AfxMessageBox("Abort!");


	
}
