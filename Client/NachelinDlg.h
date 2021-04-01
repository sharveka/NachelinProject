
// NachelinDlg.h: 헤더 파일
//

#pragma once

class ComUtil;

// CNachelinDlg 대화 상자
class CNachelinDlg : public CDialogEx
{
// 생성입니다.
public:
	CNachelinDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	void RadioProc(char* cate);
	int m_oid;
	ComUtil* m_pCU;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NACHELIN_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonreg();
	int m_nCate;
	afx_msg void OnClickedRadioKor();
	afx_msg void OnRadioChk();
	afx_msg void OnRadioChn();
	afx_msg void OnRadioEtc();
	afx_msg void OnRadioFor();
	afx_msg void OnRadioJpn();
	afx_msg void OnRadioPza();
	afx_msg void OnRadioSnk();

	afx_msg void OnBnClickedButtondel();
	CListCtrl m_ctrlList;
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
};
