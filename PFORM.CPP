// pform.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "menu.h"

#include <stdio.h>
#include <io.h>
#include <dos.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <math.h>
#include <memory.h>
#include <string.h>
#include <fcntl.h>
#include <share.h>
#include <sys\types.h>
#include <sys\stat.h>

char *PM[] = {
"P",
"                              %c %c %c %c %c\n",
"\n",
"         %s\n",
"         %s\n",
"         %s\n",
"         %s\n",
"\n",
"\n",
"         %s\n",
"         %s\n",
"         %s\n",
"         %s\n",
"                              %c %c %c %c %c\n",
"                                            %-11.11s   %s %s\n",
"         %-35.35s\n",
"         %-35.35s  %-10.10s  %c %c  %-10.10s    %c %c\n",
"         %-35.35s\n",
"         %-35.35s        %c %c %c  %-20.20s\n",
"\n",
"        %2.2s                      %s    %-10.10s %-10.10s  %-8.8s\n",
"\n",
"        GEM%sPFH  %-12.12s %3.3s          %s%c %c  %-8.8s\n",
"\n",
"         %-11.11s %3.3s  %-12.12s %3.3s  %10.10s %-12.12s  E%sF",
"\n",
"\n",
"\n",
"                    %-39.39s %-12s %-6s\n",
"        %-10.10s\n",
"                    %-39.39s %-12s %-6s\n",
"M\n",
"          %s                       NO. SURAT JAMINAN BANK : %s\n",
"\n",
"          %s                       JUMLAH                 : %s\n",
"\n",
"          %s                       TARIKH LUPUT           : %s\n",
"\n",
"          %s                       CONTRACT NO            : %s\n",
"\n",
"\n",
"\n",
"\n",
"\n",
"          %s  %s  %s  %s  %s %s %s  %s %s %s\n",
"                 (%s kg)\n",
"          %s  %s  %s  %s  %s %s %s  %sP\n",
"\n",
"          M%sP   M%sP\n",
"          M%sP   M%sP\n",
"          M%sP   M%sP\n",
"          M%sP   M%sP\n",
"          M%sP   M%sP\n",
"          M%sP   M%sP\n",
"          M%sP   M%sP\n",
"          M%sP   M%sP\n",
"          M%sP   M%sP\n",
"\n",
"                                      %-8.8s   %-8.8s     %-8.8s\n",
"         %-20.20s\n",
"\n",
"              %c %c %c %c %c %c %c %c                       \n",
"\n",
"         %-s\n",
"\n",
"\n",
"         %-s",
""};

char Bv[10][20];

int PrintTotalValue(FILE *ptr,int i,int k)
{
    int j;

    for (j=0;j<10;j++){
        switch(form3.f[k+j].mode){
        case 'N':
            strcpy(Bv[j],putnum(form3.dat[k+j]));
            break;
        default:
            strcpy(Bv[j],form3.dat[k+j]);
            break;
        }
    }
    fprintf(ptr,PM[i],Bv[0],Bv[1],Bv[2],Bv[3],Bv[4],Bv[5],Bv[6],Bv[7],Bv[8],Bv[9]);
    k +=10;
    return k;
}

void PrintLittleCode(FILE *ptr,int i,int fld)
{
    char ch[10];
    int x;

    lset(customer.dat[1],form1.dat[fld]);
    qd(customer.dat[1],&customer);
    for(x=0; x<5; x++)
        ch[x] = form1.dat[fld][x];
    fprintf(ptr,PM[i],ch[0],ch[1],ch[2],ch[3],ch[4]);
}

void PrintIC(FILE *ptr,int i)
{
    int  x;
    char ch[10];

    for (x=0; x<=7; x++ )
        ch[x] = form4.dat[5][x];
    fprintf(ptr,PM[i], ch[0],ch[1],ch[2],ch[3],ch[4],ch[5],ch[6],ch[7]);
}

