#include "stdafx.h"
#include "updater.h"
#include "StrTok.h"
#include <vector>
#include <string>
#include "LauncherDlg.h"
#include "crc.h"
#include "unzip.h"

extern LauncherDlg launcher;

char utxt[1024];

Updater::Updater()
{
	currDir = "/";
	error = "";
	isrunning = true;
	isupdated = false;
}

Updater::~Updater()
{
	ftp.Close();
}

void Updater::Run()
{
	CreateDirs();

	int ret = ftp.FTPConnect(FTP_HOST, FTP_USERNAME, FTP_PASSWORD, "");

	if(ret != UTE_SUCCESS)
	{
		error = ftp.GetLastResponse();
		error += " : ";
		error += CUT_ERR::GetErrorString (ret);
		return;
	}

	ftp.GetDirInfo();
	if(ret != UTE_SUCCESS)
	{
		error = ftp.GetLastResponse();
		error += " : ";
		error += CUT_ERR::GetErrorString (ret);
		return;
	}
	
	launcher.SetStatus("Checking for updates...");

	if(!DownloadFile("Patch.txt"))
	{
		return;
	}

	if(!ProcessPatchFile())
	{
		error = "Failed to process patch file.";
		return;
	}
	
	launcher.SetStatus("Checking files...");
	CheckFiles();

	launcher.SetStatusProgress(0);
	launcher.SetStatus("Downloading and updating files...");
	if(UpdateSelf())
	{
		return;
	}

	UpdateFiles();

	isupdated = true;
}

