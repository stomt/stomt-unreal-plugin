// Copyright 2016 Daniel Schukies. All Rights Reserved.


#pragma once
#include "StomtPluginPrivatePCH.h"
#include "StomtRestRequest.h"
#include "StomtLabel.h"
#include "Stomt.generated.h"

UCLASS()
class UStomt : public UObject
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
	FString		serversideID;
	FString		target_id;
	bool		positive;
	FString		text;
	FString		url;
	bool		anonym;
	FString		img_name;
	FString		lonlat;
	TArray<UStomtLabel*> labels;
};

