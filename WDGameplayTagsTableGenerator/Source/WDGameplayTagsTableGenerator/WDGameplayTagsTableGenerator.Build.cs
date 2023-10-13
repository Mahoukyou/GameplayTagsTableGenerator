using UnrealBuildTool;
public class WDGameplayTagsTableGenerator : ModuleRules
{
	public WDGameplayTagsTableGenerator(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		bUseUnity = false;

		PrivateDependencyModuleNames.AddRange(new string[] {
				"Core",
				"CoreUObject",
				"Engine",
				"UnrealEd",
				"GameplayTags",
				"DeveloperSettings"
			});
	}
}