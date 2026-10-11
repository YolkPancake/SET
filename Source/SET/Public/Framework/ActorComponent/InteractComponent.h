
#pragma once

// #include "Framework/ActorComponent/InteractComponent.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SET_API UInteractComponent : public UActorComponent
{
	GENERATED_BODY() 

public:	
	UInteractComponent();

	void DoOnceInteract();


protected:
	virtual void BeginPlay() override;

	// 权威端真正执行交互:射线在这里打,避免客户端伪造目标
	void Authority_OnInteract(const FVector& Pos, const FVector& Dir);

	UFUNCTION(Server,Reliable)
	void Server_DoInteract(const FVector Pos, const FVector Dir);

		
protected:
	UPROPERTY(EditAnywhere)
	float TraceLength = 300.f;
};
