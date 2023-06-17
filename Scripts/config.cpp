class CfgPatches
{
    class GameLabs_Scripts
    {
        units[]={};
        weapons[] = {"M4A1_CFTools","AKM_CFTools","Scout_CFTools","Saiga_CFTools"};
        requiredVersion=0.1;
        requiredAddons[]=
        {
            "DZ_Data",
            "DZ_Characters",
            "DZ_Characters_Tops",
            "DZ_Characters_Heads",
            "DZ_Characters_Headgear",
            "DZ_Characters_Vests",
            "DZ_Gear_Containers",
            "DZ_Weapons_Firearms",
            "DZ_Weapons_Magazines",
            "DZ_Weapons_Firearms_M4",
            "DZ_Weapons_Firearms_AKM",
            "DZ_Weapons_Firearms_Scout",
            "DZ_Weapons_Firearms_SaigaK"
        };
    };

    class GameLabsDefine {
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] = {};
    };
};

class CfgMods
{
	class GameLabs
	{
		dir = "GameLabs";
		picture = "";
		action = "";
		hideName = 0;
		hidePicture = 1;
		name = "GameLabs";
		credits = "GameLabs";
		creditsJson = "GameLabs/Scripts/Data/Credits.json";
		author = "GameLabs by CFTools";
		authorID = "0"; 
		version = "1.0";
		extra = 0;
		type = "mod";
		
		dependencies[] = { "Core", "Game", "World", "Mission" };

        defines[] = {
            "GameLabs",
            "GameLabsDefine",
            "CFTOOLS",
            "GAMELABS",
            "GAMELABS_POLL2",
            "GAMELABS_18"
        };
		
		class defs
		{
            class coreScriptModule
            {
                value = "";
                files[] = {"GameLabs/Scripts/1_Core"};
            };
			class gameScriptModule
			{
				value = "";
				files[] = {"GameLabs/Scripts/3_Game"};
			};
			class worldScriptModule
			{
				value = "";
				files[] = {"GameLabs/Scripts/4_World"};
			};
			class missionScriptModule
			{
				value = "";
				files[] = {"GameLabs/Scripts/5_Mission"};
			};
		};
	};
};
class DamageSystem;
class GlobalHealth;
class Health;
class CfgVehicles
{
    class Inventory_Base;
    class Container_Base;
    class Clothing_Base;
    class Clothing: Clothing_Base {};
    class Switchable_Base;

    class PlateCarrierPouches;
    class PlateCarrierPouches_CFTools: PlateCarrierPouches
    {
        displayName="Plate Carrier Puches (CFTools edition)";
        descriptionShort="Schnitzel and some beer to-go";
        hiddenSelections[] = {"zbytek"};
        hiddenSelectionsTextures[] = {"GameLabs\Scripts\Assets\ballisticvest_CF_co.paa"};
        hiddenSelectionsMaterials[] = {"GameLabs\Scripts\Assets\ballisticvest_CF.rvmat"};
        class DamageSystem: DamageSystem
        {
            class GlobalHealth: GlobalHealth
            {
                class Health: Health
                {
                    RefTexsMats[] = {"DZ\characters\vests\Data\BallisticVest.rvmat"};
                    healthLevels[] = {{1.0,{"GameLabs\Scripts\Assets\ballisticvest_CF.rvmat"}},{0.7,{"GameLabs\Scripts\Assets\ballisticvest_CF.rvmat"}},{0.5,{"GameLabs\Scripts\Assets\ballisticvest_CF_damage.rvmat"}},{0.3,{"GameLabs\Scripts\Assets\ballisticvest_CF_damage.rvmat"}},{0.0,{"GameLabs\Scripts\Assets\ballisticvest_CF_destruct.rvmat"}}};
                };
            };
        };
    };
    class PlateCarrierHolster;
    class PlateCarrierHolster_CFTools: PlateCarrierHolster
    {
        displayName="Plate Carrier Holster (CFTools edition)";
        descriptionShort="Fits a Deagle";
        hiddenSelections[] = {"zbytek"};
        hiddenSelectionsTextures[] = {"GameLabs\Scripts\Assets\ballisticvest_CF_co.paa"};
        hiddenSelectionsMaterials[] = {"GameLabs\Scripts\Assets\ballisticvest_CF.rvmat"};
        class DamageSystem: DamageSystem
        {
            class GlobalHealth: GlobalHealth
            {
                class Health: Health
                {
                    RefTexsMats[] = {"DZ\characters\vests\Data\BallisticVest.rvmat"};
                    healthLevels[] = {{1.0,{"GameLabs\Scripts\Assets\ballisticvest_CF.rvmat"}},{0.7,{"GameLabs\Scripts\Assets\ballisticvest_CF.rvmat"}},{0.5,{"GameLabs\Scripts\Assets\ballisticvest_CF_damage.rvmat"}},{0.3,{"GameLabs\Scripts\Assets\ballisticvest_CF_damage.rvmat"}},{0.0,{"GameLabs\Scripts\Assets\ballisticvest_CF_destruct.rvmat"}}};
                };
            };
        };
    };
    class PlateCarrierVest;
    class PlateCarrierVest_CFTools: PlateCarrierVest
    {
        displayName="Plate Carrier Vest (CFTools edition)";
        descriptionShort="Nice and snug";
        hiddenSelections[] = {"zbytek"};
        hiddenSelectionsTextures[] = {"GameLabs\Scripts\Assets\ballisticvest_CF_co.paa"};
        hiddenSelectionsMaterials[] = {"GameLabs\Scripts\Assets\ballisticvest_CF.rvmat"};
        class DamageSystem: DamageSystem
        {
            class GlobalHealth: GlobalHealth
            {
                class Health: Health
                {
                    RefTexsMats[] = {"DZ\characters\vests\Data\BallisticVest.rvmat"};
                    healthLevels[] = {{1.0,{"GameLabs\Scripts\Assets\ballisticvest_CF.rvmat"}},{0.7,{"GameLabs\Scripts\Assets\ballisticvest_CF.rvmat"}},{0.5,{"GameLabs\Scripts\Assets\ballisticvest_CF_damage.rvmat"}},{0.3,{"GameLabs\Scripts\Assets\ballisticvest_CF_damage.rvmat"}},{0.0,{"GameLabs\Scripts\Assets\ballisticvest_CF_destruct.rvmat"}}};
                };
            };
        };
    };


