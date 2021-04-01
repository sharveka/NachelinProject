#include "pch.h"
#include "ComUtil.h"

void ComUtil::initParam(char* ip, int nPort) {
	m_strIP.Format("%s", ip);
	m_nPort = nPort;
}

CString ComUtil::GetErrorMessage(unsigned long nErr)
{
	CString errMsg;

	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		nErr,
		0, // Default language
		(LPTSTR)&lpMsgBuf,
		0,
		NULL
	);
	errMsg.Format("%d:%s", nErr, lpMsgBuf);
	LocalFree(lpMsgBuf);

	return errMsg;
}

int ComUtil::loginProc(char* id)
{
	int sockfd;
	struct sockaddr_in serv_addr;

	// 1. 소켓 생성
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		errMsg = _T("1:") + GetErrorMessage(WSAGetLastError());
		return -9;
	}

	memset((char*)&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(m_nPort);
	serv_addr.sin_addr.s_addr = inet_addr((LPSTR)(LPCTSTR)m_strIP);

	// 2. 소켓 연결
	if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(struct sockaddr)) < 0)
	{
		errMsg = _T("2:") + GetErrorMessage(WSAGetLastError());
		closesocket(sockfd);
		return -9;
	}

	//=======================================================
	// 3. 서버에 보내고자하는 패킷 작성 시작
	int idlen = strlen(id);
	int buflen = 1 + 4 + 2 + idlen;

	char* sendbuf = (char*)malloc(buflen);
	memset(sendbuf, 0x00, buflen);
	sendbuf[0] = 0x01;
	int idlenn = htons(idlen);
	memcpy(sendbuf + 5, &idlenn, 2);
	memcpy(sendbuf + 7, id, idlen);
	// 서버에 보내고자하는 패킷 작성 끝
	//=======================================================

	// 4. 작성된 패킷 보내고
	if (send(sockfd, (char*)sendbuf, buflen, 0) < 0)
	{
		free(sendbuf);
		closesocket(sockfd);
		errMsg = _T("Send Error!\n");
		return -9;
	}
	free(sendbuf);

	// 5. 결과 리턴 받고
	char header[2];
	int numBytesRcvd = recv(sockfd, header, 2, 0);
	if (numBytesRcvd <= 0)
	{
		closesocket(sockfd);
		errMsg = _T("Receive Error!\n");
		return -9;
	}
	if (header[0] != 0x09)
	{
		closesocket(sockfd);
		errMsg = _T("Protocol Type Error!\n");
		return -9;
	}
	char* rbuffer;
	if (header[1] == 'F')
	{
		int len;
		recv(sockfd, (char*)&len, 4, 0);
		len = ntohl(len);

		rbuffer = (char*)malloc(len + 1);
		recv(sockfd, rbuffer, len, 0);
		rbuffer[len] = '\0';

		closesocket(sockfd);

		errMsg.Format("%s", rbuffer);
		free(rbuffer);

		return -1;
	}

	int oid;
	recv(sockfd, (char*)&oid, 4, 0);
	m_nOID = ntohl(oid);
	m_strID = id;
	// 6. 소켓 종료
	closesocket(sockfd);

	return m_nOID;
}
int ComUtil::reguserProc(char* id, char* name, char* phone) {
	int sockfd;
	struct sockaddr_in serv_addr;

	// 1. 소켓 생성
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		errMsg = _T("1:") + GetErrorMessage(WSAGetLastError());
		return -9;
	}

	memset((char*)&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(m_nPort);
	serv_addr.sin_addr.s_addr = inet_addr(m_strIP);

	// 2. 소켓 연결
	if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(struct sockaddr)) < 0)
	{
		errMsg = _T("2:") + GetErrorMessage(WSAGetLastError());
		closesocket(sockfd);
		return -9;
	}

	//=======================================================
	// 3. 서버에 보내고자하는 패킷 작성 시작
	int idlen = strlen(id);
	int namelen = strlen(name);
	int phonelen = strlen(phone);
	int buflen = 1 + 4 + 2 + idlen+2+namelen+2+phonelen;

	char* sendbuf = (char*)malloc(buflen);
	char* tbuf = sendbuf;
	memset(sendbuf, 0x00, buflen);
	sendbuf[0] = 0x02;
	int idlenn = htons(idlen);
	int namelenn = htons(namelen);
	int phonelenn = htons(phonelen);

	tbuf = tbuf + 5;
	memcpy(tbuf, &idlenn, 2); tbuf += 2;
	memcpy(tbuf, id, idlen); tbuf += idlen;

	memcpy(tbuf, &namelenn, 2); tbuf += 2;
	memcpy(tbuf, name, namelen); tbuf += namelen;
	
	memcpy(tbuf , &phonelenn, 2); tbuf += 2;
	memcpy(tbuf, phone, phonelen); tbuf += phonelen;

	// 서버에 보내고자하는 패킷 작성 끝
	//=======================================================

	// 4. 작성된 패킷 보내고
	if (send(sockfd, (char*)sendbuf, buflen, 0) < 0)
	{
		free(sendbuf);
		closesocket(sockfd);
		errMsg = _T("Send Error!\n");
		return -9;
	}
	free(sendbuf);

	// 5. 결과 리턴 받고
	char header[2];
	int numBytesRcvd = recv(sockfd, header, 2, 0);
	if (numBytesRcvd <= 0)
	{
		closesocket(sockfd);
		errMsg = _T("Receive Error!\n");
		return -9;
	}
	if (header[0] != 0x10)
	{
		closesocket(sockfd);
		errMsg = _T("Protocol Type Error!\n");
		return -9;
	}
	char* rbuffer;
	if (header[1] == 'F')
	{
		int len;
		recv(sockfd, (char*)&len, 4, 0);
		len = ntohl(len);

		rbuffer = (char*)malloc(len + 1);
		recv(sockfd, rbuffer, len, 0);
		rbuffer[len] = '\0';

		closesocket(sockfd);

		errMsg.Format("%s", rbuffer);
		free(rbuffer);

		return -1;
	}

	int oid;
	recv(sockfd, (char*)&oid, 4, 0);
	m_nOID = ntohl(oid);
	m_strID = id;
	// 6. 소켓 종료
	closesocket(sockfd);
	return m_nOID;
}
int ComUtil::regMenu(char* Category, char* Rest, char* Menu, int Price, short Rate, short Open, char* Cmnt) {
	int sockfd;
	struct sockaddr_in serv_addr;

	// 1. 소켓 생성
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		errMsg = _T("1:") + GetErrorMessage(WSAGetLastError());
		AfxMessageBox(errMsg);
		return -9;
	}

	memset((char*)&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(m_nPort);
	serv_addr.sin_addr.s_addr = inet_addr(m_strIP);

	// 2. 소켓 연결
	if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(struct sockaddr)) < 0)
	{
		errMsg = _T("2:") + GetErrorMessage(WSAGetLastError());
		closesocket(sockfd);
		AfxMessageBox(errMsg);
		return -9;
	}

	//=======================================================
	// 3. 서버에 보내고자하는 패킷 작성 시작
	int Categorylen = strlen(Category);
	int Restlen = strlen(Rest);
	int Menulen = strlen(Menu);
	int Cmntlen = strlen(Cmnt);
	int buflen = 1 + 4 + 2 + Categorylen + 2 + Restlen + 2 + Menulen+4+2+2+2+Cmntlen;

	char* sendbuf = (char*)malloc(buflen);
	char* tbuf = sendbuf;
	memset(sendbuf, 0x00, buflen);
	sendbuf[0] = 0x03;

	int noidn = htonl(m_nOID);
	short Clenn = htons(Categorylen);
	int Restlenn = htons(Restlen);
	short Menulenn = htons(Menulen);
	int  Pricen = htonl(Price);
	short Raten = htons(Rate);
	short Openn = htons(Open);
	short  Cmntlenn = htons(Cmntlen);

	tbuf = tbuf + 1;
	memcpy(tbuf, &noidn, 4); tbuf += 4;

	memcpy(tbuf, &Clenn, 2); tbuf += 2;
	memcpy(tbuf, Category, Categorylen); tbuf += Categorylen;

	memcpy(tbuf, &Restlenn, 2); tbuf += 2;
	memcpy(tbuf, Rest, Restlen); tbuf += Restlen;

	memcpy(tbuf, &Menulenn, 2); tbuf += 2;
	memcpy(tbuf, Menu, Menulen); tbuf += Menulen;

	memcpy(tbuf, &Pricen, 4); tbuf += 4;
	memcpy(tbuf, &Raten, 2); tbuf += 2;
	memcpy(tbuf, &Openn, 2); tbuf += 2;

	memcpy(tbuf, &Cmntlenn, 2); tbuf += 2;
	memcpy(tbuf, Cmnt, Cmntlen);
	
	// 서버에 보내고자하는 패킷 작성 끝
	//=======================================================

	// 4. 작성된 패킷 보내고
	if (send(sockfd, (char*)sendbuf, buflen, 0) < 0)
	{
		free(sendbuf);
		closesocket(sockfd);
		errMsg = _T("Send Error!");
		AfxMessageBox(errMsg);
		return -9;
	}
	free(sendbuf);

	// 5. 결과 리턴 받고
	char header[2];
	int numBytesRcvd = recv(sockfd, header, 2, 0);
	if (numBytesRcvd <= 0)
	{
		closesocket(sockfd);
		errMsg = _T("Receive Error!");
		AfxMessageBox(errMsg);
		return -9;
	}
	if (header[0] != 0x11)
	{
		closesocket(sockfd);
		errMsg = _T("Protocol Type Error!");
		AfxMessageBox(errMsg);
		return -9;
	}
	char* rbuffer;
	if (header[1] == 'F')
	{
		int len;
		recv(sockfd, (char*)&len, 4, 0);
		len = ntohl(len);

		rbuffer = (char*)malloc(len + 1);
		recv(sockfd, rbuffer, len, 0);
		rbuffer[len] = '\0';

		closesocket(sockfd);

		errMsg.Format("%s", rbuffer);
		free(rbuffer);

		AfxMessageBox(errMsg);

		return -1;
	}

	// 6. 소켓 종료
	closesocket(sockfd);

	return 0;
}
int ComUtil::reqData(char* Category) {
	int sockfd;
	struct sockaddr_in serv_addr;

	// 1. 소켓 생성
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		errMsg = _T("1:") + GetErrorMessage(WSAGetLastError());
		AfxMessageBox(errMsg);
		return -9;
	}

	memset((char*)&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(m_nPort);
	serv_addr.sin_addr.s_addr = inet_addr(m_strIP);

	// 2. 소켓 연결
	if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(struct sockaddr)) < 0)
	{
		errMsg = _T("2:") + GetErrorMessage(WSAGetLastError());
		closesocket(sockfd);
		AfxMessageBox(errMsg);
		return -9;
	}

	//=======================================================
	// 3. 서버에 보내고자하는 패킷 작성 시작
	int Categorylen = strlen(Category);

	int buflen = 1 + 4 + 2 + Categorylen ;

	char* sendbuf = (char*)malloc(buflen);
	char* tbuf = sendbuf;
	memset(sendbuf, 0x00, buflen);
	sendbuf[0] = 0x04;

	int noidn = htonl(m_nOID);
	short Clenn = htons(Categorylen);
	

	tbuf = tbuf + 1;
	memcpy(tbuf, &noidn, 4); tbuf += 4;

	memcpy(tbuf, &Clenn, 2); tbuf += 2;
	memcpy(tbuf, Category, Categorylen); tbuf += Categorylen;



	// 서버에 보내고자하는 패킷 작성 끝
	//=======================================================

	// 4. 작성된 패킷 보내고
	if (send(sockfd, (char*)sendbuf, buflen, 0) < 0)
	{
		free(sendbuf);
		closesocket(sockfd);
		errMsg = _T("Send Error!");
		AfxMessageBox(errMsg);
		return -9;
	}
	free(sendbuf);

	// 5. 결과 리턴 받고
	char header[2];
	int numBytesRcvd = recv(sockfd, header, 2, 0);
	if (numBytesRcvd <= 0)
	{
		closesocket(sockfd);
		errMsg = _T("Receive Error!");
		AfxMessageBox(errMsg);
		return -9;
	}
	if (header[0] != 0x12)
	{
		closesocket(sockfd);
		errMsg = _T("Protocol Type Error!");
		AfxMessageBox(errMsg);
		return -9;
	}
	char* rbuffer;
	if (header[1] == 'F')
	{
		int len;
		recv(sockfd, (char*)&len, 4, 0);
		len = ntohl(len);

		rbuffer = (char*)malloc(len + 1);
		recv(sockfd, rbuffer, len, 0);
		rbuffer[len] = '\0';

		closesocket(sockfd);

		errMsg.Format("%s", rbuffer);
		free(rbuffer);

		AfxMessageBox(errMsg);

		return -1;
	}
	int cnt, LEN;
	recv(sockfd, (char*)&cnt, 4, 0);
	cnt = ntohl(cnt);
	DataItem di;
	m_DataArray.RemoveAll();
	for (int i = 0; i < cnt; i++) {
		recv(sockfd, (char*)&di.GID, 4, 0);
		di.GID=ntohl(di.GID);
		recv(sockfd, (char*)&LEN, 2, 0);
		LEN = ntohs(LEN);
		recv(sockfd, di.MENU,LEN, 0);
		di.MENU[LEN] = '\0';
		recv(sockfd, (char*)&di.PRICE, 4, 0);
		di.PRICE = ntohl(di.PRICE);
		recv(sockfd, (char*)&di.RATE, 2, 0);
		di.RATE = ntohs(di.RATE);
		m_DataArray.Add(di);
	}
	
	// 6. 소켓 종료
	closesocket(sockfd);

	return m_DataArray.GetCount();
}
int ComUtil::reqDetail(int GID) {
	int sockfd;
	struct sockaddr_in serv_addr;

	// 1. 소켓 생성
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		errMsg = _T("1:") + GetErrorMessage(WSAGetLastError());
		AfxMessageBox(errMsg);
		return -9;
	}

	memset((char*)&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(m_nPort);
	serv_addr.sin_addr.s_addr = inet_addr(m_strIP);

	// 2. 소켓 연결
	if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(struct sockaddr)) < 0)
	{
		errMsg = _T("2:") + GetErrorMessage(WSAGetLastError());
		closesocket(sockfd);
		AfxMessageBox(errMsg);
		return -9;
	}

	//=======================================================
	// 3. 서버에 보내고자하는 패킷 작성 시작

	int buflen = 1 + 4 + 4;

	char* sendbuf = (char*)malloc(buflen);
	char* tbuf = sendbuf;
	memset(sendbuf, 0x00, buflen);
	sendbuf[0] = 0x05;

	int noidn = htonl(m_nOID);
	int GIDn = htonl(GID);


	tbuf = tbuf + 1;
	memcpy(tbuf, &noidn, 4); tbuf += 4;
	memcpy(tbuf, &GIDn, 4);


	// 서버에 보내고자하는 패킷 작성 끝
	//=======================================================

	// 4. 작성된 패킷 보내고
	if (send(sockfd, (char*)sendbuf, buflen, 0) < 0)
	{
		free(sendbuf);
		closesocket(sockfd);
		errMsg = _T("Send Error!");
		AfxMessageBox(errMsg);
		return -9;
	}
	free(sendbuf);

	// 5. 결과 리턴 받고
	char header[2];
	int numBytesRcvd = recv(sockfd, header, 2, 0);
	if (numBytesRcvd <= 0)
	{
		closesocket(sockfd);
		errMsg = _T("Receive Error!");
		AfxMessageBox(errMsg);
		return -9;
	}
	if (header[0] != 0x13)
	{
		closesocket(sockfd);
		errMsg = _T("Protocol Type Error!");
		AfxMessageBox(errMsg);
		return -9;
	}
	char* rbuffer;
	if (header[1] == 'F')
	{
		int len;
		recv(sockfd, (char*)&len, 4, 0);
		len = ntohl(len);

		rbuffer = (char*)malloc(len + 1);
		recv(sockfd, rbuffer, len, 0);
		rbuffer[len] = '\0';

		closesocket(sockfd);

		errMsg.Format("%s", rbuffer);
		free(rbuffer);

		AfxMessageBox(errMsg);

		return -1;
	}
	int LEN;

	
	
	recv(sockfd, (char*)&LEN, 2, 0);
	LEN = ntohs(LEN);
	recv(sockfd, m_DataDetail.CATE, LEN, 0);				//카테고리
	m_DataDetail.CATE[LEN] = '\0';
	recv(sockfd, (char*)&LEN, 2, 0);
	LEN = ntohs(LEN);	
	recv(sockfd, m_DataDetail.REST, LEN, 0);				//레스토랑
	m_DataDetail.REST[LEN] = '\0';

	recv(sockfd, (char*)&LEN, 2, 0);
	LEN = ntohs(LEN);	
	recv(sockfd, m_DataDetail.MENU, LEN, 0);				//메뉴
	m_DataDetail.MENU[LEN] = '\0';
	recv(sockfd, (char*)&m_DataDetail.PRICE, 4, 0);		//가격
	m_DataDetail.PRICE = ntohl(m_DataDetail.PRICE);
	recv(sockfd, (char*)&m_DataDetail.RATE, 2, 0);		//점수
	m_DataDetail.RATE = ntohs(m_DataDetail.RATE);
	recv(sockfd, (char*)&m_DataDetail.OPEN, 2, 0);		//공개
	m_DataDetail.OPEN = ntohs(m_DataDetail.OPEN);

	recv(sockfd, (char*)&LEN, 2, 0);
	LEN = ntohs(LEN);
	recv(sockfd, m_DataDetail.CMNT, LEN, 0);				//코멘트
	m_DataDetail.CMNT[LEN] = '\0';




	

	// 6. 소켓 종료
	closesocket(sockfd);

	return 1;
}
int ComUtil::delData(int GID) {
	int sockfd;
	struct sockaddr_in serv_addr;

	// 1. 소켓 생성
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		errMsg = _T("1:") + GetErrorMessage(WSAGetLastError());
		AfxMessageBox(errMsg);
		return -9;
	}

	memset((char*)&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(m_nPort);
	serv_addr.sin_addr.s_addr = inet_addr(m_strIP);

	// 2. 소켓 연결
	if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(struct sockaddr)) < 0)
	{
		errMsg = _T("2:") + GetErrorMessage(WSAGetLastError());
		closesocket(sockfd);
		AfxMessageBox(errMsg);
		return -9;
	}

	//=======================================================
	// 3. 서버에 보내고자하는 패킷 작성 시작

	int buflen = 1 + 4 + 4;

	char* sendbuf = (char*)malloc(buflen);
	char* tbuf = sendbuf;
	memset(sendbuf, 0x00, buflen);
	sendbuf[0] = 0x06;

	int noidn = htonl(m_nOID);
	int GIDn = htonl(GID);


	tbuf = tbuf + 1;
	memcpy(tbuf, &noidn, 4); tbuf += 4;
	memcpy(tbuf, &GIDn, 4);


	// 서버에 보내고자하는 패킷 작성 끝
	//=======================================================

	// 4. 작성된 패킷 보내고
	if (send(sockfd, (char*)sendbuf, buflen, 0) < 0)
	{
		free(sendbuf);
		closesocket(sockfd);
		errMsg = _T("Send Error!");
		AfxMessageBox(errMsg);
		return -9;
	}
	free(sendbuf);

	// 5. 결과 리턴 받고
	char header[2];
	int numBytesRcvd = recv(sockfd, header, 2, 0);
	if (numBytesRcvd <= 0)
	{
		closesocket(sockfd);
		errMsg = _T("Receive Error!");
		AfxMessageBox(errMsg);
		return -9;
	}
	if (header[0] != 0x14)
	{
		closesocket(sockfd);
		errMsg = _T("Protocol Type Error!");
		AfxMessageBox(errMsg);
		return -9;
	}
	char* rbuffer;
	if (header[1] == 'F')
	{
		int len;
		recv(sockfd, (char*)&len, 4, 0);
		len = ntohl(len);

		rbuffer = (char*)malloc(len + 1);
		recv(sockfd, rbuffer, len, 0);
		rbuffer[len] = '\0';

		closesocket(sockfd);

		errMsg.Format("%s", rbuffer);
		free(rbuffer);

		AfxMessageBox(errMsg);

		return -1;
	}
	
	// 6. 소켓 종료
	closesocket(sockfd);

	return 0;
}
int ComUtil::modMenu(int GID,char* Category, char* Rest, char* Menu, int Price, short Rate, short Open, char* Cmnt) {
	int sockfd;
	struct sockaddr_in serv_addr;

	// 1. 소켓 생성
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		errMsg = _T("1:") + GetErrorMessage(WSAGetLastError());
		AfxMessageBox(errMsg);
		return -9;
	}

	memset((char*)&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(m_nPort);
	serv_addr.sin_addr.s_addr = inet_addr(m_strIP);

	// 2. 소켓 연결
	if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(struct sockaddr)) < 0)
	{
		errMsg = _T("2:") + GetErrorMessage(WSAGetLastError());
		closesocket(sockfd);
		AfxMessageBox(errMsg);
		return -9;
	}

	//=======================================================
	// 3. 서버에 보내고자하는 패킷 작성 시작
	int Categorylen = strlen(Category);
	int Restlen = strlen(Rest);
	int Menulen = strlen(Menu);
	int Cmntlen = strlen(Cmnt);
	int buflen = 1 + 4 + 4+2 + Categorylen + 2 + Restlen + 2 + Menulen + 4 + 2 + 2 + 2 + Cmntlen;

	char* sendbuf = (char*)malloc(buflen);
	char* tbuf = sendbuf;
	memset(sendbuf, 0x00, buflen);
	sendbuf[0] = 0x07;

	int noidn = htonl(m_nOID);
	int GIDn = htonl(GID);
	short Clenn = htons(Categorylen);
	int Restlenn = htons(Restlen);
	short Menulenn = htons(Menulen);
	int  Pricen = htonl(Price);
	short Raten = htons(Rate);
	short Openn = htons(Open);
	short  Cmntlenn = htons(Cmntlen);

	tbuf = tbuf + 1;
	memcpy(tbuf, &noidn, 4); tbuf += 4;
	memcpy(tbuf, &GIDn, 4); tbuf += 4;
	memcpy(tbuf, &Clenn, 2); tbuf += 2;
	memcpy(tbuf, Category, Categorylen); tbuf += Categorylen;

	memcpy(tbuf, &Restlenn, 2); tbuf += 2;
	memcpy(tbuf, Rest, Restlen); tbuf += Restlen;

	memcpy(tbuf, &Menulenn, 2); tbuf += 2;
	memcpy(tbuf, Menu, Menulen); tbuf += Menulen;

	memcpy(tbuf, &Pricen, 4); tbuf += 4;
	memcpy(tbuf, &Raten, 2); tbuf += 2;
	memcpy(tbuf, &Openn, 2); tbuf += 2;

	memcpy(tbuf, &Cmntlenn, 2); tbuf += 2;
	memcpy(tbuf, Cmnt, Cmntlen);

	// 서버에 보내고자하는 패킷 작성 끝
	//=======================================================

	// 4. 작성된 패킷 보내고
	if (send(sockfd, (char*)sendbuf, buflen, 0) < 0)
	{
		free(sendbuf);
		closesocket(sockfd);
		errMsg = _T("Send Error!");
		AfxMessageBox(errMsg);
		return -9;
	}
	free(sendbuf);

	// 5. 결과 리턴 받고
	char header[2];
	int numBytesRcvd = recv(sockfd, header, 2, 0);
	if (numBytesRcvd <= 0)
	{
		closesocket(sockfd);
		errMsg = _T("Receive Error!");
		AfxMessageBox(errMsg);
		return -9;
	}
	if (header[0] != 0x15)
	{
		closesocket(sockfd);
		errMsg = _T("Protocol Type Error!");
		AfxMessageBox(errMsg);
		return -9;
	}
	char* rbuffer;
	if (header[1] == 'F')
	{
		int len;
		recv(sockfd, (char*)&len, 4, 0);
		len = ntohl(len);

		rbuffer = (char*)malloc(len + 1);
		recv(sockfd, rbuffer, len, 0);
		rbuffer[len] = '\0';

		closesocket(sockfd);

		errMsg.Format("%s", rbuffer);
		free(rbuffer);

		AfxMessageBox(errMsg);

		return -1;
	}

	// 6. 소켓 종료
	closesocket(sockfd);

	return 0;
}
int ComUtil::PicFTP(int GID, CStringArray& sa) { 
	int sockfd;
	struct sockaddr_in serv_addr;

	// 1. 소켓 생성
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		errMsg = _T("1:") + GetErrorMessage(WSAGetLastError());
		AfxMessageBox(errMsg);
		return -9;
	}

	memset((char*)&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(m_nPort);
	serv_addr.sin_addr.s_addr = inet_addr(m_strIP);

	// 2. 소켓 연결
	if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(struct sockaddr)) < 0)
	{
		errMsg = _T("2:") + GetErrorMessage(WSAGetLastError());
		closesocket(sockfd);
		AfxMessageBox(errMsg);
		return -9;
	}

	//=======================================================
	// 3. 서버에 보내고자하는 패킷 작성 시작
	/*int Piclen = strlen(Pic);
	int Cmntlen = strlen(Cmnt);
	int buflen = 1 + 4 + 4 + 2 + 4 + 1 + Piclen + 2 + Cmntlen;*/

	//char* sendbuf = (char*)malloc(buflen);
	//char* tbuf = sendbuf;
	//memset(sendbuf, 0x00, buflen);
	//sendbuf[0] = 0x08;
	char T=0x08;
	send(sockfd, &T, 1, 0);
	int oid = htonl(m_nOID);
	send(sockfd, (char*)&oid, 4,0);
	int gid = htonl(GID);
	send(sockfd, (char*)&gid, 4, 0);
	short cnt = htons((short)sa.GetSize());
	send(sockfd, (char*)&cnt, 2, 0);
	for (int i = 0; i < sa.GetSize(); i++) {

		CString fname = sa.GetAt(i);
		fname.MakeUpper();
		char ptype;
		if (fname.Right(3) == "PNG") ptype = 1;
		else ptype = 2;
		FILE* fp = fopen(fname, "rb");

		fseek(fp, 0, SEEK_END);//전체길이
		int flen = ftell(fp);

		int flenn = htonl(flen);
		send(sockfd, (char*)&flenn, 4, 0);
		send(sockfd, &ptype, 1, 0);

		fseek(fp, 0, SEEK_SET);//처음 위치로(파일포인터)

		char* buf = (char*)malloc(flen);
		fread(buf, flen, 1, fp);

		send(sockfd, buf, flen, 0);
		fclose(fp);
		free(buf);
		short CMNTLEN = 0;
		send(sockfd, (char*)&CMNTLEN, 2, 0);
	/*	char* CMNT;
		send(sockfd, CMNT, CMNTLEN, 0);*/

	}




	



	// 5. 결과 리턴 받고
	char header[2];
	int numBytesRcvd = recv(sockfd, header, 2, 0);
	if (numBytesRcvd <= 0)
	{
		closesocket(sockfd);
		errMsg = _T("Receive Error!");
		AfxMessageBox(errMsg);
		return -9;
	}
	if (header[0] != 0x16)
	{
		closesocket(sockfd);
		errMsg = _T("Protocol Type Error!");
		AfxMessageBox(errMsg);
		return -9;
	}
	char* rbuffer;
	if (header[1] == 'F')
	{
		int len;
		recv(sockfd, (char*)&len, 4, 0);
		len = ntohl(len);

		rbuffer = (char*)malloc(len + 1);
		recv(sockfd, rbuffer, len, 0);
		rbuffer[len] = '\0';

		closesocket(sockfd);

		errMsg.Format("%s", rbuffer);
		free(rbuffer);

		AfxMessageBox(errMsg);

		return -1;
	}

	// 6. 소켓 종료
	closesocket(sockfd);

	return 0;
}