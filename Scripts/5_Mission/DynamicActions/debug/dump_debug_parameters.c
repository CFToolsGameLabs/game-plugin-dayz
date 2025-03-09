class GameLabsInternal_DumpDebugParameters extends GameLabsContextAction {
        void GameLabsInternal_DumpDebugParameters() {
            this.actionCode = "GameLabsInternal_DumpDebugParameters";
            this.actionName = "Dump debug parameters";
            this.actionIcon = "terminal";
            this.actionColour = "info";
            this.actionContext = "world";
        }

        override bool Execute(GameLabsActionContext context) {
            GetGameLabs().GetLogger().Info("****************** DumpDebugParameters ******************");
            // World Name
            string worldName = "";
            GetGame().GetWorldName(worldName);
            GetGameLabs().GetLogger().Info(string.Format("GetWorldName() %1;", worldName));
            GetGameLabs().GetLogger().Info(string.Format("GetWorldSize() %1;", GetGame().GetWorld().GetWorldSize()));
            GetGameLabs().GetLogger().Info(string.Format("GetMission() %1;", GetGame().GetMission()));
            GetGameLabs().GetLogger().Info(string.Format("GetMissionPath() %1;", GetDayZGame().GetMissionPath()));
            return true;
        }
};