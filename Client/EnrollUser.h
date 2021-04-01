#pragma once

class LogIndlg;

// EnrollUser 대화 상자

class EnrollUser : public CDialogEx
{
	DECLARE_DYNAMIC(EnrollUser)

public:
	EnrollUser(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~EnrollUser();

	LogIndlg* m_pMainwnd;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EnrollUser };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnBnClickedOk();
	CString m_strID;
	CString m_strName;
	CString m_strPhone;
};
