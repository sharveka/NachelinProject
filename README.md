# NachelinProject

struct tm
{
    int tm_sec;   // seconds after the minute - [0, 60] including leap second
    int tm_min;   // minutes after the hour - [0, 59]
    int tm_hour;  // hours since midnight - [0, 23]
    int tm_mday;  // day of the month - [1, 31]
    int tm_mon;   // months since January - [0, 11]
    int tm_year;  // years since 1900
    int tm_wday;  // days since Sunday - [0, 6]
    int tm_yday;  // days since January 1 - [0, 365]
    int tm_isdst; // daylight savings time flag
};

--------------------------------------------------------------------------------

int mkdir(const char *pathname, mode_t mode);
pathname (경로) 안에 디렉토리 생성.
mode : 파일 생성 마스크

r : 파일 읽기 권한
w : 파일 쓰기 권한
x : 파일 실행 권한
3자리씩 끊어서 분석

0755 -> rwxr-xr-x
rwx : 소유자에 대한 권한 -> 읽기, 쓰기, 실행 허용
r-x : 그룹에 대한 권한 -> 소유그룹에 속하는 사용자들은 읽기, 실행 허용
r-x : 모든 사용자들에 대한 권한

권한 변경하려면 chmod 755 test.txt

----------------------------------------------------------------------------------------------------

int socket(int domain, int type, int protocol) - 서버와 연결에 사용할 socket 을 생성하는 함수.
socket() creates an endpoint for communication and returns a descriptor.
해당 소켓을 가리키는 소켓 디스크립터를 반환. 파일 디스크립터와 비슷
-1 이 반환되면 소켓 생성 실패, 0 이상의 값이 나오면 socket descriptor 반환.

domain : 생성할 소켓이 통신할 때 사용할 프로토콜 체계를 설정.
AF_INET(IPv4 인터넷 프로토콜), AF_INET6(IPv6 인터넷 프로토콜), AF_UNIX...

type : 소켓이 데이터 전송할 때 사용하는 전송 타입 설정.
SOCK_STREAM(연결 지향형 소켓, TCP), SOCK_DGRAM(비연결 지향형 소켓, UDP), SOCK_RAW(사용자 정의) 등이 올수 있으며, 데이터의 전송 형태를 의미

protocol : 두 호스트 간 통신하는데 있어 특정 프로토콜을 지정.
IPPROTO_TCP(TCP기반 연결 지항형 소켓), IPPROTO_UDP(UDP기반 비연결 지향형 소켓),  보통 0의 값을 사용한다.

여기서는 서버와의 통신을 위해 TCP(IPPROTO_TCP)라 불리는 스트림 기반의 프로토콜(SOCK_STREAM)을 이용하여 IPv4(AF_INET)환경에서 동작하도록 소켓을 생성.

-1이 반환되면 소켓 생성 실패, 0 이상의 값이 나오면 socket descriptor(해당 소켓을 가리킨다)반환한 것! (운영체제가 만든 소켓을 지칭하기 위해 부여한 숫자)


-----------------------------------------------------------------------------

출처 : https://jhnyang.tistory.com/262 , https://www.it-note.kr/122
https://www.joinc.co.kr/w/Site/Network_Programing/AdvancedComm/SocketOption

#include <sys.socket.h> / <winsock2.h>
int setsockopt(int sockfd, int level, int optname, void *optval, socklen_t optlen)
생성된 소켓의 속성값을 변경하는 함수. 성공하면 0, 실패하면 -1 반환

sockfd : 소켓의 파일 디스크립터.
level : optname 의 값이 socket level인지 특정 protocol에 대한 설정인지를 지정하는 값
-SOL_SOCKET : optname이 socket level에서 설정하는 option명임을 지정함.
IPPROTO_IP  : optname이 IP protocol level에서 설정하는 option명임을 지정함.
IPPROTO_TCP : optname이 TCP protocol level에서 설정하는 option명임을 지정함.

optname : 확인 또는 변경할 옵션의 이름 전달. level의 종류에 따라 다른 설정이름을 갖는다.

