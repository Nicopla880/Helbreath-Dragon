//
//  SEE.H (SEE.H2) Version 7.4
//
//  For WIN64 and WIN32 compilers (supporting the "declspec" keyword)
//

#ifdef STATIC_LIBRARY
   #define DLL_IMPORT_EXPORT
#else
   #ifdef DLL_SOURCE_CODE
      #define DLL_IMPORT_EXPORT __declspec(dllexport) __stdcall
   #else
      #define DLL_IMPORT_EXPORT __declspec(dllimport) __stdcall
   #endif
#endif

#ifdef __cplusplus
  #define NoMangle extern "C"
#else
  #define NoMangle
#endif

#define SEE_MIN_RESPONSE_WAIT   1
#define SEE_MAX_RESPONSE_WAIT   2
#define SEE_CONNECT_WAIT        3
#define SEE_DISABLE_MIME        4
#define SEE_QUOTED_PRINTABLE    8
#define SEE_AUTO_CALL_DRIVER    9
#define SEE_FILE_PREFIX        10
#define SEE_SLEEP_TIME         13
#define SEE_DECODE_UNNAMED     14
#define SEE_SMTP_PORT          15
#define SEE_POP3_PORT          16
#define SEE_MAX_LINE_LENGTH    17
#define SEE_BLOCKING_MODE      18
#define SEE_ALLOW_8BITS        19
#define SEE_LOG_FILE           20
#define SEE_HIDE_SAVED_MSG     21
#define SEE_HIDE_TO_ADDR       22
#define SEE_ADDRESS_DELIMITER  23
#define SEE_WSACLEANUP         24
#define SEE_PATH_DELIMITER     25
#define SEE_ATTACH_DELIMITER   26
#define SEE_ENABLE_IMAGE       27
#define SEE_RAW_MODE           28
#define SEE_ENABLE_ESMTP       29
#define SEE_ENABLE_APOP        30
#define SEE_ATTACH_BASE_NUMBER 31
#define SEE_IGNORE_REJECTED    32
#define SEE_WRITE_CONTENT_TYPE 33
#define SEE_SET_FILE_PREFIX    34
#define SEE_HTML_CHARSET       35
#define SEE_HIDE_HEADERS       36
#define SEE_KEEP_RFC822_INTACT 37
#define SEE_IMAP_PORT          38
#define SEE_EXPUNGE_ON_CLOSE   39
#define SEE_GUT_ATTACHMENTS    40
#define SEE_ALLOW_PARTIAL      41

#define CHARSET_BLANK        0
#define CHARSET_US_ASCII     1
#define CHARSET_8859         4
#define CHARSET_ISO_8859_1   4
#define CHARSET_ISO_8859_8   5
#define CHARSET_WIN_1252     6
#define CHARSET_WIN_1255     7
#define CHARSET_UTF8         8
#define CHARSET_ISO_8859_2  10
#define CHARSET_ISO_8859_7  11
#define CHARSET_WIN_1250    12
#define CHARSET_ISO_8859_3  13
#define CHARSET_ISO_8859_4  14

#define SEE_GET_ERROR_TEXT     1
#define SEE_GET_COUNTER        2
#define SEE_GET_RESPONSE       3
#define SEE_GET_SOCK_ERROR     4

#define SEE_GET_MESSAGE_BYTES_READ  10
#define SEE_GET_ATTACH_BYTES_READ   11
#define SEE_GET_TOTAL_BYTES_READ    12
#define SEE_GET_MESSAGE_BYTES_SENT  13
#define SEE_GET_ATTACH_BYTES_SENT   14
#define SEE_GET_TOTAL_BYTES_SENT    15
#define SEE_GET_VERSION             16

#define SEE_GET_MSG_COUNT           17
#define SEE_GET_MSG_SIZE            18
#define SEE_GET_BUFFER_COUNT        19

#define SEE_GET_CONNECT_STATUS      20
#define SEE_GET_REGISTRATION        21
#define SEE_GET_ATTACH_COUNT        22
#define SEE_GET_LAST_RESPONSE       23
#define SEE_GET_VERIFY_STATUS       24
#define SEE_GET_SERVER_IP           25
#define SEE_GET_BUILD               26
#define SEE_GET_SOCKET              27
#define SEE_GET_LOCAL_IP            28
#define SEE_GET_ATTACH_NAMES        29
#define SEE_GET_LAST_RECIPIENT      30
#define SEE_GET_AUTH_PROTOCOLS      31
#define SEE_GET_ATTACH_TYPES        32
#define SEE_GET_ATTACH_IDS          33
#define SEE_GET_UIDVALIDITY         34

#define SEE_COPY_BUFFER             40
#define SEE_WRITE_BUFFER            41

