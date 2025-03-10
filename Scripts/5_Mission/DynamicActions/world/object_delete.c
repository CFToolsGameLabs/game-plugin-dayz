class CFCloud_ObjectDelete extends GameLabsContextAction {
        void CFCloud_ObjectDelete() {
            this.actionCode = "CFCloud_ObjectDelete";
            this.actionName = "Delete object";
            this.actionIcon = "trash";
            this.actionColour = "danger";
            this.actionContext = "object";
        }

        override bool Execute(GameLabsActionContext context) {
            _Event eventInstance;
            _Event.CastTo(eventInstance, context.GetReferencedObject());

            GetGameLabs().GetLogger().Warn(string.Format("[ObjectDelete] Deleting object %1", eventInstance.Ref()));

            eventInstance.Ref().Delete();
            return true;
        }
};