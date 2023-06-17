// Fire and forget
class _Store_Callback : _Callback {
    override void OnError(int errorCode) {};
    override void OnTimeout() {};
    override void OnSuccess(string data, int dataSize) {};
};

/* *********** Internal payloads ************** */
// Set to KV
class _Payload_KVSet : _Payload {
    string modId;
    string key;
    string value;

    void _Payload_KVSet(string modId, string key, string value) {
        this.modId = modId;
        this.key = key;
        this.value = value;
    }
    override string ToJson() { return JsonFileLoader<_Payload_KVSet>.JsonMakeData(this); }
};
class _Response_KVSet : _Response {};

// Get from KV
class _Payload_KVGet : _Payload {
    string modId;
    string key;

    void _Payload_KVGet(string modId, string key) {
        this.modId = modId;
        this.key = key;
    }
    override string ToJson() { return JsonFileLoader<_Payload_KVGet>.JsonMakeData(this); }
};
class _Response_KVGet : _Response {
    string value;
    void _Response_KVSet(string content) { JsonFileLoader<_Response_KVGet>.JsonLoadData(content, this); }
};

// Incr KV var
class _Payload_KVIncr : _Payload {
    string modId;
    string key;
    float value;

    void _Payload_KVIncr(string modId, string key, float value) {
        this.modId = modId;
        this.key = key;
        this.value = value;
    }
    override string ToJson() { return JsonFileLoader<_Payload_KVIncr>.JsonMakeData(this); }
};
class _Response_KVIncr : _Response {};

// Decr KV var
class _Payload_KVDecr : _Payload {
    string modId;
    string key;
    float value;

    void _Payload_KVDecr(string modId, string key, float value) {
        this.modId = modId;
        this.key = key;
        this.value = value;
    }
    override string ToJson() { return JsonFileLoader<_Payload_KVDecr>.JsonMakeData(this); }
};
class _Response_KVDecr : _Response {};

/* ************************************************ */

class GameLabsDataStore {
    private ref GameLabsAPI api;

    private string modId;

    void GameLabsDataStore(string modId) {
        this.modId = modId;
        this.api = GetGameLabs().GetApi();
    }
};