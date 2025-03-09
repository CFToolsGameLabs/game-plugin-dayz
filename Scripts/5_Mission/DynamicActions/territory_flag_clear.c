class CFCloud_TerritoryFlagClear extends GameLabsContextAction {
        void CFCloud_TerritoryFlagClear() {
            this.actionCode = "CFCloud_TerritoryFlagClear";
            this.actionName = "Clear territory (Server restart required)";
            this.actionIcon = "pennant";
            this.actionColour = "warning";
            this.actionContext = "object";
            this.actionContextFilter = new array<string>;
            this.actionContextFilter.Insert("TerritoryFlag");
        }

        override bool Execute(GameLabsActionContext context) {
            _Event eventInstance;
            _Event.CastTo(eventInstance, context.GetReferencedObject());
            GetGameLabs().GetLogger().Warn(string.Format("[TerritoryFlagClear] Clearing flag and everything in radius for %1<%2>", eventInstance.Ref(), eventInstance));

            TerritoryFlag totem;
            TerritoryFlag.CastTo(totem, eventInstance.Ref());

            GetCEApi().RadiusLifetimeDecrease(totem.GetPosition(), GameConstants.REFRESHER_RADIUS, totem.GetEconomyProfile().GetLifetime() + 1);
            totem.Delete();
            GetCEApi().OnUpdate();
            return true;
        }
};