// Detail.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"

void DetailListingReport(DB *df)
{
    if ( df == &wb) {
        PeriodCrystalRpt("detail.rpt");
    }
    else {
        PeriodCrystalRpt("detail2.rpt");
    }
}
