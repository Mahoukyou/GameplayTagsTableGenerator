#include "WDGameplayTagsTableGeneratorSettings.h"

const UWDGameplayTagsTableGeneratorSettings& UWDGameplayTagsTableGeneratorSettings::Get()
{
	return *CastChecked<UWDGameplayTagsTableGeneratorSettings>(StaticClass()->GetDefaultObject());
}