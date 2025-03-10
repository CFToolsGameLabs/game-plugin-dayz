class Land_Wreck_hb01_aban1_police extends House {
    ref _Event gl_registeredInstance = new _Event(this.GetType(), "car", this, "Police Wreck");

    override void EEInit() {
        super.EEInit();
        if(!GetGameLabs()) return;
        if(!GetGameLabs().IsServer()) return;
        vector position = GetPosition();
        if(position[0] <= 0 && position[1] <= 0 && position[2] <= 0) return;
        GetGameLabs().RegisterEventRadiusExclusiveSecondary(this.gl_registeredInstance, 50.0, "StaticObj_Wreck_Decal_Small1");
    }
    override void EEDelete(EntityAI parent) {
        super.EEDelete(parent);
        if(!GetGameLabs()) return;
        if(!GetGameLabs().IsServer()) return;
        if(this.gl_registeredInstance) GetGameLabs().RemoveEvent(this.gl_registeredInstance);
    }
};

class Land_Wreck_hb01_aban2_police extends House {
    ref _Event gl_registeredInstance = new _Event(this.GetType(), "car", this, "Police Wreck");

    override void EEInit() {
        super.EEInit();
        if(!GetGameLabs()) return;
        if(!GetGameLabs().IsServer()) return;
        vector position = GetPosition();
        if(position[0] <= 0 && position[1] <= 0 && position[2] <= 0) return;
        GetGameLabs().RegisterEventRadiusExclusiveSecondary(this.gl_registeredInstance, 50.0, "StaticObj_Wreck_Decal_Small1");
    }
    override void EEDelete(EntityAI parent) {
        super.EEDelete(parent);
        if(!GetGameLabs()) return;
        if(!GetGameLabs().IsServer()) return;
        if(this.gl_registeredInstance) GetGameLabs().RemoveEvent(this.gl_registeredInstance);
    }
};

class Land_Wreck_sed01_aban1_police extends House {
    ref _Event gl_registeredInstance = new _Event(this.GetType(), "car", this, "Police Wreck");

    override void EEInit() {
        super.EEInit();
        if(!GetGameLabs()) return;
        if(!GetGameLabs().IsServer()) return;
        vector position = GetPosition();
        if(position[0] <= 0 && position[1] <= 0 && position[2] <= 0) return;
        GetGameLabs().RegisterEventRadiusExclusiveSecondary(this.gl_registeredInstance, 50.0, "StaticObj_Wreck_Decal_Small1");
    }
    override void EEDelete(EntityAI parent) {
        super.EEDelete(parent);
        if(!GetGameLabs()) return;
        if(!GetGameLabs().IsServer()) return;
        if(this.gl_registeredInstance) GetGameLabs().RemoveEvent(this.gl_registeredInstance);
    }
};

class Land_Wreck_sed01_aban2_police extends House {
    ref _Event gl_registeredInstance = new _Event(this.GetType(), "car", this, "Police Wreck");

    override void EEInit() {
        super.EEInit();
        if(!GetGameLabs()) return;
        if(!GetGameLabs().IsServer()) return;
        vector position = GetPosition();
        if(position[0] <= 0 && position[1] <= 0 && position[2] <= 0) return;
        GetGameLabs().RegisterEventRadiusExclusiveSecondary(this.gl_registeredInstance, 50.0, "StaticObj_Wreck_Decal_Small1");
    }
    override void EEDelete(EntityAI parent) {
        super.EEDelete(parent);
        if(!GetGameLabs()) return;
        if(!GetGameLabs().IsServer()) return;
        if(this.gl_registeredInstance) GetGameLabs().RemoveEvent(this.gl_registeredInstance);
    }
};

class Land_Wreck_sed02_aban1_police extends House {
        ref _Event gl_registeredInstance = new _Event(this.GetType(), "car", this, "Police Wreck");

        override void EEInit() {
            super.EEInit();
            if(!GetGameLabs()) return;
            if(!GetGameLabs().IsServer()) return;
            vector position = GetPosition();
            if(position[0] <= 0 && position[1] <= 0 && position[2] <= 0) return;
            GetGameLabs().RegisterEventRadiusExclusiveSecondary(this.gl_registeredInstance, 50.0, "StaticObj_Wreck_Decal_Small1");
        }
        override void EEDelete(EntityAI parent) {
            super.EEDelete(parent);
            if(!GetGameLabs()) return;
            if(!GetGameLabs().IsServer()) return;
            if(this.gl_registeredInstance) GetGameLabs().RemoveEvent(this.gl_registeredInstance);
        }
};

class Land_Wreck_sed02_aban2_police extends House {
        ref _Event gl_registeredInstance = new _Event(this.GetType(), "car", this, "Police Wreck");

        override void EEInit() {
            super.EEInit();
            if(!GetGameLabs()) return;
            if(!GetGameLabs().IsServer()) return;
            vector position = GetPosition();
            if(position[0] <= 0 && position[1] <= 0 && position[2] <= 0) return;
            GetGameLabs().RegisterEventRadiusExclusiveSecondary(this.gl_registeredInstance, 50.0, "StaticObj_Wreck_Decal_Small1");
        }
        override void EEDelete(EntityAI parent) {
            super.EEDelete(parent);
            if(!GetGameLabs()) return;
            if(!GetGameLabs().IsServer()) return;
            if(this.gl_registeredInstance) GetGameLabs().RemoveEvent(this.gl_registeredInstance);
        }
};

class Land_Wreck_Volha_Police extends House {
    ref _Event gl_registeredInstance = new _Event(this.GetType(), "car", this, "Police Wreck");

    override void EEInit() {
        super.EEInit();
        if(!GetGame().IsServer()) return;
        if(!GetGameLabs()) return;
        vector position = GetPosition();
        if(position[0] <= 0 && position[1] <= 0 && position[2] <= 0) return;
        GetGameLabs().RegisterEventRadiusExclusiveSecondary(this.gl_registeredInstance, 50.0, "StaticObj_Wreck_Decal_Small1");
    }
    override void EEDelete(EntityAI parent) {
        super.EEDelete(parent);
        if(!GetGame().IsServer()) return;
        if(!GetGameLabs()) return;
        if(this.gl_registeredInstance) GetGameLabs().RemoveEvent(this.gl_registeredInstance);
    }
};