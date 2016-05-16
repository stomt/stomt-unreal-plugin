// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

#include "Blueprint/UserWidget.h"
#include "MyStomtWidget.generated.h"

/**
 * 
 */
UCLASS()
class STOMT_API UMyStomtWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My New User Widget")
		FString MyNewWidgetName;
	
	
};
