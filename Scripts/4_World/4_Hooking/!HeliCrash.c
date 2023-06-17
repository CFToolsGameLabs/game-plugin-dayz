modded class Wreck_UH1Y extends CrashBase {
    ref _Event _registeredInstance = new _Event(this.GetType(), "helicopter", this);

    override void EEInit() {
        super.EEInit();
        if(!GetGame().IsServer()) return;
        vector position = GetPosition();
        if(position[0] <= 0 && position[1] <= 0 && position[2] <= 0) return;
        GetGameLabs().RegisterEvent(this._registeredInstance);
    }
    override void EEDelete(EntityAI parent) {
        super.EEDelete(parent);
        if(!GetGame().IsServer()) return;
        if(!GetGameLabs()) return;
        if(this._registeredInstance) GetGameLabs().RemoveEvent(this._registeredInstance);
    }
};

modded class Wreck_Mi8 extends CrashBase {
        ref _Event _registeredInstance = new _Event(this.GetType(), "helicopter", this);

        override void EEInit() {
            super.EEInit();
            if(!GetGame().IsServer()) return;
            vector position = GetPosition();
            if(position[0] <= 0 && position[1] <= 0 && position[2] <= 0) return;
            GetGameLabs().RegisterEvent(this._registeredInstance);
        }
        override void EEDelete(EntityAI parent) {
            super.EEDelete(parent);
            if(!GetGame().IsServer()) return;
            if(!GetGameLabs()) return;
            if(this._registeredInstance) GetGameLabs().RemoveEvent(this._registeredInstance);
        }
};

modded class Wreck_Mi8_Crashed extends CrashBase {
    ref _Event _registeredInstance = new _Event(this.GetType(), "helicopter", this);

    override void EEInit() {
        super.EEInit();
        if(!GetGame().IsServer()) return;
        vector position = GetPosition();
        if(position[0] <= 0 && position[1] <= 0 && position[2] <= 0) return;
        GetGameLabs().RegisterEvent(this._registeredInstance);
    }
    override void EEDelete(EntityAI parent) {
        super.EEDelete(parent);
        if(!GetGame().IsServer()) return;
        if(!GetGameLabs()) return;
        if(this._registeredInstance) GetGameLabs().RemoveEvent(this._registeredInstance);
    }
};


// TODO: Investigate crash issue
modded class Wreck_SantasSleigh extends CrashBase {
    ref _Event _registeredInstance = new _Event(this.GetType(), "sleigh", this);

    override void EEInit() {
        super.EEInit();
        if(!GetGame().IsServer()) return;
        vector position = GetPosition();
        if(position[0] <= 0 && position[1] <= 0 && position[2] <= 0) return;
        GetGameLabs().RegisterEvent(this._registeredInstance);
    }
    override void EEDelete(EntityAI parent) {
        super.EEDelete(parent);
        if(!GetGame().IsServer()) return;
        if(!GetGameLabs()) return;
        if(this._registeredInstance) GetGameLabs().RemoveEvent(this._registeredInstance);
    }
};