SO_REUSEADDR : 이미 사용된 주소를 재사용(bind)하도록 한다.
SO_SNDBUF : socket 에서 쓸(write) 수 있는 최대 buffer의 크기를 set (데이터 전송을 위한 버퍼공간 명시)
SO_RCVBUF : socket 에서 읽을 수 있는 최대 buffer의 크기를 set (데이터 수신을 위한 버퍼공간 명시)

optval : 설정값의 저장을 위한 버퍼의 포인터
optlen : optval 버퍼의 크기(길이)


-----------------------------------------------------------------------------
void bzero(void *ptr, size_t size)
메모리공간을 size 바이트만큼 0으로 채운다. - ptr 주소값부터 size 바이트만큼 0으로 채움.

void * memset(void *ptr, int value, size_t num) : 어떤 메모리의 시작점부터 연속된 범위를 어떤 값으로(byte단위) 모두 지정하고 싶을 때 사용하는 함수. = 특정 범위에 있는 연속된 메모리에 값을 지정할 때 사용. for문보다 빠르다.
-> 인자로 받은 서버 주소 정보를 저장.

ptr : 채우고자 하는 메모리의 시작 주소
value : 메모리에 채우고자 하는 값. int 형이지만 내부에서는 unsigned char 로 변환되어서 저장
num : 채우고자 하는 메모리의 크기

값 설정이 끝나면 포인터를 반환한다.


------------------------------------------------------------------------------

출처: https://techlog.gurucat.net/292

struct sockaddr <- 소켓의 주소를 담는 기본 구조체 
struct sockaddr { 
u_short sa_family; // address family, 2 bytes 
char sa_data[14]; // IP address + Port number, 14 bytes 
};

