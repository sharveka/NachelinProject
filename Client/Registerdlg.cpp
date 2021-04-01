// Registerdlg.cpp: 구현 파일
//

#include "pch.h"
#include "Nachelin.h"
#include "Registerdlg.h"
#include "afxdialogex.h"
#include "ComUtil.h"
#include "NachelinDlg.h"

// Registerdlg 대화 상자

IMPLEMENT_DYNAMIC(Registerdlg, CDialogEx)

Registerdlg::Registerdlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Registerdlg, pParent)
	, m_strRes(_T(""))
	, m_iPrice(0)
	, m_strMenu(_T(""))
	, m_strCom(_T(""))
	, m_strCategory(_T("korean"))
	, m_iScore(0)
	, m_nOpen(FALSE)
{
	m_pMainwnd = (CNachelinDlg*)pParent;
}

Registerdlg::~Registerdlg()
{
}

void Registerdlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_RESTAURANT, m_strRes);
	DDX_Text(pDX, IDC_PRICE, m_iPrice);
	DDX_Text(pDX, IDC_MENU, m_strMenu);
	DDX_Text(pDX, IDC_COMMENT, m_strCom);

	DDX_CBString(pDX, IDC_COMBO1, m_strCategory);
	DDX_CBIndex(pDX, IDC_COMBO2, m_iScore);
	DDX_Check(pDX, IDC_CHECK1, m_nOpen);
}


BEGIN_MESSAGE_MAP(Registerdlg, CDialogEx)
	
	ON_BN_CLICKED(ID_REG, &Registerdlg::OnBnClickedReg)
	
	//ON_BN_CLICKED(IDC_BUTTON1, &Registerdlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_REG_PIC, &Registerdlg::OnBnClickedRegPic)
END_MESSAGE_MAP()

void Registerdlg::OnBnClickedReg()
{
	UpdateData(TRUE);
	if (m_mode == 0) {
		int flag = m_pMainwnd->m_pCU->regMenu((LPSTR)(LPCTSTR)m_strCategory, (LPSTR)(LPCTSTR)m_strRes, (LPSTR)(LPCTSTR)m_strMenu, m_iPrice, m_iScore + 1, m_nOpen, (LPSTR)(LPCTSTR)m_strCom);

		//성공이면,
		if (flag == 0) {
			//m_oid에 oid받아와서 등록,
			AfxMessageBox("등록되었습니다.");
			CDialogEx::OnOK();
			return;
		}
	
	}
	else {
		int flag = m_pMainwnd->m_pCU->modMenu(m_GID, (LPSTR)(LPCTSTR)m_strCategory, (LPSTR)(LPCTSTR)m_strRes, (LPSTR)(LPCTSTR)m_strMenu, m_iPrice, m_iScore + 1, m_nOpen, (LPSTR)(LPCTSTR)m_strCom);

		//성공이면,
		if (flag == 0) {
			//m_oid에 oid받아와서 등록,
			AfxMessageBox("수정되었습니다.");
			CDialogEx::OnOK();
			return;
		}
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}


BOOL Registerdlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	if (m_mode == 1) {
		//GetDlgItem(ID_REG)->ShowWindow(SW_HIDE);
		GetDlgItem(ID_REG)->SetWindowTextA("수정");
		m_strRes=m_pMainwnd->m_pCU->m_DataDetail.REST;
		m_iPrice= m_pMainwnd->m_pCU->m_DataDetail.PRICE;
		m_iScore= m_pMainwnd->m_pCU->m_DataDetail.RATE-1;
		m_strMenu= m_pMainwnd->m_pCU->m_DataDetail.MENU;
		m_strCom= m_pMainwnd->m_pCU->m_DataDetail.CMNT;
		m_strCategory= m_pMainwnd->m_pCU->m_DataDetail.CATE;
		m_nOpen= m_pMainwnd->m_pCU->m_DataDetail.OPEN;
		GetDlgItem(IDC_REG_PIC)->ShowWindow(SW_SHOW);
		UpdateData(FALSE);
	}
	return TRUE;  
}





void Registerdlg::OnBnClickedRegPic()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CStringArray sa;
	sa.Add("E:/photo/고양이.png");
	sa.Add("E:/photo/무야호.png");
	m_pMainwnd->m_pCU->PicFTP(m_GID, sa);
}
