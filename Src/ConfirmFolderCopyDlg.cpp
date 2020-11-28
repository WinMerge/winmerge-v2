// SPDX-License-Identifier: GPL-2.0-or-later
/**
 * @file  ConfirmFolderCopyDlg.cpp
 *
 * @brief Implementation file for ConfirmFolderCopyDlg dialog
 */

#include "stdafx.h"
#include "ConfirmFolderCopyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ConfirmFolderCopyDlg dialog

IMPLEMENT_DYNAMIC(ConfirmFolderCopyDlg, CTrDialog)
ConfirmFolderCopyDlg::ConfirmFolderCopyDlg(CWnd* pParent /*= nullptr*/)
	: CTrDialog(ConfirmFolderCopyDlg::IDD, pParent)
	, m_dontAskAgain(false)
{
}

ConfirmFolderCopyDlg::~ConfirmFolderCopyDlg()
{
}

void ConfirmFolderCopyDlg::DoDataExchange(CDataExchange* pDX)
{
	CTrDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ConfirmFolderCopyDlg)
	DDX_Text(pDX, IDC_FLDCONFIRM_FROM_TEXT, m_fromText);
	DDX_Text(pDX, IDC_FLDCONFIRM_TO_TEXT, m_toText);
	DDX_Text(pDX, IDC_FLDCONFIRM_FROM_PATH, m_fromPath);
	DDX_Text(pDX, IDC_FLDCONFIRM_TO_PATH, m_toPath);
	DDX_Text(pDX, IDC_FLDCONFIRM_QUERY, m_question);
	DDX_Check(pDX, IDC_FLDCONFIRM_DONTASKAGAIN, m_dontAskAgain);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ConfirmFolderCopyDlg, CTrDialog)
	ON_BN_CLICKED(IDNO, OnBnClickedNo)
	ON_BN_CLICKED(IDYES, OnBnClickedYes)
	ON_BN_CLICKED(IDC_FLDCONFIRM_DONTASKAGAIN, OnBnClickedDontAskAgain)
END_MESSAGE_MAP()


// ConfirmFolderCopyDlg message handlers

/**
 * @brief Handler for WM_INITDIALOG; conventional location to initialize
 * controls. At this point dialog and control windows exist.
 */
BOOL ConfirmFolderCopyDlg::OnInitDialog() 
{
	CTrDialog::OnInitDialog();

	UINT storedDecision = AfxGetApp()->GetProfileInt(REGISTRY_SECTION_MESSAGEBOX, _T("FolderCopyConfirmDlgDontAskAgain"), IDNO);
	if (storedDecision == IDYES)
		EndDialog(IDYES);
	else
	{
		// Load warning icon
		// TODO: we can have per-action icons?
		HICON icon = AfxGetApp()->LoadStandardIcon(IDI_EXCLAMATION);
		SendDlgItemMessage(IDC_FLDCONFIRM_ICON, STM_SETICON, (WPARAM)icon, 0L);

		if (!m_caption.empty())
			SetWindowText(m_caption.c_str());

		// setup handler for resizing this dialog	
		m_constraint.InitializeCurrentSize(this);
		m_constraint.DisallowHeightGrowth();
		m_constraint.SubclassWnd(); // install subclassing
		// persist size via registry
		m_constraint.LoadPosition(_T("ResizeableDialogs"), _T("FolderCopyConfirmDlg"), false);

		String strDontAskAgain = LoadResString(IDS_MESSAGEBOX_DONT_ASK_AGAIN);
		GetDlgItem(IDC_FLDCONFIRM_DONTASKAGAIN)->SetWindowText(strDontAskAgain.c_str());
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

/**
 * @brief Close dialog when No button is clicked.
 */
void ConfirmFolderCopyDlg::OnBnClickedNo()
{
	EndDialog(IDNO);
}

/**
 * @brief Close dialog when Yes button is clicked.
 */
void ConfirmFolderCopyDlg::OnBnClickedYes()
{
	EndDialog(IDYES);
}

/**
 * @brief Handle the "Don't ask again" checkbox, writing to the same registry key used by CMessageBoxDialog. 
 * It doesn't use its algorithm for generating a value name, but it doesn't matter, since it won't collide
 * with a value generated by CMessageBoxDialog and will also be reset by the Reset button in the Options dialog.
 */
void ConfirmFolderCopyDlg::OnBnClickedDontAskAgain()
{
	UpdateData();
	if (m_dontAskAgain)
		AfxGetApp()->WriteProfileInt(REGISTRY_SECTION_MESSAGEBOX, _T("FolderCopyConfirmDlgDontAskAgain"), IDYES);
	else
		AfxGetApp()->WriteProfileInt(REGISTRY_SECTION_MESSAGEBOX, _T("FolderCopyConfirmDlgDontAskAgain"), IDNO);
}
