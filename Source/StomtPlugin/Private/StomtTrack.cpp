// Copyright 2018 STOMT GmbH. All Rights Reserved.
#pragma once

#include "StomtTrack.h"
#include "StomtPluginPrivatePCH.h"
#include "StomtJsonObject.h"

#include "Runtime/Core/Public/Misc/FileHelper.h"
#include "Runtime/Core/Public/Misc/Paths.h"
#include "Runtime/Core/Public/GenericPlatform/GenericPlatformFile.h"
#include "Runtime/Core/Public/Misc/App.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Runtime/Core/Public/GenericPlatform/GenericPlatformProcess.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"

UStomtTrack* UStomtTrack::ConstructStomtTrack()
{
	UStomtTrack* Track = NewObject<UStomtTrack>();
	return Track;
}

UStomtTrack::UStomtTrack()
{
	UE_LOG(StomtInit, Log, TEXT("Constuct Stomt Sdk Track"));

	this->SetDevicePlatform(UGameplayStatics::GetPlatformName());
	UE_LOG(StomtInit, Log, TEXT("DevicePlatform: %s"), *this->DevicePlatform);

	this->SetDeviceID(FGenericPlatformProcess::ComputerName());
	UE_LOG(StomtInit, Log, TEXT("DeviceId: %s"), *this->DeviceId);

	this->SetSdkType(FString("UnrealEngine") + UKismetSystemLibrary::GetEngineVersion());
	UE_LOG(StomtInit, Log, TEXT("SdkType: %s"), *this->SdkType);

	this->SetSdkVersion("2.3.1");
	UE_LOG(StomtInit, Log, TEXT("SdkVersion: %s"), *this->SdkVersion);

	this->SetSdkIntegration(UKismetSystemLibrary::GetGameName());
	UE_LOG(StomtInit, Log, TEXT("SdkIntegration: %s"), *this->SdkIntegration);
}

UStomtTrack::~UStomtTrack()
{

}

UStomtRestJsonObject * UStomtTrack::GetAsJsonObject()
{
	UStomtRestJsonObject* JsonObj = UStomtRestJsonObject::ConstructJsonObject(this);

	if (!this->DevicePlatform.IsEmpty())
	{
		JsonObj->SetStringField("device_platform", this->DevicePlatform);
	}

	if (!this->DeviceId.IsEmpty())
	{
		JsonObj->SetStringField("device_id", this->DeviceId);
	}

	if (!this->SdkType.IsEmpty())
	{
		JsonObj->SetStringField("sdk_type", this->SdkType);
	}

	if (!this->SdkVersion.IsEmpty())
	{
		JsonObj->SetStringField("sdk_version", this->SdkVersion);
	}

	if (!this->SdkIntegration.IsEmpty())
	{
		JsonObj->SetStringField("sdk_integration", this->SdkIntegration);
	}

	if (!this->TargetId.IsEmpty())
	{
		JsonObj->SetStringField("target_id", this->TargetId);
	}

	if (!this->StomtId.IsEmpty())
	{
		JsonObj->SetStringField("stomt_id", this->StomtId);
	}

	if (!this->EventCategory.IsEmpty())
	{
		JsonObj->SetStringField("event_category", this->EventCategory);
	}

	if (!this->EventAction.IsEmpty())
	{
		JsonObj->SetStringField("event_action", this->EventAction);
	}

	if (!this->EventLabel.IsEmpty())
	{
		JsonObj->SetStringField("event_label", this->EventLabel);
	}

	return JsonObj;
}

void UStomtTrack::SetDevicePlatform(FString NewDevicePlatform)
{
	this->DevicePlatform = NewDevicePlatform;
}

void  UStomtTrack::SetDeviceID(FString NewDeviceId)
{
	this->DeviceId = NewDeviceId;
}

void UStomtTrack::SetSdkType(FString NewSdkType)
{
	this->SdkType = NewSdkType;
}

void UStomtTrack::SetSdkVersion(FString NewSdkVersion)
{
	this->SdkVersion = NewSdkVersion;
}

void UStomtTrack::SetSdkIntegration(FString NewSdkIntegration)
{
	this->SdkIntegration = NewSdkIntegration;
}

void UStomtTrack::SetTargetId(FString NewTargetId)
{
	this->TargetId = NewTargetId;
}

void UStomtTrack::SetStomtId(FString NewStomtId)
{
	this->StomtId = NewStomtId;
}

void UStomtTrack::SetEventCategory(FString NewEventCategory)
{
	this->EventCategory = NewEventCategory;
}

void UStomtTrack::SetEventAction(FString NewEventAction)
{
	this->EventAction = NewEventAction;
}

void UStomtTrack::SetEventLabel(FString NewEventLabel)
{
	this->EventLabel = NewEventLabel;
}
