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

            float fuel, oil, coolant, brake;
            if(vehicle.VehicleType() == "car" || vehicle.VehicleType() == "truck") {
                Car vehicleEntity;
                Car.CastTo(vehicleEntity, vehicle.Ref());
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
                GetGameLabs().GetLogger().Warn(string.Format("[Vehicle-Refuel] %1", vehicleEntity));
            } else if(vehicle.VehicleType() == "boat") {
                Boat boatEntity;
                Boat.CastTo(boatEntity, vehicle.Ref());
                BoatScript boatEntityScript = BoatScript.Cast(boatEntity);
                fuel = boatEntityScript.GetFluidCapacity(BoatFluid.FUEL) - (boatEntityScript.GetFluidCapacity(BoatFluid.FUEL) * boatEntityScript.GetFluidFraction(BoatFluid.FUEL));
                boatEntityScript.Fill(BoatFluid.FUEL, fuel);
                boatEntityScript.SetSynchDirty();
                boatEntityScript.Synchronize();
                GetGameLabs().GetLogger().Warn(string.Format("[Vehicle-Refuel] %1", boatEntity));
            } else {
                GetGameLabs().GetLogger().Warn(string.Format("[Vehicle-Refuel] %1 is not categorized and can not be deleted.", vehicle));
            }
            return true;
        }
};
