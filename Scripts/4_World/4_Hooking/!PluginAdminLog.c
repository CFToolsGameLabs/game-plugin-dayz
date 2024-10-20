modded class PluginAdminLog extends PluginBase {
    override void OnContinouousAction(ActionData action_data) {
        super.OnContinouousAction(action_data);
        if(!GetGameLabs().IsStatReportingEnabled()) return;

        string item;
        string target;
        string action = action_data.m_Action.Type().ToString();
        if(!GetGameLabs().IsMonitoredAction(action)) {
            GetGameLabs().GetLogger().Debug(string.Format("action=%1 flagged as forbidden by API, not logging", action));
            return;
        } else {
            GetGameLabs().GetLogger().Debug(string.Format("Processing action=%1", action));
        }

        PlayerBase player = PlayerBase.Cast(action_data.m_Player);

        if(action_data.m_Target) {
            Object targetObject;
            Class.CastTo(targetObject, action_data.m_Target.GetObject());
            if(targetObject) {
                target = targetObject.GetType();
            } else {
                Class.CastTo(targetObject, action_data.m_Target.GetParent());
                if(targetObject) {
                    target = targetObject.GetType();
                }
            }
        }
        if(action_data.m_MainItem) {
            ItemBase itemBase = ItemBase.Cast(action_data.m_MainItem);
            if(itemBase) {
                item = itemBase.GetType();
            }
        }

        _LogPlayerEx logObjectPlayer = new _LogPlayerEx(player);
        _Payload_ItemInteract payload = new _Payload_ItemInteract(logObjectPlayer, item, target, action);
        GetGameLabs().GetApi().ItemInteract(new _Callback(), payload);
    };

    override void OnPlacementComplete(Man player, ItemBase item) {
        super.OnPlacementComplete(player, item);
        if(!GetGameLabs().IsStatReportingEnabled()) return;

        PlayerBase _player = PlayerBase.Cast(player);

        _LogPlayerEx logObjectPlayer = new _LogPlayerEx(_player);
        _Payload_ItemPlace payload = new _Payload_ItemPlace(logObjectPlayer, item.GetType());
        GetGameLabs().GetApi().ItemPlace(new _Callback(), payload);
    };
};