

class GameLabsRPC {
    void GameLabsRPC() {
        GetDayZGame().Event_OnRPC.Insert( OnRPC );
        GetGameLabs().GetLogger().Info(string.Format("RPC interface initiated and handler registered"));
    }

    void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx) {
        if(GetGame().IsServer()) HandleServerRPC(sender, target, rpc_type, ctx);
        else HandleClientRPC(sender, target, rpc_type, ctx);
    }

    private void HandleClientRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx) {
        PlayerBase player;
        switch(rpc_type) {
            case GameLabsRPCS.RE_SERVERFPS: {
                Param1<float> responseREServerFps = new Param1<float>(-1.0);
                ctx.Read(responseREServerFps);

                float serverFps = responseREServerFps.param1;
                GetGameLabs().SetServerFPS(serverFps);
                return;
            }
            case GameLabsRPCS.RE_SYNC: {
                player = PlayerBase.Cast(GetGame().GetPlayer());
                if(!player) return;

                Param2<bool, ref GameLabsClientSync> responseRESync = new Param2<bool, ref GameLabsClientSync>(false, null);
                if(ctx.Read(responseRESync)) {
                    GetGameLabs().GetLogger().OverrideDebugStatus(responseRESync.param1);
                    GetGameLabs().GetConfiguration().OverrideDebugStatus(responseRESync.param1);

                    if(responseRESync.param2 == NULL) {
                        GetGameLabs().GetLogger().Debug(string.Format("Sync response did not contain a valid sync object"));
                    } else {
                        GameLabsClientSync clientSync = responseRESync.param2;
                        player.SetUpstreamIdentity(clientSync.cftoolsId);
                        GetGameLabs().GetLogger().Debug(string.Format("Received upstream identity (cftoolsId=%1)", player.GetUpstreamIdentity()));
                        player.OnUpstreamIdentityReceived();

                        MissionGameplay mission = MissionGameplay.Cast(GetGame().GetMission());
                        mission.chatSanitizeBattlEyeJoinLeave = clientSync.chatSanitizeBattlEyeJoinLeave;
                        mission.chatSanitizeBattlEyePrefix = clientSync.chatSanitizeBattlEyePrefix;
                        mission.chatBlockEventProcessing = clientSync.chatBlockEventProcessing;
                        mission.advancedChatInterface = clientSync.advancedChatInterface;
                    }
                    player.OnGameLabsSync();
                } else {
                    GetGameLabs().GetLogger().Error(string.Format("Could not read from sync context"));
                }
                return;
            }
        }
    }

    PlayerBase GetPlayerByIdentity(PlayerIdentity identity) {
        array<Man> players = new array<Man>;
        GetGame().GetPlayers( players );

        for( int i = 0; i < players.Count(); i++) {
            PlayerBase player;
            Class.CastTo(player, players.Get(i));
            if(player) {
                if(player.GetPlainId() == identity.GetPlainId()) return player;
            }
        }
        return NULL;
    }

    private void HandleServerRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx) {
        PlayerBase player;
        switch(rpc_type) {
            case GameLabsRPCS.RQ_SERVERFPS: {
                Param1<float> payloadRQServerFps = new Param1<float>(GetGameLabs().GetServerFPS());
                GetGame().RPCSingleParam(null, GameLabsRPCS.RE_SERVERFPS, payloadRQServerFps, true, sender);
                return;
            }
            #ifdef EXPANSIONMODCORE
            case GameLabsRPCS.SY_EXPANSIONCHAT: {
                ChatMessageEventParams data;
                if(!ctx.Read(data)) return;
                if(!sender) return;

                player = GetPlayerByIdentity(sender);
                if(!player) return;

                string channel;
                if(data.param1 == ExpansionChatChannels.CCTransport) {
                    channel = "vehicle";
                } else if(data.param1 == ExpansionChatChannels.CCGlobal) {
                    channel = "side";
                } else if(data.param1 == ExpansionChatChannels.CCTeam){
                    channel = "team";
                } else {
                    channel = "external";
                }

                _LogPlayerEx logObjectPlayer = new _LogPlayerEx(player);
                _Payload_PlayerChat payloadExpansionChat = new _Payload_PlayerChat(logObjectPlayer, channel, data.param3);
                GetGameLabs().GetApi().PlayerChat(new _Callback(), payloadExpansionChat);
                return;
            }
            #else
            case GameLabsRPCS.SY_EXPANSIONCHAT: {
                ChatMessageEventParams data;
                if(!ctx.Read(data)) return;
                if(!sender) return;

                player = GetPlayerByIdentity(sender);
                if(!player) return;

                string channel;
                if(data.param1 == 64) {
                    channel = "vehicle";
                } else if(data.param1 == 128) {
                    channel = "side";
                } else if(data.param1 == 256){
                    channel = "team";
                } else {
                    channel = "external";
                }

                _LogPlayerEx logObjectPlayer = new _LogPlayerEx(player);
                _Payload_PlayerChat payloadExpansionChat = new _Payload_PlayerChat(logObjectPlayer, channel, data.param3);
                GetGameLabs().GetApi().PlayerChat(new _Callback(), payloadExpansionChat);
                return;
            }
            #endif
            /*
            case GameLabsRPCS.RP_HITINFO: {
                player = GetPlayerByIdentity(sender);
                if(!player) return;

                Param1<string> hitref = new Param1<string>("");
                ctx.Read(hitref);
                if(hitref.param1) {
                    GLClientHitInfo hitInfo = new GLClientHitInfo(hitref.param1);
                    player.AddHitToCache(hitInfo);
                }
            }
            */
        }
    }
};
