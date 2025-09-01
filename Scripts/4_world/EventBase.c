class BmEventBase : Managed
{
    protected ref EventSettings m_Settings;

    void BmEventBase(EventSettings settings)
    {
        m_Settings = settings;
        OnInit();
    };

    void OnInit()
    {
        Configure();
    };

    void Configure();
    void Start();
    void OnUpdate(float delta_time);
    void End();
}