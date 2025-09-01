class PlaneCrashEvent extends BmEventBase
{
    private ref PlaneNavigationData m_navigationData;
    private ref PlaneFlightState m_planeFlightState;
    private Plane_C130 m_Plane;
    private bool physicsInitiated;
    private static float TIME_TO_CHANGE_SPEED_ONE_ENGINE_PHASE = 6.0;
    private static float NOSE_STALLING_PHASE_TIME = 5.0;
    private static float ACCELERATING_PHASE_TIME = 6.0;
    private static float BARREL_ROLL_PHASE_TIME = 4.0;
    private static float LANDING_PHASE_TIME = 5.0;

    void Configure()
    {
        if (m_Settings)
        {
            CrashLocation crashLocation = m_Settings.GetRandomCrashLocation();
            m_navigationData = new PlaneNavigationData(crashLocation, m_Settings.height);
            m_planeFlightState = new PlaneFlightState();
            m_planeFlightState.m_CurrentSpeed = m_Settings.initialSpeed;
        }
    }

    void Start()
    {
        m_Plane = Plane_C130.Cast(GetGame().CreateObject("Plane_C130", m_navigationData.m_SpawnPosition, false, true, true));

        if (m_Plane)
        {
            Print("Plane spawned at pos: " + m_navigationData.m_SpawnPosition);
            m_Plane.SetPosition(m_navigationData.m_SpawnPosition);
            m_Plane.SetOrientation(m_navigationData.m_PlaneOrientation);
            SetEventStatus(PlaneStatus.FLYING);
        }
    }

    void OnUpdate(float delta_time)
    {
		if (m_Plane)
		{
            switch(m_planeFlightState.m_Status)
            {
                case PlaneStatus.FLYING:
                    LinearMove(delta_time);
                    break;
                case PlaneStatus.FIRST_ENGINE_FAILURE:
                    LinearMoveOneEngineFailed(delta_time);
                    break;
                case PlaneStatus.SECOND_ENGINE_FAILURE:
                    LinearMoveTwoEngineFailed(delta_time);
                    break;
                case PlaneStatus.ENGINE_FAILURE:
                    FallingTwoEnginesDead(delta_time);
                    break;
            }
            m_planeFlightState.m_PhaseTimeElapsed += delta_time;
		}
    }

    private void LinearMove(float delta_time)
    {
        CheckIfFirstEngShouldFail();

        vector transform[4];
        m_Plane.GetTransform(transform);

        vector velOfss = transform[2] * delta_time * m_planeFlightState.m_CurrentSpeed;

        transform[3] = transform[3] - velOfss;

        transform[3][1] = GetGame().SurfaceY(transform[3][0], transform[3][2]) + m_Settings.height;
        
        m_Plane.MoveInTime(transform, delta_time);
    }

    private void LinearMoveOneEngineFailed(float delta_time)
    {
        HitSecondEngineFailPoint();
        vector transform[4];
        m_Plane.GetTransform(transform);
		
		if (m_planeFlightState.m_CurrentSpeed > m_Settings.speedOneEngine)
		{
			m_planeFlightState.m_CurrentSpeed = m_Settings.initialSpeed - ((m_Settings.initialSpeed - m_Settings.speedOneEngine) * (m_planeFlightState.m_PhaseTimeElapsed /  TIME_TO_CHANGE_SPEED_ONE_ENGINE_PHASE));
		}
		else
		{
			m_planeFlightState.m_CurrentSpeed = m_Settings.speedOneEngine;
		}

        vector velOfss = transform[2] * delta_time * m_planeFlightState.m_CurrentSpeed;


        transform[3] = transform[3] - velOfss;

        transform[3][1] = GetGame().SurfaceY(transform[3][0], transform[3][2]) + m_Settings.height;
        
        m_Plane.MoveInTime(transform, delta_time);
    }

    private void LinearMoveTwoEngineFailed(float delta_time)
    {
        HitEngineFailPoint();
        vector transform[4];
        m_Plane.GetTransform(transform);
		
		if (m_planeFlightState.m_CurrentSpeed > m_Settings.speedOneEngine)
		{
			m_planeFlightState.m_CurrentSpeed = m_Settings.initialSpeed - ((m_Settings.initialSpeed - m_Settings.speedOneEngine) * (m_planeFlightState.m_PhaseTimeElapsed /  TIME_TO_CHANGE_SPEED_ONE_ENGINE_PHASE));
		}
		else
		{
			m_planeFlightState.m_CurrentSpeed = m_Settings.speedOneEngine;
		}

        vector velOfss = transform[2] * delta_time * m_planeFlightState.m_CurrentSpeed;


        transform[3] = transform[3] - velOfss;

        transform[3][1] = GetGame().SurfaceY(transform[3][0], transform[3][2]) + m_Settings.height;
        
        m_Plane.MoveInTime(transform, delta_time);
    }

    private void FallingTwoEnginesDead(float delta_time)
    {
        vector transform[4];
        m_Plane.GetTransform(transform);

        GetTransformBasedOnFallingPhase(transform, delta_time);
        
        m_Plane.MoveInTime(transform, delta_time);
    }

    private void GetTransformBasedOnFallingPhase(inout vector transform[4], float delta_time)
    {
        FallingPhase fallPhs = GetFallingPhase();

        vector velOfss = "0 0 0";
        vector r, f, u;
        float dAngle;

        switch (fallPhs)
        {
            case FallingPhase.NOSE_STALLING:
                    float tNorm = Math.Clamp(m_planeFlightState.m_PhaseTimeElapsed / NOSE_STALLING_PHASE_TIME, 0.0, 1.0);

                    float targetPitchRad = -(18.0 * Math.DEG2RAD);

                    dAngle = (targetPitchRad / NOSE_STALLING_PHASE_TIME) * delta_time;

                    r = transform[0];
                    f = transform[2];

                    r.Normalize();
                    r = (r - f * vector.Dot(r, f));
                    if (r.LengthSq() < 1e-6) r = f.Perpend();
                    r.Normalize();

                    f = vector.RotateAroundZeroRad(f, r, dAngle);
                    f.Normalize();

                    u = f * r;
                    if (u.LengthSq() < 1e-6) u = f.Perpend();
                    u.Normalize();

                    transform[0] = r;
                    transform[1] = u;
                    transform[2] = f;

                    float startSpeed = m_Settings.speedOneEngine * 0.5;
                    float endSpeed   = m_Settings.speedOneEngine * 1.2;
                    m_planeFlightState.m_CurrentSpeed = Math.Lerp(startSpeed, endSpeed, tNorm);

                    velOfss = transform[2] * delta_time * m_planeFlightState.m_CurrentSpeed;
                    transform[3] = transform[3] - velOfss;

                break;
            case FallingPhase.ACCELERATING:
                    float accel = 7.0;

                    m_planeFlightState.m_CurrentSpeed += accel * delta_time;

                    r = transform[0]; r.Normalize();
                    f = transform[2]; f.Normalize();
                    r = (r - f * vector.Dot(r, f));
                    if (r.LengthSq() < 1e-6) r = f.Perpend();
                    r.Normalize();
                    u = f * r;
                    u.Normalize();
                    transform[0] = r;
                    transform[1] = u;
                    transform[2] = f;

                    velOfss = transform[2] * delta_time * m_planeFlightState.m_CurrentSpeed;
                    transform[3] = transform[3] - velOfss;
                break;
            case FallingPhase.BARREL_ROLL:
                    float rollSpeed = Math.PI2 / 3.3;
                    dAngle = rollSpeed * delta_time;

                    f = transform[2];

                    m_planeFlightState.m_BarrelRollAngle += dAngle;
                    if (m_planeFlightState.m_BarrelRollAngle > Math.PI2)
                    {
                        m_planeFlightState.m_BarrelRollAngle = Math.PI2; 
                    }
                    else
                    {
                        f.Normalize();
                        r = vector.RotateAroundZeroRad(transform[0], f, dAngle);
                        u = vector.RotateAroundZeroRad(transform[1], f, dAngle);

                        r.Normalize();
                        u.Normalize();

                        transform[0] = r;
                        transform[1] = u;
                        transform[2] = f;
                    }
                    
                    velOfss = f * delta_time * m_planeFlightState.m_CurrentSpeed;
                    transform[3] = transform[3] - velOfss;
                break;
            case FallingPhase.LANDING:
                    float speenNormal = Math.Clamp(vector.Distance(m_Plane.GetPosition(), m_navigationData.m_CrashPos) / vector.Distance(m_planeFlightState.m_GroundHitPosition, m_navigationData.m_CrashPos), 0.0, 1.0);
                    Print("Normal - " + speenNormal);
				    vector pos = m_Plane.GetPosition();

				    vector targetUp = GetGame().SurfaceGetNormal(pos[0], pos[2]);
				    targetUp.Normalize();
				
				    float groundY = GetGame().SurfaceY(pos[0], pos[2]);
				
				    vector currentUp = transform[1];
				    currentUp.Normalize();
				
				    float smooth = 20 * delta_time; 
				    vector up = vector.Lerp(currentUp, targetUp, smooth);
				    up.Normalize();
				
				    vector dir = m_Plane.GetDirection();
				    dir.Normalize();
				    vector right = dir * up;
				    right.Normalize();
				    dir = up * right;
				    dir.Normalize();
				
				    float targetY = groundY + 6;
				    pos[1] = Math.Lerp(pos[1], targetY, smooth);
                    float speed = Math.Lerp(0, m_planeFlightState.m_SpeedAfterLand, speenNormal);
                    m_planeFlightState.m_CurrentSpeed = speed;
                    Print("Speed = " + m_planeFlightState.m_CurrentSpeed);
				
				    pos = pos - dir * (speed * delta_time);
				
				    transform[0] = right;
				    transform[1] = up;
				    transform[2] = dir;
				    transform[3] = pos;
			    break;
            default:
				
                break;
        }
    }

    private FallingPhase GetFallingPhase()
    {
        ChangeFallPhaseIfNeeded();
        return m_planeFlightState.m_fallPhase;
    }

    private void ChangeFallPhaseIfNeeded()
    {
        FallingPhase currPhase = m_planeFlightState.m_fallPhase;

        switch (currPhase)
        {
            case FallingPhase.NOSE_STALLING:
                if (m_planeFlightState.m_PhaseTimeElapsed >= NOSE_STALLING_PHASE_TIME)
                {
                    ChangeFallingPhase(FallingPhase.ACCELERATING);
                    Print("New phase ACCELERATING");
                }
                break;
            case FallingPhase.ACCELERATING:
                if (AtTheGround())
                {
                    ChangeFallingPhase(FallingPhase.BARREL_ROLL);
                    Print("New phase BARREL_ROLL");
                }
                break;
            case FallingPhase.BARREL_ROLL:
                if (TouchTheGround())
                {
                    m_planeFlightState.m_GroundHitPosition = m_Plane.GetPosition();
                    m_planeFlightState.m_SpeedAfterLand = m_planeFlightState.m_CurrentSpeed;
                    ChangeFallingPhase(FallingPhase.LANDING);
                    Print("New phase LANDING");
                    Print("Speed after land - " + m_planeFlightState.m_SpeedAfterLand);
                }
                break;
        }
    }

    private bool AtTheGround()
    {
        vector nosePos = m_Plane.ModelToWorld(m_Plane.GetMemoryPointPos("heightCheckPoint"));
        float heightAtNosePos = GetGame().SurfaceY(nosePos[0], nosePos[2]);
        return nosePos[1] - heightAtNosePos <= 90;
    }

    private bool TouchTheGround()
    {
        vector nosePos = m_Plane.ModelToWorld(m_Plane.GetMemoryPointPos("heightCheckPoint"));
        float heightAtNosePos = GetGame().SurfaceY(nosePos[0], nosePos[2]);
        return nosePos[1] - heightAtNosePos <= 0;
    }

    private bool CheckDistanceBetweenPoints(vector point, vector point2, float distance)
    {
        if (m_Plane)
		{
			vector temp1 = point;
			temp1[1] = 0;
			vector temp2 = point2;
			temp2[1] = 0;
			if (vector.Distance(temp1, temp2) > distance)
			{
				return true;
			}
			return false;
		}
		return false;
    }

    private void CheckIfFirstEngShouldFail()
    {
        if (m_Plane)
        {
            if (CheckDistanceBetweenPoints(m_navigationData.m_SpawnPosition, m_Plane.GetPosition(), m_navigationData.m_DistanceToFirstEngFailPos))
            {
                Print("Phase FIRST_ENGINE_FAILURE STARTED");
                SetEventStatus(PlaneStatus.FIRST_ENGINE_FAILURE);
            }
        }
    }

    private void ChangeFallingPhase(FallingPhase phase)
    {
        m_planeFlightState.m_fallPhase = phase;
        m_planeFlightState.m_PhaseTimeElapsed = 0;
    }

    private void SetEventStatus(PlaneStatus eventStatus)
    {
        m_planeFlightState.m_Status = eventStatus;
        m_Plane.SetPlaneStatus(eventStatus);
        m_planeFlightState.m_PhaseTimeElapsed = 0;
    }
	
	private bool HitSecondEngineFailPoint()
	{
		if (m_Plane)
		{
			vector temp1 = m_navigationData.m_SecondEngineFailPosition;
			temp1[1] = 0;
			vector temp2 = m_Plane.GetPosition();
			temp2[1] = 0;
			if (vector.Distance(temp1, temp2) < 30)
			{
                Print("Phase SECOND_ENGINE_FAILURE STARTED");
                SetEventStatus(PlaneStatus.SECOND_ENGINE_FAILURE);
				return true;
			}
			return false;
		}
		return false;
	}

    private bool HitEngineFailPoint()
	{
		if (m_Plane)
		{
			vector temp1 = m_navigationData.m_EngineFailPosition;
			temp1[1] = 0;
			vector temp2 = m_Plane.GetPosition();
			temp2[1] = 0;
			if (vector.Distance(temp1, temp2) < 30)
			{
                Print("Phase ENGINE_FAILURE STARTED");
                SetEventStatus(PlaneStatus.ENGINE_FAILURE);
                ChangeFallingPhase(FallingPhase.NOSE_STALLING);
				return true;
			}
			return false;
		}
		return false;
	}

    void End()
    {

    }
}