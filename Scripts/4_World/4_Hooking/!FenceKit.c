modded class FenceKit extends KitBase {
    override void OnPlacementComplete( Man player, vector position = "0 0 0", vector orientation = "0 0 0" ) {
        super.OnPlacementComplete(player, position, orientation);

        if(GetGame().IsServer()) {
            PlayerBase playerBase = PlayerBase.Cast(player);
            PlayerIdentity identity = playerBase.GetIdentity();
            string steam64 = identity.GetPlainId();

            vector playerPos				= player.GetPosition();
            array<Object> nearestObjects	= new array<Object>();

            GetGame().GetObjectsAtPosition(playerPos, 15.0, nearestObjects, null);

            Fence relatedObject, tmpObject;
            foreach (Object nearestObject : nearestObjects) {
                EntityAI ent = EntityAI.Cast(nearestObject);
                if(ent) {
                    if(ent.GetType() == "Fence") {
                        tmpObject = Fence.Cast(ent);
                        if(tmpObject.GL_GetStorage()) {
                            if(!tmpObject.GL_GetStorage().Available()) {
                                relatedObject = tmpObject;
                                break;
                            }
                        }
                    }
                }
            }

            if(relatedObject != NULL) {
                GetGameLabs().GetLogger().Debug(string.Format("FenceKit.OnPlacementComplete > relatedObject=%1; playerBase=%2; steam64=%3", relatedObject, playerBase, steam64));
                relatedObject.GL_SetSteam64(steam64);
                relatedObject.GL_GatherPersistentID();
                relatedObject.GL_Save(true);
            } else {
                GetGameLabs().GetLogger().Debug(string.Format("FenceKit.OnPlacementComplete > RELATED_OBJECT_NOT_FOUND; playerBase=%1; steam64=%2", playerBase, steam64));
            }
        }
    }
};