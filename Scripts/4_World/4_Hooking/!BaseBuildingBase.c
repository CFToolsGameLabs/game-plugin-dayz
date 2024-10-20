class GLBaseBuildingMetaStorage {
    string _id;
    string steam64;
    string cftoolsId;

    [NonSerialized()]
    private const string storagePath = "$profile:@GameLabsStorage";

    [NonSerialized()]
    private string path;

    [NonSerialized()]
    private bool ready = false;

    void GLBaseBuildingMetaStorage() {}
    string ToJson() { return JsonFileLoader<GLBaseBuildingMetaStorage>.JsonMakeData(this); }
    void SetPersistentId(string persistentId) {
        this.ready = true;
        this._id = persistentId;
        this.path = string.Format("%1/%2.json", this.storagePath, this._id);
        GetGameLabs().GetLogger().Debug(string.Format("[Storage] %1 established for %2 @ %3", this, this._id, this.path));
    }
    bool Available() {
        return this.ready;
    }
    void LoadFromDisk() {
        JsonFileLoader<GLBaseBuildingMetaStorage>.JsonLoadFile(this.path, this);
    }
    void SaveToDisk() {
        JsonFileLoader <GLBaseBuildingMetaStorage>.JsonSaveFile(this.path, this);
    }
    void DeleteFromDisk() {
        DeleteFile(this.path);
    }
    bool CheckDiskPresence() {
        return FileExist(this.path);
    }
};

modded class BaseBuildingBase extends ItemBase {
    private string gl_steam64 = "";
    private string gl_cftoolsId = "";

    private ref GLBaseBuildingMetaStorage gl_storage;

    void BaseBuildingBase() {
        if(GetGameLabs().IsServer()) {
            this.gl_storage = new GLBaseBuildingMetaStorage();

            if(GetGameLabs().GetConfiguration().GetAutoCompleteConstruction()) {
                array<ConstructionPart> parts = GetConstruction().GetConstructionParts().GetValueArray();

                array<string> incompleteParts = {};
                foreach(ConstructionPart part : parts) {
                    if(!part.IsBuilt()) {
                        incompleteParts.Insert(part.GetPartName());
                    }
                }

                if(incompleteParts.Count() > 0) {
                    GetGameLabs().GetLogger().Debug(string.Format("[Construction] Auto completing construction %1{%2, %3} - %4", this, GetType(), GetDisplayName(), incompleteParts));
                    GL_FullyBuild();
                }
            }
        }
    }

    /*
     * TODO: Other mechanism for perpetual saving
    void OnStoreSave(ParamsWriteContext ctx) {
        super.OnStoreSave(ctx);
        GL_Save();
    }
     */

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
        GetGameLabs().GetLogger().Info(string.Format("[BaseBuildingBase] %1 uuid=%2;", this, GL_GetPersistentItemId()));
    }

    void GL_Load() {
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

    void GL_Save(bool forceSave = false) {
        if(this.gl_storage == NULL) return;
        if(!this.gl_storage.Available()) return;
        if(!this.gl_storage.CheckDiskPresence()) {
            if(forceSave && GL_GetSteam64()) {
                this.gl_storage.steam64 = GL_GetSteam64();
                this.gl_storage.cftoolsId = GetGameLabs().GetPlayerUpstreamIdentity(this.gl_storage.steam64);
                this.gl_storage.SaveToDisk();
                GetGameLabs().GetLogger().Debug(string.Format("[Storage] [Save] %1 stored to disk", this));
            } else {
                GetGameLabs().GetLogger().Debug(string.Format("[Storage] [Save] %1 not creating forceSave=%2; steam64=%3;", this, forceSave, GL_GetSteam64()));
            }
        } else {
            GetGameLabs().GetLogger().Debug(string.Format("[Storage] [Save] %1 already exists, skipping", this));
        }
    }

    void GL_Delete() {
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

    // Copied BaseBuildingBase.OnPartBuiltServer
    // Removed `Man player` parameter in order to not produce null refs
    void GL_OnPartBuiltServer(string part_name, int action_id) {
        ConstructionPart construtionPart = GetConstruction().GetConstructionPart(part_name);
        if (construtionPart.IsBase()) {
            SetBaseState(true);
            CreateConstructionKit();
        }
        RegisterPartForSync(construtionPart.GetId());
        RegisterActionForSync(construtionPart.GetId(), action_id);
        SynchronizeBaseState();
        SetPartFromSyncData(construtionPart);
        UpdateNavmesh();
        UpdateVisuals();
        GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ResetActionSyncData, 100, false, this);
    }

    void GL_FullyBuild() {
        array<string> excludes;
        switch(GetType()) {
            case "TerritoryFlag": {
                excludes = {};
                break;
            }
            case "ShelterSite": {
                excludes = {};
                break;
            }
            case "Watchtower": {
                excludes = {"_wood_"};
                break;
            }
            case "Fence": {
                excludes = {"_wood_", "platform", "gate"};
            }
            default: {
                excludes = OnDebugSpawnBuildExcludes();
            }
        }

        array<ConstructionPart> parts = GetConstruction().GetConstructionParts().GetValueArray();

        foreach(ConstructionPart part : parts) {
            bool excluded = false;
            string partName = part.GetPartName();
            if(excludes) {
                foreach(string exclude : excludes) {
                    if(partName.Contains(exclude)) {
                        excluded = true;
                        break;
                    }
                }
            }
            if(!excluded) {
                GL_OnPartBuiltServer(partName, AT_BUILD_PART);
            }
        }

        GetConstruction().UpdateVisuals();
    }
};