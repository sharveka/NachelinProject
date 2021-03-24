// smnmsd.cpp : Defines the entry point for the console application.
//
#include "sys/stat.h"
#include "mysql.h"
#include "smnmsd.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////

int gi_dsock;						// Connection prepare socket for service
int gi_lock_fd;						// Socket waiting lock file for service
int gi_server;						// 서비스용 서버 프로세스 갯수
int gi_port;						// 서비스용 포트 


char gs_log_file_name[64];
char gs_lock_file_name[64];

char gs_mysql_ipaddr[32];
char gs_mysql_db[32];
char gs_mysql_user[32];
char gs_mysql_pw[32];

int gi_bDead;
int* gi_pDead;

static ProcessInfo process[MAX_PROCESS];

char gs_comment[1024];

////////////////////////////////////////////////////////////////////////////////////////////////////////////
static int initializeProcessParam(char* argv[]);
static void lock_wait(int lock_fd);
static void lock_release(int lock_fd);
static int listen_socket(short portnum);
static void make_job_process(int i);
static void dead_processing(int signo);
static void main_exit();
static int dbCheck();


void Logging(char* cmt, int bPrint);
void job_process(int dsock, int lock_fd, int pronum);
void TCPClientProcess(int clientSock);

////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
	int i;

	if (argc != 3)
	{
		printf("Usage:%s [port] [process #]\n", argv[0]);
		return 1;
	}

	if (!initializeProcessParam(argv))	return 1;

	// 연결요청을 처리하는 소켓 생성
	gi_dsock = listen_socket(gi_port); // for service
	if (gi_dsock < 0) return 1;

	// 프로세스 대기를 위한 Lock 생성
	gi_lock_fd = open(gs_lock_file_name, O_RDWR | O_CREAT, 644);
	if(gi_lock_fd == -1)
	{
		sprintf(gs_comment, "[%d]Lock file generation failed![%s]", errno, gs_lock_file_name);
		Logging(gs_comment, 1);
		return 0;
	}

	sprintf(gs_comment, ">> Total_Server : %d, Server_Port : %d", gi_server, gi_port); 
	Logging(gs_comment, 1);
	Logging("---------------------------------------------------", 1);

	for(i = 0 ; i < gi_server ; i++)
	{
		make_job_process(i);
		process[i].used = 1;
	}
	Logging("---------------------------------------------------", 1); 

	if(signal(SIGINT, main_exit) == SIG_ERR)
		fprintf(stderr,"can't catch sigint\n");

	while(1)
	{
		pause();
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int listen_socket(short portnum)
{
	int sock;		//Socket descriptor 를 받을 변수
	struct sockaddr_in server;	//IPv4 주소를 저장하는 구조체.
	int len;
	int sock_opt;
	int buffer_size = 128 * 1024;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0)	//-1 이 반환되면 소켓 생성 실패 | 0 이상의 값이 나오면 socket descriptor 반환.
	{
		sprintf(gs_comment, "[%d]cannot create socket ...", portnum);	//gs_comment 배열에 2,3 번째 인자를 조합하여 넣기
		Logging(gs_comment, 1);
		return -1;
	}

	sock_opt=1;
	len = sizeof(int);
	if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&sock_opt, len) < 0)
	{
		sprintf(gs_comment, "[%d]setsockopt error...[%s]", portnum, strerror(errno));
		Logging(gs_comment, 1);
		return -1;
	}

	if (setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (char*)&buffer_size, sizeof(buffer_size)) < 0)
	{
		sprintf(gs_comment, "[%d]setsockopt error(SO_SNDBUF)...[%s]", portnum, strerror(errno));
		Logging(gs_comment, 1);
		return -1;
	}

	if (setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (char*)&buffer_size, sizeof(buffer_size)) <0)
	{
		sprintf(gs_comment, "[%d]setsockopt error(SO_RCVBUF)...[%s]", portnum, strerror(errno));
		Logging(gs_comment, 1);
		return -1;
	}

	bzero((void*)&server, sizeof(struct sockaddr_in));	//주소가 있는 server 변수, 그 변수 크기만큼 0으로 채운다. =초기화
	server.sin_family = AF_INET;		//주소는 AF_INET
	server.sin_addr.s_addr = INADDR_ANY;	//INADDR_ANY : 서버의 ip주소를 자동으로 찾아서 대입해주는 함수.
	server.sin_port = htons(portnum); 	//host to network short / integer, long, short 정수형만 가능

	//connect 호출 전에 사용, 연결되지 않은 소켓을 필요로 한다. 소켓과 (IP 주소+포트번호)를 결합
	if(bind(sock, (struct sockaddr *)&server, sizeof(server)) < 0)	//소켓객체에 부여할 주소정보를 포함한 구조체, 구조체의 데이터 길이(byte)
	{
		sprintf(gs_comment, "[%d]cannot bind socket ...[errno:%d]", portnum, errno);
		Logging(gs_comment, 1);
		return -1;
	}

	if (listen(sock, SOMAXCONN) < 0)
	{
		sprintf(gs_comment, "[%d]cannot listen socket ...[errno:%d]", portnum, errno);
		Logging(gs_comment, 1);
		return -1;
	}

	return sock;
}

