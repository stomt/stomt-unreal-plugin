// Copyright 2018 STOMT GmbH. All Rights Reserved.
#pragma once

#include "StomtPluginPrivatePCH.h"
#include "StomtLabel.generated.h"

UCLASS()
class UStomtLabel : public UObject
{
	GENERATED_BODY()

public:

	//////////////////////////////////////////////////////////////////////////
	// Construction

	UStomtLabel();
	//~UStomtLabel();

	static UStomtLabel* ConstructLabel(FString name);


	//////////////////////////////////////////////////////////////////////////
	// Data accessors

	void SetName(FString name);
	void SetColor(FString color);
	void SetIsPublic(bool isPublic);
	void SetAsTargetOwner(bool asTargetOwner);

	FString GetName();
	FString GetColor();
	bool GetIsPublic();
	bool GetAsTargetOwner();

private:

	//////////////////////////////////////////////////////////////////////////
	// Data

	UPROPERTY()
	FString name;
	UPROPERTY()
	FString color;
	UPROPERTY()
	bool isPublic;
	UPROPERTY()
	bool asTargetOwner;

};

