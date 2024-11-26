// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "EOSSubSystemManager.generated.h"

class IOnlineSession;



DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCreateSessionCompleted, FName, SessionName, bool, bWasSuccessful);
/**
 * 
 */
UCLASS(Blueprintable)
class SWORDANDSHIELD_API UEOSSubSystemManager : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable, Category="EOS|Session")
	FOnCreateSessionCompleted OnCreateSessionCompleted;

	UFUNCTION(BlueprintCallable, Category="EOS|Session")
	void CreateEOSSession(FName SessionName, int32 maxPlayer);

private:
	class IOnlineSubsystem* OnlineSubsystem;
	TSharedPtr<class IOnlineSession, ESPMode::ThreadSafe> SessionPtr;
	
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessFul);
};
