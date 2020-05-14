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
	* @param NewTargetId - Stomt target ID
	* @param bNewPositive - whether it is wish(false) or like(true)
	* @param NewText - Stomt content text.
	*/
	static UStomt* ConstructStomt(FString NewTargetId, bool bNewPositive, FString NewText);

	//////////////////////////////////////////////////////////////////////////
	// Destruction and reset
	//TODO reset functions

	//////////////////////////////////////////////////////////////////////////
	// Data accessors

	void SetTargetID(FString NewTargetId);
	void SetPositive(bool bNewPositive);
	void SetText(FString NewText);
	void SetAnonym(bool bNewAnonym);
	void SetServersideID(FString NewServersideID);

	// Labels
	void AddLabel(UStomtLabel* NewLabel);
	void SetLabels(TArray<UStomtLabel*> NewLabels);
	void SetLabels(TArray<FString> NewLabels);

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
	FString		ServersideID;
	UPROPERTY()
	FString		TargetId;
	UPROPERTY()
	bool		bPositive;
	UPROPERTY()
	FString		Text;
	UPROPERTY()
	FString		Url;
	UPROPERTY()
	bool		bAnonym;
	UPROPERTY()
	FString		ImgName;
	UPROPERTY()
	FString		LonLat;
	UPROPERTY()
	TArray<UStomtLabel*> Labels;
};
