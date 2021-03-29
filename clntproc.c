// smnmsd.cpp : Defines the entry point for the console application.
//

#include "mysql.h"
#include "sys/stat.h"
#include "smnmsd.h"
#include <string.h>

char message[1024];
char sqlstr[1024];

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Parse HEADER data - type(req/res protocol), oid(id auto incremented in db)
int ParsingDataHeader(char* rcv, HEADER* header)
{
	memcpy(&header->TYPE, rcv, 1);
	memcpy(&header->OID, rcv + 1, 4);	header->OID = ntohl(header->OID);
	printf("header->TYPE : %d\n", header->TYPE);
	printf("header->OID : %d\n", header->OID);

	return 5;
}

//header.TYPE == 1
// 사용자 식별 - 사용자 ID와 그 길이를 받아서 파싱
void ParsingID(char* rcv, char* IDname)
{
	short len;

	memcpy(&len, rcv, 2);	len = ntohs(len);
	memcpy(IDname, rcv + 2, len);
	IDname[len]='\0';

	printf("IDNAME(len) : %s(%d)\n", IDname, len);	//데이터가 잘 도착했는지 찍어보자
}

//header.TYPE == 1
//Client에서 넘어온 ID가 db에 존재하면 로그인 성공 | 존재하지 않으면 회원가입 요구
int loginProc(char* IDname) {

	MYSQL mysql;			//mysql을 사용하기 위한 변수
	mysql_init(&mysql);		//mysql 초기화
	

	if (!mysql_real_connect(&mysql, gs_mysql_ipaddr, gs_mysql_user, gs_mysql_pw, gs_mysql_db, 0, NULL, 0))
	{
		sprintf(message, "Failed to connect to database: Error[%s]", mysql_error(&mysql));
		return -1;
	}

	sprintf(sqlstr, "select oid from user where IDUser=\"%s\"", IDname);
	if (mysql_query(&mysql, sqlstr) != 0)	//mysql 질의
	{
		sprintf(message, "Query Error : %s", mysql_error(&mysql));
		mysql_close(&mysql);
		return -1;
	}

#ifdef _LP_DEBUG
	printf("sqlstr : %s\n", sqlstr);
#endif
	MYSQL_RES* res = mysql_store_result(&mysql);
	if (!res) {
		sprintf(message, "Not existed id[0]");
		mysql_close(&mysql);
		return -1;
	}

	MYSQL_ROW row = mysql_fetch_row(res);
	if (!row)
	{
		sprintf(message, "Not existed id[1].");
		mysql_free_result(res);
		mysql_close(&mysql);
		return -1;
	}
	int ret = atoi(row[0]);

	mysql_free_result(res);
	mysql_close(&mysql);
	return ret;
}

//header.TYPE == 2
//회원가입에 쓸 데이터를 파싱 - ID, 이름, 번호
void ParsingUserData(char* rcv, REG_USER* userdata)
{
	short len;
	char* t=rcv;	

	//ID
	memcpy(&len, t, 2);		t+=2;		len=ntohs(len);
	memcpy(userdata->ID, t, len);	t+=len;
	userdata->ID[len]='\0';	//문자열 끝에는 \0 이 붙는다.
#ifdef _PUD_DEBUG
	printf("ID(len) : %s(%d)\n", userdata->ID,len);
#endif

	//이름
	memcpy(&len, t, 2);		t+=2;		len=ntohs(len);
	memcpy(userdata->NAME, t, len); t+=len;
	userdata->NAME[len]='\0';
#ifdef _PUD_DEBUG
	printf("NAME(len) : %s(%d)\n", userdata->NAME,len);
#endif

	//번호
	memcpy(&len, t, 2); 		t+=2;		len=ntohs(len);
	memcpy(userdata->PHONE, t, len);t+=len;
	userdata->PHONE[len]='\0';
#ifdef _PUD_DEBUG
	printf("PHONE(len) : %s(%d)\n", userdata->PHONE, len);
#endif
}

