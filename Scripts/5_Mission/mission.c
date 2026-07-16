modded class MissionServer {
    ScriptModule GLScript;

    private ref GameLabsCore gameLabs;

    private ref GameLabsRPC gameLabsRPC;
    private ref GameLabsReporter gameLabsReporter;
    private ref GLReportManager gameLabsReportManager;

    private bool gl_bootstrapRetryScheduled = false;
    private int gl_bootstrapAttempts = 0;

    private bool gl_MissionLoaded = false;

    static ref array<string> _testClients = {
        "76561198410213019",
        "76561198084367441"
    };

    override void OnEvent(EventType eventTypeId, Param params) {
        super.OnEvent(eventTypeId, params);
        if(!GetGameLabs()) return;

        switch(eventTypeId) {
            case ClientNewEventTypeID: {
                if(m_player.GetIdentity()) {
                    string steam64 = m_player.GetIdentity().GetPlainId();
                    string name = m_player.GetIdentity().GetName();
                    m_player.GameLabs_MakeReady(steam64, name);
                    this.PrivilegedEquip();
                }
                break;
            }
            /*
             * // Causes null ref directly after respawn
            case ClientRespawnEventTypeID: {
                GLPlayerStatistics stats = m_player.GetGLPlayerStatistics();
                if(stats) {
                    stats.playerRespawns++;
                }
                break;
            }
            */
            case ProgressEventTypeID: {
                ProgressEventParams progressParams; // typedef Param3<int, float, string> ProgressEventParams;
                if(Class.CastTo(progressParams, params)) {
                    if(progressParams.param1 == PROGRESS_FINISH) {
                        if(!this.gl_MissionLoaded) {
                            this.gl_MissionLoaded = true;
                            GetGameLabs().HandleMissionLoaded();
                        }
                    }
                }
                break;
            }
            default: {
                break;
            }
        }
    };

    override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity) {
        if(GetGameLabs().IsServer() && player && identity) {
            player.GameLabs_OnConnect(identity.GetPlainId(), identity.GetName());

            string cftoolsId = GetGameLabs().GetPlayerUpstreamIdentity(player.GetPlainId());
            if(!cftoolsId) {
                GetGameLabs().GetLogger().Debug(string.Format("Player<%1> no cached CFTools Id for steam64=%2, contacting api", player, player.GetPlainId()));

                _Payload_PlayerConnect payloadPlayerConnect = new _Payload_PlayerConnect(player.GetPlainId(), player.GetPosition());
                GetGameLabs().GetApi().PlayerConnect(new _Callback_PlayerConnect(), payloadPlayerConnect);
            } else {
                GetGameLabs().GetLogger().Debug(string.Format("Player<%1> populated with CFTools Id from cache (%2)", player, cftoolsId));
                player.SetUpstreamIdentity(cftoolsId);
                player.SetGamesessionId(GetGameLabs().GetPlayerGamesessionId(player.GetPlainId()));
                player.OnUpstreamIdentityReceived();

                GLPlayerStatistics playerStats = GetGameLabs().GetPlayerStatisticsByCFToolsId(cftoolsId);
                playerStats.startingDistance = 0;

                GameLabsClientSync clientSync = new GameLabsClientSync;
                clientSync.cftoolsId = cftoolsId;
                clientSync.gameSessionId = GetGameLabs().GetPlayerGamesessionId(player.GetPlainId());

                clientSync.chatSanitizeBattlEyeJoinLeave = GetGameLabs().GetConfiguration().GetChatSanitizeBattlEyeJoinLeave();
                clientSync.chatSanitizeBattlEyePrefix = GetGameLabs().GetConfiguration().GetChatSanitizeBattlEyePrefix();
                clientSync.chatBlockEventProcessing = GetGameLabs().GetConfiguration().GetChatEventBlock();

                clientSync.enableMagicBulletCheck = GetGameLabs().GetConfiguration().GetMagicBulletCheckEnabled();
                clientSync.enableMagicBulletInvalidation = GetGameLabs().GetConfiguration().GetMagicBulletInvalidateEnabled();

                Param2 <bool, ref GameLabsClientSync> payloadSync = new Param2<bool, ref GameLabsClientSync>(GetGameLabs().GetDebugStatus(), clientSync);
                GetGame().RPCSingleParam(null, GameLabsRPCS.RE_SYNC, payloadSync, true, identity);
            }

            // Reporting config is static server config; push it on every connect
            // regardless of the identity resolution path taken above.
            GLSendReportConfig(identity);
        }

        super.InvokeOnConnect(player, identity);
    };

    override void PlayerDisconnected(PlayerBase player, PlayerIdentity identity, string uid) {
        if(GetGameLabs().IsServer() && player) {
            string gamesessionId;
            gamesessionId = GetGameLabs().GetPlayerGamesessionId(player.GetPlainId());
            if(gamesessionId) {
                _Payload_PlayerDisconnectEx payloadPlayerDisconnect = new _Payload_PlayerDisconnectEx(gamesessionId, player);
                GetGameLabs().GetApi().PlayerDisconnect(new _Callback_PlayerDisconnect(), payloadPlayerDisconnect);

                GetGameLabs().ClearPlayerGamesessionId(player.GetPlainId());
                GetGameLabs().ClearPlayerUpstreamIdentity(player.GetPlainId());

                GetGameLabs().GetLogger().Debug(string.Format("Player<%1> disconnected at %2", player, player.GetPosition()));

            } else {
                this.gameLabs.GetLogger().Error(string.Format("PlayerDisconnected<%1> fired for already disconnected player [steam64=%2]", player, player.GetPlainId()));
            }
        }

        super.PlayerDisconnected(player, identity, uid);
    }

    void GLActionRegisterHook() {
        // Internal GameLabs actions

        // Player
        CFCloud_TeleportPlayer().Register();
        CFCloud_HealPlayer().Register();
        CFCloud_KillPlayer().Register();
        CFCloud_SpawnPlayerItem().Register();
        CFCloud_StripPlayer().Register();
        CFCloud_ExplodePlayer().Register();
        CFCloud_SetPlayerVitals().Register();
        CFCloud_GivePlayerLoadout().Register();
        //CFCloud_PlayerDropItem().Register(); // TODO: not working

        // Vehicle
        CFCloud_DeleteVehicle().Register();
        CFCloud_KillVehicleEngine().Register();
        CFCloud_RefuelVehicle().Register();
        CFCloud_RepairVehicle().Register();
        //CFCloud_VehicleEjectDriver().Register(); // TODO: sync
        CFCloud_VehicleExplode().Register();
        CFCloud_UnstuckVehicle().Register();

        // World
        CFCloud_WorldTime().Register();
        CFCloud_WorldWeather().Register();
        CFCloud_WorldWeatherSunny().Register();

        CFCloud_WorldWipeAI().Register();
        CFCloud_WorldWipeVehicles().Register();
        CFCloud_SpawnItemWorld().Register();
        CFCloud_SpawnVehicle().Register();

        // Object
        CFCloud_ObjectDelete().Register();

        // Specific
        CFCloud_TerritoryFlagClear().Register();
        CFCloud_LockedContainerOpen().Register();
        CFCloud_ScientificBriefcaseOpen().Register();

        // Terrain specific
        string terrain = GL_GetTerrainName();
        if(terrain == "sakhal") {
            GetGameLabs().GetLogger().Debug("Registering 'sakhal' specific functions");
            CFCloud_SakhalBunkerTeleport().Register();
        }

        // Other
        string tmp;
        if(GetGame().CommandlineGetParam("gamelabstesting", tmp)) {
            GetGameLabs().GetLogger().Warn("Registering 'gamelabstesting' specific functions");
            GameLabsInternal_DummyAction().Register();
            GameLabsInternal_DummyActionWithWebHook().Register();
            GameLabsInternal_DumpPlayerPosition().Register();
            GameLabsInternal_RestartMission().Register();
            GameLabsInternal_DumpDebugParameters().Register();
        }
    }

    string GL_GetTerrainName() {
        string missionPath = GetDayZGame().GetMissionPath();

        TStringArray pathParts = new TStringArray;
        missionPath.Split("\\", pathParts);

        int index = pathParts.Count() - 2;
        string missionName = pathParts.Get(index);

        TStringArray missionNameParts = new TStringArray;
        missionName.Split(".", missionNameParts);

        string terrain = missionNameParts.Get(1);
        terrain.ToLower();
        return terrain;
    }

    void MissionServer() {
        if(this.gameLabs != NULL) {
            this.gameLabs.GetLogger().Error("MissionServer attempted to initiate twice! Review mod list for faulty mod.");
            return;
        }

        this.GLActionRegisterHook();

        this.gameLabs = GetGameLabs();

        if(GetGameLabs().GetConfiguration().GetLockOnStart()) {
            GetGame().ChatPlayer("#lock");
            GetGameLabs().GetLogger().Info("Server locked on start");
        }

        this.gameLabs.GetLogger().Debug("Loaded MissionServer");

        this._GLBootstrapAttempt();

        if(GetGameLabs().GetConfiguration().GetSpeedCheckStatus()) {
            GetGameLabs().GetLogger().Info("Experimental speed check is enabled");
        }
    }

    // Runs the GameLabs auth chain (Register -> Enable -> Verify -> _Setup).
    // On transient failures (unreachable, verify race, internal error) it keeps a
    // repeating retry timer alive so the reporter/RPC come online once the API recovers.
    // Genuine misconfiguration (bad credentials, mod licensing, outdated) stays fatal.
    private void _GLBootstrapAttempt() {
        // Already bootstrapped by an earlier attempt - nothing to do.
        if(this.gameLabsReporter) {
            this._GLStopBootstrapRetry();
            return;
        }

        // Credentials not configured yet (e.g. freshly written template).
        // Do not attempt registration and never stop the server - just leave
        // GameLabs disabled so the operator can fill in gamelabs.cfg and restart.
        if(!this.gameLabs.GetConfiguration().GetServerId() || !this.gameLabs.GetConfiguration().GetApiKey()) {
            this.gameLabs.GetApi().Disable();
            this.gameLabs.GetLogger().Warn("GameLabs serverId and/or apiKey are not configured. GameLabs will stay disabled while the server keeps running. Fill in gamelabs.cfg and restart to enable it.");
            this._GLStopBootstrapRetry(); // Empty credentials will not self-heal at runtime
            return;
        }

        this.gl_bootstrapAttempts++;

        string shutdownHeader, shutdownTitle, shutdownContent, shutdownFooter;
        shutdownHeader = "************* GAME LABS *************";
        shutdownFooter = "*************************************";

        RegisterResult apiRegisterResult = this.gameLabs.GetApi().Register();
        this.gameLabs.GetLogger().Debug(string.Format("API-Register status=%1, error=%2, attempt=%3", apiRegisterResult.status, apiRegisterResult.error, this.gl_bootstrapAttempts));
        if(apiRegisterResult.status == 2) { // Credentials OK
            if(this.gameLabs.errorFlag) { // Mod licensing error
                shutdownTitle = string.Format("Server not authorized to use %1", this.gameLabs.modLicensingOffender);
                shutdownContent = "Contact the mod author for details";
                this.gameLabs.GetLogger().Error(shutdownTitle);
                Print(shutdownHeader); Print(shutdownTitle); Print(shutdownContent); Print(shutdownFooter);
                PrintToRPT(shutdownHeader); PrintToRPT(shutdownTitle); PrintToRPT(shutdownContent); PrintToRPT(shutdownFooter);
                GetGame().AdminLog(shutdownHeader); GetGame().AdminLog(shutdownTitle); GetGame().AdminLog(shutdownContent); GetGame().AdminLog(shutdownFooter);
                this._GLStopBootstrapRetry(); // Fatal - do not retry
                GetGame().RequestExit(1);
                return;
            }
            this.gameLabs.GetApi().Enable();
            int apiStatus = this.gameLabs.GetApi().Verify();
            if(apiStatus == 1) {
                this.gameLabs.GetLogger().Info(string.Format("Server up (GameLabs v%1; terrain=%2;)", this.gameLabs.GetVersionIdentifier(), GL_GetTerrainName()));
                this._Setup();
                this._GLStopBootstrapRetry(); // Success - stop retrying
            } else {
                shutdownTitle = string.Format("Failed to verify api registration (apiStatus=%1)", apiStatus);
                shutdownContent = "A race condition caused a GameLabs error, verify your server is booting in time";
                this.gameLabs.GetLogger().Error(shutdownTitle);
                Print(shutdownHeader); Print(shutdownTitle); Print(shutdownContent); Print(shutdownFooter);
                PrintToRPT(shutdownHeader); PrintToRPT(shutdownTitle); PrintToRPT(shutdownContent); PrintToRPT(shutdownFooter);
                GetGame().AdminLog(shutdownHeader); GetGame().AdminLog(shutdownTitle); GetGame().AdminLog(shutdownContent); GetGame().AdminLog(shutdownFooter);
                // Transient/race condition - keep GameLabs disabled but never stop the server, and retry.
                this.gameLabs.GetApi().Disable();
                this.gameLabs.GetLogger().Warn("GameLabs remains disabled, but the server will continue running.");
                this._GLScheduleBootstrapRetry();
            }
        } else { // Registration did not return "Credentials OK"
            this.gameLabs.GetApi().Disable();

            // The server is only stopped when the credentials are actually invalid.
            // Any other condition (outdated, unreachable, internal error) leaves
            // GameLabs disabled while the server keeps running.
            bool credentialsInvalid = false;
            bool fatal = false;
            if(apiRegisterResult.error) {
                switch(apiRegisterResult.error) {
                    case "outdated": {
                        fatal = true;
                        shutdownTitle = "GAMELABS OUTDATED";
                        shutdownContent = string.Format("Your current installed GameLabs version (version=%1) is outdated", this.gameLabs.GetVersionIdentifier());
                        break;
                    }

                    case "invalid": {
                        credentialsInvalid = true;
                        fatal = true;
                        shutdownTitle = "INVALID CREDENTIALS";
                        shutdownContent = "The configured serverId does not exist or your configuration is outdated.";
                        break;
                    }

                    case "bad-key": {
                        credentialsInvalid = true;
                        fatal = true;
                        shutdownTitle = "INVALID CREDENTIALS";
                        shutdownContent = "The configured GameLabs API credentials do not match any of our records. Please review your configuration";
                        break;
                    }

                    case "bad-server": {
                        credentialsInvalid = true;
                        fatal = true;
                        shutdownTitle = "BAD SERVER CONFIGURATION";
                        shutdownContent = "You are attempting to start GameLabs with API credentials of a different server. Startup is denied to ensure data integrity.";
                        break;
                    }

                    default: {
                        shutdownTitle = "INTERNAL ERROR";
                        shutdownContent = "GameLabs API encountered an error while attempting to initiate the connection. Please contact CFTools Cloud support for more details.";
                        break;
                    }
                }
            } else if(apiRegisterResult.status == 0) {
                shutdownTitle = "GAMELABS API UNREACHABLE";
                shutdownContent = "GameLabs API is unreachable";
            }

            this.gameLabs.GetLogger().Error(shutdownContent);
            Print(shutdownHeader); Print(shutdownTitle); Print(shutdownContent); Print(shutdownFooter);
            PrintToRPT(shutdownHeader); PrintToRPT(shutdownTitle); PrintToRPT(shutdownContent); PrintToRPT(shutdownFooter);
            GetGame().AdminLog(shutdownHeader); GetGame().AdminLog(shutdownTitle); GetGame().AdminLog(shutdownContent); GetGame().AdminLog(shutdownFooter);

            if(credentialsInvalid && this.gameLabs.GetConnectionVerificationStatus() != false) {
                this._GLStopBootstrapRetry(); // Fatal - do not retry
                GetGame().RequestExit(1);
            } else if(fatal) {
                // Misconfiguration that cannot self-heal (invalid creds with verification off, or outdated).
                this._GLStopBootstrapRetry();
                this.gameLabs.GetLogger().Warn(string.Format("CONNECTION VERIFICATION DISABLED - SERVER WILL NOT EXIT"));
            } else {
                // Transient (unreachable / internal error) - keep the server running and retry.
                this.gameLabs.GetLogger().Warn("GameLabs remains disabled, but the server will continue running.");
                this._GLScheduleBootstrapRetry();
            }
        }
    }

    private void _GLScheduleBootstrapRetry() {
        if(this.gl_bootstrapRetryScheduled) return; // Already scheduled
        this.gameLabs.GetLogger().Warn("GameLabs bootstrap failed transiently, will retry every 30s until the API recovers.");
        // MissionServer does not extend Managed, so Timer.Run() cannot target it.
        // Use the call queue with a bound func, which repeats without a Managed target.
        this.gl_bootstrapRetryScheduled = true;
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this._GLBootstrapAttempt, 30000, true);
    }

    private void _GLStopBootstrapRetry() {
        if(this.gl_bootstrapRetryScheduled) {
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(this._GLBootstrapAttempt);
            this.gl_bootstrapRetryScheduled = false;
        }
    }

    void ~MissionServer() {
        this._GLStopBootstrapRetry();
        if(this.gameLabsReporter) this.gameLabsReporter.Disable();
        if(this.gameLabs) this.gameLabs.Exit();
        if(this.gameLabs.GetLogger()) {
            this.gameLabs.GetLogger().Debug("Gracefully closing core logger");
            this.gameLabs.GetLogger().Close();
        }
    }

    void GameLabsPostPrivilegedEquip(PlayerBase player) {
        // can be overriden
    }

    private void PrivilegedEquip() {
        if(m_player.GetPlainId() && this._testClients.Find(m_player.GetPlainId()) != -1) {
            this.gameLabs.GetLogger().Warn(string.Format("(Re-Spawn) Granting %1<name=%2;steam64=%3> CFTools staff equipment", m_player, m_player.GetPlayerName(), m_player.GetPlainId()));

            /* ***** Start server with -gamelabstesting parameter for test loadout ***** */
            string tmp;
            if(GetGameLabs().GetConfiguration().GetDebugStatus() && GetGame().CommandlineGetParam("gamelabstesting", tmp)) {
                m_player.RemoveAllItems();

                m_player.GetHumanInventory().CreateInInventory("MilitaryBeret_CFTools");
                m_player.GetHumanInventory().CreateInInventory("AviatorGlasses");
                m_player.GetHumanInventory().CreateInInventory("Hoodie_CFTools");
                m_player.GetHumanInventory().CreateInInventory("OMNOGloves_Gray");
                m_player.GetHumanInventory().CreateInInventory("CargoPants_Black");
                m_player.GetHumanInventory().CreateInInventory("CombatBoots_Black");

                //EntityAI bp = m_player.GetInventory().CreateInInventory("SmershBag");
                EntityAI belt = m_player.GetInventory().CreateInInventory("MilitaryBelt");
                EntityAI sheath = belt.GetInventory().CreateInInventory("NylonKnifeSheath");
                sheath.GetInventory().CreateInInventory("CombatKnife");

                EntityAI plate_carrier = m_player.GetHumanInventory().CreateInInventory("PlateCarrierVest_CFTools");
                plate_carrier.OnDebugSpawn();
                plate_carrier.GetInventory().CreateInInventory("Mag_FNX45_15Rnd");
                plate_carrier.GetInventory().CreateInInventory("Mag_FNX45_15Rnd");
                plate_carrier.GetInventory().CreateInInventory("Mag_FNX45_15Rnd");
                plate_carrier.GetInventory().CreateInInventory("Mag_FNX45_15Rnd");
                plate_carrier.GetInventory().CreateInInventory("Mag_FNX45_15Rnd");
                plate_carrier.GetInventory().CreateInInventory("Mag_FNX45_15Rnd");

                EntityAI weapon;
                EntityAI plate_carrier_holster = belt.GetInventory().CreateInInventory("PlateCarrierHolster_CFTools");
                weapon = plate_carrier_holster.GetInventory().CreateInInventory("FNX45");
                m_player.SetQuickBarEntityShortcut(weapon, 0, true);
                weapon.OnDebugSpawn();

                weapon = m_player.GetHumanInventory().CreateInInventory("HK416_CFTools");
                m_player.SetQuickBarEntityShortcut(weapon, 1, true);
                weapon.OnDebugSpawn();

                weapon = m_player.GetInventory().CreateInInventory("Scout_CFTools");
                m_player.SetQuickBarEntityShortcut(weapon, 0, true);
                weapon.OnDebugSpawn();

                /*
                // Scout
                weapon = m_player.GetInventory().CreateInInventory("Scout_CFTools");
                m_player.SetQuickBarEntityShortcut(weapon, 0, true);
                weapon.OnDebugSpawn();

                // Saiga
                weapon = m_player.GetInventory().CreateInInventory("Saiga_CFTools");
                m_player.SetQuickBarEntityShortcut(weapon, 1, true);
                weapon.OnDebugSpawn();
                */
            } else {
                if(GetGame().CommandlineGetParam("glequip", tmp)) {
                    ItemBase item = ItemBase.Cast(m_player.GetItemInHands());
                    if (item) {
                        m_player.DropItem(item);
                    }
                    item = ItemBase.Cast(m_player.GetHumanInventory().CreateInHands("Hoodie_CFTools"));
                    item = ItemBase.Cast(m_player.GetHumanInventory().CreateInInventory("MilitaryBeret_CFTools"));
                }
            }
            /* ************************************************************************ */
        }
        this.GameLabsPostPrivilegedEquip(m_player);
    }

    private void _Setup() {
        if(this.gameLabsReporter) return; // Already set up - never create a second reporter/RPC
        this.gameLabsRPC = new GameLabsRPC();
        this.gameLabsReporter = new GameLabsReporter();
        this.gameLabsReportManager = new GLReportManager();
    }

    GLReportManager GetGLReportManager() {
        return this.gameLabsReportManager;
    }

    override void OnMissionStart() {
        super.OnMissionStart();
        if(GetGameLabs()) {
            GetGameLabs().GetLogger().Debug(string.Format("MissionServer.OnMissionStart();"));
        }
    }
    override void OnGameplayDataHandlerLoad() {
        super.OnGameplayDataHandlerLoad();
        if(GetGameLabs()) {
            GetGameLabs().GetLogger().Debug(string.Format("MissionServer.OnGameplayDataHandlerLoad();"));
        }
    }
    override void OnInit() {
        super.OnInit();
        if(GetGameLabs()) {
            GetGameLabs().GetLogger().Debug(string.Format("MissionServer.OnInit();"));
        }
    }

};

