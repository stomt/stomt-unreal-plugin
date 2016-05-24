// Fill out your copyright notice in the Description page of Project Settings.

#include "stomt.h"

#include "StomtWidget.h"

void UStomtWidget::onMessageChanged(FString text)
{
	if (!text.IsEmpty())
	{
		this->Message = text;
		UE_LOG(LogTemp, Warning, TEXT("MessageChanged %s") );
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, this->Message);
	}
	else
	{
		this->Message = FString(TEXT(""));
	}
	
}
