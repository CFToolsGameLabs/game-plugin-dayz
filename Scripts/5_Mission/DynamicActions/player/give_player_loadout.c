class CFCloud_GivePlayerLoadout extends GameLabsContextAction {
        void CFCloud_GivePlayerLoadout() {
            this.actionCode = "CFCloud_GivePlayerLoadout";
            this.actionName = "Give player loadout";
            this.actionIcon = "tshirt";
            this.actionColour = "default";
            this.actionContext = "player";

            GameLabsActionParameter preset = new GameLabsActionParameter("Loadout preset", "Preset to grant to the player", "options");

            array<ref GLLoadoutPreset> presets = GetGameLabs().GetLoadoutManager().GetPresets();
            for(int i = 0; i < presets.Count(); i++) {
                string presetName = presets.Get(i).name;
                GameLabsActionParameter option = new GameLabsActionParameter(presetName, "", "string");
                option.valueString = presetName;
                preset.options.Insert(presetName, option);
            }

            this.parameters.Insert("preset", preset);
            this.parameters.Insert("strip", GameLabsActionParameter("Strip existing gear", "Remove all current items before granting the loadout", "boolean"));
        }

        override bool Execute(GameLabsActionContext context) {
            PlayerBase player;
            PlayerBase.CastTo(player, context.GetReferencedObject());
            if(!player) return false;

            string presetName = context.parameters.Get("preset").GetString();
            GLLoadoutPreset preset = GetGameLabs().GetLoadoutManager().GetPreset(presetName);
            if(!preset) {
                GetGameLabs().GetLogger().Warn(string.Format("[Loadout] Preset \"%1\" not found for %2", presetName, player));
                return false;
            }

            GetGameLabs().GetLogger().Warn(string.Format("[Loadout] Granting preset \"%1\" (%2 items) to %3", presetName, preset.items.Count(), player));

            if(context.parameters.Get("strip").GetBoolean()) {
                player.RemoveAllItems();
            }

            for(int i = 0; i < preset.items.Count(); i++) {
                GLLoadoutItem item = preset.items.Get(i);
                EntityAI entity = player.GetHumanInventory().CreateInInventory(item.className);
                this.ApplyItem(entity, item);
            }

            return true;
        }

        private void ApplyItem(EntityAI entity, GLLoadoutItem definition) {
            if(!entity) {
                GetGameLabs().GetLogger().Warn(string.Format("[Loadout] Failed to create item \"%1\"", definition.className));
                return;
            }

            if(definition.quantity >= 0) {
                ItemBase itemBase;
                if(ItemBase.CastTo(itemBase, entity)) {
                    itemBase.SetQuantity(definition.quantity);
                }
            }

            if(definition.health >= 0) {
                entity.SetHealth01("", "", definition.health);
            }

            if(definition.attachments) {
                for(int a = 0; a < definition.attachments.Count(); a++) {
                    GLLoadoutItem attachmentDef = definition.attachments.Get(a);
                    EntityAI attachment = entity.GetInventory().CreateAttachment(attachmentDef.className);
                    this.ApplyItem(attachment, attachmentDef);
                }
            }

            if(definition.cargo) {
                for(int c = 0; c < definition.cargo.Count(); c++) {
                    GLLoadoutItem cargoDef = definition.cargo.Get(c);
                    EntityAI cargo = entity.GetInventory().CreateInInventory(cargoDef.className);
                    this.ApplyItem(cargo, cargoDef);
                }
            }
        }
};
