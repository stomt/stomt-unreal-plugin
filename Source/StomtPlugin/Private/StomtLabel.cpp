// Copyright 2018 STOMT GmbH. All Rights Reserved.
#pragma once

#include "StomtLabel.h"
#include "StomtPluginPrivatePCH.h"

//////////////////////////////////////////////////////////////////////////
// Construction

UStomtLabel::UStomtLabel()
{
	// Default 
	/*
	this->Color = FString("#5EBEFF");
	this->bIsPublic = false;
	this->bAsTargetOwner = false;
	*/
}

//////////////////////////////////////////////////////////////////////////
// Data accessors

UStomtLabel * UStomtLabel::ConstructLabel(FString NewName)
{
	UStomtLabel* StomtLabel = NewObject<UStomtLabel>();
	StomtLabel->SetName(NewName);

	return StomtLabel;
}

void UStomtLabel::SetName(FString NewName)
{
	this->Name = NewName;
}

void UStomtLabel::SetColor(FString NewColor)
{
	this->Color = NewColor;
}

void UStomtLabel::SetIsPublic(bool bNewIsPublic)
{
	this->bIsPublic = bNewIsPublic;
}

void UStomtLabel::SetAsTargetOwner(bool bNewAsTargetOwner)
{
	this->bAsTargetOwner = bNewAsTargetOwner;
}

FString UStomtLabel::GetName()
{
	return this->Name;
}

FString UStomtLabel::GetColor()
{
	return this->Color;
}

bool UStomtLabel::GetIsPublic()
{
	return this->bIsPublic;
}

bool UStomtLabel::GetAsTargetOwner()
{
	return this->bAsTargetOwner;
}
