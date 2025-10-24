// menu.h : main header file for the MENU application
//

#if !defined(AFX_MENU_H__D6EA9606_A1F2_11D0_8F37_945AC87D3839__INCLUDED_)
#define AFX_MENU_H__D6EA9606_A1F2_11D0_8F37_945AC87D3839__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "myapp.h"       // main symbols
#include <Tan.h>       // main symbols
//{{AFX_INCLUDES()
#include "msflexgrid.h"
//}}AFX_INCLUDES

extern BOOL Purchase_Flag;
extern DB	arinv,apinv,aroinv,apoinv,accpf;
extern DB  socso;
extern DB  epf_rate,stkbal;
extern DB  aroinv_wb;
extern DB  arinv_wb;
extern DB  apoinv_wb;
extern DB  apinv_wb;
extern DB  miller,up_des;
extern DB  tt;
extern DB  term;
extern DB  unit;
extern DB  lorry;
extern DB  particul;
extern DB  group;
extern DB  kod;
extern DB  locate;
extern DB  agent;
extern DB  cashbill;
extern DB  despatch;
extern DB  fdes;
extern DB  initem;
extern DB  ientry;
extern DB  driver;
extern DB  position;
extern DB  reqitem;
extern DB  ireq;
extern DB  reason;
extern DB  glgroup;
extern DB  glcode;
extern DB  salary;
extern DB  advance;
extern DB  payment;
extern DB  receipt;
extern DB  receive;
extern DB  accode;
extern DB  fwb;

extern int STCR;
extern void init_salary(DB *df);
extern void DoAutoInvoice(DB *df,DB *invmain,DB *invsub);
extern void AdminDbSalary(DB *df);
extern void AdminDbAdvance(DB *df);
extern void ExportApinvTransactions();
extern void ExportArinvTransactions();
extern void PurchaseInvoiceWb();
extern void SalesInvoiceWb();
extern void init_arinv_wb(DB *df);
extern void init_aroinv_wb(DB *df);
extern void Batch_Transaction(DB *df);
extern void SetSubsidy(DB *df);
extern void SetPercent(DB *df);
extern int UpdateUnitPrice(DB *df,DB *main);
extern void init_miller(DB *df);
extern void PostingToCashBill();
extern void init_payment(DB *df);
extern void SetDefaultAc(DB *df);
extern void SetVehicleDefault(int i,DB *df);
extern void init_ientry(DB *df);
extern void init_ireq(DB *df);
extern void init_reqitem(DB *df);
extern void StatementOfAccount();
extern void AdminDbPayment(DB *df);
extern BOOL bComOutput;
extern int UserPasswordLoginAccepted(DB *df,int level,CString str);
extern void MasterGrossTareNetReport(DB *df);
extern long GreenLight,LastLight,WeighingMode;
extern void AdminDbProfile(DB *df);
extern void CheckPorlaLicenseExpiredDate();
extern DB *ttt,Status,stype,wbsetup;
extern void CleanDatabase();
extern void ProfileData(DB *df);
extern int   PViewFlag;
extern void init_stdlist(DB *df);
extern void SetWBGridRows(CMSFlexGrid *grid,DB *df);
extern void PrintLoadingUnloadingForm(long irec,FILE *ptr,DB *df);
extern void DetailGrossTareNetListingReport(DB *df);


