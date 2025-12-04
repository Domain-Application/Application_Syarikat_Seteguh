#include "stdafx.h"
#include "menu.h"
#include "Staff.h"
#include "AskPassword.h"
#include "admindb.h"
#include "Profile.h"
#include "Wb.h"
#include "Cmprofile.h"
#include "accode.h"
#include "Conso_Einv.h"

enum menu {M_RECEIVE=1,M_DESPATCH,M_CTRL,M_UTIL,M_OTHER,M_INV,M_SALARY,M_NORMAL,M_PUBLIC};
enum kkk1 {M_REPORT = M_RECEIVE * 10 +4 };
enum kkk2 {M_PREP   = M_DESPATCH * 10 +4 };
enum kkk3 {M_CTRL1 = M_CTRL * 10 +1 };
enum kkk4 {M_CTRL2 = M_CTRL * 10 +2 };
enum kkk5 {M_UPDATE   = M_RECEIVE * 10 +8 };
enum kkk6 {M_UP_DES   = M_DESPATCH * 10 +6 };
enum kkk7 {M_CTRL3 = M_CTRL * 10 +3 };
enum kkk8 {M_EXPINV   = M_INV * 10 +5 };


int STCR = 1;

void AdminDb_CMProfile(DB *df)
{
    if(df->ID==0) {
        AfxMessageBox("Database not open");
        return;
    }
    CCmprofile p1(IDD_CMPROFILE,df,df->f[1].idx & 0x01);
    p1.LptNo = 0; //preview
	p1.OneRecordOnly = TRUE;
    p1.DoModal();
}

void AdminTicketDb(DB *df)
{
    if(df->ID==0) {
        AfxMessageBox("Database not open");
        return;
    }
    CWb p1(df->ID,df,df->f[1].idx & 0x01);
//    p1.m_WinPrint = TRUE;
    p1.LptNo       = 1;
    p1.DoModal();
}

void AdminProfileDb(DB *df)
{
    if(df->ID==0) {
        AfxMessageBox("Database not open");
        return;
    }
    CProfile p1(df->ID,df,df->f[1].idx & 0x01);
    p1.OneRecordOnly = TRUE;
    p1.DoModal();
}

void AdminDb_EPF_RATE(DB *df)
{
    if(df->ID==0) {
        AfxMessageBox("Database not open");
        return;
    }
    CStaff p1(df->ID,df,df->f[1].idx & 0x01);
    p1.OneRecordOnly = TRUE;
    p1.DoModal();
}

