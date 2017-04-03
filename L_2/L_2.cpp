#include <afxwin.h>
#include "bitmap.h"
#define ID_M 2002
#define ID_F 2004
HBITMAP hbit;
int choise = 0;
RECT r1;
CString path;
class CMyMainWnd : public CFrameWnd
{
public:
	CMyMainWnd()
	{
		Create(NULL, L"My title");
	}
	RECT r;
	int x;
	int y;
	CMenu *menu;
	afx_msg int OnCreate(LPCREATESTRUCT);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT flags, CPoint Loc);
	afx_msg void OnLButtonUp(UINT flags, CPoint Loc);
	void Open();
	void Save();
	DECLARE_MESSAGE_MAP();
};
class CMyApp : public CWinApp
{
public:
	CMyApp(){};
	virtual BOOL InitInstance()
	{
		m_pMainWnd = new CMyMainWnd();
		m_pMainWnd->ShowWindow(SW_SHOW);
		return TRUE;
	}
};
BEGIN_MESSAGE_MAP(CMyMainWnd, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_M, Open)
	ON_COMMAND(ID_F, Save)
END_MESSAGE_MAP()

afx_msg void CMyMainWnd::OnPaint()
{
	
		ShowBitMap(m_hWnd, r, hbit, x, y);
		if (choise == 2)
		{
			choise = 1;
			ClientToBmp(m_hWnd, r,path );
			path = "";
		}
}

afx_msg int CMyMainWnd::OnCreate(LPCREATESTRUCT)
{
	menu = new CMenu();
	menu->CreateMenu();
	CMenu *subMenu = new CMenu();
	subMenu->CreatePopupMenu();
	subMenu->AppendMenu(MF_STRING, ID_M, _T("Open image"));
	subMenu->AppendMenu(MF_STRING, ID_F, _T("Save bmp file"));
	this->menu->AppendMenu(MF_POPUP | MF_STRING, (UINT)subMenu->m_hMenu, _T("Tests"));
	SetMenu(menu);
	return 0;
}

afx_msg void CMyMainWnd::OnLButtonDown(UINT flags, CPoint Loc)
{
	
		r.left = Loc.x;
		r.top = Loc.y;

}

afx_msg void CMyMainWnd::OnLButtonUp(UINT flags, CPoint Loc)
{
		r.right = Loc.x;
		r.bottom = Loc.y;
	
}
void CMyMainWnd::Open()
{
	CFileDialog fileDialog(TRUE);
	int result = fileDialog.DoModal();
	if (result == IDOK)
	{
		hbit = (HBITMAP)LoadImage(0, fileDialog.GetPathName(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		choise = 1;
		x = r.left;
		y = r.top;
		InvalidateRect(0);
	}
}

void CMyMainWnd::Save()
{
	
	CFileDialog fileDialog(FALSE);
	int result = fileDialog.DoModal();
	if (result == IDOK)
	{
		path = fileDialog.GetPathName();
		choise = 2;
	}
	InvalidateRect(0);
}

CMyApp theApp;