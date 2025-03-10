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
