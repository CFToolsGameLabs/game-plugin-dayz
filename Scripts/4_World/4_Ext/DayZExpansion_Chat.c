#ifdef EXPANSIONMODCHAT
modded class ExpansionGlobalChatModule: CF_ModuleWorld {
    //TODO: doesnt get inherited from methods for some reason
    PlayerBase GL_GetPlayerByIdentity(PlayerIdentity identity) {
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

    override void AddChatMessage_Server(PlayerIdentity sender, Object target, ParamsReadContext ctx, ExpansionChatMessageEventParams data) {
        super.AddChatMessage_Server(sender, target, ctx, data);

        if(!sender) return;
        PlayerBase player;
        player = GL_GetPlayerByIdentity(sender);
        if(!player) return;

        string channel;
        switch(data.param1) {
            // ExpansionChatChannels.CCDirect
            case 4: {
                channel = "direct"
                break;
            }
            // ExpansionChatChannels.CCTransport
            case 128: {
                channel = "vehicle";
                break;
            }
            // ExpansionChatChannels.CCGlobal
            case 256: {
                channel = "side";
            }
            // ExpansionChatChannels.CCTeam
            case 512: {
                channel = "team";
                break;
            }
            default: {
                channel = "external";
            }
        }
        string message = data.param3;

        _LogPlayerEx logObjectPlayer = new _LogPlayerEx(player);
        _Payload_PlayerChat payloadExpansionChat = new _Payload_PlayerChat(logObjectPlayer, channel, message);
        GetGameLabs().GetApi().PlayerChat(new _Callback(), payloadExpansionChat);
    }
};
#endif