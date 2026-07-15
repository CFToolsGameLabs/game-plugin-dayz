class Land_Wreck_V3S_DE extends House {
        ref _Event gl_registeredInstance = new _Event(this.GetType(), "bolt", this, "Military Convoy");

        override void EEInit() {
            super.EEInit();
            if(!GetGame().IsServer()) return;
            vector position = GetPosition();
            if(position[0] <= 0 && position[1] <= 0 && position[2] <= 0) return;
            GetGameLabs().RegisterEventRadiusExclusive(this.gl_registeredInstance, 100.0);
        }
        override void EEDelete(EntityAI parent) {
            super.EEDelete(parent);
            if(!GetGame().IsServer()) return;
            if(!GetGameLabs()) return;
            if(this.gl_registeredInstance) GetGameLabs().RemoveEvent(this.gl_registeredInstance);
        }
};


class StaticObj_Wreck_BRDM_DE extends House {
        ref _Event gl_registeredInstance = new _Event(this.GetType(), "bolt", this, "Military Convoy");

        override void EEInit() {
            super.EEInit();
            if(!GetGame().IsServer()) return;
            vector position = GetPosition();
            if(position[0] <= 0 && position[1] <= 0 && position[2] <= 0) return;
            GetGameLabs().RegisterEventRadiusExclusive(this.gl_registeredInstance, 100.0);
        }
        override void EEDelete(EntityAI parent) {
            super.EEDelete(parent);
            if(!GetGame().IsServer()) return;
            if(!GetGameLabs()) return;
            if(this.gl_registeredInstance) GetGameLabs().RemoveEvent(this.gl_registeredInstance);
        }
};


class Land_Wreck_offroad02_aban1_DE extends House {
        ref _Event gl_registeredInstance = new _Event(this.GetType(), "bolt", this, "Military Convoy");

        override void EEInit() {
            super.EEInit();
            if(!GetGame().IsServer()) return;
            vector position = GetPosition();
            if(position[0] <= 0 && position[1] <= 0 && position[2] <= 0) return;
            GetGameLabs().RegisterEventRadiusExclusive(this.gl_registeredInstance, 100.0);
        }
        override void EEDelete(EntityAI parent) {
            super.EEDelete(parent);
            if(!GetGame().IsServer()) return;
            if(!GetGameLabs()) return;
            if(this.gl_registeredInstance) GetGameLabs().RemoveEvent(this.gl_registeredInstance);
        }
};


class Land_Wreck_offroad02_aban2_DE extends House {
        ref _Event gl_registeredInstance = new _Event(this.GetType(), "bolt", this, "Military Convoy");

        override void EEInit() {
            super.EEInit();
            if(!GetGame().IsServer()) return;
            vector position = GetPosition();
            if(position[0] <= 0 && position[1] <= 0 && position[2] <= 0) return;
            GetGameLabs().RegisterEventRadiusExclusive(this.gl_registeredInstance, 100.0);
        }
        override void EEDelete(EntityAI parent) {
            super.EEDelete(parent);
            if(!GetGame().IsServer()) return;
            if(!GetGameLabs()) return;
            if(this.gl_registeredInstance) GetGameLabs().RemoveEvent(this.gl_registeredInstance);
        }
};

class Land_Wreck_Offroad02_Aban1_TK_DE extends House {
    ref _Event gl_registeredInstance = new _Event(this.GetType(), "bolt", this, "Military Convoy");

    override void EEInit() {
        super.EEInit();
        if(!GetGame().IsServer()) return;
        vector position = GetPosition();
        if(position[0] <= 0 && position[1] <= 0 && position[2] <= 0) return;
        GetGameLabs().RegisterEventRadiusExclusive(this.gl_registeredInstance, 100.0);
    }
    override void EEDelete(EntityAI parent) {
        super.EEDelete(parent);
        if(!GetGame().IsServer()) return;
        if(!GetGameLabs()) return;
        if(this.gl_registeredInstance) GetGameLabs().RemoveEvent(this.gl_registeredInstance);
    }
};


class Land_Wreck_Offroad02_Aban2_TK_DE extends House {
    ref _Event gl_registeredInstance = new _Event(this.GetType(), "bolt", this, "Military Convoy");

    override void EEInit() {
        super.EEInit();
        if(!GetGame().IsServer()) return;
        vector position = GetPosition();
        if(position[0] <= 0 && position[1] <= 0 && position[2] <= 0) return;
        GetGameLabs().RegisterEventRadiusExclusive(this.gl_registeredInstance, 100.0);
    }
    override void EEDelete(EntityAI parent) {
        super.EEDelete(parent);
        if(!GetGame().IsServer()) return;
        if(!GetGameLabs()) return;
        if(this.gl_registeredInstance) GetGameLabs().RemoveEvent(this.gl_registeredInstance);
    }
};


class Land_Wreck_Offroad02_Medic_TK_DE extends House {
    ref _Event gl_registeredInstance = new _Event(this.GetType(), "bolt", this, "Military Convoy");

    override void EEInit() {
        super.EEInit();
        if(!GetGame().IsServer()) return;
        vector position = GetPosition();
        if(position[0] <= 0 && position[1] <= 0 && position[2] <= 0) return;
        GetGameLabs().RegisterEventRadiusExclusive(this.gl_registeredInstance, 100.0);
    }
    override void EEDelete(EntityAI parent) {
        super.EEDelete(parent);
        if(!GetGame().IsServer()) return;
        if(!GetGameLabs()) return;
        if(this.gl_registeredInstance) GetGameLabs().RemoveEvent(this.gl_registeredInstance);
    }
};


class StaticObj_Wreck_Stryker_DE extends House {
    ref _Event gl_registeredInstance = new _Event(this.GetType(), "bolt", this, "Military Convoy");

    override void EEInit() {
        super.EEInit();
        if(!GetGame().IsServer()) return;
        vector position = GetPosition();
        if(position[0] <= 0 && position[1] <= 0 && position[2] <= 0) return;
        GetGameLabs().RegisterEventRadiusExclusive(this.gl_registeredInstance, 100.0);
    }
    override void EEDelete(EntityAI parent) {
        super.EEDelete(parent);
        if(!GetGame().IsServer()) return;
        if(!GetGameLabs()) return;
        if(this.gl_registeredInstance) GetGameLabs().RemoveEvent(this.gl_registeredInstance);
    }
};


class StaticObj_Wreck_MTVR_Cargo_DE extends House {
    ref _Event gl_registeredInstance = new _Event(this.GetType(), "bolt", this, "Military Convoy");

    override void EEInit() {
        super.EEInit();
        if(!GetGame().IsServer()) return;
        vector position = GetPosition();
        if(position[0] <= 0 && position[1] <= 0 && position[2] <= 0) return;
        GetGameLabs().RegisterEventRadiusExclusive(this.gl_registeredInstance, 100.0);
    }
    override void EEDelete(EntityAI parent) {
        super.EEDelete(parent);
        if(!GetGame().IsServer()) return;
        if(!GetGameLabs()) return;
        if(this.gl_registeredInstance) GetGameLabs().RemoveEvent(this.gl_registeredInstance);
    }
};
