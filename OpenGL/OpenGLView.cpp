
// OpenGLView.cpp : COpenGLView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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
	// COpenGLView 构造/析构
	COpenGLView::COpenGLView()
	{
		// TODO: 在此处添加构造代码
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
		// TODO: 在此处通过修改
		//  CREATESTRUCT cs 来修改窗口类或样式
		//^v^
		cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
		//^v^
		return CView::PreCreateWindow(cs);
	}
	// COpenGLView 绘制
	void COpenGLView::OnDraw(CDC* /*pDC*/)
	{
		COpenGLDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		if (!pDoc)
			return;

		// TODO: 在此处为本机数据添加绘制代码
		//^v^
		::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );// Clear out the color & depth buffers
		RenderScene(); //具体的绘图函数，在RC 中绘制
		glFinish();
		::SwapBuffers( m_pDC->GetSafeHdc());// Now Swap the buffers
		//^v^
	}
	
	void COpenGLView::RenderScene()
	{
		//初始化视图模式
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
			//初始化2D投影x-y
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
			//3D投影 z -> x-y
			glViewport(0,0,width,height);			// 设置OpenGL视口大小。	
			glMatrixMode(GL_PROJECTION);			// 设置当前矩阵为投影矩阵。
			glLoadIdentity();						// 重置当前指定的矩阵为单位矩阵
			gluPerspective							// 设置透视图
				( 60,							// 透视角设置为 60 度
				(GLfloat)width/(GLfloat)height,	// 窗口的宽与高比
				1.0f,								// 视野透视深度:近点1.0f
				2000							// 视野透视深度:始点0.1f远点1000.0f
				);
			// 这和照象机很类似，第一个参数设置镜头广角度，第二个参数是长宽比，后面是远近剪切。
			viewer->look();
			//glMatrixMode(GL_MODELVIEW);				// 设置当前矩阵为模型视图矩阵
			//glLoadIdentity();					// 重置当前指定的矩阵为单位矩阵
			//gluLookAt(60,-400,300,0,0,0,0.0,1.0,0.0);
			//gluLookAt(30,-200,150,0,0,0,0.0,1.0,0.0);
			//gluLookAt(0,0,800,0,0,0,0.0,1.0,0.0);
			//viewer->look();
			//允许深度检测
			glEnable(GL_DEPTH_TEST);
			//glEnable(GL_LIGHT0);//添加0号光源			
			//允许反走样混合
			glEnable (GL_LINE_SMOOTH);
			glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
			glEnable (GL_BLEND);
			glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			//允许多边形填充偏移
			glPolygonOffset(1,1);
			glEnable(GL_POLYGON_OFFSET_FILL);
			break;
		}
	}
	// COpenGLView 诊断
#ifdef _DEBUG
	void COpenGLView::AssertValid() const
	{
		CView::AssertValid();
	}
	void COpenGLView::Dump(CDumpContext& dc) const
	{
		CView::Dump(dc);
	}
	COpenGLDoc* COpenGLView::GetDocument() const // 非调试版本是内联的
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
	// COpenGLView 消息处理程序
	int COpenGLView::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (CView::OnCreate(lpCreateStruct) == -1)
			return -1;

		// TODO:  在此添加您专用的创建代码
		//^v^
		InitializeOpenGL();
		//^v^
		return 0;
	}
	void COpenGLView::OnDestroy()
	{
		CView::OnDestroy();

		// TODO: 在此处添加消息处理程序代码
		wglMakeCurrent(m_pDC->m_hDC,NULL); //释放与m_hDC 对应的RC
		wglDeleteContext(m_hRC); //删除RC if (m_pDC)
		delete m_pDC; //删除当前View 拥有的DC
	}
	BOOL COpenGLView::OnEraseBkgnd(CDC* pDC)
	{
		// TODO: 在此添加消息处理程序代码和/或调用默认值

		//return CView::OnEraseBkgnd(pDC);
		//把这句话注释掉，否则，Window 会用白色背景来刷新,导致画面闪烁
		return TRUE; //只要空返回即可。
	}
	void COpenGLView::OnSize(UINT nType, int cx, int cy)
	{
		CView::OnSize(nType, cx, cy);

		// TODO: 在此处添加消息处理程序代码
		if ( 0 >= cx || 0 >= cy ) return;
		width = cx;
		height = cy;
	}
	//^v^

	void COpenGLView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		// TODO: 在此添加消息处理程序代码和/或调用默认值
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
		// TODO: 在此添加消息处理程序代码和/或调用默认值
		lastLBPos=point;
		lbDown=true;
		Invalidate();
		CView::OnLButtonDown(nFlags, point);
	}


	void COpenGLView::OnLButtonUp(UINT nFlags, CPoint point)
	{
		// TODO: 在此添加消息处理程序代码和/或调用默认值
		lbDown=false;
		lastLBPos=point;//记录历史点
		CView::OnLButtonUp(nFlags, point);
	}


	void COpenGLView::OnMouseMove(UINT nFlags, CPoint point)
	{
		// TODO: 在此添加消息处理程序代码和/或调用默认值
		
		if(lbDown)
		{
			angle+=point.x-lastLBPos.x;
			if(angle>=360)angle=0;
			if(angle<=-360)angle=0;
			Invalidate();//刷新
		}
		lastLBPos=point;//记录历史点
		CView::OnMouseMove(nFlags, point);
	}


	BOOL COpenGLView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
	{
		// TODO: 在此添加消息处理程序代码和/或调用默认值
		viewer->near_far(zDelta<0);
		Invalidate();
		return CView::OnMouseWheel(nFlags, zDelta, pt);
	}


	void COpenGLView::OnTimer(UINT_PTR nIDEvent)
	{
		// TODO: 在此添加消息处理程序代码和/或调用默认值
		if(nIDEvent==20)
		{
			if(rotate)
			{
				//旋转
				angle+=0.5;
				if(angle>=360)
					angle-=360;
			}
			//降落
			if(tmpBlock!=NULL)//生成新块
			{
				if(!tmpBlock->drop())//降落完毕否
					tmpBlock=NULL;
			}
			board->pickSquares(lastLBPos.x,lastLBPos.y);
			board->allJump();
			Invalidate();
		}
		CView::OnTimer(nIDEvent);
	}

