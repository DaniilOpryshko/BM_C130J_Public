class Helicrash_Settings
{
	bool admin_log;
	
	bool EnablePlaneCrashes;
	
	int HelicrashSpawnTime; 
	int HelicrashDespawnTime;
	
	ref array< ref CrashLocations > CrashPoints;
	
	ref array< ref HelicopterPlane > HelicopterPlane_;
	

	ref array< ref AnimalsSpawn > AnimalSpawnArray;
	ref array< ref ZombieSpawn > ZombieSpawnArray;

	ref TStringArray Loot_Planecrash;
	
	ref array< ref CrashLootUSWeapons > WeaponLootTables;
	
	void Helicrash_Settings()
	{		
		admin_log = true;
		EnablePlaneCrashes = true;
		CrashPoints = new ref array< ref CrashLocations >;
		HelicopterPlane_ = new ref array< ref HelicopterPlane >;
		AnimalSpawnArray = new ref array< ref AnimalsSpawn >;
		ZombieSpawnArray = new ref array<ref ZombieSpawn>;
		Loot_Planecrash = new ref TStringArray;
		WeaponLootTables = new ref array< ref CrashLootUSWeapons >;
		
	}
		
	void SetupJSONCFG()
	{
		admin_log = true;
		
		EnablePlaneCrashes = false;
		
		HelicrashSpawnTime = 30;
		HelicrashDespawnTime = 60;
		
		CrashPoints.Insert( new ref CrashLocations(4893, 9660, 100, "Helicopter 1") );
		CrashPoints.Insert( new ref CrashLocations(5216.25, 8850, 100, "Helicopter 2") );
		CrashPoints.Insert( new ref CrashLocations(5418.75, 8385, 100, "Helicopter 3") );

		HelicopterPlane_.Insert(new ref HelicopterPlane(250, 50, 50, 25, 5, 5, 0, 0));
		
		AnimalSpawnArray.Insert( new ref AnimalsSpawn({"Animal_BosTaurusF_Brown"}, 10, 1, 5) );
		ZombieSpawnArray.Insert( new ref ZombieSpawn({"ZmbM_usSoldier_normal_Woodland"}, 10, 1, 5) );
		
		Loot_Planecrash.Insert("Mag_STANAG_30Rnd");
		Loot_Planecrash.Insert("Mag_STANAG_30Rnd");
				
		WeaponLootTables.Insert( new ref CrashLootUSWeapons("M4A1", {"Mag_STANAG_30Rnd"}, {"M4_Suppressor"}, "M68Optic"));
		WeaponLootTables.Insert( new ref CrashLootUSWeapons("M4A1", {"Mag_STANAG_30Rnd"}, {"M4_Suppressor"}, "M68Optic"));
		
		 if (!FileExist(m_ProfileDirectory + ConfigDir + "/"))
			MakeDirectory(m_ProfileDirectory + ConfigDir + "/");
		
		JsonFileLoader<Helicrash_Settings>.JsonSaveFile(PATH_HELICRASH, this );
	}

	static ref Helicrash_Settings Load()
	{
		ref Helicrash_Settings Helicopter_Settings = new Helicrash_Settings();
		
		if (!FileExist(ConfigDir))
            MakeDirectory(ConfigDir);
		
		if (FileExist("$profile:BM_C130J_TEST/Settings.json"))
        {
			JsonFileLoader<Helicrash_Settings>.JsonLoadFile( PATH_HELICRASH, Helicopter_Settings );
		} 
		else 
		{
			
			Helicopter_Settings.SetupJSONCFG();
		}
			
		return Helicopter_Settings;
	}
}