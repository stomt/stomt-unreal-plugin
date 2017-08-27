// Copyright 2016 Daniel Schukies. All Rights Reserved.

#pragma once
#include "StomtPluginPrivatePCH.h"
#include "StomtAPI.h"
#include "Runtime/ImageWrapper/Public/Interfaces/IImageWrapper.h"
#include "Runtime/ImageWrapper/Public/Interfaces/IImageWrapperModule.h"
#include "Runtime/Engine/Public/HighResScreenshot.h"
#include "Runtime/Core/Public/Misc/FileHelper.h"
#include "Runtime/Core/Public/Misc/Paths.h"
#include "Runtime/Core/Public/GenericPlatform/GenericPlatformFile.h"
#include "Runtime/Core/Public/Misc/Base64.h"
#include "Runtime/Core/Public/Misc/App.h"
#include "StomtJsonObject.h"

UStomtAPI* UStomtAPI::ConstructRequest(FString TargetID, FString RestURL, FString AppID)
{
	UStomtAPI* api = NewObject<UStomtAPI>();
	api->SetAppID(AppID);
	api->SetTargetID(TargetID);
	api->SetRestURL(RestURL);

	return api;
}


UStomtAPI::UStomtAPI()
{
	this->configFolder = FString(TEXT("/.stomt/"));
	this->configName = FString(TEXT("stomt.conf.json"));
	this->accesstoken = FString(TEXT(""));
	ReadStomtConf(TEXT("accesstoken"));

	this->request = NewObject<UStomtRestRequest>();

	this->request->OnRequestComplete.AddDynamic(this, &UStomtAPI::OnReceiving);

	LogFileWasSend = false;
	EMailFlagWasSend = false;
}

UStomtAPI::~UStomtAPI()
{
}

void UStomtAPI::SendStomt(UStomt* stomt)
{
	this->SetupNewPostRequest();

	// Fields
	this->request->GetRequestObject()->SetField(TEXT("target_id"),	UStomtJsonValue::ConstructJsonValueString(	this, stomt->GetTargetID()	));
	this->request->GetRequestObject()->SetField(TEXT("positive"),	UStomtJsonValue::ConstructJsonValueBool(	this, stomt->GetPositive()	));
	this->request->GetRequestObject()->SetField(TEXT("text"),		UStomtJsonValue::ConstructJsonValueString(	this, stomt->GetText()		));
	this->request->GetRequestObject()->SetField(TEXT("anonym"),		UStomtJsonValue::ConstructJsonValueBool(	this, stomt->GetAnonym()	));
	
	//Labels
	UStomtRestJsonObject* jObjExtraData = UStomtRestJsonObject::ConstructJsonObject(this);
	TArray<UStomtJsonValue*> labels = TArray<UStomtJsonValue*>();

	for (int i = 0; i != stomt->GetLabels().Num(); ++i)
	{
		labels.Add(UStomtJsonValue::ConstructJsonValueString(this, stomt->GetLabels()[i]->GetName() ));
	}

	jObjExtraData->SetArrayField(TEXT("labels"), labels);
	this->request->GetRequestObject()->SetObjectField(TEXT("extradata"), jObjExtraData);

	// Error Logs
	if (!this->errorLog_file_uid.IsEmpty())
	{
		UStomtRestJsonObject* jObjFile = UStomtRestJsonObject::ConstructJsonObject(this);
		UStomtRestJsonObject* jObjFileContext = UStomtRestJsonObject::ConstructJsonObject(this);
		jObjFileContext->SetField(TEXT("file_uid"), UStomtJsonValue::ConstructJsonValueString(this, this->errorLog_file_uid));

		jObjFile->SetObjectField(TEXT("stomt"), jObjFileContext);
		this->request->GetRequestObject()->SetObjectField(TEXT("files"), jObjFile);
	}
	
	this->request->ProcessURL( this->GetRestURL().Append(TEXT("/stomts")) );
}

