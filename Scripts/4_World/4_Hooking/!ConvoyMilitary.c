class StaticObj_Wreck_Decal_Small2 extends House {
        ref _Event _registeredInstance = new _Event("Military Convoy", "bolt", this);

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
