// EnrollUser.cpp: 구현 파일
//

#include "pch.h"
#include "Nachelin.h"
#include "EnrollUser.h"
#include "afxdialogex.h"
#include "LogIndlg.h"

// EnrollUser 대화 상자

IMPLEMENT_DYNAMIC(EnrollUser, CDialogEx)

EnrollUser::EnrollUser(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EnrollUser, pParent)
	, m_strID(_T(""))
	, m_strName(_T(""))
	, m_strPhone(_T(""))
{
	m_pMainwnd = (LogIndlg*)pParent;
}

EnrollUser::~EnrollUser()
{
}

void EnrollUser::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ID, m_strID);
	DDX_Text(pDX, IDC_Name, m_strName);
	DDX_Text(pDX, IDC_Phone, m_strPhone);
}


BEGIN_MESSAGE_MAP(EnrollUser, CDialogEx)
	
	ON_BN_CLICKED(IDOK, &EnrollUser::OnBnClickedOk)
END_MESSAGE_MAP()


// EnrollUser 메시지 처리기



void EnrollUser::OnBnClickedOk()
{
	UpdateData(TRUE);

	//성공이면,
	if (m_pMainwnd->m_pCU->reguserProc((LPSTR)(LPCTSTR)m_strID, (LPSTR)(LPCTSTR)m_strName, (LPSTR)(LPCTSTR)m_strPhone) >=0) {
		//m_oid에 oid받아와서 등록,

		CDialogEx::OnOK();
		return;
	}

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}
