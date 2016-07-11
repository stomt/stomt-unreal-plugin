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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stomt Widget Plugin")
	FString Message;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stomt Widget Plugin")
	FString TargetName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stomt Widget Plugin")
	FString	ImageURL;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stomt Widget Plugin")
	//UStomtRestRequest Request;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stomt Widget Plugin")
	bool IsWish;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stomt Widget Plugin")
	bool IsScreenshotSelected;

	UFUNCTION(BlueprintCallable, Category = "Stomt Widget Plugin")
	void OnMessageChanged(FString text);

	UFUNCTION(BlueprintCallable, Category = "Stomt Widget Plugin")
	void OnSubmit();

	UFUNCTION(BlueprintCallable, Category = "Stomt Widget Plugin")
	void OnConstruction(FString targetID);

	UFUNCTION()
	void OnReceiving(UStomtRestRequest* Request);

	UFUNCTION(BlueprintCallable, Category = "Stomt Widget Plugin")
	void ChangeButtonOrder(UButton *FirstButton, UButton *SecondButton);

private:

	UStomt* stomt;
	UStomtAPI* api;
	
	
	
};
