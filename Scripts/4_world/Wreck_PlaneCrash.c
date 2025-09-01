class Wreck_PlaneCrash extends House
{
	Particle m_ParticleEfx;
	
	protected vector m_crashpos;
	protected bool m_ShouldDeleteSmoke;
	
	ref Timer m_RemovalTimer_PlaneCrash;
	
	void Wreck_PlaneCrash()
	{
		if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
		{
			m_ParticleEfx = Particle.PlayOnObject(ParticleList.SMOKING_HELI_WRECK, this, Vector(-0.5, 0, -1.0));
		}
		
		if (GetGame().IsServer()|| !GetGame().IsClient()) 
		{
			Print("Despawn Plane Time will Be " + GetHelicrashBase().m_ActiveHelicrash_Settings.HelicrashDespawnTime * 60000 + "Miliseconds");
			GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater(Destroy_Wreck, GetHelicrashBase().m_ActiveHelicrash_Settings.HelicrashDespawnTime * 60000, false);
		}
		
		if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
		{
			m_RemovalTimer_PlaneCrash = new Timer();
			m_RemovalTimer_PlaneCrash.Run(0.07, this, "Removal_Function", NULL, true);
		}			
	}
	
	void Removal_Function()
	{
		if (m_ShouldDeleteSmoke)
		{
			if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
			{
				if (m_ParticleEfx)
					m_ParticleEfx.Stop();
			}
		}	
	}
	
	void Destroy_Wreck()
	{
		if (GetGame().IsServer()) 
		{
			ref array<Object> objects = new array<Object>;
			ref array<CargoBase> proxy = new array<CargoBase>;
		
			GetGame().GetObjectsAtPosition(this.GetPosition(), 15, objects, proxy);
						
			if ( objects ) 
			{
				for ( int i = 0; i < objects.Count(); i++ ) 
				{
					Object obj = objects.Get(i); 
					
					if (obj.IsTree() || obj.IsBush() || obj.IsMan() ) 
						continue;
					
					if (obj.IsKindOf("Wreck_PlaneCrash"))
					{
						m_ShouldDeleteSmoke = true;
					}
					
					GetGame().ObjectDelete(obj);				
				}
			}
		}
	}
	
	override void EEDelete(EntityAI parent)
	{
		if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
		{
			if ( m_ParticleEfx )
				m_ParticleEfx.Stop();
		}
	}
}