void UStomtAPI::SendLoginRequest(FString UserName, FString Password)
{
	this->SetupNewPostRequest();
	this->request->UseRequestLogging(false);

	this->request->GetRequestObject()->SetStringField(TEXT("login_method"), TEXT("normal"));
	this->request->GetRequestObject()->SetStringField(TEXT("emailusername"), UserName);
	this->request->GetRequestObject()->SetStringField(TEXT("password"), Password);

	this->request->ProcessURL(this->GetRestURL().Append(TEXT("/authentication/session")));

	LoginRequestWasSend = true;
	this->request->UseRequestLogging(true);
}

void UStomtAPI::SendStomtLabels(UStomt * stomt)
{
	return;
	//ToDo: Finish this some day. Works only with target owner login.

	/*
	if (!stomt->GetServersideID().IsEmpty() && stomt->GetLabels().Max() > 0)
	{
		//Reset Request
		this->request->ResetResponseData();
		this->request->ResetRequestData();

		UE_LOG(LogTemp, Warning, TEXT("nice1"));
		this->request->SetVerb(ERequestVerb::POST);
		this->request->SetHeader(TEXT("appid"), this->GetAppID() );

		this->request->GetRequestObject()->SetField(TEXT("name"),	UStomtJsonValue::ConstructJsonValueString(	this, TEXT("newlabeltest")	));
		this->request->GetRequestObject()->SetField(TEXT("as_target_owner"), UStomtJsonValue::ConstructJsonValueString(this, TEXT("true")));

		this->request->ProcessURL( this->GetRestURL().Append(TEXT("/stomts/")).Append(stomt->GetServersideID()).Append(TEXT("/labels") ) );
	}
	*/
}

void UStomtAPI::RequestTarget(FString targetID)
{
	this->request->SetVerb(ERequestVerb::GET);
	this->request->SetHeader(TEXT("appid"), this->GetAppID() );

	this->request->ProcessURL( this->GetRestURL().Append("/targets/").Append( this->GetTargetID() ).Append("?target_id=").Append( this->GetTargetID() ) );
}

void UStomtAPI::SetRestURL(FString URL)
{
	this->restURL = URL;
}

FString UStomtAPI::GetRestURL()
{
	return this->restURL;
}

void UStomtAPI::SetAppID(FString appID)
{
	this->appID = appID;
}

FString UStomtAPI::GetAppID()
{
	return this->appID;
}

FString UStomtAPI::GetTargetName()
{
	return this->targetName;
}

void UStomtAPI::SetTargetID(FString targetID)
{
	this->targetID = targetID;
}

FString UStomtAPI::GetTargetID()
{
	return this->targetID;
}

void UStomtAPI::SetImageURL(FString URL)
{
	this->imageURL = URL;
}

FString UStomtAPI::GetImageURL()
{
	return this->imageURL;
}

void UStomtAPI::SetStomtToSend(UStomt * stomt)
{
	this->StomtToSend = stomt;
}

UStomtRestRequest * UStomtAPI::GetRequest()
{
	return this->request;
}

bool UStomtAPI::SaveAccesstoken(FString accesstoken)
{
	return SaveValueToStomtConf(TEXT("accesstoken"), accesstoken);
}

bool UStomtAPI::SaveValueToStomtConf(FString FieldName, FString FieldValue)
{
	UStomtRestJsonObject* jsonObj = ReadStomtConfAsJson();

	if (jsonObj->HasField(FieldName))
	{
		if (jsonObj->GetStringField(FieldName).Equals(FieldValue))
		{
			return false;
		}	

		jsonObj->RemoveField(FieldName);
	}
	
	jsonObj->SetStringField(FieldName, FieldValue);

	return this->WriteFile(jsonObj->EncodeJson(), configName, configFolder, true);
}



bool UStomtAPI::SaveFlag(FString FlagName, bool FlagState)
{
	return SaveValueToStomtConf(FlagName, FlagState ? TEXT("true") : TEXT("false"));
}

