modded class AnalyticsManagerServer {
    override void OnEntityKilled(Object killer, EntityAI target) {
        super.OnEntityKilled(killer, target);

        EntityAI killer_entity = EntityAI.Cast(killer);
        if(killer_entity) {
            Man killer_survivor = killer_entity.GetHierarchyRootPlayer();
            if(killer_survivor) {
                if(target.IsZombie() || target.IsZombieMilitary()) {
                    OnInfectedKilledGL(killer_survivor, target);
                } else if(target.IsAnimal()) {
                    OnAnimalKilledGL(killer_survivor, target);
                }
            }
        }
    }

    void OnInfectedKilledGL(Man killer, EntityAI target ) {
        PlayerIdentity identity = killer.GetIdentity();
        if(identity) {
            if(identity.GetPlainId()) {
                string steam64 = identity.GetPlainId();
                ref GLPlayerStatistics playerStatistics = GetGameLabs().GetPlayerStatisticsBySteam64(steam64);
                if(playerStatistics) {
                    playerStatistics.killsInfected++;
                }
            }
        }
    }

    void OnAnimalKilledGL(Man killer, EntityAI target ) {
        PlayerIdentity identity = killer.GetIdentity();
        if(identity) {
            if(identity.GetPlainId()) {
                string steam64 = identity.GetPlainId();
                ref GLPlayerStatistics playerStatistics = GetGameLabs().GetPlayerStatisticsBySteam64(steam64);
                if(playerStatistics) {
                    playerStatistics.killsAnimals++;
                }
            }
        }
    }
};