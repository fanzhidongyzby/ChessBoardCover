
// OpenGLView.h : COpenGLView ��Ľӿ�
//

#pragma once
#include "Block.h"
#include "OpenGLDoc.h"
#include "Board.h"
#include "Viewer.h"

class COpenGLView : public CView
{
protected: // �������л�����
	COpenGLView();
	DECLARE_DYNCREATE(COpenGLView)

// ����
public:
	COpenGLDoc* GetDocument() const;
	//^v^
	static int height;
	static int width;
	HGLRC m_hRC; //Rendering Context      
	CDC* m_pDC; //Device Context  
	//^v^
	static Board * board;
	static Block * tmpBlock;
	static bool runOver;//�㷨ִ�����
	static Viewer * viewer;
	CPoint lastLBPos;
	bool lbDown;
	static float angle;
	bool rotate;//�Ƿ���ת
// ����
public:
	//^v^
	bool InitializeOpenGL();
	bool SetupPixelFormat();
	void RenderScene();
	//^v^
	static int view_mode;//��ͼģʽ
	void show2or3DView();
// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// ʵ��
public:
	virtual ~COpenGLView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

#ifndef _DEBUG  // OpenGLView.cpp �еĵ��԰汾
inline COpenGLDoc* COpenGLView::GetDocument() const
   { return reinterpret_cast<COpenGLDoc*>(m_pDocument); }
#endif

