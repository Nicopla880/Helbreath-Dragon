#ifndef GLOBALDEF_H
#define GLOBALDEF_H

#define MSG_HOMEPAGE		"http://www.helbreathint.com/"

#define SELECTSERVER		// Show/Hides server selection screen.
//#define DNSSERVER			// uses dns adress

#define NAME_WORLDNAME1		"Abaddon" // Name for default (1st) server
#define MSG_WORLDNAME1		"180/200 Low-Rate Server" // Long name for 1st server...
#define SERVER_DNS			"HBInternational.ddns.net"
#define SERVER_PORT			7070
#define SERVER_IP			"192.168.1.151"
//#define SERVER_IP			"72.74.19.224"

#define NAME_WORLDNAME2		"WS2"
#define MSG_WORLDNAME2		"High-Rate Server"
#define WS2_SERVER_IP		"127.0.0.1"
#define WS2_SERVER_PORT		6060

#define LANG	ENGLISH
//#define LANG	SPANISH

#ifdef _DEBUG
#define SPEEDHAX_RUN		1.5
#define SPEEDHAX_ATK		2
#define SPEEDHAX_HIT		3
#define LOGINCFG
#define _SCDEBUG
#define WINDOWED_MODE		// Shows HB in a windowed mode (for debug purpose only, sprite will bug....)
#else
#define SPEEDHAX_RUN		1
#define SPEEDHAX_ATK		1
#define SPEEDHAX_HIT		1
#endif

/*** Some more compilation options ***/
//#define USING_WIN_IME		// Used for unicode characters (unchecked)
#define MAKE_ACCOUNT		// Enable acc creation. Missing sprites, fixed!
#define COMMA_GOLD			// Allows to show commas number (ie: 1,200,000)

//#define DEF_HTMLCOMMOM
//#define  _DEBUG			// Multi-Client, Instant-logout, Version check, Badwords,
// Mics debug msgs, CtrlQ for admins, Show status, Cursor position

#endif