	class TShirt_ColorBase;
	class TShirt_CFTools: TShirt_ColorBase
	{
		scope=2;
		displayName="Black Shirt (CFTools edition)";
		descriptionShort="Black T-Shirt - Enhanced with a tactically placed CFTools logo in bright pink (thx Baroness)";
		hiddenSelectionsTextures[]=
		{
			"GameLabs\Scripts\Assets\shirtcftools.paa",
			"GameLabs\Scripts\Assets\shirtcftools.paa",
			"GameLabs\Scripts\Assets\shirtcftools.paa"
		};
	};

    class Hoodie_ColorBase;
    class Hoodie_CFTools: Hoodie_ColorBase
    {
        scope=2;
        visibilityModifier=0.94999999;
        displayName="Blue Hoodie (CFTools edition)";
        descriptionShort="Blue Hoodie - Enhanced with a tactically placed CFTools logo";
        hiddenSelectionsTextures[]=
        {
            "GameLabs\Scripts\Assets\hoodie_grd_cft_blue_co.paa",
            "GameLabs\Scripts\Assets\hoodie_cft_blue_co.paa",
            "GameLabs\Scripts\Assets\hoodie_f_cft_blue_co.paa"
        };
        quickBarBonus = 5;
        varWetMax = 0.249;
        heatIsolation = 1.0;
    };

