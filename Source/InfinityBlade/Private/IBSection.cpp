// Fill out your copyright notice in the Description page of Project Settings.

#include "IBSection.h"
#include "IBCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AIBSection::AIBSection()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	//OverlapVolum = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapVolume"));
	//RootComponent = OverlapVolum;
	//OverlapVolum->OnComponentBeginOverlap.AddUniqueDynamic(this, &AIBSection::OverlapBegins);
	//OverlapVolum->OnComponentEndOverlap.AddUniqueDynamic(this, &AIBSection::OverlapEnds);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	RootComponent = Mesh;

	FString AssetPath = TEXT("/Game/Book/SM_SQUARE.SM_SQUARE");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_SQUARE(*AssetPath);
	if (SM_SQUARE.Succeeded())
	{
		Mesh->SetStaticMesh(SM_SQUARE.Object);
	}
	else
	{
		ABLOG(Error, TEXT("Failed"));
	}

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Trigger->SetupAttachment(RootComponent);
	Trigger->SetBoxExtent(FVector(775.0f, 775.0f, 300.0f));
	Trigger->SetRelativeLocation(FVector(0.0f, 0.0f, 250.0f));
	Trigger->SetCollisionProfileName(TEXT("IBTrigger"));
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AIBSection::OnTriggerBeginOverlap);


	FString GateAssetPath = TEXT("/Game/Book/SM_GATE.SM_GATE");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_GATE(*GateAssetPath);
	if (!SM_GATE.Succeeded())
	{
		ABLOG(Error, TEXT("Failed"));
	}
	//+X=보스방, +Y=엘븐, -X=아이스, -Y=파이어
	static FName GateSockets[] = { {TEXT("+XGate")}, {TEXT("-XGate")}, {TEXT("+YGate")}, {TEXT("-YGate")} };
	for (FName GateSocket : GateSockets)
	{
		ABCHECK(Mesh->DoesSocketExist(GateSocket));
		UStaticMeshComponent* NewGate = CreateDefaultSubobject<UStaticMeshComponent>(*GateSocket.ToString());
		NewGate->SetStaticMesh(SM_GATE.Object);
		NewGate->SetupAttachment(RootComponent, GateSocket);
		NewGate->SetRelativeLocation(FVector(0.0f, -80.5f, 0.0f));
		GateMeshs.Add(NewGate);

		UBoxComponent* NewGateTrigger = CreateDefaultSubobject<UBoxComponent>(*GateSocket.ToString().Append(TEXT("Trigger")));
		NewGateTrigger->SetBoxExtent(FVector(100.0f, 100.0f, 300.0f));
		NewGateTrigger->SetupAttachment(RootComponent, GateSocket);
		NewGateTrigger->SetRelativeLocation(FVector(70.0f, 0.0f, 250.0f));
		NewGateTrigger->SetCollisionProfileName(TEXT("IBTrigger"));
		GateTriggers.Add(NewGateTrigger);
		NewGateTrigger->OnComponentBeginOverlap.AddDynamic(this, &AIBSection::OnGateTriggerBeginOverlap);
		NewGateTrigger->ComponentTags.Add(GateSocket);
	}

	bNoBattle = false;
}

// Called when the game starts or when spawned
void AIBSection::BeginPlay()
{
	Super::BeginPlay();
	SetState(bNoBattle ? ESectionState::COMPLETE : ESectionState::READY);
}

void AIBSection::SetState(ESectionState NewState)
{
	CurrentState = NewState;
	switch (CurrentState)
	{
	case AIBSection::ESectionState::READY:
		Trigger->SetCollisionProfileName(TEXT("IBTrigger"));
		for (UBoxComponent* GateTrigger : GateTriggers)
		{
			GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
		}

		OperateGates(true);
		break;
	case AIBSection::ESectionState::BATTLE:
		Trigger->SetCollisionProfileName(TEXT("NoCollision"));
		for (UBoxComponent* GateTrigger : GateTriggers)
		{
			GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
		}
		OperateGates(false);
		break;
	case AIBSection::ESectionState::COMPLETE:
		Trigger->SetCollisionProfileName(TEXT("NoCollision"));
		for (UBoxComponent* GateTrigger : GateTriggers)
		{
			GateTrigger->SetCollisionProfileName(TEXT("IBTrigger"));
		}
		OperateGates(true);
		break;
	}
}

