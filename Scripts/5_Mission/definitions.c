// /v1/player/connect
class _Callback_PlayerConnect : _Callback {
    override void OnError(int errorCode) {
        GetGameLabs().GetLogger().Error(string.Format("PlayerConnect errorCode(%1)", errorCode));
    };

    override void OnTimeout() {
        GetGameLabs().GetLogger().Error(string.Format("PlayerConnect timed out"));
    };

    override void OnSuccess(string data, int dataSize) {
        _Response_PlayerConnect response = new _Response_PlayerConnect(data);
        GetGameLabs().SetPlayerGamesessionId(response.steam64, response.gamesession_id);
        GetGameLabs().SetPlayerUpstreamIdentity(response.steam64, response.cftools_id);

        PlayerBase player = GetPlayerBySteam64(response.steam64);
        player.SetUpstreamIdentity(response.cftools_id);
        player.SetGamesessionId(response.gamesession_id);
        player.OnUpstreamIdentityReceived();

        ref GLPlayerStatistics playerStats = GetGameLabs().GetPlayerStatisticsByCFToolsId(response.cftools_id);
        playerStats.startingDistance = player.StatGet("dist");

        GetGameLabs().GetLogger().Debug(string.Format("Player<%1> received CFTools Id from API cftools_id=%2, gamesession_id=%3", player, response.cftools_id, response.gamesession_id));

        GameLabsClientSync clientSync = new GameLabsClientSync;
        clientSync.cftoolsId = response.cftools_id;
        clientSync.gameSessionId = response.gamesession_id;

        clientSync.chatSanitizeBattlEyeJoinLeave = GetGameLabs().GetConfiguration().GetChatSanitizeBattlEyeJoinLeave();
        clientSync.chatSanitizeBattlEyePrefix = GetGameLabs().GetConfiguration().GetChatSanitizeBattlEyePrefix();
        clientSync.chatBlockEventProcessing = GetGameLabs().GetConfiguration().GetChatEventBlock();
        clientSync.advancedChatInterface = GetGameLabs().GetConfiguration().GetChatInterfaceProcessing();

        clientSync.enableMagicBulletCheck = GetGameLabs().GetConfiguration().GetMagicBulletCheckEnabled();
        clientSync.enableMagicBulletInvalidation = GetGameLabs().GetConfiguration().GetMagicBulletInvalidateEnabled();

        Param2 <bool, ref GameLabsClientSync> payloadSync = new Param2<bool, ref GameLabsClientSync>(GetGameLabs().GetDebugStatus(), clientSync);
        GetGame().RPCSingleParam(null, GameLabsRPCS.RE_SYNC, payloadSync, true, player.GetIdentity());
    };
};
// ************************

// Register: /v1/player/disconnect
class _Callback_PlayerDisconnect : _Callback {
    override void OnError(int errorCode) {
        GetGameLabs().GetLogger().Error(string.Format("PlayerDisconnect errorCode(%1)", errorCode));
    };

    override void OnTimeout() {
        GetGameLabs().GetLogger().Error(string.Format("PlayerDisconnect timed out"));
    };

    override void OnSuccess(string data, int dataSize) {
        // TODO: Something here?
    };
};
class PlayerDisconnectStatistics {
    string distance;
    string playtime;
    string killedPlayers;
    string killedInfected;
    string hitLongestSurvivor;

    void PlayerDisconnectStatistics(PlayerBase player) {
        this.distance = player.StatGet("dist").ToString();
        this.playtime = player.StatGet("playtime").ToString();
        this.killedPlayers = player.StatGet("players_killed").ToString();
        this.killedInfected = player.StatGet("infected_killed").ToString();
        this.hitLongestSurvivor = player.StatGet("longest_survivor_hit").ToString();
    }
};
class _Payload_PlayerDisconnectEx : _Payload_PlayerDisconnect {
    string gamesession_id;
    vector position;

    ref PlayerDisconnectStatistics statistics;
    ref GLPlayerStatistics playerStatistics;

    void _Payload_PlayerDisconnectEx(string gamesession_id, PlayerBase player) {
        this.gamesession_id = gamesession_id;
        this.position = player.GetPosition();

        this.statistics = new PlayerDisconnectStatistics(player);

        if(player.HasUpstreamIdentity()) {
            string cftoolsId = player.GetUpstreamIdentity();
            ref GLPlayerStatistics pS = GetGameLabs().GetPlayerStatisticsByCFToolsId(cftoolsId);
            this.playerStatistics = pS;
            if(this.playerStatistics.startingDistance != 0 && player.StatGet("dist")) {
                this.playerStatistics.distance += (player.StatGet("dist") - this.playerStatistics.startingDistance);
            }

        }
    }
    override string ToJson() { return JsonFileLoader<_Payload_PlayerDisconnectEx>.JsonMakeData(this); }
};
// ************************

// Reporter internals

