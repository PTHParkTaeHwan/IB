// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InfinityBlade.h"
#include "GameFramework/Actor.h"
#include "IBSection.generated.h"

UCLASS()
class INFINITYBLADE_API AIBSection : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AIBSection();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	enum class ESectionState : uint8
	{
		READY = 0,
		BATTLE,
		COMPLETE
	};

	void SetState(ESectionState NewState);
	ESectionState CurrentState = ESectionState::READY;

	void OperateGates(bool bOpen = true);

	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, Category = Mesh, Meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, Category = Mesh, Meta = (AllowPrivateAccess = true))
	TArray<UStaticMeshComponent*> GateMeshs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Trigger, Meta = (AllowPrivateAccess = true))
	UBoxComponent* Trigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Trigger, Meta = (AllowPrivateAccess = true))
	TArray<UBoxComponent*> GateTriggers;

	UPROPERTY(EditAnywhere, Category = State, Meta = (AllowPrivateAccess = true))
	bool bNoBattle;

private:

	//UFUNCTION()
	//void OverlapEnds(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//UPROPERTY(EditAnywhere)
	//FName LevelToLoad;

	//UPROPERTY(EditAnywhere)
	//FName LevelToUnLoad;

};
