// Fill out your copyright notice in the Description page of Project Settings.

#include "stomt.h"
#include "StomtRestRequest.h"
#include "CoreMisc.h"



StomtRestRequest::StomtRestRequest()
{
}

StomtRestRequest::~StomtRestRequest()
{
}

void StomtRestRequest::MyHttpCall() 
{
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest(); // Gets an singelton and creates request.
	
	Request->SetURL("https://test.rest.stomt.com/stomts/java-sdk-test-33956");
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("appid"), "R18OFQXmb6QzXwzP1lWdiZ7Y9");
	Request->SetHeader("Content-Type", TEXT("application/json"));

	Request->OnProcessRequestComplete().BindUObject(this, &StomtRestRequest::OnResponseReceived);

	if (!Request->ProcessRequest()) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Internal error: ProcessRequest failed"));
	}
	
	return;
}

StomtRestRequest * StomtRestRequest::ConstructRequest()
{
	return nullptr;
}

void StomtRestRequest::SetVerb(SRequestVerb::Type Verb)
{
	this->RequestVerb = Verb;
}


void StomtRestRequest::SetHeader(const FString & HeaderName, const FString & HeaderValue)
{
	this->RequestHeaders.Add(HeaderName, HeaderValue);
}

void StomtRestRequest::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
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

///////////////////////////////////////////////////////////////////////////
// Response data access

int32 StomtRestRequest::GetResponseCode()
{
	return this->ResponseCode;
}

FString StomtRestRequest::GetResponseHeader(const FString HeaderName)
{
	FString Result;

	FString* Header = ResponseHeaders.Find(HeaderName);
	if (Header != NULL)
	{
		Result = *Header;
	}

	return Result;
}


TArray<FString> StomtRestRequest::GetAllResponseHeaders()
{
	TArray<FString> Result;

	for (TMap<FString, FString>::TConstIterator It(ResponseHeaders); It; ++It)
	{
		Result.Add(It.Key() + TEXT(": ") + It.Value());
	}

	return Result;
}

//////////////////////////////////////////////////////////////////////////
// URL processing

void StomtRestRequest::ProcessRequest(TSharedRef<IHttpRequest> HttpRequest)
{
	// Set verb
	switch (RequestVerb)
	{
	case SRequestVerb::GET:
		HttpRequest->SetVerb(TEXT("GET"));
		break;

	case SRequestVerb::POST:
		HttpRequest->SetVerb(TEXT("POST"));
		break;

	case SRequestVerb::PUT:
		HttpRequest->SetVerb(TEXT("PUT"));
		break;

	case SRequestVerb::DEL:
		HttpRequest->SetVerb(TEXT("DELETE"));
		break;

	default:
		break;
	}

	// Set content-type
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	// Serialize data to json string
	FString OutputString;
	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(RequestJsonObj.ToSharedRef(), Writer);

	// Set Json content
	HttpRequest->SetContentAsString(OutputString);


	// Apply additional headers
	for (TMap<FString, FString>::TConstIterator It(RequestHeaders); It; ++It)
	{
		HttpRequest->SetHeader(It.Key(), It.Value());
	}

	// Bind event
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &StomtRestRequest::OnProcessRequestComplete);

	// Execute the request
	HttpRequest->ProcessRequest();
	
}

//////////////////////////////////////////////////////////////////////////
// Request callbacks


void StomtRestRequest::OnProcessRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	// Be sure that we have no data from previous response
	ResetResponseData();

	// Check we have result to process futher
	if (!bWasSuccessful)
	{
		//UE_LOG(LogVaRest, Error, TEXT("Request failed: %s"), *Request->GetURL());

		// Broadcast the result event
		//OnRequestFail.Broadcast(this);

		return;
	}

	// Save response data as a string
	ResponseContent = Response->GetContentAsString();

	// Save response code as int32
	ResponseCode = Response->GetResponseCode();

	// Log response state
	//UE_LOG(LogVaRest, Log, TEXT("Response (%d): %s"), Response->GetResponseCode(), *Response->GetContentAsString());

	// Process response headers
	TArray<FString> Headers = Response->GetAllHeaders();
	for (FString Header : Headers)
	{
		FString Key;
		FString Value;
		if (Header.Split(TEXT(": "), &Key, &Value))
		{
			ResponseHeaders.Add(Key, Value);
		}
	}

	// Try to deserialize data to JSON
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(ResponseContent);
	FJsonSerializer::Deserialize(JsonReader, *ResponseJsonObj);

	// Decide whether the request was successful
	bIsValidJsonResponse = bWasSuccessful && ResponseJsonObj->IsValid();

	// Log errors
	if (!bIsValidJsonResponse)
	{
		if (!ResponseJsonObj->IsValid())
		{
			// As we assume it's recommended way to use current class, but not the only one,
			// it will be the warning instead of error
			//UE_LOG(LogVaRest, Warning, TEXT("JSON could not be decoded!"));
		}
	}

	// Broadcast the result event
	//OnRequestComplete.Broadcast(this);

	/*
	// Finish the latent action
	if (ContinueAction)
	{
		FVaRestLatentAction<UVaRestJsonObject*> *K = ContinueAction;
		ContinueAction = nullptr;

		K->Call(ResponseJsonObj);
	}
	*/
}

//////////////////////////////////////////////////////////////////////////
// Destruction and reset


void StomtRestRequest::ResetResponseData()
{
	if (ResponseJsonObj != NULL)
	{
		ResponseJsonObj->Reset();
	}
	else
	{
		ResponseJsonObj = new TSharedPtr<FJsonObject>();
	}

	ResponseHeaders.Empty();
	ResponseCode = -1;

	bIsValidJsonResponse = false;
}
