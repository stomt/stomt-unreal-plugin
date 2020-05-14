// Copyright 2018 STOMT GmbH. All Rights Reserved.
#pragma once

#include "Stomt.h"
#include "StomtPluginPrivatePCH.h"

//////////////////////////////////////////////////////////////////////////
// Construction

UStomt::UStomt()
{

}

UStomt* UStomt::ConstructStomt(FString NewTargetId, bool bNewPositive, FString NewText)
{
	UStomt* Stomt = NewObject<UStomt>();
	Stomt->SetTargetID(NewTargetId);
	Stomt->SetPositive(bNewPositive);
	Stomt->SetText(NewText);

	return Stomt;
}

//////////////////////////////////////////////////////////////////////////
// Data accessors

void UStomt::SetTargetID(FString NewTargetId)
{
	this->TargetId = NewTargetId;
}

void UStomt::SetPositive(bool bNewPositive)
{
	this->bPositive = bNewPositive;
}

void UStomt::SetText(FString NewText)
{
	this->Text = NewText;
}

void UStomt::SetAnonym(bool bNewAnonym)
{
	this->bAnonym = bNewAnonym;
}

void UStomt::SetServersideID(FString NewServersideID)
{
	this->ServersideID = NewServersideID;
}

void UStomt::AddLabel(UStomtLabel* newLabel)
{
	this->Labels.Add(newLabel);
}
void UStomt::SetLabels(TArray<UStomtLabel*> NewLabels)
{
	this->Labels = NewLabels;
}

void UStomt::SetLabels(TArray<FString> NewLabels)
{
	if (NewLabels.Num() <= 0)
		return;

	this->Labels.Empty();

	for (int i = 0; i != NewLabels.Num(); ++i)
	{
		this->Labels.Add(UStomtLabel::ConstructLabel(NewLabels[i]));
	}
}

FString UStomt::GetTargetID()
{
	return this->TargetId;
}

bool UStomt::GetPositive()
{
	return this->bPositive;
}

FString UStomt::GetText()
{
	return this->Text;
}

bool UStomt::GetAnonym()
{
	return this->bAnonym;
}
TArray<UStomtLabel*> UStomt::GetLabels()
{
	return this->Labels;
}

FString UStomt::GetServersideID()
{
	return this->ServersideID;
}
