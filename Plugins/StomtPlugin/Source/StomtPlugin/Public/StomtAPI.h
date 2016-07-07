// Fill out your copyright notice in the Description page of Project Settings.


#pragma once
#include "StomtPluginPrivatePCH.h"
#include "StomtRestRequest.h"
#include "Stomt.h"
#include "StomtAPI.generated.h"

/**
 * 
 */
UCLASS()
class UStomtAPI : public UObject
{
	GENERATED_BODY()
public:

	UStomtAPI();
	~UStomtAPI();


	void SendStomt(UStomt* stomt);

	void ReadTarget(FString targetID);

	//////////////////////////////////////////////////////////////////////////
	// Data accessors


	void	SetRestURL(FString URL);
	FString GetRestURL();

	void	SetAppID(FString appID);
	FString GetAppID();

	FString GetTargetName();

	void	SetTargetID(FString targetID);
	FString	GetTargetID();

	UStomtRestRequest* GetRequest();


	//////////////////////////////////////////////////////////////////////////
	// Data

private:
	UStomtRestRequest*	request;
	FString				restURL;
	FString				targetName;
	FString				targetID;
	FString				appID;

};
