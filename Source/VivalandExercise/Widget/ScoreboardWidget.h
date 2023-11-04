// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VivalandExercise/PlayerScore.h"
#include "ScoreboardWidget.generated.h"

/**
 * 
 */
UCLASS()
class VIVALANDEXERCISE_API UScoreboardWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UScoreboardWidget(const FObjectInitializer& ObjectInitializer);

	void Setup();
	
	void RefreshScoreboard(TArray<FPlayerScore> PlayerScores);
	
private:
	TSubclassOf<class UUserWidget> SingleScoreWidgetClass;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* ScoreList;
};
