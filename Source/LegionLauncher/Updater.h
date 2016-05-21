#ifndef UPDATER_H
#define UPDATER_H
#pragma once

#include "ftp_c.h"

using namespace std;

static bool bUpdating = false;

class Updater
{
	struct File
	{
		string name;
		string hash;
		DWORD size;
		bool uptodate;
	};

	public:		
		Updater();
		~Updater();

		void Run();
		bool ProcessPatchFile();
		void CheckFiles();
		bool UpdateSelf();
		void UpdateFiles();

		bool DownloadFile(const char * file);
		void CreateDirs();

		CUT_FTPClient ftp;
		string error;
		string currDir;

		bool isrunning;
		bool isupdated;
		string updaterver;

		vector<File> files;
		DWORD currfile;
		DWORD currfilesize;

		DWORD m_totalbytes;
		DWORD m_bytesdone;
};

#endif