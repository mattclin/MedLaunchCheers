#include "stdafx.h"
#include "toolbar_bottom_ui.h"

CToolbarBottom::CToolbarBottom(CToolbarMgr* pToolbarMgr)
{
	m_hParentWnd = NULL;
	m_pToolbarMgr = pToolbarMgr;
	m_bInChat = false;
	m_bInParticipant = false;
	m_bInGalleryView = false;
}

CToolbarBottom::~CToolbarBottom()
{
	m_hParentWnd = NULL;
	m_pToolbarMgr = NULL;
	m_bInChat = false;
	m_bInParticipant = false;
	m_bInGalleryView = false;
}

void CToolbarBottom::InitWindow()
{
	//Where new buttons are defined
	RECT rc = { 0 };
	if( !::GetClientRect(m_hWnd, &rc) ) 
		return;
	rc.top = rc.bottom - DEFAULT_TOOLBAR_HEIGHT;
	rc.bottom = rc.top + DEFAULT_TOOLBAR_HEIGHT;
	if( !::AdjustWindowRectEx(&rc, GetWindowStyle(m_hWnd), false, GetWindowExStyle(m_hWnd)) ) return;
	::SetWindowPos(m_hWnd, HWND_BOTTOM,0,0,0,0, SWP_NOSIZE | SWP_NOMOVE);

	m_containerBottomButtons = static_cast<CHorizontalLayoutUI* >(m_PaintManager.FindControl(_T("container_buttons_bottom_ui")));
	m_btnParticipant = static_cast<CButtonUI* >(m_PaintManager.FindControl(_T("btn_participant")));
	if(m_btnParticipant)
	{								
		m_btnParticipant->SetText(L"Show Participant");
	}
	m_btnThumbnailVideo = static_cast<CButtonUI* >(m_PaintManager.FindControl(_T("btn_thumbnailvideo")));
	if(m_btnThumbnailVideo)            
		m_btnThumbnailVideo->SetText(L"See Everyone");
	UpdateButton(Toolbar_Button_ThumbnailVideo, Button_UnMuted);
	m_btnAudio = static_cast<CButtonUI* >(m_PaintManager.FindControl(_T("btn_audio")));
	if(m_btnAudio)
		m_btnAudio->SetText(L"Mute Audio");
	m_btnVideo = static_cast<CButtonUI* >(m_PaintManager.FindControl(_T("btn_video")));
	if(m_btnVideo)
		m_btnVideo->SetText(L"Stop video");
	//m_btnInvite = static_cast<CButtonUI* >(m_PaintManager.FindControl(_T("btn_invite")));
	
	m_btnShare = static_cast<CButtonUI* >(m_PaintManager.FindControl(_T("btn_share")));
	if(m_btnShare)
		m_btnShare->SetText(L"Share");
	UpdateButton(Toolbar_Button_Share, Button_UnMuted);
	m_btnChat = static_cast<CButtonUI* >(m_PaintManager.FindControl(_T("btn_chat")));
	if(m_btnChat)
		m_btnChat->SetText(L"Show Chat");
	UpdateButton(Toolbar_Button_Chat, Button_UnMuted);
	m_btnLeave = static_cast<CButtonUI* >(m_PaintManager.FindControl(_T("btn_leavemeeting")));
	/*if(m_btnLeave)
	{
		if(m_pToolbarMgr && m_pToolbarMgr->IsHost())
			m_btnLeave->SetText(L"End Meeting");
		else
			m_btnLeave->SetText(L"Leave Meeting");
	}*/
	UpdateButton(Toolbar_Button_Leave, Button_UnMuted);
	m_btnMore = static_cast<CButtonUI* >(m_PaintManager.FindControl(L"btn_more"));
	if (m_btnMore)
	{
		m_btnMore->SetText(L"More");
	}

	m_ToolbarBottomWorkFlow.SetUIEvent(this);
}

LRESULT CToolbarBottom::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;

	if( uMsg == WM_CREATE ) 
	{
		m_PaintManager.Init(m_hWnd);

		CDialogBuilder builder;
		STRINGorID xml(GetSkinRes());
		CControlUI* pRoot = builder.Create(xml, _T("xml"), 0, &m_PaintManager);
		ASSERT(pRoot && "Failed to parse XML");

		m_PaintManager.AttachDialog(pRoot);
		m_PaintManager.AddNotifier(this);
		InitWindow(); 
		return lRes;
	}

	if( m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes) ) 
	{
		return lRes;
	}

	return __super::HandleMessage(uMsg, wParam, lParam);
}


