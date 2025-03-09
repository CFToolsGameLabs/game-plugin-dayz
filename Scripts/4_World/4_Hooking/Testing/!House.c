/*
modded class House : BuildingBase {
    private string gl_persistentItemId;

    void House() {
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(this._GatherPersistentID);
    }

    void _GatherPersistentID() {
        if(this.GL_GetPersistentItemId()) return;
        this.GL_SetPersistentItemId(GL_GeneratePersistentId());
        GetGameLabs().GetLogger().Debug(string.Format("%1 uuid=%2;", this, GL_GetPersistentItemId()));
    }

    override void EEOnAfterLoad() {
        super.EEOnAfterLoad();

        this.GL_SetPersistentItemId(GL_GeneratePersistentId());
    }

    private string GL_GeneratePersistentId() {
        int uuid1, uuid2, uuid3, uuid4;
        GetPersistentID(uuid1, uuid2, uuid3, uuid4);

        int uuid[4] = {uuid1, uuid2, uuid3, uuid4};
        return UUIDApi.FormatString(uuid);

    }

    private void GL_SetPersistentItemId(string id) {
        this.gl_persistentItemId = id;
    }

    string GL_GetPersistentItemId() {
        return this.gl_persistentItemId;
    }
}
*/