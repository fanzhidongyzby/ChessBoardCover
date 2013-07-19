
// OpenGLView.cpp : COpenGLView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "OpenGL.h"
#endif
#include "MainFrm.h"
#include "OpenGLDoc.h"
#include "OpenGLView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COpenGLView

IMPLEMENT_DYNCREATE(COpenGLView, CView)

	BEGIN_MESSAGE_MAP(COpenGLView, CView)
		ON_WM_CREATE()
		ON_WM_DESTROY()
		ON_WM_ERASEBKGND()
		ON_WM_SIZE()
		ON_WM_KEYDOWN()
		ON_WM_LBUTTONDOWN()
		ON_WM_LBUTTONUP()
		ON_WM_MOUSEMOVE()
		ON_WM_MOUSEWHEEL()
		ON_WM_TIMER()
	END_MESSAGE_MAP()

	Board* COpenGLView::board=NULL;
	Block * COpenGLView::tmpBlock=NULL;
	bool COpenGLView::runOver=false;
	int COpenGLView::width=0;
	int COpenGLView::height=0;
	float COpenGLView::angle=0;
	Viewer * COpenGLView::viewer=NULL;
	// COpenGLView ����/����
	COpenGLView::COpenGLView()
	{
		// TODO: �ڴ˴���ӹ������
		lbDown=false;
		board=new Board();
		angle=0;
		rotate=true;
		viewer=new Viewer();
	}
	COpenGLView::~COpenGLView()
	{
		delete board;
	}
	BOOL COpenGLView::PreCreateWindow(CREATESTRUCT& cs)
	{
		// TODO: �ڴ˴�ͨ���޸�
		//  CREATESTRUCT cs ���޸Ĵ��������ʽ
		//^v^
		cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
		//^v^
		return CView::PreCreateWindow(cs);
	}
	// COpenGLView ����
	void COpenGLView::OnDraw(CDC* /*pDC*/)
	{
		COpenGLDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		if (!pDoc)
			return;

		// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
		//^v^
		::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );// Clear out the color & depth buffers
		RenderScene(); //����Ļ�ͼ��������RC �л���
		glFinish();
		::SwapBuffers( m_pDC->GetSafeHdc());// Now Swap the buffers
		//^v^
	}
	
	void COpenGLView::RenderScene()
	{
		//��ʼ����ͼģʽ
		show2or3DView();
		glPushMatrix();
		glRotatef(angle,0,0,1);
		board->draw(GL_RENDER);
		glPopMatrix();
	}
	int COpenGLView::view_mode=2;
	void COpenGLView::show2or3DView()
	{
		switch(view_mode)
		{
		case 1:
			//��ʼ��2DͶӰx-y
			::glViewport(0, 0, width, height); // select the full client area
			// select the projection matrix and clear it
			::glMatrixMode(GL_PROJECTION);
			::glLoadIdentity();
			gluOrtho2D(-width/2, width/2, -height/2, height/2); // select the viewing volume
			// switch back to the modelview matrix and clear it
			::glMatrixMode(GL_MODELVIEW);
			::glLoadIdentity();
			break;
		case 2:
			//3DͶӰ z -> x-y
			glViewport(0,0,width,height);			// ����OpenGL�ӿڴ�С��	
			glMatrixMode(GL_PROJECTION);			// ���õ�ǰ����ΪͶӰ����
			glLoadIdentity();						// ���õ�ǰָ���ľ���Ϊ��λ����
			gluPerspective							// ����͸��ͼ
				( 60,							// ͸�ӽ�����Ϊ 60 ��
				(GLfloat)width/(GLfloat)height,	// ���ڵĿ���߱�
				1.0f,								// ��Ұ͸�����:����1.0f
				2000							// ��Ұ͸�����:ʼ��0.1fԶ��1000.0f
				);
			// �������������ƣ���һ���������þ�ͷ��Ƕȣ��ڶ��������ǳ���ȣ�������Զ�����С�
			viewer->look();
			//glMatrixMode(GL_MODELVIEW);				// ���õ�ǰ����Ϊģ����ͼ����
			//glLoadIdentity();					// ���õ�ǰָ���ľ���Ϊ��λ����
			//gluLookAt(60,-400,300,0,0,0,0.0,1.0,0.0);
			//gluLookAt(30,-200,150,0,0,0,0.0,1.0,0.0);
			//gluLookAt(0,0,800,0,0,0,0.0,1.0,0.0);
			//viewer->look();
			//������ȼ��
			glEnable(GL_DEPTH_TEST);
			//glEnable(GL_LIGHT0);//���0�Ź�Դ			
			//�����������
			glEnable (GL_LINE_SMOOTH);
			glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
			glEnable (GL_BLEND);
			glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			//�����������ƫ��
			glPolygonOffset(1,1);
			glEnable(GL_POLYGON_OFFSET_FILL);
			break;
		}
	}
	// COpenGLView ���
