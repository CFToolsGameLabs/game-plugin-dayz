modded class Weapon_Base {
    float m_TargetDistance;

    override void EEFired(int muzzleType, int mode, string ammoType) {
        super.EEFired(muzzleType, mode, ammoType);
        if(!GetGameLabs().IsServer()) return;
        PlayerBase player = PlayerBase.Cast(GetHierarchyRootPlayer());
        if(player) {
            if(player.HasUpstreamIdentity()) {
                string cftoolsId = player.GetUpstreamIdentity();
                ref GLPlayerStatistics playerStatistics = GetGameLabs().GetPlayerStatisticsByCFToolsId(cftoolsId);
                playerStatistics.shotsFired++;
            }
        }
    }



    /*
    void EEFired(int muzzleType, int mode, string ammoType) {
        super.EEFired(muzzleType, mode, ammoType);
        if(GetGameLabs().IsServer()) return;
        if(!GetGameLabs().GetDebugStatus()) return;

        vector cameraDirection = GetGame().GetCurrentCameraDirection();
        vector cameraPosition = GetGame().GetCurrentCameraPosition();
        vector usti_hlavne_position = this.GetSelectionPositionMS( "usti hlavne" );//usti hlavne
        usti_hlavne_position = this.ModelToWorld(usti_hlavne_position);

        Debug_LogHit(usti_hlavne_position, cameraDirection, cameraPosition);
    }

    void Debug_LogHit(vector begin_point, vector camera_dir, vector camera_pos) {
        int contact_component;
        vector contact_point_cam_trace;
        vector contact_dir_camera;

        vector end_point = camera_pos + camera_dir * 1500;
        Man player = GetGame().GetPlayer();

        Object player_o;
        Class.CastTo(player_o, player);

        set<Object> results = new set<Object>;

        if(DayZPhysics.RaycastRV(camera_pos, end_point, contact_point_cam_trace, contact_dir_camera, contact_component, results, null, player_o , false, false, CollisionFlags.ALLOBJECTS, 0.1)){
            for(int j = 0; j < results.Count(); ++j) {
                Object contact_obj = results.Get(j);

                GetGameLabs().GetLogger().Info(string.Format("EEFired.HIT %1, %2", contact_obj, Object.GetDebugName(contact_obj)));

                Param1<string> hitref = new Param1<string>( Object.GetDebugName(contact_obj) );
                GetGame().RPCSingleParam(NULL, GameLabsRPCS.RP_HITINFO, hitref, true);
            }
        }
    }
    */
};