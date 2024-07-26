PlayerBase GLGetPlayerBySteam64(string steam64) {
    array<Man> players = new array<Man>;
    GetGame().GetWorld().GetPlayerList( players );

    PlayerBase player;
    for ( int i = 0; i < players.Count(); i++ ) {
        player = PlayerBase.Cast(players.Get(i));
        if(player.GetPlainId() == steam64) return player;
    }
    return NULL;
}

PlayerBase GLGetPlayerByIdentity(PlayerIdentity identity) {
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