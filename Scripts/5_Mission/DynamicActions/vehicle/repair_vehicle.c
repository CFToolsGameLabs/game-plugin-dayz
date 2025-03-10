
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

            float fuel, oil, coolant, brake;
            if(vehicle.VehicleType() == "car" || vehicle.VehicleType() == "truck") {
                Car vehicleEntity;
                Car.CastTo(vehicleEntity, vehicle.Ref());
                EntityAI vehicleEntityAI = vehicleEntity;

                GetGameLabs().GetLogger().Warn(string.Format("[Vehicle-Repair] %1", vehicleEntity));

                // Base entity
                vehicleEntityAI.SetHealthMax("", "Health");
                vehicleEntityAI.SetHealthMax();

                CarScript vehicleEntityScript = CarScript.Cast(vehicleEntity);
                fuel = vehicleEntityScript.GetFluidCapacity(CarFluid.FUEL) - (vehicleEntityScript.GetFluidCapacity(CarFluid.FUEL) * vehicleEntityScript.GetFluidFraction(CarFluid.FUEL));
                oil = vehicleEntityScript.GetFluidCapacity(CarFluid.OIL) - (vehicleEntityScript.GetFluidCapacity(CarFluid.OIL) * vehicleEntityScript.GetFluidFraction(CarFluid.OIL));
                coolant = vehicleEntityScript.GetFluidCapacity(CarFluid.COOLANT) - (vehicleEntityScript.GetFluidCapacity(CarFluid.COOLANT) * vehicleEntityScript.GetFluidFraction(CarFluid.COOLANT));
                brake = vehicleEntityScript.GetFluidCapacity(CarFluid.BRAKE) - (vehicleEntityScript.GetFluidCapacity(CarFluid.BRAKE) * vehicleEntityScript.GetFluidFraction(CarFluid.BRAKE));
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
            } else if(vehicle.VehicleType() == "boat") {
                Boat boatEntity;
                Boat.CastTo(boatEntity, vehicle.Ref());

                boatEntity.SetHealthMax("", "Health");
                boatEntity.SetHealthMax();

                BoatScript boatEntityScript = BoatScript.Cast(boatEntity);
                fuel = boatEntityScript.GetFluidCapacity(BoatFluid.FUEL) - (boatEntityScript.GetFluidCapacity(BoatFluid.FUEL) * boatEntityScript.GetFluidFraction(BoatFluid.FUEL));
                boatEntityScript.Fill(BoatFluid.FUEL, fuel);
                boatEntityScript.SetSynchDirty();
                boatEntityScript.Synchronize();
                GetGameLabs().GetLogger().Warn(string.Format("[Vehicle-Repair] %1", boatEntity));
            } else {
                GetGameLabs().GetLogger().Warn(string.Format("[Vehicle-Repair] %1 is not categorized and can not be deleted.", vehicle));
            }
            return true;
        }
};