
// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "OpenGL.h"
#include "OpenGLView.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

	BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
		ON_COMMAND(ID_FULLSCR, &CMainFrame::OnFullscr)
		ON_WM_GETMINMAXINFO()
		ON_COMMAND(ID_HELP, &CMainFrame::OnHelp)
	END_MESSAGE_MAP()

	// CMainFrame ����/����

	CMainFrame::CMainFrame()
	{
		// TODO: �ڴ���ӳ�Ա��ʼ������
		m_bFullScreen=false;
	}

	CMainFrame::~CMainFrame()
	{
	}

	BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
	{
		if( !CFrameWnd::PreCreateWindow(cs) )
			return FALSE;
		// TODO: �ڴ˴�ͨ���޸�
		//  CREATESTRUCT cs ���޸Ĵ��������ʽ
		cs.cx=900;
		cs.cy=700;
		cs.x=100;
		cs.y=10;
		return TRUE;
	}

	// CMainFrame ���

#ifdef _DEBUG
	void CMainFrame::AssertValid() const
	{
		CFrameWnd::AssertValid();
	}

	void CMainFrame::Dump(CDumpContext& dc) const
	{
		CFrameWnd::Dump(dc);
	}
#endif //_DEBUG


	// CMainFrame ��Ϣ�������


	//void CMainFrame::On2d()
	//{
	//	// TODO: �ڴ���������������
	//	CMenu *mainMenu=::AfxGetApp()->m_pMainWnd->GetMenu();
	//	mainMenu->CheckMenuItem(ID_3D,MF_UNCHECKED);
	//	mainMenu->CheckMenuItem(ID_2D,MF_CHECKED);
	//	COpenGLView::view_mode=1;
	//	this->Invalidate();
	//}

	void CMainFrame::switchScr(bool autoSw)
	{
		if(m_bFullScreen)
		{
			SetWindowPos(&CWnd::wndNoTopMost,100,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
			//ShowWindow(SW_HIDE);
			SetWindowPlacement(&m_OldWndPlacement);
			m_bFullScreen=false;
		}
		else
		{
			if(autoSw==false)
				return;
			GetWindowPlacement(&m_OldWndPlacement);
			CRect WindowRect;
			GetWindowRect(&WindowRect);
			CRect ClientRect;
			RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST, reposQuery, &ClientRect);
			ClientToScreen(&ClientRect);
			// ��ȡ��Ļ�ķֱ���
			int nFullWidth=GetSystemMetrics(SM_CXSCREEN);
			int nFullHeight=GetSystemMetrics(SM_CYSCREEN);
			// ������������Ŀͻ���ȫ����ʾ����(0,0)��(nFullWidth, nFullHeight)����, ��(0,0)��(nFullWidth, nFullHeight)������������ԭ���ںͳ�������֮��Ŀ�
			//����λ�ü�Ĳ�ֵ, �͵õ�ȫ����ʾ�Ĵ���λ��
			m_FullScreenRect.left=WindowRect.left-ClientRect.left-2;
			m_FullScreenRect.top=WindowRect.top-ClientRect.top-2;
			m_FullScreenRect.right=WindowRect.right-ClientRect.right+nFullWidth+2;
			m_FullScreenRect.bottom=WindowRect.bottom-ClientRect.bottom+nFullHeight+2;
			m_bFullScreen=true; // ����ȫ����ʾ��־Ϊ TRUE
			// ����ȫ����ʾ״̬
			WINDOWPLACEMENT wndpl;
			wndpl.length=sizeof(WINDOWPLACEMENT);
			wndpl.flags=0;
			wndpl.showCmd=SW_SHOWNORMAL;
			wndpl.rcNormalPosition=m_FullScreenRect;
			SetWindowPlacement(&wndpl);
			SetWindowPos(&CWnd::wndTopMost,100,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
		}
	}

	void CMainFrame::OnFullscr()
	{
		// TODO: �ڴ���������������
		switchScr();
	}


	void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
	{
		// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
		if(m_bFullScreen)
		{
			lpMMI->ptMaxSize.x=m_FullScreenRect.Width();
			lpMMI->ptMaxSize.y=m_FullScreenRect.Height();
			lpMMI->ptMaxPosition.x=0;
			lpMMI->ptMaxPosition.y=0;
			// ����Track�ߴ�ҲҪ�ı�
			lpMMI->ptMaxTrackSize.x=m_FullScreenRect.Width();
			lpMMI->ptMaxTrackSize.y=m_FullScreenRect.Height();
		}
		CFrameWnd::OnGetMinMaxInfo(lpMMI);
	}



	void CMainFrame::OnHelp()
	{
		// TODO: �ڴ���������������
		MessageBox("ȫ���л�:F11,ESC;\n\
�����л�:F5;\n\
���̿���:�����ק,�������Ҽ�;\n\
�������:������,�����ק,�ո��;\n\
��Ч����:�Զ�;\n");
	}