extern void init_accode(DB *df);
extern void init_first(DB *df);
extern void init_wb(DB *df);
extern void pslip1(long irec,FILE *ptr,DB *df);
extern int tscr_flag(DB *df);
extern void SetStdCompanyLogo();
extern long GwstWeight,ActualWeight;
extern void WeighingProcess(DB *main,DB *first);
extern void MasterReport(DB *df);
extern void PrintProductCode(long irec,FILE *ptr,DB *df);
extern void PrintDriverCode(long irec,FILE *ptr,DB *df);
extern double m[15];
extern void compute_weight(DB *df);
extern void SalesContractUpdateReport();
extern int repct(char *Menu,int IdxItem,long irec,DB *df);
extern void init_gst(DB *df);
extern void Psmf(long irec,FILE *ptr,DB *df);
extern void DateSummary(DB *df);
extern void VehicleSummary(DB *df);
extern void Pcode(long irec,FILE *ptr,DB *df);
extern int keytscr(void);
extern void ProductSummaryReport(DB *df);
extern void PorlaReport(void);
extern void BuyerTransportFeeSummary(DB *df);
extern void ProfileSetup(void);
extern void DetailListingReport(DB *df);
extern void init_product(DB *df);
extern void init_driver(DB *df);
extern void init_accode(DB *df);
extern void Weighting(DB *wb,DB *first);
extern void DispPublicWeight(DB*);
extern long input_weight(KB key,char *mesure);
extern void Pcust(long ,FILE *,DB *);
extern char *dsavemenu,*company;
extern void prec(long irec,FILE *ptr,DB *df);
extern long weight1,weight2;
extern void DespatchWeighting(void);
extern void DispFirst(DB *);
extern void Posting(void);
extern void PrintLogo(FILE *ptr,char *tl);
extern void WBmodify(DB *);
extern void tscr_report(DB *);
extern void AdminDbIreq(DB *df,DB *sub);

extern DB   accode,first,prodtype,tscr,driver,gst,logo;
extern DB   bal,wb,one,user,login,report,two;
extern DB   pr,fpr,profile,porla,transp;
extern DB sto,strun_no,state,Locate;

enum receive {TICKET=1,LORRY,Date,ACCODE,AGENT,COMM0,ALLOW0,PORLA_AC,MILL,LOCATE,
           DRIVER1,COMM1,ALLOW1,DRIVER2,COMM2,ALLOW2,
           DRIVER3,COMM3,ALLOW3,DRIVER4,COMM4,ALLOW4,
           DRIVER5,COMM5,ALLOW5,
           PT,DN,CASHBILL,
           MUDA1,MUDA2,BUSUK1,BUSUK2,HUJAN2,
           FIRST,SECOND,WASTAGE,NET,RATE,
           TFEE,HFEE,PRICE,LESS,AMT,SUBSIDY,SAMT,
           TIMEIN,TIMEOUT,REM,WFEE,PORLA_NO,UPDATE,W1,W2,Z1,Z2};
#define SHIPTO   PORLA_AC
#define SUBSIDY1 ALLOW1
#define SUBSIDY2 ALLOW2
#define SUBSIDY3 ALLOW3
#define TDATE Date
#define CUST ACCODE
#define PGROSS FIRST
#define PTARE  SECOND
enum accode {CU_CODENO=1,CU_NAME,CU_SNAME,CU_ADD1,CU_ADD2,CU_ADD3,
            CU_TEL,CU_FAX,CU_ICNO,CU_AGENT,CU_COMM0,CU_ALLOW0,CU_PORLA_AC,
            CU_POLAR,CU_EXPIRY,CU_KOD,CU_HEKTAR,CU_LOCATE,
            CU_CONTACT,CU_GROUP,CU_TERM,CU_TERMS,CU_TFEE,CU_HFEE,CU_SUBSIDY,
            CU_DCOMM,CU_HCOMM,CU_TCOMM,CU_REMARKS,
            CU_BALANCE,CU_BANK_AC,CU_DEBTOR_AC, CU_CREDITOR_AC,
            CU_MAXQTY,CU_CASHBILL};

enum prodtype {PT_CODE=1,PT_DECRIPT,PT_UNIT,PT_SPRICE,PT_REMARKS,PT_GL1,PT_GL2};
#define CU_EDATE CU_EXPIRY
#define CU_PORLA CU_POLAR

