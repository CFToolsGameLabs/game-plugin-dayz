class StaticObject_Wreck_Train_742_Blue_Mil_DE extends House {
        ref _Event _registeredInstance = new _Event(this.GetType(), "subway", this);

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

class StaticObject_Wreck_Train_742_Blue_DE extends House {
        ref _Event _registeredInstance = new _Event(this.GetType(), "subway", this);

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

class StaticObject_Wreck_Train_742_Blue extends House {
        ref _Event _registeredInstance = new _Event(this.GetType(), "subway", this);

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

class StaticObject_Wreck_Train_742_Red_Mil_DE extends House {
        ref _Event _registeredInstance = new _Event(this.GetType(), "subway", this);

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

class StaticObject_Wreck_Train_742_Red_DE extends House {
        ref _Event _registeredInstance = new _Event(this.GetType(), "subway", this);

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
