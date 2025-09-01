modded class MissionServer
{
    private ref PlaneEventManager eventManager;

    void MissionServer()
    {	    
		if (GetGame().IsServer() && !GetGame().IsClient())
        {
		   eventManager = GetPlaneEventManager();
        }

        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(CallPlaneCrash, 22000, false);

        GetRPCManager().AddRPC("MissionServer", "CallPlaneCrash", this);
    }

    private void CallPlaneCrash()
    {
        eventManager.StartEvent(BmEventType.EVENT_PLANE_CRASH);

        array<Man> players = new array<Man>;

        GetGame().GetPlayers(players);

        int i = 0;

        for (i;i < players.Count();i++)
        {
            NotificationSystem.SendNotificationToPlayerExtended(players.Get(i), 10.0, "Samolet spawn");
        }
    }
	

}

modded class MissionGameplay
{

	override void OnKeyPress(int key)
	{
		super.OnKeyPress(key);
		
		
		if (key == 37)
		{
					GetRPCManager().SendRPC(
		"MissionServer",                  // namespace
		"CallPlaneCrash", 			     // идентификатор RPC
		null,                              // параметры (Param)
		true,                              // guaranteed = true
		GetGame().GetPlayer().GetIdentity());
		}
	}

}