static int initializeProcessParam(char* argv[])
{
	time_t t;
	struct tm* tm;
 
	t = time(NULL);		// 현재 시각을 초 단위로 얻기
	tm = localtime(&t);	// 초 단위의 시간을 분리하여 구조체에 넣기 

	mkdir("logs", 0755);
	mkdir("lock", 0755);

	sprintf(gs_log_file_name, "logs/%04d%02d%02d.log", tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday); 
	sprintf(gs_lock_file_name, "lock/lock%s_%d", argv[1], getpid()); 

	gi_port = (short)atoi(argv[1]);
	if (gi_port <= 0)
	{
		printf("Invalid Port!\n");
		return 0;
	}
	
	gi_server = (short)atoi(argv[2]);
	if (gi_server <= 0)	gi_server = 1;

	gi_bDead = 0;
	gi_pDead = &gi_bDead;
	// DB연결 체크 
	if (!dbCheck())	return 0;

	strcpy(gs_mysql_ipaddr, "localhost");
	strcpy(gs_mysql_db, "mydb");
	strcpy(gs_mysql_user, "Scott");
	strcpy(gs_mysql_pw, "tiger");

	return 1;
}

static void lock_wait(int lock_fd)
{
	int result;
	result = lockf(lock_fd, F_LOCK, sizeof(int));
	return ;
}

static void lock_release(int lock_fd)
{
	lockf(lock_fd, F_ULOCK, sizeof(int));
	return ;
}

void Logging(char* cmt, int bPrint)
{
	FILE* logfp;
	char timestr[32];

	struct tm *ptm;
	time_t long_time;

	time(&long_time);
	ptm = localtime(&long_time);

	sprintf(timestr, "%d-%02d-%02d %02d:%02d:%02d", ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);

	if (logfp = fopen(gs_log_file_name, "at"))
	{
		fprintf(logfp, "[%s]%s\n", timestr, cmt);
		fclose(logfp);
	}

	if (bPrint != 0) fprintf(stderr, "[%s]%s\n", timestr, cmt);
}

