#pragma once

#include "Ninjas.h"
#include "Components/ActorComponent.h"
#include "ProjectileSpawnerComponent.generated.h"

class AProjectileBase;

/**
 * This component can be used to spawn a projectile. If 
 */
UCLASS(ClassGroup=(Ninjas), meta=(BlueprintSpawnableComponent))
class NINJAS_API UProjectileSpawnerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UProjectileSpawnerComponent();

	// Begin UObject Interface
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	// End UObject Interface

	// Begin UActorComponent Interface
	virtual void BeginPlay() override;
	// End UActorComponent Interface

	/** Spawns a new projectile */
	UFUNCTION(BlueprintCallable, Category = "Spawner")
	AProjectileBase* SpawnProjectile();

	/** Get the component used as source for spawning projectiles.
	Will attempt to find it if it hasn't been cached already */
	UFUNCTION(BlueprintPure, Category = "Spawner")
	class USceneComponent* GetSpawnSourceComponent();

	/** Get the component used as source for spawning projectiles */
	class USceneComponent* GetSpawnSourceComponent() const;

	/** Resets the cached source spawn component */
	UFUNCTION(BlueprintCallable, Category = "Spawner")
	USceneComponent* ResetSpawnSourceComponent();

	/** If a new projectile can be spawned */
	UFUNCTION(BlueprintPure, Category = "Spawner")
	virtual bool CanSpawnProjectile() const;

public:

	/** The projectile to spawn */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf< AProjectileBase> ProjectileClass;

	/** The component to use as the source for a projectile spawn.
	This should be a scene component attached to this actor */
	UPROPERTY(EditDefaultsOnly)
	FComponentReference SpawnSourceComponent;

	/** The socket to shoot from if SpawnSourceComponent has it */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName SpawnSourceSocket;

private:

	/** Tries to get the spawn source component from set reference */
	class USceneComponent* GetSpawnSourceComponentImpl() const;

	/** Cached reference to spawn source component */
	TWeakObjectPtr<class USceneComponent> CachedSpawnSource;

public:

	/** Set this spawner to automatically start spawning projectiles */
	UFUNCTION(BlueprintCallable, Category = "Spawner")
	void SetAutomaticSpawning(bool bEnable);

protected:

	/** If this spawner is currently set to automatically spawn */
	bool IsAutomaticallySpawning() const;

	/** The delay to use to prevent projectiles from begin spammed.
	Can be set to zero to allow for no delay between spawns */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin="0"))
	float SpawnDelay;

	/** Should this spawner start as automatically spawning projectiles? */
	UPROPERTY(EditAnywhere, AdvancedDisplay)
	bool bStartWithAutomaticSpawning;

private:

	/** Implementation for handling setting auto spawning */
	void SetAutomaticSpawningImpl(bool bEnable);

	/** Tick from timer manager to spawn another projectile */
	UFUNCTION()
	void OnAutomaticSpawnTick();

	/** The last time a projectile was spawned */
	float LastSpawnTime;

	/** Timer handle that manages automatic spawning */
	FTimerHandle Handle_AutomaticSpawning;
};
