// Client-side controller for in-game reporting. Holds the reporting config pushed
// by the server, the death context, and orchestrates the report dialog, the
// death-screen overlay button and the rebindable hotkey.
class GameLabsReportClient {
    private ref GameLabsReportConfig config;
    private ref GameLabsReportDeathContext deathContext;
    private ref GameLabsReportDialog dialog;
    private ref GameLabsReportDeathOverlay deathOverlay;

    void SetConfig(GameLabsReportConfig cfg) {
        this.config = cfg;
        if(GetGameLabs() && GetGameLabs().GetLogger()) {
            GetGameLabs().GetLogger().Info(string.Format("[Reporting] Config received (available=%1, deathScreen=%2, escapeMenu=%3)", cfg.available, cfg.deathScreen, cfg.escapeMenu));
        }
    }

    GameLabsReportConfig GetConfig() { return this.config; }

    bool IsAvailable() { return this.config != NULL && this.config.available; }
    bool IsDeathScreenEnabled() { return this.IsAvailable() && this.config.deathScreen; }
    bool IsEscapeMenuEnabled() { return this.IsAvailable() && this.config.escapeMenu; }

    void SetDeathContext(GameLabsReportDeathContext ctx) { this.deathContext = ctx; }
    void ClearDeathContext() { this.deathContext = NULL; }
    GameLabsReportDeathContext GetDeathContext() { return this.deathContext; }

    bool IsDialogOpen() { return this.dialog != NULL; }
    void OnDialogClosed() { this.dialog = NULL; }

    // Called from the RE_REPORTPLAYERS handler with the online player list.
    void OnReportPlayersReceived(array<ref GameLabsReportTarget> targets) {
        if(this.dialog) this.dialog.PopulateTargets(targets);
    }

    // Opens the report dialog. When element is not forced it is chosen from context:
    // dead + deathScreen enabled -> death report; otherwise escape-menu style report.
    void OpenReportDialog(string forcedElement = "") {
        if(!this.IsAvailable()) return;
        if(this.dialog) return;

        string element = forcedElement;
        if(element == "") {
            PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
            bool isDead = (player == NULL || !player.IsAlive());
            if(isDead && this.IsDeathScreenEnabled()) element = "deathScreen";
            else if(this.IsEscapeMenuEnabled()) element = "escapeMenu";
            else if(this.IsDeathScreenEnabled()) element = "deathScreen";
        }

        if(element == "deathScreen" && !this.IsDeathScreenEnabled()) return;
        if(element == "escapeMenu" && !this.IsEscapeMenuEnabled()) return;
        if(element == "") return;

        this.dialog = GameLabsReportDialog.CreateReportDialog(this, element, this.deathContext);
    }

    // Polled every frame from MissionGameplay.OnUpdate.
    void OnUpdate() {
        this.HandleHotkey();
        this.UpdateDeathScreenOverlay();
    }

    private void HandleHotkey() {
        if(!this.IsAvailable()) return;
        if(this.dialog) return;

        UAInput input = GetUApi().GetInputByName("UAGameLabsReport");
        if(input && input.LocalPress()) {
            this.OpenReportDialog();
        }
    }

    private void UpdateDeathScreenOverlay() {
        if(!this.IsDeathScreenEnabled()) {
            this._RemoveDeathOverlay();
            return;
        }

        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        bool dead = (player != NULL && !player.IsAlive());

        if(dead) {
            if(!this.deathOverlay && !this.dialog) {
                this.deathOverlay = GameLabsReportDeathOverlay.Create(this);
            }
        } else {
            this._RemoveDeathOverlay();
            this.ClearDeathContext();
        }
    }

    private void _RemoveDeathOverlay() {
        if(this.deathOverlay) {
            if(this.deathOverlay.GetRoot()) this.deathOverlay.GetRoot().Unlink();
            this.deathOverlay = NULL;
        }
    }
};
