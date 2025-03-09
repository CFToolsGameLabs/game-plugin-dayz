class GameLabsInternal_DumpPlayerPosition extends GameLabsContextAction {
        void GameLabsInternal_DumpPlayerPosition() {
            this.actionCode = "GameLabsInternal_DumpPlayerPosition";
            this.actionName = "Dump player coordinates";
            this.actionIcon = "terminal";
            this.actionColour = "info";
            this.actionContext = "player";

            GameLabsActionParameter message = new GameLabsActionParameter("Message", "", "string");
            message.valueString = "";
            this.parameters.Insert("message", message);
        }

        override bool Execute(GameLabsActionContext context) {
            PlayerBase player;
            PlayerBase.CastTo(player, context.GetReferencedObject());

            vector position = player.GetPosition();
            GetGameLabs().GetLogger().Info(string.Format("[DumpPlayerPosition] {%1} player=%2; position=%3;", context.parameters.Get("message").GetString(), player, position));
            return true;
        }
};