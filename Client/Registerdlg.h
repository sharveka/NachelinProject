#pragma once

class CNachelinDlg;

// Registerdlg 대화 상자

class Registerdlg : public CDialogEx
{
	DECLARE_DYNAMIC(Registerdlg)

public:
	Registerdlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~Registerdlg();
	CNachelinDlg* m_pMainwnd;
	int m_oid;
	int m_mode=0;
	int m_GID;
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Registerdlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnBnClickedReg();
	CString m_strRes;
	int m_iPrice;
	CString m_strMenu;
	CString m_strCom;

	virtual BOOL OnInitDialog();

	CString m_strCategory;
	int m_iScore;
	BOOL m_nOpen;
	//afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedRegPic();
};
