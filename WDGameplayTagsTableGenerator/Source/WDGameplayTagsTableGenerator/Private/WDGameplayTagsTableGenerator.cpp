#include "WDGameplayTagsTableGenerator.h"
#include "WDGameplayTagsTableGeneratorSettings.h"

#include "DataTableEditorUtils.h"
#include "FileHelpers.h"
#include "GameplayTagsManager.h"
#include "Engine/DataTable.h"

void FWDGameplayTagsTableGeneratorModule::StartupModule()
{
	OnObjectPreSave_DelegateHandle = FCoreUObjectDelegates::OnObjectPreSave.AddRaw(this, &FWDGameplayTagsTableGeneratorModule::OnDataTableChanged);
}

void FWDGameplayTagsTableGeneratorModule::ShutdownModule()
{
	FCoreUObjectDelegates::OnObjectPreSave.Remove(OnObjectPreSave_DelegateHandle);
	OnObjectPreSave_DelegateHandle = FDelegateHandle();
}

void FWDGameplayTagsTableGeneratorModule::OnDataTableChanged(UObject* const InObjectSaved, const FObjectPreSaveContext InSaveContext)
{
	if (InSaveContext.IsCooking())
	{
		return;
	}

	if (!InObjectSaved || InObjectSaved->GetClass() != UDataTable::StaticClass())
	{
		return;
	}

	const FWDGameplayTagsTableInfo* const GameplayTagsTables = FindDataTablePair(TSoftObjectPtr<UDataTable>(InObjectSaved));
	if (!GameplayTagsTables)
	{
		return;
	}

	const UDataTable* const SourceDataTable = GameplayTagsTables->SourceDataTableSoft.Get();
	UDataTable* const GameplayTagsDataTable = GameplayTagsTables->GameplayTagsDataTableSoft.LoadSynchronous();

	if (!SourceDataTable || !GameplayTagsDataTable || SourceDataTable == GameplayTagsDataTable || SourceDataTable != InObjectSaved)
	{
		return;
	}

	if (GameplayTagsDataTable->GetRowStruct() != FGameplayTagTableRow::StaticStruct())
	{
		return;
	}

	// Needed to update data table editor if open, as it needs to handle the change (otherwise crash)
	FDataTableEditorUtils::BroadcastPreChange(GameplayTagsDataTable, FDataTableEditorUtils::EDataTableChangeInfo::RowList);

	GameplayTagsDataTable->EmptyTable();
	for (const FName& RowName : SourceDataTable->GetRowNames())
	{
		FGameplayTagTableRow NewRow;
		NewRow.Tag = RowName;

		GameplayTagsDataTable->AddRow(RowName, NewRow);
	}

	GameplayTagsDataTable->HandleDataTableChanged(NAME_None);
	GameplayTagsDataTable->MarkPackageDirty();

	FDataTableEditorUtils::BroadcastPostChange(GameplayTagsDataTable, FDataTableEditorUtils::EDataTableChangeInfo::RowList);

	const TArray<UPackage*> PackagesToSave{ GameplayTagsDataTable->GetPackage() };
	UEditorLoadingAndSavingUtils::SavePackages(PackagesToSave, true);
}

const FWDGameplayTagsTableInfo* FWDGameplayTagsTableGeneratorModule::FindDataTablePair(const TSoftObjectPtr<UDataTable>& InSourceDataTable)
{
	const TArray<FWDGameplayTagsTableInfo>& TablesList = UWDGameplayTagsTableGeneratorSettings::Get().GameplayTagsTables;

	return TablesList.FindByPredicate([InSourceDataTable](const FWDGameplayTagsTableInfo& InTablePair) {
		return InTablePair.SourceDataTableSoft == InSourceDataTable;
	});
}

IMPLEMENT_MODULE(FWDGameplayTagsTableGeneratorModule, WDGameplayTagsTableGenerator)