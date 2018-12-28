// Copyright 2018 STOMT GmbH. All Rights Reserved.
#pragma once

#include "StomtPluginWidget.h"
#include "StomtPluginPrivatePCH.h"
#include "StomtRestRequest.h"
#include "StomtLabel.h"

#include "Runtime/Engine/Classes/Components/SceneCaptureComponent2D.h"

UStomtPluginWidget::~UStomtPluginWidget()
{
	this->LoginErrorCode = 0;
}

void UStomtPluginWidget::OnConstruction(FString AppID)
{
	// Create API Object
	if (api == NULL)
	{
		api = UStomtAPI::ConstructStomtAPI(AppID);
	}
	else
	{
		this->api->SetAppID(AppID);
	}

	this->Config = this->api->Config;

	// Request Target Name
	UStomtRestRequest* request = this->api->RequestTargetByAppID();
	request->OnRequestComplete.AddDynamic(this, &UStomtPluginWidget::OnTargetResponse);

	//Lookup EMail
	this->IsEMailAlreadyKnown = this->api->Config->GetSubscribed();
	this->IsUserLoggedIn = this->api->Config->GetLoggedIn();
}

void UStomtPluginWidget::OnMessageChanged(FString text)
{
	if (!text.IsEmpty())
	{
		this->Message = text;
	}
	else
	{
		this->Message = FString(TEXT(""));
	}
}

void UStomtPluginWidget::OnSubmit()
{
	if (this->Message.IsEmpty())
	{
		return;
	}

	// Create Stomt Instance
	this->stomt = UStomt::ConstructStomt(this->api->GetTargetID(), !this->IsWish, this->Message);
	this->stomt->SetLabels(this->Labels);
	this->stomt->SetAnonym(false);

	this->api->SetStomtToSend(stomt);

	FString LogFileName = FApp::GetProjectName() + FString(TEXT(".log"));

	this->api->HandleOfflineStomts();

	if (this->UploadLogs)
	{
		FString logFile = this->api->ReadLogFile(LogFileName);
		if (!logFile.IsEmpty())
		{
			this->api->SendLogFile(logFile, LogFileName);
		}
		else
		{
			this->api->IsLogUploadComplete = true;
			if (!this->api->UseImageUpload)
			{
				this->api->SendStomt(stomt);
			}
		}
	}
	else
	{
		this->api->IsLogUploadComplete = true;
		if (!this->api->UseImageUpload)
		{
			this->api->SendStomt(stomt);
		}
	}

	// Check EMail
	this->IsEMailAlreadyKnown = this->api->Config->GetSubscribed();
	UE_LOG(Stomt, Log, TEXT("Is EMail Already Known: %s"), this->IsEMailAlreadyKnown ? TEXT("true") : TEXT("false"));
}

void UStomtPluginWidget::OnSubmitLastLayer()
{

}

bool UStomtPluginWidget::OnSubmitLogin()
{
	if (!this->UserName.IsEmpty() && !this->UserPassword.IsEmpty())
	{
		UStomtRestRequest* request = this->api->SendLoginRequest(this->UserName, this->UserPassword);
		request->OnRequestComplete.AddDynamic(this, &UStomtPluginWidget::OnLoginResponse);
		this->LoginErrorCode = 0;
		return true;
	}
	else
	{
		return false;
	}
}

void UStomtPluginWidget::OnSubmitEMail()
{
	if (!this->EMail.IsEmpty())
	{
		this->api->SendSubscription(this->EMail, !UsePhoneNumber);
	}
}

void UStomtPluginWidget::OnLogout()
{
	this->api->SendLogoutRequest();
}

void UStomtPluginWidget::OnLoginResponse(UStomtRestRequest * LoginRequest)
{
	this->LoginErrorCode = LoginRequest->GetResponseCode();
	this->IsUserLoggedIn = this->api->Config->GetLoggedIn();
	this->api->OnLoginRequestComplete.Broadcast(LoginRequest);
}

void UStomtPluginWidget::OnTargetResponse(UStomtRestRequest * TargetRequest)
{
	this->TargetName = this->api->GetTargetName();

	UE_LOG(Stomt, Log, TEXT("OnTargetResponse: %s"), *this->TargetName);

	this->ImageURL = this->api->GetImageURL();

	this->api->OnTargetRequestComplete.Broadcast(TargetRequest);
}

FString UStomtPluginWidget::AppendStomtURLParams(FString url, FString utm_content)
{
	url += FString("?utm_source=" + FString("stomt"));
	url += FString("&utm_medium=" + FString("sdk"));
	url += FString("&utm_campaign=" + FString("unreal"));
	url += FString("&utm_term=" + FString(FApp::GetProjectName()) );

	if ( !utm_content.IsEmpty() )
	{
		url += FString("&utm_content=" + utm_content);
	}

	if ( !this->api->Config->GetAccessToken().IsEmpty() )
	{
		url += FString("&accesstoken=" + this->api->Config->GetAccessToken() );
	}
	
	return url;
}

void UStomtPluginWidget::UploadScreenshot()
{
	this->api->SendImageFile(this->api->ReadScreenshotAsBase64());
}
