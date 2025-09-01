class PlaneCrash_Crash extends House
{
	ref Timer m_Timer;
	ref Timer m_RemovalTimer;
	ref Helicrash_Settings m_ActiveHelicrash_Settings;

	protected bool m_Loot;
	protected bool m_IsDeleted;
	protected bool m_bShouldDeleteSmoke;
	
	protected vector m_GroundPos;
	
		
	void PlaneCrash_Crash()
	{
		// SetEventMask( EntityEvent.CONTACT | EntityEvent.SIMULATE );

		// if (GetGame().IsServer() && !GetGame().IsClient()) 
		// {
		// 	m_ActiveHelicrash_Settings = GetHelicrashBase().m_ActiveHelicrash_Settings;
		// }

		// PlaneCrash_Crash_Init();
			
		// if (GetGame().IsClient() || !GetGame().IsMultiplayer())
		// 	PlayAudioClient();
	}	

	void PlayAudioClient()
	{
		PlaySound("Explosion_Init", 2000);
	}
	
	void ~PlaneCrash_Crash()
	{
		
		if (m_Timer) 
			m_Timer.Stop();
		
		if (m_RemovalTimer) 
			m_RemovalTimer.Stop();
	}

	void PlaneCrash_Crash_Init()
	{
		if (GetGame().IsClient() || !GetGame().IsMultiplayer()) 
		{
			//m_ParticleEfx = Particle.PlayOnObject(ParticleList.SMOKING_HELI_WRECK,this, Vector(-0.5, 0, -1.0));
			//m_ExpansionEfx = Particle.PlayOnObject( ParticleList.EXPANSION_FIRE_HELICOPTER, this, Vector(-0.5, 0, -1.0));
		}

		if (GetGame().IsServer() && !GetGame().IsClient()) 
		{
			SetOrientation(Vector(GetOrientation()[0], 0, 0));
			SetDirection(GetDirection());

			m_Timer = new Timer();
			m_Timer.Run(0.07, this, "PlaneCrashGroundStage", NULL, true);
		}
		
		if (GetGame().IsServer() && !GetGame().IsClient()) 
		{
			m_RemovalTimer = new Timer();
			m_RemovalTimer.Run(0.07, this, "Depawn_Plane", NULL, true);
		}			
	}
	
	void Depawn_Plane()
	{
		if (m_IsDeleted && m_bShouldDeleteSmoke)
		{
			if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
			{
				/*if (m_ParticleEfx)
					m_ParticleEfx.Stop();
			
				if (m_ExpansionEfx)
					m_ExpansionEfx.Stop();*/
			}
		}	
	}
	
	void Helicrash_Rotate()
	{
		if (GetVelocity(this).Length() >= 0) 
		{
			SetDirection(GetDirection() - Vector(-40, -10, 0));

			CreateDynamicPhysics(PhxInteractionLayers.DYNAMICITEM);
			EnableDynamicCCD(true);

			//SetOrientation(Vector(GetOrientation()[0] + 15, 0, GetOrientation()[2] - 5)); //spin the object
			SetOrientation(Vector(GetOrientation()[0], GetOrientation()[1], GetOrientation()[2] - Math.RandomInt(0, 3))); //spin the object
			
			SetVelocity(this, Vector(-40, -10, 0));
		}
	}
	
	static Weapon_Base CreateWeaponWithMagazine( string sWeapon, string sMagazine, vector pos )
	{
	    Weapon_Base oWpn = Weapon_Base.Cast(GetGame().CreateObject( sWeapon , pos ));
	
	    if ( !oWpn )
	        return NULL;
		
		if ( sMagazine != "" )
			oWpn.GetInventory().CreateAttachment( sMagazine );
		  
	    return oWpn;
	}
	
	void Helicrash_SpawnLoot()
	{
		if (m_IsDeleted)
			return;
		
		vector orientation;
		
		SetVelocity( this, Vector( 0, 0, 0 ) );

        dBodySetAngularVelocity( this, Vector( 0, 0, 0 ) );
        dBodyActive( this, ActiveState.INACTIVE );
        dBodyDynamic( this, false );
				
		SetDirection(GetDirection());
		SetOrientation(Vector(GetOrientation()[0], 0, 0));
		
		orientation = GetOrientation();
		
		PlaceOnSurface();
		
		SetPosition(m_GroundPos);
		
		if (!m_Loot)
		{
			int backpack_count = 0;
					
			for (int ac = 0; ac < m_ActiveHelicrash_Settings.AnimalSpawnArray.Count(); ac++) 
			{
				ref AnimalsSpawn AnimalSpawnArray = m_ActiveHelicrash_Settings.AnimalSpawnArray.Get(ac);

				int animal_spawn = Math.RandomInt(AnimalSpawnArray.amount_minimum, AnimalSpawnArray.amount_maximum);
				
				for (int SpawnCountA = 0; SpawnCountA < animal_spawn; SpawnCountA++) 
				{
					vector Animal_Spawn = m_GroundPos;
					
					Animal_Spawn[0] = Animal_Spawn[0] + (5.5 * Math.Cos(SpawnCountA));
					Animal_Spawn[2] = Animal_Spawn[2] + (5.5 * Math.Sin(SpawnCountA));
					Animal_Spawn[1] = GetGame().SurfaceY(Animal_Spawn[0], Animal_Spawn[2]) + 0.2;
					
					GetGame().CreateObject(AnimalSpawnArray.animal_name.GetRandomElement(), Animal_Spawn, false, true);
				}
				
			}
				
			for (int zb = 0; zb < m_ActiveHelicrash_Settings.ZombieSpawnArray.Count(); zb++) 
			{
				ref ZombieSpawn ZombieSpawnArray = m_ActiveHelicrash_Settings.ZombieSpawnArray.Get(zb);

				int ZombieSpawnAmount = Math.RandomInt(ZombieSpawnArray.amount_minimum, ZombieSpawnArray.amount_maximum);
				
				for (int SpawnCountZ = 0; SpawnCountZ < ZombieSpawnAmount; SpawnCountZ++) 
				{
					vector Zombie_Spawn = m_GroundPos;
					
					Zombie_Spawn[0] = Zombie_Spawn[0] + (3.5 * Math.Cos(SpawnCountZ));
					Zombie_Spawn[2] = Zombie_Spawn[2] + (3.5 * Math.Sin(SpawnCountZ));
					Zombie_Spawn[1] = GetGame().SurfaceY(Zombie_Spawn[0], Zombie_Spawn[2]) + 0.2;
					
					GetGame().CreateObject(ZombieSpawnArray.zombie_name.GetRandomElement(), Zombie_Spawn, false, true);
				}
				
			}
			

			ref TStringArray Loot_Tables = new TStringArray;

			for (int ab = 0; ab < m_ActiveHelicrash_Settings.Loot_Planecrash.Count(); ab++) 
			{
				Loot_Tables.Insert(m_ActiveHelicrash_Settings.Loot_Planecrash.Get(ab));
			}
				
			int loot_spawn_amount = 0; 
			int loot_weapon_spawn_amount = 0;
			
			for (int v = 0; v < m_ActiveHelicrash_Settings.HelicopterPlane_.Count(); v++)
			{
				loot_spawn_amount = Math.RandomIntInclusive(m_ActiveHelicrash_Settings.HelicopterPlane_.Get(v).Minimum_Loot_Planecrash, m_ActiveHelicrash_Settings.HelicopterPlane_.Get(v).Maximum_Loot_Planecrash);
				loot_weapon_spawn_amount = Math.RandomIntInclusive(m_ActiveHelicrash_Settings.HelicopterPlane_.Get(v).Minimum_Weapons_Planecrash, m_ActiveHelicrash_Settings.HelicopterPlane_.Get(v).Maximum_Weapons_Planecrash);
			}
				
			for (int i = 0; i < loot_spawn_amount; i++) 
			{							
				vector loot_pos = m_GroundPos;
							
				loot_pos[0] = loot_pos[0] + (2.5 * Math.Cos(i));
				loot_pos[2] = loot_pos[2] + (2.5 * Math.Sin(i));
				loot_pos[1] = GetGame().SurfaceY(loot_pos[0], loot_pos[2]) + 0.2;
		
				Print(Loot_Tables.GetRandomElement());
				EntityAI SpawnedObject;
				SpawnedObject = EntityAI.Cast( GetGame().CreateObject(Loot_Tables.GetRandomElement(), loot_pos, false, true) );
				
			}
				
			for (int j = 0; j < loot_weapon_spawn_amount; j++)
			{
				CrashLootUSWeapons WeaponLootTables_RU;
					
				WeaponLootTables_RU = m_ActiveHelicrash_Settings.WeaponLootTables.GetRandomElement();
					
				if (WeaponLootTables_RU)
				{
					vector loot_pos_ = m_GroundPos;
								
					loot_pos_[0] = loot_pos_[0] + (3.5 * Math.Cos(j));
					loot_pos_[2] = loot_pos_[2] + (3.5 * Math.Sin(j));
					loot_pos_[1] = GetGame().SurfaceY(loot_pos_[0], loot_pos_[2]) + 0.2;
			
					string weapon_name = WeaponLootTables_RU.WeaponName;
					string Sight = WeaponLootTables_RU.Sight;
	
					Weapon_Base weapon;
					ItemBase optic;
												
					if (WeaponLootTables_RU.Magazines.Count() > 0)
					{
						weapon = CreateWeaponWithMagazine( WeaponLootTables_RU.WeaponName, WeaponLootTables_RU.Magazines.Get(0), loot_pos_);
		
					}
					else
					{
						weapon = Weapon_Base.Cast( GetGame().CreateObject(WeaponLootTables_RU.WeaponName, loot_pos_, false, true) );
					}
						
					if (weapon)
					{
						if (WeaponLootTables_RU.Attachments.Count() > 0 )
						{
							for (int attch = 0; attch < WeaponLootTables_RU.Attachments.Count(); attch++)
							{
								weapon.GetInventory().CreateAttachment( WeaponLootTables_RU.Attachments.Get(attch) );
							}	
						}
							
						if (WeaponLootTables_RU.Sight != "")
						{
							optic = weapon.GetInventory().CreateAttachment( WeaponLootTables_RU.Sight );
								
							if (optic)
								optic.GetInventory().CreateAttachment("Battery9V");
						}
					}
				}
				m_Loot = true;
				SetSynchDirty();
			}		
		}
			
		if (this && !m_IsDeleted)
		{
			GetGame().ObjectDelete(this);
				
			m_IsDeleted = true;
			m_bShouldDeleteSmoke = true;
		}
			
		Object PlaneCrashObject = GetGame().CreateObject( "Wreck_PlaneCrash", m_GroundPos, false, true, true);
		PlaneCrashObject.SetOrientation(Vector(orientation[0], 0, 0));
		Print("Plane " + GetModelName() + " " + "Crashed at " + m_GroundPos);
		PlaneCrashObject.PlaceOnSurface();
		
		ref array<Object> objects = new array<Object>;
		ref array<CargoBase> proxy = new array<CargoBase>;
		
		GetGame().GetObjectsAtPosition(this.GetPosition(), 10, objects, proxy);
						
		if ( objects )
		{
			for ( int d = 0; d < objects.Count(); d++ )
			{
				Object obj = objects.Get(d);

				if (obj.IsTree() || obj.IsBush())
					obj.SetHealth("", "", 0);
				else 
					continue;
				
			}
		}
		
		NotificationSystem.Create( new StringLocaliser( "Aircraft Crashed" ), new StringLocaliser( "The Aircraft crashed on the map there is no exact location, Hurry to pick up the loot before the plane explodes!" ), "", ARGB(255, 221, 38, 38), 10, NULL );

	}

	void PlaneCrashGroundStage()
	{
		if (GetGame().IsServer() && !GetGame().IsClient() && !m_IsDeleted) 
		{
			if (!IsHitTheGround(5))
			{
				Helicrash_Rotate();
			}
			else 
			{
				m_GroundPos[0] = GetPosition()[0];
				m_GroundPos[1] = GetPosition()[1] - 4;
				m_GroundPos[2] = GetPosition()[2];
				
				Helicrash_SpawnLoot();		
			}
		}
	}
	
	bool IsHitTheGround(float height)
	{
		vector m_Start = this.GetPosition();
		vector m_End = this.GetPosition() - Vector(4, height, 5);
		
		vector m_Hit;
		vector m_HitPos;
		
		int m_HitIndex;

		return DayZPhysics.RaycastRV(m_Start, m_End, m_HitPos, m_Hit, m_HitIndex, NULL, NULL, this);
	}

	override bool CanPutIntoHands( EntityAI parent )
	{
		return false;
	}

	override bool CanPutInCargo ( EntityAI parent )
	{
		return false;
	}

	override bool IsInventoryVisible()
    {
        return true;
    }

	override bool CanUseConstruction()
    {
        return true;
    }
}