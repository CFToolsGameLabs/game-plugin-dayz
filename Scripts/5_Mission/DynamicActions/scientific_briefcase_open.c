class CFCloud_ScientificBriefcaseOpen extends GameLabsContextAction {
        void CFCloud_ScientificBriefcaseOpen() {
            this.actionCode = "CFCloud_ScientificBriefcaseOpen";
            this.actionName = "Open a locked scientific briefcase";
            this.actionIcon = "key-skeleton";
            this.actionColour = "default";
            this.actionContext = "object";
            this.actionContextFilter = new array<string>;
            this.actionContextFilter.Insert("ScientificBriefcase");
        }

        override bool Execute(GameLabsActionContext context) {
            _Event eventInstance;
            _Event.CastTo(eventInstance, context.GetReferencedObject());
            GetGameLabs().GetLogger().Warn(string.Format("[ScientificBriefcaseOpen] Opening container %1<%2>", eventInstance.Ref(), eventInstance));

            ScientificBriefcase container;
            ScientificBriefcase.CastTo(container, eventInstance.Ref());

            container.Open();
            return true;
        }
};