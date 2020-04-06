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

// TODO: Add a fire handler (so spawn projectile can't be spammed).
// TODO: Add a feature to enable a timer that works based of fire rate (this is useful for testing)
};
