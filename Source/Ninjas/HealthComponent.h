// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHealthChangedSignature, class UHealthComponent*, Component, float, OldHealth, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeathSignature, class UHealthComponent*, DeadComponent);

/**
 * This component tracks the health of an actor
 */
UCLASS(ClassGroup=(Ninjas), meta=(BlueprintSpawnableComponent))
class NINJAS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UHealthComponent();

	/** Apply damage to this actor. Get the amount of damage dealt */
	UFUNCTION(BlueprintCallable, Category = "Health")
	float ApplyDamage(float Damage);

	/** Heal this actor. Get the amount of health restored */
	UFUNCTION(BlueprintCallable, Category = "Health")
	float Heal(float Amount);

	/** If actor is considered 'dead'. By default, returns if health is at zero */
	UFUNCTION(BlueprintPure, Category = "Health")
	virtual bool IsDead() const;

	/** If this actor can be damaged. By default, returns bCanBeDamaged */
	UFUNCTION(BlueprintPure, Category = "Damage")
	virtual bool CanBeDamaged() const;

	/** Kill this actor if still alive */
	UFUNCTION(BlueprintCallable, Category = "Damage")
	void Kill();

protected:

	// Begin UObject Interface
	virtual void PostInitProperties() override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	// End UObject Interface

public:

	/** Event called when health has changed, called before OnDeath */
	UPROPERTY(BlueprintAssignable)
	FOnHealthChangedSignature OnHealthChanged;

	/** Event called when health has been depleted */
	UPROPERTY(BlueprintAssignable)
	FOnDeathSignature OnDeath;

protected:

	/** How much health we have */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	float Health;

	/** Max amount of health we can have at once */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	float MaxHealth;

	/** If we can be damaged (or if we are invincible) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	bool bCanBeDamaged;

private:

	float ApplyDelta(float Delta);
};
