// LogIndlg.cpp: 구현 파일
//

#include "pch.h"
#include "Nachelin.h"
#include "LogIndlg.h"
#include "afxdialogex.h"
#include "EnrollUser.h"
#include "LogIndlg.h"

// LogIndlg 대화 상자

IMPLEMENT_DYNAMIC(LogIndlg, CDialogEx)

LogIndlg::LogIndlg(CWnd* pParent, ComUtil* pCU)
	: CDialogEx(IDD_LogIndlg, pParent)
	, m_strID(_T("koreano1"))
	, m_nPort(5000)
	, m_strIP(_T("164.125.121.178"))
{
	m_pCU = pCU;
}

LogIndlg::~LogIndlg()
{
}

void LogIndlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strID);
	DDX_Text(pDX, IDC_EDIT3, m_nPort);
	DDX_Text(pDX, IDC_EDIT4, m_strIP);
}


BEGIN_MESSAGE_MAP(LogIndlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &LogIndlg::OnBnClickedButton1)
END_MESSAGE_MAP()



// LogIndlg 메시지 처리기


BOOL LogIndlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;
}

void LogIndlg::OnBnClickedButton1()
{
	UpdateData(TRUE);
	m_pCU->initParam((LPSTR)(LPCTSTR)m_strIP, m_nPort);
	int oid = m_pCU->loginProc((LPSTR)(LPCTSTR)m_strID);
	//AfxMessageBox("로그인되었습니다.");
	CString str;
	if (oid ==-1)
	{
		AfxMessageBox(m_pCU->errMsg);
		//사용자가 없으므로 등록화면.
		
		EnrollUser dlg(this);
		dlg.m_strID = m_strID;
		if (dlg.DoModal() == IDOK) {
			//str.Format("oid:%d", oid);
			//AfxMessageBox(str);
			CDialogEx::OnOK();
		}
		else
		{
			AfxMessageBox(m_pCU->errMsg);
			CDialogEx::OnCancel();
			
		}

		return;

	}
	else if (oid < -1) {
		//서버와 연결 실패는 에러메시지
		AfxMessageBox(m_pCU->errMsg);
		CDialogEx::OnCancel();
		return;
	}
	//oid넘어오면 성공 메인페이지
	

	//str.Format("oid:%d", oid);
	//AfxMessageBox(str);
	CDialogEx::OnOK();
}

