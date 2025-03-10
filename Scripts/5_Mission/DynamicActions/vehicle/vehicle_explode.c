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

            if(vehicle.VehicleType() == "car" || vehicle.VehicleType() == "truck") {
                Car vehicleEntity;
                Car.CastTo(vehicleEntity, vehicle.Ref());
                vehicleEntity.Explode(DT_EXPLOSION, "LandFuelFeed_Ammo");
                GetGameLabs().GetLogger().Warn(string.Format("[Vehicle-Explode] %1", vehicleEntity));
            } else if(vehicle.VehicleType() == "boat") {
                Boat boatEntity;
                Boat.CastTo(boatEntity, vehicle.Ref());
                boatEntity.Explode(DT_EXPLOSION, "LandFuelFeed_Ammo");
                GetGameLabs().GetLogger().Warn(string.Format("[Vehicle-Explode] %1", boatEntity));
            } else {
                GetGameLabs().GetLogger().Warn(string.Format("[Vehicle-Explode] %1 is not categorized and can not be deleted.", vehicle));
            }
            return true;
        }
};