// ProgressBar.cpp : implementation file
//

#include "stdafx.h"
#include "AnMap.h"
#include "ProgressBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProgressBar

CProgressBar::CProgressBar()
{
}

CProgressBar::~CProgressBar()
{
}


BEGIN_MESSAGE_MAP(CProgressBar, CProgressCtrl)
	//{{AFX_MSG_MAP(CProgressBar)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProgressBar message handlers

BOOL CProgressBar::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// ���¹ٸ� ����
	CStatusBar *pStatusBar = GetStatusBar();
	if( !pStatusBar ) return FALSE;

	// ���¹� ���� ���α׷��� ��Ʈ�� ����
	if( !CProgressCtrl::Create(WS_CHILD | WS_VISIBLE, CRect(0,0,0,0), pStatusBar, 1)) return FALSE;

	// ���α׷��� ��Ʈ���� ������ ���� ����
	SetRange(0, 100);
	SetStep(1);
	
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

CStatusBar* CProgressBar::GetStatusBar()
{
	CFrameWnd *pFrame = (CFrameWnd*)AfxGetMainWnd();
	if(!pFrame || !pFrame->IsKindOf(RUNTIME_CLASS(CFrameWnd))) return NULL;
	CStatusBar *pBar = (CStatusBar*) pFrame->GetMessageBar();
	if( !pBar || !pBar->IsKindOf(RUNTIME_CLASS(CStatusBar))) return NULL;
	return pBar;
}
