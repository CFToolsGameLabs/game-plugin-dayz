/* Player actions */
class CFCloud_TeleportPlayer extends GameLabsContextAction {
        void CFCloud_TeleportPlayer() {
            this.actionCode = "CFCloud_TeleportPlayer";
            this.actionName = "Teleport player to target location";
            this.actionIcon = "map-marker-alt";
            this.actionColour = "default";
            this.actionContext = "player";

            this.parameters.Insert("vector", GameLabsActionParameter("Target location", "Target location as DayZ parsable vector", "vector"));
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

/* Vehicle Actions */

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

/* World Actions */

class CFCloud_WorldTime extends GameLabsContextAction {
        void CFCloud_WorldTime() {
            this.actionCode = "CFCloud_WorldTime";
            this.actionName = "Update world time";
            this.actionIcon = "clock";
            this.actionColour = "default";
            this.actionContext = "world";

            // Set default value for parameter
            GameLabsActionParameter hour = new GameLabsActionParameter("Hour", "", "int");
            hour.valueInt = 12;
            this.parameters.Insert("hour", hour);

            this.parameters.Insert("minute", GameLabsActionParameter("Minute", "", "int"));
        }

        bool Execute(GameLabsActionContext context) {
            GetGameLabs().GetLogger().Warn(string.Format("[Time] Updating game time hour=%1, minute=%2", context.parameters.Get("hour").GetInt(), context.parameters.Get("minute").GetInt()));

            int year, month, day, hour, minute;
            GetGame().GetWorld().GetDate(year, month, day, hour, minute);
            GetGame().GetWorld().SetDate(year, month, day, context.parameters.Get("hour").GetInt(), context.parameters.Get("minute").GetInt());
            return true;
        }
};

class CFCloud_WorldWeather extends GameLabsContextAction {
        void CFCloud_WorldWeather() {
            this.actionCode = "CFCloud_WorldWeather";
            this.actionName = "Update weather";
            this.actionIcon = "clouds-sun";
            this.actionColour = "default";
            this.actionContext = "world";

            this.parameters.Insert("overcast", GameLabsActionParameter("Overcast", "Number between 0 and 1", "float"));
            this.parameters.Insert("fog", GameLabsActionParameter("Fog", "Number between 0 and 1", "float"));
            this.parameters.Insert("rain", GameLabsActionParameter("Rain", "Number between 0 and 1", "float"));
            this.parameters.Insert("wind", GameLabsActionParameter("Wind speed", "Wind speed in km/h", "int"));
        }

        bool Execute(GameLabsActionContext context) {
            GetGameLabs().GetLogger().Warn(string.Format("[Weather] Updating weather overcast=%1, fog=%2, rain=%3, wind=%4",context.parameters.Get("overcast").GetFloat(),context.parameters.Get("fog").GetFloat(),context.parameters.Get("rain").GetFloat(),context.parameters.Get("wind").GetInt()));

            Weather weather = GetGame().GetWeather();
            if(!weather) return false;

            if(weather.GetOvercast()) weather.GetOvercast().Set(context.parameters.Get("overcast").GetFloat(), context.parameters.Get("overcast").GetFloat(), context.parameters.Get("overcast").GetFloat());
            if(weather.GetFog()) weather.GetFog().Set(context.parameters.Get("fog").GetFloat(), context.parameters.Get("fog").GetFloat(), context.parameters.Get("fog").GetFloat());
            if(weather.GetRain()) weather.GetRain().Set(context.parameters.Get("rain").GetFloat(), context.parameters.Get("rain").GetFloat(), context.parameters.Get("rain").GetFloat());
            weather.SetWindSpeed(context.parameters.Get("wind").GetInt());

            return true;
        }
};

class CFCloud_WorldWeatherSunny extends GameLabsContextAction {
        void CFCloud_WorldWeatherSunny() {
            this.actionCode = "CFCloud_WorldWeatherSunny";
            this.actionName = "Set clear weather";
            this.actionIcon = "sun";
            this.actionColour = "success";
            this.actionContext = "world";
        }

        bool Execute(GameLabsActionContext context) {
            GetGameLabs().GetLogger().Warn(string.Format("[Weather] Updating weather (sunny/clear)"));

            Weather weather = GetGame().GetWeather();
            if(!weather) return false;

            if(weather.GetOvercast()) weather.GetOvercast().Set(0.0, 0.0, 0.0);
            if(weather.GetFog()) weather.GetFog().Set(0.0, 0.0, 0.0);
            if(weather.GetRain()) weather.GetRain().Set(0.0, 0.0, 0.0);
            weather.SetWindSpeed(0);

            return true;
        }
};

class CFCloud_WorldWipeAI extends GameLabsContextAction {
        void CFCloud_WorldWipeAI() {
            this.actionCode = "CFCloud_WorldWipeAI";
            this.actionName = "Clear all world AI";
            this.actionIcon = "redo";
            this.actionColour = "danger";
            this.actionContext = "world";
        }

        bool Execute(GameLabsActionContext context) {
            GetGameLabs().GetLogger().Warn(string.Format("[World (AI)] Clearing all world ai..."));

            GetGameLabs().ClearAI();

            GetGameLabs().GetLogger().Warn(string.Format("[World (AI)] AI cleared"));
            return true;
        }
};

class CFCloud_WorldWipeVehicles extends GameLabsContextAction {
        void CFCloud_WorldWipeVehicles() {
            this.actionCode = "CFCloud_WorldWipeVehicles";
            this.actionName = "Clear all world vehicles";
            this.actionIcon = "redo";
            this.actionColour = "danger";
            this.actionContext = "world";
        }

        bool Execute(GameLabsActionContext context) {
            GetGameLabs().GetLogger().Warn(string.Format("[World (AI)] Clearing all world vehicles..."));

            GetGameLabs().ClearVehicles();

            GetGameLabs().GetLogger().Warn(string.Format("[World (AI)] Vehicles cleared"));
            return true;
        }
};

class CFCloud_SpawnItemWorld extends GameLabsContextAction {
        void CFCloud_SpawnItemWorld() {
            this.actionCode = "CFCloud_SpawnItemWorld";
            this.actionName = "Spawn an item at coordinates";
            this.actionIcon = "gem";
            this.actionColour = "default";
            this.actionContext = "world";

            this.parameters.Insert("vector", GameLabsActionParameter("Coordinates", "World coordinates", "vector"));
            this.parameters.Insert("item", GameLabsActionParameter("Item class name", "Class name of item to be spawned", "cf_itemlist"));
            this.parameters.Insert("quantity", GameLabsActionParameter("Quantity", "Amount if items to be spawned", "int"));
            this.parameters.Insert("debug", GameLabsActionParameter("Debug spawn", "Use debug spawn method to automatically populate specific items", "boolean"));
            this.parameters.Insert("stacked", GameLabsActionParameter("Stacked", "Spawn items as a stack (only works if item supports to be stacked)", "boolean"));
        }

        bool Execute(GameLabsActionContext context) {
            string itemType = context.parameters.Get("item").GetString();
            GetGameLabs().GetLogger().Warn(string.Format("[Spawn] Spawning %1 (x%2) at %3", itemType, context.parameters.Get("quantity").GetInt(), context.parameters.Get("vector").GetVector()));

            int entityFlags;
            if(GetGame().IsKindOf(itemType,"DZ_LightAI") || GetGame().IsKindOf(itemType, "SurvivorBase")) {
                entityFlags = ECE_INITAI | ECE_CREATEPHYSICS;
            } else {
                entityFlags = ECE_SETUP | ECE_KEEPHEIGHT | ECE_PLACE_ON_SURFACE;
            }

            EntityAI entity;
            if(context.parameters.Get("stacked").GetBoolean()) {
                entity = EntityAI.Cast(GetGame().CreateObjectEx(itemType, context.parameters.Get("vector").GetVector(), entityFlags));
                if (context.parameters.Get("debug").GetBoolean()) {
                    entity.OnDebugSpawn();
                }
                ItemBase itemBase;
                ItemBase.CastTo(itemBase, entity);
                itemBase.SetQuantity(context.parameters.Get("quantity").GetInt());
            } else {
                for (int i = 1; i <= context.parameters.Get("quantity").GetInt(); i++) {
                    entity = EntityAI.Cast(GetGame().CreateObjectEx(itemType, context.parameters.Get("vector").GetVector(), entityFlags));
                    if (context.parameters.Get("debug").GetBoolean()) {
                        entity.OnDebugSpawn();
                    }
                }
            }
            return true;
        }
};

class CFCloud_ObjectDelete extends GameLabsContextAction {
        void CFCloud_ObjectDelete() {
            this.actionCode = "CFCloud_ObjectDelete";
            this.actionName = "Delete object";
            this.actionIcon = "trash";
            this.actionColour = "danger";
            this.actionContext = "object";
        }

        bool Execute(GameLabsActionContext context) {
            _Event eventInstance;
            _Event.CastTo(eventInstance, context.GetReferencedObject());

            GetGameLabs().GetLogger().Warn(string.Format("[ObjectDelete] Deleting object %1", eventInstance.Ref()));

            eventInstance.Ref().Delete();
            return true;
        }
};

class CFCloud_TerritoryFlagClear extends GameLabsContextAction {
        void CFCloud_TerritoryFlagClear() {
            this.actionCode = "CFCloud_TerritoryFlagClear";
            this.actionName = "Clear territory (Server restart required)";
            this.actionIcon = "flag-pennant";
            this.actionColour = "warning";
            this.actionContext = "object";
            this.actionContextFilter = new array<string>;
            this.actionContextFilter.Insert("TerritoryFlag");
        }

        bool Execute(GameLabsActionContext context) {
            _Event eventInstance;
            _Event.CastTo(eventInstance, context.GetReferencedObject());
            GetGameLabs().GetLogger().Warn(string.Format("[TerritoryFlagClear] Clearing flag and everything in radius for %1<%2>", eventInstance.Ref(), eventInstance));

            TerritoryFlag totem;
            TerritoryFlag.CastTo(totem, eventInstance.Ref());

            GetCEApi().RadiusLifetimeDecrease(totem.GetPosition(), GameConstants.REFRESHER_RADIUS, totem.GetEconomyProfile().GetLifetime() + 1);
            totem.Delete();
            GetCEApi().OnUpdate();
            return true;
        }
};

/* Other Actions */

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
