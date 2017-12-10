// Copyright 2016 Daniel Schukies. All Rights Reserved.


#include "StomtPluginPrivatePCH.h"
#include "StomtPluginWidget.h"
#include "StomtRestRequest.h"
#include "StomtLabel.h"
#include "Runtime/Engine/Classes/Components/SceneCaptureComponent2D.h"



UStomtPluginWidget::~UStomtPluginWidget()
{
	this->LoginErrorCode = 0;
}

void UStomtPluginWidget::OnConstruction(FString TargetID, FString RestURL, FString AppID)
{
	// Create API Object
	if (api == NULL)
	{
		api = UStomtAPI::ConstructStomtAPI(TargetID, RestURL, AppID);
	}
	else
	{
		this->api->SetAppID(AppID);
		this->api->SetTargetID(TargetID);
		this->api->SetRestURL(RestURL);
	}

	this->Config = this->api->Config;

	// Request Target Name
	UStomtRestRequest* request = this->api->RequestTarget(TargetID);
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
	if (!this->Message.IsEmpty())
	{
		this->stomt = UStomt::ConstructStomt(this->api->GetTargetID(), !this->IsWish, this->Message);
		this->stomt->SetLabels(this->Labels);

		stomt->SetAnonym(false);

		// API Object should not be null ;)
		this->api->SetStomtToSend(stomt);

		FString LogFileName = FApp::GetGameName() + FString(TEXT(".log"));

		this->api->SendLogFile(this->api->ReadLogFile(LogFileName), LogFileName);

		// Check EMail
		this->IsEMailAlreadyKnown = this->api->Config->GetSubscribed();

		UE_LOG(Stomt, Log, TEXT("Is EMail Already Known: %s"), this->IsEMailAlreadyKnown ? TEXT("true") : TEXT("false"));
	}
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
	this->ImageURL = this->api->GetImageURL();

	this->api->OnTargetRequestComplete.Broadcast(TargetRequest);
}

void UStomtPluginWidget::UploadScreenshot()
{
	this->api->SendImageFile(this->api->ReadScreenshotAsBase64());
}
