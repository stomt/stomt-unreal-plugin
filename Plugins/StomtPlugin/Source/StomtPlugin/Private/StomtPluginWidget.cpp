// Copyright 2016 Daniel Schukies. All Rights Reserved.


#include "StomtPluginPrivatePCH.h"
#include "StomtPluginWidget.h"
#include "StomtRestRequest.h"



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
		UStomt* stomt = UStomt::ConstructStomt(TEXT("unreal"), !this->IsWish, this->Message);

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

	api->ReadTarget(TargetID);
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

void UStomtPluginWidget::ChangeButtonOrder(UButton *FirstButton, UButton *SecondButton)
{
	//FirstButton->GetSlots

}
