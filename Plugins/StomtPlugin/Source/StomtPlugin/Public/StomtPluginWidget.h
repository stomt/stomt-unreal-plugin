// Copyright 2016 Daniel Schukies. All Rights Reserved.

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

#include "StomtPluginWidget.generated.h"

/**
 * 
 */
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
	*	REST request to send stomt.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stomt Widget Plugin")
	UStomtRestRequest* Request;

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
	*	Called at the widget startup to initialize variables.
	*	@param TargetID - Stomt target-id
	*	@param RestURL - Stomt REST-API URL
	*	@param AppID - Stomt application-ID
	*/
	UFUNCTION(BlueprintCallable, Category = "Stomt Widget Plugin")
	void OnConstruction(FString TargetID, FString RestURL, FString AppID);

	/**
	*	Event called after the stomt server responded.
	*	@param CurrentRequest - Stomt request that carries the response information.
	*/
	UFUNCTION(BlueprintCallable, Category = "Stomt Widget Plugin")
	void OnReceiving(UStomtRestRequest* CurrentRequest);


	UFUNCTION(BlueprintCallable, Category = "Stomt Widget Plugin")
	void TakeScreenshot();

private:

	UPROPERTY()
	UStomt* stomt;

	UPROPERTY()
	UStomtAPI* api;
};
