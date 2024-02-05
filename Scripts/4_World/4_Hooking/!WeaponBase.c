class GameLabsBulletPostprocessor {
    private bool isActive = true;
    private bool appliedMagic = false;

    float start;
    float deviation;

    ref array<Object> bullets = new array<Object>;
    ref map<Object, ref array<vector>> bulletTracing = new map<Object, ref array<vector>>;
    void GameLabsBulletPostprocessor(ref array<Object> p_bullets) {
        this.bullets = p_bullets;
        this.start = GetGame().GetTickTime();
    }

    float GetTimeSinceShot() {
        return GetGame().GetTickTime() - start;
    }

    // C&P of BIs "Sleep"...
    void Sleep(float timeS) {
        float startTime = GetGame().GetTickTime();
        while (GetGame().GetTickTime() - startTime < timeS) {
            // Zzz
        }
    }

    void SendInvalidBulletNotification() {
        GetGameLabs().GetLogger().Info("SendInvalidBulletNotification()");

        this.isActive = false;
        if(GetGameLabs().GetConfiguration().GetMagicBulletInvalidateEnabled()) {
            foreach(Object bullet : this.bullets) {
                if(bullet != NULL) {
                    GetGame().ObjectDelete(bullet);
                }
            }
        }
        GetGame().RPCSingleParam(NULL, GameLabsRPCS.RQ_INVALIDBULLET, NULL, true);
    }

    // TODO: Cleanup calculation
    private void CalculateTrajectory(Object bullet, array<vector> trajectory) {
            #ifdef GAMELABSBPDEBUG
            GetGameLabs().GetLogger().Info("************* CalculateTrajectory *************");
            #endif

            float totalDistance = 0.0;
            float totalDeviation = 0.0;

            float totalDeviationVertical = 0.0;
            float totalDeviationHorizontal = 0.0;

            if (trajectory.Count() > 1) {
                vector initialPosition = trajectory[0];
                vector initialDirection = trajectory[1] - trajectory[0];
                float initialDistance = vector.Distance(trajectory[1], trajectory[0]);

                vector lastExpectedPosition = initialPosition;
                vector lastActualPosition = initialPosition;

                float previousDeviation = 0.0;
                float previousSpeed = 0.0;

                for (int i = 1; i < trajectory.Count(); i++) {
                    vector currentPosition = trajectory[i];
                    vector previousPosition = trajectory[i - 1];

                    float distance = vector.Distance(currentPosition, previousPosition);
                    totalDistance += distance;

                    vector expectedPosition = initialPosition + (initialDirection * (vector.Distance(trajectory[i], initialPosition) / initialDistance));

                    // Calculate the deviation between expected and actual positions
                    vector deviation = currentPosition - expectedPosition;
                    float actualDeviation = vector.Distance(currentPosition, expectedPosition);
                    totalDeviation += actualDeviation;

                    float currentSpeed = totalDistance / this.GetTimeSinceShot();

                    /*
                    // Doesnt really make sense
                    if(trajectory.Count() >= 10 && i > (trajectory.Count()/2) && previousSpeed != 0.0) {
                        if(currentSpeed > previousSpeed && (currentSpeed/previousSpeed) >= 1.03) {
                            GetGameLabs().GetLogger().Info(string.Format("currentSpeed > previousSpeed && (currentSpeed/previousSpeed) >= 1.03 [%1 > %2 && %3 >= 1.03] -> SendInvalidBulletNotification", currentSpeed, previousSpeed, (currentSpeed/previousSpeed)));
                            this.SendInvalidBulletNotification();
                            return;
                        }
                    }
                    */

                    previousSpeed = currentSpeed;
                    previousDeviation = actualDeviation;

                    // Update total deviations for vertical and horizontal axes
                    totalDeviationVertical += Math.AbsFloat(deviation[2]);
                    totalDeviationHorizontal += Math.AbsFloat(deviation[0]);

                    // Update initial values for the next iteration
                    initialPosition = lastExpectedPosition;
                    initialDirection = currentPosition - lastActualPosition;
                    initialDistance = vector.Distance(currentPosition, lastActualPosition);

                    lastExpectedPosition = expectedPosition;
                    lastActualPosition = currentPosition;
                }
            }

            float deviationPercentage = (totalDeviation / totalDistance) * 100.0;
            float speed = totalDistance / this.GetTimeSinceShot();

            if(deviationPercentage >= 10.0) {
                GetGameLabs().GetLogger().Info(string.Format("deviationPercentage >= 10.0 [deviationPercentage >= %1] -> SendInvalidBulletNotification", deviationPercentage));
                this.SendInvalidBulletNotification();
                return;
            }

            #ifdef GAMELABSBPDEBUG
            GetGameLabs().GetLogger().Info(string.Format("Total Deviation - Distance: %1, Horizontal: %2, Vertical: %3, Bullet Speed: %4, Deviation: %5(%6) | %7 | %8", totalDistance, totalDeviationHorizontal, totalDeviationVertical, speed, totalDeviation, deviationPercentage, trajectory.Count(), this.GetTimeSinceShot()));
            #endif
    }

    EntityAI GetClosestEntity(ref array<EntityAI> entities, vector bulletPosition) {
        int n = entities.Count();

        for (int i = 0; i < n - 1; ++i) {
            int minIndex = i;
            float minDistance = vector.Distance(entities[i].GetPosition(), bulletPosition);

            // Find the minimum distance entity from entities[i+1] to entities[n-1]
            for (int j = i + 1; j < n; ++j) {
                float distance = vector.Distance(entities[j].GetPosition(), bulletPosition);
                if (distance < minDistance) {
                    minDistance = distance;
                    minIndex = j;
                }
            }

            // Swap the found minimum distance entity with entities[i]
            EntityAI temp = entities[minIndex];
            entities[minIndex] = entities[i];
            entities[i] = temp;
        }
        return entities[0];
    }

    private void EvaluateBullets() {
        int bulletNullRefs = 0;
        foreach(Object bullet : this.bullets) {
            if(bullet != NULL) {
                vector position = bullet.GetPosition();

                ref array<vector> trajectory = this.bulletTracing.Get(bullet);
                if(trajectory == NULL) {
                    trajectory = new ref array<vector>;
                    this.bulletTracing.Set(bullet, trajectory);
                    trajectory.Insert(position);
                } else {
                    vector previousPosition = trajectory[trajectory.Count() - 1];
                    if(previousPosition != position) {
                        trajectory.Insert(position);
                        this.CalculateTrajectory(bullet, trajectory);
                        //GetGameLabs().GetLogger().Info(string.Format("bullet=%1; position=%1; trajectory=%2;", bullet, position, trajectory));
                    }
                }
                #ifdef GAMELABSBPDEBUG
                GetGameLabs().GetLogger().Info(string.Format("bullet=%1; position=%2; trajectory=%3; direction=%4", bullet, position, trajectory, bullet.GetDirection()));
                #endif

                #ifdef GAMELABSBPMAGIC
                if(!appliedMagic) {
                    array<Object> nearestObjects = new array<Object>;
                    ref array<EntityAI> entities = new array<EntityAI>;
                    GetGame().GetObjectsAtPosition3D(position, 9.0, nearestObjects, null);

                    foreach (Object nearestObject : nearestObjects) {
                        if (!appliedMagic) {
                            EntityAI nentity = EntityAI.Cast(nearestObject);
                            if (nentity) {
                                if (nentity.IsDayZCreature() && nentity.IsAlive()) {
                                    entities.Insert(nentity);
                                }
                            }
                        }
                    }

                    if(entities.Count()) {
                        EntityAI entity = GetClosestEntity(entities, bullet.GetPosition());
                        vector bulletPosition = bullet.GetPosition();
                        vector entityPos = entity.GetPosition();

                        // Calculate direction from bullet to entity
                        vector direction = entityPos - bulletPosition;
                        direction.Normalize();

                        // Calculate the position 0.1 units before the entity along the direction
                        vector newPosition = entityPos - (direction * 0.01);
                        newPosition[1] = newPosition[1] + 1.1;

                        // Set the direction of the bullet towards the entity
                        vector bulletDirection = direction;
                        bulletDirection[1] = 0.0; // Keep pitch level (optional)
                        bullet.SetDirection(bulletDirection);

                        // Set the bullet position just before the entity
                        bullet.SetPosition(newPosition);

                        appliedMagic = true;
                        GetGame().CreateObject("Apple", newPosition, true);
                        GetGameLabs().GetLogger().Info(string.Format("bullet=%1; MAGIC", bullet));
                    }
                }
                #endif
            } else {
                bulletNullRefs += 1;
            }
        }
        if(bulletNullRefs == this.bullets.Count()) {
            this.isActive = false;
        }
    }

    void TraceBullets() {
        #ifdef GAMELABSBPDEBUG
        GetGameLabs().GetLogger().Info("#########################################################################################");
        GetGameLabs().GetLogger().Info(string.Format("TraceBullets %1 - Started", this.bullets));
        #endif
        while(this.isActive) {
            EvaluateBullets();
            Sleep(0.0000000001); // Sleep for as short as possible to catch all increments to position
        }
        #ifdef GAMELABSBPDEBUG
        GetGameLabs().GetLogger().Info(string.Format("TraceBullets %1 - Exited after %2", this.bullets, this.GetTimeSinceShot()));
        #endif
    }
};


