modded class ActionAttachToConstruction: ActionSingleUseBase {
    void ActionAttachToConstruction() {}

    override void OnExecuteServer(ActionData action_data) {
        super.OnExecuteServer(action_data);
        if(!GetGameLabs()) return;
        if(!GetGameLabs().IsServer()) return;

        ItemBase item = ItemBase.Cast(action_data.m_MainItem);
        if(!item) return;
        if(!item.IsExternalLockType()) return;

        PlayerBase playerBase = PlayerBase.Cast(action_data.m_Player);
        if(!playerBase) return;

        PlayerIdentity identity = playerBase.GetIdentity();
        if(!identity) return;
        string steam64 = identity.GetPlainId();

        CombinationLock combinationLock = CombinationLock.Cast(item);
        if(combinationLock) {
            combinationLock.GL_SetSteam64(steam64);
            combinationLock.GL_GatherPersistentID();
            combinationLock.GL_Save(true);
            GetGameLabs().GetLogger().Debug(string.Format("ActionAttachToConstruction.OnExecuteServer > relatedObject=%1; playerBase=%2; steam64=%3", combinationLock, playerBase, steam64));
            return;
        }

        DigitalCodeLock codeLock = DigitalCodeLock.Cast(item);
        if(codeLock) {
            if(codeLock.IsInherited(DigitalCodeLock_Bunker)) return;
            codeLock.GL_SetSteam64(steam64);
            codeLock.GL_GatherPersistentID();
            codeLock.GL_Save(true);
            GetGameLabs().GetLogger().Debug(string.Format("ActionAttachToConstruction.OnExecuteServer > relatedObject=%1; playerBase=%2; steam64=%3", codeLock, playerBase, steam64));
        }
    }
}