enum two {Tw_D1=1,Tw_D2,Tw_PAGE};
enum scv {Scv_SO1=1,Scv_SO2,Scv_ACCODE,Scv_PT,Scv_STYPE,Scv_PAGE};
enum profile {Pro_CODE=1,Pro_ADD1,Pro_ADD2,Pro_ADD3,Pro_CWTIME,
Pro_VW,Pro_STTIME,Pro_AUTOGREEN,
Pro_STABLE,Pro_ZERO,Pro_CAPTION1,Pro_CAPTION2,Pro_PORLA_NO,Pro_GRADING,Pro_TEXTMODE};
enum logerr {LGR_DATE=1,LGR_CASHIER,LGR_ACCESS,LGR_TICKET,LGR_TIME,LGR_PASS_TYPE,LGR_PROGRAM};
enum logo {Logo_CODE=1,Logo_NAME,Logo_ADD1,Logo_ADD2,Logo_ADD3,Logo_ADD4,
           Logo_DATE,Logo_DATE1,Logo_DATE2,Logo_TIME1,Logo_TIME2,
           Logo_SELECT1,Logo_SELECT2,Logo_SELECT3,Logo_RTITLE,Logo_GROUP,
           Logo_USER,Logo_USERNAME,Logo_PROGRAM_ID,Logo_CONDITION,Logo_PRINT_RC};

enum particul {PAR_SPEC=1,PAR_TITLE,PAR_REMARKS};
enum lorry {LO_VEHICLE=1,LO_SHORTCUT,LO_FLAG1,LO_DRIVER1,LO_FLAG2,LO_DRIVER2,LO_FLAG3,LO_DRIVER3,LO_FLAG4,LO_TARE,
LO_REMARKS,LO_ACCODE,LO_PT};enum unit {UNIT_UNIT=1,UNIT_NAME,UNIT_REMARKS};
enum locate {LOCATE_CODE=1,LOCATE_NAME,LOCATE_COMM,LOCATE_REMARKS};
enum kod {KOD_VTTYPE=1,KOD_VTNAME};
enum reason {REASON_CODE=1,REASON_NAME,REASON_REMARKS};
enum ireq {IQ_REFNO=1,IQ_DATE,IQ_ACCODE,IQ_ISSUED,IQ_PARTICULAR,
           IQ_DRIVER1,IQ_COMM1,IQ_DRIVER2,IQ_COMM2,IQ_DRIVER3,IQ_COMM3,
           IQ_BAL,IQ_REM1,IQ_REM2,IQ_CASHBILL,IQ_NAME};
enum reqitem {PI_REFNO=1,PI_SQNO,PI_DN,PI_LORRY,
              PI_PT,PI_NAME,PI_QTY,PI_UNIT,PI_PRICE,PI_AMOUNT,
              PI_DRIVER,PI_COMM,PI_NETCOMM};
#define PI_ITEM PI_PT
enum payment {PY_REFNO=1,PY_ACCODE,PY_DATE,PY_BANKIN,PY_AC,PY_TOTAL,
              PY_PARTICULAR,PY_DUE,PY_CHEQUE,PY_REM};
#define PY_GLCODE PY_AC
#define IP_GLCODE PY_AC
#define IP_TOTAL  PY_TOTAL
enum condition { St_D1=1,St_D2,St_ACCODE1,St_ACCODE2,St_GROUP,
                 St_TERM,St_BANK,};
