class CFCloud_SetPlayerVitals extends GameLabsContextAction {
        void CFCloud_SetPlayerVitals() {
            this.actionCode = "CFCloud_SetPlayerVitals";
            this.actionName = "Set player vitals";
            this.actionIcon = "heart-rate";
            this.actionColour = "default";
            this.actionContext = "player";

            this.parameters.Insert("health", GameLabsActionParameter("Health", "Absolute health value, use -1 to leave unchanged", "float"));
            this.parameters.Insert("blood", GameLabsActionParameter("Blood", "Absolute blood value, use -1 to leave unchanged", "float"));
            this.parameters.Insert("shock", GameLabsActionParameter("Shock", "Absolute shock value, use -1 to leave unchanged", "float"));
            this.parameters.Insert("water", GameLabsActionParameter("Water", "Absolute water value, use -1 to leave unchanged", "float"));
            this.parameters.Insert("energy", GameLabsActionParameter("Energy", "Absolute energy value, use -1 to leave unchanged", "float"));
            this.parameters.Insert("stamina", GameLabsActionParameter("Stamina", "Absolute stamina value, use -1 to leave unchanged", "float"));
        }

        override bool Execute(GameLabsActionContext context) {
            PlayerBase player;
            PlayerBase.CastTo(player, context.GetReferencedObject());
            if(!player) return false;

            float health = context.parameters.Get("health").GetFloat();
            float blood = context.parameters.Get("blood").GetFloat();
            float shock = context.parameters.Get("shock").GetFloat();
            float water = context.parameters.Get("water").GetFloat();
            float energy = context.parameters.Get("energy").GetFloat();
            float stamina = context.parameters.Get("stamina").GetFloat();

            GetGameLabs().GetLogger().Warn(string.Format("[Vitals] %1 health=%2 blood=%3 shock=%4 water=%5 energy=%6 stamina=%7", player, health, blood, shock, water, energy, stamina));

            if(health >= 0) player.SetHealth("", "Health", health);
            if(blood >= 0) player.SetHealth("", "Blood", blood);
            if(shock >= 0) player.SetHealth("", "Shock", shock);
            if(water >= 0) player.GetStatWater().Set(water);
            if(energy >= 0) player.GetStatEnergy().Set(energy);
            if(stamina >= 0 && player.GetStaminaHandler()) player.GetStaminaHandler().SetStamina(stamina);

            return true;
        }
};
