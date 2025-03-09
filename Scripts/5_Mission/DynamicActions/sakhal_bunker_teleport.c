class CFCloud_SakhalBunkerTeleport extends GameLabsContextAction {
        void CFCloud_SakhalBunkerTeleport() {
            this.actionCode = "CFCloud_SakhalBunkerTeleport";
            this.actionName = "Sakhal Bunker Teleport";
            this.actionIcon = "map-marker-alt";
            this.actionColour = "default";
            this.actionContext = "player";

            GameLabsActionParameter teleportLocation = new GameLabsActionParameter("Teleport to Bunker POI", "", "options");

            vector generatorVector = {3004.38,33.2784,5852.18};
            GameLabsActionParameter generator = new GameLabsActionParameter("Generator", "", "vector");
            generator.valueVectorX = generatorVector[0];
            generator.valueVectorZ = generatorVector[1];
            generator.valueVectorY = generatorVector[2];
            teleportLocation.options.Insert("generator", generator);

            vector room1Vector = {2566.97,15.2524,5954.86};
            GameLabsActionParameter room1 = new GameLabsActionParameter("Room A-1", "", "vector");
            room1.valueVectorX = room1Vector[0];
            room1.valueVectorZ = room1Vector[1];
            room1.valueVectorY = room1Vector[2];
            teleportLocation.options.Insert("room1", room1);

            vector room2Vector = {2568.14,15.2524,5961.31};
            GameLabsActionParameter room2 = new GameLabsActionParameter("Room A-2", "", "vector");
            room2.valueVectorX = room2Vector[0];
            room2.valueVectorZ = room2Vector[1];
            room2.valueVectorY = room2Vector[2];
            teleportLocation.options.Insert("room2", room2);

            vector room3Vector = {2574.2,15.2524,5966.15};
            GameLabsActionParameter room3 = new GameLabsActionParameter("Room A-3", "", "vector");
            room3.valueVectorX = room3Vector[0];
            room3.valueVectorZ = room3Vector[1];
            room3.valueVectorY = room3Vector[2];
            teleportLocation.options.Insert("room3", room3);

            vector room4Vector = {2573.62,15.2523,5972.42};
            GameLabsActionParameter room4 = new GameLabsActionParameter("Room A-4", "", "vector");
            room4.valueVectorX = room4Vector[0];
            room4.valueVectorZ = room4Vector[1];
            room4.valueVectorY = room4Vector[2];
            teleportLocation.options.Insert("room4", room4);

            this.parameters.Insert("teleportLocation", teleportLocation);
        }

        override bool Execute(GameLabsActionContext context) {
            PlayerBase player;
            PlayerBase.CastTo(player, context.GetReferencedObject());

            vector position = context.parameters.Get("teleportLocation").GetVector();

            GetGameLabs().GetLogger().Warn(string.Format("[SakhalBunkerDebug] %1 position=%2", player, position));

            player.SetPositionEx(position);
            return true;
        }
};