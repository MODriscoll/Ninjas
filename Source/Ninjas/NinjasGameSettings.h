#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "UObjectGlobals.h"
#include "GameFramework/MovementComponent.h"
#include "NinjasGameSettings.generated.h"

UCLASS()
class NINJAS_API UNinjasGameSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:

	UNinjasGameSettings();

	/** Default axis to constrain characters to */
	UPROPERTY(EditAnywhere, Category = "Character")
	EPlaneConstraintAxisSetting DefaultPlaneConstraintAxis;
};