//header.TYPE == 2
//사용자 회원가입
int InsertUserData(REG_USER* data)
{
	MYSQL mysql;
	mysql_init(&mysql);
	
	if (!mysql_real_connect(&mysql, gs_mysql_ipaddr, gs_mysql_user, gs_mysql_pw, gs_mysql_db, 0, NULL, 0))
	{
		sprintf(message, "Failed to connect to database: Error[%s]", mysql_error(&mysql));
		return -1;
	}
 	sprintf(sqlstr,
         "INSERT INTO USER (IDUser,UserName,PhoneNum) VALUES('%s','%s','%s');", data->ID, data->NAME, data->PHONE);
	if (mysql_query(&mysql, sqlstr) != 0)	//mysql 질의
	{
		sprintf(message, "Query Error : %s", mysql_error(&mysql));
		mysql_close(&mysql);
		return -1;
	}
	
	sprintf(sqlstr, "select oid from user where IDUser=\"%s\"", data->ID);
#ifdef _PUD_DEBUG
	printf("sqlstr : %s\n", sqlstr);
#endif
	if (mysql_query(&mysql, sqlstr) != 0)	//mysql 질의
	{
		sprintf(message, "Query Error : %s", mysql_error(&mysql));
		mysql_close(&mysql);
		return -1;
	}

	MYSQL_RES* res = mysql_store_result(&mysql);
	if (!res) {
		sprintf(message, "Not existed id[0]");
		mysql_close(&mysql);
		return -1;
	}

	MYSQL_ROW row = mysql_fetch_row(res);
	if (!row)
	{
		sprintf(message, "Not existed id[1].");
		mysql_free_result(res);
		mysql_close(&mysql);
		return -1;
	}
	int ret = atoi(row[0]);

	mysql_free_result(res);
	mysql_close(&mysql);
	return ret;
}

//header.TYPE == 3
//맛집 등록 정보 파싱 - 분류, 식당이름, 메뉴이름, 가격, 별점, 타인에게 공개여부, 후기
void ParsingGourmetData(char* rcv, REG_DATA* data){
	short len;
	int len2;	
	char* t=rcv;

	memcpy(&len, t, 2);		t+=2;		len=ntohs(len);
	memcpy(data->CATEGORY, t, len); t+=len;
	data->CATEGORY[len]='\0';
#ifdef _PGD_DEBUG
	printf("CATEGORY(len) : %s(%d)\n", data->CATEGORY,len);
#endif

	memcpy(&len, t, 2);		t+=2;		len=ntohs(len);
	memcpy(data->REST, t, len);	t+=len;
	data->REST[len]='\0';
#ifdef _PGD_DEBUG
	printf("REST(len) : %s(%d)\n", data->REST,len);
#endif

	memcpy(&len, t ,2);		t+=2;		len=ntohs(len);
	memcpy(data->MENU, t, len);	t+=len;
	data->MENU[len]='\0';
#ifdef _PGD_DEBUG
	printf("MENU(len) : %s(%d)\n", data->MENU,len);
#endif

	memcpy(&len2, t, 4);	t+=4;		data->PRICE=ntohl(len2);
	memcpy(&len, t, 2);	t+=2;		data->RATE=ntohs(len);
	memcpy(&len, t, 2);	t+=2;		data->OPEN=ntohs(len);
#ifdef _PGD_DEBUG
	printf("PRICE : %d\n", data->PRICE);
	printf("RATE : %d\n", data->RATE);
	printf("OPNE : %d\n", data->OPEN);	
#endif

	memcpy(&len, t, 2);	t+=2;		len=ntohs(len);
	memcpy(data->CMNT, t, len);
	data->CMNT[len]='\0';
#ifdef _PGD_DEBUG
	printf("CMNT(len) : %s(%d)\n", data->CMNT,len);
#endif
}