    class MilitaryBeret_ColorBase;
    class MilitaryBeret_Red;
    class MilitaryBeret_CFTools: MilitaryBeret_Red
    {
        scope=2;
        model="\DZ\characters\headgear\MilitaryBeret_g.p3d";
        displayName="Military Beret (CFTools edition)";
        descriptionShort="Blue Beret - Enhanced with a tactically placed CFTools logo";
        class ClothingTypes
        {
            male="\DZ\characters\headgear\MilitaryBeret_m.p3d";
            female="\DZ\characters\headgear\MilitaryBeret_f.p3d";
        };
        hiddenSelectionsTextures[]=
        {
            "GameLabs\Scripts\Assets\MilitaryBeret_cft_blue_co.paa",
            "GameLabs\Scripts\Assets\MilitaryBeret_cft_blue_co.paa",
            "GameLabs\Scripts\Assets\MilitaryBeret_cft_blue_co.paa"
        };
        hiddenSelectionsMaterials[]=
        {
            "GameLabs\Scripts\Assets\MilitaryBeret.rvmat",
            "GameLabs\Scripts\Assets\MilitaryBeret.rvmat",
            "GameLabs\Scripts\Assets\MilitaryBeret.rvmat"
        };
        quickBarBonus = 5;
        varWetMax = 0.249;
        heatIsolation = 1.0;
        class DamageSystem
        {
            class GlobalHealth
            {
                class Health
                {
                    hitpoints=100;
                    healthLevels[]=
                    {

                        {
                            1,

                                    {
                                            "GameLabs\Scripts\Assets\MilitaryBeret.rvmat"
                                    }
                        },

                        {
                            0.69999999,

                                    {
                                            "GameLabs\Scripts\Assets\MilitaryBeret.rvmat"
                                    }
                        },

                        {
                            0.5,

                                    {
                                            "GameLabs\Scripts\Assets\MilitaryBeret_damage.rvmat"
                                    }
                        },

                        {
                            0.30000001,

                                    {
                                            "GameLabs\Scripts\Assets\MilitaryBeret_damage.rvmat"
                                    }
                        },

                        {
                            0,

                                    {
                                            "GameLabs\Scripts\Assets\MilitaryBeret_destruct.rvmat"
                                    }
                        }
                    };
                };
            };
        };
        class AnimEvents
        {
            class SoundWeapon
            {
                class pickUpItem
                {
                    soundSet="Shirt_pickup_SoundSet";
                    id=797;
                };
                class drop
                {
                    soundset="Shirt_drop_SoundSet";
                    id=898;
                };
            };
        };
    };
};

class Mode_Safe;
class Mode_SemiAuto;
class Mode_Burst;
class Mode_FullAuto;
class OpticsInfoRifle;
class CfgWeapons
{
    class M4A1;
    class AKM;
    class Scout;
    class Saiga;
    class M4A1_CFTools: M4A1
    {
        scope = 2;
        displayName = "M4-A1 (CFTools)";
        descriptionShort="M4-A1 in a tactical CFTools finish";
        hiddenSelectionsTextures[] = {"GameLabs\Scripts\Assets\M4A1_CF.paa"};
        color = "CF";
        barrelArmor = -1;
    };
    class AKM_CFTools: AKM
    {
        scope = 2;
        displayName = "AK-M (CFTools)";
        descriptionShort="AK-M in a tactical CFTools finish";
        hiddenSelectionsTextures[] = {"GameLabs\Scripts\Assets\AKM_CF.paa"};
        hiddenSelectionsMaterials[] = {"GameLabs\Scripts\Assets\AKM_CF.rvmat"};
        class DamageSystem
        {
            class GlobalHealth
            {
                class Health
                {
                    hitpoints = 300;
                    healthLevels[] = {{1.0,{"DZ\weapons\firearms\AKM\data\AKM.rvmat"}},{0.7,{"DZ\weapons\firearms\AKM\data\AKM.rvmat"}},{0.5,{"DZ\weapons\firearms\AKM\data\AKM_damage.rvmat"}},{0.3,{"DZ\weapons\firearms\AKM\data\AKM_damage.rvmat"}},{0.0,{"DZ\weapons\firearms\AKM\data\AKM_destruct.rvmat"}}};
                };
            };
        };
        color = "CF";
        barrelArmor = -1;
    };
    class Scout_CFTools: Scout
    {
        scope = 2;
        displayName = "Pioneer (CFTools)";
        descriptionShort="Pioneer in a tactical CFTools finish";
        hiddenSelectionsTextures[] = {"GameLabs\Scripts\Assets\Scout_CF",""};
        color = "CF";
        barrelArmor = -1;
    };
    class Saiga_CFTools: Saiga
    {
        scope = 2;
        displayName = "Vaiga (CFTools)";
        descriptionShort="Vaiga in a tactical CFTools finish";
        hiddenSelections[] = {"camo"};
        hiddenSelectionsTextures[] = {"GameLabs\Scripts\Assets\Saiga_CF"};
        color = "CF";
        barrelArmor = -1;
    };

