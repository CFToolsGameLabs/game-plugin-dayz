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

            if(vehicle.VehicleType() == "car" || vehicle.VehicleType() == "truck") {
                Car vehicleEntity;
                Car.CastTo(vehicleEntity, vehicle.Ref());
                CarScript.Cast(vehicleEntity).EngineStop();
                GetGameLabs().GetLogger().Warn(string.Format("[Vehicle-EngineStop] %1", vehicleEntity));
            } else if(vehicle.VehicleType() == "boat") {
                Boat boatEntity;
                Boat.CastTo(boatEntity, vehicle.Ref());
                BoatScript.Cast(boatEntity).EngineStop();
                GetGameLabs().GetLogger().Warn(string.Format("[Vehicle-EngineStop] %1", boatEntity));
            } else if(vehicle.VehicleType() == "bike") {
                Motorbike bikeEntity;
                Motorbike.CastTo(bikeEntity, vehicle.Ref());
                MotorbikeScript.Cast(bikeEntity).EngineStop();
                GetGameLabs().GetLogger().Warn(string.Format("[Vehicle-EngineStop] %1", bikeEntity));
            } else {
                GetGameLabs().GetLogger().Warn(string.Format("[Vehicle-EngineStop] %1 is not categorized and can not be deleted.", vehicle));
            }


            return true;
        }
};