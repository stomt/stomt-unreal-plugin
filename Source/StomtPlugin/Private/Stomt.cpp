// Copyright 2018 STOMT GmbH. All Rights Reserved.
#pragma once

#include "Stomt.h"
#include "StomtPluginPrivatePCH.h"

//////////////////////////////////////////////////////////////////////////
// Construction

UStomt::UStomt()
{

}

UStomt* UStomt::ConstructStomt(FString target_id, bool positive, FString text)
{
	UStomt* stomt = NewObject<UStomt>();
	stomt->SetTargetID(target_id);
	stomt->SetPositive(positive);
	stomt->SetText(text);

	return stomt;
}

//////////////////////////////////////////////////////////////////////////
// Data accessors

void UStomt::SetTargetID(FString target_id)
{
	this->target_id = target_id;
}

void UStomt::SetPositive(bool positive)
{
	this->positive = positive;
}

void UStomt::SetText(FString text)
{
	this->text = text;
}

void UStomt::SetAnonym(bool IsAnonym)
{
	this->anonym = IsAnonym;
}

void UStomt::SetServersideID(FString serversideID)
{
	this->serversideID = serversideID;
}

void UStomt::AddLabel(UStomtLabel* label)
{
	this->labels.Add(label);
}
void UStomt::SetLabels(TArray<UStomtLabel*> labels)
{
	this->labels = labels;
}

void UStomt::SetLabels(TArray<FString> labels)
{
	if (labels.Num() <= 0)
		return;

	this->labels.Empty();

	for (int i = 0; i != labels.Num(); ++i)
	{
		this->labels.Add(UStomtLabel::ConstructLabel(labels[i]));
	}
}

FString UStomt::GetTargetID()
{
	return this->target_id;
}

bool UStomt::GetPositive()
{
	return this->positive;
}

FString UStomt::GetText()
{
	return this->text;
}

bool UStomt::GetAnonym()
{
	return this->anonym;
}
TArray<UStomtLabel*> UStomt::GetLabels()
{
	return this->labels;
}

FString UStomt::GetServersideID()
{
	return this->serversideID;
}