//header.TYPE == 3
//맛집 정보 db에 insert
int InsertGourmetData(unsigned int OID, REG_DATA* data){
	MYSQL mysql;
	mysql_init(&mysql);
	
	if (!mysql_real_connect(&mysql, gs_mysql_ipaddr, gs_mysql_user, gs_mysql_pw, gs_mysql_db, 0, NULL, 0))
	{
		sprintf(message, "Failed to connect to database: Error[%s]", mysql_error(&mysql));
		return -1;
	}
 	sprintf(sqlstr,
         "INSERT INTO GOURMET (Category, NameRest, NameMenu, Price, Rate, Open, Comment, OID) VALUES('%s','%s','%s',%d, %d, %d, '%s', %d);", data->CATEGORY, data->REST, data->MENU, data->PRICE, data->RATE, data->OPEN, data->CMNT, OID);

#ifdef _PGD_DEBUG
	printf("sqlstr : %s\n", sqlstr);
#endif
	if (mysql_query(&mysql, sqlstr) != 0)	//mysql 질의
	{
		sprintf(message, "Query Error : %s", mysql_error(&mysql));
		mysql_close(&mysql);
		return -1;
	}
	
	//mysql_free_result(res);
	mysql_close(&mysql);
	return 1;
}

//header.TYPE == 4
//데이터 분류별 목록 조회할 때 쓸 분류명을 파싱(Korean, Western...)
void ParsingCategory(char* rcv, char* category)
{
	short len;

	memcpy(&len, rcv, 2);	len = ntohs(len);
	memcpy(category, rcv + 2, len);
	category[len]='\0';
}

//header.TYPE == 4
//앱 화면에서 분류(Korean, Western...)를 클릭하면 해당 분류의 맛집 정보들을 조회해서 client에게 전송
int SelectCategory(int clientSock, char* category)
{
	MYSQL mysql;
	mysql_init(&mysql);
	
	if (!mysql_real_connect(&mysql, gs_mysql_ipaddr, gs_mysql_user, gs_mysql_pw, gs_mysql_db, 0, NULL, 0))
	{
		sprintf(message, "Failed to connect to database: Error[%s]", mysql_error(&mysql));
		return -1;
	}

  //gid, 메뉴이름, 가격, 별점만 조회해서 보여줌. 이후 조회된 데이터의 상세정보(식당이름, 후기 등)를 보고싶으면 상세정보를 다시 조회
 	sprintf(sqlstr,
         "select gid, namemenu, price, rate from gourmet where category=\"%s\";", category);

#ifdef _SC_DEBUG
	printf("sqlstr : %s\n", sqlstr);
#endif
	if (mysql_query(&mysql, sqlstr) != 0)	//mysql 질의
	{
		sprintf(message, "Query Error : [%s]", mysql_error(&mysql));
		mysql_close(&mysql);
		return -1;
	}
	
	MYSQL_RES *res=mysql_store_result(&mysql);
	if(!res)
	{
		sprintf(message, "store result error : [%s]", mysql_error(&mysql));
		mysql_close(&mysql);
		return -1;
	}
	
	//Client에게 보낼 데이터를 프로토콜에 맞게 잘 정리해보자
	
	//db에서 읽어들인 데이터의 행 개수
	int count=mysql_num_rows(res);
	
	//구조체 SELECT_ITEM 변수 선언 - gid, NameMENU, PRICE, RATE
	SELECT_ITEM* psi=(SELECT_ITEM*)malloc(sizeof(SELECT_ITEM)*count);
	
	int idx=0;
	MYSQL_ROW row;
	while(row=mysql_fetch_row(res))
	{	
		//varchar data -> int 변환
		psi[idx].GID=atoi(row[0]);		//gid
		strcpy(psi[idx].MENU, row[1]);		//MenuName
		psi[idx].PRICE=atoi(row[2]);		//price
		psi[idx].RATE=(short)atoi(row[3]);	//rate(별점)
		idx++;
	}
	
	mysql_free_result(res);
	mysql_close(&mysql);

	//1 : type(12), 1 : T/F, 4 : 보내는 데이터 개수
	int totallen=1+1+4;
	for(int i=0;i<count;i++)
	{
		totallen+=4;	//gid
		totallen+=2;	//menulen
		totallen+=strlen(psi[i].MENU);	//menu
		totallen+=4;	//price
		totallen+=2;	//rate
	}
	
	char* itembuf=(char*)malloc(totallen);
	itembuf[0]=0x12;	//protocol type 12 : '목록 조회 결과를 줄게' 라는 뜻
	itembuf[1]='T';		//client의 요청을 성공적으로 받아들였어요
	int nc=htonl(count);	//데이터 개수

	short sc;
	memcpy(itembuf+2, &nc, 4);	//memcpy - itembuf[0],[1]은 이미 배정, +2부터 넣는다

	char *t=itembuf+6;
	for(int i=0;i<count;i++)
	{
		//(unsigned int) gid
		nc=htonl(psi[i].GID);
		memcpy(t, &nc, 4);	t+=4;

		//menulen(unsigned short)
		sc=htons((short)strlen(psi[i].MENU));	
		memcpy(t, &sc, 2);	t+=2;
		
		//(byte) MenuName
		memcpy(t, psi[i].MENU, strlen(psi[i].MENU));	t+=strlen(psi[i].MENU);
		
		//(unsigned int) price
		nc=htonl(psi[i].PRICE);
		memcpy(t, &nc, 4);	t+=4;

		//(unsigned short)
		sc=htons(psi[i].RATE);
		memcpy(t, &sc, 2);	t+=2;
	}

	send(clientSock, itembuf, totallen, 0);

	free(itembuf);
	free(psi);

	return 1;
}

