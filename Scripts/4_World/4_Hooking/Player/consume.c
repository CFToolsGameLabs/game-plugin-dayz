modded class ActionEatBig {
    override void OnStartServer(ActionData action_data) {
        super.OnStartServer(action_data);
        ItemBase it = ItemBase.Cast(action_data.m_MainItem);
        //GetGameLabs().GetLogger().Debug(string.Format("ActionEatBig.OnStartServer(action_data=%1; item=%2;)", action_data, it));
        if(it) {
            //GetGameLabs().GetLogger().Debug(string.Format("^ ActionEatBig.OnStartServer(...) item ref exists, setting quantity... (%1)", it.GetQuantity()));
            it.m_GLStartQty = it.GetQuantity(); // grams
        }
    }

    override void OnEndServer(ActionData action_data) {
        super.OnEndServer(action_data);
        PlayerBase p = PlayerBase.Cast(action_data.m_Player);
        ItemBase it = ItemBase.Cast(action_data.m_MainItem);
        if(!p || !it) return;

        //GetGameLabs().GetLogger().Debug(string.Format("ActionEatBig.OnEndServer(action_data=%1; item=%2; player=%3;)", action_data, it, p));

        p.GL_IncFoodItemConsumed();

        float start = it.m_GLStartQty;
        float now   = it.GetQuantity();
        //GetGameLabs().GetLogger().Debug(string.Format("^ ActionEatBig.OnEndServer(...) (start=%1, now=%2)", start, now));
        if(start > 0 && now >= 0) {
            float delta = Math.Max(0, start - now); // assume grams
            p.GL_AddFoodWeightKg(delta / 1000.0);
            //GetGameLabs().GetLogger().Debug(string.Format("^ ActionEatBig.OnEndServer(...) Adding delta value %1/1000", delta));
        }
    }
}

// -------- DRINKING FROM ITEM (bottle/canteen/etc.) --------
modded class ActionDrink {
    override void OnStartServer(ActionData action_data) {
        super.OnStartServer(action_data);
        ItemBase it = ItemBase.Cast(action_data.m_MainItem);
        if(it) it.m_GLStartQty = it.GetQuantity(); // milliliters
    }

    override void OnEndServer(ActionData action_data) {
        super.OnEndServer(action_data);
        PlayerBase p = PlayerBase.Cast(action_data.m_Player);
        ItemBase it = ItemBase.Cast(action_data.m_MainItem);
        if(!p) return;

        p.GL_IncDrinkItemConsumed();

        // volume only when we drank from an item that actually has liquid quantity
        if(it) {
            float start = it.m_GLStartQty;
            float now   = it.GetQuantity();
            if(start > 0 && now >= 0) {
                float delta = Math.Max(0, start - now); // assume milliliters
                p.GL_AddDrinkVolumeLiters(delta / 1000.0);
            }
        }
    }
}

// -------- DRINKING FROM WORLD (pond/well) --------
modded class ActionDrinkPondContinuous {
    override void OnEndServer(ActionData action_data) {
        super.OnEndServer(action_data);
        PlayerBase p = PlayerBase.Cast(action_data.m_Player);
        if (p) p.GL_IncDrinkItemConsumed(); // no reliable volume -> count only
    }
}

modded class ActionDrinkWellContinuous {
    override void OnEndServer(ActionData action_data) {
        super.OnEndServer(action_data);
        PlayerBase p = PlayerBase.Cast(action_data.m_Player);
        if (p) p.GL_IncDrinkItemConsumed(); // count only
    }
}