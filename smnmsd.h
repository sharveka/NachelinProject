#ifndef _DATASERVER_H_
#define _DATASERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/uio.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <setjmp.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
#include <memory.h>
#include <sys/wait.h>

#define _LP_DEBUG
#define _PUD_DEBUG
#define _PGD_DEBUG
#define _SC_DEBUG
#define _SD_DEBUG
#define _DD_DEBUG
#define _MD_DEBUG

#define MAX_PROCESS			2048
#define MAXINTERFACECOUNT	20
#define INET_ADDRSTRLEN		16
#define BUFSIZE			1024	
#define SQLMAXCOUNT			50
#define SQLMAXLENGTH		4096
#define MAXPENDING			100		// 연결요청을 대기할 수 있는 최대수

////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct _ProcessInfo
{
	pid_t pid;
	int pfd;
	int pfd_2;
	int pst;
	int ctrl_sd;
	int used;
}ProcessInfo; 

typedef struct _HEADER
{
	unsigned char	TYPE;
	unsigned int	OID;
}HEADER;


typedef struct _REGISTER1
{
	char	ID[45];
	char	NAME[45];
	char	PHONE[45];
}REG_USER;

typedef struct _REGISTER2
{
	char	CATEGORY[45];
	char	REST[45];
	char	MENU[45];
	unsigned int	PRICE;
	unsigned short	RATE;
	unsigned short	OPEN;
	char	CMNT[256];
}REG_DATA;

typedef struct _SELECTITEM
{
	unsigned int GID;
	char MENU[45];
	unsigned int	PRICE;
	unsigned short	RATE;
}SELECT_ITEM;

extern char gs_mysql_ipaddr[32];
extern char gs_mysql_db[32];
extern char gs_mysql_user[32];
extern char gs_mysql_pw[32];

extern char gs_comment[1024];

#endif
