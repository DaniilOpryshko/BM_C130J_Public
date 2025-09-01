class CrashLocations
{
    float x, y;
    float Radius;
    string Crash_Message;

    void CrashLocations(float x_, float y_, float Radius_, string Crash_Message_) 
	{
        x = x_;
        y = y_;
        Radius = Radius_;
        Crash_Message = Crash_Message_;
    }
}

class AnimalsSpawn
{
	ref TStringArray animal_name;
	int radius;
	
	int amount_minimum;
	int amount_maximum;

	void AnimalsSpawn(TStringArray animal_name_, int radius_, int amount_minimum_, int amount_maximum_)
	{
		animal_name = animal_name_;
		radius = radius_;
		amount_minimum = amount_minimum_;
		amount_maximum = amount_maximum_;
		
	}
}