int ProgramMenuNo,level;
int CMenuApp::SubProgram(int key)
{
    ProgramMenuNo = 0;
    switch(key) {
	case M_SALARY*10+1:
        AdminDbAdvance(&advance);
        break;
    case M_SALARY*10+2:
        AdminDbSalary(&salary);
        break;
	case M_SALARY*10+3:
        TableContainer("salctn");
        break;
    case M_RECEIVE*10+1:
        WeighingProcess(&receive,&first);
        break;
    case M_RECEIVE*10+2:
        AdminTicketDb(&receive);
        break;
    case M_RECEIVE*10+3:
        AdminTicketDb(&first);
        break;
    case M_RECEIVE*10+4:
        STCR = 1;
        TableContainer("rcvctn");
        break;
    case M_RECEIVE*10+5:
        AdminDbPayment(&payment);
        break;
    case M_RECEIVE*10+6:
        CheckPorlaLicenseExpiredDate();
        break;
    case M_RECEIVE*10+7:
        AdminTicketDb(&cashbill);
        break;
	case M_RECEIVE*10+9:
        PostingToCashBill();
        break;
	case M_UPDATE*10+1:   
		UpdateUnitPrice(&miller,&receive);
        break;
	case M_UPDATE*10+2:  
		Batch_Transaction(&receive);
        break;
	case M_UPDATE*10+3:   
		SetPercent(&receive);
        break;
	case M_UPDATE*10+4:
        SetSubsidy(&receive);
        break;
	case M_UPDATE*10+5:
		Purchase_Flag = TRUE;
        DoAutoInvoice(&receive,&apinv_wb,&apoinv_wb);
        break;
    case M_DESPATCH*10+1:
        WeighingProcess(&despatch,&fdes);
        break;
    case M_DESPATCH*10+2:
        AdminTicketDb(&despatch);
        break;
    case M_DESPATCH*10+3:
        AdminTicketDb(&fdes);
        break;
    case M_DESPATCH*10+4:
        STCR = 2;
        TableContainer("desctn");
		STCR = 1;
        break;
    case M_DESPATCH*10+5:
        STCR = 2;
        AdminDbPayment(&receipt);
		STCR = 1;
        break;
	case M_UP_DES*10+1:   
		UpdateUnitPrice(&up_des,&despatch);
        break;	
	case M_UP_DES*10+2:  
		Batch_Transaction(&despatch);
        break;
	case M_UP_DES*10+3:   
		SetPercent(&despatch);
        break;
	case M_UP_DES*10+4:
        SetSubsidy(&despatch);
        break;
	case M_UP_DES*10+5:
		Purchase_Flag = FALSE;
        DoAutoInvoice(&despatch,&arinv_wb,&aroinv_wb);
        break;
    case M_UTIL*10+2:
        AdminProfileDb(&profile);
        break;
    case M_UTIL*10+3:
		SetUser();
        break;
    case M_UTIL*10+4:
        return OnSystemUpgrade("WB.EXE", app->internetFolder);
        break;
    case M_UTIL*10+5:
        BackupProcess();
        break;
    case M_UTIL*10+6:
        CleanDatabase();
        break;
    case M_UTIL*10+7:
		{
			DB adoTmp(IDD_CMPROFILE);
			ado(&adoTmp, "profile_einv");
			AdminDb_CMProfile(&adoTmp);
			dclose(&adoTmp);
		}
        break;
	case M_CTRL1 * 10 +1:
		AdminDb(&prodtype);
		break;
	case M_CTRL1 * 10 +2:
		AdminDb(&particul);
		break;
	case M_CTRL1 * 10 +3:
		AdminDb(&lorry);
		break;
	case M_CTRL1 * 10 +4:
		AdminDb(&unit);
		break;
	case M_CTRL1 * 10 +5:
		AdminDb(&locate);
		break;
	case M_CTRL1 * 10 +6:
		AdminDb(&kod);
		break;
	case M_CTRL1 * 10 +7:
		AdminDb(&reason);
		break;
	case M_CTRL2 * 10 +1:
		// AdminDb(&accode);
        {
            CAccode h1(IDD_ACCODE, &accode, 1);
            h1.DoModal();
        }

        break;
	case M_CTRL2 * 10 +2:
		AdminDb(&driver);
		break;
	case M_CTRL2 * 10 +3:
		AdminDb(&term);
		break;
	case M_CTRL2 * 10 +4:
		AdminDb(&group);
		break;
	case M_CTRL2 * 10 +5:
		AdminDb(&agent);
		break;
	case M_CTRL2 * 10 +6:
		AdminDb(&position);
		break;
	case M_CTRL3 * 10 +1:
		AdminDb_EPF_RATE(&epf_rate);
		break;				
	case M_CTRL3 * 10 +2:
		AdminDb(&socso);
		break;	
    case M_CTRL * 10 + 4:
    {
        Conso_Einv();
        break;
    }
    case M_OTHER*10+1:
        AdminDbIreq(&ireq,&reqitem);
        break;
    case M_OTHER*10+2:
        AdminDbIreq(&ientry,&initem);
        break;
    case M_OTHER*10+3:
        TableContainer("othctn");
        break;
	case M_OTHER*10+4:
		StatementOfAccount();        
        break;
	case M_INV*10+1:
        PurchaseInvoiceWb();
        break;	
	case M_INV*10+2:
        SalesInvoiceWb();
        break;			
	case M_INV*10+3:        
        TableContainer("invctn");
        break;
	case M_EXPINV*10+1:
		ExportApinvTransactions();
        break;	
	case M_EXPINV*10+2:
		ExportArinvTransactions();
        break;	
	case M_NORMAL*10+1:
        WeighingProcess(&wb,&fwb);
        break;
    case M_NORMAL*10+2:
        AdminTicketDb(&wb);
        break;
    case M_NORMAL*10+3:
        AdminTicketDb(&fwb);
        break;
    case M_NORMAL*10+4:
        STCR = 1;
        TableContainer("wbctn");
        break;
	case M_PUBLIC*10+1:
        WeighingProcess(&pr,&fpr);
        break;
    case M_PUBLIC*10+2:
        AdminTicketDb(&pr);
        break;
    case M_PUBLIC*10+3:
        AdminTicketDb(&fpr);
        break;
    case M_PUBLIC*10+4:
        STCR = 1;
        TableContainer("prctn");
        break;
		
    default:
         return 0;
    }
    WeighingMode = 0;
    return 1;
}