void CToolbarBottom::Notify(TNotifyUI& msg)
{
	if(msg.sType == _T("click"))
	{
		if(msg.pSender == m_btnAudio)
		{
			DoAudioButtonClick();
		}
		else if(msg.pSender == m_btnVideo)
		{
			DoVideoButtonClick();
		}
		/*
		else if(msg.pSender == m_btnInvite)
		{
			DoInviteButtonClick();
		}
		*/
		else if (msg.pSender == m_btnThumbnailVideo)
		{
			DoThumbnailVideoClick();
		}
		else if(msg.pSender == m_btnParticipant)
		{
			DoParticipantButtonClick();
		}
		else if(msg.pSender == m_btnChat)
		{
			DoChatButtonClick();
		}
		else if(msg.pSender == m_btnShare)
		{
			DoShareButtonClick();
		}
		else if(msg.pSender == m_btnLeave)
		{
			DoLeaveButtonClick();
		}
		else if (msg.pSender == m_btnMore)
		{
			DoMoreMenuClick();
		}

	}
}

bool CToolbarBottom::IsWindow()
{
	return ::IsWindow(m_hWnd) ? true:false;
}

bool CToolbarBottom::DestroyWindow()
{
	if(IsWindow())
		return ::DestroyWindow(m_hWnd) ? true:false;
	return false;
}

void CToolbarBottom::SetParent(HWND hParentWnd)
{
	m_hParentWnd = hParentWnd;
}
CButtonUI* CToolbarBottom::GetButton(ToolbarButtonType buttonType)
{
	switch(buttonType)
	{
	case Toolbar_Button_Audio:
		return m_btnAudio;
	case Toolbar_Button_Video:
		return m_btnVideo;
	/*
	case Toolbar_Button_Invite:
		return m_btnInvite;
	*/
	case Toolbar_Button_Chat:
		return m_btnChat;
	case Toolbar_Button_Participant:
		return m_btnParticipant;
	case Toolbar_Button_Share:
		return m_btnShare;
	case Toolbar_Button_Leave:
		return m_btnLeave;
	case Toolbar_Button_ThumbnailVideo:
		return m_btnThumbnailVideo;
	}
	return NULL;
}

void CToolbarBottom::DoAudioButtonClick()
{
	m_ToolbarBottomWorkFlow.AudioButtonClicked();
}
void CToolbarBottom::DoVideoButtonClick()
{
	m_ToolbarBottomWorkFlow.VideoButtonClicked();
}

void CToolbarBottom::DoParticipantButtonClick()
{
	if(m_pToolbarMgr)
		m_pToolbarMgr->ShowParticipantWindow();
	buttonStatusType btnStatusType = Button_Disabled;
	if(m_bInParticipant)
		btnStatusType = Button_Muted;
	else
		btnStatusType = Button_UnMuted;
	UpdateButton(Toolbar_Button_Participant, btnStatusType);
	
}

void CToolbarBottom::DoChatButtonClick()
{
	if(m_pToolbarMgr)
		m_pToolbarMgr->ShowChatWindow();
	buttonStatusType btnStatusType = Button_Disabled;
	if(m_bInChat)
		btnStatusType = Button_Muted;
	else
		btnStatusType = Button_UnMuted;
	UpdateButton(Toolbar_Button_Chat, btnStatusType);
}

void CToolbarBottom::DoShareButtonClick()
{
	if(m_pToolbarMgr)
		m_pToolbarMgr->ShowShareWindow();
	buttonStatusType btnStatusType = Button_Disabled;
	if(m_bInShare)
		btnStatusType = Button_Muted;
	else
		btnStatusType = Button_UnMuted;
	UpdateButton(Toolbar_Button_Share, btnStatusType);
}

void CToolbarBottom::DoLeaveButtonClick()
{
	m_ToolbarBottomWorkFlow.LeaveButtonClicked();
}

void CToolbarBottom::onBtnClickedResult(ToolbarButtonType btnType, ZOOM_SDK_NAMESPACE::SDKError ret,  bool bOn)
{
	CButtonUI* button = GetButton(btnType);
	if(NULL == button)
		return;

	buttonStatusType btnStatusType = Button_Enabled;
	if(ZOOM_SDK_NAMESPACE::SDKERR_SUCCESS == ret)
	{
		if(bOn)
			btnStatusType = Button_UnMuted;
		else
			btnStatusType = Button_Muted;
	}
	UpdateButton(btnType, btnStatusType);
}

void CToolbarBottom::DoThumbnailVideoClick()
{
	//to do
	//show the gallery view vidoes
	if(m_pToolbarMgr)
	{
		m_pToolbarMgr->ShowGalleryViewWindow();
		buttonStatusType btnStatusType = Button_Disabled;
		if(m_bInGalleryView)
			btnStatusType = Button_Muted;
		else
			btnStatusType = Button_UnMuted;
		UpdateButton(Toolbar_Button_ThumbnailVideo, btnStatusType);
	}
}

