// Copyright 2016 Daniel Schukies. All Rights Reserved.

#pragma once
#include "StomtPluginPrivatePCH.h"
#include "StomtLabel.h"


//////////////////////////////////////////////////////////////////////////
// Construction



UStomtLabel::UStomtLabel()
{
	// Default 
	/*
	this->color = FString("#5EBEFF");
	this->isPublic = false;
	this->asTargetOwner = false;
	*/
}

/*
UStomtLabel::~UStomtLabel()
{
}
*/

//////////////////////////////////////////////////////////////////////////
// Data accessors

UStomtLabel * UStomtLabel::ConstructLabel(FString name)
{
	UStomtLabel* stomtLabel = NewObject<UStomtLabel>();
	stomtLabel->SetName(name);

	return stomtLabel;
}

void UStomtLabel::SetName(FString name)
{
	this->name = name;
}

void UStomtLabel::SetColor(FString color)
{
	this->color = color;
}

void UStomtLabel::SetIsPublic(bool isPublic)
{
	this->isPublic = isPublic;
}

void UStomtLabel::SetAsTargetOwner(bool asTargetOwner)
{
	this->asTargetOwner = asTargetOwner;
}

FString UStomtLabel::GetName()
{
	return this->name;
}

FString UStomtLabel::GetColor()
{
	return this->color;
}

bool UStomtLabel::GetIsPublic()
{
	return this->isPublic;
}

bool UStomtLabel::GetAsTargetOwner()
{
	return this->asTargetOwner;
}
