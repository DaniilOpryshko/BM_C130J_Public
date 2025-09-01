class HelicrashBase
{	
    protected ref array< Plane_C130 > m_Helicopter_Plane;
    
    ref Helicrash_Settings    m_ActiveHelicrash_Settings;
    ref CrashLocations        m_ActiveCrashLocations;
    
    protected ref Timer        m_HelicopterTimer;
                        
    void HelicrashBase()
    {
        if (GetGame().IsServer() && !GetGame().IsClient())
        {
            m_Helicopter_Plane = new array< Plane_C130 >;
            
            m_ActiveHelicrash_Settings = Helicrash_Settings.Load();
            m_ActiveCrashLocations      = m_ActiveHelicrash_Settings.CrashPoints.GetRandomElement();

            m_HelicopterTimer = new Timer();
                    
            int Helicopter_CrashTime = m_ActiveHelicrash_Settings.HelicrashSpawnTime;
            if (Helicopter_CrashTime <= 0)
            {
                Helicopter_CrashTime = 30;
            }

            if (m_ActiveCrashLocations == null)
            {
                Print("HelicrashBase: no crash locations defined");
            }

            m_HelicopterTimer.Run(Helicopter_CrashTime * 60, this, "SpawnCrash", NULL, true);	
        }	
    }
    
    void ~HelicrashBase()
    {
    }
    
    void ClientCallCrash(PlayerBase player, vector PlayerPosition)
    {
        // 
    }
    
    void SpawnCrash()
    {		
        if (GetGame().IsServer() && !GetGame().IsClient())
        {			
            Print("Spawn Crash - Server");

            m_ActiveCrashLocations = m_ActiveHelicrash_Settings.CrashPoints.GetRandomElement();
                        
            if (!m_ActiveCrashLocations)
            {
                Print("HelicrashBase: error in ActiveCrashLocations");
                return;
            }
            
            if (m_ActiveHelicrash_Settings.admin_log)
                Print("[Helicrash] Spawn_Crash start");

            int idx = Math.RandomIntInclusive(0, 2);
            vector helicopter_spawn;
            
            switch (idx)
            {
                case 0:
                    helicopter_spawn = Vector(15347.541016, 0, 9887.758789);
                    break;
                case 1:
                    helicopter_spawn = Vector(15339.140625,  0, 4959.052246);
                    break;
                default:
                    helicopter_spawn = Vector(15319.248047,   0, 58.280090);
                    break;
            }

            if (m_ActiveHelicrash_Settings.admin_log)
                Print("[Helicrash] Spawning Plane_C130");
            
            Plane_C130 plane = Plane_C130.Cast(GetGame().CreateObject("Plane_C130", helicopter_spawn, false, true, true));
        
            if (plane)
            {
                plane.SetPosition(helicopter_spawn);
                m_Helicopter_Plane.Insert(plane);

                vector crash_pos;
                crash_pos[0] = m_ActiveCrashLocations.x;
                crash_pos[2] = m_ActiveCrashLocations.y;

                if (m_ActiveHelicrash_Settings.admin_log)
                    Print("[Helicrash] Plane initialized, setting flight");

                plane.Helicopter_Fly(m_ActiveCrashLocations);
                plane.SetDirection(helicopter_spawn - crash_pos);

                if (m_ActiveHelicrash_Settings.admin_log)
                    Print("[Helicrash] Plane direction set");
            }
        }
    }
}

static ref HelicrashBase g_HelicrashBase;
static ref HelicrashBase GetHelicrashBase()
{
    if (!g_HelicrashBase)
    {
        g_HelicrashBase = new ref HelicrashBase();
    }
    
    return g_HelicrashBase;
}
