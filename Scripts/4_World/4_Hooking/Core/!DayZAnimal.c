modded class AnimalBase extends DayZAnimal  {
    private ref _AI gl_registeredInstance;

    void AnimalBase () {
        if(!GetGameLabs()) return;
        if(!GetGameLabs().IsServer()) return;

        // Deferred init
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(this._InitGameLabs);
    }

    void ~AnimalBase () {
        if(!GetGameLabs()) return;
        if(!GetGameLabs().IsServer()) return;

        GetGameLabs().DecrAnimalCount();
        if(this.gl_registeredInstance) GetGameLabs().RemoveAI(this.gl_registeredInstance);
    }

    private void _InitGameLabs() {
        if(!GetGameLabs()) return;
        if(!GetGameLabs().IsServer()) return;
        GetGameLabs().IncrAnimalCount();

        this.gl_registeredInstance = new _AI(this, false);
        GetGameLabs().RegisterAI(this.gl_registeredInstance);
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
            playerStatistics.shotsHitAnimals++;
        }
    }
};