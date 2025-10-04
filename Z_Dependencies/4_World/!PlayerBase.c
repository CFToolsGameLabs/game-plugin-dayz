modded class PlayerBase extends ManBase {
    override void EEKilled(Object killer) {
        super.EEKilled(killer);

        if(!GetGameLabs()) return;
        if(!GetGameLabs().IsServer()) return;

        PlayerBase killerPlayer = PlayerBase.Cast(killer);
        if(killerPlayer) {
            #ifdef EXPANSIONMODAI
            if(this.Expansion_IsAI()) {
                OnAIKilledGL(killerPlayer);
            }
            if(killerPlayer.Expansion_IsAI()) {
                OnAIKilledByGL(this);
            }
            #endif
        }
    }

    void OnAIKilledGL(PlayerBase killer) {
        PlayerIdentity identity = killer.GetIdentity();
        if(identity) {
            if(identity.GetPlainId()) {
                string steam64 = identity.GetPlainId();
                GLPlayerStatistics playerStatistics = GetGameLabs().GetPlayerStatisticsBySteam64(steam64);
                if(playerStatistics) {
                    playerStatistics.killsAI++;
                }
            }
        }
    }
    void OnAIKilledByGL(PlayerBase target) {
        PlayerIdentity identity = target.GetIdentity();
        if(identity) {
            if(identity.GetPlainId()) {
                string steam64 = identity.GetPlainId();
                GLPlayerStatistics playerStatistics = GetGameLabs().GetPlayerStatisticsBySteam64(steam64);
                if(playerStatistics) {
                    playerStatistics.deathsAI++;
                }
            }
        }
    }
};
