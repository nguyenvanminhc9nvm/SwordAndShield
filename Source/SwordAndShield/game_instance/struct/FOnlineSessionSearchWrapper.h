#pragma once

#include "CoreMinimal.h"
#include "FOnlineSessionSearchWrapper.generated.h"

USTRUCT(BlueprintType)
struct FOnlineSessionSearchEOSWrapper
{

	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Session")
	FString ServerName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Session")
	int32 CurrentPlayers;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Session")
	int32 MaxPlayers;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Session")
	int32 Ping;

	FOnlineSessionSearchEOSWrapper()
		: ServerName(TEXT("DefaultServer")),
		  CurrentPlayers(0),
		  MaxPlayers(0),
		  Ping(0)
	{
	}
	
	FOnlineSessionSearchEOSWrapper(const FString& ServerName, int32 CurrentPlayers, int32 MaxPlayers, int32 Ping)
		: ServerName(ServerName),
		  CurrentPlayers(CurrentPlayers),
		  MaxPlayers(MaxPlayers),
		  Ping(Ping)
	{
	}
};
