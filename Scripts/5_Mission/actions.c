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

        override bool Execute(GameLabsActionContext context) {
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
            this.actionContext = "player";
        }

        override bool Execute(GameLabsActionContext context) {
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
            this.actionContext = "player";
        }

        override bool Execute(GameLabsActionContext context) {
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
            this.actionContext = "player";

            this.parameters.Insert("item", GameLabsActionParameter("Item class name", "Class name of item to be spawned", "cf_itemlist"));
            this.parameters.Insert("quantity", GameLabsActionParameter("Quantity", "Amount if items to be spawned", "int"));
            this.parameters.Insert("debug", GameLabsActionParameter("Debug spawn", "Use debug spawn method to automatically populate specific items", "boolean"));
            this.parameters.Insert("stacked", GameLabsActionParameter("Stacked", "Spawn items as a stack (only works if item supports to be stacked)", "boolean"));
        }

        override bool Execute(GameLabsActionContext context) {
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

        override bool Execute(GameLabsActionContext context) {
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

        override bool Execute(GameLabsActionContext context) {
            PlayerBase player;
            PlayerBase.CastTo(player, context.GetReferencedObject());

            GetGameLabs().GetLogger().Warn(string.Format("[Player-Strip] %1", player));
            player.RemoveAllItems();
            return true;
        }
};


class CFCloud_PlayerDropItem extends GameLabsContextAction {
        void CFCloud_PlayerDropItem() {
            this.actionCode = "CFCloud_PlayerDropItem";
            this.actionName = "Drop player item from hands";
            this.actionIcon = "hand-paper";
            this.actionColour = "default";
            this.actionContext = "player";
        }

        override bool Execute(GameLabsActionContext context) {
            PlayerBase player;
            PlayerBase.CastTo(player, context.GetReferencedObject());

            GetGameLabs().GetLogger().Warn(string.Format("[Player-DropItem] %1", player));
            ItemBase item = ItemBase.Cast(player.GetItemInHands());
            if(item) {
                player.DropItem(item);
            }
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

        override bool Execute(GameLabsActionContext context) {
            _Vehicle vehicle;
            _Vehicle.CastTo(vehicle, context.GetReferencedObject());

            Car vehicleEntity;
            Car.CastTo(vehicleEntity, vehicle.Ref());

            GetGameLabs().GetLogger().Warn(string.Format("[Vehicle-Delete] %1", vehicleEntity));
            vehicleEntity.Delete();
            return true;
        }
};

// TODO: This requires additional sync
class CFCloud_VehicleEjectDriver extends GameLabsContextAction {
        void CFCloud_VehicleEjectDriver() {
            this.actionCode = "CFCloud_VehicleEjectDriver";
            this.actionName = "Eject driver";
            this.actionIcon = "car-tilt";
            this.actionColour = "default";
            this.actionContext = "vehicle";
        }

        override bool Execute(GameLabsActionContext context) {
            _Vehicle vehicle;
            _Vehicle.CastTo(vehicle, context.GetReferencedObject());

            Car vehicleEntity;
            Car.CastTo(vehicleEntity, vehicle.Ref());

            for(int c = 0; c < vehicleEntity.CrewSize(); ++c) {
                Human crew = vehicleEntity.CrewMember(c);
                if(!crew)
                    continue;

                PlayerBase player;
                if(Class.CastTo(player, crew)) {
                    if(vehicleEntity.CrewMemberIndex(player) == DayZPlayerConstants.VEHICLESEAT_DRIVER) {
                        HumanCommandVehicle vehCommand = player.GetCommand_Vehicle();
                        vehCommand.GetOutVehicle();
                        GetGameLabs().GetLogger().Warn(string.Format("[Vehicle-EjectDriver] %1 driver=%2", vehicleEntity, player));
                        return true;
                    }
                }
            }
            GetGameLabs().GetLogger().Warn(string.Format("[Vehicle-EjectDriver] %1 - no driver found", vehicleEntity));
            return true;
        }
};

class CFCloud_VehicleExplode extends GameLabsContextAction {
        void CFCloud_VehicleExplode() {
            this.actionCode = "CFCloud_VehicleExplode";
            this.actionName = "Explode vehicle";
            this.actionIcon = "car-crash";
            this.actionColour = "danger";
            this.actionContext = "vehicle";
        }

        override bool Execute(GameLabsActionContext context) {
            _Vehicle vehicle;
            _Vehicle.CastTo(vehicle, context.GetReferencedObject());

            Car vehicleEntity;
            Car.CastTo(vehicleEntity, vehicle.Ref());

            GetGameLabs().GetLogger().Warn(string.Format("[Vehicle-Explode] %1", vehicleEntity));

            vehicleEntity.Explode(DT_EXPLOSION, "LandFuelFeed_Ammo");
            return true;
        }
};

class CFCloud_KillVehicleEngine extends GameLabsContextAction {
        void CFCloud_KillVehicleEngine() {
            this.actionCode = "CFCloud_KillVehicleEngine";
            this.actionName = "Stop engine";
            this.actionIcon = "engine-warning";
            this.actionColour = "default";
            this.actionContext = "vehicle";
        }

        override bool Execute(GameLabsActionContext context) {
            _Vehicle vehicle;
            _Vehicle.CastTo(vehicle, context.GetReferencedObject());

            Car vehicleEntity;
            Car.CastTo(vehicleEntity, vehicle.Ref());

            GetGameLabs().GetLogger().Warn(string.Format("[Vehicle-EngineStop] %1", vehicleEntity));

            CarScript.Cast(vehicleEntity).EngineStop();
            return true;
        }
};

class CFCloud_RefuelVehicle extends GameLabsContextAction {
        void CFCloud_RefuelVehicle() {
            this.actionCode = "CFCloud_RefuelVehicle";
            this.actionName = "Refuel vehicle";
            this.actionIcon = "gas-pump";
            this.actionColour = "default";
            this.actionContext = "vehicle";
        }

        override bool Execute(GameLabsActionContext context) {
            _Vehicle vehicle;
            _Vehicle.CastTo(vehicle, context.GetReferencedObject());

            Car vehicleEntity;
            Car.CastTo(vehicleEntity, vehicle.Ref());

            GetGameLabs().GetLogger().Warn(string.Format("[Vehicle-Refuel] %1", vehicleEntity));

            CarScript vehicleEntityScript = CarScript.Cast(vehicleEntity);
            float fuel = vehicleEntityScript.GetFluidCapacity(CarFluid.FUEL) - (vehicleEntityScript.GetFluidCapacity(CarFluid.FUEL) * vehicleEntityScript.GetFluidFraction(CarFluid.FUEL));
            float oil = vehicleEntityScript.GetFluidCapacity(CarFluid.OIL) - (vehicleEntityScript.GetFluidCapacity(CarFluid.OIL) * vehicleEntityScript.GetFluidFraction(CarFluid.OIL));
            float coolant = vehicleEntityScript.GetFluidCapacity(CarFluid.COOLANT) - (vehicleEntityScript.GetFluidCapacity(CarFluid.COOLANT) * vehicleEntityScript.GetFluidFraction(CarFluid.COOLANT));
            float brake = vehicleEntityScript.GetFluidCapacity(CarFluid.BRAKE) - (vehicleEntityScript.GetFluidCapacity(CarFluid.BRAKE) * vehicleEntityScript.GetFluidFraction(CarFluid.BRAKE));
            vehicleEntityScript.Fill(CarFluid.FUEL, fuel);
            vehicleEntityScript.Fill(CarFluid.OIL, oil);
            vehicleEntityScript.Fill(CarFluid.COOLANT, coolant);
            vehicleEntityScript.Fill(CarFluid.BRAKE, brake);
            vehicleEntityScript.SetSynchDirty();
            vehicleEntityScript.Synchronize();
            return true;
        }
};

class CFCloud_RepairVehicle extends GameLabsContextAction {
        void CFCloud_RepairVehicle() {
            this.actionCode = "CFCloud_RepairVehicle";
            this.actionName = "Repair vehicle and refill";
            this.actionIcon = "car-mechanic";
            this.actionColour = "primary";
            this.actionContext = "vehicle";
        }

        override bool Execute(GameLabsActionContext context) {
            _Vehicle vehicle;
            _Vehicle.CastTo(vehicle, context.GetReferencedObject());

            Car vehicleEntity;
            Car.CastTo(vehicleEntity, vehicle.Ref());
            EntityAI vehicleEntityAI = vehicleEntity;

            GetGameLabs().GetLogger().Warn(string.Format("[Vehicle-Repair] %1", vehicleEntity));

            // Base entity
            vehicleEntityAI.SetHealthMax("", "Health");
            vehicleEntityAI.SetHealthMax();

            CarScript vehicleEntityScript = CarScript.Cast(vehicleEntity);
            float fuel = vehicleEntityScript.GetFluidCapacity(CarFluid.FUEL) - (vehicleEntityScript.GetFluidCapacity(CarFluid.FUEL) * vehicleEntityScript.GetFluidFraction(CarFluid.FUEL));
            float oil = vehicleEntityScript.GetFluidCapacity(CarFluid.OIL) - (vehicleEntityScript.GetFluidCapacity(CarFluid.OIL) * vehicleEntityScript.GetFluidFraction(CarFluid.OIL));
            float coolant = vehicleEntityScript.GetFluidCapacity(CarFluid.COOLANT) - (vehicleEntityScript.GetFluidCapacity(CarFluid.COOLANT) * vehicleEntityScript.GetFluidFraction(CarFluid.COOLANT));
            float brake = vehicleEntityScript.GetFluidCapacity(CarFluid.BRAKE) - (vehicleEntityScript.GetFluidCapacity(CarFluid.BRAKE) * vehicleEntityScript.GetFluidFraction(CarFluid.BRAKE));
            vehicleEntityScript.Fill(CarFluid.FUEL, fuel);
            vehicleEntityScript.Fill(CarFluid.OIL, oil);
            vehicleEntityScript.Fill(CarFluid.COOLANT, coolant);
            vehicleEntityScript.Fill(CarFluid.BRAKE, brake);
            vehicleEntityScript.SetSynchDirty();
            vehicleEntityScript.Synchronize();

            // Repair components
            string cfg_path = string.Format("%1 %2 DamageSystem", CFG_VEHICLESPATH, vehicleEntity.GetType());
            if(GetGame().ConfigIsExisting(cfg_path)) {
                string child_zone;
                string child_class;
                array<string> damaged_zones = new array<string>;

                int zone_count = GetGame().ConfigGetChildrenCount(cfg_path);
                if(zone_count > 0) {
                    for(int x = 0; x < zone_count; ++x) {
                        GetGame().ConfigGetChildName(cfg_path, x, child_class);
                        child_class.ToLower();
                        if(child_class == "damagezones") {
                            for (int y = 0; y < GetGame().ConfigGetChildrenCount(string.Format("%1 DamageZones", cfg_path)); ++y) {
                                GetGame().ConfigGetChildName(string.Format("%1 DamageZones", cfg_path), y, child_zone);
                                damaged_zones.Insert(child_zone);
                            }
                        }
                    }
                }

                if(damaged_zones.Count() > 0) {
                    foreach(string zone: damaged_zones) {
                        vehicleEntityAI.SetHealthMax(zone, "Health");
                    }
                }
            }

            // Repair & complete attachments
            TStringArray vehicle_slots = new TStringArray;
            cfg_path = string.Format("%1 %2 attachments", CFG_VEHICLESPATH, vehicleEntity.GetType());
            GetGame().ConfigGetTextArray(cfg_path, vehicle_slots);

            foreach(string slot : vehicle_slots) {
                slot.ToLower();

                int slot_id = InventorySlots.GetSlotIdFromString(slot);
                EntityAI attachment = vehicleEntity.GetInventory().FindAttachment(slot_id);
                if(!attachment) {
                    string type = GetGameLabs()._vehicleSlotMap[slot].GetRandomElement();
                    type.ToLower();
                    if(type.Contains("_ruined")) {
                        type = GetGameLabs()._vehicleSlotMap[slot][0];
                    }
                    vehicleEntity.GetInventory().CreateAttachmentEx(type, slot_id);

                } else {
                    string part = attachment.GetType();
                    part.ToLower();
                    if(part.Contains("_ruined")) {
                        part.Replace("_ruined", "");
                        GetGame().ObjectDelete(attachment);
                        vehicleEntity.GetInventory().CreateInInventory(part);
                    } else {
                        attachment.SetHealthMax("", "Health");
                        attachment.SetSynchDirty();
                    }
                }
            }
            return true;
        }
};

class CFCloud_UnstuckVehicle extends GameLabsContextAction {
        void CFCloud_UnstuckVehicle() {
            this.actionCode = "CFCloud_UnstuckVehicle";
            this.actionName = "Unstuck vehicle from below the map";
            this.actionIcon = "caret-square-up";
            this.actionColour = "default";
            this.actionContext = "vehicle";
        }

        override bool Execute(GameLabsActionContext context) {
            _Vehicle vehicle;
            _Vehicle.CastTo(vehicle, context.GetReferencedObject());

            Car vehicleEntity;
            Car.CastTo(vehicleEntity, vehicle.Ref());

            GetGameLabs().GetLogger().Warn(string.Format("[Vehicle-Unstuck] %1", vehicleEntity));

            vector position = vehicleEntity.GetPosition();
            position[1] = GetGame().SurfaceY(position[0], position[2]) + 1;
            vehicleEntity.SetPosition(position);
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

        override bool Execute(GameLabsActionContext context) {
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

        override bool Execute(GameLabsActionContext context) {
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

        override bool Execute(GameLabsActionContext context) {
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

        override bool Execute(GameLabsActionContext context) {
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

        override bool Execute(GameLabsActionContext context) {
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

        override bool Execute(GameLabsActionContext context) {
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

        override bool Execute(GameLabsActionContext context) {
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
            this.actionIcon = "pennant";
            this.actionColour = "warning";
            this.actionContext = "object";
            this.actionContextFilter = new array<string>;
            this.actionContextFilter.Insert("TerritoryFlag");
        }

        override bool Execute(GameLabsActionContext context) {
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

        override bool Execute(GameLabsActionContext context) {
            GetGameLabs().GetLogger().Debug(string.Format("Dummy action called testInt=%1, testString=%2, testBool=%3", context.parameters.Get("testInt").GetInt(), context.parameters.Get("testString").GetString(), context.parameters.Get("testBool").GetBoolean()));
            return true;
        }
};

class GameLabsInternal_DummyActionWithWebHook extends GameLabsContextAction {
        void GameLabsInternal_DummyActionWithWebHook() {
            this.actionCode = "GameLabsInternal_DummyActionWithWebHook";
            this.actionName = "Dummy action (WebHook)";
            this.actionIcon = "accessible-icon";
            this.actionColour = "danger";
            this.actionContext = "world";

            this.responseSuccess = new GameLabsActionResponse("webhook");
            this.responseFailed = new GameLabsActionResponse("webhook");

            this.parameters.Insert("webhookUrl", GameLabsActionParameter("WebHook URL", "Any valid WebHook URL", "webhook_url"));
        }

        override bool Execute(GameLabsActionContext context) {
            GetGameLabs().GetLogger().Debug(string.Format("Dummy action called webhookUrl=%1", context.parameters.Get("webhookUrl").GetString()));
            return true;
        }
};
