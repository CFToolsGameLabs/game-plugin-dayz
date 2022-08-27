class GameLabsActions {
    private ref map<string, int> actionMap = new map<string, int>;

    map<string, int> GetMap() {
        return this.actionMap;
    }

    void RegisterAction(string appId, int flag) {
        Print(string.Format("[GameLabs] Registered action=%1 for app=%2", flag, appId));
        this.actionMap.Set(appId, flag);
    }
};

private static ref GameLabsActions g_GameLabsActions;
static ref GameLabsActions GetGameLabsActions() {
    if(!g_GameLabsActions) {
        g_GameLabsActions = GameLabsActions();
    }
    return g_GameLabsActions;
};


class CFCloud_TeleportPlayer extends GameLabsContextAction {
        void CFCloud_TeleportPlayer() {
            this.actionCode = "CFCloud_TeleportPlayer";
            this.actionName = "Teleport a player to target location";
            this.actionIcon = "map-marker-alt";
            this.actionColour = "default";
            this.actionContext = "player";

            this.parameters.Insert("vector", GameLabsActionParameter("Target location", "Target location as DayZ parsable vector", "string"));
        }

        static bool Execute(GameLabsActionContext context) {
            PlayerBase player = PlayerBase.CastTo(context.GetReferencedObject());
            vector position = context.parameters.Get("vector").GetValue();

            GetGameLabs().GetLogger().Warn(string.Format("[Teleport] %1 position=%2", player, position));

            player.SetPositionEx(position);
            return true;
        }
};


class CFCloud_HealPlayer extends GameLabsContextAction {
        void CFCloud_HealPlayer() {
            this.actionCode = "CFCloud_HealPlayer";
            this.actionName = "Replenish player vitals";
            this.actionIcon = "heartbeat";
            this.actionColour = "default";
            this.actionContext = "player"
        }

        static bool Execute(GameLabsActionContext context) {
            PlayerBase player = PlayerBase.CastTo(context.GetReferencedObject());

            GetGameLabs().GetLogger().Warn(string.Format("[Heal] %1", player));
            player.GLHealEx();
            return true;
        }
};

class CFCloud_KillPlayer extends GameLabsContextAction {
        void CFCloud_KillPlayer() {
            this.actionCode = "CFCloud_KillPlayer";
            this.actionName = "Kill a player character";
            this.actionIcon = "skull";
            this.actionColour = "default";
            this.actionContext = "player"
        }

        static bool Execute(GameLabsActionContext context) {
            PlayerBase player = PlayerBase.CastTo(context.GetReferencedObject());

            GetGameLabs().GetLogger().Warn(string.Format("[Kill] %1", player));
            player.SetHealth(0);
            return true;
        }
};

class CFCloud_SpawnPlayerItem extends GameLabsContextAction {
        void CFCloud_SpawnPlayerItem() {
            this.actionCode = "CFCloud_SpawnPlayerItem";
            this.actionName = "Spawn an item at player position";
            this.actionIcon = "gem";
            this.actionColour = "default";
            this.actionContext = "player"

            this.parameters.Insert("item", GameLabsActionParameter("Item class name", "Class name of item to be spawned", "cf_itemlist"));
            this.parameters.Insert("quantity", GameLabsActionParameter("Quantity", "Amount if items to be spawned", "int"));
            this.parameters.Insert("debug", GameLabsActionParameter("Debug spawn", "Use debug spawn method to automatically populate specific items", "boolean"));
            this.parameters.Insert("stacked", GameLabsActionParameter("Stacked", "Spawn items as a stack (only works if item supports to be stacked)", "boolean"));
        }

        static bool Execute(GameLabsActionContext context) {
            PlayerBase player = PlayerBase.CastTo(context.GetReferencedObject());

            GetGameLabs().GetLogger().Warn(string.Format("[Spawn] Spawning %1 (x%2) for %3", params.gameClass, context.parameters.Get("item").GetValue(), context.parameters.Get("quantity").GetValue(), player));

            EntityAI entity;
            if(params.gameClass, context.parameters.Get("stacked").GetValue()) {
                entity = player.SpawnEntityOnGroundPos(context.parameters.Get("item").GetValue(), player.GetPosition());
                if (context.parameters.Get("debug").GetValue()) {
                    entity.OnDebugSpawn();
                }
                entity.SetQuantity(context.parameters.Get("quantity").GetValue());
            } else {
                for (int i = 1; i <= context.parameters.Get("quantity").GetValue(); i++) {
                    entity = player.SpawnEntityOnGroundPos(context.parameters.Get("item").GetValue(), player.GetPosition());
                    if (context.parameters.Get("debug").GetValue()) {
                        entity.OnDebugSpawn();
                    }
                }
            }
            return true;
        }
};

class CFCloud_ExplodePlayer extends GameLabsContextAction {
    void CFCloud_ExplodePlayer() {
        this.actionCode = "CFCloud_ExplodePlayer";
        this.actionName = "Explode player";
        this.actionIcon = "bomb";
        this.actionColour = "danger";
        this.actionContext = "player";
    }

    static bool Execute(GameLabsActionContext context) {
        PlayerBase player = PlayerBase.CastTo(context.GetReferencedObject());

        GetGameLabs().GetLogger().Warn(string.Format("[Player-Explode] %1", player));
        player.Explode(DT_EXPLOSION, "LandFuelFeed_Ammo");
        return true;
    }
};

