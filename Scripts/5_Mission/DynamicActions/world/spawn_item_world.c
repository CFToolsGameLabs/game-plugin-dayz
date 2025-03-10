class CFCloud_SpawnItemWorld extends GameLabsContextAction {
        void CFCloud_SpawnItemWorld() {
            this.actionCode = "CFCloud_SpawnItemWorld";
            this.actionName = "Spawn an item at coordinates";
            this.actionIcon = "gem";
            this.actionColour = "default";
            this.actionContext = "world";

            this.parameters.Insert("vector", GameLabsActionParameter("Coordinates", "World coordinates", "vector"));
            this.parameters.Insert("item", GameLabsActionParameter("Item class name", "Class name of item to be spawned", "cf_itemlist"));
            this.parameters.Insert("quantity", GameLabsActionParameter("Quantity", "Amount if items to be spawned", "int"));
            this.parameters.Insert("debug", GameLabsActionParameter("Debug spawn", "Use debug spawn method to automatically populate specific items", "boolean"));
            this.parameters.Insert("stacked", GameLabsActionParameter("Stacked", "Spawn items as a stack (only works if item supports to be stacked)", "boolean"));
        }

        override bool Execute(GameLabsActionContext context) {
            string itemType = context.parameters.Get("item").GetString();
            GetGameLabs().GetLogger().Warn(string.Format("[Spawn] Spawning %1 (x%2) at %3", itemType, context.parameters.Get("quantity").GetInt(), context.parameters.Get("vector").GetVector()));

            int entityFlags;
            if(GetGame().IsKindOf(itemType,"DZ_LightAI") || GetGame().IsKindOf(itemType, "SurvivorBase")) {
                entityFlags = ECE_INITAI | ECE_CREATEPHYSICS;
            } else {
                entityFlags = ECE_SETUP | ECE_KEEPHEIGHT | ECE_PLACE_ON_SURFACE;
            }

            EntityAI entity;
            if(context.parameters.Get("stacked").GetBoolean()) {
                entity = EntityAI.Cast(GetGame().CreateObjectEx(itemType, context.parameters.Get("vector").GetVector(), entityFlags));
                if (context.parameters.Get("debug").GetBoolean()) {
                    entity.OnDebugSpawn();
                }
                ItemBase itemBase;
                ItemBase.CastTo(itemBase, entity);
                itemBase.SetQuantity(context.parameters.Get("quantity").GetInt());
            } else {
                for (int i = 1; i <= context.parameters.Get("quantity").GetInt(); i++) {
                    entity = EntityAI.Cast(GetGame().CreateObjectEx(itemType, context.parameters.Get("vector").GetVector(), entityFlags));
                    if (context.parameters.Get("debug").GetBoolean()) {
                        entity.OnDebugSpawn();
                    }
                }
            }
            return true;
        }
};

