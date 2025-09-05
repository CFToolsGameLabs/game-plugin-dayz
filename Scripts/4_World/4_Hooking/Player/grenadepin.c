modded class ActionPin {
    override void OnEndServer(ActionData action_data) {
        super.OnEndServer(action_data);
        PlayerBase p = PlayerBase.Cast(action_data.m_Player);
        Grenade_Base g = Grenade_Base.Cast(action_data.m_MainItem);
        if(p && g) p.GL_IncGrenadeUsed();
    }
}