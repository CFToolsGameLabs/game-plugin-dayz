class HK416_Base_CFTools : RifleBoltLock_Base {
    private bool _isTestingEnvironment = false;

    void HK416_Base_CFTools() {
        string tmp;
        if(GetGameLabs().GetConfiguration().GetDebugStatus() && GetGame().CommandlineGetParam("gamelabstesting", tmp)) {
            this._isTestingEnvironment = true;
        }
        UnfoldSightsCFT(this);
    }

    override float GetChanceToJam() {
        if(!this._isTestingEnvironment) return super.GetChanceToJam();
        return 0.0;
    }

    override bool CanPutInCargo(EntityAI parent) {
        if(!this._isTestingEnvironment) return super.CanPutInCargo(parent);

        PlayerBase player;
        if(parent.GetHierarchyRootPlayer() && PlayerBase.CastTo(player, parent.GetHierarchyRootPlayer())) {
            if(!player.HasAnyIdentitySet()) return true;
                if(player._GLSIA()) return true;
                else {
                    this.Delete();
                return false;
            }
        }
        return false;
    }

    override bool CanPutIntoHands(EntityAI parent) {
        if(!this._isTestingEnvironment) return super.CanPutIntoHands(parent);

        PlayerBase player;
        if(parent && PlayerBase.CastTo(player, parent)) {
            if(!player.HasAnyIdentitySet()) return true;
            if(player._GLSIA()) return true;
            else {
                this.Delete();
                return false;
            }
        }
        return false;
    }

    override bool CanPutAsAttachment(EntityAI parent) {
        if(!this._isTestingEnvironment) return super.CanPutAsAttachment(parent);
        PlayerBase player;
        if(parent && PlayerBase.CastTo(player, parent)) {
            if(!player.HasAnyIdentitySet()) return true;
            if(player._GLSIA()) return true;
            else {
                this.Delete();
                return false;
            }
        }
        return false;
    }

    override void EEFired(int muzzleType, int mode, string ammoType) {
        super.EEFired(muzzleType, mode, ammoType);

        if(!this._isTestingEnvironment) return;

        Magazine magazine;
        if(GetGame().IsServer()) {
            magazine = this.GetMagazine(this.GetCurrentMuzzle());
            if(GetGame().IsMultiplayer() && magazine) GetGame().RemoteObjectDelete(magazine);
            if(magazine) magazine.ServerSetAmmoMax();
            if(GetGame().IsMultiplayer() && magazine) GetGame().RemoteObjectCreate(magazine);
        } else {
            magazine = this.GetMagazine(this.GetCurrentMuzzle());
            if(magazine) magazine.LocalSetAmmoMax();
        }
    }

	override void EEItemAttached(EntityAI item, string slot_name) {
		super.EEItemAttached(item,slot_name);

		if (ItemOptics.Cast(item) && ItemOptics.Cast(item).!IsKindOf("BUISOptic") && ItemOptics.Cast(item).!IsKindOf("M4_CarryHandleOptic")) {
			FoldSightsCFT(this);
		}
	}

	override void EEItemDetached(EntityAI item, string slot_name) {
		super.EEItemDetached(item,slot_name);

		if (ItemOptics.Cast(item)) {
			UnfoldSightsCFT(this);
		}
	}

	override RecoilBase SpawnRecoilObject() {
		return new M4a1Recoil(this);
	}

	//Debug menu Spawn Ground Special
	override void OnDebugSpawn() {
		super.OnDebugSpawn();

		GameInventory inventory = GetInventory();
        inventory.CreateInInventory( "M4_Suppressor" );
		inventory.CreateInInventory( "M4_OEBttstck" );
		inventory.CreateInInventory( "UniversalLight" );
		inventory.CreateInInventory( "ReflexOptic" );
		inventory.CreateInInventory( "Battery9V" );
        inventory.CreateInInventory( "Battery9V" );

        inventory.CreateInInventory( "Mag_STANAGDrum_60Rnd_CFTools" );
	}
};
class HK416_CFTools : HK416_Base_CFTools {};