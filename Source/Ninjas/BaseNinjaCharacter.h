// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseNinjaCharacter.generated.h"

class UHealthComponent;

/**
 * Base for the characters the player will control and fight
 */
UCLASS()
class NINJAS_API ABaseNinjaCharacter : public ACharacter
{
	GENERATED_BODY()
	
public:

	ABaseNinjaCharacter();

	UHealthComponent* GetHealthComponent() const { return HealthComponent; }

public:
	
	/** Set the state of this characters ragdoll */
	UFUNCTION(BlueprintCallable, Category = "Character")
	void SetRagdollEnabled(bool bEnable);

	/** Puts this character into a ragdoll state */
	UFUNCTION(BlueprintCallable, Category = "Character")
	virtual void EnterRagdoll();

	/** Puts this character out of ragdoll state */
	UFUNCTION(BlueprintCallable, Category = "Character")
	virtual void ExitRagdoll();

	/** If this character is in a ragdoll state */
	UFUNCTION(BlueprintPure, Category = "Character")
	virtual bool IsRagdolling() const;

private:

	/** Event called when this character dies */
	UFUNCTION()
	virtual void OnDeath(UHealthComponent* HealthComp);

	/** This ninjas health, is used to determine which Ninja is alive or not */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess="true"))
	UHealthComponent* HealthComponent;

	/** If this character is currently in ragdoll state */
	bool bIsRagdoll;
};
