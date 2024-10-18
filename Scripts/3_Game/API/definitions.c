class _Payload {
        string _v = GetGameLabs().GetVersionIdentifier();
        string ToJson() { return JsonFileLoader<_Payload>.JsonMakeData(this); }
};

class _Response {
        int status;
        string error;

        void _Response(string content) { JsonFileLoader<_Response>.JsonLoadData(content, this); }
};

class _Callback : RestCallback {
    override void OnError(int errorCode) {};
    override void OnTimeout() {};
    override void OnSuccess(string data, int dataSize) {};
};

class _LogPlayer {
    string id;

    vector position;

    float water = -1;
    float blood = -1;
    float health = -1;
    float energy = -1;
    int bleedingSources = -1;
};

class _DebugDynamicActions {
    array<ref GameLabsContextAction> dynamicActions;
    void _DebugDynamicActions() {
        this.dynamicActions = GetGameLabs().GetGameLabsActions();
        JsonFileLoader <_DebugDynamicActions>.JsonSaveFile("$profile:@Logging/dynamic_actions.json", this);
    }
    string ToJson() { return JsonFileLoader<_DebugDynamicActions>.JsonMakeData(this); }
};

// Register: /v1/auth/register
class _Payload_Register : _Payload {
    string version = GetGameLabs().GetVersionIdentifier();
    string serverId;
    string apiKey;
    string localPort;
    bool flagReauth;
    array<ref GameLabsContextAction> availableActions;

    void _Payload_Register(string _serverId, string _apiKey, bool _flagReauth) {
        this.serverId = _serverId;
        this.apiKey = _apiKey;
        string tmp;
        if(GetGame().CommandlineGetParam("port", tmp)) {
            this.localPort = tmp;
        } else {
            this.localPort = "0";
        }
        this.flagReauth = _flagReauth;
        this.availableActions = GetGameLabs().GetGameLabsActions();
        if(GetGameLabs().GetDebugStatus()) {
            _DebugDynamicActions debugDynamicActions = new _DebugDynamicActions();
        }
    }
    override string ToJson() { return JsonFileLoader<_Payload_Register>.JsonMakeData(this); }
};
class _Response_Register_Features {
    int allowReporting = 2;
    int statisticsReporting = 2;

    int metricsInterval = 5;
    int reportingInterval = 10;

    int pollProtocolVersion = 2;

    bool kvEnabled = false;
};
class _Response_Register : _Response {
    string authKey;

    int modLicensingStatus = 1;
    string modLicensingOffender;

    _Response_Register_Features features;

    array<ref string> monitoredActions;
    void _Response_Register(string content) { JsonFileLoader<_Response_Register>.JsonLoadData(content, this); }
};
// ************************

// Register: /v1/server/poll
class _Payload_ServerPoll : _Payload {
    float serverFps;

    int aiCount;
    int animalCount;
    int vehicleCount;
    int entityCount;

    int aiActive;

    float overcast;
    float rain;
    float fog;
    float windspeed;

    void _Payload_ServerPoll(bool isFirst = false) {
        this.serverFps = GetGameLabs().GetServerFPS();

        this.aiCount = GetGameLabs().GetAICount();
        this.animalCount = GetGameLabs().GetAnimalCount();
        this.vehicleCount = GetGameLabs().GetVehicleCount();
        this.entityCount = GetGameLabs().GetEntityCount();

        if(!isFirst) this.aiActive = GetGameLabs().GetAIActiveCount();
        else this.aiActive = 0;

        Weather weather = GetGame().GetWeather();
        if(weather) {
            // Percentages 0-100
            this.overcast = weather.GetOvercast().GetActual()*100;
            this.rain = weather.GetRain().GetActual()*100;
            this.fog = weather.GetFog().GetActual()*100;
            // Windspeed in km/h
            this.windspeed = weather.GetWindSpeed();
        }
    }
    override string ToJson() { return JsonFileLoader<_Payload_ServerPoll>.JsonMakeData(this); }
};
class ServerPollItem {
    string action;
    string target;
    string parameter;
    int quantity = 1;
    float x;
    float y;
};
class _Response_ServerPoll : _Response {
    ref array<ServerPollItem> orders;
    void _Response_ServerPoll(string content) { JsonFileLoader<_Response_ServerPoll>.JsonLoadData(content, this); }
};
// ************************

// Register: /v1/server/events
class _ServerEvent {
    string id;
    string icon;
    string className;
    string displayName;
    vector position;