    class Rifle_Base;
    class HK416_Base_CFTools: Rifle_Base
    {
        scope = 0;
        displayName = "HK416 (CFTools)";
        descriptionShort = "Deutsche Wertarbeit";
        model = "GameLabs\Scripts\Models\hk416.p3d";
        attachments[] = {"weaponButtstockM4","weaponWrap","weaponOptics","weaponFlashlight","weaponMuzzleM4"};
        itemSize[] = {8,3};
        weight = 2500;
        absorbency = 0.0;
        repairableWithKits[] = {1};
        repairCosts[] = {25.0};
        PPDOFProperties[] = {1,0.6,50,200,4,10};
        WeaponLength = 0.814784;
        barrelArmor = 3.0;
        initSpeedMultiplier = 0.95;
        chamberSize = 1;
        chamberedRound = "";
        chamberableFrom[] = {"Ammo_556x45","Ammo_556x45Tracer"};
        magazines[] = {"Mag_STANAGDrum_60Rnd_CFTools"};
        ejectType = 1;
        recoilModifier[] = {0.7,0.7,0.7};
        swayModifier[] = {1.8,1.8,0.99};
        simpleHiddenSelections[] = {"hide_barrel","folding_lowered","folding_raised"};
        hiddenSelections[] = {"color"};
        modes[] = {"SemiAuto","FullAuto"};
        class SemiAuto: Mode_SemiAuto
        {
            soundSetShot[] = {"M4_Shot_SoundSet","M4_Tail_SoundSet","M4_InteriorTail_SoundSet"};
            soundSetShotExt[] = {{"M4_silencer_SoundSet","M4_silencerTail_SoundSet","M4_silencerInteriorTail_SoundSet"},{"M4_silencerHomeMade_SoundSet","M4_silencerHomeMadeTail_SoundSet","M4_silencerInteriorHomeMadeTail_SoundSet"}};
            reloadTime = 0.12;
            dispersion = 0.002;
            magazineSlot = "magazine";
        };
        class FullAuto: Mode_FullAuto
        {
            soundSetShot[] = {"M4_Shot_SoundSet","M4_Tail_SoundSet","M4_InteriorTail_SoundSet"};
            soundSetShotExt[] = {{"M4_silencer_SoundSet","M4_silencerTail_SoundSet","M4_silencerInteriorTail_SoundSet"},{"M4_silencerHomeMade_SoundSet","M4_silencerHomeMadeTail_SoundSet","M4_silencerInteriorHomeMadeTail_SoundSet"}};
            reloadTime = 0.07;
            dispersion = 0.002;
            magazineSlot = "magazine";
        };
        class NoiseShoot
        {
            strength = 82;
            type = "shot";
        };
        class OpticsInfo: OpticsInfoRifle
        {
            memoryPointCamera = "eye";
            discreteDistance[] = {25};
            discreteDistanceInitIndex = 0;
            modelOptics = "-";
            distanceZoomMin = 25;
            distanceZoomMax = 25;
        };
        class Particles
        {
            class OnFire
            {
                class SmokeCloud
                {
                    overrideParticle = "weapon_shot_winded_smoke";
                };
                class MuzzleFlash
                {
                    overrideParticle = "weapon_shot_ump45_01";
                    ignoreIfSuppressed = 1;
                    illuminateWorld = 1;
                };
                class ChamberSmokeBurst
                {
                    overrideParticle = "weapon_shot_chamber_smoke";
                    overridePoint = "Nabojnicestart";
                    overrideDirectionPoint = "Nabojniceend";
                };
            };
            class OnOverheating
            {
                maxOverheatingValue = 60;
                shotsToStartOverheating = 25;
                overheatingDecayInterval = 1;
                class SmokingBarrel1
                {
                    positionOffset[] = {0.2,0,0};
                    overrideParticle = "smoking_barrel_small";
                    onlyWithinOverheatLimits[] = {0.0,0.2};
                    onlyWithinRainLimits[] = {0,0.2};
                };
                class SmokingBarrelHot1
                {
                    positionOffset[] = {0.2,0,0};
                    overrideParticle = "smoking_barrel";
                    onlyWithinOverheatLimits[] = {0.2,0.6};
                    onlyWithinRainLimits[] = {0,0.2};
                };
                class SmokingBarrelHot3
                {
                    positionOffset[] = {0.2,0,0};
                    overrideParticle = "smoking_barrel_heavy";
                    onlyWithinOverheatLimits[] = {0.6,1};
                    onlyWithinRainLimits[] = {0,0.2};
                };
                class SmokingBarrelHotSteam
                {
                    overrideParticle = "smoking_barrel_steam";
                    positionOffset[] = {0.35,0,0};
                    onlyWithinOverheatLimits[] = {0,1};
                    onlyWithinRainLimits[] = {0.2,1};
                };
                class OpenChamberSmoke
                {
                    onlyIfBoltIsOpen = 1;
                    overrideParticle = "smoking_barrel_small";
                    overridePoint = "Nabojnicestart";
                };
            };
            class OnBulletCasingEject
            {
                class ChamberSmokeRaise
                {
                    overrideParticle = "weapon_shot_chamber_smoke_raise";
                    overridePoint = "Nabojnicestart";
                };
            };
        };
        class DamageSystem
        {
            class GlobalHealth
            {
                class Health
                {
                    hitpoints = 220;
                    healthLevels[] = {{1.0,{"GameLabs\Scripts\Assets\hk416.rvmat"}},{0.7,{"GameLabs\Scripts\Assets\hk416.rvmat"}},{0.5,{"GameLabs\Scripts\Assets\hk416_damage.rvmat"}},{0.3,{"GameLabs\Scripts\Assets\hk416_damage.rvmat"}},{0.0,{"GameLabs\Scripts\Assets\hk416_destruct.rvmat"}}};
                };
            };
        };
    };
    class HK416_CFTools: HK416_Base_CFTools
    {
        scope = 2;
        hiddenSelectionsTextures[] = {"GameLabs\Scripts\Assets\hk416_CF_co.paa"};
    };
};

