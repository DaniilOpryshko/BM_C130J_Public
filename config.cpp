class CfgPatches
{
	class BM_C130J_Public
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
	};
};

class CfgMods
{
	class BM_C130J_Public
	{
		dir = "BM_C130J_Public";
		picture = "";
		action = "";
		hideName = 0;
		hidePicture = 1;
		name = "";
		credits = "";
		author = "";
		authorID = "";
		extra = 0;
		type = "mod";
		dependencies[] = {"Game","World","Mission"};
		class defs
		{
			class gameScriptModule
			{
				value = "";
				files[] = {"BM_C130J_Public/Scripts/3_Game"};
			};

			class worldScriptModule
			{
				value = "";
				files[] = {"BM_C130J_Public/Scripts/4_World"};
			};

			class missionScriptModule
			{
				value = "";
				files[] = {"BM_C130J_Public/Scripts/5_Mission"};
			};
		};
	};
};