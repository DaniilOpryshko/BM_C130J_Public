class CfgPatches
{
	class BM_C130J_SoundEffects
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","DZ_Gear_Consumables"};
	};
};

class CfgSoundSets
{
	class baseHelicopterEngine_EXT_SoundSet
	{
		sound3DProcessingType = "Vehicle_Ext_3DProcessingType";
		distanceFilter = "softVehiclesDistanceFreqAttenuationFilter";
		volumeCurve = "vehicleEngineAttenuationCurve";
		volumeFactor = 1;
		spatial = 1;
		loop = 0;
	};

	class BM_C130J_Loop_Fly_SoundSet: baseHelicopterEngine_EXT_SoundSet
	{
		soundShaders[] = {"BM_C130J_Loop_Fly_SoundShader"};
		volumeFactor = 1;
	};
};

class CfgSoundShaders
{
	class baseEngineC130J_SoundShader
	{
		range = 1750;
	};

	class BM_C130J_Loop_Fly_SoundShader: baseEngineC130J_SoundShader
	{
		samples[] = {{"BM_C130J_Public\Assets\Sounds\Airplane",1}};
		volume = 4;
	};
};

class CfgSounds
{
	class default
	{
		name = "";
		titles[] = {};
	};

	class BM_C130J_Loop_Fly: default
	{
		sound[] = {"BM_C130J_Public\Assets\Sounds\Airplane",1,1,1750};
	};
};