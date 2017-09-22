// Copyright 2017 Daniel Schukies. All Rights Reserved.


#pragma once
#include "StomtPluginPrivatePCH.h"
#include "StomtRestRequest.h"
#include "StomtTrack.generated.h"



/**
 * 
 */
UCLASS()
class UStomtTrack : public UObject
{
	GENERATED_BODY()
public:

	static UStomtTrack* ConstructStomtConfig();

	UStomtTrack();

	~UStomtTrack();
	//////////////////////////////////////////////////////////////////////////
	// Public





	//////////////////////////////////////////////////////////////////////////
	// Read Track

private:




	//////////////////////////////////////////////////////////////////////////
	// Write Track

private:



private:


	//////////////////////////////////////////////////////////////////////////
	// Data


	FString DevicePlatform;
	FString	DeviceID;

	FString	SDKType;
	FString	SDKVersion;
	FString	SDKIntegration;

	FString	TargetID;
	FString	StomtID;

	FString	EventCategory;
	FString EventAction;
	FString EventLabel;

};
