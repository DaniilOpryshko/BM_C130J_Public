class PlaneEventManager : PluginBase
{
    protected ref array<ref BmEventBase> m_ActiveEvents;

    override void OnInit()
    {
        super.OnInit();
        m_ActiveEvents = new array<ref BmEventBase>();
        Print("[PlaneEventManager] Инициализация");
        //Тут подгрузка конфигов будет
    }

    override void OnUpdate(float delta_time)
    {
        int i = 0;
        for (i; i < m_ActiveEvents.Count(); i++)
        {
            m_ActiveEvents.Get(i).OnUpdate(delta_time);
        }
    }

    void StartEvent(BmEventType eventType)
    {
        BmEventBase eventCrash = CreateEvent(eventType);

        if (eventCrash)
        {
            m_ActiveEvents.Insert(eventCrash);
			eventCrash.Start();
        }
    }

    BmEventBase CreateEvent(BmEventType type)
    {
        switch (type)
        {
            case BmEventType.EVENT_PLANE_CRASH:
                return new PlaneCrashEvent(CreateMockSettings());

            case BmEventType.EVENT_HELI_CRASH:
                return null;
        }
        return null;
    }

    private EventSettings CreateMockSettings()
    {
        EventSettings settingi = new EventSettings();
        CrashLocation crashLoc = new CrashLocation();
        crashLoc.location = "4456.98 319.917 8618.52";
        crashLoc.orientation = "-124.321 0 -0";
        settingi.crashLocations.Insert(crashLoc);
        settingi.height = 200;

        return settingi;
    }

}

PlaneEventManager GetPlaneEventManager()
{
	return PlaneEventManager.Cast(GetPlugin(PlaneEventManager));
}