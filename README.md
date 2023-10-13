# Gameplay Tags Table Generator
A simple helper plugin, that automatically updates gameplay tags in a `FGameplayTagTableRow` data table, based on row names in a source table when the source table is saved.
This mitigates the need to add update tags manually if you are using RowName as a tag.

Note: It does simple clear and regenerate, so any previous changes won't be saved and it won't do any automatic redirections. It will just generate tags 1:1 from source table.

# Setup
1. Create your `FGameplayTagTableRow` for gameplay tags
2. Setup pair Source and Gameplay Tags tables in project settings.

![image](https://github.com/Mahoukyou/GameplayTagsTableGenerator/assets/10081294/7ed696ec-c4a6-4781-9de2-b4012e106847)

# Preview
![Preview](https://github.com/Mahoukyou/GameplayTagsTableGenerator/assets/10081294/90d7522f-11f8-46bf-9069-ac62104cafa0)
