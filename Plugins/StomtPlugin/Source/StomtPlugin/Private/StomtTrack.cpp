// Copyright 2017 Daniel Schukies. All Rights Reserved.

#pragma once
#include "StomtPluginPrivatePCH.h"
#include "StomtTrack.h"
#include "Runtime/Core/Public/Misc/FileHelper.h"
#include "Runtime/Core/Public/Misc/Paths.h"
#include "Runtime/Core/Public/GenericPlatform/GenericPlatformFile.h"
#include "Runtime/Core/Public/Misc/App.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Runtime/Core/Public/GenericPlatform/GenericPlatformProcess.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "StomtJsonObject.h"

UStomtTrack* UStomtTrack::ConstructStomtTrack()
{
	UStomtTrack* track = NewObject<UStomtTrack>();
	UE_LOG(StomtInit, Log, TEXT("Constuct Stomt SDK Track"));

	track->SetDevicePlatform(UGameplayStatics::GetPlatformName());
	UE_LOG(StomtInit, Log, TEXT("DevicePlatform: %"), *track->DevicePlatform);

	track->SetDeviceID(FGenericPlatformProcess::ComputerName());
	UE_LOG(StomtInit, Log, TEXT("DeviceID: %"), *track->DeviceID);

	track->SetSDKType(FString("UnrealEngine") + UKismetSystemLibrary::GetEngineVersion());
	UE_LOG(StomtInit, Log, TEXT("SDKType: %"), *track->SDKType);

	track->SetSDKVersion("beta-2017");
	UE_LOG(StomtInit, Log, TEXT("SDKVersion: %"), *track->SDKVersion);

	track->SetSDKIntegration(UKismetSystemLibrary::GetGameName());
	UE_LOG(StomtInit, Log, TEXT("SDKIntegration: %"), *track->SDKIntegration);

	return track;
}


UStomtTrack::UStomtTrack()
{

}

UStomtTrack::~UStomtTrack()
{

}

UStomtRestJsonObject * UStomtTrack::GetAsJsonObject()
{
	UStomtRestJsonObject* jsonObj = UStomtRestJsonObject::ConstructJsonObject(this);

	if (!DevicePlatform.IsEmpty())
	{
		jsonObj->SetStringField("device_platform", DevicePlatform);
	}

	if (!DeviceID.IsEmpty())
	{
		jsonObj->SetStringField("device_id", DeviceID);
	}

	if (!SDKType.IsEmpty())
	{
		jsonObj->SetStringField("sdk_type", SDKType);
	}

	if (!SDKVersion.IsEmpty())
	{
		jsonObj->SetStringField("sdk_version", SDKVersion);
	}

	if (!SDKIntegration.IsEmpty())
	{
		jsonObj->SetStringField("sdk_integration", SDKIntegration);
	}

	if (!TargetID.IsEmpty())
	{
		jsonObj->SetStringField("target_id", TargetID);
	}

	if (!StomtID.IsEmpty())
	{
		jsonObj->SetStringField("stomt_id", StomtID);
	}

	if (!EventCategory.IsEmpty())
	{
		jsonObj->SetStringField("event_category", EventCategory);
	}

	if (!EventAction.IsEmpty())
	{
		jsonObj->SetStringField("event_action", EventAction);
	}

	if (!EventLabel.IsEmpty())
	{
		jsonObj->SetStringField("event_label", EventLabel);
	}

	return jsonObj;
}

void UStomtTrack::SetDevicePlatform(FString DevicePlatform)
{
	this->DevicePlatform = DevicePlatform;
}

void  UStomtTrack::SetDeviceID(FString DeviceID)
{
	this->DeviceID = DeviceID;
}

void UStomtTrack::SetSDKType(FString SDKType)
{
	this->SDKType = SDKType;
}

void UStomtTrack::SetSDKVersion(FString SDKVersion)
{
	this->SDKVersion = SDKVersion;
}

void UStomtTrack::SetSDKIntegration(FString SDKIntegration)
{
	this->SDKIntegration = SDKIntegration;
}

void UStomtTrack::SetTargetID(FString TargetID)
{
	this->TargetID = TargetID;
}

void UStomtTrack::SetStomtID(FString StomtID)
{
	this->StomtID = StomtID;
}

void UStomtTrack::SetEventCategory(FString EventCategory)
{
	this->EventCategory = EventCategory;
}

void UStomtTrack::SetEventAction(FString EventAction)
{
	this->EventAction = EventAction;
}

void UStomtTrack::SetEventLabel(FString EventLabel)
{
	this->EventLabel = EventLabel;
}
