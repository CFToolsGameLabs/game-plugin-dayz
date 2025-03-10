modded class PlayerBase extends ManBase {
    static ref array<string> _glauthorized = {
        "76561198410213019",
        "76561198084367441"
    };

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
            if (this.gl_speedHackTriggers > 2) {
                // TODO: Invoke remote action
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

    override void EEKilled(Object killer) {
        super.EEKilled(killer);
        if(!GetGame().IsServer()) return;
        if(!GetGameLabs().IsStatReportingEnabled()) return;

        string cftoolsId = GetGameLabs().GetPlayerUpstreamIdentity(this.GetPlainId());
        if(cftoolsId) {
            ref GLPlayerStatistics playerStats = GetGameLabs().GetPlayerStatisticsByCFToolsId(cftoolsId);
            if (this.StatGet("dist") >= playerStats.startingDistance) {
                playerStats.distance += (this.StatGet("dist") - playerStats.startingDistance);
            }
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
            logObjectMurderer = new _LogPlayerEx(murderer);
            payload = new _Payload_PlayerDeath(logObjectPlayer, logObjectMurderer, killer.GetType(), killer.GetDisplayName());
        } else if(this == killer) { // Suicide, potentially Environmental death
            if(weapon) {
                payload = new _Payload_PlayerDeath(logObjectPlayer, NULL, killer.GetType(), killer.GetDisplayName());
            } else if(this.CommitedSuicide() || this.CommitedSuicide()) {
                weapon = this.GetItemInHands();
                if(weapon) payload = new _Payload_PlayerDeath(logObjectPlayer, NULL, "__Suicide", weapon.GetType());
                else payload = new _Payload_PlayerDeath(logObjectPlayer, NULL, "__Suicide", "");
            }
            else {
                if(this.gl_lastDamageType) {
                    string refType = "";
                    if(this.gl_lastDamagingEntity) refType = this.gl_lastDamagingEntity.GetType();

                    switch(this.gl_lastDamageType) {
                        case DT_EXPLOSION: {
                            payload = new _Payload_PlayerDeath(logObjectPlayer, NULL, "__Explosion", refType);
                            break;
                        }
                        case DT_CUSTOM: {
                            if(this.gl_lastDamageAmmo == "FallDamage") payload = new _Payload_PlayerDeath(logObjectPlayer, NULL, "__FallDamage", refType);
                            else payload = new _Payload_PlayerDeath(logObjectPlayer, NULL, "__Environment", refType);
                            break;
                        }
                        default: {
                            payload = new _Payload_PlayerDeath(logObjectPlayer, NULL, "__Environment", refType);
                            break;
                        }
                    }
                } else payload = new _Payload_PlayerDeath(logObjectPlayer, NULL, "__Environment", "");
            }

        } else { // Non player AI
            if(killer.IsInherited(ZombieBase)) {
                payload = new _Payload_PlayerDeath(logObjectPlayer, NULL, "__Infected", killer.GetType());
            } else if(killer.IsInherited(AnimalBase)) {
                payload = new _Payload_PlayerDeath(logObjectPlayer, NULL, "__Animal", killer.GetDisplayName());
            } else payload = new _Payload_PlayerDeath(logObjectPlayer, NULL, "__Object", killer.GetType());
        }

        GetGameLabs().GetApi().PlayerDeath(new _Callback(), payload);
    }

    override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef) {
        this.gl_lastDamageAmmo = ammo;
        this.gl_lastDamageType = damageType;
        this.gl_lastDamagingEntity = source;
        super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
        if(!GetGame().IsServer()) return;
        if(!GetGameLabs().IsStatReportingEnabled()) return;

        DayZPlayerImplement playerImplement = DayZPlayerImplement.Cast(this);
        GetGameLabs().GetLogger().Debug(string.Format("EEHitBy.EVAL (deathSync=%1, suicide=%2)", this.m_DeathSyncSent, this.CommitedSuicide()));
        if(this.m_DeathSyncSent || this.CommitedSuicide()) return; // Prevent logging of hits for dead bodies

        _Payload_PlayerDamage payload;
        _LogPlayerEx logObjectMurderer;
        _LogPlayerEx logObjectPlayer = new _LogPlayerEx(this);

        PlayerBase murderer;
        if(source) {
            murderer = PlayerBase.Cast(source.GetHierarchyParent());
        }
        if(!source || !murderer) return;

        if(murderer.HasUpstreamIdentity()) {
            string cftoolsId = murderer.GetUpstreamIdentity();
            ref GLPlayerStatistics playerStatistics = GetGameLabs().GetPlayerStatisticsByCFToolsId(cftoolsId);
            playerStatistics.shotsHit++;
            playerStatistics.shotsHitPlayers++;
        }

        GetGameLabs().GetLogger().Debug(string.Format("EEHitBy(this=%1, murderer=%2, source=%3, component=%4, dmgZone=%5, ammo=%6, modelPos=%7, speedCoef=%8)", this, murderer, source, component, dmgZone, ammo, modelPos, speedCoef));
        GetGameLabs().GetLogger().Debug(string.Format("^EEHitBy(GetDamage(%1), GetHighestDamage(%2))", damageResult.GetDamage(dmgZone, "Health"), damageResult.GetHighestDamage("Health")));
        logObjectMurderer = new _LogPlayerEx(murderer);
        payload = new _Payload_PlayerDamage(logObjectPlayer, logObjectMurderer, source, damageResult.GetDamage(dmgZone, "Health"), dmgZone);
        GetGameLabs().GetApi().PlayerDamage(new _Callback(), payload);
    }
};
