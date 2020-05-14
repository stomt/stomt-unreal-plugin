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

	static UStomtLabel* ConstructLabel(FString NewName);


	//////////////////////////////////////////////////////////////////////////
	// Data accessors

	void SetName(FString NewName);
	void SetColor(FString NewColor);
	void SetIsPublic(bool bNewIsPublic);
	void SetAsTargetOwner(bool bNewAsTargetOwner);

	FString GetName();
	FString GetColor();
	bool GetIsPublic();
	bool GetAsTargetOwner();

private:

	//////////////////////////////////////////////////////////////////////////
	// Data

	UPROPERTY()
	FString Name;
	UPROPERTY()
	FString Color;
	UPROPERTY()
	bool bIsPublic;
	UPROPERTY()
	bool bAsTargetOwner;

};

