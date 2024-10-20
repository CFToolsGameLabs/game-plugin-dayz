modded class TerritoryFlagKit extends KitBase {
    override void OnPlacementComplete( Man player, vector position = "0 0 0", vector orientation = "0 0 0" ) {
        super.OnPlacementComplete(player, position, orientation);

        if(GetGame().IsServer()) {
            PlayerBase playerBase = PlayerBase.Cast(player);
            PlayerIdentity identity = playerBase.GetIdentity();
            string steam64 = identity.GetPlainId();

            vector playerPos				= player.GetPosition();
            array<Object> nearestObjects	= new array<Object>();

            GetGame().GetObjectsAtPosition(playerPos, 10.0, nearestObjects, null);

            TerritoryFlag totem;
            foreach (Object nearestObject : nearestObjects) {
                EntityAI ent = EntityAI.Cast(nearestObject);
                if(ent) {
                    if(ent.GetType() == "TerritoryFlag") {
                        totem = TerritoryFlag.Cast(ent);
                        break;
                    }
                }
            }

            if(totem != NULL) {
                GetGameLabs().GetLogger().Debug(string.Format("TerritoryFlagKit.OnPlacementComplete > totem=%1; playerBase=%2; steam64=%3", totem, playerBase, steam64));
                totem.GL_SetSteam64(steam64);
                totem.GL_GatherPersistentID();
                totem.GL_Save(true);
            } else {
                GetGameLabs().GetLogger().Debug(string.Format("TerritoryFlagKit.OnPlacementComplete > TOTEM_NOT_FOUND; playerBase=%1; steam64=%2", playerBase, steam64));
            }
        }
    }
};