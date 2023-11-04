// Fill out your copyright notice in the Description page of Project Settings.


#include "SingleScoreWidget.h"

#include "Components/TextBlock.h"

void USingleScoreWidget::SetPlace(int32 Place)
{
	FString PlaceString = FString::FromInt(Place) + ".";
	TextPlace->SetText(FText::FromString(PlaceString));
}

void USingleScoreWidget::SetPlayerName(FString PlayerName)
{
	TextPlayerName->SetText(FText::FromString(PlayerName));
}

void USingleScoreWidget::SetScore(int32 Score)
{
	TextScore->SetText(FText::FromString(FString::FromInt(Score)));
}
