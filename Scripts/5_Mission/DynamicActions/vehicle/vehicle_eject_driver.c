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

            int c;
            Human crew;
            PlayerBase player;
            HumanCommandVehicle vehCommand;
            if(vehicle.VehicleType() == "car" || vehicle.VehicleType() == "truck") {
                Car vehicleEntity;
                Car.CastTo(vehicleEntity, vehicle.Ref());
                for(c = 0; c < vehicleEntity.CrewSize(); ++c) {
                    crew = vehicleEntity.CrewMember(c);
                    if(!crew)
                        continue;

                    if(Class.CastTo(player, crew)) {
                        if(vehicleEntity.CrewMemberIndex(player) == DayZPlayerConstants.VEHICLESEAT_DRIVER) {
                            vehCommand = player.GetCommand_Vehicle();
                            vehCommand.GetOutVehicle();
                            GetGameLabs().GetLogger().Warn(string.Format("[Vehicle-EjectDriver] %1 driver=%2", vehicleEntity, player));
                            return true;
                        }
                    }
                }
                GetGameLabs().GetLogger().Warn(string.Format("[Vehicle-EjectDriver] %1 - no driver found", vehicleEntity));
            } else if(vehicle.VehicleType() == "boat") {
                Boat boatEntity;
                Boat.CastTo(boatEntity, vehicle.Ref());
                for(c = 0; c < boatEntity.CrewSize(); ++c) {
                    crew = boatEntity.CrewMember(c);
                    if(!crew)
                        continue;

                    if(Class.CastTo(player, crew)) {
                        if(boatEntity.CrewMemberIndex(player) == DayZPlayerConstants.VEHICLESEAT_DRIVER) {
                            vehCommand = player.GetCommand_Vehicle();
                            vehCommand.GetOutVehicle();
                            GetGameLabs().GetLogger().Warn(string.Format("[Vehicle-EjectDriver] %1 driver=%2", boatEntity, player));
                            return true;
                        }
                    }
                }
                GetGameLabs().GetLogger().Warn(string.Format("[Vehicle-EjectDriver] %1 - no driver found", boatEntity));
            } else {
                GetGameLabs().GetLogger().Warn(string.Format("[Vehicle-EjectDriver] %1 is not categorized and can not be deleted.", vehicle));
            }
            return true;
        }
};