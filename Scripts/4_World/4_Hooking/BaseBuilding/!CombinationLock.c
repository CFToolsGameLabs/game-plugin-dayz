/*
 * IMPORTANT: Tracking of CombinationLock items starts with the player attaching a lock the first time. Afterwards it is perpetually tracked.
 */
class GLBaseBuildingLockStorage extends GLBaseBuildingMetaStorage {
    int combination;
    bool isLocked;

    void GLBaseBuildingLockStorage() {}
    override string ToJson() { return JsonFileLoader<GLBaseBuildingLockStorage>.JsonMakeData(this); }
    override void LoadFromDisk() {
        JsonFileLoader<GLBaseBuildingLockStorage>.JsonLoadFile(this.path, this);
    }
    override void SaveToDisk() {
        JsonFileLoader <GLBaseBuildingLockStorage>.JsonSaveFile(this.path, this);
    }
};

modded class CombinationLock extends ItemBase {
    private string gl_steam64 = "";
    private string gl_cftoolsId = "";

    private ref GLBaseBuildingLockStorage gl_storage;

    void CombinationLock() {
        if(GetGameLabs().IsServer()) {
            this.gl_storage = new GLBaseBuildingLockStorage();
        }
    }

    override void LockServer( EntityAI parent, bool ignore_combination = false ) {
        super.LockServer(parent, ignore_combination);
        this.GL_Save(true);
    }

    override void UnlockServer( EntityAI player, EntityAI parent ) {
        super.UnlockServer(player, parent);
        this.GL_Save(true);
    }

    /* Internals for storage */

    override void EEOnAfterLoad() {
        super.EEOnAfterLoad();
        if(this.gl_storage == NULL) return;
        this.gl_storage.SetPersistentId(GL_GetPersistentItemId());
        GL_Load();
    }

    override void EEDelete(EntityAI parent) {
        super.EEDelete(parent);
        GL_Delete();
    }

    GLBaseBuildingMetaStorage GL_GetStorage() {
        return this.gl_storage;
    }

    override void GL_GatherPersistentID() {
        if(this.GL_GetPersistentItemId()) return;
        this.GL_SetPersistentItemId(GL_GeneratePersistentId());
        this.gl_storage.SetPersistentId(GL_GetPersistentItemId());
        GetGameLabs().GetLogger().Debug(string.Format("[BaseBuildingBase] %1 uuid=%2;", this, GL_GetPersistentItemId()));
    }

    override void GL_Load() {
        if(this.gl_storage == NULL) return;
        if(!this.gl_storage.Available()) return;
        if(this.gl_storage.CheckDiskPresence()) {
            this.gl_storage.LoadFromDisk();
            if(this.gl_storage.steam64) {
                GL_SetSteam64(this.gl_storage.steam64);
                GL_SetOwnerUpstreamIdentity(this.gl_storage.cftoolsId);
                GetGameLabs().GetLogger().Debug(string.Format("[Storage] [Load] %1 loaded owner", this, GL_GetSteam64()));
            } else {
                GetGameLabs().GetLogger().Debug(string.Format("[Storage] [Load] %1 has no stored owner", this));
            }
        } else {
            GetGameLabs().GetLogger().Debug(string.Format("[Storage] [Load] %1 has no disk presence", this));
        }
    }

    override void GL_Save(bool forceSave = false) {
        if(this.gl_storage == NULL) return;
        if(!this.gl_storage.Available()) return;
        if(!this.gl_storage.CheckDiskPresence()) {
            if(forceSave && GL_GetSteam64()) {
                this.gl_storage.objectType = GetType();
                this.gl_storage.steam64 = GL_GetSteam64();
                this.gl_storage.cftoolsId = GetGameLabs().GetPlayerUpstreamIdentity(this.gl_storage.steam64);
                this.gl_storage.combination = this.m_Combination;
                this.gl_storage.isLocked = this.m_IsLocked;
                this.gl_storage.SaveToDisk();
                GetGameLabs().GetLogger().Debug(string.Format("[Storage] [Save] %1 stored to disk", this));
            } else {
                GetGameLabs().GetLogger().Debug(string.Format("[Storage] [Save] %1 not creating forceSave=%2; steam64=%3;", this, forceSave, GL_GetSteam64()));
            }
        } else {
            GetGameLabs().GetLogger().Debug(string.Format("[Storage] [Save] %1 already exists, skipping", this));
        }
    }

    override void GL_Delete() {
        if(this.gl_storage == NULL) return;
        if(!this.gl_storage.Available()) return;
        if(this.gl_storage.CheckDiskPresence()) {
            this.gl_storage.DeleteFromDisk();
            GetGameLabs().GetLogger().Debug(string.Format("[Storage] [Delete] %1 deleted", this));
        } else {
            GetGameLabs().GetLogger().Debug(string.Format("[Storage] [Delete] %1 does not exist, cant delete", this));
        }
    }

    // No storage implemented
    void GL_SetSteam64(string steam64) {
        this.gl_steam64 = steam64;
    }

    string GL_GetSteam64() {
        return this.gl_steam64;
    }

    void GL_SetOwnerUpstreamIdentity(string cftoolsId) {
        this.gl_cftoolsId = cftoolsId;
    }

    string GL_GetOwnerUpstreamIdentity() {
        return this.gl_cftoolsId;
    }
};