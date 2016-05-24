// LauncherDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LegionLauncher.h"
#include "LauncherDlg.h"
#include "afxdialogex.h"

HANDLE ftpthread = NULL;
extern LauncherDlg launcher;
Updater & updater = launcher.updater;
CProgressCtrl * statusprog;
CProgressCtrl * substatusprog;

char txt[1024];

DWORD WINAPI FtpThreadFunc( LPVOID lpParam )
{
	launcher.updater.Run();
	launcher.updater.isrunning = false;

	//int j = 0;
	//for(int i = 0; i < 100000000; i++)
	//{
	//	j += i%20 + i/(j+i+1);
	//}

	return 0;
}

bool DownloadStatus(long bytesReceived)
{
	if(updater.files.size()  > 0)
	{		
		substatusprog->ShowWindow(SW_SHOW);

		float subprog = (float)bytesReceived / 
			(float)updater.currfilesize;

		launcher.SetSubStatusProgress((int)(subprog * 1000.0));


		float prog = ((float)updater.m_bytesdone + (float)(updater.files[updater.currfile].size * subprog)) 
			/ (float)updater.m_totalbytes;
		if(updater.m_bytesdone + (float)(updater.files[updater.currfile].size * subprog) >= updater.m_totalbytes)
			prog = 0.0;
		launcher.SetStatusProgress((int)(prog * 1000.0));
	}

	return true;
}

// LauncherDlg dialog

IMPLEMENT_DYNAMIC(LauncherDlg, CDialogEx)

LauncherDlg::LauncherDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(LauncherDlg::IDD, pParent)
{

}

LauncherDlg::~LauncherDlg()
{
}

void LauncherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(LauncherDlg, CDialogEx)
	ON_WM_CREATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// LauncherDlg message handlers

int LauncherDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetTimer(TIMER_CREATE, 100, 0);
	
	return 0;
}

void LauncherDlg::SetStatus(char * desc)
{
	SetDlgItemTextA(IDC_STATUS, desc);
}

void LauncherDlg::SetStatusProgress(int i)
{
	statusprog->SetPos(i+1); // trick for progress bar to move immediately (goes forward then backward)
	statusprog->SetPos(i);
}

void LauncherDlg::SetSubStatus(char * desc)
{
	SetDlgItemTextA(IDC_SUBSTATUS, desc);
}

void LauncherDlg::SetSubStatusProgress(int i)
{
	substatusprog->ShowWindow(SW_SHOW);
	substatusprog->SetPos(i+1); // trick for progress bar to move immediately (goes forward then backward)
	substatusprog->SetPos(i);
}


void LauncherDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{
	case TIMER_CREATE:
		ftpthread = CreateThread( 
			NULL,                   // default security attributes
			0,                      // use default stack size  
			FtpThreadFunc,			   // thread function name
			0,							   // argument to thread function 
			0,                      // use default creation flags 
			0);   // returns the thread identifier 
		
		statusprog = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1);
		statusprog->SetRange(0, 1000);

		substatusprog = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS2);
		substatusprog->SetRange(0, 1000);

		KillTimer(nIDEvent);

		SetTimer(TIMER_LOOP, 100, 0);
		break;

	case TIMER_LOOP:
		if(!updater.isrunning)
		{
			KillTimer(nIDEvent);
			if(updater.error != "")
			{
				string error = "There was a problem running the updater. Please restart the launcher to try again. If the problem persists, visit our website (www.helbreathdragon.com) or forums for more help. \n\nError details:\n";
				error += updater.error;
				int wsae = WSAGetLastError();
				sprintf(txt, "\nWSA: %d", wsae);

				error += txt;
				MessageBox(error.c_str(), APP_NAME);
			} else if(updater.isupdated)
			{
				ShellExecute(NULL, NULL, "LegionGame.exe", "_uK!(-*\\", NULL, SW_SHOWNORMAL);
			} else {
				// update the updater
				MessageBox("Restarting Launcher for update...", APP_NAME);
				ShellExecute(NULL, NULL, "LegionClient.exe", NULL, NULL, SW_SHOWNORMAL);
			}

			DestroyWindow();
		}
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}
