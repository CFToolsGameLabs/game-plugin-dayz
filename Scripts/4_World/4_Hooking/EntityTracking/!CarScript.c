modded class CarScript {
    private ref _Vehicle gl_registeredInstance;

    void CarScript() {
        if(!GetGameLabs()) return;
        if(!GetGameLabs().IsServer()) return;

        // Deferred init
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(this._InitGameLabs);
    }

    void ~CarScript() {
        if(!GetGameLabs()) return;
        if(!GetGameLabs().IsServer()) return;

        GetGameLabs().DecrVehicleCount();
        if(this.gl_registeredInstance) GetGameLabs().RemoveVehicle(this.gl_registeredInstance);
    }

    private void _InitGameLabs() {
        if(!GetGameLabs()) return;
        if(!GetGameLabs().IsServer()) return;

        GetGameLabs().IncrVehicleCount();

        this.gl_registeredInstance = new _Vehicle(this, "fa-car", "car");
        GetGameLabs().RegisterVehicle(this.gl_registeredInstance);
        switch(this.GetType()) {
            case "Truck_01_Base": {
                this.gl_registeredInstance.SetVehicleType("truck");
                break;
            }
            case "Truck_02": {
                this.gl_registeredInstance.SetVehicleType("truck");
                break;
            }
            default: {};
        }
    }

    override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef) {
        super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);

        if(damageType != DamageType.FIRE_ARM) return;

        PlayerBase player;
        if(source) {
            player = PlayerBase.Cast(source.GetHierarchyRootPlayer());
        }
        if(!player) return;

        if(player.HasUpstreamIdentity()) {
            string cftoolsId = player.GetUpstreamIdentity();
            GLPlayerStatistics playerStatistics = GetGameLabs().GetPlayerStatisticsByCFToolsId(cftoolsId);
            playerStatistics.shotsHit++;
            playerStatistics.shotsHitVehicles++;
        }
    }
};