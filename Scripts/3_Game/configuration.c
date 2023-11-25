class GameLabsConfiguration {
    private bool debugEnabled = false;
    private bool connectionVerification = true;

    private string serverId = "";
    private string apiKey = "";

    private string baseUrl = "https://api.gamelabs.cloud/dz";
    private string storeUrl = "https://api.gamelabs.cloud/dz";

    private bool speedCheckDisabled = false;
    private float speedCheckThresholdFoot = 8.0;
    private float speedCheckThresholdVehicle = 100.0;

    private float playerTickInterval = 2.5;

    private bool preventDynamicItemPopulation = false;

    private bool chatSanitizeBattlEyeJoinLeave = false;
    private bool chatSanitizeBattlEyePrefix = false;
    private bool chatBlockEventProcessing = false;
    private bool advancedChatInterface = false;

    private bool requireASCIIItemNames = false;
    private bool filterASCIIItemNames = true;

    private bool enableMagicBulletCheck = false;
    private bool enableMagicBulletInvalidation = false;

    [NonSerialized()]
    private const string path = "$profile:gamelabs.cfg";

    void GameLabsConfiguration() {
        // TODO: Ensure configuration structure
    }

    bool CheckDiskPresence() {
        return FileExist(this.path);
    }

    void LoadFromDisk() {
        JsonFileLoader<GameLabsConfiguration>.JsonLoadFile(this.path, this);
        this._ValidateAndExtend();
    }

    float GetPlayerTickInterval() { return this.playerTickInterval; }

    bool GetSpeedCheckStatus() { return !this.speedCheckDisabled; }
    float GetSpeedCheckThresholdFoot() { return this.speedCheckThresholdFoot; }
    float GetSpeedCheckThresholdVehicle() { return this.speedCheckThresholdVehicle; }

    bool GetMagicBulletCheckEnabled() { return this.enableMagicBulletCheck; }
    void SetMagicBulletCheckEnabled(bool value) { this.enableMagicBulletCheck = value; }
    bool GetMagicBulletInvalidateEnabled() { return this.enableMagicBulletInvalidation; }
    void SetMagicBulletInvalidateEnabled(bool value) { this.enableMagicBulletInvalidation = value; }

    bool GetChatSanitizeBattlEyeJoinLeave() { return this.chatSanitizeBattlEyeJoinLeave; }
    bool GetChatSanitizeBattlEyePrefix() { return this.chatSanitizeBattlEyePrefix; }
    bool GetChatEventBlock() { return this.chatBlockEventProcessing; }
    bool GetChatInterfaceProcessing() { return this.advancedChatInterface; }

    bool GetItemNameASCIIRequired() { return this.requireASCIIItemNames; }
    bool GetItemNameASCIIFilter() { return this.filterASCIIItemNames; }

    string GetServerId() { return this.serverId; }
    string GetApiKey() { return this.apiKey; }
    string GetBaseURL() { return this.baseUrl; }
    string GetStoreURL() { return this.storeUrl; }
    bool GetConnectionVerificationStatus() { return this.connectionVerification; }
    bool GetDebugStatus() { return this.debugEnabled; }
    bool CanSendDynamicItemList() { return !this.preventDynamicItemPopulation; }
    void OverrideDebugStatus(bool debugEnabled) { this.debugEnabled = debugEnabled; }

    void _ValidateAndExtend() {
        if(this.playerTickInterval < 1) this.playerTickInterval = 2.5;
        if(this.speedCheckThresholdFoot <= 1) this.speedCheckThresholdFoot = 8.0;
        if(this.speedCheckThresholdVehicle <= 1) this.speedCheckThresholdVehicle = 100.0;
    }
};