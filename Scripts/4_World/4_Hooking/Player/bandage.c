modded class ActionBandageSelf {
    override void OnEndServer(ActionData action_data) {
        super.OnEndServer(action_data);
        PlayerBase p = PlayerBase.Cast(action_data.m_Player);
        if (p) p.GL_IncBleedFixed();
    }
}

modded class ActionBandageTarget {
    override void OnEndServer(ActionData action_data) {
        super.OnEndServer(action_data);
        PlayerBase p = PlayerBase.Cast(action_data.m_Player);
        if (p) p.GL_IncBleedFixed();
    }
}
