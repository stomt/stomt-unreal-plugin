// Copyright 2016 Daniel Schukies. All Rights Reserved.


#pragma once
#include "StomtPluginPrivatePCH.h"
#include "StomtRestRequest.h"
#include "Stomt.h"
#include "Runtime/Engine/Classes/Engine/TextureRenderTarget2D.h"

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

	/**
	* Sends a stomt. 
	* @param stomt - Stomt Object with some content.
	*/
	void SendStomt(UStomt* stomt);

	/**
	* Sends stomt labels.
	* @param stomt - Stomt Object with serverside ID set and labels set.
	*/
	void SendStomtLabels(UStomt* stomt);

	/**
	* Sends an REST request for a stomt target.
	* To receive the respose it is necessary to add a event delegate function.
	* For example: api->GetRequest()->OnRequestComplete.AddDynamic(this, &UStomtPluginWidget::OnReceiving).
	* @param targetID - ID of the requested stomt target.
	*/
	void RequestTarget(FString targetID);

	//////////////////////////////////////////////////////////////////////////
	// Data accessors

	/**
	* Sets the Stomt rest Server URL.
	* For example: https://rest.stomt.com
	* @param URL - Stomt REST URL
	*/
	void	SetRestURL(FString URL);
	FString GetRestURL();

	/**
	* Sets the Stomt App ID.
	* That was created here: https://www.stomt.com/dev/my-apps
	* @param appID - Stomt APP ID
	*/
	void	SetAppID(FString appID);
	FString GetAppID();

	/**
	* Returns the Target-Name that can be displayed.
	* Only call this after a target was requested.
	*/
	FString GetTargetName();

	/**
	* Sets a Stomt Target
	* @param targetID - ID of Stomt Target
	*/
	void	SetTargetID(FString targetID);
	FString	GetTargetID();

	/**
	* Sets the URL for the target image.
	* @param URL - Target-Image URL
	*/
	void	SetImageURL(FString URL);
	FString	GetImageURL();

	/**
	* Gets the request object that contains request/response information.
	*/
	UStomtRestRequest* GetRequest();

	UFUNCTION(BlueprintCallable, Category = "Stomt Widget Plugin")
	bool CaptureComponent2D_SaveImage(class USceneCaptureComponent2D* Target, const FString ImagePath, const FLinearColor ClearColour);

	UFUNCTION(BlueprintCallable, Category = "Stomt Widget Plugin")
	void SaveRenderTargetToDisk(UTextureRenderTarget2D* InRenderTarget, FString Filename);



	//////////////////////////////////////////////////////////////////////////
	// Data

private:
	UPROPERTY()
	UStomtRestRequest*	request;

	FString				restURL;
	FString				targetName;
	FString				targetID;
	FString				appID;
	FString				imageURL;

};
