// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Http.h"


/**
 * 
 */

class STOMT_API StomtRestRequest
{
public:
	StomtRestRequest();
	~StomtRestRequest();

	void MyHttpCall();

	static StomtRestRequest* ConstructRequest();

	/** Set verb to the request */
	void SetVerb(SRequestVerb::Type Verb);

	/** Sets header info */
	void SetHeader(const FString& HeaderName, const FString& HeaderValue);

	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};

/** Verb (GET, PUT, POST) */
UENUM(BlueprintType)
namespace SRequestVerb
{
	enum Type
	{
		GET,
		POST,
		PUT,
		DEL UMETA(DisplayName = "DELETE"),
	};
}
