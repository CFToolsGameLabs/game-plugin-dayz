modded class ItemBase extends InventoryItem {
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