class _ServerPlayerEx : _ServerPlayer {
    void _ServerPlayerEx(PlayerBase player) {
        if(player != NULL) {
            this.id = player.GetPlainId(); // Steam64
            this.name = player.GetPlayerName();

            this.gamesessionId = player.GetUpstreamIdentity();

            this.position = player.GetPosition();
            this.health = player.GetHealth("GlobalHealth", "Health");
            if(GetGame().GetMission().IsPlayerDisconnecting(player))
                this.loggingOut = 1;

            if(player.IsInVehicle())
                this.insideVehicle = 1;

            if(player.IsUnconscious())
                this.unconscious = 1;
        }

        if(player.GetItemInHands())
            this.item = player.GetItemInHands().GetType();
    };
};

class _Callback_ServerDummy : _Callback {};

// Poll Protocols

// v1
class _Callback_ServerPoll : _Callback {
    override void OnError(int errorCode) {
        GetGameLabs().GetLogger().Error(string.Format("ServerPoll errorCode(%1)", errorCode));
        if(errorCode == 6 || errorCode == 5) {
            // Server side error, only thrown when either offline or authentication error
            GetGameLabs().GetLogger().Error(string.Format("GameLabs API error, reauthenticating..."));
            GetGameLabs().GetApi().RegisterAsync();
        }
    };

    override void OnTimeout() {
        GetGameLabs().GetLogger().Error(string.Format("ServerPoll timed out"));
    };

    override void OnSuccess(string data, int dataSize) {
        _Response_ServerPoll response = new _Response_ServerPoll(data);

        int q;
        Man man;
        vector position;
        PlayerBase player;
        ServerPollItem order;

        for ( int i = 0; i < response.orders.Count(); i++ ) {
            order = response.orders.Get(i);
            // TODO: Add new abstraction layer in 4_World
            if(order.action == "teleport") { //GetGameLabs()._TeleportPlayer(order.target, order.x, order.y);
                GetGameLabs().GetLogger().Warn(string.Format("[Order] Teleporting %1 to %2, %3", order.target, order.x, order.y));
                man = GetPlayerBySteam64(order.target);
                if(man != NULL) {
                    player = PlayerBase.Cast(man);

                    position[0] = order.x;
                    position[1] = GetGame().SurfaceY(order.x, order.y) + 0.2;
                    position[2] = order.y;
                    player.SetPositionEx(position);
                }
            } else if(order.action == "heal") {
                GetGameLabs().GetLogger().Warn(string.Format("[Order] Healing %1", order.target));
                // GetGameLabs()._KillPlayer(order.target);
                man = GetPlayerBySteam64(order.target);
                if(man != NULL) {
                    player = PlayerBase.Cast(man);

                    player.SetHealth(player.GetMaxHealth("", ""));
                    player.SetHealth("", "Blood", player.GetMaxHealth("", "Blood"));
                    player.GetStatHeatComfort().Set(player.GetStatHeatComfort().GetMax());
                    player.GetStatTremor().Set(player.GetStatTremor().GetMin());
                    player.GetStatWet().Set(player.GetStatWet().GetMin());
                    player.GetStatEnergy().Set(player.GetStatEnergy().GetMax());
                    player.GetStatWater().Set(player.GetStatWater().GetMax());
                    player.GetStatDiet().Set(player.GetStatDiet().GetMax());
                    player.GetStatSpecialty().Set(player.GetStatSpecialty().GetMax());
                    player.SetBleedingBits(0);
                }
            }
            else if(order.action == "kill") {
                GetGameLabs().GetLogger().Warn(string.Format("[Order] Killing %1", order.target));
                // GetGameLabs()._HealPlayer(order.target);
                man = GetPlayerBySteam64(order.target);
                if(man != NULL) {
                    player = PlayerBase.Cast(man);
                    player.SetHealth(0);
                }
            }
            else if(order.action == "spawn") {
                GetGameLabs().GetLogger().Warn(string.Format("[Order] Spawning %1 for %2 (x%3)", order.parameter, order.target, order.quantity));
                // GetGameLabs()._SpawnItemForPlayer(order.target, order.item);
                man = GetPlayerBySteam64(order.target);
                if(man != NULL) {
                    player = PlayerBase.Cast(man);
                    if(order.quantity <= 1) player.SpawnEntityOnGroundPos(order.parameter, player.GetPosition());
                    else {
                        for (q = 1; q <= order.quantity; q++) player.SpawnEntityOnGroundPos(order.parameter, player.GetPosition());
                    }
                }
            }
            else if(order.action == "spawnat") {
                position[0] = order.x;
                position[1] = GetGame().SurfaceY(order.x, order.y) + 0.1;
                position[2] = order.y;

                GetGameLabs().GetLogger().Warn(string.Format("[Order] Spawning %1 at %2 (x%3)", order.parameter, position, order.quantity));

                if(order.quantity <= 1) GetGame().CreateObjectEx(order.parameter, position, ECE_PLACE_ON_SURFACE);
                else {
                    for (q = 1; q <= order.quantity; q++) GetGame().CreateObjectEx(order.parameter, position, ECE_PLACE_ON_SURFACE);
                }
            }
        }
    };
};

