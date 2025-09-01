class ZombieSpawn
{
	ref TStringArray zombie_name;
	int radius;
	int amount_minimum;
	int amount_maximum;
	
	void ZombieSpawn(ref TStringArray zombie_name_, int radius_, int amount_minimum_, int amount_maximum_)
	{
		zombie_name = zombie_name_;
		radius = radius_;
		amount_minimum = amount_minimum_;
		amount_maximum = amount_maximum_;
	}
}

class CrashLootRUWeapons
{
	string WeaponName;
	ref TStringArray Magazines;
	ref TStringArray Attachments;
	string Sight;
	
	void CrashLootRUWeapons( string WeaponName_, ref TStringArray Magazine_, ref TStringArray Attachments_, string Sight_)
	{
		WeaponName = WeaponName_;
		Magazines = Magazine_;
		Attachments = Attachments_;
		Sight = Sight_;
	}
}