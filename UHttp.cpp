// Fill out your copyright notice in the Description page of Project Settings.

#include "stomt.h"
#include "UHttp.h"


UHttp::UHttp()
{
}

UHttp::~UHttp()
{
}

void UHttp::MyHttpCall() 
{
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest(); // Gets an Singelton and creates Request.
	
	Request->SetURL("https://test.rest.stomt.com/stomts/java-sdk-test-33956");
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("appid"), "R18OFQXmb6QzXwzP1lWdiZ7Y9");
	Request->SetHeader("Content-Type", TEXT("application/json"));

	Request->OnProcessRequestComplete().BindRaw(this, &UHttp::OnResponseReceived);

	if (!Request->ProcessRequest()) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Internal error: ProcessRequest failed"));
	}
	
	return;
}


void UHttp::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) 
{
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode())) 
	{
		UE_LOG(LogTemp, Warning, TEXT("EHttpResponseCodes::IsOk"));
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT( "not successful: %s"), *Response->GetContentAsString() );
	}

	return;
}