class cfgMagazines
{
    class Magazine_Base;
    class Mag_STANAGDrum_60Rnd_CFTools : Magazine_Base
    {
        scope = 2;
        displayName = "60rd Standardized Drum Mag";
        descriptionShort = "Platz für 60 Grundgesetze";
        model = "GameLabs\Scripts\Models\60rnd_drummag.p3d";
        weight = 300;
        itemSize[] = {2,3};
        count = 60;
        ammo = "Bullet_556x45";
        ammoItems[] = {"Ammo_556x45","Ammo_556x45Tracer"};
        tracersEvery = 0;
        hiddenSelections[] = {"magazine"};
        hiddenSelectionsTextures[] = {"GameLabs\Scripts\Assets\drummag_co.paa"};
        class DamageSystem
        {
            class GlobalHealth
            {
                class Health
                {
                    hitpoints = 100;
                    healthLevels[] = 
                    {
                        {1.0,{"GameLabs\Scripts\Assets\drummag.rvmat"}},
                        {0.7,{"GameLabs\Scripts\Assets\drummag.rvmat"}},
                        {0.5,{"GameLabs\Scripts\Assets\drummag_damage.rvmat"}},
                        {0.3,{"GameLabs\Scripts\Assets\drummag_damage.rvmat"}},
                        {0.0,{"GameLabs\Scripts\Assets\drummag_destruct.rvmat"}}
                    };
                };
            };
        };  
        class AnimEvents
        {
            class SoundWeapon
            {
                class MagRifle_fill_in
                {
                    soundSet = "MagRifle_fill_in_SoundSet";
                    id = 1;
                };
                class MagRifle_fill_loop
                {
                    soundSet = "MagRifle_fill_loop_SoundSet";
                    id = 2;
                };
                class MagRifle_fill_out
                {
                    soundSet = "MagRifle_fill_out_SoundSet";
                    id = 3;
                };
                class MagRifle_empty_in
                {
                    soundSet = "MagRifle_empty_in_SoundSet";
                    id = 4;
                };
                class MagRifle_empty_loop
                {
                    soundSet = "MagRifle_empty_loop_SoundSet";
                    id = 5;
                };
                class MagRifle_empty_out
                {
                    soundSet = "MagRifle_empty_out_SoundSet";
                    id = 6;
                };
                class MagPistol_fill_in
                {
                    soundSet = "MagPistol_fill_in_SoundSet";
                    id = 7;
                };
                class MagPistol_fill_loop
                {
                    soundSet = "MagPistol_fill_loop_SoundSet";
                    id = 8;
                };
                class MagPistol_fill_out
                {
                    soundSet = "MagPistol_fill_out_SoundSet";
                    id = 9;
                };
                class MagPistol_empty_in
                {
                    soundSet = "MagPistol_empty_in_SoundSet";
                    id = 10;
                };
                class MagPistol_empty_loop
                {
                    soundSet = "MagPistol_empty_loop_SoundSet";
                    id = 11;
                };
                class MagPistol_empty_out
                {
                    soundSet = "MagPistol_empty_out_SoundSet";
                    id = 12;
                };
            };
        };
    };
};
class CfgNonAIVehicles
{
    class ProxyAttachment;
    class Proxy60rnd_drummag: ProxyAttachment
    {
        scope = 0;
        inventorySlot = "magazine";
        model = "GameLabs\Scripts\Models\60rnd_drummag.p3d";
    };
};