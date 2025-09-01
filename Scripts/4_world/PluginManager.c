modded class PluginManager
{
    override void Init()
    {
        super.Init();

        RegisterPlugin( "PlaneEventManager",							false, 	true );
    }
}