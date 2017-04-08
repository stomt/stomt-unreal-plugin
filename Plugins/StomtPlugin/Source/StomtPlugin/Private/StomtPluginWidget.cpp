// Copyright 2016 Daniel Schukies. All Rights Reserved.


#include "StomtPluginPrivatePCH.h"
#include "StomtPluginWidget.h"
#include "StomtRestRequest.h"
#include "Runtime/Engine/Classes/Components/SceneCaptureComponent2D.h"



UStomtPluginWidget::~UStomtPluginWidget()
{
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
		UStomt* stomt = UStomt::ConstructStomt(this->api->GetTargetID(), !this->IsWish, this->Message);

		stomt->SetAnonym(true);

		if (api == NULL)
		{
			api = NewObject<UStomtAPI>();
		}

		api->SendStomt(stomt);
	}
}

void UStomtPluginWidget::OnConstruction(FString TargetID, FString RestURL, FString AppID)
{
	if (api == NULL)
	{
		api = NewObject<UStomtAPI>();
	}
	this->api->SetAppID(AppID);
	this->api->SetTargetID(TargetID);

	this->Request = this->api->GetRequest();

	api->RequestTarget(TargetID);
	this->api->GetRequest()->OnRequestComplete.AddDynamic(this, &UStomtPluginWidget::OnReceiving);
}

void UStomtPluginWidget::OnReceiving(UStomtRestRequest * Request)
{
	if (Request->GetResponseObject()->HasField(TEXT("data")))
	{
		if (Request->GetResponseObject()->GetObjectField(TEXT("data"))->HasField(TEXT("displayname")))
		{
			this->TargetName = Request->GetResponseObject()->GetObjectField(TEXT("data"))->GetStringField(TEXT("displayname"));
		}

		this->api->SetImageURL(Request->GetResponseObject()
			->GetObjectField(TEXT("data"))
			->GetObjectField(TEXT("images"))
			->GetObjectField(TEXT("profile"))
			->GetStringField(TEXT("url")));

		this->ImageURL = this->api->GetImageURL();
	}
}

void UStomtPluginWidget::TakeScreenshot()
{
	USceneCaptureComponent2D* cap = NewObject<USceneCaptureComponent2D>();

	api->CaptureComponent2D_SaveImage(cap, FString("/bild/"), FLinearColor());
	/*
	FTexureRenderTarget2DResource *Texture = (FTextureRenderTarget2DResource *)SceneCapture->TextureTarget->Resource;
	TArray<FColor> ColorBuffer;
	Texture->ReadPixels(ColorBuffer);*/
}