void AIBSection::OperateGates(bool bOpen)
{
	for (UStaticMeshComponent* Gate : GateMeshs)
	{
		Gate->SetRelativeRotation(bOpen ? FRotator(0.0f, -90.0f, 0.0f) : FRotator::ZeroRotator);
	}
}

void AIBSection::OnTriggerBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (CurrentState == ESectionState::READY)
	{
		SetState(ESectionState::BATTLE);
	}
}

void AIBSection::OnGateTriggerBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	ABCHECK(OverlappedComponent->ComponentTags.Num() == 1);
	FName ComponentTag = OverlappedComponent->ComponentTags[0];
	FName SocketName = FName(*ComponentTag.ToString());
	ABLOG(Warning, TEXT("%s"), *ComponentTag.ToString().Left(2));
	if (!Mesh->DoesSocketExist(SocketName))
		return;

	//+X=보스방, -X=아이스, +Y=엘븐,, -Y=파이어
	static FName GateSockets[] = { {TEXT("+XGate")}, {TEXT("-XGate")}, {TEXT("+YGate")}, {TEXT("-YGate")} };
	if (SocketName == GateSockets[1])
	{
		FName LevelToLoad = TEXT("FrozenCove");
		FLatentActionInfo LatentInfo;
		UGameplayStatics::LoadStreamLevel(this, LevelToLoad, true, true, LatentInfo);
		SetState(ESectionState::BATTLE);
	}
	else if (SocketName == GateSockets[2])
	{
		FName LevelToLoad = TEXT("ElvenRuins");
		FLatentActionInfo LatentInfo;
		UGameplayStatics::LoadStreamLevel(this, LevelToLoad, true, true, LatentInfo);
		SetState(ESectionState::BATTLE);
	}
	else if (SocketName == GateSockets[3])
	{
		FName LevelToLoad = TEXT("Forge");
		FLatentActionInfo LatentInfo;
		UGameplayStatics::LoadStreamLevel(this, LevelToLoad, true, true, LatentInfo);
		//SetState(ESectionState::BATTLE);
	}
	else if (SocketName == GateSockets[0])
	{
		FName LevelToLoad = TEXT("BossStage");
		FLatentActionInfo LatentInfo;
		UGameplayStatics::LoadStreamLevel(this, LevelToLoad, true, true, LatentInfo);
		SetState(ESectionState::BATTLE);
	}

	/*FVector NewLocation = Mesh->GetSocketLocation(SocketName);

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, this);
	FCollisionObjectQueryParams ObjectQueryParam(FCollisionObjectQueryParams::InitType::AllObjects);
	bool bResult = GetWorld()->OverlapMultiByObjectType(
		OverlapResults,
		NewLocation,
		FQuat::Identity,
		ObjectQueryParam,
		FCollisionShape::MakeSphere(775.0f),
		CollisionQueryParam
	);

	if (!bResult)
	{
		auto NewSection = GetWorld()->SpawnActor<AIBSection>(NewLocation, FRotator::ZeroRotator);
	}
	else
	{
		ABLOG(Warning, TEXT("not Empty"));
	}*/
}

// Called every frame
void AIBSection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//void AIBSection::OverlapBegins(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
//{
//
//	ACharacter* MyCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
//
//	if (OtherActor == MyCharacter && LevelToLoad != "")
//	{
//		FLatentActionInfo LatentInfo;
//		UGameplayStatics::LoadStreamLevel(this, LevelToLoad, true, true, LatentInfo);
//
//	}
//
//}
//
//void AIBSection::OverlapEnds(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
//{
//	ACharacter* MyCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
//	if (OtherActor == MyCharacter && LevelToLoad != "")
//	{
//		ABLOG(Warning, TEXT("LevelToLoad UnLoad"));
//		FLatentActionInfo LatentInfo;
//		UGameplayStatics::UnloadStreamLevel(this, LevelToLoad, LatentInfo);
//	}
//}

