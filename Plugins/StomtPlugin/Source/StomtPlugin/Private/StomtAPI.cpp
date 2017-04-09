// Copyright 2016 Daniel Schukies. All Rights Reserved.

#pragma once
#include "StomtPluginPrivatePCH.h"
#include "StomtAPI.h"
#include "Runtime/ImageWrapper/Public/Interfaces/IImageWrapper.h"
#include "Runtime/ImageWrapper/Public/Interfaces/IImageWrapperModule.h"
#include "Runtime/Engine/Public/HighResScreenshot.h"




UStomtAPI::UStomtAPI()
{
	this->request = NewObject<UStomtRestRequest>();
	this->restURL = TEXT("https://test.rest.stomt.com");
	this->targetName = TEXT("...");
	this->SetAppID("R18OFQXmb6QzXwzP1lWdiZ7Y9");
	this->SetTargetID("unreal");
}

UStomtAPI::~UStomtAPI()
{
}

void UStomtAPI::SendStomt(UStomt* stomt)
{
	this->request->SetVerb(ERequestVerb::POST);
	this->request->SetHeader(TEXT("appid"), this->GetAppID() );

	this->request->GetRequestObject()->SetField(TEXT("target_id"),	UStomtJsonValue::ConstructJsonValueString(	this, stomt->GetTargetID()	));
	this->request->GetRequestObject()->SetField(TEXT("positive"),	UStomtJsonValue::ConstructJsonValueBool(	this, stomt->GetPositive()	));
	this->request->GetRequestObject()->SetField(TEXT("text"),		UStomtJsonValue::ConstructJsonValueString(	this, stomt->GetText()		));
	this->request->GetRequestObject()->SetField(TEXT("anonym"),		UStomtJsonValue::ConstructJsonValueBool(	this, stomt->GetAnonym()	));

	this->request->ProcessURL( this->GetRestURL().Append(TEXT("/stomts")) );
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

UStomtRestRequest * UStomtAPI::GetRequest()
{
	return this->request;
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

