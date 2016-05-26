// Fill out your copyright notice in the Description page of Project Settings.

//#include "stomt.h"
#include "StomtPluginPrivatePCH.h"
#include "StomtPluginWidget.h"

void UStomtPluginWidget::OnMessageChanged(FString text)
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

void UStomtPluginWidget::ChangeButtonOrder(UButton *FirstButton, UButton *SecondButton)
{
	//FirstButton->GetSlots

}
