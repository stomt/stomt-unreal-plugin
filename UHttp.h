// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Http.h"


/**
 * 
 */

class STOMT_API UHttp
{
public:
	UHttp();
	~UHttp();

	void MyHttpCall();

	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};