void CToolbarBottom::UpdateButtonImage(void* pbutton, wchar_t* strImage, int& nImageRight, int nImageWidth)
{
	CButtonUI* pButton = static_cast<CButtonUI*>(pbutton);
	if (NULL == pButton || NULL == strImage) return;

	int nWidth = max(pButton->GetFixedWidth(), pButton->GetMinWidth());
	wchar_t strDest[128] = {0};
	int nLeft = (nWidth - nImageWidth) / 2;
	nImageRight = nLeft + nImageWidth;
	wsprintf(strDest,_T("dest='%d,4,%d,54'"), nLeft, nImageRight);

	wchar_t strImagePath[128] = {0};
	wsprintf(strImagePath, L"res='%s' restype='ZPIMGRES' source='0,0,%d,60' %s", strImage, nImageWidth, strDest);
	pButton->SetForeImage(strImagePath);
	wsprintf(strImagePath, L"res='%s' restype='ZPIMGRES' source='%d,0,%d,60' %s", strImage, nImageWidth, nImageWidth * 2, strDest);
	pButton->SetHotForeImage(strImagePath);
	wsprintf(strImagePath, L"res='%s' restype='ZPIMGRES' source='%d,0,%d,60' %s", strImage, nImageWidth * 2, nImageWidth * 3, strDest);
	pButton->SetPushedImage(strImagePath);
}