modded class Weapon_Base {
    override void EEFired(int muzzleType, int mode, string ammoType) {
        super.EEFired(muzzleType, mode, ammoType);

        if(!GetGameLabs().IsServer()) {
            if(GetGameLabs().GetConfiguration().GetMagicBulletCheckEnabled()) {
                array<Object> nearestObjects = new array<Object>;
                GetGame().GetObjectsAtPosition3D(this.GetPosition(), 1, nearestObjects, null);

                ref array<Object> bullets = new array<Object>;

                string objectType;
                foreach (Object nearestObject : nearestObjects) {
                    objectType = nearestObject.GetType();
                    if(objectType.IndexOf("Bullet") != -1) {
                        bullets.Insert(nearestObject);
                    }
                }

                if(bullets.Count() == 0) {
                    // Handle no Bullets found
                } else {
                    GameLabsBulletPostprocessor bulletHandler = new GameLabsBulletPostprocessor(bullets);
                    GetGame().GameScript.Call(bulletHandler, "TraceBullets", NULL);
                }
            }
        } else {
            PlayerBase player = PlayerBase.Cast(GetHierarchyRootPlayer());
            if(player) {
                if(player.HasUpstreamIdentity()) {
                    string cftoolsId = player.GetUpstreamIdentity();
                    ref GLPlayerStatistics playerStatistics = GetGameLabs().GetPlayerStatisticsByCFToolsId(cftoolsId);
                    if(ammoType == "Bullet_12GaugePellets") {
                        playerStatistics.shotsFired += 8; // One "shot" per pellet. Without killing performance, there is no better way. Sorry shotgun users.
                    } else {
                        playerStatistics.shotsFired++;
                    }
                }
            }
        }
    }
};