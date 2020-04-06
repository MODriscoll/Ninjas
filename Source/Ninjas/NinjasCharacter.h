#pragma once

#include "CoreMinimal.h"
#include "BaseNinjaCharacter.h"
#include "NinjasCharacter.generated.h"

UCLASS(config=Game)
class ANinjasCharacter : public ABaseNinjaCharacter
{
	GENERATED_BODY()

	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Spawner for when throwing a projectile */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapons, meta =(AllowPrivateAccess = "true"))
	class UProjectileSpawnerComponent* ProjectileSpawner;

protected:

	/** Called for side to side input */
	void MoveRight(float Val);

	/** Handle touch inputs. */
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	/** Handle touch stop event. */
	void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface


public:
	ANinjasCharacter();

	/** Returns SideViewCameraComponent subobject **/
	class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns ProjectileSpawner subobject */
	class UProjectileSpawnerComponent* GetProjectileSpawner() const { return ProjectileSpawner; }
};
