class EventSettings
{
    ref array<ref CrashLocation> crashLocations;
    int height = 300;
    int initialSpeed = 50;
    int speedOneEngine = 33;
    int slideSpeed = 15;

    void EventSettings()
    {
        crashLocations = new array<ref CrashLocation>;
    }

    CrashLocation GetRandomCrashLocation()
    {
        if (crashLocations.Count() == 0)
        {
            Print("[EventSettings] Нет доступных точек спавна!");
            return null;
        }

        int randomIndex = Math.RandomInt(0, crashLocations.Count());
        return crashLocations[randomIndex];
    }
}