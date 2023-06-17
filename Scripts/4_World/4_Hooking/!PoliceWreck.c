class Land_Wreck_hb01_aban1_police extends House {
    ref _Event _registeredInstance = new _Event("Police Wreck", "car", this);

    override void EEInit() {
        super.EEInit();
        if(!GetGameLabs()) return;
        if(!GetGameLabs().IsServer()) return;
        vector position = GetPosition();
        if(position[0] <= 0 && position[1] <= 0 && position[2] <= 0) return;
        GetGameLabs().RegisterEventRadiusExclusiveSecondary(this._registeredInstance, 50.0, "StaticObj_Wreck_Decal_Small1");
    }
    override void EEDelete(EntityAI parent) {
        super.EEDelete(parent);
        if(!GetGameLabs()) return;
        if(!GetGameLabs().IsServer()) return;
        if(this._registeredInstance) GetGameLabs().RemoveEvent(this._registeredInstance);
    }
};

class Land_Wreck_hb01_aban2_police extends House {
    ref _Event _registeredInstance = new _Event("Police Wreck", "car", this);

    override void EEInit() {
        super.EEInit();
        if(!GetGameLabs()) return;
        if(!GetGameLabs().IsServer()) return;
        vector position = GetPosition();
        if(position[0] <= 0 && position[1] <= 0 && position[2] <= 0) return;
        GetGameLabs().RegisterEventRadiusExclusiveSecondary(this._registeredInstance, 50.0, "StaticObj_Wreck_Decal_Small1");
    }
    override void EEDelete(EntityAI parent) {
        super.EEDelete(parent);
        if(!GetGameLabs()) return;
        if(!GetGameLabs().IsServer()) return;
        if(this._registeredInstance) GetGameLabs().RemoveEvent(this._registeredInstance);
    }
};

class Land_Wreck_sed01_aban1_police extends House {
    ref _Event _registeredInstance = new _Event("Police Wreck", "car", this);

    override void EEInit() {
        super.EEInit();
        if(!GetGameLabs()) return;
        if(!GetGameLabs().IsServer()) return;
        vector position = GetPosition();
        if(position[0] <= 0 && position[1] <= 0 && position[2] <= 0) return;
        GetGameLabs().RegisterEventRadiusExclusiveSecondary(this._registeredInstance, 50.0, "StaticObj_Wreck_Decal_Small1");
    }
    override void EEDelete(EntityAI parent) {
        super.EEDelete(parent);
        if(!GetGameLabs()) return;
        if(!GetGameLabs().IsServer()) return;
        if(this._registeredInstance) GetGameLabs().RemoveEvent(this._registeredInstance);
    }
};

class Land_Wreck_sed01_aban2_police extends House {
    ref _Event _registeredInstance = new _Event("Police Wreck", "car", this);

    override void EEInit() {
        super.EEInit();
        if(!GetGameLabs()) return;
        if(!GetGameLabs().IsServer()) return;
        vector position = GetPosition();
        if(position[0] <= 0 && position[1] <= 0 && position[2] <= 0) return;
        GetGameLabs().RegisterEventRadiusExclusiveSecondary(this._registeredInstance, 50.0, "StaticObj_Wreck_Decal_Small1");
    }
    override void EEDelete(EntityAI parent) {
        super.EEDelete(parent);
        if(!GetGameLabs()) return;
        if(!GetGameLabs().IsServer()) return;
        if(this._registeredInstance) GetGameLabs().RemoveEvent(this._registeredInstance);
    }
};

class Land_Wreck_Volha_Police extends House {
    ref _Event _registeredInstance = new _Event("Police Wreck", "car", this);

    override void EEInit() {
        super.EEInit();
        if(!GetGame().IsServer()) return;
        if(!GetGameLabs()) return;
        vector position = GetPosition();
        if(position[0] <= 0 && position[1] <= 0 && position[2] <= 0) return;
        GetGameLabs().RegisterEventRadiusExclusiveSecondary(this._registeredInstance, 50.0, "StaticObj_Wreck_Decal_Small1");
    }
    override void EEDelete(EntityAI parent) {
        super.EEDelete(parent);
        if(!GetGame().IsServer()) return;
        if(!GetGameLabs()) return;
        if(this._registeredInstance) GetGameLabs().RemoveEvent(this._registeredInstance);
    }
};