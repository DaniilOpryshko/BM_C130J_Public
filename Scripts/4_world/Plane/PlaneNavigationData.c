class PlaneNavigationData
{
    vector m_SpawnPosition;
    vector m_PlaneOrientation;
    vector m_EngineFailPosition;
    float m_EngineFailHeightRelative;
    vector m_SpawnFlat;
    vector m_FailFlat;
    float m_DistanceToFailPos;
    float m_DistanceToFirstEngFailPos;
    vector m_Offset;
    vector m_SecondEngineFailPosition;
    vector m_CrashPos;


    void PlaneNavigationData(CrashLocation crashLocation, int spawnHeight)
    {
        CalculateOffset(crashLocation);
        CalculateSpawnLocation(crashLocation, spawnHeight);
        ConfigureEngineFailPosition(crashLocation);
        ConfigureSecondEngineFailPosition(crashLocation);
        m_PlaneOrientation = crashLocation.orientation;
        m_PlaneOrientation[0] = m_PlaneOrientation[0] + 180;
        m_SpawnFlat = Vector(m_SpawnPosition[0], 0, m_SpawnPosition[2]);
        m_FailFlat = Vector(m_EngineFailPosition[0], 0, m_EngineFailPosition[2]);

        m_DistanceToFailPos = vector.Distance(m_SpawnFlat, m_FailFlat);
        m_DistanceToFirstEngFailPos = m_DistanceToFailPos * 0.2;
        m_CrashPos = crashLocation.location;
    }

    private void CalculateOffset(CrashLocation crashLoc)
    {
        float dir = crashLoc.orientation[0];
        m_Offset = Vector( Math.Sin(Math.DEG2RAD * dir), 0, Math.Cos(Math.DEG2RAD * dir));
    }

    private void CalculateSpawnLocation(CrashLocation crashLoc, int height)
    {
        vector spawnPos = crashLoc.location - (m_Offset * 1500);
        spawnPos[1] = GetGame().SurfaceY(spawnPos[0], spawnPos[2]) + height;
        m_SpawnPosition = spawnPos;
    }

    private void ConfigureEngineFailPosition(CrashLocation crashLoc)
    {
        vector engFailPos = crashLoc.location - (m_Offset * 770);
        engFailPos[1] = crashLoc.location[1] + 300;
        m_EngineFailPosition = engFailPos;
        m_EngineFailHeightRelative = engFailPos[1] - GetGame().SurfaceY(m_EngineFailPosition[0], m_EngineFailPosition[2]);
    }

    private void ConfigureSecondEngineFailPosition(CrashLocation crashLoc)
    {
        vector engFailPos = crashLoc.location - (m_Offset * 900);
        engFailPos[1] = GetGame().SurfaceY(engFailPos[0], engFailPos[2]);
        m_SecondEngineFailPosition = engFailPos;
    }

}

class PlaneFlightState
{
    float m_CurrentSpeed;
    float m_NosePitch;
    float m_PhaseTimeElapsed;
    float m_TotalFlightTime;
    PlaneStatus m_Status;
    bool m_FirstEngFail;
    bool m_PhysicsInitiated;
    FallingPhase m_fallPhase;
    float m_BarrelRollAngle;
    vector m_GroundHitPosition;
    float m_SpeedAfterLand;

    void PlaneFlightState()
    {
        m_CurrentSpeed = 0;
        m_PhaseTimeElapsed = 0;
        m_TotalFlightTime = 0;
        m_NosePitch = 0;
        m_FirstEngFail = false;
        m_PhysicsInitiated = false;
    }
}

enum FallingPhase
{
    NOSE_STALLING = 0,
    ACCELERATING = 1,
    BARREL_ROLL = 2,
    LANDING = 3;
};