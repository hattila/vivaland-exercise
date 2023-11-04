// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreboardWidget.h"

#include "SingleScoreWidget.h"
#include "Components/VerticalBox.h"
#include "UObject/ConstructorHelpers.h"

UScoreboardWidget::UScoreboardWidget(const FObjectInitializer& ObjectInitializer)
: UUserWidget(ObjectInitializer)
{
	// use the constructor helper to define the SingleScoreClass
	ConstructorHelpers::FClassFinder<UUserWidget> SingleScoreBPClass(TEXT("/Game/Widget/WBP_SingleScore"));
	if (!ensure(SingleScoreBPClass.Class != nullptr)) return;
	
	SingleScoreWidgetClass = SingleScoreBPClass.Class;
}

void UScoreboardWidget::Setup()
{
	AddToViewport();
}

void UScoreboardWidget::RefreshScoreboard(TArray<FPlayerScore> PlayerScores)
{
	ScoreList->ClearChildren();
	
	// iterate over PlayerScores
	int32 i = 1;
	for (FPlayerScore PlayerScore : PlayerScores)
	{
		// create a new SingleScore widget
		USingleScoreWidget* SingleScore = CreateWidget<USingleScoreWidget>(GetWorld(), SingleScoreWidgetClass);
		// add it to the ScoreList
		ScoreList->AddChild(SingleScore);
		// set the PlayerName and PlayerScore
		SingleScore->SetPlace(i++);
		SingleScore->SetPlayerName(PlayerScore.PlayerName);
		SingleScore->SetScore(PlayerScore.Score);
	}
}
