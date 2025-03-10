modded class Wreck_UH1Y extends CrashBase {
    ref _Event gl_registeredInstance = new _Event(this.GetType(), "helicopter", this, "UH1Y Wreck");

    override void EEInit() {
        super.EEInit();
        if(!GetGame().IsServer()) return;
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

modded class Wreck_Mi8 extends CrashBase {
        ref _Event gl_registeredInstance = new _Event(this.GetType(), "helicopter", this, "Mi8 Wreck");

        override void EEInit() {
            super.EEInit();
            if(!GetGame().IsServer()) return;
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

modded class Wreck_Mi8_Crashed extends CrashBase {
    ref _Event gl_registeredInstance = new _Event(this.GetType(), "helicopter", this, "Mi8 Wreck");

    override void EEInit() {
        super.EEInit();
        if(!GetGame().IsServer()) return;
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


// TODO: Investigate crash issue
modded class Wreck_SantasSleigh extends CrashBase {
    ref _Event gl_registeredInstance = new _Event(this.GetType(), "sleigh", this, "Santa's Sleigh");

    override void EEInit() {
        super.EEInit();
        if(!GetGame().IsServer()) return;
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