    void _ServerEvent(_Event _event) {
        this.id = _event.ToString();
        this.icon = _event.Icon();
        this.className = _event.Class();
        this.displayName = _event.DisplayName();
        if(_event.Ref() != NULL && _event.Ref() != NULL) {
            this.position = _event.Ref().GetPosition();
        }
    }
};
class _Payload_ServerEvents : _Payload {
    int initial;
    int interval;
    ref array<ref _ServerEvent> added = new array<ref _ServerEvent>();
    ref array<ref _ServerEvent> removed = new array<ref _ServerEvent>();

    void _Payload_ServerEvents(bool _initial, int _interval, array<ref _Event> _added, array<ref _Event> _removed) {
        if(_initial) { this.initial = 1; } else { this.initial = 0; }
        this.interval = _interval;

        for(int i = 0; i < _added.Count(); i++) {
            this.added.Insert(new _ServerEvent(_added.Get(i)));
        }
        for(int y = 0; y < _removed.Count(); y++) {
            this.removed.Insert(new _ServerEvent(_removed.Get(y)));
        }
    }
    override string ToJson() { return JsonFileLoader<_Payload_ServerEvents>.JsonMakeData(this); }
};
class _Response_ServerEvents : _Response {
    void _Response_ServerEvents(string content) { JsonFileLoader<_Response_ServerEvents>.JsonLoadData(content, this); }
};
// ************************

// Register: /v1/server/vehicles
class _ServerVehicle {
    string id;
    string icon;
    string vehicleType;
    string className;
    vector position;

    int health;
    int speed;

    void _ServerVehicle(_Vehicle vehicle) {
        this.id = vehicle.ToString();
        this.className = vehicle.Class();
        this.icon = vehicle.Icon();
        this.vehicleType = vehicle.VehicleType();
        if(vehicle != NULL && vehicle.Ref() != NULL) {
            this.position = vehicle.Ref().GetPosition();

            this.health = vehicle.Ref().GetHealth();
            Car _car = Car.Cast(vehicle.Ref());
            if(_car != NULL) {
                this.speed = _car.GetSpeedometer();
            } else {
                Boat _boat = Boat.Cast(vehicle.Ref());
                if(_boat != NULL) {
                    vector transform[4];
                    _boat.GetTransform(transform);
                    vector speedVector = GetVelocity(_boat).InvMultiply3(transform);
                    if(speedVector && speedVector.Length() == 3) {
                        float vehicleSpeed = speedVector[2];
                        if(speed < 0.0) {
                            this.speed = 0.0;
                        } else {
                            this.speed = Math.AbsFloat(vehicleSpeed);
                        }
                    } else {
                        this.speed = 0.0;
                    }
                } else {
                    this.speed = 0.0;
                }

            }

        }
    }
};
class _Payload_ServerVehicles : _Payload {
    int initial;
    int interval;
    ref array<ref _ServerVehicle> added = new array<ref _ServerVehicle>();
    ref array<ref _ServerVehicle> updated = new array<ref _ServerVehicle>();
    ref array<ref _ServerVehicle> removed = new array<ref _ServerVehicle>();

    void _Payload_ServerVehicles(bool _initial, int _interval, array<ref _Vehicle> _added, array<ref _Vehicle> _updated, array<ref _Vehicle> _removed) {
        if(_initial) { this.initial = 1; } else { this.initial = 0; }
        this.interval = _interval;

        for(int i = 0; i < _added.Count(); i++) {
            this.added.Insert(new _ServerVehicle(_added.Get(i)));
        }
        for(int x = 0; x < _updated.Count(); x++) {
            this.updated.Insert(new _ServerVehicle(_updated.Get(x)));
        }
        for(int y = 0; y < _removed.Count(); y++) {
            this.removed.Insert(new _ServerVehicle(_removed.Get(y)));
        }
    }
    override string ToJson() { return JsonFileLoader<_Payload_ServerVehicles>.JsonMakeData(this); }
};

class _Response_ServerVehicles : _Response {
    void _Response_ServerVehicles(string content) { JsonFileLoader<_Response_ServerVehicles>.JsonLoadData(content, this); }
};
// ************************


// Register: /v1/server/players
class _ServerPlayer {
    string id;
    string name;

    string gamesessionId;

    int loggingOut = 0;
    int insideVehicle = 0;

    int health;
    string item;
    vector position;

    int unconscious = 0;

