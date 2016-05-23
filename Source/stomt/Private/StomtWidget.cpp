// Fill out your copyright notice in the Description page of Project Settings.

#include "stomt.h"
#include "StomtWidget.h"

void UStomtWidget::onMessageChanged()
{
	UE_LOG(LogTemp, Warning, TEXT("MessageChanged %s"), *this->Message );
}
