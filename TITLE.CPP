// title.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"


int AutoTitleCenter(int xlen,char *tmp,char *source)
{
    char text[200];
	
    strcpy(tmp,source);
    rtrim(tmp);
    int l2 = strlen(tmp);
    if(l2==0) return 0;
    int len = (xlen - l2) / 2;
    if(xlen % 2) len--;
    space_str(text,len);
    strcat(text,tmp);
    strcpy(tmp,text);
    return l2;
}

void PrintAddress(FILE *ptr,DB *df)
{
    fprintf(ptr,"M\n%s",profile.dat[2]);
    fprintf(ptr,"\n%s",profile.dat[3]);
    //fprintf(ptr,"\n%s",profile.dat[3]);

    fprintf(ptr,"\n%s       PTICKET NO: E%sFM",profile.dat[4],df->dat[1]);
    fprintf(ptr,"\n%s       PDATE     : E%sFM",profile.dat[5],mdate(df->dat[Date]));
}

void PrintAddress2(FILE *ptr)
{
    char tmp[200];

    for(int i=1; i<=5; i++){
        int flag = AutoTitleCenter(80,tmp,profile.dat[i]);
        if(flag>0) {
            fprintf(ptr,"\n%s",tmp);
        }
    }
}

void PrintTicket(FILE *ptr,char *tl)
{
    fprintf(ptr,"2CBP");

    char szTmp[120], tmp[120];

    int tlen = strlen(title);
    int len = (35 - tlen) / 2;
    if(len<0) len = 0;

    if(len >0 ) {
        space_str(tmp, len);
        sprintf(szTmp, "E%s", tmp);
        strcat(szTmp, "%sF (%s)");
    }
    else {
        strcpy(szTmp, "EP%sFM (%s)");
    }

    PrintAddress2(ptr);
}


void PrintTicketSlip(FILE *ptr,char *tl,DB *df)
{
    char tmp[50];

    strcpy(tmp,profile.dat[1]);
    rtrim(tmp);
    fprintf(ptr,"PC!3%c\n2",1);
    //PrintChineseTitle(ptr);
    int len = strlen(title);
    if(len>35) {
        fprintf(ptr,"ME%sFP",title);
    }
    else {
        fprintf(ptr,"PE%sF",title);
    }
    
    fprintf(ptr,"tP");
    PrintAddress(ptr,df);
    
}



void PrintLogo(FILE *ptr,char *title)
{
    char tmp[200];


    int flag = AutoTitleCenter(48,tmp,title);
    fprintf(ptr,"ME%sFP",tmp);
    for(int i=1; i<=5; i++){
        flag = AutoTitleCenter(80,tmp,profile.dat[i]);
        if(flag>0) {
            fprintf(ptr,"\n%s",tmp);
        }
    }
}
