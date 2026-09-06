/*
 * Copyright (c) 2026, abart27 (https://github.com/abart27).
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

// This demo dialog is a Win32 port of the one from msstyleEditor (cf.
// https://github.com/nptr/msstyleEditor/blob/master/msstyleEditorSharp/Dialogs/ControlDemoDialog.cs)

/*
 * MIT License
 *
 * Copyright (c) 2019 Jakob K.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma warning(push, 0)
#include "../WinDarkMode.h"
#pragma warning(pop)

#include <CommCtrl.h>
#include <commdlg.h>
#include <string>
#include "resource.h"

static HWND m_hwnd_status = nullptr;

static void set_theme(WinDarkMode::Theme theme)
{
    WinDarkMode::set(theme);

    const wchar_t *label = L"Theme: System";
    if (theme == WinDarkMode::Theme::Light)
        label = L"Theme: Light";
    else if (theme == WinDarkMode::Theme::Dark)
        label = L"Theme: Dark";

    if (m_hwnd_status) SendMessage(m_hwnd_status, SB_SETTEXT, 0, reinterpret_cast<LPARAM>(label));
}

static void setup_listview(HWND hwnd)
{
    HWND hLV = GetDlgItem(hwnd, IDC_LISTVIEW);

    LVCOLUMNW lvc = {};
    lvc.mask = LVCF_TEXT | LVCF_WIDTH;
    wchar_t col1[] = L"Col 1";
    wchar_t col2[] = L"Col 2";
    lvc.cx = 40;
    lvc.pszText = col1;
    ListView_InsertColumn(hLV, 0, &lvc);
    lvc.cx = 36;
    lvc.pszText = col2;
    ListView_InsertColumn(hLV, 1, &lvc);

    ListView_EnableGroupView(hLV, TRUE);
    LVGROUP grp = {};
    grp.cbSize = sizeof(LVGROUP);
    grp.mask = LVGF_HEADER | LVGF_GROUPID;
    wchar_t grp1[] = L"Group 1";
    wchar_t grp2[] = L"Group 2";
    grp.iGroupId = 0;
    grp.pszHeader = grp1;
    ListView_InsertGroup(hLV, -1, &grp);
    grp.iGroupId = 1;
    grp.pszHeader = grp2;
    ListView_InsertGroup(hLV, -1, &grp);

    wchar_t it1[] = L"Item 1";
    wchar_t it2[] = L"Item 2";
    LVITEMW lvi = {};
    lvi.mask = LVIF_TEXT | LVIF_GROUPID;
    lvi.iGroupId = 0;
    lvi.iItem = 0;
    lvi.pszText = it1;
    ListView_InsertItem(hLV, &lvi);
    ListView_SetItemText(hLV, 0, 1, it2);

    lvi.iGroupId = 1;
    lvi.iItem = 1;
    lvi.pszText = it1;
    ListView_InsertItem(hLV, &lvi);
    ListView_SetItemText(hLV, 1, 1, it2);
}

static void setup_treeview(HWND hwnd)
{
    HWND hTV = GetDlgItem(hwnd, IDC_TREEVIEW);

    TVINSERTSTRUCTW tvi = {};
    tvi.hInsertAfter = TVI_LAST;

    wchar_t item1[] = L"Item 1";
    tvi.hParent = TVI_ROOT;
    tvi.item.mask = TVIF_TEXT;
    tvi.item.pszText = item1;
    HTREEITEM hItem1 = TreeView_InsertItem(hTV, &tvi);

    wchar_t sub1[] = L"SubItem 1";
    wchar_t sub2[] = L"SubItem 2";
    tvi.hParent = hItem1;
    tvi.item.pszText = sub1;
    TreeView_InsertItem(hTV, &tvi);
    tvi.item.pszText = sub2;
    TreeView_InsertItem(hTV, &tvi);

    wchar_t item2[] = L"Item 2";
    tvi.hParent = TVI_ROOT;
    tvi.item.pszText = item2;
    HTREEITEM hItem2 = TreeView_InsertItem(hTV, &tvi);

    tvi.hParent = hItem2;
    tvi.item.pszText = sub1;
    TreeView_InsertItem(hTV, &tvi);

    wchar_t item3[] = L"Item 3";
    tvi.hParent = TVI_ROOT;
    tvi.item.pszText = item3;
    TreeView_InsertItem(hTV, &tvi);

    TreeView_Expand(hTV, hItem1, TVE_EXPAND);
    TreeView_Expand(hTV, hItem2, TVE_EXPAND);
}

static void reflow(HWND hwnd)
{
    RECT rc = {};
    GetClientRect(hwnd, &rc);
    const int cx = rc.right;
    const int cy = rc.bottom;

    int sbh = 0;
    if (m_hwnd_status)
    {
        SendMessage(m_hwnd_status, WM_SIZE, 0, MAKELPARAM(cx, cy));
        RECT sbr = {};
        GetWindowRect(m_hwnd_status, &sbr);
        sbh = sbr.bottom - sbr.top;
    }

    const int vsbW = GetSystemMetrics(SM_CXVSCROLL);
    const int hsbH = GetSystemMetrics(SM_CYHSCROLL);
    const int avail = cy - sbh;

    if (HWND h = GetDlgItem(hwnd, IDC_HSCROLL))
        SetWindowPos(h, nullptr, 0, avail - hsbH, cx - vsbW, hsbH, SWP_NOZORDER | SWP_NOACTIVATE);
    if (HWND h = GetDlgItem(hwnd, IDC_VSCROLL))
        SetWindowPos(h, nullptr, cx - vsbW, 0, vsbW, avail - hsbH, SWP_NOZORDER | SWP_NOACTIVATE);
}

static INT_PTR CALLBACK DlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (msg)
    {
    case WM_INITDIALOG: {
        m_hwnd_status = CreateWindowExW(0, STATUSCLASSNAME, nullptr, WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP, 0, 0, 0, 0,
                                        hwnd, reinterpret_cast<HMENU>(IDC_STATUS), GetModuleHandleW(nullptr), nullptr);
        int sbParts[] = {220, -1};
        SendMessage(m_hwnd_status, SB_SETPARTS, 2, reinterpret_cast<LPARAM>(sbParts));
        SendMessage(m_hwnd_status, SB_SETTEXT, 0, reinterpret_cast<LPARAM>(L"Theme: System (follow OS)"));
        SendMessage(m_hwnd_status, SB_SETTEXT, 1, reinterpret_cast<LPARAM>(L"Panel 1"));

        CheckRadioButton(hwnd, IDC_THEME_LIGHT, IDC_THEME_SYSTEM, IDC_THEME_SYSTEM);

        CheckRadioButton(hwnd, IDC_DEMO_RADIO1, IDC_DEMO_RADIO3, IDC_DEMO_RADIO3);

        CheckDlgButton(hwnd, IDC_CHK3, BST_CHECKED);
        CheckDlgButton(hwnd, IDC_CHK4, BST_CHECKED);
        CheckDlgButton(hwnd, IDC_CHK5, BST_INDETERMINATE);
        CheckDlgButton(hwnd, IDC_CHK6, BST_INDETERMINATE);

        SetDlgItemTextW(hwnd, IDC_EDIT_NORMAL, L"normal");
        SetDlgItemTextW(hwnd, IDC_EDIT_DISABLED, L"disabled");
        SetDlgItemTextW(hwnd, IDC_EDIT_PASSWORD, L"password");
        SetDlgItemTextW(hwnd, IDC_EDIT_READONLY, L"readonly");

        static const wchar_t *const kItems[] = {L"Item 1", L"Item 2", L"Item 3"};
        for (int id : {IDC_COMBO1, IDC_COMBO2, IDC_COMBO3})
        {
            HWND hC = GetDlgItem(hwnd, id);
            for (const wchar_t *it : kItems) SendMessage(hC, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(it));
            SendMessage(hC, CB_SETCURSEL, 0, 0);
        }
        SetDlgItemTextW(hwnd, IDC_COMBO1, L"Item 1");

        HWND hSpinner =
            CreateWindowW(L"msctls_updown32", nullptr,
                          WS_CHILD | WS_VISIBLE | UDS_SETBUDDYINT | UDS_ALIGNRIGHT | UDS_ARROWKEYS | UDS_NOTHOUSANDS, 0,
                          0, 0, 0, hwnd, reinterpret_cast<HMENU>(IDC_SPINNER), GetModuleHandleW(nullptr), nullptr);
        SendMessage(hSpinner, UDM_SETBUDDY, reinterpret_cast<WPARAM>(GetDlgItem(hwnd, IDC_SPINNER_EDIT)), 0);
        SendMessage(hSpinner, UDM_SETRANGE, 0, MAKELPARAM(100, 0));
        SendMessage(hSpinner, UDM_SETPOS32, 0, 50);

        HWND hTab = GetDlgItem(hwnd, IDC_TAB1);
        TCITEMW tci = {};
        tci.mask = TCIF_TEXT;
        const wchar_t *tabs[] = {L"Tab 1", L"Tab 2", L"Tab 3", L"Tab 4"};
        for (int i = 0; i < 4; ++i)
        {
            tci.pszText = const_cast<LPWSTR>(tabs[i]);
            TabCtrl_InsertItem(hTab, i, &tci);
        }

        SendDlgItemMessage(hwnd, IDC_PROG_NORMAL, PBM_SETRANGE32, 0, 100);
        SendDlgItemMessage(hwnd, IDC_PROG_ERROR, PBM_SETRANGE32, 0, 100);
        SendDlgItemMessage(hwnd, IDC_PROG_PAUSED, PBM_SETRANGE32, 0, 100);

        SendDlgItemMessage(hwnd, IDC_PROG_NORMAL, PBM_SETPOS, 70, 0);
        SendDlgItemMessage(hwnd, IDC_PROG_ERROR, PBM_SETPOS, 50, 0);
        SendDlgItemMessage(hwnd, IDC_PROG_PAUSED, PBM_SETPOS, 30, 0);

        SendDlgItemMessage(hwnd, IDC_PROG_ERROR, PBM_SETSTATE, PBST_ERROR, 0);
        SendDlgItemMessage(hwnd, IDC_PROG_PAUSED, PBM_SETSTATE, PBST_PAUSED, 0);
        SendDlgItemMessage(hwnd, IDC_PROG_MARQUEE, PBM_SETMARQUEE, TRUE, 50);

        HWND hLB = GetDlgItem(hwnd, IDC_LISTBOX);
        for (const wchar_t *it : kItems) SendMessage(hLB, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(it));

        setup_listview(hwnd);
        setup_treeview(hwnd);

        WinDarkMode::attach(hwnd, { .is_dialog = true });
        reflow(hwnd);
        return TRUE;
    }
    case WM_SIZE:
        reflow(hwnd);
        return FALSE;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_THEME_LIGHT:
            set_theme(WinDarkMode::Theme::Light);
            return TRUE;
        case IDC_THEME_DARK:
            set_theme(WinDarkMode::Theme::Dark);
            return TRUE;
        case IDC_THEME_SYSTEM:
            set_theme(WinDarkMode::Theme::System);
            return TRUE;
        case IDC_BUTTON1:
            MessageBoxW(hwnd,
                        L"Hello from WinDarkMode!\n\n"
                        L"This dialog showcases the full set of common Win32 controls\n"
                        L"with dark mode applied via WinDarkMode::attach().",
                        L"WinDarkMode Demo", MB_OK | MB_ICONINFORMATION);
            return TRUE;
        case IDM_CLOSE:
            DestroyWindow(hwnd);
            return TRUE;
        case IDM_TOGGLE_WINDOW: {
            LONG_PTR ex = GetWindowLongPtrW(hwnd, GWL_EXSTYLE);
            SetWindowLongPtrW(hwnd, GWL_EXSTYLE,
                              (ex & WS_EX_TOOLWINDOW) ? (ex & ~static_cast<LONG_PTR>(WS_EX_TOOLWINDOW))
                                                      : (ex | static_cast<LONG_PTR>(WS_EX_TOOLWINDOW)));
            SetWindowPos(hwnd, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
            WinDarkMode::attach(hwnd);
            return TRUE;
        }
        case IDM_FILE_DLG: {
            OPENFILENAMEW ofn = {};
            wchar_t szFile[MAX_PATH] = {};
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = hwnd;
            ofn.lpstrFile = szFile;
            ofn.nMaxFile = MAX_PATH;
            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
            GetOpenFileNameW(&ofn);
            return TRUE;
        }
        case IDM_COLOR_DLG: {
            static COLORREF custColors[16] = {};
            CHOOSECOLORW cc = {};
            cc.lStructSize = sizeof(cc);
            cc.hwndOwner = hwnd;
            cc.rgbResult = RGB(64, 128, 192);
            cc.lpCustColors = custColors;
            cc.Flags = CC_FULLOPEN | CC_RGBINIT;
            ChooseColorW(&cc);
            return TRUE;
        }
        case IDCANCEL:
            DestroyWindow(hwnd);
            return TRUE;
        }
        return FALSE;
    case WM_CLOSE:
        DestroyWindow(hwnd);
        return TRUE;
    case WM_DESTROY:
        m_hwnd_status = nullptr;
        PostQuitMessage(0);
        return TRUE;
    }
    return FALSE;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int nShowCmd)
{
    INITCOMMONCONTROLSEX icc = {};
    icc.dwSize = sizeof(icc);
    icc.dwICC = ICC_WIN95_CLASSES | ICC_TAB_CLASSES | ICC_BAR_CLASSES | ICC_PROGRESS_CLASS | ICC_UPDOWN_CLASS |
                ICC_LISTVIEW_CLASSES | ICC_TREEVIEW_CLASSES | ICC_STANDARD_CLASSES;
    InitCommonControlsEx(&icc);

    WinDarkMode::init();

    HWND hwnd = CreateDialogParamW(hInstance, MAKEINTRESOURCE(IDD_MAIN_DIALOG), nullptr, DlgProc, 0);

    if (!hwnd) return 1;

    ShowWindow(hwnd, nShowCmd);
    UpdateWindow(hwnd);

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!IsDialogMessage(hwnd, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return static_cast<int>(msg.wParam);
}
