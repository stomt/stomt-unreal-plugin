// Copyright 2018 STOMT GmbH. All Rights Reserved.
#pragma once

#include "StomtPluginPrivatePCH.h"
#include "StomtRestRequest.h"
#include "StomtTrack.generated.h"

UCLASS()
class UStomtTrack : public UObject
{
	GENERATED_BODY()

public:

	static UStomtTrack* ConstructStomtTrack();

	UStomtTrack();

	~UStomtTrack();


public:
	//////////////////////////////////////////////////////////////////////////
	// Read Track

	UStomtRestJsonObject* GetAsJsonObject();

	//////////////////////////////////////////////////////////////////////////
	// Write Track

	void SetDevicePlatform(FString NewDevicePlatform);
	
	void SetDeviceId(FString NewDeviceId);
	
	void SetSdkType(FString NewSdkType);
	
	void SetSdkVersion(FString NewSdkVersion);
	
	void SetSdkIntegration(FString NewSdkIntegration);
	
	void SetTargetId(FString NewTargetId);
	
	void SetStomtId(FString NewStomtId);

	UFUNCTION(BlueprintCallable, Category = "Stomt Track")
	void SetEventCategory(FString NewEventCategory);

	UFUNCTION(BlueprintCallable, Category = "Stomt Track")
	void SetEventAction(FString NewEventAction);

	UFUNCTION(BlueprintCallable, Category = "Stomt Track")
	void SetEventLabel(FString NewEventLabel);


private:

	//////////////////////////////////////////////////////////////////////////
	// Data

	// Automatically set in ConstructStomtTrack() //

	// optional, Window, Linux, …
	FString DevicePlatform;
	
	// optional, unique custom identifier
	FString	DeviceId;

	// required, unreal, unity, ios, …
	FString	SdkType;

	// optional
	FString	SdkVersion;

	// optional, the name of the game or build...
	FString	SdkIntegration;


	// NOT automatically set //

	// optional, id of the target selected in the Sdk
	FString TargetId;
	
	// optional, id of the created/voted/commented Stomt
	FString	StomtId;

	// required, Typically the object that was interacted with (e.g. 'Video')
	FString	EventCategory;

	// required, The type of interaction (e.g. 'play')
	FString EventAction;

	// optional, Useful for categorization
	FString EventLabel;

};
