#include "WDGameplayTagsTableGenerator.h"
#include "WDGameplayTagsTableGeneratorSettings.h"

#include "DataTableEditorUtils.h"
#include "FileHelpers.h"
#include "GameplayTagsManager.h"
#include "Engine/DataTable.h"

// TODO: Logs
// TODO: CDT Support as alternative to array of tables

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

	UDataTable* const GameplayTagsDataTable = GameplayTagsTables->GameplayTagsDataTableSoft.LoadSynchronous();
	if (GameplayTagsDataTable->GetRowStruct() != FGameplayTagTableRow::StaticStruct())
	{
		return;
	}

	// Needed to update data table editor if open, as it needs to handle the change (otherwise crash)
	FDataTableEditorUtils::BroadcastPreChange(GameplayTagsDataTable, FDataTableEditorUtils::EDataTableChangeInfo::RowList);

	TSet<FName> RowsToAdd;
	for (const TSoftObjectPtr<UDataTable> SourceDataTableSoft : GameplayTagsTables->SourceDataTablesSoft)
	{
		const UDataTable* const SourceDataTable = SourceDataTableSoft.LoadSynchronous();
		if (SourceDataTable == GameplayTagsDataTable)
		{
			continue;
		}

		// Push into set first, so we can sort them across all source tables
		RowsToAdd.Append(SourceDataTable->GetRowNames());
	}

	RowsToAdd.StableSort([](const FName& InLHS, const FName& InRHS) { return InLHS.LexicalLess(InRHS); });
	GameplayTagsDataTable->EmptyTable();
	for (const FName& RowName : RowsToAdd)
	{
		FGameplayTagTableRow NewRow;
		NewRow.Tag = RowName;

		GameplayTagsDataTable->AddRow(RowName, NewRow);
	}

	GameplayTagsDataTable->HandleDataTableChanged(NAME_None);
	(void) GameplayTagsDataTable->MarkPackageDirty();

	FDataTableEditorUtils::BroadcastPostChange(GameplayTagsDataTable, FDataTableEditorUtils::EDataTableChangeInfo::RowList);

	const TArray<UPackage*> PackagesToSave{ GameplayTagsDataTable->GetPackage() };
	UEditorLoadingAndSavingUtils::SavePackages(PackagesToSave, true);
}

const FWDGameplayTagsTableInfo* FWDGameplayTagsTableGeneratorModule::FindDataTablePair(const TSoftObjectPtr<UDataTable>& InSourceDataTable)
{
	const TArray<FWDGameplayTagsTableInfo>& TablesList = UWDGameplayTagsTableGeneratorSettings::Get().GameplayTagsTables;

	return TablesList.FindByPredicate([InSourceDataTable](const FWDGameplayTagsTableInfo& InTablePair) {
		return InTablePair.SourceDataTablesSoft.Contains(InSourceDataTable);
	});
}

IMPLEMENT_MODULE(FWDGameplayTagsTableGeneratorModule, WDGameplayTagsTableGenerator)