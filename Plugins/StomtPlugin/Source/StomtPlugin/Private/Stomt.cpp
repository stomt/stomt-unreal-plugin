// Copyright 2016 Daniel Schukies. All Rights Reserved.

#pragma once
#include "StomtPluginPrivatePCH.h"
#include "Stomt.h"

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

void UStomt::SetLabels(TArray<FString> labels)
{
//	UStomt::labels.Append(label);
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

TArray<FString> UStomt::GetLabels()
{
	return TArray<FString>();
}


