#pragma once
typedef struct _DataItem
{
	int GID;
	char MENU[256];
	int PRICE;
	short RATE;
}DataItem;
typedef struct _DataDetail{
	char CATE[256];
	char REST[256];
	char MENU[256];
	int PRICE;
	short RATE;
	short OPEN;
	char CMNT[256];

}DataDetail;
class ComUtil
{
public:
	ComUtil()
	{
		errMsg = _T("");

	}

	CString m_strIP;
	int m_nPort;
	int m_nOID;
	CString m_strID;
	CString errMsg;
	CString GetErrorMessage(unsigned long nErr);
	void initParam(char* ip, int nPort);
	CArray<DataItem, DataItem> m_DataArray;
	DataDetail m_DataDetail;
	// 로그인 통신
	int loginProc( char* id);

	// 사용자 등록 통신
	int reguserProc(char* id,char* name,char* phone);
	// 데이터 등록 통신
	int regMenu(char*Category,char*Rest,char*Menu,int Price,short Rate,short Open,char*Cmnt);
	// 데이터목록 조회 통신
	int reqData(char* Category);
	// 상세내용조회 통신
	int reqDetail(int GID);
	// 데이터 삭제 통신
	int delData(int GID);
	//데이터 수정 통신
	int modMenu(int GID, char* Category, char* Rest, char* Menu, int Price, short Rate, short Open, char* Cmnt);
	//사진정송 통신
	int PicFTP(int GID, CStringArray &sa);
};

