#pragma once
#include "stdafx.h"


#define HOST_ID L"1"
#define PARTICIPANT_ID L"0"

#include "zoom_sdk.h"
#include "auth_service_interface.h"
#include "meeting_service_interface.h"
#include "MeetingServEvt.h"

#include <string>
using namespace std;
using namespace ZOOM_SDK_NAMESPACE; 

class AuthServEvt :
	public IAuthServiceEvent
{
private: 
	wstring ut;
public:
	AuthServEvt();
	~AuthServEvt();
	void AuthServEvt::onAuthenticationReturn(AuthResult ret);
	void AuthServEvt::onLoginRet(LOGINSTATUS ret, IAccountInfo* pAccountInfo);
	void AuthServEvt::onLogout();

	void AuthServEvt::setServiceManager(IMeetingService ** p);

	IMeetingService * pMeethingService;
	
	
	void setCurrentUserType(wstring m);

	wstring getCurrentUserType();
	
};

