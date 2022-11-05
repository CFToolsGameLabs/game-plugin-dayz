class PlateCarrierVest_CFTools extends PlateCarrierVest {

    void PlateCarrierVest_CFTools() {
        string tmp;
        if(GetGameLabs().GetConfiguration().GetDebugStatus() && GetGame().CommandlineGetParam("gamelabstesting", tmp)) {
            this._isTestingEnvironment = true;
        }
    }

    override bool CanPutInCargo(EntityAI parent) {
        if(!this._isTestingEnvironment) return super.CanPutInCargo(parent);

        PlayerBase player;
        if(parent.GetHierarchyRootPlayer() && PlayerBase.CastTo(player, parent.GetHierarchyRootPlayer())) {
            if(!player.HasAnyIdentitySet()) return true;
            if(player._GLSIA()) return true;
            else {
                this.Delete();
                return false;
            }
        }
        return false;
    }

    override bool CanPutIntoHands(EntityAI parent) {
        if(!this._isTestingEnvironment) return super.CanPutIntoHands(parent);

        PlayerBase player;
        if(parent && PlayerBase.CastTo(player, parent)) {
            if(!player.HasAnyIdentitySet()) return true;
            if(player._GLSIA()) return true;
            else {
                this.Delete();
                return false;
            }
        }
        return false;
    }

    override bool CanPutAsAttachment(EntityAI parent) {
        if(!this._isTestingEnvironment) return super.CanPutAsAttachment(parent);

        PlayerBase player;
        if(parent && PlayerBase.CastTo(player, parent)) {
            if(!player.HasAnyIdentitySet()) return true;
            if(player._GLSIA()) return true;
            else {
                this.Delete();
                return false;
            }
        }
        return false;
    }

    override void OnDebugSpawn() {
        EntityAI entity;
        if (Class.CastTo(entity, this)) {
            entity.GetInventory().CreateInInventory( "PlateCarrierPouches_CFTools" );
            //entity.GetInventory().CreateInInventory( "PlateCarrierHolster_CFTools" );
        }
    }
};