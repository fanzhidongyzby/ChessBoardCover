
// OpenGL.h : OpenGL Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// COpenGLApp:
// �йش����ʵ�֣������ OpenGL.cpp
//

class COpenGLApp : public CWinApp
{
public:
	COpenGLApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern COpenGLApp theApp;