#define SEE_SET_REPLY               50
#define SEE_SET_HEADER              51
#define SEE_WRITE_TO_LOG            52
#define SEE_SET_FROM                53
#define SEE_SET_CONTENT_TYPE        54
#define SEE_SET_TRANSFER_ENCODING   55
#define SEE_ADD_HEADER              56
#define SEE_SET_SECRET              57
#define SEE_SET_USER                58
#define SEE_SET_TEXT_MESSAGE        59
#define SEE_FORCE_INLINE            60
#define SEE_SET_ATTACH_CONTENT_TYPE 61
#define SEE_AUTHENTICATE_PROTOCOL   62
#define SEE_SET_CONTENT_TYPE_PREFIX 63
#define SEE_ENABLE_XMAILER          64
#define SEE_SET_DEFAULT_ZONE        65
#define SEE_SET_ATTACH_DESCRIPTION  66
#define SEE_REPLACE_WITH_COMMAS     67
#define SEE_SET_MULTIPART           68
#define SEE_SET_TIME_ZONE           69
#define SEE_SET_RAWFILE_PREFIX      70
#define SEE_SET_IMAP_LIST_ARG       71
#define SEE_SET_FOOTER              72
#define SEE_SET_LOCAL_IP            73
#define SEE_SET_HELO_STRING         74
#define SEE_SET_CONNECT_ATTEMPTS    75
#define SEE_REPLACE_UNDERSCORES     76

#define QUOTED_OFF          0
#define QUOTED_PLAIN        1
#define QUOTED_HTML         2
#define QUOTED_RICH         3
#define QUOTED_8859         4
#define QUOTED_ISO_8859_1   4
#define QUOTED_ISO_8859_8   5
#define QUOTED_WIN_1252     6
#define QUOTED_WIN_1255     7
#define QUOTED_USER         9
#define QUOTED_ISO_8859_2  10
#define QUOTED_ISO_8859_7  11
#define QUOTED_WIN_1250    12
#define QUOTED_ISO_8859_3  13
#define QUOTED_ISO_8859_4  14
#define QUOTED_UTF8        15

#define INLINE_TEXT_OFF         0
#define INLINE_TEXT_INLINE      1
#define INLINE_TEXT_ATTACHMENT  2

#define AUTHENTICATE_CRAM    1
#define AUTHENTICATE_LOGIN   2
#define AUTHENTICATE_PLAIN   4

#define CONTENT_TYPE_DEFAULT       0
#define CONTENT_TYPE_MIXED         1
#define CONTENT_TYPE_ALTERNATIVE   2
#define CONTENT_TYPE_RELATED       3

#define BYTE  unsigned char

#define IMAP_FLAG_SEEN         1
#define IMAP_FLAG_ANSWERED     2
#define IMAP_FLAG_FLAGGED      4
#define IMAP_FLAG_DELETED      8
#define IMAP_FLAG_DRAFT       16
#define IMAP_FLAG_RECENT      32

#define IMAP_GET_FLAGS         1
#define IMAP_SET_FLAGS         2
#define IMAP_DEL_FLAGS         3

#define IMAP_SEARCH_MSG_COUNT  1
#define IMAP_SEARCH_FIRST_MSG  2
#define IMAP_SEARCH_NEXT_MSG   3

#define STUNNEL_TASKBAR_ICON    0x01
#define STUNNEL_DISABLE_LOGGING 0x02

#define SEE_GET_DATE           1
#define SEE_GET_FROM           2
#define SEE_GET_REPLY_TO       3
#define SEE_GET_SUBJECT        4
#define SEE_GET_TO             5

#define SSL_CONFIG_OPTIONS     1

