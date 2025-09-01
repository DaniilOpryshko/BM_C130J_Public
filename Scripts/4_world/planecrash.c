class Plane_C130 extends House
{
	ref Helicrash_Settings m_ActiveHelicrash_Settings;
	ref CrashLocations m_ActiveCrashLocations;

	protected EffectSound m_Sound;

	protected bool m_HasCrashed;
	protected float m_RotationalA;
	protected float m_Radius;
	protected vector m_Location;
	protected float m_height = 250;
	protected float m_speed = 50;
	protected float m_minimum_speed = 25;
	
	protected float m_RotorSpeed;
	protected float m_RotorAnimationPosition;

	// NEW FIELDS BY DANIELELE
	private PlaneStatus planeStatus;
	private Particle FirstengineSmoke, SecondengineSmoke;
	private ref Timer flareTimer;
	
	void Helicopter_Fly(ref CrashLocations activeCrashLocations)
	{		
		m_ActiveCrashLocations = activeCrashLocations;

		m_Location = Vector(m_ActiveCrashLocations.x, 0, m_ActiveCrashLocations.y);
		m_Radius = m_ActiveCrashLocations.Radius;

		vector position = GetPosition();
		
		m_RotationalA = Math.Atan2( m_ActiveCrashLocations.y - position[2], m_ActiveCrashLocations.x - position[0] );
	}

	void SetPlaneStatus(PlaneStatus status)
	{
		planeStatus = status;
		SetSynchDirty();
	}

	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();

		if (planeStatus == PlaneStatus.FIRST_ENGINE_FAILURE)
		{
			FirstengineSmoke = ParticleManager.GetInstance().PlayOnObject( ParticleList.SMOKING_HELI_WRECK, this, GetMemoryPointPos("1Engine_Smoke") );
			flareTimer = new Timer();
			flareTimer.Run(0.25, this, "ejectFlares", null, true);
		}
		
		if (planeStatus == PlaneStatus.SECOND_ENGINE_FAILURE)
		{
			SecondengineSmoke = ParticleManager.GetInstance().PlayOnObject( ParticleList.SMOKING_HELI_WRECK, this, GetMemoryPointPos("2Engine_Smoke") );
			if (flareTimer)
			{
				flareTimer.Stop();
			}
		}
	}

	private void ejectFlares()
	{
		EffectSound m_FireSound;
		PlaySoundSet( m_FireSound, "FireworksLauncher_Shot_SoundSet", 0, 1 );
		ParticleManager.GetInstance().PlayInWorldEx( ParticleList.C130_FLARE_LEFT, null, ModelToWorld(GetMemoryPointPos("FlareLeftEject")), GetOrientation() + "0 180 135" , true);
		ParticleManager.GetInstance().PlayInWorldEx( ParticleList.C130_FLARE_DOWN, null, ModelToWorld(GetMemoryPointPos("FlareDownEject")), GetOrientation() + "0 135 180" , true );
		ParticleManager.GetInstance().PlayInWorldEx( ParticleList.C130_FLARE_RIGHT, null, ModelToWorld(GetMemoryPointPos("FlareRightEject")), GetOrientation() + "0 180 225" , true );
		Print("flares ejected");
	}

	void Plane_C130()
	{
		RegisterNetSyncVariableInt("planeStatus", 0, EnumTools.GetEnumSize(PlaneStatus));
		if (GetGame().IsServer() && !GetGame().IsClient())
		{
			m_ActiveHelicrash_Settings = Helicrash_Settings.Load();
			this.SetAllowDamage(true);
		}
		
		SetEventMask( EntityEvent.CONTACT | EntityEvent.SIMULATE );

		if (GetGame().IsClient() || !GetGame().IsMultiplayer())
			PlayPlaneAudio();
	}
	
	void ~Plane_C130()
	{
		if (m_Sound)
			m_Sound.Stop();
	}

	void PlayPlaneAudio()
	{
		if (GetGame().IsClient() || !GetGame().IsServer())
			PlaySoundLoop("BM_C130J_Loop_Fly", 1750);
	}

	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		Print("EEHitBy: " + this + "; damageType: "+ damageType +"; source: "+ source +"; component: "+ component +"; dmgZone: "+ dmgZone +"; ammo: "+ ammo +"; modelPos: "+ modelPos);
					
		if (this.GetHealth("", "") <= 0)
			PlaneCrashExplosion(true);
	}
	
	override void EOnSimulate( IEntity owner, float dt )
	{
		// if ( GetGame().IsServer() )
		// {			
		// 	this.ShowSelection( "hiderotorblur" );
		// 	this.HideSelection( "hiderotor" );
			
		// 	m_RotorAnimationPosition += 0.45 * dt * 0.5;
			
		// 	if ( m_RotorAnimationPosition >= 1 )
		// 		m_RotorAnimationPosition -= 1;
		
		// 	SetAnimationPhase( "rotor", m_RotorAnimationPosition );
		// 	SetAnimationPhase( "rearrotor", m_RotorAnimationPosition );

		// 	vector position = GetPosition();
		// 	vector velocity = Vector( Math.Cos( m_RotationalA ), 0, Math.Sin( m_RotationalA ) );

		// 	if (m_ActiveHelicrash_Settings)
		// 	{
		// 		for (int d = 0; d < m_ActiveHelicrash_Settings.HelicopterPlane_.Count(); d++)
		// 		{
		// 			m_speed = m_ActiveHelicrash_Settings.HelicopterPlane_.Get(d).speed;
		// 			m_minimum_speed = m_ActiveHelicrash_Settings.HelicopterPlane_.Get(d).minimum_speed;
		// 			m_height = m_ActiveHelicrash_Settings.HelicopterPlane_.Get(d).start_height;
		// 		}
		// 	}

		// 	velocity[0] = velocity[0]  * (m_speed) * dt;
		// 	velocity[2] = velocity[2]  * (m_speed) * dt;
			
		// 	vector transform[4];
		// 	GetTransform( transform );
		// 	transform[3] = transform[3] + velocity;
		// 	transform[3][1] = GetGame().SurfaceY(GetPosition()[0], GetPosition()[2]) + m_height;

		// 	MoveInTime( transform, dt );

		// 	float distance_ = Math.Sqrt(Math.Pow(m_Location[0] - GetPosition()[0], 2) + Math.Pow(m_Location[2] - GetPosition()[2], 2));

		// 	if (distance_ >= 2000 && m_height >= 50)
		// 		m_height--;

		// 	if (distance_ <= 2000)
		// 	  	m_speed = m_minimum_speed;
						
		// 	PlaneCrashExplosion(false);

		// }
	}

	void PlaneCrashExplosion(bool should_force_crash_)
	{
		float dist = Math.Sqrt(Math.Pow(m_Location[0] - GetPosition()[0], 2) + Math.Pow(m_Location[2] - GetPosition()[2], 2));
		
		if (dist <= Math.RandomFloat(0, m_Radius) || should_force_crash_) 
		{
			if (!m_HasCrashed)
			{
				CreateWreckModel();
				m_HasCrashed = true;
			}
		}
	}

	void CreateWreckModel()
	{
		Object drop = Object.Cast( GetGame().CreateObject( "PlaneCrash_Crash", Vector( GetPosition()[0], GetPosition()[1], GetPosition()[2]) ));	
						
		drop.SetPosition(Vector( GetPosition()[0], GetPosition()[1], GetPosition()[2]))
		drop.SetOrientation(Vector(GetOrientation()[0], GetOrientation()[1], GetOrientation()[2]));
		
		GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( GetGame().ObjectDelete, 0, false, this );
	}
}