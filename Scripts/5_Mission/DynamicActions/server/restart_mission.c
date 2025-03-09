class GameLabsInternal_RestartMission extends GameLabsContextAction {
        void GameLabsInternal_RestartMission() {
            this.actionCode = "GameLabsInternal_RestartMission";
            this.actionName = "Restart Mission";
            this.actionIcon = "terminal";
            this.actionColour = "danger";
            this.actionContext = "world";
        }

        override bool Execute(GameLabsActionContext context) {
            GetGame().RestartMission();
            return true;
        }
};