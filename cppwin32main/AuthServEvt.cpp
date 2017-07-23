
#include "stdafx.h"
#include "AuthServEvt.h"

#include "zoom_sdk.h"
#include "auth_service_interface.h"
#include "meeting_service_interface.h"

#include "MeetingServEvt.h"
#include "AuthServEvt.h"

#include <Windows.h>

#include "rapidjson\rapidjson.h"
#include "rapidjson\document.h"
#include "curl.h"
#include <string>
#include <locale>
#include <codecvt>
void APIUserStart(IMeetingService * p);
void APIUserJoin(IMeetingService * p);

using namespace rapidjson;

AuthServEvt::AuthServEvt()
{
}


AuthServEvt::~AuthServEvt()
{
}

void AuthServEvt::setCurrentUserType(wstring m) {
	this->ut = m;
}

wstring AuthServEvt::getCurrentUserType() {
	return this->ut;
}
void AuthServEvt::onAuthenticationReturn(AuthResult ret) {
	//IMeetingServiceEvent * pMeetingServiceEvent;
	SDKError err = CreateMeetingService(&pMeethingService);
	if (err == SDKERR_SUCCESS) {
		MeetingServEvt * evt = new MeetingServEvt();

		evt->setServiceManager(pMeethingService);
		pMeethingService->SetEvent(evt);
		
		if (getCurrentUserType() == HOST_ID) {			
			APIUserStart(pMeethingService);
		}
		else {
			APIUserJoin(pMeethingService);

		}
	}

};
void AuthServEvt::onLoginRet(LOGINSTATUS ret, IAccountInfo* pAccountInfo) {
	int i = 0;
	
};
void AuthServEvt::onLogout() {
	int i = 0;
};

void AuthServEvt::setServiceManager(IMeetingService ** p) {
	this->pMeethingService = *p;
};



