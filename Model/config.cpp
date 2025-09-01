class CfgPatches
{
	class BM_C130J_Models
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
	};
};

class CfgVehicles
{
	class House;
	
	class Plane_C130: House
	{

		scope = 2;
		model = "BM_C130J_Public\Model\C130\model\C130.p3d";

		class AnimationSources
		{
			class propeller_01
			{
				source="user";
				sourceAdress="loop";
				initPhase=1;
				animPeriod=45;
			};
			class propeller_02
			{
				source="user";
				sourceAdress="loop";
				initPhase=1;
				animPeriod=45;
			};
			class propeller_03
			{
				source="user";
				sourceAdress="loop";
				initPhase=1;
				animPeriod=45;
			};
			class propeller_04
			{
				source="user";
				sourceAdress="loop";
				initPhase=1;
				animPeriod=45;
			};
		};
	};

	class PlaneCrash_Crash: House
	{
		scope = 2;
		model = "BM_C130J_Public\Model\C130\model\C130.p3d";

		class AnimationSources
		{
			class propeller_01
			{
				source="user";
				sourceAdress="loop";
				initPhase=1;
				animPeriod=45;
			};
			class propeller_02
			{
				source="user";
				sourceAdress="loop";
				initPhase=1;
				animPeriod=45;
			};
			class propeller_03
			{
				source="user";
				sourceAdress="loop";
				initPhase=1;
				animPeriod=45;
			};
			class propeller_04
			{
				source="user";
				sourceAdress="loop";
				initPhase=1;
				animPeriod=45;
			};
		};
	};

	class Wreck_PlaneCrash: House
	{
		scope = 2;
		model = "\DZ\structures\Wrecks\Aircraft\Wreck_C130J_Cargo.p3d";
	};
};
