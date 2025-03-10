class CFCloud_PlayerDropItem extends GameLabsContextAction {
        void CFCloud_PlayerDropItem() {
            this.actionCode = "CFCloud_PlayerDropItem";
            this.actionName = "Drop player item from hands";
            this.actionIcon = "hand-paper";
            this.actionColour = "default";
            this.actionContext = "player";
        }

        override bool Execute(GameLabsActionContext context) {
            PlayerBase player;
            PlayerBase.CastTo(player, context.GetReferencedObject());

            GetGameLabs().GetLogger().Warn(string.Format("[Player-DropItem] %1", player));
            ItemBase item = ItemBase.Cast(player.GetItemInHands());
            if(item) {
                player.DropItem(item);
            }
            return true;
        }
};