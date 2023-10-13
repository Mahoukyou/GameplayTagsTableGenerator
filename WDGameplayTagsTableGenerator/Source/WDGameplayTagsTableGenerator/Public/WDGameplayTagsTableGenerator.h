#pragma once

#include "UObject/ObjectSaveContext.h"
#include "Modules/ModuleManager.h"

struct FWDGameplayTagsTableInfo;
class UDataTable;

class FWDGameplayTagsTableGeneratorModule : public IModuleInterface
{

public:
	// *** IModuleInterface implementation ***
	void StartupModule() override;
	void ShutdownModule() override;
	// ***

protected:
	void OnDataTableChanged(UObject* InObjectSaved, FObjectPreSaveContext InSaveContext);
	static const FWDGameplayTagsTableInfo* FindDataTablePair(const TSoftObjectPtr<UDataTable>& InSourceDataTable);

	FDelegateHandle OnObjectPreSave_DelegateHandle;
};