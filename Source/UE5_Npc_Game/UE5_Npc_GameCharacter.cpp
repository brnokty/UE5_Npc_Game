// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE5_Npc_GameCharacter.h"
#include "UE5_Npc_GameProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "InteractableInterface.h" // Include the interface header
#include "DrawDebugHelpers.h" // For debugging
#include "Blueprint/UserWidget.h" // Include for UUserWidget
#include "NPCCharacter.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AUE5_Npc_GameCharacter

AUE5_Npc_GameCharacter::AUE5_Npc_GameCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

}


//////////////////////////////////////////////////////////////////////////// Input

void AUE5_Npc_GameCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AUE5_Npc_GameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AUE5_Npc_GameCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AUE5_Npc_GameCharacter::Look);

		//EnhancedInputComponent->BindAction("Interact", IE_Pressed, this, &AUE5_Npc_GameCharacter::Interact);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AUE5_Npc_GameCharacter::Interact);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}

	
}


void AUE5_Npc_GameCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AUE5_Npc_GameCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();
		
	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

//Interact
// Interact
void AUE5_Npc_GameCharacter::Interact()
{

	if (InteractUIClass)
	{
		// UI widget'ını oluştur
		InteractUI = CreateWidget<UUserWidget>(GetWorld(), InteractUIClass);
		if (InteractUI)
		{
			InteractUI->AddToViewport();

			// PlayerController al ve null kontrolü yap
			APlayerController* PlayerController = Cast<APlayerController>(GetController());
			if (PlayerController)
			{
				// NPC ile etkileşime gir
				if (CurrentInteractableActor)
				{
					IInteractableInterface::Execute_Interact(CurrentInteractableActor, PlayerController);
					//CurrentInteractableActor->
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("CurrentInteractableActor null!"));
				}

				// Player hareketini devre dışı bırak
				DisableInput(PlayerController);

				// Fare imlecini göster
				PlayerController->bShowMouseCursor = true;
				PlayerController->SetInputMode(FInputModeUIOnly());
			}

			// Blueprint içindeki "NPC_Character" değişkenini bul
			FName VarName = FName(TEXT("NPC_Character"));
			FProperty* Property = InteractUI->GetClass()->FindPropertyByName(VarName);

			if (Property)
			{
				// NPC objesine erişim sağla
				UObject* NpcObject = Property->ContainerPtrToValuePtr<UObject>(InteractUI);

				if (NpcObject)
				{
					// NPC objesini cast et
					ANPCCharacter* NpcChar = Cast<ANPCCharacter>(CurrentInteractableActor);
					if (NpcChar)
					{
						// NPC referansını Blueprint değişkenine ata
						*reinterpret_cast<ANPCCharacter**>(NpcObject) = NpcChar;
						UE_LOG(LogTemp, Warning, TEXT("NpcCharacter başarıyla Widget'a atandı: %s"), *NpcChar->GetName());
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("CurrentInteractableActor, ANPCCharacter değil!"));
					}
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Blueprint içindeki NPC_Character değişkenine erişilemedi!"));
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Blueprint içinde 'NPC_Character' değişkeni bulunamadı!"));
			}
		}
	}
}

void AUE5_Npc_GameCharacter::EnableInputAgain()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	EnableInput(PlayerController);
	PlayerController->bShowMouseCursor = false;
        PlayerController->SetInputMode(FInputModeGameOnly());
}

void AUE5_Npc_GameCharacter::SetCurrentInteractableActor(AActor* Actor)
{
	CurrentInteractableActor = Actor;
}