//header.TYPE == 5
//카테고리에서 봤던 데이터로는 모자라 -> 상세 데이터 더 가져와 - 맛집정보카드를 통째로 가져오자
int SelectDataCard(int clientSock, int gid)
{
	MYSQL mysql;
	mysql_init(&mysql);
	
	if (!mysql_real_connect(&mysql, gs_mysql_ipaddr, gs_mysql_user, gs_mysql_pw, gs_mysql_db, 0, NULL, 0))
	{
		sprintf(message, "Failed to connect to database: Error[%s]", mysql_error(&mysql));
		return -1;
	}
 	sprintf(sqlstr,
         "SELECT Category, namerest, namemenu, price, rate, open, comment from gourmet where gid=%d;", gid);

#ifdef _SD_DEBUG	//역시나 찍어보는 용도
	printf("sqlstr : %s\n", sqlstr);
#endif

	if (mysql_query(&mysql, sqlstr) != 0)	
	{
		sprintf(message, "Query Error : [%s]", mysql_error(&mysql));
		mysql_close(&mysql);
		return -1;
	}
	
	MYSQL_RES *res=mysql_store_result(&mysql);
	if(!res)
	{
		sprintf(message, "store result error : [%s]", mysql_error(&mysql));
		mysql_close(&mysql);
		return -1;
	}
	
	//client에게 보낼 데이터를 예쁘게 만들어보자
	REG_DATA data;
	
	MYSQL_ROW row=mysql_fetch_row(res);
	
	strcpy(data.CATEGORY, row[0]);
	strcpy(data.REST, row[1]);
	strcpy(data.MENU, row[2]);

	data.PRICE=atoi(row[3]);
	data.RATE=(short)atoi(row[4]);
	data.OPEN=(short)atoi(row[5]);
	strcpy(data.CMNT, row[6]);

	mysql_free_result(res);
	mysql_close(&mysql);
	

	int totallen=1+1;			//header
	totallen+=2;				//category len
	totallen+=strlen(data.CATEGORY);	//category
	totallen+=2;				//restaurant name len
	totallen+=strlen(data.REST);		//restaurant name
	totallen+=2;				//menuname len
	totallen+=strlen(data.MENU);		//menuname
	totallen+=4+2+2;			//price, rate, open
	totallen+=2;				//comment len
	totallen+=strlen(data.CMNT);		//comment

	char* cardbuf=(char*)malloc(totallen);
	cardbuf[0]=0x13;	//protocol type : 13 '상세조회결과 리턴'이라는 뜻	
	cardbuf[1]='T';		//client의 요청을 성공적으로 받아들였다
	
	short sc;
	int nc;
	char *t=cardbuf+2;

	//(unsigned short)CATELEN
	sc=htons((short)strlen(data.CATEGORY));
	memcpy(t, &sc, 2);	t+=2;
	
	//(byte)CATEGORY
	memcpy(t, data.CATEGORY, strlen(data.CATEGORY));	t+=strlen(data.CATEGORY);

	//(unsigned short)RESTLEN
	sc=htons((short)strlen(data.REST));
	memcpy(t, &sc, 2);	t+=2;
	
	//(byte)REST(restaurant)
	memcpy(t, data.REST, strlen(data.REST));	t+=strlen(data.REST);

	
	//(unsigned short)MENULEN
	sc=htons((short)strlen(data.MENU));
	memcpy(t, &sc, 2);	t+=2;
	
	//(byte)MENU
	memcpy(t, data.MENU, strlen(data.MENU));	t+=strlen(data.MENU);


	//(unsigned int)PRICE
	nc=htonl(data.PRICE);
	memcpy(t, &nc, 4);	t+=4;

	//(unsigned short)RATE
	sc=htons(data.RATE);
	memcpy(t, &sc, 2);	t+=2;

	//(unsigned short)OPEN
	sc=htons(data.OPEN);
	memcpy(t, &sc, 2);	t+=2;

	//(unsigned short)CMNTLEN
	sc=htons((short)strlen(data.CMNT));
	memcpy(t, &sc, 2);	t+=2;
	
	//(byte)CMNT(comment)
	memcpy(t, data.CMNT, strlen(data.CMNT));	t+=strlen(data.CMNT);

	send(clientSock, cardbuf, totallen, 0);
	free(cardbuf);

	return 1;
}

