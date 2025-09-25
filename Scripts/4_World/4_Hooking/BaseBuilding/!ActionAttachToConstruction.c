modded class ActionAttachToConstruction: ActionSingleUseBase {
    void ActionAttachToConstruction() {}

    override void OnExecuteServer(ActionData action_data) {
        super.OnExecuteServer(action_data);
        if(!GetGameLabs()) return;
        if(!GetGameLabs().IsServer()) return;

        ItemBase item = ItemBase.Cast(action_data.m_MainItem);
        if(!item.IsKindOf("CombinationLock")) return;

        CombinationLock gl_item = CombinationLock.Cast(item);
        if(!gl_item) return;

        PlayerBase playerBase = PlayerBase.Cast(action_data.m_Player);
        if(!playerBase) return;

        PlayerIdentity identity = playerBase.GetIdentity();
        string steam64 = identity.GetPlainId();
        gl_item.GL_SetSteam64(steam64);
        gl_item.GL_GatherPersistentID();
        gl_item.GL_Save(true);

        GetGameLabs().GetLogger().Debug(string.Format("ActionAttachToConstruction.OnExecuteServer > relatedObject=%1; playerBase=%2; steam64=%3", gl_item, playerBase, steam64));
    }
}