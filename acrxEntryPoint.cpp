// (C) Copyright 2002-2012 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include <restclient-cpp/restclient.h>
#include <json/json.h>

using namespace std;

//-----------------------------------------------------------------------------
#define szRDS _RXST("ADSK")

bool bIsUserLoggedIn = true;
#define CONNECTTOWEB _T("CONNECTWEBSERVICES")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CEntitlementAPIApp : public AcRxArxApp {

private:
	HMODULE	 hLibcurl, hjsoncpp;
public:
	CEntitlementAPIApp () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		
		// TODO: Add your initialization code here
		
		hLibcurl = LoadLibrary(_T("libcurl.dll"));
		hjsoncpp = LoadLibrary(_T("jsoncpp.dll"));
		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		// TODO: Unload dependencies here
		FreeLibrary(hLibcurl);
		FreeLibrary(hjsoncpp);
		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}
	
	static void onlineUI()
	{
		//if user isn't logged, onlineuserid will be empty
		
		// pop once only
	
			// prompt user to login
			// ensure unload-able from ui
			acrxLoadModule(_T("AcConnectWebServices.arx"), false, true);
			acrxLoadModule(_T("AcConnectWebServices.arx"), false, false);

			assert(acrxServiceIsRegistered(CONNECTTOWEB));
			if (acrxServiceIsRegistered(CONNECTTOWEB))
			{
				typedef void(*ADSKLOGIN) ();
				ADSKLOGIN pAdskLogin = (ADSKLOGIN)
					acrxDynamicLinker->getSymbolAddress(CONNECTTOWEB,
						_T("AcConnectWebServicesLogin"));
				assert(pAdskLogin);
				if (pAdskLogin != NULL)
					pAdskLogin();
			}
			acrxUnloadModule(_T("AcConnectWebServices.arx"), false);
		

	}
	
	static AcString getUserId() {
		struct resbuf rb;
		AcString userId;
		acedGetVar(_T("ONLINEUSERID"), &rb);
		if (rb.resval.rstring != NULL) {

			userId = rb.resval.rstring;
			if (userId[0] == _T('\0')) bIsUserLoggedIn = false;
			free(rb.resval.rstring);
		}
		return userId;
	}
	static std::string toNarrow(const wstring& str)
	{
		ostringstream stm;
		const ctype<wchar_t>& ctfacet =
			use_facet< ctype<wchar_t> >(stm.getloc());
		for (size_t i = 0; i < str.size(); ++i)
			stm << ctfacet.narrow(str[i], 0);
		return stm.str();
	}
	static void ADSKMyGroupCheckEntitlement() {
		
		
		if (getUserId()[0] == _T('\0')) {
			//User is not logged in, we will prompt to login!
			onlineUI();
		}
		//plumbing wide
		TCHAR buffer[512];
		acedGetString(0,_T("Enter AppId To Test CheckEntitlement:\t"), buffer);
		string userId = toNarrow(getUserId().kwszPtr());
		/*
		  Valid = "3563402343067399513";
		  InValid= "1006119760063675415"; 
		 */ 
		string appId = toNarrow(buffer);
		string url ="https://apps.exchange.autodesk.com/webservices/checkentitlement"+ string("?userid=") + userId + string("&appid=") + appId;
		RestClient::Response response = RestClient::get(url);

		Json::Reader reader;
		Json::Value root;

		bool isparseSuccessful = reader.parse(response.body, root);
		if (!isparseSuccessful) {
			acutPrintf(_T("Error parsing URL"));
		}
		if ((root["IsValid"].asString()) == "true")
		{
			acutPrintf(_T("IsValid is True"));
		}
		else
		{
			acutPrintf(_T("IsValid is False"));
		}
	}

	
	
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CEntitlementAPIApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CEntitlementAPIApp, ADSKMyGroup, CheckEntitlement, CheckEntitlement, ACRX_CMD_MODAL, NULL)


