// 夜阑说说服务端Dlg.h : header file
//

#if !defined(AFX_DLG_H__CDF11D11_87CF_4835_9815_111D3D1F19B0__INCLUDED_)
#define AFX_DLG_H__CDF11D11_87CF_4835_9815_111D3D1F19B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMyDlg dialog

class CMyDlg : public CDialog
{
// Construction
public:
	CMyDlg(CWnd* pParent = NULL);	// standard constructor
	void update(CString s);
// Dialog Data
	//{{AFX_DATA(CMyDlg)
	enum { IDD = IDD_MY_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMyDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnCancel();
	afx_msg void OnbtnSEND();
	afx_msg void OnClose();
	afx_msg void OnCancelMode();
	afx_msg void OnOK() ;
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	//---------------夜阑说说--------------
private:
	CEdit * show_edit;
	CEdit * send_edit;
	CEdit * getname;
};
	//---------------夜阑说说--------------
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_H__CDF11D11_87CF_4835_9815_111D3D1F19B0__INCLUDED_)