    // Constructor is in reporter.c to circumvent layer limitations
};
class _Payload_ServerPlayers : _Payload {
    int initial;
    int interval;
    ref array<ref _ServerPlayer> updated = new array<ref _ServerPlayer>();

    void _Payload_ServerPlayers(bool _initial, int _interval, array<ref _ServerPlayer> _updated) {
        if(_initial) { this.initial = 1; } else { this.initial = 0; }
        this.interval = _interval;
        this.updated = _updated;
    }
    override string ToJson() { return JsonFileLoader<_Payload_ServerPlayers>.JsonMakeData(this); }
};

class _Response_ServerPlayers : _Response {
    void _Response_ServerPlayers(string content) { JsonFileLoader<_Response_ServerPlayers>.JsonLoadData(content, this); }
};
// ************************

// Register: /v1/player/death
class _Payload_PlayerDeath : _Payload {
    string weapon;
    string weaponNiceName;
    float distance;

    ref _LogPlayer player;
    ref _LogPlayer murderer;

    void _Payload_PlayerDeath(_LogPlayer _player, _LogPlayer _murderer, string _weapon, string _weaponNiceName) {
        this.player = _player;
        this.murderer = _murderer;

        if(_weapon) this.weapon = _weapon;
        if(_weaponNiceName) this.weaponNiceName = _weaponNiceName;
        if(this.murderer) {
            this.distance = vector.Distance(player.position, murderer.position);
        }
    }
    override string ToJson() { return JsonFileLoader<_Payload_PlayerDeath>.JsonMakeData(this); }
};
// ************************

// Register: /v1/player/damage
class _Payload_PlayerDamage : _Payload {
    string weapon;
    string weaponNiceName;

    string zone;
    float damage;
    float distance;

    ref _LogPlayer player;
    ref _LogPlayer murderer;

    void _Payload_PlayerDamage(ref _LogPlayer _player, ref _LogPlayer _murderer, Object _weapon, float _damage, string _zone) {
        this.player = _player;
        this.murderer = _murderer;

        this.zone = _zone;
        this.damage = _damage;
        if(_weapon) {
            this.weapon = _weapon.GetType();
            this.weaponNiceName = _weapon.GetDisplayName();
        }
        if(this.murderer) {
            this.distance = vector.Distance(player.position, murderer.position);
        }
    }
    override string ToJson() { return JsonFileLoader<_Payload_PlayerDamage>.JsonMakeData(this); }
};
// ************************

// Register: /v1/item/interact
class _Payload_ItemInteract : _Payload {
    string item;
    string target;
    string action;

    ref _LogPlayer player;


    void _Payload_ItemInteract(ref _LogPlayer _player, string _item, string _target, string _action) {
        this.player = _player;

        this.item = _item;
        this.target = _target;
        this.action = _action;
    }
    override string ToJson() { return JsonFileLoader<_Payload_ItemInteract>.JsonMakeData(this); }
};
// ************************

// Register: /v1/item/place
class _Payload_ItemPlace : _Payload {
    string item;

    ref _LogPlayer player;

    void _Payload_ItemPlace(ref _LogPlayer _player, string _item) {
        this.player = _player;

        this.item = _item;
    }
    override string ToJson() { return JsonFileLoader<_Payload_ItemPlace>.JsonMakeData(this); }
};
// ************************

// Register: /v1/player/chat
class _Payload_PlayerChat : _Payload {
    string channel;
    string message;

    ref _LogPlayer player;

    void _Payload_PlayerChat(ref _LogPlayer _player, string _channel, string _message) {
        this.player = _player;

        this.channel = _channel;
        this.message = _message;
    }
    override string ToJson() { return JsonFileLoader<_Payload_PlayerChat>.JsonMakeData(this); }
};
// ************************

// Register: /v1/player/connect
//! Callback is located in 5_Mission/definitions
class _Payload_PlayerConnect : _Payload {
    string steam64;
    vector position;

    void _Payload_PlayerConnect(string _steam64, vector _position) {
        this.steam64 = _steam64;
        this.position = _position;
    }
    override string ToJson() { return JsonFileLoader<_Payload_PlayerConnect>.JsonMakeData(this); }
};
class _Response_PlayerConnect : _Response {
    string steam64;
    string cftools_id;
    string gamesession_id;

    void _Response_PlayerConnect(string content) { JsonFileLoader<_Response_PlayerConnect>.JsonLoadData(content, this); }
};
// ************************

