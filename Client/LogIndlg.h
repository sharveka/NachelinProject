#pragma once

class ComUtil;

// LogIndlg 대화 상자

class LogIndlg : public CDialogEx
{
	DECLARE_DYNAMIC(LogIndlg)

public:
	LogIndlg(CWnd* pParent, ComUtil* pCU);   // 표준 생성자입니다.
	virtual ~LogIndlg();

	ComUtil* m_pCU;
	int m_oid;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LogIndlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CString m_strID;
	int m_nPort;
	virtual BOOL OnInitDialog();
	CString m_strIP;
};
