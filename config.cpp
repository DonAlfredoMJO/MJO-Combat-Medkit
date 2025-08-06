class CfgPatches
{
	class MJO_MedicKit
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","DZ_Scripts","DZ_Gear_Camping"};
	};
};
class CfgMods
{
	class MJO_MedicKit /* this classname can be used as the "#ifdef MJO_MedicKit" / "#ifndef MJO_MedicKit" */
	{
		dir = "MJO_MedicKit";
		picture = "";
		action = "";
		hideName = 1;
		hidePicture = 1;
		name = "MJO_MedicKit";
		credits = "Don Alfredo";
		author = "Don Alfredo";
		authorID = "";
		version = "1";
		extra = 0;
		type = "mod";
		dependencies[] = {"World"};
		class defs
		{
			class worldScriptModule
			{
				value = "";
				files[] = {"MJO_MedicBag_source/scripts/4_World"};
			};
		};
	};
};
class CfgVehicles
{
	class TentBase;
	class MJO_MedKit_close_Kit: TentBase
	{
		scope = 2;
		displayName = "Combat MedKit Kit";
		descriptionShort = "Once placed, this Med Kit delivers health to you and or your comrades within a certain radius! Once the healing timer runs out, the Combat Med Kit deletes itself so use it wisely!";
		model = "MJO_MedicBag_source\data\MJO_MedKit_closed.p3d";
		rotationFlags = 4;		
		itemSize[] = {4,4};
		weight = 200;		
		lootCategory = "Materials";
		physLayer = "item_large";			
		projectionTypename = "MJO_MedKit_open";	
		storageCategory=10;		
		class AnimationSources
		{
			class Placing
			{
				source="user";
				animPeriod=2.00;
				initPhase=1;
			};
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 200;
					healthLevels[] = {};
				};
			};
		};		
		hiddenSelections[]=
		{
			"camo"
		};			
		hiddenSelectionsTextures[]=
		{
			"MJO_MedicBag_source\data\MJO_MedKit\MJO_MedKit_Heals_co.paa"			
		};
		hiddenSelectionsMaterials[]=
		{
			"MJO_MedicBag_source\data\MJO_MedKit\MJO_MedKit.rvmat"
		};		
	};
	class MJO_MedKit_open: MJO_MedKit_close_Kit
	{
		scope = 1;
		displayName = "Combat MedKit";
		descriptionShort = "The Combat Med Kit is now deployed and will delete itself after use!";
		model = "MJO_MedicBag_source\data\MJO_MedKit_opened.p3d";		
		itemBehaviour = 0;
		allowDamage = 0;			
	};
	class MJO_MedKit_close_bleed_Kit: MJO_MedKit_close_Kit
	{
		scope = 2;
		displayName = "Combat MedKit Kit";
		descriptionShort = "Once placed, this Med Kit treats up to 6 cuts! Once the timer runs out, the Combat Med Kit deletes itself so use it wisely!";		
		projectionTypename = "MJO_MedKit_open_bleeds";					
		hiddenSelectionsTextures[]=
		{
			"MJO_MedicBag_source\data\MJO_MedKit\MJO_MedKit_Bleeds_co.paa"			
		};	
	};
	class MJO_MedKit_open_bleeds: MJO_MedKit_close_bleed_Kit
	{
		scope = 1;
		displayName = "Combat MedKit";
		descriptionShort = "The Combat Med Kit is now deployed and will delete itself after use!";
		model = "MJO_MedicBag_source\data\MJO_MedKit_opened.p3d";			
		itemBehaviour = 0;
		allowDamage = 0;		
	};	

	class MJO_MedKit_close_illness_Kit: MJO_MedKit_close_Kit
	{
		scope = 2;
		displayName = "Combat MedKit Kit";
		descriptionShort = "Once placed, this Med Kit treats all illnesses! Once the timer runs out, the Combat Med Kit deletes itself so use it wisely!";		
		projectionTypename = "MJO_MedKit_open_illness";				
		hiddenSelectionsTextures[]=
		{
			"MJO_MedicBag_source\data\MJO_MedKit\MJO_MedKit_Illness_co.paa"		
		};	
	};
	class MJO_MedKit_open_illness: MJO_MedKit_close_illness_Kit
	{
		scope = 1;
		displayName = "Combat MedKit";
		descriptionShort = "The Combat Med Kit is now deployed and will delete itself after use!";
		model = "MJO_MedicBag_source\data\MJO_MedKit_opened.p3d";	
		itemBehaviour = 0;
		allowDamage = 0;		
	};	
};
