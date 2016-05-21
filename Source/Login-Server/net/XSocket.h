
#if !defined(AFX_XSOCKET_H__F9D1BA42_7338_11D2_A8E6_00001C7030A6__INCLUDED_)
#define AFX_XSOCKET_H__F9D1BA42_7338_11D2_A8E6_00001C7030A6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define _WINSOCK2API_

#include <windows.h>
#include <windowsx.h>
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <malloc.h>
#include "..\main.h"
#include "..\defs.hpp"

#define XSOCK_LISTENSOCK			1
#define XSOCK_NORMALSOCK			2				
#define XSOCK_SHUTDOWNEDSOCK		3				

#define XSOCKSTATUS_READINGHEADER	        11
#define XSOCKSTATUS_READINGBODY		12

#define XSOCKEVENT_SOCKETMISMATCH		-121
#define XSOCKEVENT_CONNECTIONESTABLISH	-122
#define XSOCKEVENT_RETRYINGCONNECTION	-123
#define XSOCKEVENT_ONREAD			-124
#define XSOCKEVENT_READCOMPLETE		-125
#define XSOCKEVENT_UNKNOWN			-126
#define XSOCKEVENT_SOCKETCLOSED		-127
#define XSOCKEVENT_BLOCK			-128
#define XSOCKEVENT_SOCKETERROR		-129
#define XSOCKEVENT_CRITICALERROR		-130
#define XSOCKEVENT_NOTINITIALIZED		-131
#define XSOCKEVENT_MSGSIZETOOLARGE		-132
#define XSOCKEVENT_CONFIRMCODENOTMATCH	-133
#define XSOCKEVENT_QUENEFULL                -134
#define XSOCKEVENT_UNSENTDATASENDBLOCK	-135
#define XSOCKEVENT_UNSENTDATASENDCOMPLETE	-136

#define XSOCKBLOCKLIMIT			500
#define CLIENTSOCKBLOCKLIMIT                15
#define MSGBUFFERSIZE                       50000
#define MSGCLIENTBUFFERSIZE                 30000

class XSocket  
{
public:						 
	int iGetPeerAddress(char * pAddrString);
	char * pGetRcvDataPointer(DWORD * pMsgSize, char * pKey = NULL);
	SOCKET iGetSocket();
	BOOL bAccept(class XSocket * pXSock, unsigned int uiMsg);
	BOOL bListen(char * pAddr, int iPort, unsigned int uiMsg);
	int iSendMsg(char * cData, DWORD dwSize, char cKey = NULL, BOOL log = NULL);
	BOOL bConnect(char * pAddr, int iPort, unsigned int uiMsg);
	int  iOnSocketEvent(WPARAM wParam, LPARAM lParam);
	BOOL bInitBufferSize(DWORD dwBufferSize);
	XSocket(HWND hWnd, int iBlockLimit);
	virtual ~XSocket();

	int  m_WSAErr;
	BOOL m_bIsAvailable, IsRegistered;

    sBYTE   GSID, GSSocketID;
	
private:
	void _CloseConn();
	
	int _iSendUnsentData();
	int _iRegisterUnsentData(char * cData, int iSize);
	int _iSend(char * cData, int iSize, BOOL bSaveFlag);
	int _iSend_ForInternalUse(char * cData, int iSize);
	int _iOnRead();
	
	char   m_cType;
	char * m_pRcvBuffer;
	char * m_pSndBuffer;
	DWORD  m_dwBufferSize;
	
	SOCKET m_Sock;
	char   m_cStatus;
	DWORD  m_dwReadSize;
	DWORD  m_dwTotalReadSize;
	char   m_pAddr[30];
	int    m_iPortNum;

	char * m_pUnsentDataList[XSOCKBLOCKLIMIT];
	int    m_iUnsentDataSize[XSOCKBLOCKLIMIT];
	short  m_sHead, m_sTail;

	unsigned int m_uiMsg;
	HWND         m_hWnd;

	int			 m_iBlockLimit;
};


BOOL _InitWinsock();
void _TermWinsock();

#endif // !defined(AFX_XSOCKET_H__F9D1BA42_7338_11D2_A8E6_00001C7030A6__INCLUDED_)
