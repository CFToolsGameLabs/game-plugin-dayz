class CFCloud_StripPlayer extends GameLabsContextAction {
        void CFCloud_StripPlayer() {
            this.actionCode = "CFCloud_StripPlayer";
            this.actionName = "Delete all player items";
            this.actionIcon = "tshirt";
            this.actionColour = "danger";
            this.actionContext = "player";
        }

        override bool Execute(GameLabsActionContext context) {
            PlayerBase player;
            PlayerBase.CastTo(player, context.GetReferencedObject());

            GetGameLabs().GetLogger().Warn(string.Format("[Player-Strip] %1", player));
            player.RemoveAllItems();
            return true;
        }
};