//header.TYPE == 6
//맛집 정보를 삭제
int DeleteDataCard(int clientSock, int gid)
{
	MYSQL mysql;
	mysql_init(&mysql);
	
	if (!mysql_real_connect(&mysql, gs_mysql_ipaddr, gs_mysql_user, gs_mysql_pw, gs_mysql_db, 0, NULL, 0))
	{
		sprintf(message, "Failed to connect to database: Error[%s]", mysql_error(&mysql));
		return -1;
	}
 	sprintf(sqlstr, "delete from gourmet where gid=%d;", gid);

#ifdef _DD_DEBUG	//역시나 찍어보는 용도
	printf("sqlstr : %s\n", sqlstr);
#endif

	if (mysql_query(&mysql, sqlstr) != 0)	
	{
		sprintf(message, "Query Error : [%s]", mysql_error(&mysql));
		mysql_close(&mysql);
		return -1;
	}
	
	return 1;
}

//header.TYPE == 7
//맛집 정보 수정
int ModifyDataCard(int clientSock, int gid, REG_DATA* data)
{
	MYSQL mysql;
	mysql_init(&mysql);
	
	if (!mysql_real_connect(&mysql, gs_mysql_ipaddr, gs_mysql_user, gs_mysql_pw, gs_mysql_db, 0, NULL, 0))
	{
		sprintf(message, "Failed to connect to database: Error[%s]", mysql_error(&mysql));
		return -1;
	}
	sprintf(sqlstr, "update gourmet set Category = \"%s\", NameRest = \"%s\", NameMenu = \"%s\", Price = %d, Rate = %d , Open = %d, Comment = \"%s\" where gid=%d;", data->CATEGORY, data->REST, data->MENU, data->PRICE, data->RATE, data->OPEN, data->CMNT, gid);
#ifdef _MD_DEBUG	//역시나 찍어보는 용도
	printf("sqlstr : %s\n", sqlstr);
#endif

	if (mysql_query(&mysql, sqlstr) != 0)	
	{
		sprintf(message, "Query Error : [%s]", mysql_error(&mysql));
		mysql_close(&mysql);
		return -1;
	}
	return 1;
}

