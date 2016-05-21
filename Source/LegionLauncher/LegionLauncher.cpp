
// LegionLauncher.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "LegionLauncher.h"
#include "LauncherDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLauncherApp

BEGIN_MESSAGE_MAP(CLauncherApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CLauncherApp construction

CLauncherApp::CLauncherApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

CLauncherApp theApp;
LauncherDlg launcher;


// CLauncherApp initialization

BOOL CLauncherApp::InitInstance()
{
	char _filename[1024];
	GetModuleFileName(NULL, _filename, sizeof(_filename));
	string filename = _filename;

	// new updater
	if(filename.find("LegionClient.exe") != string::npos)
	{
		system("copy LegionClient.exe LegionLauncher.exe");
		ShellExecute(NULL, NULL, "LegionLauncher.exe", NULL, NULL, SW_SHOWNORMAL);
		return false;
	}
	
	remove("LegionClient.exe");
	remove("ICSharpCode.SharpZipLib.dll");
	remove("updater log.txt");

	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}


	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	//CShellManager *pShellManager = new CShellManager;

	SetRegistryKey(_T(REGISTRY_KEY));

	m_pMainWnd = &launcher;
	INT_PTR nResponse = launcher.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	remove("Patch.txt");

	// Delete the shell manager created above.
	//if (pShellManager != NULL)
	//{
	//	delete pShellManager;
	//}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

