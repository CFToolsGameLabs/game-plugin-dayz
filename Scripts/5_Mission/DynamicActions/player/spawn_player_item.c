class CFCloud_SpawnPlayerItem extends GameLabsContextAction {
        void CFCloud_SpawnPlayerItem() {
            this.actionCode = "CFCloud_SpawnPlayerItem";
            this.actionName = "Spawn an item at player position";
            this.actionIcon = "gem";
            this.actionColour = "default";
            this.actionContext = "player";

            this.parameters.Insert("item", GameLabsActionParameter("Item class name", "Class name of item to be spawned", "cf_itemlist"));
            this.parameters.Insert("quantity", GameLabsActionParameter("Quantity", "Amount if items to be spawned", "int"));
            this.parameters.Insert("debug", GameLabsActionParameter("Debug spawn", "Use debug spawn method to automatically populate specific items", "boolean"));
            this.parameters.Insert("stacked", GameLabsActionParameter("Stacked", "Spawn items as a stack (only works if item supports to be stacked)", "boolean"));
        }

        override bool Execute(GameLabsActionContext context) {
            PlayerBase player;
            PlayerBase.CastTo(player, context.GetReferencedObject());

            GetGameLabs().GetLogger().Warn(string.Format("[Spawn] Spawning %1 (x%2) for %3", context.parameters.Get("item").GetString(), context.parameters.Get("quantity").GetInt(), player));

            EntityAI entity;
            if(context.parameters.Get("stacked").GetBoolean()) {
                entity = player.SpawnEntityOnGroundPos(context.parameters.Get("item").GetString(), player.GetPosition());
                if (context.parameters.Get("debug").GetBoolean()) {
                    entity.OnDebugSpawn();
                }
                ItemBase itemBase;
                ItemBase.CastTo(itemBase, entity);
                itemBase.SetQuantity(context.parameters.Get("quantity").GetInt());
            } else {
                for (int i = 1; i <= context.parameters.Get("quantity").GetInt(); i++) {
                    entity = player.SpawnEntityOnGroundPos(context.parameters.Get("item").GetString(), player.GetPosition());
                    if (context.parameters.Get("debug").GetBoolean()) {
                        entity.OnDebugSpawn();
                    }
                }
            }
            return true;
        }
};