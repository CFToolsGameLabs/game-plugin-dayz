#ifdef EXPANSIONMODCHAT
modded class ExpansionGlobalChatModule: CF_ModuleWorld {
    override void AddChatMessage_Server(PlayerIdentity sender, Object target, ParamsReadContext ctx, ExpansionChatMessageEventParams data) {
        super.AddChatMessage_Server(sender, target, ctx, data);

        if(!sender) return;
        PlayerBase player;
        player = GetPlayerByIdentity(sender);
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