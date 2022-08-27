class GameLabsActions {
    private ref map<string, int> actionMap = new map<string, int>;

    map<string, int> GetMap() {
        return this.actionMap;
    }

    void RegisterAction(string appId, int flag) {
        Print(string.Format("[GameLabs] Registered action=%1 for app=%2", flag, appId));
        this.actionMap.Set(appId, flag);
    }
};

private static ref GameLabsActions g_GameLabsActions;
static ref GameLabsActions GetGameLabsActions() {
    if(!g_GameLabsActions) {
        g_GameLabsActions = GameLabsActions();
    }
    return g_GameLabsActions;
};


