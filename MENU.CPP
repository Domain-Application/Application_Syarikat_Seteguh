// menu.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "menu.h"

#include "MainFrm.h"
#include "menuDoc.h"
#include "menuView.h"
#include "password.h"
#include "start.h"
#include <locale.h>
#include "initial.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

DB  malaysia_state;
DB  malaysia_city;
DB  postcode;
DB  conso_einv, conso_einv_sub, classification;

DB first(IDD_RECEIVE),prodtype(IDD_PT,4);
DB Status(IDD_STATUS),gst(IDD_GWST),stype;
DB *ttt;
DB pr(IDD_PR),fpr(IDD_PR);
DB lorry(IDD_LORRY),wbsetup;
DB  state,sto,strun_no(IDD_ACCODE,3),tscr;

long GreenLight,LastLight,WeighingMode,LastWeight;
/////////////////////////////////////////////////////////////////////////////
// CMenuApp initialization
int Purchase_Flag;
DB  socso(IDD_SOCSO);
DB  epf_rate(IDD_EPF_RATE);
DB  arinv,apinv,aroinv,apoinv;
DB  aroinv_wb(IDD_AROINV_WB);
DB  arinv_wb(IDD_ARINV_WB,4);
DB  apoinv_wb(IDD_APOINV_WB);
DB  apinv_wb(IDD_APINV_WB,4);
DB  miller(IDD_MILLER),up_des(IDD_MILLER);
DB  tt(IDD_TT);
DB  term(IDD_TERM);
DB  unit(IDD_UNIT);
DB  particul(IDD_PARTICUL);
DB  group(IDD_GROUP);
DB  kod(IDD_KOD);
DB  locate(IDD_LOCATE);
DB  agent(IDD_AGENT);
DB  cashbill(IDD_CASHBILL);
DB  despatch(IDD_DESPATCH);
DB  fdes(IDD_DESPATCH);
DB  initem(IDD_INITEM);
DB  ientry(IDD_IENTRY);
DB  driver(IDD_DRIVER);
DB  position(IDD_POSITION);
DB  reqitem(IDD_REQITEM);
DB  ireq(IDD_IREQ);
DB  reason(IDD_REASON);
DB  glgroup(IDD_GLGROUP);
DB  glcode(IDD_GLCODE);
DB  salary(IDD_SALARY);
DB  advance(IDD_ADVANCE);
DB  payment(IDD_PAYMENT);
DB  receipt(IDD_RECEIPT);
DB  receive(IDD_RECEIVE);
DB  accode(IDD_ACCODE);
DB  transp(IDD_TRANSP);
DB  wb(IDD_PR);
DB  fwb(IDD_PR);

CMenuApp theApp;
CMenuApp::CMenuApp()
{
    GreenLight = WeighingMode = LastWeightCount = 0;
    LastLight = -1;
	bComOutput = TRUE;
    LastWeight = -1;

	IsSqlFile = TRUE;
    //company = "E9DAC95AA9EC59DADA0A5AECGA5A2AECE9DA2A1AECB9AA0AECCA6AAA";    //PARIT YAANI OIL PALM SDN BHD
    //company = "CADA3AA95AEC4ADA79DAECA9C9DAAA5A0A7A";    //BAKTI JAWA TRADING
	//company = "7ADA59DAEC79DAC95AB9EC6AGA2AAA5A0A7AB9ECB9AA0AECCA6AAA";	//GAYA WARIS HOLDINGS SDN BHD

	company = "B959DAC95A3ADAA9ECB99AA99A7A996A";  //SYARIKAT SETEGUH 2025-10-08


	GetSystemProviderString();
	GetInternetFolderString();
	
}

void CMenuApp::Profile()
{

    SetRegistryKey(_T(title));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)
}

