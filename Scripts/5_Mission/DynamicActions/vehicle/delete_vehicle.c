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

            if(vehicle.VehicleType() == "car" || vehicle.VehicleType() == "truck") {
                Car carEntity;
                Car.CastTo(carEntity, vehicle.Ref());
                carEntity.Delete();
                GetGameLabs().GetLogger().Warn(string.Format("[Vehicle-Delete] %1", carEntity));
            } else if(vehicle.VehicleType() == "boat") {
                Boat boatEntity;
                Boat.CastTo(boatEntity, vehicle.Ref());
                boatEntity.Delete();
                GetGameLabs().GetLogger().Warn(string.Format("[Vehicle-Delete] %1", boatEntity));
            } else {
                GetGameLabs().GetLogger().Warn(string.Format("[Vehicle-Delete] %1 is not categorized and can not be deleted.", vehicle));
            }
            return true;
        }
};