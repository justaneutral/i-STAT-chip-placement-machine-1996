// mainfrm.h : interface of the mainFrame class
//
/////////////////////////////////////////////////////////////////////////////
#include "EditColor.h"

class mainFrame : public CFrameWnd
{
public:
	mainFrame();
	HANDLE m_proc_thread;
	bool is_suspended;
	CWnd m_window;
	CEditColor *m_text_output;

	void ProcStop();
	int timer_counter;

	CHAR s[80];

// Generated message map functions
protected:
	//{{AFX_MSG(mainFrame)
	afx_msg void onExit();
	afx_msg void onSimple();
	afx_msg void onModal();
	afx_msg void onModeless();
	afx_msg void onMessageBox();
	afx_msg void Ondiagnostic();
	afx_msg void OnSmacTx();
	afx_msg void OnProcStart();
	afx_msg void OnGetBaseCoord();
	afx_msg void OnCreatenewuser();
	afx_msg void OnProcSuspend();
	afx_msg void OnSettingsupdate();
	afx_msg void OnSettingsRecreate();
	afx_msg void OnReadfrommenu();
	afx_msg void Onchangeuser();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void Onfetchersequenceparameters();
	afx_msg void Onmotorparameters1();
	afx_msg void Onmotorparameters2();
	afx_msg void Onmotorparameters3();
	afx_msg void Onmotorparameters4();
	afx_msg void Onmotorparameters5();
	afx_msg void Onmotorparameters6();
	afx_msg void Onmotorparameters7();
	afx_msg void Onmotorparameters8();
	afx_msg void Onmotorparameters9();
	afx_msg void Onpickplaceparameters();
	afx_msg void Onpickpointparameters();
	afx_msg void Onplacepiontpirameters();
	afx_msg void Onwaferreferencepointparameters();
	afx_msg void Onwafercameraparameters();
	afx_msg void Onsavesettings();
	afx_msg void Onprocesscurrentconditions();
	afx_msg void OnPaint();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnAbout();
	afx_msg void Onwindow();
	afx_msg void Onchangeproducttype();
	afx_msg void Onstatusscreen();
	afx_msg void Onuserlogin();
	afx_msg void OnEditUsers();
	afx_msg void OnBaseBlack();
	afx_msg void OnBaseWhite();
	afx_msg void OnCollectHomeStrokes();
	afx_msg void OnProductList();
	afx_msg void OnManipulator();
	afx_msg void OnWaferAdjust();
	afx_msg void OnInitMachine();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
