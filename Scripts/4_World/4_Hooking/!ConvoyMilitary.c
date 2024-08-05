class Land_Wreck_V3S_DE extends House {
        ref _Event _registeredInstance = new _Event(this.GetType(), "bolt", this, "Military Convoy");

        override void EEInit() {
            super.EEInit();
            if(!GetGame().IsServer()) return;
            vector position = GetPosition();
            if(position[0] <= 0 && position[1] <= 0 && position[2] <= 0) return;
            GetGameLabs().RegisterEventRadiusExclusive(this._registeredInstance, 100.0);
        }
        override void EEDelete(EntityAI parent) {
            super.EEDelete(parent);
            if(!GetGame().IsServer()) return;
            if(!GetGameLabs()) return;
            if(this._registeredInstance) GetGameLabs().RemoveEvent(this._registeredInstance);
        }
};


class StaticObj_Wreck_BRDM_DE extends House {
        ref _Event _registeredInstance = new _Event(this.GetType(), "bolt", this, "Military Convoy");

        override void EEInit() {
            super.EEInit();
            if(!GetGame().IsServer()) return;
            vector position = GetPosition();
            if(position[0] <= 0 && position[1] <= 0 && position[2] <= 0) return;
            GetGameLabs().RegisterEventRadiusExclusive(this._registeredInstance, 100.0);
        }
        override void EEDelete(EntityAI parent) {
            super.EEDelete(parent);
            if(!GetGame().IsServer()) return;
            if(!GetGameLabs()) return;
            if(this._registeredInstance) GetGameLabs().RemoveEvent(this._registeredInstance);
        }
};


class Land_Wreck_offroad02_aban1_DE extends House {
        ref _Event _registeredInstance = new _Event(this.GetType(), "bolt", this, "Military Convoy");

        override void EEInit() {
            super.EEInit();
            if(!GetGame().IsServer()) return;
            vector position = GetPosition();
            if(position[0] <= 0 && position[1] <= 0 && position[2] <= 0) return;
            GetGameLabs().RegisterEventRadiusExclusive(this._registeredInstance, 100.0);
        }
        override void EEDelete(EntityAI parent) {
            super.EEDelete(parent);
            if(!GetGame().IsServer()) return;
            if(!GetGameLabs()) return;
            if(this._registeredInstance) GetGameLabs().RemoveEvent(this._registeredInstance);
        }
};


class Land_Wreck_offroad02_aban2_DE extends House {
        ref _Event _registeredInstance = new _Event(this.GetType(), "bolt", this, "Military Convoy");

        override void EEInit() {
            super.EEInit();
            if(!GetGame().IsServer()) return;
            vector position = GetPosition();
            if(position[0] <= 0 && position[1] <= 0 && position[2] <= 0) return;
            GetGameLabs().RegisterEventRadiusExclusive(this._registeredInstance, 100.0);
        }
        override void EEDelete(EntityAI parent) {
            super.EEDelete(parent);
            if(!GetGame().IsServer()) return;
            if(!GetGameLabs()) return;
            if(this._registeredInstance) GetGameLabs().RemoveEvent(this._registeredInstance);
        }
};
