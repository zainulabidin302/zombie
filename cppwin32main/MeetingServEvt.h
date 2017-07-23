#pragma once

#include "zoom_sdk.h"
#include "auth_service_interface.h"
#include "meeting_service_interface.h"



using namespace ZOOM_SDK_NAMESPACE;

extern HINSTANCE hInst;

class MeetingServEvt :
	public IMeetingServiceEvent {
public:
	
	MeetingServEvt();
	~MeetingServEvt();
	

	HWND hButton1;
	HWND hButton2;


	void setServiceManager(IMeetingService * p) {
		this->pMeethingService = p;
	}
private:
	virtual void onMeetingStatusChanged(MeetingStatus status, int iResult = 0)

	{
		if (status == MEETING_STATUS_INMEETING) {
			IMeetingUIController * cntrl = this->pMeethingService->GetUIController();

			cntrl->ShowBottomFloatToolbarWnd(true);
			cntrl->ShowSharingToolbar(true);
			//cntrl->EnterFullScreen(true, true);
			
			IMeetingConfiguration * conf = this->pMeethingService->GetMeetingConfiguration();
			
			conf->SetMeetingIDForMeetingUITitle(false);
			conf->SetSharingToolbarVisibility(false);
			conf->DisableWaitingForHostDialog(true);
			conf->DisablePopupMeetingWrongPSWDlg(true);
			conf->EnableAutoAdjustMicVolumeWhenJoinAudio(true);
			conf->EnableEnterAndExitFullScreenButtonOnMeetingUI(false);
			
			conf->HideMeetingInfoFromMeetingUITitle(true);
			
			HWND hFirstView, hSecondView;

			conf->EnableInviteButtonOnMeetingUI(false);
			conf->EnableApproveRemoteControlDlg(false);
			
			cntrl->GetMeetingUIWnd(hFirstView, hSecondView);
			
			
			
		}
	}


	IMeetingService * pMeethingService;


	virtual void onRecording2MP4Done(bool bsuccess, int iResult, const wchar_t* szPath) {
	}
	virtual void onRecording2MP4Processing(int iPercentage) {
	}
	virtual void onRecordingStatus(RecordingStatus status) {
	}
	virtual void onUserJoin(IList<unsigned int >* lstUserID, const wchar_t* strUserList = NULL) {}
	virtual void onUserLeft(IList<unsigned int >* lstUserID, const wchar_t* strUserList = NULL) {}
	virtual void onRemoteControlStatus(RemoteControlStatus status, unsigned int userId) {}
	virtual void onSharingStatus(SharingStatus status, unsigned int userId) {}
	virtual void onLockShareStatus(bool bLocked) {}
	virtual void onUserAudioStatusChange(IList<IUserAudioStatus* >* lstAudioStatusChange, const wchar_t* strAudioStatusList = NULL) {}
	virtual void onChatMsgNotifcation(IChatMsgInfo* chatMsg, const wchar_t* content = NULL) {}
	virtual void onUserVideoStatusChange(unsigned int userId, VideoStatus status) {}
	virtual void onHostChangeNotification(unsigned int userId) {}
	virtual void onSpotlightVideoChangeNotification(bool bSpotlight, unsigned int userid) {}
	virtual void onRecordPriviligeChanged(bool bCanRec) {}
	virtual void onLowOrRaiseHandStatusChanged(bool bLow, unsigned int userid) {}

};



