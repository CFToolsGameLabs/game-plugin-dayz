class GameLabsCore {
    private const string modControlledVersionIdentifier = "1.821";

    private ref GameLabsAPI api;
    private ref GameLabsLogger logger;
    private ref GameLabsConfiguration configuration;

    private bool blockProcessing = false;
    private bool isServer = false;

    bool errorFlag = false;
    string modLicensingOffender;

    private ref map<string, string> gamesessionIdMap = new map<string, string>;
    private ref map<string, string> upstreamIdentityMap = new map<string, string>;

    private int _computedServerFps = 0;
    private int _computedAI = 0;
    private int _computedAnimals = 0;
    private int _computedVehicles = 0;

    private int _metricsInterval = 5;
    private int _reportingInterval = 10;
    private bool _reportingEnabled = false;
    private bool _reportStatistics = false;
    private ref array <ref string> _monitoredActions = new array<ref string>;
    private ref array <ref GameLabsContextAction> _gamelabsActions = new array<ref GameLabsContextAction>;

    private ref array<ref _AI> _serverAI = new array<ref _AI>;
    private ref array<ref _Event> _serverEvents = new array<ref _Event>;
    private ref array<ref _Vehicle> _serverVehicles = new array<ref _Vehicle>;

    ref array<ref _Event> _serverEventsBufferAdded = new array<ref _Event>;
    ref array<ref _Event> _serverEventsBufferRemoved = new array<ref _Event>;

    ref array<ref _Vehicle> _serverVehiclesBufferAdded = new array<ref _Vehicle>;
    ref array<ref _Vehicle> _serverVehiclesBufferRemoved = new array<ref _Vehicle>;

    void Exit() {
        this.blockProcessing = true;
        if(this.logger) this.logger.Info("(Core) Attempting graceful exit");
        this._serverAI.Clear();
        this._serverEvents.Clear();
        this._serverVehicles.Clear();
        this._serverEventsBufferAdded.Clear();
        this._serverEventsBufferRemoved.Clear();
        this._serverVehiclesBufferAdded.Clear();
        this._serverVehiclesBufferRemoved.Clear();
        if(this.logger) this.logger.Debug("(Core) All ref holding buffers cleared");
    }

    void OnStoreLoad() {
        // hehe
    }

    bool IsProcessingBlocked() {
        return this.blockProcessing;
    }

    bool IsServer() {
        return this.isServer;
    }

    void GameLabsCore() {
        // TODO: Complete bootstrapping
        this.isServer = GetGame().IsServer();
        this.configuration = GameLabsConfiguration();
        if(!this.configuration.CheckDiskPresence()) {
            // Shut down when no config is present on server
            if(GetGame().IsServer()) {
                this.errorFlag = true;
            } else {
                // Instance logger with logging disabled
                this.logger = GameLabsLogger("GameLabsCore", false);
            }
        } else {
            this.configuration.LoadFromDisk();
            this.logger = GameLabsLogger("GameLabsCore", this.GetDebugStatus());

            this.api = GameLabsAPI(this.configuration.GetServerId(),this.configuration.GetApiKey(),this.configuration.GetBaseURL(),this.configuration.GetStoreURL());

            // TODO: Move this somewhere else in the future
            this.logger.Debug(string.Format("baseUrl=%1", this.configuration.GetBaseURL()));
            if(this.configuration.GetBaseURL() != "https://api.cftools.cloud/gamelabs/dz" && this.configuration.GetBaseURL() != "") {
                this.logger.Warn(string.Format("API Base URL has been modified, your data may get compromised! (%1)", this.configuration.GetBaseURL()));
            }

            this.logger.Debug(string.Format("storeUrl=%1", this.configuration.GetStoreURL()));
            if(this.configuration.GetStoreURL() != "https://api.cftools.cloud/gamelabs/dz" && this.configuration.GetStoreURL() != "") {
                this.logger.Warn(string.Format("API Store URL has been modified, your data may get compromised! (%1)", this.configuration.GetStoreURL()));
            }

            if(!this.configuration.GetBaseURL().Contains("https://api.cftools.")) {
                GetGame().RequestExit(1);
            }
        }
    }

    string GetVersionIdentifier() { return this.modControlledVersionIdentifier; }

    // Module getters
    GameLabsAPI GetApi() { return this.api; }
    GameLabsLogger GetLogger() { return this.logger; }
    GameLabsConfiguration GetConfiguration() { return this.configuration; }

    // Config getter passthrough
    bool GetDebugStatus() { return this.configuration.GetDebugStatus(); }
    bool GetConnectionVerificationStatus() { return this.configuration.GetConnectionVerificationStatus(); }

    /* Public API */
    void RequestServerFPS() {
        if(GetGame().IsServer()) return;
        GetGame().RPCSingleParam(NULL, GameLabsRPCS.RQ_SERVERFPS, NULL, true);
    }

    void SetPlayerUpstreamIdentity(string steam64, string cftoolsId) {
        if(!this.IsServer()) return;
        this.upstreamIdentityMap.Set(steam64, cftoolsId);
    }
    void ClearPlayerUpstreamIdentity(string steam64) {
        if(!this.IsServer()) return;
        this.upstreamIdentityMap.Remove(steam64);
    }
    string GetPlayerUpstreamIdentity(string steam64) {
        if(!this.IsServer()) return "";
        return this.upstreamIdentityMap.Get(steam64);
    }

    void SetPlayerGamesessionId(string steam64, string gamesessionId) {
        if(!this.IsServer()) return;
        this.gamesessionIdMap.Set(steam64, gamesessionId);
    }
    void ClearPlayerGamesessionId(string steam64) {
        if(!this.IsServer()) return;
        this.gamesessionIdMap.Remove(steam64);
    }
    string GetPlayerGamesessionId(string steam64) {
        if(!this.IsServer()) return "";
        return this.gamesessionIdMap.Get(steam64);
    }

    int GetServerFPS() { return this._computedServerFps; }
    void SetServerFPS(int fpsValue) { this._computedServerFps = fpsValue; }

    int GetAICount() { return this._computedAI; }
    void IncrAICount() { this._computedAI++; }
    void DecrAICount() { this._computedAI--; }
    int GetAIActiveCount() {
        int count = 0;
        _AI obj;
        for(int i = 0; i < this._serverAI.Count(); i++) {
            obj = this._serverAI.Get(i);
            if(obj.IsInfected() && obj.IsActive()) count++;
        }
        return count;
    }

    int GetAnimalCount() { return this._computedAnimals; }
    void IncrAnimalCount() { this._computedAnimals++; }
    void DecrAnimalCount() { this._computedAnimals--; }
    int GetAnimalActiveCount() {
        int count = 0;
        _AI obj;
        for(int i = 0; i < this._serverAI.Count(); i++) {
            obj = this._serverAI.Get(i);
            if(!obj.IsInfected() && obj.IsActive()) count++;
        }
        return count;
    }

    int GetVehicleCount() { return this._computedVehicles; }
    void IncrVehicleCount() { this._computedVehicles++; }
    void DecrVehicleCount() { this._computedVehicles--; }

    array<ref _AI> GetAI() {
        return this._serverAI;
    }
    void RegisterAI(_AI _reference) {
        if(IsProcessingBlocked()) return;
        this._serverAI.Insert(_reference);
    }
    void RemoveAI(_AI _reference) {
        if(IsProcessingBlocked()) return;
        for(int i = 0; i < this._serverAI.Count(); i++) {
            if(this._serverAI.Get(i) == _reference) {
                this._serverAI.Remove(i);
            }
        }
    }
    void ClearAI() {
        if(!GetGame().IsServer()) return;
        for(int i = 0; i < this._serverAI.Count(); i++) {
            this._serverAI.Get(i).Ref().Delete();
        }
    }


    array<ref _Vehicle> GetVehicles() {
        return this._serverVehicles;
    }
    void RegisterVehicle(_Vehicle _reference) {
        if(IsProcessingBlocked()) return;
        this._serverVehicles.Insert(_reference);
        this._serverVehiclesBufferAdded.Insert(_reference);
    }
    void RemoveVehicle(_Vehicle _reference) {
        if(IsProcessingBlocked()) return;
        for(int i = 0; i < this._serverVehicles.Count(); i++) {
            if(this._serverVehicles.Get(i) == _reference) {
                this._serverVehiclesBufferRemoved.Insert(_reference);
                this._serverVehicles.Remove(i);
            }
        }
    }
    _Vehicle GetVehicle(string referenceKey) {
        for(int i = 0; i < this._serverVehicles.Count(); i++) {
            if(this._serverVehicles.Get(i).ToString() == referenceKey) {
                return this._serverVehicles.Get(i);
            }
        }
        return NULL;
    }
    void ClearVehicles() {
        if(!GetGame().IsServer()) return;
        for(int i = 0; i < this._serverVehicles.Count(); i++) {
            this._serverVehicles.Get(i).Ref().Delete();
        }
    }

    array<ref _Event> GetEvents() {
        return this._serverEvents;
    }
    void RegisterEvent(_Event _reference) {
        if(IsProcessingBlocked()) return;
        for(int i = 0; i < this._serverEvents.Count(); i++) {
            if(this._serverEvents.Get(i).Equals(_reference)) {
                return;
            }
        }
        this._serverEvents.Insert(_reference);
        this._serverEventsBufferAdded.Insert(_reference);
    }
    void RemoveEvent(_Event _reference) {
        if(IsProcessingBlocked()) return;
        for(int i = 0; i < this._serverEvents.Count(); i++) {
            if(this._serverEvents.Get(i) == _reference) {
                this._serverEventsBufferRemoved.Insert(this._serverEvents.Get(i));
                this._serverEvents.Remove(i);
            }
        }
    }
    _Event GetEvent(string referenceKey) {
        for(int i = 0; i < this._serverEvents.Count(); i++) {
            if(this._serverEvents.Get(i).ToString() == referenceKey) {
                return this._serverEvents.Get(i);
            }
        }
        return NULL;
    }

    int GetMetricsInterval() { return this._metricsInterval; }
    int GetReportingInterval() { return this._reportingInterval; }
    bool IsReportingEnabled() { return this._reportingEnabled; }
    bool IsStatReportingEnabled() { return this._reportStatistics; }
    bool IsMonitoredAction(string action) { if(this._monitoredActions.Find(action) == -1) {return false;} else {return true;}  }

    void AddMonitoredAction(string action) {
        if(this.IsMonitoredAction(action)) return;
        this._monitoredActions.Insert(action);
        this.GetLogger().Warn(string.Format("\"%1\" has been added as monitored action by a third party application", action));
    }

    void AddGameLabsAction(ref GameLabsContextAction action) {
        this._gamelabsActions.Insert(action);
    }

    array <ref GameLabsContextAction> GetGameLabsActions() {
        return this._gamelabsActions;
    }

    GameLabsContextAction GetGameLabsAction(string actionCode) {
        for(int i = 0; i < this._gamelabsActions.Count(); i++) {
            if (this._gamelabsActions.Get(i).actionCode == actionCode) {
                return this._gamelabsActions.Get(i);
            }
        }
        return NULL;
    }

    // Private calls, do not access
    void _PropagateFeatures(_Response_Register registerResponse) {
        if(registerResponse.modLicensingStatus == 1) {
            this.errorFlag = true;
            this.modLicensingOffender = registerResponse.modLicensingOffender;
            this.GetLogger().Error(string.Format("Server is not authorized to use %1, contact mod author for details", this.modLicensingOffender));
            return;
        }

        this._monitoredActions = registerResponse.monitoredActions;

        this._metricsInterval = registerResponse.features.metricsInterval;
        this._reportingInterval = registerResponse.features.reportingInterval;

        if(registerResponse.features.allowReporting == 2) {
            this.GetLogger().Debug("allowReporting = 2 => Reporting enabled");
            this._reportingEnabled = true;
        } else {
            this.GetLogger().Debug("allowReporting != 2 => Reporting NOT enabled");
            this._reportingEnabled = false;
        }
        if(registerResponse.features.statisticsReporting == 2) {
            this.GetLogger().Debug("statisticsReporting = 2 => Statistics enabled");
            this._reportStatistics = true;
        } else {
            this.GetLogger().Debug("statisticsReporting != 2 => Statistics NOT enabled");
            this._reportStatistics = false;
        }
    }
};

// Public interface access
private static ref GameLabsCore g_GameLabsCore;
static ref GameLabsCore GetGameLabs() {
    if(!g_GameLabsCore) {
        g_GameLabsCore = GameLabsCore();
    }
    return g_GameLabsCore;
};