void CToolbarBottom::UpdateButton(ToolbarButtonType btnNeedUpdate, buttonStatusType btnStatus)
{
	unsigned int textColor = 0x00000000;
	CButtonUI* button = GetButton(btnNeedUpdate);
	if(NULL == button)
		return;
	button->SetTextColor(textColor);
	unsigned int hotTextColor = 0x00256AE6;
	button->SetHotTextColor(hotTextColor);
	button->SetHotBkColor(hotTextColor);
	switch(btnNeedUpdate)
	{
	case Toolbar_Button_Audio:
		{
			if(Button_UnMuted==btnStatus)
			{
				//LPCTSTR strText = _T("Mute\nYourself");
				//button->SetText(strText);
				int nWidth = max(button->GetFixedWidth(), button->GetMinWidth());
				button->SetFixedWidth(nWidth);
				int nImageRight = 0;
				button->SetText(L"Mute Yourself");
				UpdateButtonImage((void*)button, _T("toolbar_btn_mute.png"), nImageRight);
			}
			else if(Button_Muted == btnStatus)
			{
				wchar_t* strText = L"Unmute Yourself";
				button->SetText(strText);
				int nWidth = max(button->GetFixedWidth(), button->GetMinWidth());
				button->SetFixedWidth(nWidth);
				int nImageRight = 0;
				UpdateButtonImage((void*)button, _T("toolbar_btn_unmute.png"), nImageRight);				
			}
			else if(Button_Enabled == btnStatus)
			{
				button->SetEnabled(true);
			}
			else if(Button_Disabled == btnStatus)
			{
				button->SetEnabled(false);
			}
		}
		break;
	case Toolbar_Button_Video:
		{
			if(Button_UnMuted == btnStatus)
			{
				wchar_t* strText = L"Turn Your Video Off";
				button->SetText(strText);
				int nWidth = max(button->GetFixedWidth(), button->GetMinWidth());
				button->SetFixedWidth(nWidth);
				int nImageRight = 0;
				UpdateButtonImage((void*)button, _T("toolbar_btn_stopVideo.png"), nImageRight);
			}
			else if(Button_Muted == btnStatus)
			{
				wchar_t* strText = L"Turn Your Video On";
				button->SetText(strText);
				int nWidth = max(button->GetFixedWidth(), button->GetMinWidth());
				button->SetFixedWidth(nWidth);
				int nImageRight = 0;
				UpdateButtonImage((void*)button, _T("toolbar_btn_startVideo.png"), nImageRight);				
			}
			else if(Button_Enabled == btnStatus)
			{
				button->SetEnabled(true);
			}
			else if(Button_Disabled == btnStatus)
			{
				button->SetEnabled(false);
			}
		}
		break;
	case Toolbar_Button_Invite:
		break;
	case Toolbar_Button_Chat:
		{
			if(Button_UnMuted == btnStatus)
			{
				wchar_t* strText = L"Show Chat";
				button->SetText(strText);
				int nWidth = max(button->GetFixedWidth(), button->GetMinWidth());
				button->SetFixedWidth(nWidth);
				int nImageRight = 0;
				UpdateButtonImage((void*)button, _T("toolbar_btn_chat.png"), nImageRight);
			}
			else if(Button_Muted == btnStatus)
			{
				wchar_t* strText = L"Hide Chat";
				button->SetText(strText);
				int nWidth = max(button->GetFixedWidth(), button->GetMinWidth());
				button->SetFixedWidth(nWidth);
				int nImageRight = 0;
				UpdateButtonImage((void*)button, _T("toolbar_btn_chat.png"), nImageRight);
			}
		}
		break;
	case Toolbar_Button_Participant:
		{
			if(Button_UnMuted == btnStatus)
			{
				wchar_t* strText = L"Show Participant";
				button->SetText(strText);
			}
			else if(Button_Muted == btnStatus)
			{
				wchar_t* strText = L"Hide Participant";
				button->SetText(strText);
			}
		}
		break;
	case Toolbar_Button_ThumbnailVideo:
		{
			if(Button_UnMuted == btnStatus)
			{
				wchar_t* strText = L"See Everyone";
				button->SetText(strText);
				int nWidth = max(button->GetFixedWidth(), button->GetMinWidth());
				button->SetFixedWidth(nWidth);
				int nImageRight = 0;
				UpdateButtonImage((void*)button, _T("toolbar_btn_participants.png"), nImageRight);
			}
			else if(Button_Muted == btnStatus)
			{
				wchar_t* strText = L"Show Only Speaker";
				button->SetText(strText);
				int nWidth = max(button->GetFixedWidth(), button->GetMinWidth());
				button->SetFixedWidth(nWidth);
				int nImageRight = 0;
				UpdateButtonImage((void*)button, _T("toolbar_btn_speaker_only.png"), nImageRight);
				//TODO: change this image file
			}
		}
		break;
	case Toolbar_Button_Share:
		{
			if(Button_UnMuted == btnStatus)
			{
				wchar_t* strText = L"Share Your Screen";
				button->SetText(strText);
				int nWidth = max(button->GetFixedWidth(), button->GetMinWidth());
				button->SetFixedWidth(nWidth);
				int nImageRight = 0;
				UpdateButtonImage((void*)button, _T("toolbar_btn_shareScreen.png"), nImageRight);
			}
			else if(Button_Muted == btnStatus)
			{
				wchar_t* strText = L"Stop Sharing";
				button->SetText(strText);
				int nWidth = max(button->GetFixedWidth(), button->GetMinWidth());
				button->SetFixedWidth(nWidth);
				int nImageRight = 0;
				UpdateButtonImage((void*)button, _T("toolbar_btn_shareScreen.png"), nImageRight);
			}
		}
		break;
	case Toolbar_Button_Leave:
	{
		/*wchar_t* strText = L"Leave Call";
		button->SetText(strText);*/

		int nWidth = max(button->GetFixedWidth(), button->GetMinWidth());
		button->SetFixedWidth(nWidth);
		int nImageRight = 0;
		int nImageWidth = DEFAULT_IMAGE_WIDTH;
		wchar_t* strImage = _T("toolbar_btn_leaveMeeting.png");

		wchar_t strDest[128] = { 0 };
		int nLeft = (nWidth - nImageWidth) / 2;
		nImageRight = nLeft + (int)(nImageWidth * 1.5);
		wsprintf(strDest, _T("dest='%d,4,%d,81'"), nLeft, nImageRight);

		wchar_t strImagePath[128] = { 0 };
		wsprintf(strImagePath, L"res='%s' restype='ZPIMGRES' source='0,0,%d,60' %s", strImage, nImageWidth, strDest);
		button->SetForeImage(strImagePath);
		wsprintf(strImagePath, L"res='%s' restype='ZPIMGRES' source='%d,0,%d,60' %s", strImage, nImageWidth, nImageWidth * 2, strDest);
		button->SetHotForeImage(strImagePath);
		wsprintf(strImagePath, L"res='%s' restype='ZPIMGRES' source='%d,0,%d,60' %s", strImage, nImageWidth * 2, nImageWidth * 3, strDest);
		button->SetPushedImage(strImagePath);
		break;
	}
	default:
		break;
	}
}

void CToolbarBottom::SetGalleryViewStatus(bool bOn)
{
	m_bInGalleryView = bOn;
}

void CToolbarBottom::SetChatUIStatus(bool bOn)
{
	m_bInChat = bOn;
	buttonStatusType btnStatusType = Button_Disabled;
	if(m_bInChat)
		btnStatusType = Button_Muted;
	else
		btnStatusType = Button_UnMuted;
	UpdateButton(Toolbar_Button_Chat, btnStatusType);
}

void CToolbarBottom::SetParticipantUIStatus(bool bOn)
{
	m_bInParticipant = bOn;
}

void CToolbarBottom::SetShareUIStatus(bool bOn)
{
	m_bInShare = bOn;
}

void CToolbarBottom::DoMoreMenuClick()
{
	if (m_pToolbarMgr)
	{
		RECT WndRC = {0};
		GetWindowRect(m_hWnd,&WndRC);
		m_pToolbarMgr->ShowMoreMenuWindow(m_btnMore->GetX()+WndRC.left, WndRC.top);
	}
	
}
