//This file is part of JSLint Plugin for Notepad++
//Copyright (C) 2010 Martin Vladic <martin.vladic@gmail.com>
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#include "StdHeaders.h"
#include "OutputDlg.h"
#include "PluginDefinition.h"

OutputDlg::OutputDlg()
	: DockingDlgInterface(IDD_OUTPUT)
	, m_hWndListView(NULL)
{
}

OutputDlg::~OutputDlg()
{
	DestroyIcon(m_hTabIcon);
}

BOOL CALLBACK OutputDlg::run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
		case WM_INITDIALOG:
			InitializeListViewCtrl();
			break;

		case WM_NOTIFY: {
				LPNMHDR pNMHDR = (LPNMHDR) lParam;
				if (pNMHDR->code == NM_DBLCLK && pNMHDR->idFrom == IDC_OUTPUT) {
					LPNMITEMACTIVATE lpnmitem = (LPNMITEMACTIVATE) lParam;
					if (lpnmitem->iItem != -1) {
						ShowLint(lpnmitem->iItem);
					}
				}
			}
			break;

		case WM_SIZE: 
			::MoveWindow(m_hWndListView, 1, 1, LOWORD(lParam) - 2, HIWORD(lParam) - 2, TRUE);
			break;
	}

	return DockingDlgInterface::run_dlgProc(message, wParam, lParam);
}

void OutputDlg::InitializeListViewCtrl()
{
	m_hWndListView = ::GetDlgItem(_hSelf, IDC_OUTPUT);

	ListView_SetExtendedListViewStyle(m_hWndListView, LVS_EX_FULLROWSELECT);

	LVCOLUMN lvc;

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	lvc.iSubItem = COL_NUM;
	lvc.pszText = TEXT("No.");
	lvc.cx = 40;
	lvc.fmt = LVCFMT_RIGHT;
	ListView_InsertColumn(m_hWndListView, COL_NUM, &lvc);

	lvc.iSubItem = COL_REASON;
	lvc.pszText = TEXT("Reason");
	lvc.cx = 500;
	lvc.fmt = LVCFMT_LEFT;
	ListView_InsertColumn(m_hWndListView, COL_REASON, &lvc);

	lvc.iSubItem = COL_FILE;
	lvc.pszText = TEXT("File");
	lvc.cx = 200;
	lvc.fmt = LVCFMT_LEFT;
	ListView_InsertColumn(m_hWndListView, COL_FILE, &lvc);

	lvc.iSubItem = COL_LINE;
	lvc.pszText = TEXT("Line");
	lvc.cx = 50;
	lvc.fmt = LVCFMT_RIGHT;
	ListView_InsertColumn(m_hWndListView, COL_LINE, &lvc);

	lvc.iSubItem = COL_COLUMN;
	lvc.pszText = TEXT("Column");
	lvc.cx = 50;
	lvc.fmt = LVCFMT_RIGHT;
	ListView_InsertColumn(m_hWndListView, COL_COLUMN, &lvc);
}

HICON OutputDlg::GetTabIcon()
{
	if (m_hTabIcon == NULL) {
		m_hTabIcon = (HICON) ::LoadImage((HINSTANCE)g_hDllModule,
			MAKEINTRESOURCE(IDI_JSLINT_TAB), IMAGE_ICON, 0, 0,
			LR_LOADMAP3DCOLORS | LR_LOADTRANSPARENT );
	}
	return m_hTabIcon;
}

void OutputDlg::ClearAllLints()
{
	m_fileLints.clear();
	ListView_DeleteAllItems(m_hWndListView);
}

void OutputDlg::AddLints(const tstring& strFilePath, const list<JSLintReportItem>& lints)
{
	basic_stringstream<TCHAR> stream;

	LVITEM lvI;
	lvI.mask = LVIF_TEXT | LVIF_STATE;

	for (list<JSLintReportItem>::const_iterator it = lints.begin(); it != lints.end(); ++it) {
		const JSLintReportItem& lint = *it;

		lvI.iSubItem = 0;
		lvI.iItem = ListView_GetItemCount(m_hWndListView);
		lvI.state = 0;
		lvI.stateMask = 0;

		stream.str(TEXT(""));
		stream << lvI.iItem + 1;
		tstring strNum = stream.str();

		lvI.pszText = (LPTSTR)strNum.c_str();
		
		ListView_InsertItem(m_hWndListView, &lvI);

		tstring strReason = TextConversion::UTF8_To_T(lint.GetReason());
		ListView_SetItemText(m_hWndListView, lvI.iItem, COL_REASON, (LPTSTR)strReason.c_str());

		tstring strFile = Path::GetFileName(strFilePath);
		ListView_SetItemText(m_hWndListView, lvI.iItem, COL_FILE, (LPTSTR)strFile.c_str());

		stream.str(TEXT(""));
		stream << lint.GetLine();
		tstring strLine = stream.str();
		ListView_SetItemText(m_hWndListView, lvI.iItem, COL_LINE, (LPTSTR)strLine.c_str());
		
		stream.str(TEXT(""));
		stream << lint.GetCharacter();
		tstring strColumn = stream.str();
		ListView_SetItemText(m_hWndListView, lvI.iItem, COL_COLUMN, (LPTSTR)strColumn.c_str());

		m_fileLints.push_back(FileLint(strFilePath, lint));
	}
}

void OutputDlg::SelectNextLint()
{
	if (_hSelf == NULL)
		return;

	int count = ListView_GetItemCount(m_hWndListView);
	if (count == 0)
		return;

	int i = ListView_GetNextItem(m_hWndListView, -1, LVNI_FOCUSED | LVNI_SELECTED);
	if (++i == count)
		i = 0;
	
	ListView_SetItemState(m_hWndListView, i, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	ListView_EnsureVisible(m_hWndListView, i, FALSE);
	ShowLint(i);
}

void OutputDlg::SelectPrevLint()
{
	if (_hSelf == NULL)
		return;

	int count = ListView_GetItemCount(m_hWndListView);
	if (count == 0)
		return;

	int i = ListView_GetNextItem(m_hWndListView, -1, LVNI_FOCUSED | LVNI_SELECTED);
	if (--i == -1)
		i = count - 1;
	
	ListView_SetItemState(m_hWndListView, i, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	ListView_EnsureVisible(m_hWndListView, i, FALSE);
	ShowLint(i);
}

void OutputDlg::ShowLint(int i)
{
	const FileLint& fileLint = m_fileLints[i];
	
	int line = fileLint.lint.GetLine() - 1;
	int column = fileLint.lint.GetCharacter() - 1;
	
	if (!fileLint.strFilePath.empty() && line >= 0 && column >= 0) {
		LRESULT lRes = ::SendMessage(g_nppData._nppHandle, NPPM_SWITCHTOFILE, 0, (LPARAM)fileLint.strFilePath.c_str());
		if (lRes) {
			HWND hWndScintilla = getCurrentScintillaWindow();
			if (hWndScintilla != NULL) {
				::SendMessage(hWndScintilla, SCI_GOTOLINE, line, 0);
				int pos = (int) ::SendMessage(hWndScintilla, SCI_GETCURRENTPOS, 0, 0);
				for (int i = 0; i < column; i++)
					::SendMessage(hWndScintilla, SCI_CHARRIGHT, 0, 0);
			}
		}
	}
}