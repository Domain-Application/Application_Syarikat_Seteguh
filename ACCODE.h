
#include "Staff.h"
class CAccode : public CStaff
{
// Construction
public:
	CAccode(UINT IDD,DB *pdf,int Idx,CWnd* pParent = NULL);
	// virtual void OnDel();
	enum { IDD = IDD_ACCODE};

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnEinv();
protected:
	// virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
};