NoMangle int  DLL_IMPORT_EXPORT  seeAbort(int);
NoMangle int  DLL_IMPORT_EXPORT  seeAttach(int, unsigned);
NoMangle int  DLL_IMPORT_EXPORT  seeAttachmentParams(char *,char *,char *,char *);
NoMangle int  DLL_IMPORT_EXPORT  seeByteToShort(char *);
NoMangle int  DLL_IMPORT_EXPORT  seeClose(int);
NoMangle int  DLL_IMPORT_EXPORT  seeCommand(int,char *);
NoMangle int  DLL_IMPORT_EXPORT  seeDebug(int,int,char *,int);
NoMangle int  DLL_IMPORT_EXPORT  seeDecodeBuffer(char *,char *,int);
NoMangle int  DLL_IMPORT_EXPORT  seeDecodeUTF8(char *,char *);
NoMangle int  DLL_IMPORT_EXPORT  seeDecodeUU(char *, char *);
NoMangle int  DLL_IMPORT_EXPORT  seeDeleteEmail(int,int);
NoMangle int  DLL_IMPORT_EXPORT  seeDriver(int);
NoMangle int  DLL_IMPORT_EXPORT  seeEncodeBuffer(char *,char *,int);
NoMangle int  DLL_IMPORT_EXPORT  seeEncodeUTF8(int,char *);
NoMangle int  DLL_IMPORT_EXPORT  seeErrorText(int,int,char *,int);
NoMangle int  DLL_IMPORT_EXPORT  seeExtractLine(char *,int,char *,int);
NoMangle int  DLL_IMPORT_EXPORT  seeExtractText(char *,char *,char *,int);
NoMangle int  DLL_IMPORT_EXPORT  seeForwardEmail(int,char *,char *,char *,char *,char *,char *);
NoMangle int  DLL_IMPORT_EXPORT  seeGetEmailCount(int);
NoMangle int  DLL_IMPORT_EXPORT  seeGetEmailFile(int,int,char *,char *,char *);
NoMangle int  DLL_IMPORT_EXPORT  seeGetEmailLines(int,int,int,char *,int);
NoMangle unsigned DLL_IMPORT_EXPORT  seeGetEmailSize(int,int);
NoMangle int  DLL_IMPORT_EXPORT  seeGetEmailUID(int,int,char *,int);
NoMangle unsigned DLL_IMPORT_EXPORT seeGetTicks(int);
NoMangle int  DLL_IMPORT_EXPORT  seeImapConnect(int,char *,char *,char *);
NoMangle int  DLL_IMPORT_EXPORT  seeImapCopyMBmail(int,int,char *);
NoMangle int  DLL_IMPORT_EXPORT  seeImapCreateMB(int,char *);
NoMangle int  DLL_IMPORT_EXPORT  seeImapDeleteMB(int,char *);
NoMangle int  DLL_IMPORT_EXPORT  seeImapFlags(int,int,int,int);
NoMangle int  DLL_IMPORT_EXPORT  seeImapListMB(int,char *,int);
NoMangle int  DLL_IMPORT_EXPORT  seeImapMsgNumber(int,int);
NoMangle int  DLL_IMPORT_EXPORT  seeImapRenameMB(int,char *,char *);
NoMangle int  DLL_IMPORT_EXPORT  seeImapSearch(int,char *,char *,int);
NoMangle int  DLL_IMPORT_EXPORT  seeImapSelectMB(int,char *);
NoMangle int  DLL_IMPORT_EXPORT  seeImapSource(int,char *);
NoMangle int  DLL_IMPORT_EXPORT  seeIntegerParam(int,int,unsigned);
NoMangle int  DLL_IMPORT_EXPORT  seeKillProgram(unsigned,int);
NoMangle int  DLL_IMPORT_EXPORT  seePop3Connect(int,char *,char *,char *);
NoMangle int  DLL_IMPORT_EXPORT  seePop3Source(int,char *);
NoMangle int  DLL_IMPORT_EXPORT  seeQuoteBuffer(char *,char *,int);
NoMangle int  DLL_IMPORT_EXPORT  seeReadQuoted(char *,char *,int,int);
NoMangle int  DLL_IMPORT_EXPORT  seeRelease(void);
NoMangle int  DLL_IMPORT_EXPORT  seeSendEmail(int,char *,char *,char *,char *,char *,char *);
NoMangle int  DLL_IMPORT_EXPORT  seeSendHTML(int,char *,char *,char *,char *,char *,char *,char *,char *);
NoMangle int  DLL_IMPORT_EXPORT  seeSetErrorText(int, char *);
NoMangle int  DLL_IMPORT_EXPORT  seeShortToByte(char *);
NoMangle int  DLL_IMPORT_EXPORT  seeSleep(int);
NoMangle int  DLL_IMPORT_EXPORT  seeSmtpConnect(int,char *,char *,char *);
NoMangle int  DLL_IMPORT_EXPORT  seeSmtpTarget(int,char *,char *,char *);
NoMangle unsigned DLL_IMPORT_EXPORT seeStartProgram(char *);
NoMangle unsigned DLL_IMPORT_EXPORT seeStatistics(int,int);
NoMangle int  DLL_IMPORT_EXPORT  seeStringParam(int,int,char *);
NoMangle int  DLL_IMPORT_EXPORT  seeTestFileSet(int, char *,char *,int);
NoMangle int  DLL_IMPORT_EXPORT  seeVerifyFormat(char *);
NoMangle int  DLL_IMPORT_EXPORT  seeVerifyUser(int,char *);
NoMangle int  DLL_IMPORT_EXPORT  seeIsConnected(int);
NoMangle int  DLL_IMPORT_EXPORT  seeSetProxySSL(int,int,char *,char *,char *);              
NoMangle int  DLL_IMPORT_EXPORT  seeConfigSSL(int,char *);
NoMangle int  DLL_IMPORT_EXPORT  seeImapConnectSSL(int,int,int,char *,char *,char *,char *); 
NoMangle int  DLL_IMPORT_EXPORT  seePop3ConnectSSL(int,int,int,char *,char *,char *,char *);
NoMangle int  DLL_IMPORT_EXPORT  seeSmtpConnectSSL(int,int,int,char *,char *,char *,char *,char *,char *);
NoMangle int  DLL_IMPORT_EXPORT  seeGetHeader(int, int, char *, int);
NoMangle int  DLL_IMPORT_EXPORT  seeUnQuoteBuffer(char *,char *,int);
NoMangle int  DLL_IMPORT_EXPORT  seeMakeSubject(char *,int,char *,int);
NoMangle int  DLL_IMPORT_EXPORT  seeTestConnect(int,char *,int,char *,int);