int RegisterPhoto(int clientSock, HEADER header)
{
	int gid;
	short pcnt,cmntlen;
	char ptype;
	int plen;
	int rlen, tlen;
	char path[512];
	int newBufsize=BUFSIZE*100;
	char buffer[newBufsize];
	recv(clientSock, &gid, 4, 0);	gid=ntohl(gid);
	recv(clientSock, &pcnt, 2, 0);  pcnt=ntohs(pcnt);
	
	sprintf(path, "./photo/%d", header.OID);
	
	mkdir("./photo", 0755);
	mkdir(path, 0755);
	
	for(int i=0;i<pcnt;i++)
	{	
		tlen = 0;
printf("1\n");
		recv(clientSock, &plen, 4, 0);	plen=ntohl(plen);
printf("2\n");
		recv(clientSock, &ptype, 1, 0);	
		char fname[1024];
printf("3\n");
		if(ptype==1)	sprintf(fname, "./%s/%d-%d.PNG", path, gid, i+1);
		else		sprintf(fname, "./%s/%d-%d.JPG", path, gid, i+1);
printf("4\n");
				
		FILE *fp=fopen(fname, "wb");
		while(1){
printf("5-1\n");
			rlen=recv(clientSock, buffer, newBufsize, 0);
			tlen += rlen;
printf("5-2\n");
			fwrite(buffer, rlen, 1, fp);

printf("5-3\n");
			if(plen<=tlen)	break;			
		}
		fclose(fp);
		//comment
printf("6\n");
		recv(clientSock, &cmntlen, 2, 0);
printf("7-1(%d)\n", cmntlen);
		cmntlen = ntohs(cmntlen);
printf("7-2(%d)\n", cmntlen);
		if (0 < cmntlen)
			recv(clientSock, buffer, cmntlen, 0);	
printf("8\n");
	}
printf("END\n");
	return 1;
}


