// Copyright 2018 STOMT GmbH. All Rights Reserved.
#pragma once

#include "StomtPluginPrivatePCH.h"
#include "StomtRestRequest.h"
#include "StomtLabel.h"
#include "Stomt.generated.h"

UCLASS()
class STOMTPLUGIN_API UStomt : public UObject
{
	GENERATED_BODY()

public:
	//////////////////////////////////////////////////////////////////////////
	// Construction

	UStomt();

	/** 
	* Creates new Stomt object 
	* @param target_id - Stomt target ID
	* @param positive - whether it is wish(false) or like(true)
	* @param text - Stomt content text.
	*/
	static UStomt* ConstructStomt(FString target_id, bool positive, FString text);

	//////////////////////////////////////////////////////////////////////////
	// Destruction and reset
	//TODO reset functions

	//////////////////////////////////////////////////////////////////////////
	// Data accessors

	void SetTargetID(FString target_id);
	void SetPositive(bool positive);
	void SetText(FString text);
	void SetAnonym(bool IsAnonym);
	void SetServersideID(FString serversideID);

	// Labels
	void AddLabel(UStomtLabel* label);
	void SetLabels(TArray<UStomtLabel*> labels);
	void SetLabels(TArray<FString> labels);

	FString					GetTargetID();
	bool					GetPositive();
	FString					GetText();
	bool					GetAnonym();
	TArray<UStomtLabel*>	GetLabels();
	FString					GetServersideID();
	

	//////////////////////////////////////////////////////////////////////////
	// Data

private:
	UPROPERTY()
	FString		serversideID;
	UPROPERTY()
	FString		target_id;
	UPROPERTY()
	bool		positive;
	UPROPERTY()
	FString		text;
	UPROPERTY()
	FString		url;
	UPROPERTY()
	bool		anonym;
	UPROPERTY()
	FString		img_name;
	UPROPERTY()
	FString		lonlat;
	UPROPERTY()
	TArray<UStomtLabel*> labels;
};

