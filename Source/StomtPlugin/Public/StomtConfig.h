// Copyright 2018 STOMT GmbH. All Rights Reserved.
#pragma once
#include "StomtJsonObject.h"
#include "Stomt.h"
#include "StomtPluginPrivatePCH.h"
#include "StomtRestRequest.h"
#include "StomtConfig.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConfigUpdated, class UStomtConfig*, Config);

UCLASS()
class UStomtConfig : public UObject
{
	GENERATED_BODY()

public:

	static UStomtConfig* ConstructStomtConfig();

	UStomtConfig();

	~UStomtConfig();
	//////////////////////////////////////////////////////////////////////////
	// Public

	/**
	*	checks if a config file exists, creates one or reads it
	**/
	UFUNCTION(BlueprintCallable, Category = "Stomt Widget Plugin")
	void Load();

	/**
	*	deletes to current config file
	**/
	void Delete();

	UFUNCTION(BlueprintCallable, Category = "Stomt Widget Plugin")
	FString GetAccessToken();

	void SetAccessToken(FString NewAccessToken);

	UFUNCTION(BlueprintCallable, Category = "Stomt Widget Plugin")
	bool GetSubscribed();

	void SetSubscribed(bool bNewSubscribed);

	UFUNCTION(BlueprintCallable, Category = "Stomt Widget Plugin")
	bool	GetLoggedIn();

	void SetLoggedIn(bool bNewLoggedIn);

	UFUNCTION(BlueprintCallable, Category = "Stomt Widget Plugin")
	bool GetAcceptScreenshotUpload();

	UFUNCTION(BlueprintCallable, Category = "Stomt Widget Plugin")
	void SetAcceptScreenshotUpload(bool bNewAcceptScreenshotUpload);

	UFUNCTION(BlueprintCallable, Category = "Stomt Widget Plugin")
	bool GetAcceptLogUpload();

	UFUNCTION(BlueprintCallable, Category = "Stomt Widget Plugin")
	void SetAcceptLogUpload(bool bNewAcceptLogUpload);

	UPROPERTY(BlueprintAssignable, Category = "Stomt|Event")
	FOnConfigUpdated OnConfigUpdated;

	UFUNCTION(BlueprintCallable, Category = "Stomt Widget Plugin")
	TArray<UStomtJsonValue*> GetStomtsAsJson();

	UFUNCTION(BlueprintCallable, Category = "Stomt Widget Plugin")
	TArray<UStomt*> GetStomts();
	
	UFUNCTION(BlueprintCallable, Category = "Stomt Widget Plugin")
	bool AddStomt(UStomt* NewStomt);

	/**
	* Clears the stomt array in /stomt/stomt.conf.json
	*/
	bool ClearStomts();

private:

	//////////////////////////////////////////////////////////////////////////
	// Read Config

	/**
	* Loads the access token from disk.
	*/
	FString ReadStomtConf(FString FieldName);

	/**
	* Loads the flag from disk.
	*/
	bool ReadFlag(FString FlagName);

	/**
	* Loads the access token from disk.
	*/
	UStomtRestJsonObject* ReadStomtConfAsJson();

	/**
	* Loads the access token from disk.
	*/
	FString ReadAccesstoken();

	//////////////////////////////////////////////////////////////////////////
	// Write Config

	/**
	* Saves the access token in /stomt/stomt.conf.json
	*/
	bool SaveAccessToken(FString NewAccessToken);

	/**
	* Saves the a value in /stomt/stomt.conf.json
	*/
	bool SaveValueToStomtConf(FString FieldName, FString FieldValue);

	/**
	* Saves the a stomt in /stomt/stomt.conf.json
	*/
	bool SaveStomtToConf(UStomt& NewStomt);

	/**
	* Saves the flag in /stomt/stomt.conf.json
	*/
	bool SaveFlag(FString FlagName, bool bFlagState);

	/**
	* Loads the access token from disk.
	*/
	bool WriteStomtConfAsJson(UStomtRestJsonObject* StomtConf);

	/**
	* Delete stomt.conf.json
	*/
	void DeleteStomtConf();

	/**
	* Loads an Log file from disk.
	* @param LogFileName - Log File Name.
	*/
	FString ReadLogFile(FString LogFileName);

	bool WriteFile(FString TextToSave, FString FileName, FString SaveDirectory, bool AllowOverwriting);
	
	bool ReadFile(FString& Result, FString FileName, FString SaveDirectory);

	//////////////////////////////////////////////////////////////////////////
	// Data

	bool bSubscribed;
	bool bLoggedIn;
	bool bAcceptLogUpload;
	bool bAcceptScreenshotUpload;

	FString SubscribedFieldName;
	FString LoggedInFieldName;
	FString AccessTokenFieldName;
	FString StomtsFieldName;
	FString LogUploadFieldName;
	FString ScreenshotUploadFieldName;

	FString AccessToken;
	FString ConfigFolder;
	FString ConfigName;

	UPROPERTY()
	TArray<UStomtJsonValue*> Stomts;
};