void TCPClientProcess(int clientSock)
{
	int numBytesRcvd, totnumBytesRcvd = 0;
	char buffer[BUFSIZE];
	HEADER header;	
	//(socket descriptor, void*buff(수신할 버퍼의 포인터), size_t len(버퍼의 byte단위 길이), int flags)
	numBytesRcvd = recv(clientSock, &header.TYPE, 1, 0);	//실제 수신한 바이트 수를 반환
	numBytesRcvd = recv(clientSock, &header.OID, 4, 0);
	header.OID=ntohl(header.OID);
	
printf("type : %d, OID : %d\n",header.TYPE ,header.OID);
	if (numBytesRcvd < 0)	//실패하면 -1 반환
	{
		printf("fail to receive!\n");
		close(clientSock);
		return;
	}

	// Parsing -> Gen SQL -> InsertDB
	// Header Parsing
	REG_USER userdata;
	int oid;

	// Header (5 byte)
	//int headerlen = ParsingDataHeader(buffer, &header);


	// Body
	if (header.TYPE == 1)	//login
	{
		char IDname[45];
		recv(clientSock, buffer, BUFSIZE, 0);

		ParsingID(buffer, IDname);

		oid = loginProc(IDname);
		if (0 <= oid)
		{
			printf("oid : %d\n", oid);

			buffer[0] = 0x09;
			buffer[1] = 'T';
			int noid = htonl(oid);
			memcpy(buffer+2, (char*)&noid, sizeof(int));
			send(clientSock, buffer, 6, 0);
		}			
		else
		{
			printf("oid : -1 ==> Error! [%s]\n", message);
			// message

			buffer[0] = 0x09;
			buffer[1] = 'F';
			int len = strlen(message);
			int nlen = htonl(len);
			memcpy(buffer+2, (char*)&nlen, sizeof(int));
			memcpy(buffer+6, message, len);
			send(clientSock, buffer, 1+1+4+len, 0);					
		}
		
	}
	else if (header.TYPE == 2)	//register user
	{
		//REG_USER userdata;
		recv(clientSock, buffer, BUFSIZE, 0);
		ParsingUserData(buffer, &userdata);
		oid=InsertUserData(&userdata);
		if (0 <= oid)
		{
			printf("oid : %d\n", oid);

			buffer[0] = 0x10;
			buffer[1] = 'T';
			int noid = htonl(oid);
			memcpy(buffer+2, (char*)&noid, sizeof(int));
			send(clientSock, buffer, 6, 0);
		}			
		else
		{
			printf("oid : -1 ==> Error! [%s]\n", message);
	
			buffer[0] = 0x10;
			buffer[1] = 'F';
			int len = strlen(message);
			int nlen = htonl(len);
			memcpy(buffer+2, (char*)&nlen, sizeof(int));
			memcpy(buffer+6, message, len);
			send(clientSock, buffer, 2+4+len, 0);					
		}
		
	}
	else if (header.TYPE == 3)	//register gourmet data
	{
printf("2\n");
		REG_DATA data;
		recv(clientSock, buffer, BUFSIZE, 0);
		ParsingGourmetData(buffer, &data);
		int ret = InsertGourmetData(header.OID, &data);
		if (ret > 0)
		{
			buffer[0] = 0x11;
			buffer[1] = 'T';
			send(clientSock, buffer, 2, 0);
		}			
		else
		{
			buffer[0] = 0x11;
			buffer[1] = 'F';
			int len = strlen(message);
			int nlen = htonl(len);
			memcpy(buffer+2, (char*)&nlen, sizeof(int));
			memcpy(buffer+6, message, len);
			send(clientSock, buffer, 2+4+len, 0);					
		}

	}
	else if(header.TYPE == 4)	//select category -> show brief information
	{
		char category[45];
		recv(clientSock, buffer, BUFSIZE, 0);
		ParsingCategory(buffer, category);
		if(SelectCategory(clientSock, category) <0)
		{
			buffer[0] = 0x12;
			buffer[1] = 'F';
			int len = strlen(message);
			int nlen = htonl(len);
			memcpy(buffer+2, (char*)&nlen, sizeof(int));
			memcpy(buffer+6, message, len);
			send(clientSock, buffer, 2+4+len, 0);					
		}
	}
	else if(header.TYPE==5)		//select datacard -> show all information
	{	
		int gid;
		recv(clientSock, buffer, BUFSIZE, 0);
		memcpy(&gid, buffer, 4);	gid = ntohl(gid);
		if(SelectDataCard(clientSock, gid) <0)
		{
			buffer[0] = 0x13;
			buffer[1] = 'F';
			int len = strlen(message);
			int nlen = htonl(len);
			memcpy(buffer+2, (char*)&nlen, sizeof(int));
			memcpy(buffer+6, message, len);
			send(clientSock, buffer, 2+4+len, 0);					
		}
	}
	else if(header.TYPE==6)		//delete gourmet datacard
	{
		int gid;
		recv(clientSock, buffer, BUFSIZE, 0);
		memcpy(&gid, buffer, 4);	gid = ntohl(gid);
		int ret=DeleteDataCard(clientSock, gid);
		if (ret > 0)
		{
			buffer[0] = 0x14;
			buffer[1] = 'T';
			send(clientSock, buffer, 2, 0);
		}		
		else
		{
			buffer[0] = 0x14;
			buffer[1] = 'F';
			int len = strlen(message);
			int nlen = htonl(len);
			
			memcpy(buffer+2, (char*)&nlen, sizeof(int));
			memcpy(buffer+6, message, len);
			send(clientSock, buffer, 2+4+len, 0);
		}	
	}
	else if(header.TYPE==7)		//Modify gourmet data
	{
		int gid;
		recv(clientSock, buffer, BUFSIZE, 0);
		memcpy(&gid, buffer, 4);	gid = ntohl(gid);

		REG_DATA data;
		ParsingGourmetData(buffer+4, &data);

		int ret = ModifyDataCard(clientSock, gid, &data);
		if (ret > 0)
		{
			buffer[0] = 0x15;
			buffer[1] = 'T';
			send(clientSock, buffer, 2, 0);
		}		
		else
		{
			buffer[0] = 0x15;
			buffer[1] = 'F';
			int len = strlen(message);
			int nlen = htonl(len);
			
			memcpy(buffer+2, (char*)&nlen, sizeof(int));
			memcpy(buffer+6, message, len);
			send(clientSock, buffer, 2+4+len, 0);
		}
	}
	else if(header.TYPE==8)		//register photo
	{
		printf("1\n");
		int ret = RegisterPhoto(clientSock, header);
		
		if (ret > 0)
		{
			buffer[0] = 0x16;
			buffer[1] = 'T';
			send(clientSock, buffer, 2, 0);
		}		
		else
		{
			buffer[0] = 0x16;
			buffer[1] = 'F';
			int len = strlen(message);
			int nlen = htonl(len);
			
			memcpy(buffer+2, (char*)&nlen, sizeof(int));
			memcpy(buffer+6, message, len);
			send(clientSock, buffer, 2+4+len, 0);
		}
	}
	close(clientSock);
}
