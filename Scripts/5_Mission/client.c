class GameLabsClient {
    private bool processReporting = true;
    ref Timer timerSync;

    void GameLabsClient() {
        GetGame().RPCSingleParam(NULL, GameLabsRPCS.RQ_SERVERCFG, NULL, true);
    }

    void StartSynchronization() {
        this.timerSync = new Timer(CALL_CATEGORY_SYSTEM);
        this.timerSync.Run(5, this, "clientSync", NULL, true);
    }

    void Disable() {
        this.processReporting = false;
        if(this.timerSync) this.timerSync.Stop();
        if(GetGameLabs()) {
            if(GetGameLabs().GetLogger()) {
                GetGameLabs().GetLogger().Info("(Client) Disabled and timers gracefully closed");
            }
        }
    }

    void SyncExpansionChat(ChatMessageEventParams params) {
        GetGame().RPCSingleParam(NULL, GameLabsRPCS.SY_EXPANSIONCHAT, params, true);
    }

    private void clientSync() {
        if(!this.processReporting) return;
        GetGameLabs().RequestServerFPS();
    }
};