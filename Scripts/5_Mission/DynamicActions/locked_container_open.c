class CFCloud_LockedContainerOpen extends GameLabsContextAction {
        void CFCloud_LockedContainerOpen() {
            this.actionCode = "CFCloud_LockedContainerOpen";
            this.actionName = "Open a locked container";
            this.actionIcon = "key-skeleton";
            this.actionColour = "default";
            this.actionContext = "object";
            this.actionContextFilter = new array<string>;
            this.actionContextFilter.Insert("Land_ContainerLocked_Blue_DE");
            this.actionContextFilter.Insert("Land_ContainerLocked_Yellow_DE");
            this.actionContextFilter.Insert("Land_ContainerLocked_Orange_DE");
            this.actionContextFilter.Insert("Land_ContainerLocked_Red_DE");
        }

        override bool Execute(GameLabsActionContext context) {
            _Event eventInstance;
            _Event.CastTo(eventInstance, context.GetReferencedObject());
            GetGameLabs().GetLogger().Warn(string.Format("[LockedContainerOpen] Opening container %1<%2>", eventInstance.Ref(), eventInstance));

            ContainerLockedBase container;
            ContainerLockedBase.CastTo(container, eventInstance.Ref());

            int i;
            int count = container.GetDoorCount();

            for (i = 0; i < count; i++) {
                container.UnlockDoor(i);
            }

            for (i = 0; i < count; i++) {
                if(!container.IsDoorLocked(i)) {
                    container.OpenDoor(i);
                }
            }
            return true;
        }

        /* 4_world/classes/useractionscomponent/actions/continuous/actionunlockcontainerdoor.c */
        private int TranslateLockSelectionIntoDoorIdx(ActionTarget target) {
            //side1_lock
            ContainerLockedBase shipCont;
            if(Class.CastTo(shipCont, target.GetObject())) {
                string selectionName = shipCont.GetActionComponentName( target.GetComponentIndex() );

                if (selectionName.Contains("_lock"))
                    return (selectionName.Substring(4,1).ToInt() - 1);
            }
            return -1;
        }
};