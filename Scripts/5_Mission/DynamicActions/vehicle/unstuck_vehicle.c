
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

            vector position;
            if(vehicle.VehicleType() == "car" || vehicle.VehicleType() == "truck") {
                Car vehicleEntity;
                Car.CastTo(vehicleEntity, vehicle.Ref());

                position = vehicleEntity.GetPosition();
                position[1] = GetGame().SurfaceY(position[0], position[2]) + 1;
                vehicleEntity.SetPosition(position);

                GetGameLabs().GetLogger().Warn(string.Format("[Vehicle-Unstuck] %1", vehicleEntity));
            } else if(vehicle.VehicleType() == "boat") {
                Boat boatEntity;
                Boat.CastTo(boatEntity, vehicle.Ref());

                position = boatEntity.GetPosition();
                position[1] = GetGame().SurfaceY(position[0], position[2]) + 1;
                boatEntity.SetPosition(position);

                GetGameLabs().GetLogger().Warn(string.Format("[Vehicle-Unstuck] %1", boatEntity));
            } else {
                GetGameLabs().GetLogger().Warn(string.Format("[Vehicle-Unstuck] %1 is not categorized and can not be deleted.", vehicle));
            }
            return true;
        }
};