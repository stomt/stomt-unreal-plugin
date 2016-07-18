// Copyright 2016 Daniel Schukies. All Rights Reserved.

#pragma once
#include "StomtPluginPrivatePCH.h"
#include "StomtAPI.h"




UStomtAPI::UStomtAPI()
{
	this->request = NewObject<UStomtRestRequest>();
	this->restURL = TEXT("https://test.rest.stomt.com");
	this->targetName = TEXT("...");
	this->SetAppID("R18OFQXmb6QzXwzP1lWdiZ7Y9");
	this->SetTargetID("unreal");
}

UStomtAPI::~UStomtAPI()
{
}

void UStomtAPI::SendStomt(UStomt* stomt)
{
	this->request->SetVerb(ERequestVerb::POST);
	this->request->SetHeader(TEXT("appid"), this->GetAppID() );

	this->request->GetRequestObject()->SetField(TEXT("target_id"),	UStomtJsonValue::ConstructJsonValueString(	this, stomt->GetTargetID()	));
	this->request->GetRequestObject()->SetField(TEXT("positive"),	UStomtJsonValue::ConstructJsonValueBool(	this, stomt->GetPositive()	));
	this->request->GetRequestObject()->SetField(TEXT("text"),		UStomtJsonValue::ConstructJsonValueString(	this, stomt->GetText()		));
	this->request->GetRequestObject()->SetField(TEXT("anonym"),		UStomtJsonValue::ConstructJsonValueBool(	this, stomt->GetAnonym()	));

	this->request->ProcessURL( this->GetRestURL().Append(TEXT("/stomts")) );
}

void UStomtAPI::ReadTarget(FString targetID)
{
	this->request->SetVerb(ERequestVerb::GET);
	this->request->SetHeader(TEXT("appid"), this->GetAppID() );

	this->request->ProcessURL( this->GetRestURL().Append("/targets/").Append( this->GetTargetID() ).Append("?target_id=").Append( this->GetTargetID() ) );
}

void UStomtAPI::SetRestURL(FString URL)
{
	this->restURL = URL;
}

FString UStomtAPI::GetRestURL()
{
	return this->restURL;
}

void UStomtAPI::SetAppID(FString appID)
{
	this->appID = appID;
}

FString UStomtAPI::GetAppID()
{
	return this->appID;
}

FString UStomtAPI::GetTargetName()
{
	return this->targetName;
}

void UStomtAPI::SetTargetID(FString targetID)
{
	this->targetID = targetID;
}

FString UStomtAPI::GetTargetID()
{
	return this->targetID;
}

void UStomtAPI::SetImageURL(FString URL)
{
	this->imageURL = URL;
}

FString UStomtAPI::GetImageURL()
{
	return this->imageURL;
}

UStomtRestRequest * UStomtAPI::GetRequest()
{
	return this->request;
}

