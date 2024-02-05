modded class ZombieBase  {
    private ref _AI _registeredInstance = new _AI(this, true);

    void ZombieBase () {
        if(!GetGameLabs()) return;
        if(!GetGameLabs().IsServer()) return;
        GetGameLabs().IncrAICount();
        GetGameLabs().RegisterAI(this._registeredInstance);
    }

    void ~ZombieBase () {
        if(!GetGameLabs()) return;
        if(!GetGameLabs().IsServer()) return;
        GetGameLabs().DecrAICount();
        if(this._registeredInstance) GetGameLabs().RemoveAI(this._registeredInstance);
    }

    override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef) {
        super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);

        if(damageType != DamageType.FIRE_ARM) return;

        if( !IsAlive() ) return;

        PlayerBase player;
        if(source) {
            player = PlayerBase.Cast(source.GetHierarchyRootPlayer());
        }
        if(!player) return;

        if(player.HasUpstreamIdentity()) {
            string cftoolsId = player.GetUpstreamIdentity();
            ref GLPlayerStatistics playerStatistics = GetGameLabs().GetPlayerStatisticsByCFToolsId(cftoolsId);
            playerStatistics.shotsHit++;
            playerStatistics.shotsHitInfected++;
        }
    }
};