sockaddr_in : IPv4주소를 저장하는 구조체. sa_family 가 AF_INET인 경우에 사용하는 구조체이다.
struct sockaddr_in{
sa_family_t	sin_family;	//주소 체계를 저장하는 필드. ipv4를 위한 주소체계이므로 AF_INET
uint16_t	sin_port;	//포트 번호 저장
struct in_addr	sin_addr;	//IPv4 주소를 저장. in_addr 구조체.
char		sin_zero[8];	//sockaddr과 같은 크기를 유지하기 위해 필요한 패딩(padding) 공간. 8 bytes dummy data. 반드시 모두 0으로 채워야한다.

struct in_addr{
  uint32_t	s_addr;			32비트 ipv4주소
}
sin_addr : ipv4 주소를 저장하는 구조체 변수 -> 이 구조체 내부 변수 s_addr : 32비트 ipv4주소 저장.

과거 BSD소켓이 개발될 때, 향후 하나의 프로토콜 체계 안에서 여러 주소 체계가 사용될 수 있을 것이라고 예상.
sockaddr_in 타입을 정의할 때 주소체계를 독립적으로 설정할 수 있도록 sin_family라는 이름의 멤버를 넣어두었다.
따라서 반드시 sin_family의 설정을 해주어야 한다.


https://m.blog.naver.com/PostView.nhn?blogId=wlsgh31&logNo=220071545203&proxyReferer=https:%2F%2Fwww.google.com%2F

INADDR_ANY : 서버 소켓의 생성과정에서 매번 서버의 IP주소를 할당하기는 귀찮음. 
소켓의 IP주소를 이렇게 할당할 경우, 소켓이 동작하는 컴퓨터의 IP 주소가 자동으로 할당되기 때문에, IP주소를 직접 입력하는 수고를 덜 수 있다. 
컴퓨터 내에 두개 이상의 IP를 할당받아서 사용하는 경우에도, 할당 받은 IP 중 어떤 주소를 통해서 데이터가 들어오더라도 PORT번호만 일치하면 수신할 수 있게 된다. 따라서 서버 프로그램의 구현에 많이 선호되는 방법
소켓이 동작하는 컴퓨터의 Ip 주소가 자동으로 할당되기 때문에 Ip 주소를 직접 입력하는 수고를 덜 수 있다.

---------------------------------------------------------------------------------------------------
htons : host to network short
host시스템에서 network로 short형 데이터를 보낼 때 byte order를 바꿔주는 함수

네트워크 통신시 빅 엔디안(큰 단위부터 메모리에 적는다) 방식 사용.
연산을 많이하는 일반 PC는 리틀엔디안(작은 단위부터 메모리에 적는다) 방식 사용

host : client, server | network

big-endian : 0x00, 0xFF <-> little-endian : 0xFF, 0x00

---------------------------------------------------------------------------------------------------
bind() - Bind to the local address

int bind(socket s, const struct sockaddr *name, int namelen) : listen, connect 호출 전에 사용. "연결되지 않은 소켓"을 필요로 한다. 종단점(address, port)를 소켓과 결합
성공하면 0 , 실패하면 -1 , errno을 설정.

s : socket으로 생성된 소켓 객체
name : 소켓객체에 부여할 주소정보를 포함한 구조체
namelen : name의 데이터 길이, byte단위

성공하면 0리턴, 실패하면 SOCKET_ERROR(-1)리턴.


---------------------------------------------------------------------------------------------------

int listen(int sockfd, int backlog) : 클라이언트 접속 대기 (connect 대기)
성공하면 0 , 실패하면 -1 , errno을 설정.
backlog : 연결되지 않은 요청(연결 대기중)을 넣을 수 있는 큐의 최대 길이(연결 대기 제한 수, 연결 제한 수가 아니다)
백로그큐가 모두 찬 상태에서 새로운 연결을 시도하면, 클라이언트는 ECONNREFUSED 에러를 받게 된다.

---------------------------------------------------------------------------------------------------

int open(const char * pathname, int flags, mode_t mode);
이미 존재하는 파일을 열거나 새로운 파일을 생성하는 함수. 
파일을 성공적으로 열었다면 파일지정번호(파일 디스크립터)를 반환. 실패하면 -1 반환
pathname : 파일명. 
flags : 파일을 어떤 목적으로 open 할것인지, 초기화할지 여부 등을 지정. #include <fcntl.h>

O_RDWR : 파일 읽고 쓰기를 모두 할 수 있도록 OPEN 한다
O_CREAT : 파일이 존재하지 않으면 파일을 새로 만든다.

rw-r--r-- (644) : 유저는 읽고 쓰기, 그룹사용자와 일반사용자는 읽기만 가능

---------------------------------------------------------------------------------------------------

pid(process identifier) 운영체제에서 프로세스를 식별하기 위해 할당하ㅡㄴ 고유한 번호

getpid() / fork()

unix 는 프로세스 ID 라는 고유한 정수 값으로 프로세스를 구별한다.
각 프로세스는 자신을 생성한 부모 프로세스가 있으며, 자신을 생성한 부모 프로세스가 종료되면 시스템 프로세스가 새부모 프로세스(프로세스 id =1 )가 된다. -> 항상 부모프로세스가 존재.

pid_t getpid(void); 
-> 함수를 호출한 프로세스의 ID를 리턴. 
pid_t getppid(void);
-> 부모 프로세스 ID 를 리턴.

pid_t : 프로세스 ID, 부호가 없는 정수 타입.

pid_t fork() : 현재 실행되고 있는 프로세스를 복사해주는 함수.
현재 프로세스에 대해 자식 프로세스(복사본 프로세스)를 생성하는 함수. 
서버는 하나의 서비스 내에서 여러 개의 클라이언트를 받아내야 한다. 서버의 프로세스가 클라이언트의 요청을 받으면 자신이 응답하는 것이 아니라, 자식을 fork 하여 전달.
-> 서버 프로세스가 접속을 받을때마다 요청을 처리할 프로세스를 만들어 응답하는 것.

실패하면 -1 을 반환.
성공하면 부모 프로세스에게는 자식 프로세스의 pid 를, 자식 프로세스에는 0을 반환한다.
원본 프로세스가 부모, 복사된 프로세스가 자식이다.


--------------------------------------------------------------------------------

void *signal(int sig, void(*func)(int))(int);

singal : 어떤 이벤트(인터럽트, 타이머 종료 등)가 발생했을 때 운영체제가 프로그램에 이를 알리는 기법
software interrupt로, process에 무언가 발생했음을 알리는 간단한 메시지를 비동기적으로 보내는 것이다. signal을 받은 프로세스는 시그널에 따른 미리 지정된 기본 동작을 수행할 수도 있고, 사용자가 미리정의해 놓은 함수에 의해서 무시하거나, 특별한 처리를 할 수 있다.

예) 프로세스가 진행중에 Ctrl+c 를 누른다거나, 프로세스에 kill 명령이 내려지는 것이 발생했을 때 시그널이 발생.