// Register: /v1/player/disconnect
//! Everything else is located in 5_Mission/definitions
class _Payload_PlayerDisconnect : _Payload {};
class _Response_PlayerDisconnect : _Response {
    void _Response_PlayerDisconnect(string content) { JsonFileLoader<_Response_PlayerDisconnect>.JsonLoadData(content, this); }
};
// ************************

// Register: /v1/item/list
class TrackedItem {
    string className;
    string displayName;

    void TrackedItem(string _className, string _displayName) {
        this.className = _className;
        this.displayName = _displayName;
    }
};
class _Payload_ItemList : _Payload {
    ref array<ref TrackedItem> items = new array<ref TrackedItem>();

    void _Payload_ItemList(array<ref TrackedItem> _items) {
        this.items = _items;
        if(GetGameLabs().GetDebugStatus()) {
            JsonFileLoader <_Payload_ItemList>.JsonSaveFile("$profile:@Logging/server_items.json", this);
        }
    }
    override string ToJson() { return JsonFileLoader<_Payload_ItemList>.JsonMakeData(this); }
};
class _Response_ItemList : _Response {
    void _Response_ItemList(string content) { JsonFileLoader<_Response_ItemList>.JsonLoadData(content, this); }
};
// ************************

// Register: /v1/kv/server
class _Payload_KVOperation : _Payload {
    string operation;

    string key;
    string value;

    int expires;

    void _Payload_KVOperation(string _operation, string _key, string _value, int _expires) {
        this.operation = _operation;

        this.key = _key;
        this.value = _value;

        this.expires = _expires;
    }
    override string ToJson() { return JsonFileLoader<_Payload_KVOperation>.JsonMakeData(this); }
};
class _Response_KVOperation : _Response {
    string key;
    string value;
    void _Response_KVOperation(string content) { JsonFileLoader<_Response_KVOperation>.JsonLoadData(content, this); }
};
class _Callback_KVOperation : _Callback {
    string operation;

    string key;
    string value;

    int expires;

    void _Callback_KVOperation(string _operation, string _key, string _value, int _expires) {
        this.operation = _operation;

        this.key = _key;
        this.value = _value;

        this.expires = _expires;
    }

    override void OnError(int errorCode) {
        GetGameLabs().GetLogger().Error(string.Format("[KV] (%1) %2=%3 [expires=%4] => error: server; errorCode: %5", this.operation, this.key, this.value, this.expires, errorCode));
    };
    override void OnTimeout() {
        GetGameLabs().GetLogger().Error(string.Format("[KV] (%1) %2=%3 [expires=%4] => error: timeout", this.operation, this.key, this.value, this.expires));
    };
    override void OnSuccess(string data, int dataSize) {
        GetGameLabs().GetLogger().Info(string.Format("[KV] (%1) %2=%3 [expires=%4] => success: %5[%6]", this.operation, this.key, this.value, this.expires, data, dataSize));
    };
};
// ************************

// Discord

class _Payload_DiscordWebHookEmbedField {
    string name;
    string value;
    bool inline;

    void _Payload_DiscordWebHookEmbedField(string _name, string _value, bool _inlineField) {
        this.name = _name;
        this.value = _value;
        this.inline = _inlineField;
    }
};

class _Payload_DiscordWebHookEmbed {
    string title;
    string description;
    int color;

    ref array<ref _Payload_DiscordWebHookEmbedField> fields = new array<ref _Payload_DiscordWebHookEmbedField>();

    void SetTitle(string _title) {
        this.title = _title;
    }

    void SetDescription(string _description) {
        this.description = _description;
    }

    void SetColor(int _color) {
        this.color = _color;
    }

    void AddField(string name, string value, bool inlineField) {
        ref _Payload_DiscordWebHookEmbedField field = new _Payload_DiscordWebHookEmbedField(name, value, inlineField);
        this.fields.Insert(field);
    }
};

class _Payload_DiscordWebHook : _Payload {
    string username = "GameLabs";
    string avatar_url = "https://cdn.cftools.de/brand/cloud/avatar-blue.png";

    string content;
    ref array<_Payload_DiscordWebHookEmbed> embeds = new array<_Payload_DiscordWebHookEmbed>();

    void _Payload_DiscordWebHook() {}

    void SetContent(string _content) {
        this.content = _content;
    }

    void AddEmbed(_Payload_DiscordWebHookEmbed _embed) {
        this.embeds.Insert(_embed);
    }
    override string ToJson() { return JsonFileLoader<_Payload_DiscordWebHook>.JsonMakeData(this); }
};