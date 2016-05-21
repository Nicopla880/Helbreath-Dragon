// LauncherDlg.h : header file
//
#include "resource.h"		// main symbols
#include "Updater.h"

#pragma once

// LauncherDlg dialog

class LauncherDlg : public CDialogEx
{
	DECLARE_DYNAMIC(LauncherDlg)

public:
	LauncherDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~LauncherDlg();

// Dialog Data
	enum { IDD = IDD_LAUNCHER_DIALOG };
	enum { TIMER_CREATE, TIMER_LOOP };

	Updater updater;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void SetStatus(char * desc);
	void SetStatusProgress(int i);
	void SetSubStatus(char * desc);
	void SetSubStatusProgress(int i);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
