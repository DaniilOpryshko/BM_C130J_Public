modded class PlayerBase
{
    override void OnJumpStart()
	{
		super.OnJumpStart();

        if ( GetGame().IsServer() )
        {
            
        }
	}
}