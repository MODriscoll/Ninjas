// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

/** This component does not define a root component */
UCLASS()
class NINJAS_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AProjectileBase();

public:

	// Begin AActor Interface
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual bool IsComponentRelevantForNavigation(UActorComponent* Component) const override;
	// End AActor Interface

	/** Get projectile movement */
	class UProjectileMovementComponent* GetProjectileMovement() const { return MovementComponent; }

protected:

	/** Checks if collision with actor should be ignored */
	bool ShouldIgnoreCollisionWith(AActor* OtherActor) const;

	/** Event called when overlapped an actor (bReportOverlaps is true) */
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="On Actor Overlapped"))
	void BP_OnActorOverlapped(AActor* OverlappedActor);

	/** Event called when hit an actor (bReportHits is true) */
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="On Actor Hit"))
	void BP_OnActorHit(AActor* HitActor, const FVector& NormalImpulse, const FHitResult& Hit);

	/** Notify that this projectile overlaps another actor */
	UFUNCTION()
	virtual void OnOverlap(AActor* SelfActor, AActor* OtherActor);

	/** Notify that this projectile has hit another actor */
	UFUNCTION()
	virtual void OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

public:

	/* If this projectile should respond to overlap events */
	UPROPERTY(EditDefaultsOnly)
	uint8 bReportOverlaps : 1;

	/** If this projectile should respond to hit events */
	UPROPERTY(EditDefaultsOnly)
	uint8 bReportHits : 1;

	/** If we should ignore any collision events with our instigator */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	uint8 bIgnoreInstigator : 1;

private:

	/** Component responsible for components movement */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess="true"))
	class UProjectileMovementComponent* MovementComponent;
};
