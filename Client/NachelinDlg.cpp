
// NachelinDlg.cpp: 구현 파일
//
#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include "framework.h"
#include "Nachelin.h"
#include "NachelinDlg.h"
#include "afxdialogex.h"
#include "Registerdlg.h"
#include "ComUtil.h"
#include <string>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

char* category[] = {
	"Korean","Western","Chinese","Japanese","Snack","Others"
};


// CNachelinDlg 대화 상자



CNachelinDlg::CNachelinDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_NACHELIN_DIALOG, pParent)
	, m_nCate(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNachelinDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_KOR, m_nCate);
	DDX_Control(pDX, IDC_LIST1, m_ctrlList);
}

BEGIN_MESSAGE_MAP(CNachelinDlg, CDialogEx)
	ON_BN_CLICKED(IDC_Button_reg, &CNachelinDlg::OnBnClickedButtonreg)
	ON_BN_CLICKED(IDC_RADIO_KOR, &CNachelinDlg::OnClickedRadioKor)
	ON_COMMAND(IDC_RADIO_CHK, &CNachelinDlg::OnRadioChk)
	ON_COMMAND(IDC_RADIO_CHN, &CNachelinDlg::OnRadioChn)
	ON_COMMAND(IDC_RADIO_ETC, &CNachelinDlg::OnRadioEtc)
	ON_COMMAND(IDC_RADIO_FOR, &CNachelinDlg::OnRadioFor)
	ON_COMMAND(IDC_RADIO_JPN, &CNachelinDlg::OnRadioJpn)
	ON_COMMAND(IDC_RADIO_PZA, &CNachelinDlg::OnRadioPza)
	ON_COMMAND(IDC_RADIO_SNK, &CNachelinDlg::OnRadioSnk)
	ON_BN_CLICKED(IDC_BUTTON_del, &CNachelinDlg::OnBnClickedButtondel)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CNachelinDlg::OnDblclkList1)
END_MESSAGE_MAP()


// CNachelinDlg 메시지 처리기

BOOL CNachelinDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	
	SetWindowText(_T("Nachelin <"+m_pCU->m_strID+">"));

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
		// List Ctrl Title Setting
	LV_COLUMN lvcolumn;	// 리스트 컨트롤 컬럼 속성을 입력할 변수 선언
	lvcolumn.mask = LVCF_TEXT | LVCF_SUBITEM | LVCF_WIDTH | LVCF_FMT;	// 컬럼 마스크 설정

	// 리스트 컨트롤 스타일 수정
	m_ctrlList.SetExtendedStyle(m_ctrlList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	CString titlearr[4] = { _T("GID"), _T("메뉴"), _T("가격"), _T("점수")};	// 컬럼 타이틀 문자 배열
	UINT titlewdt[4] = { 50, 120,80,50  };				// 컬럼 타이틀 폭 배열
	UINT titlefmt[4] = { LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_RIGHT, LVCFMT_CENTER };
	// 컬럼 정렬 설정 배열

	for (int i = 0; i < 4; i++) {
		lvcolumn.pszText = (LPTSTR)(LPCTSTR)titlearr[i];	// 컬럼 타이틀 설정
		lvcolumn.iSubItem = i;			// 컬럼 인덱스 설정
		lvcolumn.cx = titlewdt[i];			// 컬럼 폭 설정
		lvcolumn.fmt = titlefmt[i];		// 컬럼 정렬 설정

		m_ctrlList.InsertColumn(i, &lvcolumn);	// 리스트 컨트롤에 컬럼 삽입
	}
	RadioProc(category[0]);
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}




void CNachelinDlg::OnBnClickedButtonreg()
{
	Registerdlg dlg(this);
	
	dlg.DoModal();
}

void CNachelinDlg::OnClickedRadioKor(){RadioProc(category[0]);}
void CNachelinDlg::OnRadioChk()	{}
void CNachelinDlg::OnRadioPza() {}
void CNachelinDlg::OnRadioChn()	{ RadioProc(category[2]);}
void CNachelinDlg::OnRadioEtc()	{ RadioProc(category[5]);}
void CNachelinDlg::OnRadioFor() { RadioProc(category[1]);}
void CNachelinDlg::OnRadioJpn()	{ RadioProc(category[3]);}
void CNachelinDlg::OnRadioSnk() { RadioProc(category[4]);}

void CNachelinDlg::RadioProc(char* cate) {
	UpdateData(TRUE);
	int cnt=m_pCU->reqData(cate);
	m_ctrlList.DeleteAllItems();
	CString str;
	for (int i = 0; i < cnt; i++) {
		DataItem di = m_pCU->m_DataArray.GetAt(i);

		//str.Format("%d | %d | 이름: %s| 가격: %d 원 | %d 점", i,di.GID, di.MENU, di.PRICE, di.RATE);
		str.Format("%d", di.GID);
		m_ctrlList.InsertItem(i,str);
		m_ctrlList.SetItemText(i,1,di.MENU);
		m_ctrlList.SetItemText(i,2,to_string(di.PRICE).c_str());
		m_ctrlList.SetItemText(i,3,to_string(di.RATE).c_str());
		
	}

}

void CNachelinDlg::OnBnClickedButtondel()
{

	POSITION pos = m_ctrlList.GetFirstSelectedItemPosition();
	if (pos == NULL) return;

	int nItem = m_ctrlList.GetNextSelectedItem(pos);
	
	

	CString str=m_ctrlList.GetItemText(nItem,0);//(row,col)


	int GID = atoi(str);
	
	if (m_pCU->delData(GID) == 0) {
		RadioProc(category[m_nCate]);
		AfxMessageBox("Successfully deleted");
	}


}


void CNachelinDlg::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	if (pNMItemActivate->iItem != -1)
	{
		CString str_GID = m_ctrlList.GetItemText(pNMItemActivate->iItem, 0);
		int GID = atoi(str_GID);
		if (m_pCU->reqDetail(GID) == 1) {
			Registerdlg dlg(this);
			dlg.m_GID = GID;
			dlg.m_mode = 1;
			if(dlg.DoModal()==IDOK)	RadioProc(category[m_nCate]);
		}
	}



	*pResult = 0;
}
