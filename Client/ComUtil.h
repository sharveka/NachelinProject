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
	// �α��� ���
	int loginProc( char* id);

	// ����� ��� ���
	int reguserProc(char* id,char* name,char* phone);
	// ������ ��� ���
	int regMenu(char*Category,char*Rest,char*Menu,int Price,short Rate,short Open,char*Cmnt);
	// �����͸�� ��ȸ ���
	int reqData(char* Category);
	// �󼼳�����ȸ ���
	int reqDetail(int GID);
	// ������ ���� ���
	int delData(int GID);
	//������ ���� ���
	int modMenu(int GID, char* Category, char* Rest, char* Menu, int Price, short Rate, short Open, char* Cmnt);
	//�������� ���
	int PicFTP(int GID, CStringArray &sa);
};

