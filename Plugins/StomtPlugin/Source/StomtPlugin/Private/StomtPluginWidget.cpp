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
	//TakeScreenshot();

	if (!this->Message.IsEmpty())
	{
		this->stomt = UStomt::ConstructStomt(this->api->GetTargetID(), !this->IsWish, this->Message);
		this->stomt->SetLabels(this->Labels);

		stomt->SetAnonym(false);

		// API Object should not be null ;)
		this->api->SetStomtToSend(stomt);

		FString LogFileName = FApp::GetGameName() + FString(TEXT(".log"));

		this->api->SendLogFile(this->api->ReadLogFile(LogFileName), LogFileName);
	}
}

void UStomtPluginWidget::OnSubmitLastLayer()
{
	/*
	if (!this->EMail.IsEmpty() && !this->UserPassword.IsEmpty())
	{
		this->api->SendLoginRequest(this->EMail, this->UserPassword);
	}

	if (!this->EMail.IsEmpty())
	{
		this->api->SendEMail(this->EMail);
	}
	*/
}

void UStomtPluginWidget::OnSubmitLogin()
{
	if (!this->UserName.IsEmpty() && !this->UserPassword.IsEmpty())
	{
		this->api->SendLoginRequest(this->UserName, this->UserPassword);
	}

}

void UStomtPluginWidget::OnSubmitEMail()
{
	if (!this->EMail.IsEmpty())
	{
		this->api->SendEMail(this->EMail);
	}
}

void UStomtPluginWidget::OnConstruction(FString TargetID, FString RestURL, FString AppID)
{
	// Create API Object
	if (api == NULL)
	{
		api = UStomtAPI::ConstructRequest(TargetID, RestURL, AppID);
	}

	// Request Target Name
	this->api->SetAppID(AppID);
	this->api->SetTargetID(TargetID);
	this->api->SetRestURL(RestURL);

	this->Request = this->api->GetRequest();

	this->api->RequestTarget(TargetID);
	this->api->GetRequest()->OnRequestComplete.AddDynamic(this, &UStomtPluginWidget::OnReceiving);

	//Lookup EMail
	this->IsEMailAlreadyKnown = this->api->ReadFlag(TEXT("email"));
}


void UStomtPluginWidget::OnReceiving(UStomtRestRequest * Request)
{
	if (Request->GetResponseObject()->HasField(TEXT("data")))
	{
		if (Request->GetResponseObject()->GetObjectField(TEXT("data"))->HasField(TEXT("displayname")))
		{
			this->TargetName = Request->GetResponseObject()->GetObjectField(TEXT("data"))->GetStringField(TEXT("displayname"));

			this->api->SetImageURL(Request->GetResponseObject()
				->GetObjectField(TEXT("data"))
				->GetObjectField(TEXT("images"))
				->GetObjectField(TEXT("profile"))
				->GetStringField(TEXT("url")));

			this->ImageURL = this->api->GetImageURL();

		}
	}

	if (Request->GetResponseCode() == 403 || Request->GetResponseCode() == 404)
	{
		this->LoginErrorCode = Request->GetResponseCode();
	}
}

void UStomtPluginWidget::TakeScreenshot()
{
	USceneCaptureComponent2D* cap = NewObject<USceneCaptureComponent2D>();
	UTextureRenderTarget2D* RenderTarget = NewObject<UTextureRenderTarget2D>();
	RenderTarget->InitAutoFormat(1024, 1024);
	//UMaterialInstanceDynamic* ScopeMat = UMaterialInstanceDynamic::Create(AssetScopeMat_Default, this);
	cap->TextureTarget = RenderTarget;
	if (!api->CaptureComponent2D_SaveImage(cap, FString("bild.png"), FLinearColor()))
	{
		UE_LOG(LogTemp, Warning, TEXT("bad!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("nice"));
	}
	/*
	FTexureRenderTarget2DResource *Texture = (FTextureRenderTarget2DResource *)SceneCapture->TextureTarget->Resource;
	TArray<FColor> ColorBuffer;
	Texture->ReadPixels(ColorBuffer);*/
}

