// Copyright 2018 STOMT GmbH. All Rights Reserved.
#pragma once

#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

#include "Blueprint/UserWidget.h"

#include "StomtRestRequest.h"
#include "Stomt.h"
#include "StomtAPI.h"
#include "StomtConfig.h"

#include "StomtPluginWidget.generated.h"

UCLASS()
class UStomtPluginWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	~UStomtPluginWidget();

	/**
	 *	Stomt content-text from the UI textbox.	
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stomt Widget Plugin")
	FString Message;

	/**
	*	EMail from the UI EMail layer.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stomt Widget Plugin")
	FString EMail;

	/**
	*	UserName from the UI Login layer.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stomt Widget Plugin")
	FString UserName;

	/**
	*	UserPassword from the UI Login layer.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stomt Widget Plugin")
	FString UserPassword;

	/**
	*	Target-name that will be shown in the widget.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stomt Widget Plugin")
	FString TargetName;

	/**
	*	Target-image URL
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stomt Widget Plugin")
	FString	ImageURL;

	/**
	*	STOMT API.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stomt Widget Plugin")
	UStomtAPI* api;

	/**
	*	STOMT Config.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stomt Widget Plugin")
	UStomtConfig* Config;

	/**
	*	Whether the E-Mail variable is a phone number.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stomt Widget Plugin")
	bool UsePhoneNumber;

	/**
	*	Whether the stomt is not positive (a wish).
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stomt Widget Plugin")
	bool IsWish;

	/**
	*	Whether a screenshot should be appended. (not implemented)
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stomt Widget Plugin")
	bool IsScreenshotSelected;

	/**
	*	Whether the user mail is already known. 
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stomt Widget Plugin")
	bool IsEMailAlreadyKnown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stomt Widget Plugin")
	bool IsUserLoggedIn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stomt Widget Plugin")
	bool UploadLogs;

	/**
	*	Error-Code whether the user login worked. (0: OK, 403: wrong password, 404: account does not exist. )
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stomt Widget Plugin")
	int LoginErrorCode;

	/**
	*	The labels which will be appended to the stomt.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stomt Widget Plugin")
	TArray<FString> Labels;

	/**
	*	Event when text inside the widget changed.
	*	@param text - Current text from the widget-textbox
	*/
	UFUNCTION(BlueprintCallable, Category = "Stomt Widget Plugin")
	void OnMessageChanged(FString text);

	/**
	*	Once the user sends off the stomt.
	*/
	UFUNCTION(BlueprintCallable, Category = "Stomt Widget Plugin")
	void OnSubmit();

	/**
	*	Once the user finishes all layers.
	*/
	UFUNCTION(BlueprintCallable, Category = "Stomt Widget Plugin")
	void OnSubmitLastLayer();

	/**
	*	Once the user finishes the login.
	*	@return Wether the login Request was send.
	*/
	UFUNCTION(BlueprintCallable, Category = "Stomt Widget Plugin")
	bool OnSubmitLogin();

	/**
	*	Once the user finishes the email input.
	*/
	UFUNCTION(BlueprintCallable, Category = "Stomt Widget Plugin")
	void OnSubmitEMail();

	/**
	*	Once the user logs out.
	*/
	UFUNCTION(BlueprintCallable, Category = "Stomt Widget Plugin")
	void OnLogout();

	/**
	*	Called at the widget startup to initialize variables.
	*	@param TargetID - Stomt target-id
	*	@param RestURL - Stomt REST-API URL
	*	@param AppID - Stomt application-ID
	*/
	UFUNCTION(BlueprintCallable, Category = "Stomt Widget Plugin")
	void OnConstruction(FString AppID);

	/**
	*	Event called after the stomt server responded.
	*	@param CurrentRequest - Stomt Request that carries the response information.
	*/
	UFUNCTION(BlueprintCallable, Category = "Stomt Widget Plugin")
	void OnLoginResponse(UStomtRestRequest* CurrentRequest);

	UFUNCTION()
	void OnTargetResponse(UStomtRestRequest * TargetRequest);
	
	UFUNCTION(BlueprintCallable, Category = "Stomt Widget Plugin")
	FString AppendStomtURLParams(FString url, FString utm_content);
	
	// Not ready yet.
	UFUNCTION(BlueprintCallable, Category = "Stomt Widget Plugin")
	void UploadScreenshot();

private:

	UPROPERTY()
	UStomt* stomt;

};