FString UStomtAPI::ReadStomtConf(FString FieldName)
{
	FString result;

	if (this->ReadFile(result, configName, configFolder))
	{
		UStomtRestJsonObject* jsonObj = UStomtRestJsonObject::ConstructJsonObject(this);
		jsonObj->DecodeJson(result);
		this->accesstoken = jsonObj->GetField(FieldName)->AsString();
	}

	return result;
}

bool UStomtAPI::ReadFlag(FString FlagName)
{
	FString result;
	bool FlagState = false;

	if (this->ReadFile(result, configName, configFolder))
	{
		UStomtRestJsonObject* jsonObj = UStomtRestJsonObject::ConstructJsonObject(this);
		jsonObj->DecodeJson(result);
		FlagState = jsonObj->GetField(FlagName)->AsBool();
	}

	return FlagState;
}

UStomtRestJsonObject* UStomtAPI::ReadStomtConfAsJson()
{
	FString result;
	UStomtRestJsonObject* jsonObj = UStomtRestJsonObject::ConstructJsonObject(this);

	if (this->ReadFile(result, configName, configFolder))
	{
		jsonObj->DecodeJson(result);
	}

	return jsonObj;
}

bool UStomtAPI::WriteStomtConfAsJson(UStomtRestJsonObject * StomtConf)
{
	return this->WriteFile(StomtConf->EncodeJson(), configName, configFolder, true);
}

void UStomtAPI::DeleteStomtConf()
{
	FString file = this->configFolder + this->configName;
	if (!FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*file))
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not delete stomt.conf.json: %s"), *file);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Deleted stomt.conf.json because of wrong access token Path: %s"), *file);
	}
}


FString UStomtAPI::ReadLogFile(FString LogFileName)
{
	FString errorLog;

	FString LogFilePath = FPaths::GameLogDir() + LogFileName;
	FString LogFileCopyPath = FPaths::GameLogDir() + LogFileName + TEXT("Copy.log");
	FString LogFileCopyName = LogFileName + TEXT("Copy.log");

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	PlatformFile.BypassSecurity(true);

	// Copy LogFileData
	if (!PlatformFile.CopyFile(*LogFileCopyPath, *LogFilePath, EPlatformFileRead::AllowWrite, EPlatformFileWrite::AllowRead))
	{
		UE_LOG(LogTemp, Warning, TEXT("LogFile Copy did not work FromFile: %s | ToFile %s"), *LogFilePath, *LogFileCopyPath);
	}

	// Read LogFileCopy from Disk
	if (!this->ReadFile(errorLog, LogFileCopyName, FPaths::GameLogDir() ))
	{
		if (FPaths::FileExists(FPaths::GameLogDir() + LogFileCopyName))
		{
			UE_LOG(LogTemp, Warning, TEXT("Could not read LogFile %s, but it exists"), *LogFileCopyName);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Could not read LogFile %s, because it does not exist"), *LogFileCopyName);
		}
	}

	// Delete LogFileCopy
	if (!PlatformFile.DeleteFile(*LogFileCopyPath))
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not delete LogFileCopy %s"), *LogFileCopyPath);
	}

	return errorLog;
}

void UStomtAPI::SendLogFile(FString LogFileData, FString LogFileName)
{
	this->SetupNewPostRequest();

	FString logJson = FString(TEXT("{ \"files\": { \"stomt\": [ { \"data\":\"") + FBase64::Encode(LogFileData) + TEXT("\", \"filename\" : \"") + LogFileName + TEXT("\" } ] } }"));

	this->request->UseStaticJsonString(true);
	this->request->SetStaticJsonString(logJson);

	this->request->ProcessURL(this->GetRestURL().Append(TEXT("/files")));

	LogFileWasSend = true;
}

void UStomtAPI::SendEMail(FString EMail)
{
	this->SetupNewPostRequest();

	this->request->GetRequestObject()->SetStringField(TEXT("email"), EMail);

	this->request->ProcessURL(this->GetRestURL().Append(TEXT("/authentication/subscribe")));

	this->EMailFlagWasSend = true;
}