bool Updater::ProcessPatchFile()
{
	HANDLE h = CreateFile("Patch.txt", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if(h == INVALID_HANDLE_VALUE)
		return false;

	DWORD filesize = GetFileSize(h, NULL);
	CloseHandle(h);


	FILE * patchfile = fopen("Patch.txt", "r");
	if(patchfile == 0)
		return false;

	char * buff = new char[filesize+1];
	ZeroMemory(buff, filesize+1);
	fread(buff, 1, filesize+1, patchfile);

 	StrTok tokens( new CStrTok(buff, "\n") );
	
	char * token;

	while(token = tokens->pGet())
	{
		StrTok subtokens( new CStrTok(token, "*" )  );

		File file;
		file.name = subtokens->pGet();
		file.hash = subtokens->pGet();
		file.size = atoi( subtokens->pGet() );
		
		if(file.name == "LegionClient.exe")
		{
			updaterver = file.hash;
		}
		else 
			files.push_back(file);
	}
	
	fclose(patchfile);
	return true;
}

void Updater::CheckFiles()
{
	char cHash[50];
	DWORD crc;
	char *hash;

	DWORD totalbytes = 0;
	for(int i = 0; i < files.size(); i++)
	{
		totalbytes += files[i].size;
	}

	DWORD bytesdone = 0;
	for(int i = 0; i < files.size();	i++)
	{
		crc32file(files[i].name.c_str(), &crc);				
		sprintf(cHash, "%08lX",crc);
		sscanf(files[i].hash.c_str(), "%d", &hash);

		float prog = 1000.0 * ((float)bytesdone / (float)totalbytes);
		launcher.SetStatusProgress((int)prog);

		files[i].uptodate = (strcmp(files[i].hash.c_str(), cHash) != 0) ? false : true;

		//if(!files[i].uptodate)
		//{
		//	console->PrintLog("File: %s is not up to date!\n", files[i].name.c_str());
		//}
		//else
		//{
		//	console->PrintLog("File: %s is up to date!\n", files[i].name.c_str());
		//}

		bytesdone += files[i].size;
	}

	return;
}

bool Updater::UpdateSelf()
{
	Boolean_T errors;
	DWORD crc;
	char cHash[50];
	errors = crc32file("LegionLauncher.exe", &crc);
	sprintf(cHash,"%08lX",crc);
	if(updaterver != cHash)
	{
		sprintf(utxt, "Current file: LegionLauncher.exe");
		launcher.SetSubStatus(utxt);
		launcher.SetSubStatusProgress(0);

		DownloadFile("LegionClient.exe.zip");
		HZIP hz = OpenZip("LegionClient.exe.zip",NULL);
		ZIPENTRY ze; GetZipItem(hz,-1,&ze); int numitems=ze.index;
		for (int zi=0; zi<numitems; zi++)
		{ 
			ZIPENTRY ze; GetZipItem(hz,zi,&ze); 
			UnzipItem(hz, zi, ze.name);        
		}
		CloseZip(hz);
		remove("LegionClient.exe.zip");
		return true;
	}
	return false;
}

void Updater::UpdateFiles()
{
	Boolean_T errors;
	char cHash[50];
	DWORD crc;
	char *hash;

	m_totalbytes = 0, m_bytesdone = 0;
	
	for(int i = 0; i < files.size(); i++)
	{
		if(files[i].uptodate)
		{
			files.erase(files.begin() + i);
			i--;
		} else 
			m_totalbytes += files[i].size;
	}

	for(int i = 0; i < files.size(); i++)
	{
		currfile = i;
		sprintf(utxt, "Current file: %s (%d / %d)", files[i].name.c_str(), i+1, files.size());
		launcher.SetSubStatus(utxt);
		launcher.SetSubStatusProgress(0);

		char zip[1024];
		ZeroMemory(zip, sizeof(zip));
		strcpy(zip, files[i].name.c_str());
		strcat(zip, ".zip");

		DownloadFile(zip);
		if(error != "")
			return;

		HZIP hz = OpenZip(zip, NULL);
		if(hz)
		{
			StrTok tokens(new CStrTok(zip, "\\"));
			vector<string> dirtokens;
			while(char * token = tokens->pGet())
			{
				dirtokens.push_back(token);
			}

			string dir = "";
			for(int i = 0; i < dirtokens.size() - 1; i++)
			{
				dir.append( dirtokens[i] );
				dir.append("\\");
			}

			ZIPENTRY ze;
			GetZipItem(hz,-1,&ze);
			int numitems=ze.index;
			// -1 gives overall information about the zipfile

			for (int zi=0; zi<numitems; zi++)
			{ 
				ZIPENTRY ze; GetZipItem(hz,zi,&ze); // fetch individual details
				dir.append(ze.name);
				UnzipItem(hz, zi, dir.c_str());         // e.g. the item's name.
			}
			CloseZip(hz);

			remove( zip );
		}

		m_bytesdone += files[i].size;

		float prog = 1000.0 * ((float)m_bytesdone / (float)m_totalbytes);
		launcher.SetStatusProgress((int)prog);
	}
}
bool Updater::DownloadFile(const char * file)
{
	int ret;
	char * filename = new char[strlen(file) + 10];
	strcpy(filename, file);

	StrTok tokens(new CStrTok(filename, "\\"));
	vector<string> dirtokens;
	while(char * token = tokens->pGet())
	{
		dirtokens.push_back(token);
	}

	string dir = "/";
	for(int i = 0; i < dirtokens.size() - 1; i++)
	{
		dir.append( dirtokens[i] );
		dir.append( "/" );
	}

	if(dir != currDir)
	{
		currDir = dir;
		ftp.ChDir(currDir.c_str());
		ftp.GetDirInfo();
	}

	CUT_DIRINFO dirinfo;
	for(int i = 0; i < ftp.GetDirInfoCount(); i++)
	{
		ftp.GetDirEntry(i, &dirinfo);
		if(strcmp(dirtokens[dirtokens.size()-1].c_str(), dirinfo.fileName) == 0)
		{
			currfilesize = dirinfo.fileSize;
			break;
		}
	}

	ret = ftp.ReceiveFile( dirtokens[dirtokens.size()-1].c_str(), filename);

	if(ret != UTE_SUCCESS)
	{
		error = CUT_ERR::GetErrorString (ret);
		return false;
	}
	
	return true;
}

void Updater::CreateDirs()
{
	if(GetFileAttributesA("SPRITES\\") == -1)
	{
		::CreateDirectoryA("SPRITES\\",NULL);
	}
	if(GetFileAttributesA("MUSIC\\") == -1)
	{
		::CreateDirectoryA("MUSIC\\",NULL);
	}
	if(GetFileAttributesA("MAPDATA\\") == -1)
	{
		::CreateDirectoryA("MAPDATA\\",NULL);
	}
	if(GetFileAttributesA("SOUNDS\\") == -1)
	{
		::CreateDirectoryA("SOUNDS\\",NULL);
	}
	if(GetFileAttributesA("CONTENTS\\") == -1)
	{
		::CreateDirectoryA("CONTENTS\\",NULL);
	}
	if(GetFileAttributesA("CONTENTS\\friends\\") == -1)
	{
		::CreateDirectoryA("CONTENTS\\friends\\",NULL);
	}
	if(GetFileAttributesA("CONTENTS\\Shop\\") == -1)
	{
		::CreateDirectoryA("CONTENTS\\Shop\\",NULL);
	}
	if(GetFileAttributesA("CONTENTS\\mutes") == -1)
	{
		::CreateDirectoryA("CONTENTS\\mutes",NULL);
	}
}
