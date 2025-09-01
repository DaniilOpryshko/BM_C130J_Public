class HelicopterPlane
{
	int start_height;
	int minimum_height;
	int speed;
	int minimum_speed;
	
	int Maximum_Loot_Planecrash;
	int Maximum_Weapons_Planecrash;
	
	int Minimum_Loot_Planecrash;
	int Minimum_Weapons_Planecrash;
	
 	void HelicopterPlane(int start_height_, int minimm_height_, int speed_, int minimum_speed_, int Maximum_Loot_Planecrash_, int Maximum_Weapons_Planecrash_, int Minimum_Loot_Planecrash_, int Minimum_Weapons_Planecrash_) 
	{
        start_height = start_height_;
        minimum_height = minimm_height_;
        speed = speed_;
        minimum_speed = minimum_speed_;
		
		Maximum_Loot_Planecrash = Maximum_Loot_Planecrash_;
		Maximum_Weapons_Planecrash = Maximum_Weapons_Planecrash_;
		
		Minimum_Loot_Planecrash = Minimum_Loot_Planecrash_;
		Minimum_Weapons_Planecrash = Minimum_Weapons_Planecrash_;
    }
}