void UStomtAPI::OnReceiving(UStomtRestRequest * Request)
{

	// Wrong access token
	if (Request->GetResponseCode() == 403 || Request->GetResponseCode() == 419)
	{
		LoginRequestWasSend = false;

		UStomtRestJsonObject* stomtconf = this->ReadStomtConfAsJson();
		stomtconf->RemoveField(accesstoken);
		WriteStomtConfAsJson(stomtconf);

		this->accesstoken.Empty();
	}


	if (LoginRequestWasSend)
	{
		if (Request->GetResponseCode() == 403 || Request->GetResponseCode() == 404 || Request->GetResponseCode() == 413)
		{
			LoginRequestWasSend = false;

			if (StomtToSend != NULL)
			{
				this->SendStomt(StomtToSend);
			}
		}
		else
		{
			if (Request->GetResponseObject()->HasField(TEXT("data")))
			{
				if (Request->GetResponseObject()->GetObjectField(TEXT("data"))->HasField(TEXT("accesstoken")))
				{
					this->accesstoken = Request->GetResponseObject()->GetObjectField(TEXT("data"))->GetStringField(TEXT("accesstoken"));
					this->SaveAccesstoken(this->accesstoken);
					LoginRequestWasSend = false;
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Login did not work"));
				}
			}
		}
	}

	if (LogFileWasSend)
	{
		// Request access token
		if (this->accesstoken.IsEmpty())
		{
			if (Request->GetResponseObject()->HasField(TEXT("meta")))
			{
				if (Request->GetResponseObject()->GetObjectField(TEXT("meta"))->HasField(TEXT("accesstoken")))
				{
					this->accesstoken = Request->GetResponseObject()->GetObjectField(TEXT("meta"))->GetStringField(TEXT("accesstoken"));
					this->SaveAccesstoken(this->accesstoken);
				}
			}
		}

		// Get File uid of the error log and send stomt
		if (Request->GetResponseObject()->HasField(TEXT("data")))
		{
			if (Request->GetResponseObject()->GetObjectField(TEXT("data"))->HasField(TEXT("files")))
			{
				this->errorLog_file_uid = Request->GetResponseObject()->GetObjectField(TEXT("data"))->GetObjectField(TEXT("files"))->GetObjectField(TEXT("stomt"))->GetStringField("file_uid");
				this->LogFileWasSend = false;
				this->SendStomt(StomtToSend);
			}
		}

		if (Request->GetResponseCode() == 403 || Request->GetResponseCode() == 419 || Request->GetResponseCode() == 413)
		{
			if (StomtToSend != NULL)
			{
				this->SendStomt(StomtToSend);
				LogFileWasSend = false;
			}
		}
	}

	if (EMailFlagWasSend)
	{
		if (! (Request->GetResponseCode() == 400))
		{
			if (Request->GetResponseObject()->HasField(TEXT("data")))
			{
				if (Request->GetResponseObject()->GetObjectField(TEXT("data"))->HasField(TEXT("success")))
				{
					this->SaveFlag(TEXT("email"), true);
				}
				else
				{
					this->SaveFlag(TEXT("email"), false);
				}
			}
			
		}
		else
		{
			this->SaveFlag(TEXT("email"), true);
		}

		EMailFlagWasSend = false;
	}



}

