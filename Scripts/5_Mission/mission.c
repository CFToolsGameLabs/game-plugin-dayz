modded class MissionServer {
    ScriptModule GLScript;

    private ref GameLabsCore gameLabs;

    private ref GameLabsRPC gameLabsRPC;
    private ref GameLabsReporter gameLabsReporter;

    static ref array<string> _testClients = {
        "76561198410213019",
        "76561198084367441"
    };

    override void OnEvent(EventType eventTypeId, Param params) {
        super.OnEvent(eventTypeId, params);
        if(eventTypeId == ClientNewEventTypeID) {
            m_player.GameLabs_MakeReady(m_player.GetIdentity().GetPlainId(), m_player.GetIdentity().GetName());
            this.PrivilegedEquip();
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

                GameLabsClientSync clientSync = new GameLabsClientSync;
                clientSync.cftoolsId = cftoolsId;
                clientSync.gameSessionId = GetGameLabs().GetPlayerGamesessionId(player.GetPlainId());
                clientSync.chatSanitizeBattlEyeJoinLeave = GetGameLabs().GetConfiguration().GetChatSanitizeBattlEyeJoinLeave();
                clientSync.chatSanitizeBattlEyePrefix = GetGameLabs().GetConfiguration().GetChatSanitizeBattlEyePrefix();
                clientSync.chatBlockEventProcessing = GetGameLabs().GetConfiguration().GetChatEventBlock();

                Param2 <bool, ref GameLabsClientSync> payloadSync = new Param2<bool, ref GameLabsClientSync>(GetGameLabs().GetDebugStatus(), clientSync);
                GetGame().RPCSingleParam(null, GameLabsRPCS.RE_SYNC, payloadSync, true, identity);
            }
        }

        super.InvokeOnConnect(player, identity);
    };

    override void PlayerDisconnected(PlayerBase player, PlayerIdentity identity, string uid) {
        if(GetGameLabs().IsServer() && player) {
            string gamesessionId;
            gamesessionId = GetGameLabs().GetPlayerGamesessionId(player.GetPlainId());
            if(gamesessionId) {
                GetGameLabs().ClearPlayerGamesessionId(player.GetPlainId());
                GetGameLabs().ClearPlayerUpstreamIdentity(player.GetPlainId());

                _Payload_PlayerDisconnectEx payloadPlayerDisconnect = new _Payload_PlayerDisconnectEx(gamesessionId, player);
                GetGameLabs().GetApi().PlayerDisconnect(new _Callback_PlayerDisconnect(), payloadPlayerDisconnect);

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
        CFCloud_ExplodePlayer().Register();
        CFCloud_StripPlayer().Register();

        // Vehicle
        CFCloud_DeleteVehicle().Register();

        // World
        CFCloud_WorldTime().Register();
        CFCloud_WorldWeather().Register();
        CFCloud_WorldWeatherSunny().Register();

        CFCloud_WorldWipeAI().Register();
        CFCloud_WorldWipeVehicles().Register();
        CFCloud_SpawnItemWorld().Register();

        // Object
        CFCloud_ObjectDelete().Register();
        CFCloud_TerritoryFlagClear().Register();

        // Other
        string tmp;
        if(GetGame().CommandlineGetParam("gamelabstesting", tmp)) {
            GameLabsInternal_DummyAction().Register();
            GameLabsInternal_DummyActionWithWebHook().Register();
        }
    }

    void MissionServer() {
        if(this.gameLabs != NULL) {
            this.gameLabs.GetLogger().Error("MissionServer attempted to initiate twice! Review mod list for faulty mod.");
            return;
        }

        this.GLActionRegisterHook();

        this.gameLabs = GetGameLabs();
        string shutdownHeader, shutdownTitle, shutdownContent, shutdownFooter;
        shutdownHeader = "************* GAME LABS *************";
        shutdownFooter = "*************************************";
        if(this.gameLabs.errorFlag) {
            shutdownTitle = "YOUR SERVER WAS FORCEFULLY CLOSED";
            shutdownContent = "You are missing a gamelabs.cfg, server can not start without it";
            Print(shutdownHeader); Print(shutdownTitle); Print(shutdownContent); Print(shutdownFooter);
            PrintToRPT(shutdownHeader); PrintToRPT(shutdownTitle); PrintToRPT(shutdownContent); PrintToRPT(shutdownFooter);
            GetGame().AdminLog(shutdownHeader); GetGame().AdminLog(shutdownTitle); GetGame().AdminLog(shutdownContent); GetGame().AdminLog(shutdownFooter);
            GetGame().RequestExit(1);
            return;
        } else {
            this.gameLabs.GetLogger().Debug("Loaded MissionServer");
        }

        RegisterResult apiRegisterResult = this.gameLabs.GetApi().Register();
        this.gameLabs.GetLogger().Debug(string.Format("API-Register status=%1, error=%2", apiRegisterResult.status, apiRegisterResult.error));
        if(apiRegisterResult.status == 2) { // Credentials OK
            if(this.gameLabs.errorFlag) { // Mod licensing error
                shutdownTitle = string.Format("Server not authorized to use %1", this.gameLabs.modLicensingOffender);
                shutdownContent = "Contact the mod author for details";
                this.gameLabs.GetLogger().Error(shutdownTitle);
                Print(shutdownHeader); Print(shutdownTitle); Print(shutdownContent); Print(shutdownFooter);
                PrintToRPT(shutdownHeader); PrintToRPT(shutdownTitle); PrintToRPT(shutdownContent); PrintToRPT(shutdownFooter);
                GetGame().AdminLog(shutdownHeader); GetGame().AdminLog(shutdownTitle); GetGame().AdminLog(shutdownContent); GetGame().AdminLog(shutdownFooter);
                GetGame().RequestExit(1);
                return;
            }
            this.gameLabs.GetApi().Enable();
            int apiStatus = this.gameLabs.GetApi().Verify();
            if(apiStatus == 1) {
                this.gameLabs.GetLogger().Info(string.Format("Server up (GameLabs v%1)", this.gameLabs.GetVersionIdentifier()));
                this._Setup();
            } else {
                shutdownTitle = string.Format("Failed to verify api registration (apiStatus=%1)", apiStatus);
                shutdownContent = "A race condition caused a GameLabs error, verify your server is booting in time";
                this.gameLabs.GetLogger().Error(shutdownTitle);
                Print(shutdownHeader); Print(shutdownTitle); Print(shutdownContent); Print(shutdownFooter);
                PrintToRPT(shutdownHeader); PrintToRPT(shutdownTitle); PrintToRPT(shutdownContent); PrintToRPT(shutdownFooter);
                GetGame().AdminLog(shutdownHeader); GetGame().AdminLog(shutdownTitle); GetGame().AdminLog(shutdownContent); GetGame().AdminLog(shutdownFooter);
                GetGame().RequestExit(1); // This should never happen
            }
        } else { // Unreachable
            this.gameLabs.GetApi().Disable();

            bool ignoreConnectionVerification = false;
            if(apiRegisterResult.error) {
                switch(apiRegisterResult.error) {
                    case "outdated": {
                        ignoreConnectionVerification = true; // Outdated versions may cause detrimental effects on server performance

                        shutdownTitle = "GAMELABS OUTDATED";
                        shutdownContent = string.Format("Your current installed GameLabs version (version=%1) is outdated", this.gameLabs.GetVersionIdentifier());
                        break;
                    }

                    case "invalid": {
                        shutdownTitle = "INTERNAL ERROR";
                        shutdownContent = "GameLabs API encountered an error while attempting to initiate the connection. Please contact CFTools Cloud support for more details.";
                        break;
                    }

                    case "bad-key": {
                        shutdownTitle = "INVALID CREDENTIALS";
                        shutdownContent = "The configured GameLabs API credentials do not match any of our records. Please review your configuration";
                        break;
                    }

                    case "bad-server": {
                        shutdownTitle = "BAD SERVER CONFIGURATION";
                        shutdownContent = "You are attempting to start GameLabs with API credentials of a different server. Startup is denied to ensure data integrity.";
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

            if(this.gameLabs.GetConnectionVerificationStatus() == false && !ignoreConnectionVerification) {
                this.gameLabs.GetLogger().Warn(string.Format("CONNECTION VERIFICATION DISABLED - SERVER WILL NOT EXIT"));
            } else {
                GetGame().RequestExit(1);
            }
            return;
        }

        if(GetGameLabs().GetConfiguration().GetSpeedCheckStatus()) {
            GetGameLabs().GetLogger().Info("Experimental speed check is enabled");
        }
    }

    void ~MissionServer() {
        if(this.gameLabsReporter) this.gameLabsReporter.Disable();
        if(this.gameLabs) this.gameLabs.Exit();
        if(this.gameLabs.GetLogger()) {
            this.gameLabs.GetLogger().Debug("Gracefully closing core logger");
            this.gameLabs.GetLogger().Close();
        }
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
                ItemBase item = ItemBase.Cast(m_player.GetItemInHands());
                if(item) {
                    m_player.DropItem(item);
                }
                item = ItemBase.Cast(m_player.GetHumanInventory().CreateInHands("Hoodie_CFTools"));
                item = ItemBase.Cast(m_player.GetHumanInventory().CreateInInventory("MilitaryBeret_CFTools"));
            }
            /* ************************************************************************ */
        }
    }

    private void _Setup() {
        this.gameLabsRPC = new GameLabsRPC();
        this.gameLabsReporter = new GameLabsReporter();
    }
};

modded class MissionGameplay extends MissionBase {
    private string name;
    private ref GameLabsCore gameLabs;
    private ref GameLabsClient gameLabsClient;

    private ref GameLabsRPC gameLabsRPC;

    bool chatSanitizeBattlEyeJoinLeave = false;
    bool chatSanitizeBattlEyePrefix = false;
    bool chatBlockEventProcessing = false;
    bool advancedChatInterface = false;

    void MissionGameplay() {
        if(this.gameLabs != NULL) {
            this.gameLabs.GetLogger().Error("MissionServer attempted to initiate twice! Review mod list for faulty mod.");
            return;
        }

        GetGame().GetPlayerName(this.name);
        this.gameLabs = GetGameLabs();
        this.gameLabsClient = new GameLabsClient();

        this.gameLabsRPC = new GameLabsRPC();
        this.gameLabs.GetLogger().Info("Loaded MissionGameplay");
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

        if(eventTypeId == ChatMessageEventTypeID) {
            if(chatParams == NULL) chatParams = ChatMessageEventParams.Cast(params);
            #ifdef EXPANSIONMODCORE
            if(chatParams.param1 <= CCBattlEye) return;
            #else
            if(chatParams.param1 < 64) return;
            #endif
            if(this.name != chatParams.param2) return;
            this.gameLabsClient.SyncExpansionChat(chatParams);
        }
    }
};