modded class MissionGameplay extends MissionBase {
    private string gl_name;
    private ref GameLabsCore gameLabs;
    private ref GameLabsClient gameLabsClient;

    private ref GameLabsRPC gameLabsRPC;
    private ref GameLabsReportClient gameLabsReportClient;

    bool chatSanitizeBattlEyeJoinLeave = false;
    bool chatSanitizeBattlEyePrefix = false;
    bool chatBlockEventProcessing = false;
    bool advancedChatInterface = false;

    void MissionGameplay() {
        if(this.gameLabs != NULL) {
            this.gameLabs.GetLogger().Error("MissionServer attempted to initiate twice! Review mod list for faulty mod.");
            return;
        }

        GetGame().GetPlayerName(this.gl_name);
        this.gameLabs = GetGameLabs();
        this.gameLabsClient = new GameLabsClient();

        this.gameLabsRPC = new GameLabsRPC();
        this.gameLabsReportClient = new GameLabsReportClient();
        this.gameLabs.GetLogger().Info("Loaded MissionGameplay (Client)");
        this.gameLabs.GetLogger().Info(string.Format("Player name: %1", this.gl_name));
    }

    GameLabsReportClient GetGLReportClient() {
        return this.gameLabsReportClient;
    }

    override void OnUpdate(float timeslice) {
        super.OnUpdate(timeslice);
        if(this.gameLabsReportClient) this.gameLabsReportClient.OnUpdate();
    }

    void ~MissionGameplay() {
        if(this.gameLabs) this.gameLabs.Exit();
        if(this.gameLabsClient) this.gameLabsClient.Disable();
    }

    override void OnInit() {
        super.OnInit();
        this.gameLabs.GetLogger().Info("OnInit");
        this.gameLabsClient.StartSynchronization();
    }

    override void OnEvent(EventType eventTypeId, Param params) {
        ChatMessageEventParams chatParams;

        if(this.gameLabs == NULL || (this.gameLabs != NULL && this.gameLabs.GetConfiguration() == NULL)) {
            super.OnEvent(eventTypeId, params);
            return;
        }

        if(eventTypeId == ChatMessageEventTypeID) {
            chatParams = ChatMessageEventParams.Cast(params);

            // Old expansion stuff is deprecated
            /*
            #ifdef EXPANSIONMODCORE
            if(chatParams.param1 > CCBattlEye) {
                if(this.gl_name == chatParams.param2) {
                    this.gameLabsClient.SyncExpansionChat(chatParams);
                }
            }
            #else
            if(chatParams.param1 > 64) {
                if(this.gl_name == chatParams.param2) {
                    this.gameLabsClient.SyncExpansionChat(chatParams);
                }
            }
            #endif
            */

            if(m_LifeState == EPlayerStates.ALIVE) {
                int channel = chatParams.param1;
                string message = chatParams.param3;

                if(this.chatSanitizeBattlEyeJoinLeave) {
                    if(message.Contains("BattlEye: Admin Kick")) return;
                }

                if(this.chatSanitizeBattlEyePrefix) {
                    if(message.Contains("(Global) Admin:") || message.Contains("(Private) Admin:")) {
                        message.Replace("(Global) Admin:", "(Global)");
                        message.Replace("(Private) Admin:", "(Private)");
                        chatParams.param3 = message;
                    }
                }

                if(this.advancedChatInterface) {
                    if(channel & CCBattlEye) {
                        int prefix_position = message.IndexOf(m_Chat.MESSAGE_HANDLER_PREFIX);
                        if (prefix_position != -1) {
                            string messageType = message.Get(prefix_position + m_Chat.MESSAGE_HANDLER_PREFIX.Length());
                            if (messageType == "N") { // Notification
                                message = message.Substring(prefix_position + m_Chat.MESSAGE_HANDLER_PREFIX.Length() + 1, message.Length() - (prefix_position + m_Chat.MESSAGE_HANDLER_PREFIX.Length() + 1));
                                NotificationSystem.AddNotificationExtended(5.0, "Server", message);
                                return;
                            } else if (messageType == "C") { // Coloured
                                m_Chat.AddGameLabs(chatParams);
                                return;
                            } else if (messageType == "X") { // Customizable notification
                                string payload = message.Substring(prefix_position + m_Chat.MESSAGE_HANDLER_PREFIX.Length() + 1, message.Length() - (prefix_position + m_Chat.MESSAGE_HANDLER_PREFIX.Length() + 1));
                                float nDuration; string nIcon; string nTitle; string nText;
                                if (GameLabsUtil.ParseNotificationCommand(payload, nDuration, nIcon, nTitle, nText)) {
                                    NotificationSystem.AddNotificationExtended(nDuration, nTitle, nText, nIcon);
                                }
                                return;
                            }
                        }
                    }
                }
                
                if(this.chatBlockEventProcessing) {
                    m_Chat.Add(chatParams);
                } else {
                    super.OnEvent(ChatMessageEventTypeID, chatParams);
                }
                return;
            }
        }

        super.OnEvent(eventTypeId, params);
    }
};
