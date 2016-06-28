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
	/*	//UE_LOG(LogTemp, Warning, TEXT("MessageChanged %s", text) ); */
	/*	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, this->Message); */
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
		UStomt* stomt = UStomt::ConstructStomt(TEXT("unreal"), true, this->Message);

		if (api == NULL)
		{
			api = NewObject<UStomtAPI>();
		}

		api->SendStomt(stomt);
	}
}

void UStomtPluginWidget::OnConstruction()
{
	if (api == NULL)
	{
		api = NewObject<UStomtAPI>();
	}

	api->ReadTarget(TEXT("unreal"));

	this->TargetName = api->GetTargetName();
}

void UStomtPluginWidget::ChangeButtonOrder(UButton *FirstButton, UButton *SecondButton)
{
	//FirstButton->GetSlots

}
