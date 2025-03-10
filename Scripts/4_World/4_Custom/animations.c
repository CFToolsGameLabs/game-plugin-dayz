modded class ModItemRegisterCallbacks {
	override void RegisterFireArms(DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior) {
        super.RegisterFireArms( pType, pBehavior );
              
		//pType.AddItemInHandsProfileIK("HK416_Base_CFTools", "dz/anims/workspaces/player/player_main/weapons/player_main_m4a1.asi", pBehavior, "dz/anims/anm/player/ik/weapons/m4a1_ik.anm", "dz/anims/anm/player/reloads/M4A1/w_M4A1_states.anm");
	}
	
	override void RegisterOneHanded(DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior) {
		super.RegisterOneHanded(pType, pBehavior);
		
		//pType.AddItemInHandsProfileIK("Mag_STANAGDrum_60Rnd_CFTools", "dz/anims/workspaces/player/player_main/props/player_main_STANAG_mag.asi", pBehavior, "dz/anims/anm/player/ik/attachments/magazines/magazine_stanag30.anm");
	}
};