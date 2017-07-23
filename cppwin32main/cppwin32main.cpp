// cppwin32main.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "cppwin32main.h"


#define HOST_ID L"1"
#define PARTICIPANT_ID L"0"

#include "zoom_sdk.h"
#include "auth_service_interface.h"
#include "meeting_service_interface.h"

#include "MeetingServEvt.h"
#include "AuthServEvt.h"

#include <Windows.h>
#include <shellapi.h>

#include "rapidjson\rapidjson.h"
#include "rapidjson\document.h"
#include "curl.h"
#include <string>
#include <locale>
#include <codecvt>


using namespace ZOOM_SDK_NAMESPACE;
using namespace std;

using namespace rapidjson;




#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void ZOOOOOOM();
void APIUserJoin(IMeetingService *pMeetingService);

IMeetingUIController * mCtrl;
IMeetingService * pMeethingService;
IAuthService* pAuthService;
HWND zoom1Wnd;
HWND zoom2Wnd;

wstring current_user_type;
wstring meeting_id;
wstring user_email;




int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    
	LPWSTR *szArglist;
	int nArgs;
	int i;

	szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
	if (NULL == szArglist)
	{
		wprintf(L"CommandLineToArgvW failed\n");
		LocalFree(szArglist);
		return 0;
	}
	wstring mtype = szArglist[1];
	
	if (mtype == L"1") {
		current_user_type = HOST_ID;
		user_email = szArglist[2];
	}
	else if (mtype == PARTICIPANT_ID) {
		current_user_type = PARTICIPANT_ID;
		meeting_id = szArglist[2];
		user_email = szArglist[3];
	}

	// Free memory allocated for CommandLineToArgvW arguments.






    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CPPWIN32MAIN, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CPPWIN32MAIN));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
	LocalFree(szArglist);

	CleanUPSDK();
    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CPPWIN32MAIN));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CPPWIN32MAIN);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
     CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
   
    

   if (!hWnd)
   {
      return FALSE;
   }

   //ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   //ZOOOOOOM();

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		ZOOOOOOM();
	break;

	case WM_RBUTTONDOWN:

		mCtrl = pMeethingService->GetUIController();
		mCtrl->GetMeetingUIWnd(zoom1Wnd, zoom2Wnd);
	break;

	case WM_MOVE:
	{
		HWND hWnd = zoom1Wnd;
////		ShowWindow(hWnd, 0);
	}
		
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
			
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}







size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
	userp->append((char*)contents, size * nmemb);
	return size * nmemb;
}






Document create_user_in_zoom(char * url, const char * data) {
	CURL *curl;
	CURLcode res;

	/* In windows, this will init the winsock stuff */
	curl_global_init(CURL_GLOBAL_ALL);

	/* get a curl handle */
	curl = curl_easy_init();

	std::string readBuffer;

	if (curl) {
		/* First set the URL that is about to receive our POST. This URL can
		just as well be a https:// URL if that is what should receive the
		data. */
		curl_easy_setopt(curl, CURLOPT_URL, url);
		/* Now specify the POST data */
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);
		/* Check for errors */



		if (res != CURLE_OK)
			return NULL;


		/* always cleanup */
		curl_easy_cleanup(curl);
	}
	curl_global_cleanup();

	Document d;
	d.Parse(readBuffer.c_str());
	return d;
}








