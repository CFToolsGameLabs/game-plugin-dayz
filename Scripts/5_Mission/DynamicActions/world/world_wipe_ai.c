class CFCloud_WorldWipeAI extends GameLabsContextAction {
        void CFCloud_WorldWipeAI() {
            this.actionCode = "CFCloud_WorldWipeAI";
            this.actionName = "Clear all world AI";
            this.actionIcon = "redo";
            this.actionColour = "danger";
            this.actionContext = "world";
        }

        override bool Execute(GameLabsActionContext context) {
            GetGameLabs().GetLogger().Warn(string.Format("[World (AI)] Clearing all world ai..."));

            GetGameLabs().ClearAI();

            GetGameLabs().GetLogger().Warn(string.Format("[World (AI)] AI cleared"));
            return true;
        }
};

