class GLBaseBuildingMetaStorage {
    string _id;
    string steam64;
    string cftoolsId;
    string objectType;

    [NonSerialized()]
    protected const string storagePath = "$profile:@GameLabsStorage";

    [NonSerialized()]
    protected string path;

    [NonSerialized()]
    protected bool ready = false;

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