enum driver {DR_CODE=1,DR_NAME,DR_NEWIC,DR_NRIC,DR_ADD1,DR_ADD2,DR_ADD3,
DR_SALARY,DR_TYPE,DR_EPF,DR_EPF_COMM,DR_SOCSO,DR_SOCSO_COMM,DR_REMARKS,DR_GLCODE,DR_POSITION};
enum miller {ML_REFNO=1,ML_DATE1,ML_DATE2,ML_ACCODE,ML_PRODUCT,
ML_GROUP,ML_TERM,ML_PRICE,ML_REMARKS};
enum report { IvS_DATE1=1,IvS_DATE2,IvS_ACCODE1,IvS_ACCODE2,IvS_PRODUCT,IvS_GROUP,
IvS_TERM,IvS_PAGE,IvS_TODAY};
enum arinv_wb {IVWB_INVOICE=1,IVWB_DATE,IVWB_ACCODE,IVWB_TERMS,IVWB_YOUR_REP,IVWB_OUR_REF,IVWB_REMARKS,IVWB_PAYDATE,IVWB_STATUS,IVWB_CURRENCY,IVWB_RATE};
enum aroinv_wb {VIWB_INVOICE=1,VIWB_SQNO,VIWB_DO,VIWB_QTY,VIWB_AMOUNT};
enum glgroup {Gp_CODE=1,Gp_DESCRIPT,Gp_TYPE,Gp_SQNO};
enum glcode {C_CODE=1,C_DESCRIPT,C_GLGROUP,C_TYPE,C_DBCR, CBAL,C_PRIMARY};
enum arinv {IV_BILLNO=1,IV_INVOICE,IV_DATE,IV_ETA,IV_CUSTOMER,
IV_TYPE,IV_STAFF,IV_PREFIX,IV_TERMS,
IV_CURR,IV_RATE,IV_PCCODE,IV_REM,IV_HOLD_PAY,IV_REASON,IV_TOTAL,
IV_BANK,IV_CHEQUE,IV_CQ_DATE,IV_BANKIN};
#define IV_TT       IV_TYPE
#define IV_CURRENCY IV_CURR
#define IV_DEBTOR IV_CUSTOMER
enum oinv {Oiv_REFNO=1,Oiv_SQNO,Oiv_GLCODE,Oiv_NAME,
Oiv_QTY,Oiv_PRICE,Oiv_AMT,Oiv_T,Oiv_VAT};
#define Oiv_AMOUNT Oiv_AMT
enum accpf {PF_COMPANY_NO=1,PF_ADD1,PF_ADD2,PF_ADD3,PF_TAXID,
PF_DATE,PF_DEBTOR,
PF_SALES,PF_CREDITOR,PF_PURCHASE,PF_PC,PF_BANK,PF_BC,PF_APF,PF_PF,
PF_VAT,PF_AR_CGL,PF_AP_CGL,PF_CURRENCY,
PF_ARPAY,PF_APPAY,PF_GLWAC,PF_ALIGN,PF_RPRINTER,
PF_AR,PF_AP,PF_GL,PF_TAX,PF_GLTAX,PF_PREVIEW,PF_CUSTOMER_BANK,
PF_TEMP_REC,PF_TAX_NO,PF_ETA_DATE,PF_PCCODE,
PF_CUST_PO,PF_SUPP_INV,PF_COLLECT_BY,PF_PAID_BY,PF_PAYTYPE,
PF_EMODE,PF_MULTI_CURR,PF_SCHEME,PF_TT,PF_PREFIX,
PF_SHOW_OVER_CREDIT_LIMIT,PF_AREA,PF_SALESMAN,PF_PD_CHQ,
PF_CONTRA,PF_RET_CHQ_RECORDS,PF_HOLD_PAYMENT,PF_PERIOD,
PF_CN_CATEGORY,PF_MULTI_USER,PF_CLIENT_SERVER,PF_SERVER_NAME,
};

#define Pro_SITE Pro_CODE
enum socso { SOCSO_CODE=1, SOCSO_BSALARY, SOCSO_RANGE, SOCSO_ESALARY,
SOCSO_EMPLOYEE, SOCSO_EMPLOYER, SOCSO_EMPLOYER2 } ; // SOCSO FILE

enum epf_rate {EPFRATE_CODE=1,EPFRATE_BOSS,EPFRATE_STAFF,EPFRATE_EPFMAX};

/////////////////////////////////////////////////////////////////////////////
// CMenuApp:
// See menu.cpp for the implementation of this class
//

class CMenuApp : public CMyApp
{
public:
	CMenuApp();
    virtual BOOL Domain();
    void Profile();
    virtual void OpenAllDataFile();
    virtual void SetStdCompanyLogo();
    virtual int SubProgram(int key);
    virtual BOOL RptResetFiles(const CString m_CrystalReportName,const CString repname,CString &newname);
    virtual CString ConfirmCrystalTableFile(char *FieldName,CString m_CrystalReportName);
    virtual void SetDBRelations();
    virtual void ToText(CString m_CrystalReportName,DB *scr,DB *rep);
    virtual void ToDisplay(HANDLE hComm,long weight, long &GwstWeight);
    DWORD len;
    DWORD NumberOfBytesWritten;
    BOOL bResult;
	int LastWeightCount;
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MENU_H__D6EA9606_A1F2_11D0_8F37_945AC87D3839__INCLUDED_)
