
// MainFrm.h : CMainFrame ��Ľӿ�
//

#pragma once

class CMainFrame : public CFrameWnd
{

protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

	// ����
public:
	WINDOWPLACEMENT m_OldWndPlacement;//��������ԭ����λ��
	bool m_bFullScreen; //ȫ����ʾ��־
	CRect m_FullScreenRect;//��ʾȫ����ʾʱ�Ĵ���λ��
	// ����
public:
	void switchScr(bool autoSw=true);
	// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


	// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnFullscr();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnHelp();
};