void ZOOOOOOM() {


	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

	InitParam initParam;
	initParam.strWebDomain = _T("https://zoom.us");
	initParam.strBrandingName = L"ZOOOOMER";
	initParam.strSupportUrl = L"Zooomer.com";

	SDKError err = InitSDK(initParam);

	if (err == SDKERR_SUCCESS) {
		
		pAuthService = NULL;
		AuthServEvt* pAuthServiceEvent = new AuthServEvt();

		pAuthServiceEvent->setCurrentUserType(current_user_type);

		
		SDKError err = CreateAuthService(&pAuthService);

		if (err == SDKERR_SUCCESS) {
			pAuthService->SetEvent(pAuthServiceEvent);
			
			AuthParam authParam;
			std::wstring strKey = _T("9FFrAu6HnHfXHqTbRua38ZqBoextTVo4X3AI");
			std::wstring strSecret = _T("KlYINxyaNpnL8cr526nOvF9ZM8QhZxTA53Px");
			authParam.appKey = strKey.c_str();
			
			authParam.appSecret = strSecret.c_str();
			SDKError err = pAuthService->SDKAuth(authParam);
			
			
			if (err != SDKERR_SUCCESS) {
				
			}


		}
	} 


	

}
void APIUserStart(IMeetingService * p) {

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

	string data = "api_key=I_gJ1XMsR86kGldNiMh2ng&api_secret=eCeivLKVFIYBmnwXK0Rv5caTGk5jF3kbTpbx&type=1&email=";
	data = data + converter.to_bytes(user_email);

	Document d = create_user_in_zoom("https://api.zoom.us/v1/user/custcreate", data.c_str());
	if (d == NULL) { }

	string dddd = "api_key=I_gJ1XMsR86kGldNiMh2ng&api_secret=eCeivLKVFIYBmnwXK0Rv5caTGk5jF3kbTpbx&id=";

	dddd = dddd + d["id"].GetString();
	dddd = dddd + "&enable_annotation=false";
	dddd = dddd + "&disable_jbh_reminder=true";
	dddd = dddd + "&first_name=" + "Moodle";
	dddd = dddd + "&last_name=true";
	dddd = dddd + "&enable_share_dual_camera=true";
	

	Document d2 = create_user_in_zoom("https://api.zoom.us/v1/user/get", dddd.c_str());


	string id = d2["id"].GetString();
	string tkn = d2["token"].GetString();
	string email = d2["email"].GetString();
	string first_name = d2["first_name"].GetString();
	string last_name = d2["last_name"].GetString();
	string name = first_name + " " + last_name;



	string create_meeting_post_uri = "api_key=I_gJ1XMsR86kGldNiMh2ng&api_secret=eCeivLKVFIYBmnwXK0Rv5caTGk5jF3kbTpbx&type=1&topic=Lets-start-Learning&option_jbh=false&host_id=";
	create_meeting_post_uri = create_meeting_post_uri + id;
	create_meeting_post_uri += "&option_host_video=false";


	Document document_meeting = create_user_in_zoom("https://api.zoom.us/v1/meeting/create", create_meeting_post_uri.c_str());

	Value& v = document_meeting["id"];

	uint64_t m_id = v.GetUint64();


	string m_id_str = std::to_string(m_id);
	

	std::wstring strMeetingNumber, strUserId, strUserToken, strUserName;
	strMeetingNumber = converter.from_bytes(m_id_str.c_str());
	strUserId = converter.from_bytes(id.c_str());

	strUserToken = converter.from_bytes(tkn.c_str());;
	strUserName = converter.from_bytes(name.c_str());

	if (strMeetingNumber.length() <= 0 || strUserId.length() <= 0 || strUserToken.length() <= 0 || strUserName.length() <= 0)
		return ;

	ZOOM_SDK_NAMESPACE::StartParam startParam;
	startParam.userType = ZOOM_SDK_NAMESPACE::SDK_UT_APIUSER;
	ZOOM_SDK_NAMESPACE::StartParam4APIUser& apiuserParam = startParam.param.apiuserStart;
	
	apiuserParam.userID = strUserId.c_str();
	apiuserParam.userToken = strUserToken.c_str();
	apiuserParam.meetingNumber = _wtoi64(strMeetingNumber.c_str());
	apiuserParam.userName = strUserName.c_str();
	apiuserParam.isDirectShareDesktop = false;
	
	p->Start(startParam);

	return;

}

void APIUserJoin(IMeetingService *pMeetingService) {


	Document d = create_user_in_zoom("https://api.zoom.us/v1/user/custcreate", "api_key=I_gJ1XMsR86kGldNiMh2ng&api_secret=eCeivLKVFIYBmnwXK0Rv5caTGk5jF3kbTpbx&email=hol1@no-company.com&type=1");
	if (d == NULL) {
		//return -1;
	}

	string dddd = "api_key=I_gJ1XMsR86kGldNiMh2ng&api_secret=eCeivLKVFIYBmnwXK0Rv5caTGk5jF3kbTpbx&id=";
	dddd = dddd + d["id"].GetString();
	Document d2 = create_user_in_zoom("https://api.zoom.us/v1/user/get", dddd.c_str());



	//std::wstring strMeetingNumber, strUserId, strUserToken, strUserName;
	//strMeetingNumber = L"868840536";
	string id = d2["id"].GetString();
	string tkn = d2["token"].GetString();
	string email = d2["email"].GetString();

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

	if (id.length() < 1) {
		// generate message box
		return;
	}

	std::wstring strMeetingNumber = meeting_id;

	std::wstring strUserId = converter.from_bytes(id.c_str());
	std::wstring strUserToken = converter.from_bytes(tkn.c_str());
	std::wstring strUserName = converter.from_bytes(email.c_str());
	if (strMeetingNumber.length() <= 0 || strUserName.length() <= 0)
		return ;

	ZOOM_SDK_NAMESPACE::JoinParam joinParam;
	joinParam.userType = ZOOM_SDK_NAMESPACE::SDK_UT_APIUSER;
	
	ZOOM_SDK_NAMESPACE::JoinParam4APIUser& apiParam = joinParam.param.apiuserJoin;
	apiParam.meetingNumber = _wtoi64(strMeetingNumber.c_str());
	apiParam.userName = strUserName.c_str();
	apiParam.psw = strUserToken.c_str();
	apiParam.isDirectShareDesktop = true;
	
	
	pMeetingService->Join(joinParam);
	

	
	
}



class MeetingEvt :
	public IMeetingUIControllerEvent {
	virtual void onInviteBtnClicked() {
		int i = 0;
		printf("Hello");
	};

};
