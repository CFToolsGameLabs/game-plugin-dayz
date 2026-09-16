/*
 * IMPORTANT: Tracking of DigitalCodeLock items starts with the player attaching a lock the first time. Afterwards it is perpetually tracked.
 * DigitalCodeLock_Bunker is server-owned and is not persisted.
 */
class GLBaseBuildingCodeLockStorage extends GLBaseBuildingMetaStorage {
    string pin;
    bool isLocked;

    void GLBaseBuildingCodeLockStorage() {}
    override string ToJson() { return JsonFileLoader<GLBaseBuildingCodeLockStorage>.JsonMakeData(this); }
    override void LoadFromDisk() {
        JsonFileLoader<GLBaseBuildingCodeLockStorage>.JsonLoadFile(this.path, this);
    }
    override void SaveToDisk() {
        JsonFileLoader<GLBaseBuildingCodeLockStorage>.JsonSaveFile(this.path, this);
    }
};

modded class DigitalCodeLock extends ItemBase {
    private string gl_steam64 = "";
    private string gl_cftoolsId = "";

    private ref GLBaseBuildingCodeLockStorage gl_storage;

    void DigitalCodeLock() {
        if(!GetGameLabs()) return;
        if(!GetGameLabs().IsServer()) return;
        if(this.IsInherited(DigitalCodeLock_Bunker)) return;
        this.gl_storage = new GLBaseBuildingCodeLockStorage();
    }

    override void SetLockState(bool state) {
        super.SetLockState(state);
        this.GL_Save(true);
    }

    override void UnlockOnServer(EntityAI parent) {
        super.UnlockOnServer(parent);
        this.GL_Save(true);
    }

    override void ResetPIN() {
        super.ResetPIN();
        this.GL_Save(true);
    }

    override void OnWorkStop() {
        super.OnWorkStop();
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
        if(this.gl_storage) this.gl_storage.SetPersistentId(GL_GetPersistentItemId());
        GetGameLabs().GetLogger().Debug(string.Format("[DigitalCodeLock] %1 uuid=%2;", this, GL_GetPersistentItemId()));
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
                this.gl_storage.pin = this.GetLockPIN();
                this.gl_storage.isLocked = this.IsLocked();
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
