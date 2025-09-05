modded class ItemBase extends InventoryItem {
    private string gl_persistentItemId;
    float m_GLStartQty; // Used for quantity measurements

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

    /* **************** Pickup Tracking **************** */
    override void EEItemLocationChanged(notnull InventoryLocation oldLoc, notnull InventoryLocation newLoc) {
        super.EEItemLocationChanged(oldLoc, newLoc);
        if(!GetGameLabs()) return;
        if(!GetGameLabs().IsServer()) return;

        PlayerBase oldOwner = null;
        PlayerBase newOwner = null;

        EntityAI oldParent = oldLoc.GetParent();
        if(oldParent) oldOwner = PlayerBase.Cast(oldParent.GetHierarchyRootPlayer());

        EntityAI newParent = newLoc.GetParent();
        if(newParent) newOwner = PlayerBase.Cast(newParent.GetHierarchyRootPlayer());

        if(newOwner && newOwner != oldOwner)
            newOwner.GL_IncPickedUp();

        if(oldOwner && oldOwner != newOwner)
            oldOwner.GL_IncDropped();

        // classify the *source* for the new owner
        if(newOwner && newOwner != oldOwner) {
            // player -> player transfer
            if (oldOwner) newOwner.GL_IncPlayersLooted();

            // AI (infected/animal) -> player
            if (oldParent && (oldParent.IsInherited(DayZInfected) || oldParent.IsInherited(AnimalBase)))
                newOwner.GL_IncAILooted();

            // weapons looted from anywhere
            if (IsInherited(Weapon_Base))
                newOwner.GL_IncWeaponsLooted();
        }
    }
};
