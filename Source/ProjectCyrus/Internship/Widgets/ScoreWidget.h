#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreWidget.generated.h"

UCLASS()

class PROJECTCYRUS_API UScoreWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	//Event that is called when the score is updated
	UFUNCTION(BlueprintImplementableEvent, Category = "Shooter", meta = (DisplayName = "UpdateScore"))
	void BP_UpdateScore(float Score);
	
};
