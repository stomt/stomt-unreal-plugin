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

public:
	//////////////////////////////////////////////////////////////////////////
	// Write Track

	void SetDevicePlatform(FString DevicePlatform);
	void SetDeviceID(FString DeviceID);
	void SetSDKType(FString SDKType);

	void SetSDKVersion(FString SDKVersion);
	void SetSDKIntegration(FString SDKIntegration);
	void SetTargetID(FString TargetID);
	void SetStomtID(FString StomtID);

	UFUNCTION(BlueprintCallable, Category = "Stomt Track")
	void SetEventCategory(FString EventCategory);

	UFUNCTION(BlueprintCallable, Category = "Stomt Track")
	void SetEventAction(FString EventAction);

	UFUNCTION(BlueprintCallable, Category = "Stomt Track")
	void SetEventLabel(FString EventLabel);


private:

	//////////////////////////////////////////////////////////////////////////
	// Data

	// automatically set in ConstructStomtTrack() //

	FString DevicePlatform;		// optional, Window, Linux, …
	FString	DeviceID;			// optional, unique custom identifier

	FString	SDKType;			// required, unreal, unity, ios, …
	FString	SDKVersion;			// optional
	FString	SDKIntegration;		// optional, the name of the game or build...

	// NOT automatically set //

	FString TargetID;			// optional, id of the target selected in the sdk
	FString	StomtID;			// optional, id of the created/voted/commented stomt

	FString	EventCategory;		// required, Typically the object that was interacted with (e.g. 'Video')
	FString EventAction;		// required, The type of interaction (e.g. 'play')
	FString EventLabel;			// optional, Useful for categorization

};
