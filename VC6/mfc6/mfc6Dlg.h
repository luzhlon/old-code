// mfc6Dlg.h : header file
//

#if !defined(AFX_MFC6DLG_H__5D7B750D_F4D6_4D53_A36D_A522416B043A__INCLUDED_)
#define AFX_MFC6DLG_H__5D7B750D_F4D6_4D53_A36D_A522416B043A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMfc6Dlg dialog

class CMfc6Dlg : public CDialog
{
// Construction
public:
	CMfc6Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMfc6Dlg)
	enum { IDD = IDD_MFC6_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMfc6Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMfc6Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnHscrollEdit1();
	afx_msg void OnChangeEdit1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFC6DLG_H__5D7B750D_F4D6_4D53_A36D_A522416B043A__INCLUDED_)
