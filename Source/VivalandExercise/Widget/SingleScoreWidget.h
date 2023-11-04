// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SingleScoreWidget.generated.h"

/**
 * 
 */
UCLASS()
class VIVALANDEXERCISE_API USingleScoreWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// generate setters
	void SetPlace(int32 Place);
	void SetPlayerName(FString PlayerName);
	void SetScore(int32 Score);

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextPlace;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextPlayerName;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextScore;
	
	
};
