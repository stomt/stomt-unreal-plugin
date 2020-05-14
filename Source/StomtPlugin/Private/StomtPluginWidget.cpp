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

void UStomtPluginWidget::OnConstruction(FString AppId)
{
	// Create API Object
	if (this->Api == NULL)
	{
		this->Api = UStomtAPI::ConstructStomtAPI(AppId);
	}
	else
	{
		this->Api->SetAppId(AppId);
	}

	this->Config = this->Api->Config;

	// Request Target Name
	UStomtRestRequest* Request = this->Api->RequestTargetByAppId();
	Request->OnRequestComplete.AddDynamic(this, &UStomtPluginWidget::OnTargetResponse);

	//Lookup EMail
	this->bIsEMailAlreadyKnown = this->Api->Config->GetSubscribed();
	this->bIsUserLoggedIn = this->Api->Config->GetLoggedIn();
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
	this->Stomt = UStomt::ConstructStomt(this->Api->GetTargetId(), !this->bIsWish, this->Message);
	this->Stomt->SetLabels(this->Labels);
	this->Stomt->SetAnonym(false);

	this->Api->SetStomtToSend(this->Stomt);

	FString LogFileName = FApp::GetProjectName() + FString(TEXT(".log"));

	this->Api->HandleOfflineStomts();

	if (this->bUploadLogs)
	{
		FString LogFile = this->Api->ReadLogFile(LogFileName);
		if (!LogFile.IsEmpty())
		{
			this->Api->SendLogFile(LogFile, LogFileName);
		}
		else
		{
			this->Api->bIsLogUploadComplete = true;
			if (!this->Api->bUseImageUpload)
			{
				this->Api->SendStomt(this->Stomt);
			}
		}
	}
	else
	{
		this->Api->bIsLogUploadComplete = true;
		if (!this->Api->bUseImageUpload)
		{
			this->Api->SendStomt(this->Stomt);
		}
	}

	// Check EMail
	this->bIsEMailAlreadyKnown = this->Api->Config->GetSubscribed();
	UE_LOG(StomtLog, Log, TEXT("Is EMail Already Known: %s"), this->bIsEMailAlreadyKnown ? TEXT("true") : TEXT("false"));
}

void UStomtPluginWidget::OnSubmitLastLayer()
{

}

bool UStomtPluginWidget::OnSubmitLogin()
{
	if (!this->UserName.IsEmpty() && !this->UserPassword.IsEmpty())
	{
		UStomtRestRequest* Request = this->Api->SendLoginRequest(this->UserName, this->UserPassword);
		Request->OnRequestComplete.AddDynamic(this, &UStomtPluginWidget::OnLoginResponse);
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
		this->Api->SendSubscription(this->EMail, !this->bUsePhoneNumber);
	}
}

void UStomtPluginWidget::OnLogout()
{
	this->Api->SendLogoutRequest();
}

void UStomtPluginWidget::OnLoginResponse(UStomtRestRequest * LoginRequest)
{
	this->LoginErrorCode = LoginRequest->GetResponseCode();
	this->bIsUserLoggedIn = this->Api->Config->GetLoggedIn();
	this->Api->OnLoginRequestComplete.Broadcast(LoginRequest);
}

void UStomtPluginWidget::OnTargetResponse(UStomtRestRequest * TargetRequest)
{
	this->TargetName = this->Api->GetTargetName();

	UE_LOG(StomtLog, Log, TEXT("OnTargetResponse: %s"), *this->TargetName);

	this->ImageURL = this->Api->GetImageURL();

	this->Api->OnTargetRequestComplete.Broadcast(TargetRequest);
}

FString UStomtPluginWidget::AppendStomtURLParams(FString Url, FString UtmContent)
{
	Url += FString("?utm_source=" + FString("stomt"));
	Url += FString("&utm_medium=" + FString("sdk"));
	Url += FString("&utm_campaign=" + FString("unreal"));
	Url += FString("&utm_term=" + FString(FApp::GetProjectName()) );

	if (!UtmContent.IsEmpty())
	{
		Url += FString("&utm_content=" + UtmContent);
	}

	if (!this->Api->Config->GetAccessToken().IsEmpty())
	{
		Url += FString("&accesstoken=" + this->Api->Config->GetAccessToken());
	}
	
	return Url;
}

void UStomtPluginWidget::UploadScreenshot()
{
	this->Api->SendImageFile(this->Api->ReadScreenshotAsBase64());
}
