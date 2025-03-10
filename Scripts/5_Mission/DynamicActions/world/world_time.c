class CFCloud_WorldTime extends GameLabsContextAction {
        void CFCloud_WorldTime() {
            this.actionCode = "CFCloud_WorldTime";
            this.actionName = "Update world time";
            this.actionIcon = "clock";
            this.actionColour = "default";
            this.actionContext = "world";

            // Set default value for parameter
            GameLabsActionParameter hour = new GameLabsActionParameter("Hour", "", "int");
            hour.valueInt = 12;
            this.parameters.Insert("hour", hour);

            this.parameters.Insert("minute", GameLabsActionParameter("Minute", "", "int"));
        }

        override bool Execute(GameLabsActionContext context) {
            GetGameLabs().GetLogger().Warn(string.Format("[Time] Updating game time hour=%1, minute=%2", context.parameters.Get("hour").GetInt(), context.parameters.Get("minute").GetInt()));

            int year, month, day, hour, minute;
            GetGame().GetWorld().GetDate(year, month, day, hour, minute);
            GetGame().GetWorld().SetDate(year, month, day, context.parameters.Get("hour").GetInt(), context.parameters.Get("minute").GetInt());
            return true;
        }
};