void pform(FILE *ptr)
{
    int    i,j=1,k=1,l=1;
    double wt;
    char   kw1[20],kw2[20];
    char   ch[10];

    for(i=0; PM[i][0]; i++) {
        switch(i) {
        case 1:
            PrintLittleCode(ptr,i,EXPORTER);
            break;
        case 3:
            fprintf(ptr,PM[i],customer.dat[CU_CUSTOMER]);
            break;
        case 4:
        case 5:
        case 6:
            fprintf(ptr,PM[i],customer.dat[i-4+CU_ADD1]);
            break;
        case 9:
            qd(form1.dat[IMPORTER],&customer);
            fprintf(ptr,PM[i],customer.dat[CU_CUSTOMER]);
            break;
        case 10:
        case 11:
        case 12:
            fprintf(ptr,PM[i],customer.dat[i-10+CU_ADD1]);
            break;
        case 13:
            PrintLittleCode(ptr,i,AGENT);
            break;
        case 14:
            fprintf(ptr,PM[i],form1.dat[TREATMENT],form1.dat[PERMIT_NO],form1.dat[KPWXNO]);
            break;
        case 15:
            fprintf(ptr,PM[i],customer.dat[2]);
            break;
        case 16:
            qd(form1.dat[ORIGIN],&place);
            strcpy(kw1,place.dat[2]);
            qd(form1.dat[DESTINA],&place);
            strcpy(kw2,place.dat[2]);
            ch[0] = form1.dat[ORIGIN][0];
            ch[1] = form1.dat[ORIGIN][1];
            ch[2] = form1.dat[DESTINA][0];
            ch[3] = form1.dat[DESTINA][1];
            fprintf(ptr,PM[i],customer.dat[CU_ADD1],kw1,ch[0],ch[1],kw2,ch[2],ch[3]);
            break;
        case 17:
            fprintf(ptr,PM[i],customer.dat[CU_ADD2]);
            break;
        case 18:
            ch[0] = form1.dat[CURRENCY1][0];
            ch[1] = form1.dat[CURRENCY1][1];
            ch[2] = form1.dat[CURRENCY1][2];
            fprintf(ptr,PM[i],customer.dat[CU_ADD3],ch[0],ch[1],ch[2],form1.dat[AMOUNT]);
            break;
        case 20:
            if(isblank(form1.dat[EXPORTDATE]))
                fprintf(ptr,PM[i], form1.dat[MODE], " " ,form1.dat[EXCHANGE],form1.dat[EQUIVALENT],form1.dat[INSURANCE]);
            else
                fprintf(ptr,PM[i], form1.dat[MODE], rdate(form1.dat[EXPORTDATE]) ,form1.dat[EXCHANGE],form1.dat[EQUIVALENT],form1.dat[INSURANCE]);
            break;
        case 22:
            qd(form1.dat[P_EXPORT],&place);
            strcpy(kw1,place.dat[2]);
            ch[0] = form1.dat[PAYMENT][0];
            ch[1] = form1.dat[PAYMENT][1];
            qd(form1.dat[PAYMENT],&place);
            strcpy(kw2,place.dat[2]);
            fprintf(ptr,PM[i], form1.dat[VESSEL],kw1,form1.dat[P_EXPORT],kw2,ch[0],ch[1],form1.dat[FREIGHT]);
            break;
        case 24:
            qd(form1.dat[PORT_DSG],&place);
            strcpy(kw1,place.dat[2]);
            qd(form1.dat[VIA],&place);
            strcpy(kw2,place.dat[2]);
            fprintf(ptr,PM[i], kw1,form1.dat[PORT_DSG],kw2,form1.dat[VIA],form1.dat[GROSS],form1.dat[MEASURE],putnum(form1.dat[FOB]));
            if(!strcmp(custom.dat[4],"24")){
                fprintf(ptr,"\n");
            }
            break;
        case 28:
        case 30:
            fprintf(ptr,PM[i], form2.dat[j+0],form2.dat[j+1],form2.dat[j+2]);
            j +=3;
            break;
        case 29:
            fprintf(ptr,PM[i], form1.dat[CONTAINER]);
            break;
        case 32:
        case 34:
        case 36:
        case 38:
            fprintf(ptr,PM[i],form5.dat[l+0],form5.dat[l+1]);
            l +=2;
            break;
        case 44:
        case 46:
            k = PrintTotalValue(ptr,i,k);
            break;
        case 45:
            if(pt.dat[PT_UNIT][0]=='K') {
                wt = atof(form3.dat[F3_TRAIFF1]) * 1000;
                dtoa(wt,kw1,6,0);
                fprintf(ptr,PM[i],putnum(kw1));
            }
            else {
                fprintf(ptr,"\n");
            }
            break;
        case 47:
            if(!strcmp(custom.dat[4],"24")){
                fprintf(ptr,"3");
            }
            else {
                fprintf(ptr,"\n");
            }
            break;
        case 48:
        case 49:
        case 50:
        case 51:
        case 52:
        case 53:
        case 54:
        case 55:
        case 56:
            fprintf(ptr,PM[i], sign.dat[i-48+SN_REM1],format.dat[i-48+FM_REM1]);
            break;
        case 58:
            fprintf(ptr,PM[i], form4.dat[1],form4.dat[2],form4.dat[3]);
            break;
        case 59:
            fprintf(ptr,PM[i], form4.dat[4]);
            if(!strcmp(custom.dat[4],"24")){
                fprintf(ptr,"3#");
            }
            break;
        case 61:
            PrintIC(ptr,i);
            if(!strcmp(custom.dat[4],"24")){
                fprintf(ptr,"3");
            }
            break;
        case 63:
            fprintf(ptr,PM[i], form4.dat[6]);
            break;
        case 66:
            fprintf(ptr,PM[i],rdate(form1.dat[EXPORTDATE]));
            break;
        default:
            fprintf(ptr,PM[i]);
            break;
       }
    }
}
