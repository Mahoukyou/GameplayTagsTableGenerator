#pragma once

#include "Engine/DeveloperSettings.h"
#include "WDGameplayTagsTableGeneratorSettings.generated.h"

class UDataTable;

USTRUCT(BlueprintType)
struct FWDGameplayTagsTableInfo
{
	GENERATED_BODY();

public:
	UPROPERTY(Config, EditAnywhere, DisplayName = "Source Data Table")
	TSoftObjectPtr<UDataTable> SourceDataTableSoft = nullptr;

	UPROPERTY(Config, EditAnywhere, DisplayName = "Gameplay Tags Data Table") // todo, constrain to specific table
	TSoftObjectPtr<UDataTable> GameplayTagsDataTableSoft = nullptr;
};

UCLASS(config = Editor, defaultconfig, meta = (DisplayName = "Gameplay Tags Table Generator Plugin"))
class UWDGameplayTagsTableGeneratorSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	static const UWDGameplayTagsTableGeneratorSettings& Get();

	UPROPERTY(Config, EditAnywhere, Category = "Data Tables")
	TArray<FWDGameplayTagsTableInfo> GameplayTagsTables;
};