// 불의의 사고로 child process가 죽었을 경우
static void dead_processing(int signo)
{
	int i, pid;
	int statloc;

	if (*gi_pDead == 1)	return;

	// 죽은 process의 pid를 가져온다.
	pid = wait(&statloc);

	if (0 < pid)
	{
		if (WIFEXITED(statloc))
		{
			sprintf(gs_comment, "[%d]Normally terminated. Status values = %d", pid, WEXITSTATUS(statloc));
			Logging(gs_comment, 1);
		}
		if (WIFSIGNALED(statloc))
		{
			sprintf(gs_comment, "[%d]Abnormally terminated. Status values = %d", pid, WTERMSIG(statloc));
			Logging(gs_comment, 1);
		}
		if (WCOREDUMP(statloc))
		{
			sprintf(gs_comment, "[%d]Core dump terminated.", pid);
			Logging(gs_comment, 1);
		}
		if (WIFSTOPPED(statloc))
		{
			sprintf(gs_comment, "[%d]Stopped by signal %d.", pid, WSTOPSIG(statloc));
			Logging(gs_comment, 1);
		}
		
		for(i = 0 ; i <  MAX_PROCESS ; i++)
		{
			if(process[i].used && pid == process[i].pid)
			{
				make_job_process(i);	// process를 다시 생성한다.
				return;
			}
		}
	}
	else
	{
		sprintf(gs_comment, "[%d]Cannot find Child process PID ==> Create New Child Process", pid);
		Logging(gs_comment, 1);

		for(i = 0 ; i <  MAX_PROCESS ; i++)
		{
			if(process[i].used == 0)
			{
				make_job_process(i);	// process를 다시 생성한다.
				return;
			}
		}
	}
}

// 부모가 죽으면
static void main_exit()
{
	int i;

	gi_bDead = 1;

	Logging("terminate map service!", 1);

	close(gi_lock_fd);
	unlink(gs_lock_file_name);

	close(gi_dsock);

	for(i = 0 ; i < MAX_PROCESS ; i++)
	{
		if(process[i].used)
		{
			sprintf(gs_comment, "process %d is terminating ...", i);
			Logging(gs_comment, 1);
			kill(process[i].pid, SIGINT);
		}
	}

	exit(0);
}

// 자식을 만든다.
static void make_job_process(int i)
{ 
	pid_t pid;
	pid = fork();
	if(pid == 0) // Child Process
	{
		job_process(gi_dsock, gi_lock_fd, i);
		exit(0);
	}
	else		// parent Process
	{
		process[i].pid = pid;

		sprintf(gs_comment, "process %d with PID : %d is starting ...", i, pid);
		Logging(gs_comment, 1);

		fflush(stdout);
	} 
	return;
}
static int dbCheck()
{
	MYSQL mysql;

	mysql_init(&mysql);
	//if (!mysql_real_connect(&mysql, gs_mysql_ipaddr, gs_mysql_user, gs_mysql_pw, gs_mysql_db, 0, NULL, 0))
	if (!mysql_real_connect(&mysql, "localhost", "Scott", "tiger", "mydb", 0, NULL, 0))	
	{
		sprintf(gs_comment, "Couldn't connect to Database engine[%s]!!!", mysql_error(&mysql));
		Logging(gs_comment, 1);

		return 0;
	}

	mysql_close(&mysql);

	return 1;
}

// 생성되는 Process 
void job_process(int dsock, int lock_fd, int pronum)
{
	int i, datafd;
	int cli_len;
	struct sockaddr_in client;

	// client socket
	struct sockaddr_in *client_in;
	char *client_address; // 나중에 free하면 안됨

	cli_len = sizeof(client);

	while(1)
	{
// lock을 이용하여 대기한다. 한번에 한개의 프로세스만 accept Block Mode로 들어가도록 한다.
		
		lock_wait(lock_fd);		
		printf("stanby [%d]\n", getpid());

		// client의 접근을 기다린다.
		datafd = accept(dsock, (struct sockaddr *)&client, &cli_len);	//socket descriptor, client address, address가 가리키는 구조체의 크기를 저장하고 있는 변수의 포인터
// lock을 해제해 준다. 다른 프로세스가 accept Block Mode로 들어가도록 한다.
		lock_release(lock_fd);
		if(datafd < 0)	//accept 가 성공하면 > 0 | 실패하면 -1
		{
			// 사용자 socket이 뭔가 잘못 되었다.
			sprintf(gs_comment, "Socket accpet error...[%d]", errno);
			Logging(gs_comment, 1);
			close(datafd);
			continue;
		}

		client_in = (&client);
		client_address = inet_ntoa(client_in->sin_addr);

		sprintf(gs_comment, "%s", client_address);
		Logging(gs_comment, 1);

		TCPClientProcess(datafd);

	} // end of while
}
