// Copyright 2017 Daniel Schukies. All Rights Reserved.


#pragma once
#include "StomtPluginPrivatePCH.h"
#include "StomtRestRequest.h"
#include "Stomt.h"
#include "StomtConfig.h"
#include "Runtime/Engine/Classes/Engine/TextureRenderTarget2D.h"
#include "StomtAPI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetRequestComplete, class UStomtRestRequest*, Request);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoginRequestComplete, class UStomtRestRequest*, Request);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRequestFailed, class UStomtRestRequest*, Request);


/**
 * 
 */
UCLASS()
class UStomtAPI : public UObject
{
	GENERATED_BODY()
public:

	static UStomtAPI* ConstructStomtAPI(FString TargetID, FString RestURL, FString AppID);

	UStomtAPI();

	~UStomtAPI();

	/**
	* Sends a stomt. 
	* @param stomt - Stomt Object with some content.
	*/
	void SendStomt(UStomt* stomt);

	UFUNCTION()
	void OnSendStomtRequestResponse(UStomtRestRequest * Request);

	UStomtRestRequest* SendLoginRequest(FString UserName, FString Password);

	UFUNCTION()
	void OnLoginRequestResponse(UStomtRestRequest * Request);

	/**
	* Sends stomt labels. (deprecated)
	* @param stomt - Stomt Object with serverside ID set and labels set.
	*/
	void SendStomtLabels(UStomt* stomt);

	/**
	* Sends an REST Request for a stomt target.
	* To receive the respose it is necessary to add a event delegate function.
	* For example: api->GetRequest()->OnRequestComplete.AddDynamic(this, &UStomtPluginWidget::OnLoginRequestResponse).
	* @param TargetID - ID of the requested stomt target.
	*/
	UStomtRestRequest* RequestTarget(FString targetID);

	UFUNCTION()
	void OnRequestTargetResponse(UStomtRestRequest * Request);

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
	* @param AppID - Stomt APP ID
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
	* @param TargetID - ID of Stomt Target
	*/
	void	SetTargetID(FString targetID);
	FString	GetTargetID();

	/**
	* Sets the URL for the target image.
	* @param URL - Target-Image URL
	*/
	void	SetImageURL(FString URL);
	FString	GetImageURL();

	void SetStomtToSend(UStomt* stomt);

	/**
	* Gets the Request object that contains Request/response information.
	*/
	UStomtRestRequest* GetRequest();

	//////////////////////////////////////////////////////////////////////////
	// Stomt File Access

	/**
	* Loads an Log file from disk.
	* @param LogFileName - Log File Name.
	*/
	FString ReadLogFile(FString LogFileName);

	//////////////////////////////////////////////////////////////////////////
	// Network

	/**
	* Sends the LogFileData to stomt.com server.
	*/
	void SendLogFile(FString LogFileData, FString LogFileName);

	UFUNCTION()
	void OnSendLogFileResponse(UStomtRestRequest * Request);

	/**
	* Sends the Image to stomt.com server.
	*/
	void SendImageFile(FString ImageFileDataBase64);

	UFUNCTION()
	void OnSendImageFileResponse(UStomtRestRequest * Request);

	void SendEMail(FString EMail);

	UFUNCTION()
	void OnSendEMailResponse(UStomtRestRequest * Request);

	void SendLogoutRequest();

	UFUNCTION()
	void OnSendLogoutResponse(UStomtRestRequest * Request);


	//////////////////////////////////////////////////////////////////////////
	// Screenshot

	UFUNCTION()
	FString ReadScreenshotAsBase64();

	UFUNCTION(BlueprintCallable, Category = "Stomt Widget Plugin")
	bool CaptureComponent2D_SaveImage(class USceneCaptureComponent2D* Target, const FString ImagePath, const FLinearColor ClearColour);

	UFUNCTION(BlueprintCallable, Category = "Stomt Widget Plugin")
	void SaveRenderTargetToDisk(UTextureRenderTarget2D* InRenderTarget, FString Filename);


	//////////////////////////////////////////////////////////////////////////
	// Request callbacks

public:
	/** Event occured when the Request has been completed */
	UPROPERTY(BlueprintAssignable, Category = "Stomt|Event")
	FOnTargetRequestComplete OnTargetRequestComplete; 

	/** Event occured when the Request has been completed */
	UPROPERTY(BlueprintAssignable, Category = "Stomt|Event")
	FOnLoginRequestComplete OnLoginRequestComplete;

	/** Event occured when the Request has been completed */
	//UPROPERTY(BlueprintAssignable, Category = "Stomt|Event")
	//FOnRequestComplete OnRequestComplete; // ToDo Trigger this

	/** Event occured when the Request wasn't successfull */
	UPROPERTY(BlueprintAssignable, Category = "Stomt|Event")
	FOnRequestFailed OnRequestFailed; 

	UFUNCTION()
	void OnARequestFailed(UStomtRestRequest* Request);

	//////////////////////////////////////////////////////////////////////////
	// Data
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stomt API")
	UStomtConfig*	Config;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stomt API")
	FString			ImageUploadName;

	FString			errorLog_file_uid;
	FString			DefaultScreenshotName;

	bool			IsLogUploadComplete;
	bool			IsImageUploadComplete;
	bool			LoginRequestWasSend;
	bool			EMailFlagWasSend;
	bool			LogFileWasSend;
	UStomt*			StomtToSend;

	FString			RestURL;
	FString			TargetName;
	FString			TargetID;
	FString			AppID;
	FString			ImageURL;

	//////////////////////////////////////////////////////////////////////////
	// Helper Functions

public:

	UFUNCTION(BlueprintCallable, Category = "Stomt API")
	bool DoesScreenshotFileExist();
	
private:

	bool WriteFile(FString TextToSave, FString FileName, FString SaveDirectory, bool AllowOverwriting);
	bool ReadFile(FString& Result, FString FileName, FString SaveDirectory);
	TArray<uint8> ReadBinaryFile(FString FilePath);

	UStomtRestRequest*	SetupNewPostRequest();
	UStomtRestRequest*	SetupNewDeleteRequest();
	void				AddAccesstokenToRequest(UStomtRestRequest* Request);
};
