modded class ItemBase {
    private string gl_persistentItemId;


    void ItemBase() {
        if(!GetGameLabs()) return;
        if(!GetGameLabs().IsServer()) return;
        GetGameLabs().IncrEntityCount();
    }

    void ~ItemBase() {
        if(!GetGameLabs()) return;
        if(!GetGameLabs().IsServer()) return;
        GetGameLabs().DecrEntityCount();
    }

    override void EEOnAfterLoad() {
        super.EEOnAfterLoad();
        GL_GatherPersistentID();
    }

    void GL_GatherPersistentID() {
        if(this.GL_GetPersistentItemId()) return;
        this.GL_SetPersistentItemId(GL_GeneratePersistentId());
    }

    protected void GL_SetPersistentItemId(string id) {
        if(this.gl_persistentItemId) {
            if(this.gl_persistentItemId != id) {
                GetGameLabs().GetLogger().Warn(string.Format("%1 Illegal UUID change detected. This may corrupt stored vars: %2 > %3", this, this.gl_persistentItemId, id));
            }
        }
        this.gl_persistentItemId = id;
    }

    string GL_GetPersistentItemId() {
        return this.gl_persistentItemId;
    }

    protected string GL_GeneratePersistentId() {
        int uuid1, uuid2, uuid3, uuid4;
        GetPersistentID(uuid1, uuid2, uuid3, uuid4);

        int uuid[4] = {uuid1, uuid2, uuid3, uuid4};
        return UUIDApi.FormatString(uuid);

    }

    void GL_Load() {}
    void GL_SaveHook() {}
    void GL_Save(bool forceSave = false) {}
    void GL_Delete() {}
};


modded class ZombieBase  {
    private ref _AI _registeredInstance = new _AI(this, true);

    void ZombieBase () {
        if(!GetGameLabs()) return;
        if(!GetGameLabs().IsServer()) return;
        GetGameLabs().IncrAICount();
        GetGameLabs().RegisterAI(this._registeredInstance);
    }

    void ~ZombieBase () {
        if(!GetGameLabs()) return;
        if(!GetGameLabs().IsServer()) return;
        GetGameLabs().DecrAICount();
        if(this._registeredInstance) GetGameLabs().RemoveAI(this._registeredInstance);
    }

    override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef) {
        super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);

        if(damageType != DamageType.FIRE_ARM) return;

        if( !IsAlive() ) return;

        PlayerBase player;
        if(source) {
            player = PlayerBase.Cast(source.GetHierarchyRootPlayer());
        }
        if(!player) return;

        if(player.HasUpstreamIdentity()) {
            string cftoolsId = player.GetUpstreamIdentity();
            ref GLPlayerStatistics playerStatistics = GetGameLabs().GetPlayerStatisticsByCFToolsId(cftoolsId);
            playerStatistics.shotsHit++;
            playerStatistics.shotsHitInfected++;
        }
    }
};