시그널 함수의 종류 - kill, raise, abort, sigsend

현재 실행되고 있는 프로그램에 시그널이 전달되었을 시 4가지 상황이 발생
1. (운영체제에 의해)시그널이 무시된다. 프로세스는 시그널이 도착한 것을 알지 못한다.
2. 운영체제는 프로그램을 강제로 종료한다.
3. 프로그램 실행이 인터럽트 되면 이후에 프로그램이 지정한 시그널 처리 루틴이 실행
4. 시그널이 블로킹된다.(프로그램이 시그널을 허용할 때까지 아무런 영향을 미치지 못함.)
해당 프로세서에서 어떤 시그널이 블록되었는지 마스크를 갖고 있다.

종류      이벤트                            기본동작
SIGCHLD : 자식 프로세서가 종료됨             시그널 무시
SIGPIPE : 종료된 소켓에 쓰기를 시도할 때     프로그램 종료 


-----------------------------------------------------------------------------------




int lockf(int fd, int cmd, off_t len)
파일의 현재 위치에서 len 길이만큼 잠금, 또는 잠금 해제, 잠금 검사.
성공하면 0 실패하면 -1을 반환.

F_LOCK : 지정된 영역에 잠금을 설정.
F_ULOCK : 지정된 영역의 잠금을 해제한다.

한 프로세스가 파일 내용을 수정하는 동안 다른 프로세스가 파일을 읽거나, 
두 프로세스가 한 파일에 동시에 접근하여 데이터를 쓰는 경우 문제가 발생함. 

->
한 프로세스가 그 영역을 읽거나 수정할 때 다른 프로세스의 접근을 제한.
잠금된 영역에 한번에 하나의 프로세스만 접근
특히 레코드를 수정할 경우 대상 레코드에 대해 잠금을 해서

---------------------------------------------------------------------------
4. accept() - Wait for a client to connect -> block mode
TCP프로토콜을 이용하는 서버에서는 클라이언트의 연결을 수락해야 한다. ->

int accept(int sockfd, struct sockaddr *clientaddr, int *addrlen);

sockfd : 서버 소켓의 파일 디스크립터
addr : 연결 요청을 수락할 클라이언트의 주소 정보를 저장할 변수의 포인터
addrlen : addr이 가리키는 구조체의 크기를 저장하고 있는 변수의 포인터


client의 접속 요청을 받아들여 client와 연결.

성공하면 생성된 소켓의 파일 디스크립터를 반환.
실패하면 -1 반환.


----------------------------------------------------------------------------

char * inet_ntoa(struct in_addr addr)
네트워크 바이트 순서의 32비트 값을 dotted_decimal notation 주소값으로 변환

성공하면 변환된 해당 문자열의 포인터를 반환
실패하면 -1

------------------------------------------------------------------------------
5. recv() - Receive message from client

int recv(int sockfd, void*buff, size_t len, int flags);

sockfd : 소켓 디스크립터
void *buff : 수신할 버퍼 포인터
size_t len : 버퍼의 byte단위 길이
flags : 옵션. MSG_DONTWAIT : 수신 데이터가 없다면 기다리지 않고 -1을 반환하면서 바로 복귀
MSG_NOSIGNAL : 상대방과 연결이 끊겼을 때 SIGPIPE시그널을 받지 않도록

실패하면 -1반환, 성공하면 실제 수신한 바이트 수를 반환
