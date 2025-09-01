class CrashLootUSWeapons
{
	string WeaponName;
	ref TStringArray Magazines;
	ref TStringArray Attachments;
	string Sight;
	
	void CrashLootUSWeapons( string WeaponName_, ref TStringArray Magazine_, ref TStringArray Attachments_, string Sight_)
	{
		WeaponName = WeaponName_;
		Magazines = Magazine_;
		Attachments = Attachments_;
		Sight = Sight_;
	}
}