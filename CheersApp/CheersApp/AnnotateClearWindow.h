#pragma once
#include "resource.h"
#include "UIlib.h"
#include "sdk_util.h"

using namespace DuiLib;

class AnnotateClearWindow:
	public CWindowWnd ,
	public INotifyUI
{
public:
	AnnotateClearWindow(ZOOM_SDK_NAMESPACE::ICustomizedAnnotationObj* pShareRender);
	~AnnotateClearWindow();

	virtual LPCTSTR		GetWindowClassName() const { return _T("zSDKDemoUI"); }
	UINT                GetClassStyle()      const { return UI_CLASSSTYLE_FRAME | CS_DBLCLKS ; }
	virtual UINT		GetSkinRes()               { return IDXML_ANNOTATE_CLEAR_WINDOW_UI; }
	UILIB_RESOURCETYPE  GetResourceType()    const { return UILIB_RESOURCE; }

	virtual void		InitWindow();
	virtual void		OnFinalMessage(HWND) {}

	virtual void		Notify( TNotifyUI& msg );
	virtual LRESULT		HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	void MoveWindowPosTo(HWND hWnd);

private:
	CPaintManagerUI m_PaintManager;
	ZOOM_SDK_NAMESPACE::ICustomizedAnnotationObj* m_customerAnnoObj;
	CButtonUI* m_btnClearAll;
	CButtonUI* m_btnClearMy;
	CButtonUI* m_btnClearViewer;
};