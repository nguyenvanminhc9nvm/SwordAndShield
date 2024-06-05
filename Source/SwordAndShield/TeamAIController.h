// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TeamAIController.generated.h"

/**
 * 
 */
UCLASS()
class SWORDANDSHIELD_API ATeamAIController : public AAIController
{
	GENERATED_BODY()
public:
	ATeamAIController();
    
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	virtual FGenericTeamId GetGenericTeamId() const override;
    
private:
	UPROPERTY(EditDefaultsOnly)
	FGenericTeamId TeamId = FGenericTeamId(0);
};
