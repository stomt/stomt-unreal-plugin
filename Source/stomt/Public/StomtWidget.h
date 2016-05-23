// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

#include "Blueprint/UserWidget.h"
#include "StomtWidget.generated.h"

/**
 * 
 */
UCLASS()
class STOMT_API UStomtWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stomt Widget")
	FString Message;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stomt Widget")
	FString TargetName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stomt Widget")
	bool IsWish;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stomt Widget")
	bool IsScreenshotSelected;

	UFUNCTION(BlueprintCallable, Category = "Stomt Widget")
	void onMessageChanged();
	
	
	
};
