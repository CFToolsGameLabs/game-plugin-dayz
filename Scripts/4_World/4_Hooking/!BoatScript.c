modded class BoatScript {
    private ref _Vehicle _registeredInstance = new _Vehicle(this, "fa-ship", "boat");
    void BoatScript() {
        if(!GetGameLabs()) return;
        if(!GetGameLabs().IsServer()) return;
        GetGameLabs().RegisterVehicle(this._registeredInstance);
        GetGameLabs().IncrVehicleCount();
        switch(this.GetType()) {
            /*
            // Example type difference
            case "Boat_01": {
                this._registeredInstance.SetVehicleType("other-boat");
                break;
            }
            */
            default: {};
        }
    }

    void ~BoatScript() {
        if(!GetGameLabs()) return;
        if(!GetGameLabs().IsServer()) return;
        if(this._registeredInstance) GetGameLabs().RemoveVehicle(this._registeredInstance);
        GetGameLabs().DecrVehicleCount();
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
            ref GLPlayerStatistics playerStatistics = GetGameLabs().GetPlayerStatisticsByCFToolsId(cftoolsId);
            playerStatistics.shotsHit++;
            playerStatistics.shotsHitVehicles++;
        }
    }
};