bool UStomtAPI::CaptureComponent2D_SaveImage(USceneCaptureComponent2D * Target, const FString ImagePath, const FLinearColor ClearColour)
{
	// Bad scene capture component! No render target! Stay! Stay! Ok, feed!... wait, where was I?
	if ((Target == nullptr) || (Target->TextureTarget == nullptr))
	{
		return false;
	}

	FRenderTarget* RenderTarget = Target->TextureTarget->GameThread_GetRenderTargetResource();
	if (RenderTarget == nullptr)
	{
		return false;
	}

	TArray<FColor> RawPixels;

	// Format not supported - use PF_B8G8R8A8.
	if (Target->TextureTarget->GetFormat() != PF_B8G8R8A8)
	{
		// TRACEWARN("Format not supported - use PF_B8G8R8A8.");
		return false;
	}

	if (!RenderTarget->ReadPixels(RawPixels))
	{
		return false;
	}

	// Convert to FColor.
	FColor ClearFColour = ClearColour.ToFColor(false); // FIXME - want sRGB or not?

	for (auto& Pixel : RawPixels)
	{
		// Switch Red/Blue changes.
		const uint8 PR = Pixel.R;
		const uint8 PB = Pixel.B;
		Pixel.R = PB;
		Pixel.B = PR;

		// Set alpha based on RGB values of ClearColour.
		Pixel.A = ((Pixel.R == ClearFColour.R) && (Pixel.G == ClearFColour.G) && (Pixel.B == ClearFColour.B)) ? 0 : 255;
	}

	//IImageWrapperPtr ImageWrapper = GetImageWrapperByExtention(ImagePath);
	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	IImageWrapperPtr ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);

	const int32 Width = Target->TextureTarget->SizeX;
	const int32 Height = Target->TextureTarget->SizeY;

	if (ImageWrapper.IsValid() && ImageWrapper->SetRaw(&RawPixels[0], RawPixels.Num() * sizeof(FColor), Width, Height, ERGBFormat::RGBA, 8))
	{
		FFileHelper::SaveArrayToFile(ImageWrapper->GetCompressed(), *ImagePath);
		return true;
	}

	return false;
}

void UStomtAPI::SaveRenderTargetToDisk(UTextureRenderTarget2D* InRenderTarget, FString Filename)
{
	FTextureRenderTargetResource* RTResource = InRenderTarget->GameThread_GetRenderTargetResource();

	FReadSurfaceDataFlags ReadPixelFlags(RCM_UNorm);
	ReadPixelFlags.SetLinearToGamma(true);

	TArray<FColor> OutBMP;
	RTResource->ReadPixels(OutBMP, ReadPixelFlags);

	for (FColor& color : OutBMP)
	{
		color.A = 255;
	}


	FIntRect SourceRect;

	FIntPoint DestSize(InRenderTarget->GetSurfaceWidth(), InRenderTarget->GetSurfaceHeight());


	FString ResultPath;
	FHighResScreenshotConfig& HighResScreenshotConfig = GetHighResScreenshotConfig();
	HighResScreenshotConfig.SaveImage(Filename, OutBMP, DestSize, &ResultPath);
}

bool UStomtAPI::WriteFile(FString TextToSave, FString FileName, FString SaveDirectory, bool AllowOverwriting)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	// CreateDirectoryTree returns true if the destination
	// directory existed prior to call or has been created
	// during the call.
	if (PlatformFile.CreateDirectoryTree(*SaveDirectory))
	{
		// Get absolute file path
		FString AbsoluteFilePath = SaveDirectory + "/" + FileName;

		// Allow overwriting or file doesn't already exist
		if (AllowOverwriting || !FPaths::FileExists(*AbsoluteFilePath))
		{
			//Use " FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);" for append
			return FFileHelper::SaveStringToFile(TextToSave, *AbsoluteFilePath);
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool UStomtAPI::ReadFile(FString& Result, FString FileName, FString SaveDirectory)
{	

	FString path = SaveDirectory + FileName;

/*
	if (SaveDirectory.GetCharArray()[SaveDirectory.Len() - 2] == '/')
	{
		UE_LOG(LogTemp, Warning, TEXT(" / am enden: %s "), *path);
	}*/

	if (!FPaths::FileExists(path))
	{
		UE_LOG(LogTemp, Warning, TEXT("File with this path does not exist: %s "), *path);
	}

	return FFileHelper::LoadFileToString( Result, *path);

}

inline void UStomtAPI::SetupNewPostRequest()
{
	//Reset Request
	this->request->ResetResponseData();
	this->request->ResetRequestData();

	this->request->SetVerb(ERequestVerb::POST);
	this->request->SetHeader(TEXT("appid"), this->GetAppID());

	if (!this->accesstoken.IsEmpty())
	{
		this->request->SetHeader(TEXT("accesstoken"), this->accesstoken);
	}
}