class _Response_ServerPoll2 : _Response {
    ref array<GameLabsContextAction> orders;
    void _Response_ServerPoll2(string content) { JsonFileLoader<_Response_ServerPoll2>.JsonLoadData(content, this); }
};

class _Callback_ServerPoll2 : _Callback {
    override void OnError(int errorCode) {
        GetGameLabs().GetLogger().Error(string.Format("ServerPoll2 errorCode(%1)", errorCode));
        if(errorCode == 6 || errorCode == 5) {
            // Server side error, only thrown when either offline or authentication error
            GetGameLabs().GetLogger().Error(string.Format("GameLabs API error, reauthenticating..."));
            GetGameLabs().GetApi().RegisterAsync();
        }
    };

    override void OnTimeout() {
        GetGameLabs().GetLogger().Error(string.Format("ServerPoll2 timed out"));
    };

    override void OnSuccess(string data, int dataSize) {
        _Response_ServerPoll2 response = new _Response_ServerPoll2(data);

        Man man;
        PlayerBase player;

        vector position;

        bool orderStatus;
        bool requiresPlayer = false;

        GameLabsContextAction orderAction;
        GameLabsContextAction order;
        GameLabsActionContext context;
        Class referencedObject;
        for ( int i = 0; i < response.orders.Count(); i++ ) {
            order = response.orders.Get(i);

            GetGameLabs().GetLogger().Debug(string.Format("Processing order %1/%2 action=%3", i+1, response.orders.Count(), order.InfoString()));

            switch(order.actionContext) {
                case "player": {
                    referencedObject = GetPlayerBySteam64(order.referenceKey);
                    if(referencedObject == NULL) {
                        GetGameLabs().GetLogger().Warn(string.Format("Order requires PLAYER reference, but no player found by referenceKey=%1", order.referenceKey));
                    }
                    break;
                }
                case "vehicle": {
                    referencedObject = GetGameLabs().GetVehicle(order.referenceKey);
                    if(referencedObject == NULL) {
                        GetGameLabs().GetLogger().Warn(string.Format("Order requires VEHICLE reference, but no vehicle found by referenceKey=%1", order.referenceKey));
                    }
                    break;
                }
                case "world": {
                    referencedObject = GetGame().GetWorld();
                    break;
                }
                case "object": {
                    referencedObject = GetGameLabs().GetEvent(order.referenceKey);
                    if(referencedObject == NULL) {
                        GetGameLabs().GetLogger().Warn(string.Format("Order requires OBJECT/EVENT reference, but nothing found by referenceKey=%1", order.referenceKey));
                    }
                    break;
                }
                default: {
                    referencedObject = NULL;
                }
            }
            if(referencedObject != NULL) {
                context = new GameLabsActionContext(order.actionContext, referencedObject, order.parameters);

                orderAction = GetGameLabs().GetGameLabsAction(order.actionCode);

                if(order.actionContext == "object") {
                    if(order.actionContextFilter.Count() > 0) {
                        _Event eventInstance;
                        _Event.CastTo(eventInstance, context.GetReferencedObject());
                        if(order.actionContextFilter.Find(eventInstance.Ref().ClassName()) == -1) {
                            GetGameLabs().GetLogger().Warn(string.Format("Order requires OBJECT/EVENT reference of specific types, but %1 is not supported", eventInstance.Ref().ClassName()));
                            referencedObject = NULL;
                        }
                    }
                    if(referencedObject != NULL) orderStatus = orderAction.Execute(context);
                } else orderStatus = orderAction.Execute(context);

            } else {
                orderStatus = false;
            }

            GetGameLabs().GetLogger().Debug(string.Format("Order %1/%2 [action=%3] status=%4", i+1, response.orders.Count(), order.actionCode, orderStatus));

            string webhookUrl;
            foreach(string parameterName, ref GameLabsActionParameter parameterRef : order.parameters) {
                if(!webhookUrl) {
                    if(parameterRef.dataType == "webhook_url") {
                        if(parameterRef.GetString() && parameterRef.GetString().Length()) {
                            webhookUrl = parameterRef.GetString();
                        }
                    }
                }
            }

            if(orderStatus) {
                if(orderAction && orderAction.GetResponseSuccess()) {
                    if(webhookUrl) orderAction.GetResponseSuccess().SetWebHookURL(webhookUrl);
                    orderAction.GetResponseSuccess().Execute(orderStatus, context);
                }
            } else {
                GetGameLabs().GetLogger().Warn(string.Format("Order %1/%2 [action=%3] failed", i+1, response.orders.Count(), order.actionCode));

                if(orderAction && orderAction.GetResponseFailed()) {
                    if(webhookUrl) orderAction.GetResponseFailed().SetWebHookURL(webhookUrl);
                    orderAction.GetResponseFailed().Execute(orderStatus, context);
                }
            }
        }
    };
};
