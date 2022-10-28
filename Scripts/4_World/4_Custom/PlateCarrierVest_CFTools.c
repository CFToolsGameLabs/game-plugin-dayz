class PlateCarrierVest_CFTools extends PlateCarrierVest {
    override bool CanPutInCargo(EntityAI parent) {
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