// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "struct/FOnlineSessionSearchWrapper.h"
#include "SASGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLoginCompleted, bool, bWasSuccessful, FString, ErrorMessage);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLogoutCompleted, bool, bWasSuccessful);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCreateEOSSessionCompleted, FName, SessionName, bool, bWasSuccessful);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFindSessionCompleted, bool, SearchResults);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSearchSessionEOSCompleted, bool, SearchResults);



/**
 * 
 */
UCLASS(Blueprintable)
class SWORDANDSHIELD_API USASGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	/*
	 * Authentication
	 */
	UFUNCTION(BlueprintCallable, Category = "Authentication")
	void Login();

	UFUNCTION(BlueprintCallable, Category = "Authentication")
	bool CheckLoginStatus();

	UFUNCTION(BlueprintCallable, Category = "Authentication")
	void Logout();

	// Delegates để Blueprints có thể bind vào
	UPROPERTY(BlueprintAssignable, Category = "Authentication")
	FOnLoginCompleted OnLoginCompletedEvent;
	
	UPROPERTY(BlueprintAssignable, Category = "Authentication")
	FOnLogoutCompleted OnLogoutCompletedEvent;

	/*
	 * Session
	 */
	UFUNCTION(BlueprintCallable, Category = "Session")
	void CreateSession(const FString& SessionName, FString LobbyName, int NumOfPlayer);

	UPROPERTY(BlueprintAssignable, Category = "Session")
	FOnCreateEOSSessionCompleted OnCreateEOSSessionCompleted;

	UFUNCTION(BlueprintCallable, Category="Session")
	void FindSession();

	UPROPERTY(BlueprintAssignable, Category="Session")
	FOnFindSessionCompleted OnFindSessionCompleted;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> MultiplayerLevel;

	FORCEINLINE FName GetSessionName() const { return  SessionNameKey; }

	FString GetSessionName(const FOnlineSessionSearchResult& SearchResult) const;

	UPROPERTY(BlueprintReadWrite, Category="Session")
	TArray<FOnlineSessionSearchEOSWrapper> SessionSearchEosWrappers;

	UPROPERTY(BlueprintAssignable, Category = "Session")
	FOnSearchSessionEOSCompleted OnSearchSessionEosCompleted;
protected:
	virtual void Init() override;

private:
	class IOnlineSubsystem* OnlineSubsystem;
	TSharedPtr<class IOnlineIdentity, ESPMode::ThreadSafe> Identity;
	TSharedPtr<class IOnlineSession, ESPMode::ThreadSafe> Session;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;
	const FName SessionNameKey{"SessionNameKey"};

	void OnLoginCompleted(signed int NumberOfPlayer,
		bool IsSuccessFul,
		const FUniqueNetId& UniqueNetId,
	                      const FString& ErrorMessage) const;
	void OnCreateSessionComplete(FName Name, bool IsSuccessFul) const;
	void OnLogoutCompleted(signed int I, bool IsSuccessFul) const;
	void OnFindSessionComplete(bool IsSuccessFul);
	void OnJoinSessionComplete(FName Name, EOnJoinSessionCompleteResult::Type Arg) const;
};
