class CFCloud_SpawnVehicle extends GameLabsContextAction {
        void CFCloud_SpawnVehicle() {
            this.actionCode = "CFCloud_SpawnVehicle";
            this.actionName = "Spawn a vehicle at coordinates";
            this.actionIcon = "car";
            this.actionColour = "default";
            this.actionContext = "world";

            this.parameters.Insert("vector", GameLabsActionParameter("Coordinates", "World coordinates", "vector"));

            GameLabsActionParameter type = new GameLabsActionParameter("Vehicle type", "Vehicle to be spawned", "options");

            int nClasses = GetGame().ConfigGetChildrenCount("CfgVehicles");
            for(int nClass = 0; nClass < nClasses; nClass++) {
                string strName;
                if(!GetGame().ConfigGetChildName("CfgVehicles", nClass, strName))
                    continue;

                int scope = GetGame().ConfigGetInt("CfgVehicles " + strName + " scope");
                if(scope != 2)
                    continue;

                if(!GetGame().IsKindOf(strName, "CarScript") && !GetGame().IsKindOf(strName, "BoatScript"))
                    continue;

                string displayName = "";
                if(!GetGame().ConfigGetText("CfgVehicles " + strName + " displayName", displayName) || displayName == "")
                    displayName = strName;

                GameLabsActionParameter option = new GameLabsActionParameter(displayName, "", "string");
                option.valueString = strName;
                type.options.Insert(strName, option);
            }

            this.parameters.Insert("type", type);
        }

        override bool Execute(GameLabsActionContext context) {
            string vehicleType = context.parameters.Get("type").GetString();
            vector position = context.parameters.Get("vector").GetVector();

            if(vehicleType == "") {
                GetGameLabs().GetLogger().Warn("[SpawnVehicle] No vehicle type selected");
                return false;
            }

            GetGameLabs().GetLogger().Warn(string.Format("[SpawnVehicle] Spawning %1 at %2", vehicleType, position));

            EntityAI entity = EntityAI.Cast(GetGame().CreateObjectEx(vehicleType, position, ECE_PLACE_ON_SURFACE | ECE_INITAI | ECE_CREATEPHYSICS));
            if(!entity) {
                GetGameLabs().GetLogger().Warn(string.Format("[SpawnVehicle] Failed to spawn %1", vehicleType));
                return false;
            }

            return true;
        }
};
