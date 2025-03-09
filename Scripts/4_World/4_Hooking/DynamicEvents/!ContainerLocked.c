modded class Land_ContainerLocked_Blue_DE : ContainerLockedBase {
    ref _Event gl_registeredInstance = new _Event(this.GetType(), "container-storage", this, "Locked Container");

    override void EEInit() {
        super.EEInit();
        if(!GetGame().IsServer()) return;
        if(!GetGameLabs()) return;
        vector position = GetPosition();
        if(position[0] <= 0 && position[1] <= 0 && position[2] <= 0) return;
        GetGameLabs().RegisterEvent(this.gl_registeredInstance);
    }
    override void EEDelete(EntityAI parent) {
        super.EEDelete(parent);
        if(!GetGame().IsServer()) return;
        if(!GetGameLabs()) return;
        if(this.gl_registeredInstance) GetGameLabs().RemoveEvent(this.gl_registeredInstance);
    }
};

modded class Land_ContainerLocked_Yellow_DE  : ContainerLockedBase {
    ref _Event gl_registeredInstance = new _Event(this.GetType(), "container-storage", this, "Locked Container");

    override void EEInit() {
        super.EEInit();
        if(!GetGame().IsServer()) return;
        if(!GetGameLabs()) return;
        vector position = GetPosition();
        if(position[0] <= 0 && position[1] <= 0 && position[2] <= 0) return;
        GetGameLabs().RegisterEvent(this.gl_registeredInstance);
    }
    override void EEDelete(EntityAI parent) {
        super.EEDelete(parent);
        if(!GetGame().IsServer()) return;
        if(!GetGameLabs()) return;
        if(this.gl_registeredInstance) GetGameLabs().RemoveEvent(this.gl_registeredInstance);
    }
};

modded class Land_ContainerLocked_Orange_DE  : ContainerLockedBase {
    ref _Event gl_registeredInstance = new _Event(this.GetType(), "container-storage", this, "Locked Container");

    override void EEInit() {
        super.EEInit();
        if(!GetGame().IsServer()) return;
        if(!GetGameLabs()) return;
        vector position = GetPosition();
        if(position[0] <= 0 && position[1] <= 0 && position[2] <= 0) return;
        GetGameLabs().RegisterEvent(this.gl_registeredInstance);
    }
    override void EEDelete(EntityAI parent) {
        super.EEDelete(parent);
        if(!GetGame().IsServer()) return;
        if(!GetGameLabs()) return;
        if(this.gl_registeredInstance) GetGameLabs().RemoveEvent(this.gl_registeredInstance);
    }
};

modded class Land_ContainerLocked_Red_DE  : ContainerLockedBase {
    ref _Event gl_registeredInstance = new _Event(this.GetType(), "container-storage", this, "Locked Container");

    override void EEInit() {
        super.EEInit();
        if(!GetGame().IsServer()) return;
        if(!GetGameLabs()) return;
        vector position = GetPosition();
        if(position[0] <= 0 && position[1] <= 0 && position[2] <= 0) return;
        GetGameLabs().RegisterEvent(this.gl_registeredInstance);
    }
    override void EEDelete(EntityAI parent) {
        super.EEDelete(parent);
        if(!GetGame().IsServer()) return;
        if(!GetGameLabs()) return;
        if(this.gl_registeredInstance) GetGameLabs().RemoveEvent(this.gl_registeredInstance);
    }
};