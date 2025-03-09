class StaticObj_Wreck_Train_742_Blue_DE extends House {
        ref _Event gl_registeredInstance = new _Event(this.GetType(), "subway", this, "Train Wreck");

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

class StaticObj_Wreck_Train_742_Red_DE extends House {
        ref _Event gl_registeredInstance = new _Event(this.GetType(), "subway", this, "Train Wreck");

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

class StaticObj_Wreck_Train_742_Red_Mil_DE extends House {
        ref _Event gl_registeredInstance = new _Event(this.GetType(), "subway", this, "Military Train Wreck");

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

class StaticObj_Wreck_Train_742_Blue_Mil_DE extends House {
        ref _Event gl_registeredInstance = new _Event(this.GetType(), "subway", this, "Military Train Wreck");

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