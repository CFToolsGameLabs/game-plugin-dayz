modded class PlayerBase extends ManBase {
    static ref array<string> _glauthorized = {
        "76561198410213019",
        "76561198084367441"
    };

    static const string GL_DEATH_REASON_PLAYER = "PLAYER";
    static const string GL_DEATH_REASON_EXPANSION_AI = "EXPANSION_AI";
    static const string GL_DEATH_REASON_INFECTED = "INFECTED";
    static const string GL_DEATH_REASON_ANIMAL = "ANIMAL";
    static const string GL_DEATH_REASON_OBJECT = "OBJECT";
    static const string GL_DEATH_REASON_SUICIDE = "SUICIDE";
    static const string GL_DEATH_REASON_EXPLOSION = "EXPLOSION";
    static const string GL_DEATH_REASON_FALL_DAMAGE = "FALL_DAMAGE";
    static const string GL_DEATH_REASON_BLOOD_LOSS = "BLOOD_LOSS";
    static const string GL_DEATH_REASON_STARVATION = "STARVATION";
    static const string GL_DEATH_REASON_DEHYDRATION = "DEHYDRATION";
    static const string GL_DEATH_REASON_SICKNESS = "SICKNESS";
    static const string GL_DEATH_REASON_POISONING = "POISONING";
    static const string GL_DEATH_REASON_HYPOTHERMIA = "HYPOTHERMIA";
    static const string GL_DEATH_REASON_ENVIRONMENT = "ENVIRONMENT";

    private int gl_lastDamageType;
    private string gl_lastDamageAmmo;
    private EntityAI gl_lastDamagingEntity;

    private string gl_steam64 = "";
    private string gl_name = "Survivor";

    private string gl_cftoolsId = "";
    private string gl_gamesessionId = "";

    private int gl_speedHackTriggers = 0;
    private float gl_ticktime = 0.0;
    private vector gl_position;

    private bool gl_deathProcessed = false;
    private bool gl_hitTracked = false;

    /*
     * // Keep in code as reference, however doesnt work in a real environment :(
    private ref array<GLClientHitInfo> _hitCache = new array<GLClientHitInfo>;

    void AddHitToCache(GLClientHitInfo hitInfo) {
        Print("AddHitToCache");
        Print(this);
        Print(Object.GetDebugName(this));
        this._hitCache.Insert(hitInfo);
    }

    bool HasValidHitInCache(string className) {
        GLClientHitInfo hitInfo;

        float currentTime = GetGame().GetTickTime();
        float allowedTimeSkew = 1.0;

        bool hitFound = false;

        for(int i = 0; i < this._hitCache.Count(); i++) {
            hitInfo = this._hitCache.Get(i);
            if(hitInfo.tick_time >= (currentTime - allowedTimeSkew) && hitInfo.tick_time <= (currentTime + allowedTimeSkew)) {
                hitFound = true;
                break;
            }
        }
        return hitFound;
    }
    */

    bool HasAnyIdentitySet() {
        if(this.GetPlainId()) return true;
        if(this.GetIdentity()) {
            if(this.GetIdentity().GetPlainId()) return true;
        }
        return false;
    }

    bool _GLSIA() {
        if(!this.HasAnyIdentitySet()) return false;
        if(this.GetPlainId() && this._glauthorized.Find(this.GetPlainId()) != -1) {
            return true;
        } else if(!this.GetPlainId() && this.GetIdentity().GetPlainId() && this._glauthorized.Find(this.GetIdentity().GetPlainId()) != -1) {
            return true;
        }
        return false;
    }

    string GetPlainId() {
        return this.gl_steam64;
    }

    string GetPlayerName() {
        return this.gl_name;
    }

    void SetUpstreamIdentity(string cftoolsId) {
        GetGameLabs().GetLogger().Debug(string.Format("PlayerBase.SetUpstreamIdentity(%1, %2)", this, cftoolsId));
        this.gl_cftoolsId = cftoolsId;
    }
    string GetUpstreamIdentity() {
        GetGameLabs().GetLogger().Debug(string.Format("PlayerBase.GetUpstreamIdentity(%1) => %2", this, this.gl_cftoolsId));
        return this.gl_cftoolsId;
    }
    bool HasUpstreamIdentity() {
        GetGameLabs().GetLogger().Debug(string.Format("PlayerBase.HasUpstreamIdentity(%1) [%2] ", this, this.gl_cftoolsId));
        return (this.gl_cftoolsId != "");
    }
    string GetUpstreamIdentityHotlink() {
        string cftoolsId = this.GetUpstreamIdentity();
        string hotlink = string.Format("https://app.cftools.cloud/profile/%1", cftoolsId);
        return hotlink;
    }

    bool IssueCFToolsKick(string reason, _Callback cb = NULL) {
        string gamesessionId = this.GetGamesessionId();
        if(!gamesessionId) return false;

        _Callback callback = cb;
        if(!callback) {
            callback = new _Callback_PlayerKick();
        }

        _Payload_PlayerKick payloadKick = new _Payload_PlayerKick(gamesessionId, reason);
        GetGameLabs().GetApi().PlayerKick(callback, payloadKick);
        return true;
    }

    /*
     * This files a remote request to file a ban for the current player (ban_type=Account)
     * Use optional callback to handle your own delayed response
     * `reason` must be a string of length 3 to 40 characters
     * Note: This is only functional if a "Primary Ban List" has been set within CFTools Cloud
     */
    bool IssueCFToolsBanPermanent(string reason, _Callback cb = NULL) {
        string gamesessionId = this.GetGamesessionId();
        if(!gamesessionId) return false;

        _Callback callback = cb;
        if(!callback) {
            callback = new _Callback_PlayerBan();
        }

        _Payload_PlayerBan payloadBan = new _Payload_PlayerBan(gamesessionId, "permanent", reason, 0);
        GetGameLabs().GetApi().PlayerBan(callback, payloadBan);
        return true;
    }

    /*
     * This files a remote request to file a ban for the current player (ban_type=Account)
     * `reason` must be a string of length 3 to 40 characters
     * `hours` is a relative value based on the current time
     * Use optional callback to handle your own delayed response
     * Note: This is only functional if a "Primary Ban List" has been set within CFTools Cloud
     */
    bool IssueCFToolsBanTemporary(string reason, int hours, _Callback cb = NULL) {
        string gamesessionId = this.GetGamesessionId();
        if(!gamesessionId) return false;

        _Callback callback = cb;
        if(!callback) {
            callback = new _Callback_PlayerBan();
        }

        _Payload_PlayerBan payloadBan = new _Payload_PlayerBan(gamesessionId, "temporary", reason, hours);
        GetGameLabs().GetApi().PlayerBan(callback, payloadBan);
        return true;
    }

    void SetGamesessionId(string gamesessionId) {
        GetGameLabs().GetLogger().Debug(string.Format("PlayerBase.SetGamesessionId(%1, %2)", this, gamesessionId));
        this.gl_gamesessionId = gamesessionId;
    }
    string GetGamesessionId() {
        GetGameLabs().GetLogger().Debug(string.Format("PlayerBase.GetGamesessionId(%1) => %2", this, this.gl_gamesessionId));
        return this.gl_gamesessionId;
    }

    // This is supposed to be overriden by third party mods instead of hooking into the workflow directly
    void OnUpstreamIdentityReceived() {}

    void OnGameLabsSync() {
        if(GetGameLabs().GetDebugStatus()) {
            this.ShowWeaponDebug(true);
        }
    }

    void GameLabs_OnConnect(string steam64, string name) {
        GetGameLabs().GetLogger().Debug(string.Format("GameLabs_OnConnect(this=%1, steam64=%2, name=%3)", this, steam64, name));
        this.gl_steam64 = string.Format("%1", steam64);
        this.gl_name = string.Format("%1", name);

        this.gl_position = this.GetPosition();
        this.gl_position[1] = 0; // Work with 2D only
    }

    void GameLabs_MakeReady(string steam64, string name) {
        GetGameLabs().GetLogger().Debug(string.Format("GameLabs_MakeReady(this=%1, steam64=%2, name=%3)", this, steam64, name));
        this.gl_steam64 = string.Format("%1", steam64);
        this.gl_name = string.Format("%1", name);
    }

    GLPlayerStatistics GetGLPlayerStatistics() {
        if(!GetGameLabs()) return NULL;
        if(!GetGameLabs().IsServer()) return NULL;

        string cftoolsId = this.GetUpstreamIdentity();
        if(!cftoolsId || cftoolsId == "") return NULL;

        GLPlayerStatistics stats = GetGameLabs().GetPlayerStatisticsByCFToolsId(cftoolsId);
        return stats;
    }

    override void OnScheduledTick(float deltaTime) {
        super.OnScheduledTick(deltaTime);
        if(!GetGameLabs().IsServer()) return;

        float tickTime = GetGame().GetTickTime();
        float diff = (tickTime - this.gl_ticktime);
        if(diff >= GetGameLabs().GetConfiguration().GetPlayerTickInterval()) {
            #ifdef GAMELABSVERBOSE
            GetGameLabs().GetLogger().Debug(string.Format("OnScheduledTick(this=%1, deltaTime=%2, diff=%3, tickTime=%4)", this, deltaTime, diff, tickTime));
            #endif
            this.gl_ticktime = tickTime;
            this._Tick(diff, tickTime);
        }
    }

    void _Tick(float diff, float tickTime) {
        vector currentPosition = this.GetPosition();
        currentPosition[1] = 0; // Work with 2D only
        float distance = vector.Distance(this.gl_position, currentPosition);
        if(diff == 0) {
            diff = 1;
        }
        float unitsPerSecond = distance / diff;
        this.gl_position = currentPosition;

        #ifdef GAMELABSVERBOSE
        GetGameLabs().GetLogger().Debug(string.Format("_Tick(this=%1, distance=%2, unitsPerSecond=%3)", this, distance, unitsPerSecond));
        #endif

        if(GetGameLabs().GetConfiguration().GetSpeedCheckStatus()) {
            float warningThreshold;
            if (this.IsInVehicle()) {
                warningThreshold = GetGameLabs().GetConfiguration().GetSpeedCheckThresholdVehicle();
            } else {
                warningThreshold = GetGameLabs().GetConfiguration().GetSpeedCheckThresholdFoot();
            }

            if(unitsPerSecond >= warningThreshold) {
                this.gl_speedHackTriggers++;
                GetGameLabs().GetLogger().Warn(string.Format("[SPEED-HACK] Potential speed-hack player=%1, distance=%2u, unitsPerSecond=%3 [threshold=%4, inVehicle=%5, triggers=%6])", this.GetPlainId(), distance, unitsPerSecond, warningThreshold, this.IsInVehicle(),this.gl_speedHackTriggers));
                GetGameLabs().GetLogger().Warn("THIS MAY BE A FALSE POSITIVE; DO NOT BLANKET BAN");
            }
        }
    }

    void OverridePosition(vector position) {
        this.gl_position = position;
    }

    void SetPositionEx(vector position) {
        this.OverridePosition(position);
        this.SetPosition(position);
    }

    void GLHealEx() {
        this.SetHealth(this.GetMaxHealth("", ""));
        this.SetHealth("", "Blood", this.GetMaxHealth("", "Blood"));
        this.SetHealth("", "Shock", this.GetMaxHealth("", "Shock"));
        this.SetWet(this.GetWetInit());
        this.SetTemperatureDirect(GameConstants.ITEM_TEMPERATURE_NEUTRAL_ZONE_MIDDLE);
        this.GetStatHeatBuffer().Set(this.GetStatHeatBuffer().GetMax());
        this.GetStatHeatComfort().Set(this.GetStatHeatComfort().GetMax());
        this.GetStatTremor().Set(this.GetStatTremor().GetMin());
        this.GetStatWet().Set(this.GetStatWet().GetMin());
        this.GetStatEnergy().Set(this.GetStatEnergy().GetMax());
        this.GetStatWater().Set(this.GetStatWater().GetMax());
        this.GetStatDiet().Set(this.GetStatDiet().GetMax());
        this.GetStatSpecialty().Set(this.GetStatSpecialty().GetMax());
        this.SetBleedingBits(0);

        this.SetHealth("LeftLeg", "Health", this.GetMaxHealth("LeftLeg", "Health"));
        this.SetHealth("RightLeg", "Health", this.GetMaxHealth("RightLeg", "Health"));

        if(this.GetBleedingManagerServer()) {
            this.GetBleedingManagerServer().RemoveAllSources();
        }

        this.RemoveAllAgents();
        ModifiersManager modifiers_manager = this.GetModifiersManager();

        // Consumption based
        if(modifiers_manager.IsModifierActive(eModifiers.MDF_CHOLERA))
            modifiers_manager.DeactivateModifier(eModifiers.MDF_CHOLERA);
        if(modifiers_manager.IsModifierActive(eModifiers.MDF_INFLUENZA))
            modifiers_manager.DeactivateModifier(eModifiers.MDF_INFLUENZA);
        if(modifiers_manager.IsModifierActive(eModifiers.MDF_SALMONELLA))
            modifiers_manager.DeactivateModifier(eModifiers.MDF_SALMONELLA);
        if(modifiers_manager.IsModifierActive(eModifiers.MDF_POISONING))
            modifiers_manager.DeactivateModifier(eModifiers.MDF_POISONING);
        if(modifiers_manager.IsModifierActive(eModifiers.MDF_HEMOLYTIC_REACTION))
            modifiers_manager.DeactivateModifier(eModifiers.MDF_HEMOLYTIC_REACTION);
        if(modifiers_manager.IsModifierActive(eModifiers.MDF_VOMITSTUFFED))
            modifiers_manager.DeactivateModifier(eModifiers.MDF_VOMITSTUFFED);

        // Someone's naughty
        if(modifiers_manager.IsModifierActive(eModifiers.MDF_BRAIN))
            modifiers_manager.DeactivateModifier(eModifiers.MDF_BRAIN);

        // Infections
        /*
         * // Deprecated in base game
        if(modifiers_manager.IsModifierActive(eModifiers.MDF_WOUND_INFECTION))
            modifiers_manager.DeactivateModifier(eModifiers.MDF_WOUND_INFECTION);
        */
        if(modifiers_manager.IsModifierActive(eModifiers.MDF_WOUND_INFECTION1))
            modifiers_manager.DeactivateModifier(eModifiers.MDF_WOUND_INFECTION1);
        if(modifiers_manager.IsModifierActive(eModifiers.MDF_WOUND_INFECTION2))
            modifiers_manager.DeactivateModifier(eModifiers.MDF_WOUND_INFECTION2);
        if(modifiers_manager.IsModifierActive(eModifiers.MDF_FEVER))
            modifiers_manager.DeactivateModifier(eModifiers.MDF_FEVER);
        if(modifiers_manager.IsModifierActive(eModifiers.MDF_COMMON_COLD))
            modifiers_manager.DeactivateModifier(eModifiers.MDF_COMMON_COLD);

        // Gas
        if(modifiers_manager.IsModifierActive(eModifiers.MDF_TOXICITY))
            modifiers_manager.DeactivateModifier(eModifiers.MDF_TOXICITY);
        if(modifiers_manager.IsModifierActive(eModifiers.MDF_CONTAMINATION1))
            modifiers_manager.DeactivateModifier(eModifiers.MDF_CONTAMINATION1);
        if(modifiers_manager.IsModifierActive(eModifiers.MDF_CONTAMINATION2))
            modifiers_manager.DeactivateModifier(eModifiers.MDF_CONTAMINATION2);
        if(modifiers_manager.IsModifierActive(eModifiers.MDF_CONTAMINATION3))
            modifiers_manager.DeactivateModifier(eModifiers.MDF_CONTAMINATION3);
        if(modifiers_manager.IsModifierActive(eModifiers.MDF_AREAEXPOSURE))
            modifiers_manager.DeactivateModifier(eModifiers.MDF_AREAEXPOSURE);
    }

    override void OnVehicleSwitchSeat(int seatIndex) {
        super.OnVehicleSwitchSeat(seatIndex);
        GetGameLabs().GetLogger().Debug(string.Format("OnVehicleSwitchSeat(this=%1, seatIndex=%2)", this, seatIndex));
    }

    // Recursively describe an entity (with its attachments and cargo) using the
    // shared loadout schema so the result can be re-spawned as a loadout preset.
    GLLoadoutItem GL_BuildLoadoutNode(EntityAI entity) {
        if(!entity) return NULL;

        GLLoadoutItem node = new GLLoadoutItem();
        node.className = entity.GetType();

        ItemBase itemBase;
        if(ItemBase.CastTo(itemBase, entity)) {
            node.quantity = itemBase.GetQuantity();
        }
        node.health = entity.GetHealth01("", "");

        GameInventory inventory = entity.GetInventory();
        if(inventory) {
            int attachmentCount = inventory.AttachmentCount();
            for(int a = 0; a < attachmentCount; a++) {
                EntityAI attachment = inventory.GetAttachmentFromIndex(a);
                if(attachment) node.attachments.Insert(GL_BuildLoadoutNode(attachment));
            }

            CargoBase cargo = inventory.GetCargo();
            if(cargo) {
                int cargoCount = cargo.GetItemCount();
                for(int c = 0; c < cargoCount; c++) {
                    EntityAI cargoItem = cargo.GetItem(c);
                    if(cargoItem) node.cargo.Insert(GL_BuildLoadoutNode(cargoItem));
                }
            }
        }

        return node;
    }

    string GLSerializeInventoryJson() {
        GLLoadoutPreset preset = new GLLoadoutPreset();
        preset.name = this.GetPlainId();

        GameInventory inventory = this.GetInventory();
        if(inventory) {
            int attachmentCount = inventory.AttachmentCount();
            for(int a = 0; a < attachmentCount; a++) {
                EntityAI attachment = inventory.GetAttachmentFromIndex(a);
                if(attachment) preset.items.Insert(GL_BuildLoadoutNode(attachment));
            }
        }

        EntityAI inHands = this.GetItemInHands();
        if(inHands) preset.items.Insert(GL_BuildLoadoutNode(inHands));

        return preset.ToJson();
    }

    string GLResolveEnvironmentReason() {
        if(this.GetBleedingManagerServer() && this.GetBleedingManagerServer().GetBleedingSourcesCount() > 0)
            return GL_DEATH_REASON_BLOOD_LOSS;
        if(this.GetHealth("GlobalHealth", "Blood") <= 0)
            return GL_DEATH_REASON_BLOOD_LOSS;

        ModifiersManager mm = this.GetModifiersManager();
        if(mm) {
            if(mm.IsModifierActive(eModifiers.MDF_POISONING) || mm.IsModifierActive(eModifiers.MDF_TOXICITY) || mm.IsModifierActive(eModifiers.MDF_CONTAMINATION1) || mm.IsModifierActive(eModifiers.MDF_CONTAMINATION2) || mm.IsModifierActive(eModifiers.MDF_CONTAMINATION3) || mm.IsModifierActive(eModifiers.MDF_AREAEXPOSURE))
                return GL_DEATH_REASON_POISONING;
            if(mm.IsModifierActive(eModifiers.MDF_CHOLERA) || mm.IsModifierActive(eModifiers.MDF_INFLUENZA) || mm.IsModifierActive(eModifiers.MDF_SALMONELLA) || mm.IsModifierActive(eModifiers.MDF_WOUND_INFECTION1) || mm.IsModifierActive(eModifiers.MDF_WOUND_INFECTION2) || mm.IsModifierActive(eModifiers.MDF_BRAIN) || mm.IsModifierActive(eModifiers.MDF_FEVER))
                return GL_DEATH_REASON_SICKNESS;
        }

        if(this.GetStatEnergy() && this.GetStatEnergy().Get() <= 0) return GL_DEATH_REASON_STARVATION;
        if(this.GetStatWater() && this.GetStatWater().Get() <= 0) return GL_DEATH_REASON_DEHYDRATION;
        if(this.GetStatHeatComfort() && this.GetStatHeatComfort().Get() <= this.GetStatHeatComfort().GetMin())
            return GL_DEATH_REASON_HYPOTHERMIA;

        return GL_DEATH_REASON_ENVIRONMENT;
    }

    void GLProcessKill(Object killer) {
        string cftoolsId = GetGameLabs().GetPlayerUpstreamIdentity(this.GetPlainId());
        if(cftoolsId) {
            GLPlayerStatistics playerStats = GetGameLabs().GetPlayerStatisticsByCFToolsId(cftoolsId);
            if (this.StatGet("dist") >= playerStats.startingDistance) {
                playerStats.distance += (this.StatGet("dist") - playerStats.startingDistance);
            }
            playerStats.deaths++;
        }

        _Payload_PlayerDeath payload;
        _LogPlayerEx logObjectMurderer;
        _LogPlayerEx logObjectPlayer = new _LogPlayerEx(this);

        EntityAI weapon;

        PlayerBase murderer;
        if(killer.IsWeapon() || killer.IsMeleeWeapon()) {
            weapon = EntityAI.Cast(killer);
            murderer = PlayerBase.Cast(weapon.GetHierarchyParent());
        }

        GetGameLabs().GetLogger().Debug(string.Format("EEKilled(this=%1, killer=%2, weapon=%3, murderer=%4)", this, killer, weapon, murderer));

        if(murderer) {
            string murdererReason = GL_DEATH_REASON_PLAYER;
            #ifdef EXPANSIONMODAI
            if(murderer.Expansion_IsAI()) murdererReason = GL_DEATH_REASON_EXPANSION_AI;
            #endif
            
            if(murderer.GetPlainId() != "") {
                logObjectMurderer = new _LogPlayerEx(murderer);
                payload = new _Payload_PlayerDeath(logObjectPlayer, logObjectMurderer, killer.GetType(), killer.GetDisplayName(), murdererReason);
            } else {
                payload = new _Payload_PlayerDeath(logObjectPlayer, NULL, killer.GetType(), killer.GetDisplayName(), murdererReason);
            }
        } else if(this == killer) { // Suicide, potentially Environmental death
            if(weapon) {
                payload = new _Payload_PlayerDeath(logObjectPlayer, NULL, killer.GetType(), killer.GetDisplayName(), GL_DEATH_REASON_SUICIDE);
            } else if(this.CommitedSuicide() || this.CommitedSuicide()) {
                weapon = this.GetItemInHands();
                if(weapon) payload = new _Payload_PlayerDeath(logObjectPlayer, NULL, "__Suicide", weapon.GetType(), GL_DEATH_REASON_SUICIDE);
                else payload = new _Payload_PlayerDeath(logObjectPlayer, NULL, "__Suicide", "", GL_DEATH_REASON_SUICIDE);
            }
            else {
                if(this.gl_lastDamageType) {
                    string refType = "";
                    if(this.gl_lastDamagingEntity) refType = this.gl_lastDamagingEntity.GetType();

                    switch(this.gl_lastDamageType) {
                        case DT_EXPLOSION: {
                            payload = new _Payload_PlayerDeath(logObjectPlayer, NULL, "__Explosion", refType, GL_DEATH_REASON_EXPLOSION);
                            break;
                        }
                        case DT_CUSTOM: {
                            if(this.gl_lastDamageAmmo == "FallDamage") payload = new _Payload_PlayerDeath(logObjectPlayer, NULL, "__FallDamage", refType, GL_DEATH_REASON_FALL_DAMAGE);
                            else payload = new _Payload_PlayerDeath(logObjectPlayer, NULL, "__Environment", refType, this.GLResolveEnvironmentReason());
                            break;
                        }
                        default: {
                            payload = new _Payload_PlayerDeath(logObjectPlayer, NULL, "__Environment", refType, this.GLResolveEnvironmentReason());
                            break;
                        }
                    }
                } else payload = new _Payload_PlayerDeath(logObjectPlayer, NULL, "__Environment", "", this.GLResolveEnvironmentReason());
            }

        } else { // Non player AI
            if(killer.IsInherited(ZombieBase)) {
                payload = new _Payload_PlayerDeath(logObjectPlayer, NULL, "__Infected", killer.GetType(), GL_DEATH_REASON_INFECTED);
            } else if(killer.IsInherited(AnimalBase)) {
                payload = new _Payload_PlayerDeath(logObjectPlayer, NULL, "__Animal", killer.GetDisplayName(), GL_DEATH_REASON_ANIMAL);
            } else payload = new _Payload_PlayerDeath(logObjectPlayer, NULL, "__Object", killer.GetType(), GL_DEATH_REASON_OBJECT);
        }

        GetGameLabs().GetApi().PlayerDeath(new _Callback(), payload);
    }

    // Sends killer context to the victim's client so the death-screen report can
    // pre-fill the target. Only a display name and an opaque pseudo id are sent;
    // the real steam id never leaves the server.
    void GLSendReportDeathContext(Object killer) {
        if(!GetGame().IsServer()) return;
        if(!this.GetIdentity()) return;

        GameLabsReportDeathContext deathContext = new GameLabsReportDeathContext();
        deathContext.hasPlayerKiller = false;

        PlayerBase murderer;
        if(killer) {
            if(killer.IsWeapon() || killer.IsMeleeWeapon()) {
                EntityAI killerWeapon = EntityAI.Cast(killer);
                if(killerWeapon) murderer = PlayerBase.Cast(killerWeapon.GetHierarchyParent());
            }
            if(!murderer) murderer = PlayerBase.Cast(killer);
        }

        if(murderer && murderer != this && murderer.GetIdentity()) {
            string killerSteam64 = murderer.GetPlainId();
            if(killerSteam64 != "") {
                deathContext.hasPlayerKiller = true;
                deathContext.killerName = murderer.GetPlayerName();
                deathContext.killerPseudoId = GetGameLabs().RegisterReportPseudo(killerSteam64);
            }
        }

        Param1<ref GameLabsReportDeathContext> payload = new Param1<ref GameLabsReportDeathContext>(deathContext);
        GetGame().RPCSingleParam(null, GameLabsRPCS.RE_REPORTDEATHCTX, payload, true, this.GetIdentity());
    }

    override void EEKilled(Object killer) {
        super.EEKilled(killer);
        if(!GetGame().IsServer()) return;

        // In-game reporting death context is independent of statistics reporting.
        if(GetGameLabs() && GetGameLabs().GetConfiguration() && GetGameLabs().GetConfiguration().IsReportingAvailable()) {
            this.GLSendReportDeathContext(killer);
        }

        if(!GetGameLabs().IsStatReportingEnabled()) return;
        if(this.gl_deathProcessed) return;
        this.gl_deathProcessed = true;

        this.GLProcessKill(killer);
    }

    override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef) {
        this.gl_lastDamageAmmo = ammo;
        this.gl_lastDamageType = damageType;
        this.gl_lastDamagingEntity = source;
        super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
        if(!GetGame().IsServer()) return;
        if(!GetGameLabs().IsStatReportingEnabled()) return;
        if(!damageResult) return; // NULL for some scripted damage sources

        GetGameLabs().GetLogger().Debug(string.Format("EEHitBy.EVAL (deathSync=%1, suicide=%2)", this.m_DeathSyncSent, this.CommitedSuicide()));
        if(this.m_DeathSyncSent || this.CommitedSuicide()) return; // Prevent logging of hits for dead bodies

        // Damage taken by the victim, regardless of source type
        GLPlayerStatistics gl_victimStats = this.GetGLPlayerStatistics();
        if(gl_victimStats) gl_victimStats.damageTaken += damageResult.GetDamage(dmgZone, "Health");

        _Payload_PlayerDamage payload;
        _LogPlayerEx logObjectMurderer;
        _LogPlayerEx logObjectPlayer = new _LogPlayerEx(this);

        PlayerBase murderer;
        if(source) {
            murderer = PlayerBase.Cast(source.GetHierarchyParent());
        }
        if(!source || !murderer) return;

        // Only tracks players that have an upstream identity eg. are actual players with CFCloud info
        if(murderer.HasUpstreamIdentity()) {
            string cftoolsId = murderer.GetUpstreamIdentity();
            GLPlayerStatistics playerStatistics = GetGameLabs().GetPlayerStatisticsByCFToolsId(cftoolsId);

            playerStatistics.damageDealt += damageResult.GetDamage(dmgZone, "Health");

            if(!IsAlive()) {
                // Player dead, last shot that actually killed the player
                if(!this.gl_hitTracked) {
                    this.gl_hitTracked = true;
                    playerStatistics.shotsHit++;
                    playerStatistics.shotsHitPlayers++;

                    playerStatistics.pvpKills++;
                    if(dmgZone == "Head") playerStatistics.headshotKills++;

                    float killDistance = vector.Distance(murderer.GetPosition(), this.GetPosition());
                    if(killDistance > playerStatistics.longestKillDistanceMeters) {
                        playerStatistics.longestKillDistanceMeters = killDistance;
                    }
                }
            } else {
                // Player alive
                playerStatistics.shotsHit++;
                playerStatistics.shotsHitPlayers++;
            }
        }

        GetGameLabs().GetLogger().Debug(string.Format("EEHitBy(this=%1, murderer=%2, source=%3, component=%4, dmgZone=%5, ammo=%6, modelPos=%7, speedCoef=%8)", this, murderer, source, component, dmgZone, ammo, modelPos, speedCoef));
        GetGameLabs().GetLogger().Debug(string.Format("^EEHitBy(GetDamage(%1), GetHighestDamage(%2))", damageResult.GetDamage(dmgZone, "Health"), damageResult.GetHighestDamage("Health")));

        // AI attackers (e.g. Expansion AI) have no upstream identity
        if(murderer.GetPlainId() == "") return;

        logObjectMurderer = new _LogPlayerEx(murderer);
        payload = new _Payload_PlayerDamage(logObjectPlayer, logObjectMurderer, source, damageResult.GetDamage(dmgZone, "Health"), dmgZone);
        GetGameLabs().GetApi().PlayerDamage(new _Callback(), payload);

        if(IsAlive()) {
            this.gl_deathProcessed = false;
        } else if(!this.gl_deathProcessed) {
            this.gl_deathProcessed = true;
            this.GLProcessKill(source);
        }
    }

    /* ********* STATS SETTER ********** */
    void GL_IncPickedUp() {
        GLPlayerStatistics stats = this.GetGLPlayerStatistics();
        if(!stats) return;
        stats.itemsPickedUp++;
    }
    void GL_IncDropped() {
        GLPlayerStatistics stats = this.GetGLPlayerStatistics();
        if(!stats) return;
        stats.itemsDropped++;
    }
    void GL_IncWeaponsLooted() {
        GLPlayerStatistics stats = this.GetGLPlayerStatistics();
        if(!stats) return;
        stats.weaponsLooted++;
    }
    void GL_IncPlayersLooted()  {
        GLPlayerStatistics stats = this.GetGLPlayerStatistics();
        if(!stats) return;
        stats.playersLooted++;
    }
    void GL_IncAILooted()       {
        GLPlayerStatistics stats = this.GetGLPlayerStatistics();
        if(!stats) return;
        stats.aiLooted++;
    }
    void GL_IncGrenadeUsed()    {
        GLPlayerStatistics stats = this.GetGLPlayerStatistics();
        if(!stats) return;
        stats.grenadesUsed++;
    }
    void GL_IncBleedFixed()     {
        GLPlayerStatistics stats = this.GetGLPlayerStatistics();
        if(!stats) return;
        stats.bleedsFixed++;
    }
    void GL_IncHealthItemUsed() {
        GLPlayerStatistics stats = this.GetGLPlayerStatistics();
        if(!stats) return;
        stats.healthItemsUsed++;
    }
    void GL_IncRespawn()        {
        GLPlayerStatistics stats = this.GetGLPlayerStatistics();
        if(!stats) return;
        stats.playerRespawns++;
    }
    void GL_IncFoodItemConsumed()   {
        GLPlayerStatistics stats = this.GetGLPlayerStatistics();
        if(!stats) return;
        stats.foodItemsConsumed++;
    }
    void GL_IncDrinkItemConsumed()  {
        GLPlayerStatistics stats = this.GetGLPlayerStatistics();
        if(!stats) return;
        stats.drinkItemsConsumed++;
    }
    void GL_AddFoodWeightKg(float kg)      {
        GLPlayerStatistics stats = this.GetGLPlayerStatistics();
        if(!stats) return;
        stats.foodWeightConsumed += kg;
    }
    void GL_AddDrinkVolumeLiters(float l)  {
        GLPlayerStatistics stats = this.GetGLPlayerStatistics();
        if(!stats) return;
        stats.drinkVolumeConsumed += l;
    }
    void GL_AddVehicleDist(float meters)   {
        GLPlayerStatistics stats = this.GetGLPlayerStatistics();
        if(!stats) return;
        stats.vehicleDistance += meters;
    }
};
