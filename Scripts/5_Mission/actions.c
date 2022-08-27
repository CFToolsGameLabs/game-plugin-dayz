class CFCloud_TeleportPlayer extends GameLabsContextAction {
        void CFCloud_TeleportPlayer() {
            this.actionCode = "CFCloud_TeleportPlayer";
            this.actionName = "Teleport player to target location";
            this.actionIcon = "map-marker-alt";
            this.actionColour = "default";
            this.actionContext = "player";

            this.parameters.Insert("vector", GameLabsActionParameter("Target location", "Target location as DayZ parsable vector", "string"));
        }

        bool Execute(GameLabsActionContext context) {
            PlayerBase player;
            PlayerBase.CastTo(player, context.GetReferencedObject());

            vector position = context.parameters.Get("vector").GetVector();

            GetGameLabs().GetLogger().Warn(string.Format("[Teleport] %1 position=%2", player, position));

            player.SetPositionEx(position);
            return true;
        }
};


class CFCloud_HealPlayer extends GameLabsContextAction {
        void CFCloud_HealPlayer() {
            this.actionCode = "CFCloud_HealPlayer";
            this.actionName = "Replenish player vitals";
            this.actionIcon = "heartbeat";
            this.actionColour = "default";
            this.actionContext = "player"
        }

        bool Execute(GameLabsActionContext context) {
            PlayerBase player;
            PlayerBase.CastTo(player, context.GetReferencedObject());

            GetGameLabs().GetLogger().Warn(string.Format("[Heal] %1", player));
            player.GLHealEx();
            return true;
        }
};


class CFCloud_KillPlayer extends GameLabsContextAction {
        void CFCloud_KillPlayer() {
            this.actionCode = "CFCloud_KillPlayer";
            this.actionName = "Kill player character";
            this.actionIcon = "skull";
            this.actionColour = "warning";
            this.actionContext = "player"
        }

        bool Execute(GameLabsActionContext context) {
            PlayerBase player;
            PlayerBase.CastTo(player, context.GetReferencedObject());

            GetGameLabs().GetLogger().Warn(string.Format("[Kill] %1", player));
            player.SetHealth(0);
            return true;
        }
};


class CFCloud_SpawnPlayerItem extends GameLabsContextAction {
        void CFCloud_SpawnPlayerItem() {
            this.actionCode = "CFCloud_SpawnPlayerItem";
            this.actionName = "Spawn an item at player position";
            this.actionIcon = "gem";
            this.actionColour = "default";
            this.actionContext = "player"

            this.parameters.Insert("item", GameLabsActionParameter("Item class name", "Class name of item to be spawned", "cf_itemlist"));
            this.parameters.Insert("quantity", GameLabsActionParameter("Quantity", "Amount if items to be spawned", "int"));
            this.parameters.Insert("debug", GameLabsActionParameter("Debug spawn", "Use debug spawn method to automatically populate specific items", "boolean"));
            this.parameters.Insert("stacked", GameLabsActionParameter("Stacked", "Spawn items as a stack (only works if item supports to be stacked)", "boolean"));
        }

        bool Execute(GameLabsActionContext context) {
            PlayerBase player;
            PlayerBase.CastTo(player, context.GetReferencedObject());

            GetGameLabs().GetLogger().Warn(string.Format("[Spawn] Spawning %1 (x%2) for %3", context.parameters.Get("item").GetString(), context.parameters.Get("quantity").GetInt(), player));

            EntityAI entity;
            if(context.parameters.Get("stacked").GetBoolean()) {
                entity = player.SpawnEntityOnGroundPos(context.parameters.Get("item").GetString(), player.GetPosition());
                if (context.parameters.Get("debug").GetBoolean()) {
                    entity.OnDebugSpawn();
                }
                ItemBase itemBase;
                ItemBase.CastTo(itemBase, entity);
                itemBase.SetQuantity(context.parameters.Get("quantity").GetInt());
            } else {
                for (int i = 1; i <= context.parameters.Get("quantity").GetInt(); i++) {
                    entity = player.SpawnEntityOnGroundPos(context.parameters.Get("item").GetString(), player.GetPosition());
                    if (context.parameters.Get("debug").GetBoolean()) {
                        entity.OnDebugSpawn();
                    }
                }
            }
            return true;
        }
};