BOOL CMenuApp::Domain()
{
    CStart p1;
    p1.InitialFlag = 0x0002 | INIT_ADO_USER |
		INIT_ENCRYPT_PROGRAM_PASSWORD | INIT_ADO_USER |
		INIT_ADO_PROGRAM_PASSWORD;
    p1.DefaultUser = m_defaultUser;
    p1.DoModal();

    if(p1.rec==0) return FALSE;
    UserLogin(p1.UserID);

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
        IDR_MAINMENU,
		RUNTIME_CLASS(CMenuDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CMenuView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	return TRUE;
}

BOOL CMenuApp::RptResetFiles(const CString m_CrystalReportName,const CString repname,CString &newname)
{
    if(STCR == 1) return FALSE;
    if(STCR == 2) {
        if(!repname.CompareNoCase("receive")) {
            newname = "despatch";
            return TRUE;
        }
    }
    return FALSE;
}

CString CMenuApp::ConfirmCrystalTableFile(char *FieldName,CString m_CrystalReportName)
{
    if(STCR == 1) return FieldName;
    CString repname = FieldName;
    if(STCR == 2) {
        if(!repname.CompareNoCase("receive")) {
            return "despatch";
        }
    }
    return repname;
}

void CMenuApp::SetDBRelations()
{
    arinv_wb.sub = &aroinv_wb;
    apinv_wb.sub = &apoinv_wb;
    ientry.sub = &initem;
    ireq.sub = &reqitem;

	first.m_MACMask[TIMEIN] = "2#:5#:5#";
	first.m_MACMask[TIMEOUT] = "2#:5#:5#";
	receive.m_MACMask[TIMEIN] = "2#:5#:5#";
	receive.m_MACMask[TIMEOUT] = "2#:5#:5#";
	fdes.m_MACMask[TIMEIN] = "2#:5#:5#";
	fdes.m_MACMask[TIMEOUT] = "2#:5#:5#";
	despatch.m_MACMask[TIMEIN] = "2#:5#:5#";
	despatch.m_MACMask[TIMEOUT] = "2#:5#:5#";
	cashbill.m_MACMask[TIMEIN] = "2#:5#:5#";
	cashbill.m_MACMask[TIMEOUT] = "2#:5#:5#";

}

void solve_state(FILE *ptr,int line,FM *fm,char *text)
{
    replace(text,"%s",mdate(two.dat[2]));
    image(fm,ptr,text);
}   

void CMenuApp::ToText(CString m_CrystalReportName,DB *scr,DB *rep)
{
    FM fm;
    plan(&fm,scr,rep,solve_state);
}

void CMenuApp::ToDisplay(HANDLE hComm,long weight, long &GwstWeight)
{
	long oweight = weight;

	if( weight > 1000 ) {
        if(GwstWeight) {
            weight -= GwstWeight;
        }
        else if(GwstPercent) {
            weight *= double (100-GwstPercent) / 100;
        }
        weight = radix( weight );
    }

	if(weight < 1000 || oweight < 1000) {
		weight = oweight;
	}



    if(weight>=0) {
        str.Format("%c%c %6ldkg %c%c",
            2,'+',weight,13,10);     
    }
    else {
        str.Format("%c%c %6ldkg %c%c",
            2,'-',weight * -1,13,10);            
    }
    DWORD len = 14;
    DWORD NumberOfBytesWritten;
    BOOL bResult = WriteFile(hComm,str,len,&NumberOfBytesWritten,NULL);
    if(!bResult) {
    }

    LastWeight = weight;
	
}

void CMenuApp::OpenAllDataFile()
{
	ado(&socso,"socso");    
	if(ado(&epf_rate,"epf_rate")) {
        readrec(1,&epf_rate);
    }
	//tado(&arinv,"arinv");	
    //tado(&apinv,"apinv");
    ado(&malaysia_state,"malaysia_state");
    ado(&malaysia_city,"malaysia_city");
    ado(&postcode,"postcode");
    tado(&conso_einv, "conso_einv");
    tado(&conso_einv_sub, "conso_einv_sub");
    ado(&classification, "classification");
    tado(&aroinv_wb,"aroinv_wb",init_aroinv_wb);
    tado(&arinv_wb,"arinv_wb",init_arinv_wb);
    tado(&apoinv_wb,"apoinv_wb",init_aroinv_wb);
    tado(&apinv_wb,"apinv_wb",init_arinv_wb);
    tado(&miller,"miller",init_miller);
	tado(&up_des,"up_des",init_miller);
    ado(&tt,"tt");
    ado(&term,"term");
    ado(&unit,"unit");
	ado(&lorry,"lorry");
    ado(&particul,"particul");
    ado(&group,"group");
    ado(&kod,"kod");
    ado(&locate,"locate");
    ado(&agent,"agent");
    tado(&cashbill,"cashbill",init_wb);
    tado(&despatch,"despatch",init_wb);
	ado(&fdes,"fdes",init_first); // despatch first weight
	tado(&receive,"receive",init_wb);
	ado(&first,"first",init_first); // receive first weight

    tado(&initem,"initem",init_reqitem);
    tado(&ientry,"ientry",init_ientry);
    tado(&reqitem,"reqitem",init_reqitem);
    tado(&ireq,"ireq",init_ireq);
    ado(&position,"position");
    ado(&reason,"reason");
    ado(&glgroup,"glgroup");
    ado(&glcode,"glcode");
    tado(&salary,"salary",init_salary);
    tado(&advance,"advance");
    tado(&payment,"payment",init_payment);
    tado(&receipt,"receipt",init_payment);
    
    if(ado(&profile,"profile")) {
        readrec(1,&profile);
        profile.ID = IDD_PROFILE;
    }
    
    ado(&accode,"accode",init_accode);
    ado(&prodtype,"pt",init_stdlist);
    ado(&driver,"driver",init_stdlist);
    ado(&transp,"transp",init_stdlist);
    ado(&fpr,"fpr",init_first);
    tado(&pr,"pr",init_wb);
	tado(&wb,"wb",init_wb);
	ado(&fwb,"fwb",init_first);
    if(ado(&gst,"gst")) {
        readrec(1,&gst);
    }
    ado(&stype,"stype");
    ado(&Status,"status");

    if(ado(&logo,"logo")) {
       SetStdCompanyLogo();
       writerec(1,&logo);
    }

    dopen(&one,"one.scr");
    dopen(&two,"two.scr");
    dopen(&wbsetup,"wbsetup.scr");
    dopen(&strun_no,"strun_no.dbf");
}