#ifdef _DEBUG
	void COpenGLView::AssertValid() const
	{
		CView::AssertValid();
	}
	void COpenGLView::Dump(CDumpContext& dc) const
	{
		CView::Dump(dc);
	}
	COpenGLDoc* COpenGLView::GetDocument() const // �ǵ��԰汾��������
	{
		ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COpenGLDoc)));
		return (COpenGLDoc*)m_pDocument;
	}
#endif //_DEBUG
	//^v^
	bool COpenGLView::InitializeOpenGL()
	{
		//Get a DC for the Client Area
		m_pDC = new CClientDC(this);
		//Failure to Get DC
		if(m_pDC == NULL)
		{
			MessageBox("Error Obtaining DC");
			return FALSE;
		}
		//Failure to set the pixel format
		if(!SetupPixelFormat())
		{
			return FALSE;
		}
		//Create Rendering Context
		m_hRC = ::wglCreateContext (m_pDC->GetSafeHdc ());
		//Failure to Create Rendering Context
		if(m_hRC == 0)
		{
			MessageBox("Error Creating RC");
			return FALSE;
		}
		//Make the RC Current
		if(::wglMakeCurrent (m_pDC->GetSafeHdc (), m_hRC)==FALSE)
		{
			MessageBox("Error making RC Current");
			return FALSE;
		}
		//Specify Black as the clear color
		//::glClearColor(1.0f,1.0f,1.0f,1.0f);
		::glClearColor(0.1f,0.1f,0.4f,1.0f);
		//::glClearColor(1,1,1,0);
		//Specify the back of the buffer as clear depth
		::glClearDepth(1.0f);
		//Enable Depth Testing
		::glEnable(GL_DEPTH_TEST);
		SetTimer(20,10,NULL);
		return true;
	}
	bool COpenGLView::SetupPixelFormat()
	{
		static PIXELFORMATDESCRIPTOR pfd =
		{
			sizeof(PIXELFORMATDESCRIPTOR), // size of this pfd
			1, // version number
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL // support window | support OpenGL|
			| PFD_DOUBLEBUFFER , //double buffered
			PFD_TYPE_RGBA, // RGBA type
			24, // 24-bit color depth
			0, 0, 0, 0, 0, 0, // color bits ignored
			0, 0, // no alpha buffer // shift bit ignored
			0, // no accumulation buffer
			0, 0, 0, 0, // accum bits ignored
			16, 0, // 16-bit z-buffer // no stencil buffer
			0, PFD_MAIN_PLANE, 0, // no auxiliary buffer // main layer// reserved
			0, 0, 0 // layer masks ignored
		};
		int m_nPixelFormat = ::ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd);
		if ( m_nPixelFormat == 0 ) 
			return FALSE;
		if ( ::SetPixelFormat(m_pDC->GetSafeHdc(), m_nPixelFormat, &pfd) == FALSE) 
			return FALSE;
		return true;
	}
	// COpenGLView ��Ϣ�������
	int COpenGLView::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (CView::OnCreate(lpCreateStruct) == -1)
			return -1;

		// TODO:  �ڴ������ר�õĴ�������
		//^v^
		InitializeOpenGL();
		//^v^
		return 0;
	}
	void COpenGLView::OnDestroy()
	{
		CView::OnDestroy();

		// TODO: �ڴ˴������Ϣ����������
		wglMakeCurrent(m_pDC->m_hDC,NULL); //�ͷ���m_hDC ��Ӧ��RC
		wglDeleteContext(m_hRC); //ɾ��RC if (m_pDC)
		delete m_pDC; //ɾ����ǰView ӵ�е�DC
	}
	BOOL COpenGLView::OnEraseBkgnd(CDC* pDC)
	{
		// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

		//return CView::OnEraseBkgnd(pDC);
		//����仰ע�͵�������Window ���ð�ɫ������ˢ��,���»�����˸
		return TRUE; //ֻҪ�շ��ؼ��ɡ�
	}
	void COpenGLView::OnSize(UINT nType, int cx, int cy)
	{
		CView::OnSize(nType, cx, cy);

		// TODO: �ڴ˴������Ϣ����������
		if ( 0 >= cx || 0 >= cy ) return;
		width = cx;
		height = cy;
	}
	//^v^

	void COpenGLView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
		if(nChar==VK_ESCAPE)
		{
			CMainFrame *pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
			pFrame->switchScr(false);
		}
		if(nChar==VK_UP)
		{
			viewer->up_down(true);
			Invalidate();
		}
		if(nChar==VK_DOWN)
		{
			viewer->up_down(false);
			Invalidate();
		}
		if(nChar==VK_LEFT)
		{
			angle-=2;
			if(angle<=-360)angle=0;
			Invalidate();
		}
		if(nChar==VK_RIGHT)
		{
			angle+=2;
			if(angle>=360)angle=0;
			Invalidate();
		}
		if(nChar==VK_F5)
		{
			if(runOver)
			{
				runOver=false;
				delete board;
				board=new Board();
				Invalidate();
			}			
		}
		if(nChar==VK_SPACE)
		{
			rotate=!rotate;
		}
		CView::OnKeyDown(nChar, nRepCnt, nFlags);
	}


	void COpenGLView::OnLButtonDown(UINT nFlags, CPoint point)
	{
		// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
		lastLBPos=point;
		lbDown=true;
		Invalidate();
		CView::OnLButtonDown(nFlags, point);
	}


	void COpenGLView::OnLButtonUp(UINT nFlags, CPoint point)
	{
		// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
		lbDown=false;
		lastLBPos=point;//��¼��ʷ��
		CView::OnLButtonUp(nFlags, point);
	}


	void COpenGLView::OnMouseMove(UINT nFlags, CPoint point)
	{
		// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
		
		if(lbDown)
		{
			angle+=point.x-lastLBPos.x;
			if(angle>=360)angle=0;
			if(angle<=-360)angle=0;
			Invalidate();//ˢ��
		}
		lastLBPos=point;//��¼��ʷ��
		CView::OnMouseMove(nFlags, point);
	}


	BOOL COpenGLView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
	{
		// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
		viewer->near_far(zDelta<0);
		Invalidate();
		return CView::OnMouseWheel(nFlags, zDelta, pt);
	}


	void COpenGLView::OnTimer(UINT_PTR nIDEvent)
	{
		// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
		if(nIDEvent==20)
		{
			if(rotate)
			{
				//��ת
				angle+=0.5;
				if(angle>=360)
					angle-=360;
			}
			//����
			if(tmpBlock!=NULL)//�����¿�
			{
				if(!tmpBlock->drop())//������Ϸ�
					tmpBlock=NULL;
			}
			board->pickSquares(lastLBPos.x,lastLBPos.y);
			board->allJump();
			Invalidate();
		}
		CView::OnTimer(nIDEvent);
	}