class CFCloud_ExplodePlayer extends GameLabsContextAction {
        void CFCloud_ExplodePlayer() {
            this.actionCode = "CFCloud_ExplodePlayer";
            this.actionName = "Explode player";
            this.actionIcon = "bomb";
            this.actionColour = "danger";
            this.actionContext = "player";
        }

        bool Execute(GameLabsActionContext context) {
            PlayerBase player;
            PlayerBase.CastTo(player, context.GetReferencedObject());

            GetGameLabs().GetLogger().Warn(string.Format("[Player-Explode] %1", player));
            player.Explode(DT_EXPLOSION, "LandFuelFeed_Ammo");
            return true;
        }
};


class CFCloud_StripPlayer extends GameLabsContextAction {
        void CFCloud_StripPlayer() {
            this.actionCode = "CFCloud_StripPlayer";
            this.actionName = "Delete all player items";
            this.actionIcon = "tshirt";
            this.actionColour = "danger";
            this.actionContext = "player";
        }

        bool Execute(GameLabsActionContext context) {
            PlayerBase player;
            PlayerBase.CastTo(player, context.GetReferencedObject());

            GetGameLabs().GetLogger().Warn(string.Format("[Player-Strip] %1", player));
            player.RemoveAllItems();
            return true;
        }
};


class CFCloud_DeleteVehicle extends GameLabsContextAction {
        void CFCloud_DeleteVehicle() {
            this.actionCode = "CFCloud_DeleteVehicle";
            this.actionName = "Delete vehicle";
            this.actionIcon = "trash-alt";
            this.actionColour = "danger";
            this.actionContext = "vehicle";
        }

        bool Execute(GameLabsActionContext context) {
            _Vehicle vehicle;
            _Vehicle.CastTo(vehicle, context.GetReferencedObject());

            Car vehicleEntity;
            Car.CastTo(vehicleEntity, vehicle.Ref());

            GetGameLabs().GetLogger().Warn(string.Format("[Vehicle-Delete] %1", vehicleEntity));
            vehicleEntity.Delete();
            return true;
        }
};

// This is an example action for internal use, and it's not transmitted unless testing mode is locally enabled
class GameLabsInternal_DummyAction extends GameLabsContextAction {
        void GameLabsInternal_DummyAction() {
            this.actionCode = "GameLabsInternal_DummyAction";
            this.actionName = "Dummy action";
            this.actionIcon = "accessible-icon";
            this.actionColour = "danger";
            this.actionContext = "world";

            this.parameters.Insert("testInt", GameLabsActionParameter("Integer", "Description goes here", "int"));
            this.parameters.Insert("testString", GameLabsActionParameter("String", "Description goes here", "string"));
            this.parameters.Insert("testBool", GameLabsActionParameter("Boolean", "Description goes here", "boolean"));
        }

        bool Execute(GameLabsActionContext context) {
            GetGameLabs().GetLogger().Debug(string.Format("Dummy action called testInt=%1, testString=%2, testBool=%3", context.parameters.Get("testInt").GetInt(), context.parameters.Get("testString").GetString(), context.parameters.Get("testBool").GetBoolean()));
            return true;
        }
};

/*
 * TODO: Implement with new action system
 * bool _ProcessWeatherServer(PlayerBase player, vector position, _SP2OrderParams params) {
    GetGameLabs().GetLogger().Warn(string.Format("[Weather] Updating weather overcast=%1, fog=%2, rain=%3, wind=%4", params.overcast, params.fog, params.rain, params.wind));
    Weather weather = GetGame().GetWeather();
    if(!weather) return false;

    if(params.overcast && weather.GetOvercast()) weather.GetOvercast().Set(params.overcast[0], params.overcast[1], params.overcast[2]);
    if(params.fog && weather.GetFog()) weather.GetFog().Set(params.fog[0], params.fog[1], params.fog[2]);
    if(params.rain && weather.GetRain()) weather.GetRain().Set(params.rain[0], params.rain[1], params.rain[2]);
    if(params.wind) weather.SetWindSpeed(params.wind);

    return true;
};

bool _ProcessTimeServer(PlayerBase player, vector position, _SP2OrderParams params) {
    GetGameLabs().GetLogger().Warn(string.Format("[Time] Updating game time hour=%1, minute=%2", params.hour, params.minute));

    int year, month, day, hour, minute;
    GetGame().GetWorld().GetDate(year, month, day, hour, minute);
    GetGame().GetWorld().SetDate(year, month, day, params.hour, params.minute);

    return true;
};
 */