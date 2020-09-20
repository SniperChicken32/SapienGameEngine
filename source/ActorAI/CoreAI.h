//
// AI decision processing

namespace ActorAI {
    
    namespace RandomGeneration {
        
        // Generate random number between given range
        int Random(int Min, int Max) {return rand() % Max + Min;}
        
    }
    
    // Path-finding and scene query
    namespace PathFinding {
        
        // Ray cast between two points
        bool RayCastBetweenPoints(Physics::Vector3 StartPoint, Physics::Vector3 EndPoint, Physics::Collider* ColliderPtr) {
        
        // Create the ray 
        Physics::Ray ray(StartPoint, EndPoint);
        
        // Create the ray cast info object for the results
        Physics::RaycastInfo raycastInfo;
        
        // Test casting against a rigid body`s collider
        if (ColliderPtr ->raycast(ray, raycastInfo)) return true;
        
        return false;
    }
        
    }
    
    // Animation cycles
    namespace AnimationCycles {
        
        // Reset the cycle state to default
        void ResetCycle(Actor* ActorPtr) {
            
            ActorPtr ->LimbSwingDirection = false;
            
            if (ActorPtr ->HeadEntity != nullptr) ActorPtr ->HeadEntity ->RotationOffset = glm::vec3(0.0,0.0,0.0);
            if (ActorPtr ->TorsoEntity != nullptr) ActorPtr ->TorsoEntity ->RotationOffset = glm::vec3(0.0,0.0,0.0);
            if (ActorPtr ->ArmREntity != nullptr) ActorPtr ->ArmREntity ->RotationOffset = glm::vec3(0.0,0.0,0.0);
            if (ActorPtr ->ArmLEntity != nullptr) ActorPtr ->ArmLEntity ->RotationOffset = glm::vec3(0.0,0.0,0.0);
            if (ActorPtr ->LegREntity != nullptr) ActorPtr ->LegREntity ->RotationOffset = glm::vec3(0.0,0.0,0.0);
            if (ActorPtr ->LegLEntity != nullptr) ActorPtr ->LegLEntity ->RotationOffset = glm::vec3(0.0,0.0,0.0);
            
        }
        
        void IdleCycle(Actor* ActorPtr) {
            
        }
        
        void WalkCycle(Actor* ActorPtr) {
            
            // Get some values
            float LimbSwingRange = ActorPtr ->LimbSwingRangeWalk;
            float LimbSwingRate  = ActorPtr ->LimbSwingRateWalk;
            
            if (ActorPtr ->LimbSwingDirection) {
                
                // Check swing reversal
                if (ActorPtr ->ArmREntity ->RotationOffset.y > LimbSwingRange) {ActorPtr ->LimbSwingDirection = false;}
                
                // Swing forward
                if (ActorPtr ->ArmREntity != nullptr) ActorPtr ->ArmREntity ->RotationOffset.y += LimbSwingRate;
                if (ActorPtr ->ArmLEntity != nullptr) ActorPtr ->ArmLEntity ->RotationOffset.y -= LimbSwingRate;
                if (ActorPtr ->LegREntity != nullptr) ActorPtr ->LegREntity ->RotationOffset.y += LimbSwingRate;
                if (ActorPtr ->LegLEntity != nullptr) ActorPtr ->LegLEntity ->RotationOffset.y -= LimbSwingRate;
                
            } else {
                
                // Check swing reversal
                if (ActorPtr ->ArmREntity ->RotationOffset.y < -LimbSwingRange) {ActorPtr ->LimbSwingDirection = true;}
                
                // Swing backward
                if (ActorPtr ->ArmREntity != nullptr) ActorPtr ->ArmREntity ->RotationOffset.y -= LimbSwingRate;
                if (ActorPtr ->ArmLEntity != nullptr) ActorPtr ->ArmLEntity ->RotationOffset.y += LimbSwingRate;
                if (ActorPtr ->LegREntity != nullptr) ActorPtr ->LegREntity ->RotationOffset.y -= LimbSwingRate;
                if (ActorPtr ->LegLEntity != nullptr) ActorPtr ->LegLEntity ->RotationOffset.y += LimbSwingRate;
                
            }
            
        }
        
        void RunCycle(Actor* ActorPtr) {
            
            // Get some values
            float LimbSwingRange = ActorPtr ->LimbSwingRangeRun;
            float LimbSwingRate  = ActorPtr ->LimbSwingRateRun;
            
            if (ActorPtr ->LimbSwingDirection) {
                
                // Check swing reversal
                if (ActorPtr ->ArmREntity ->RotationOffset.y > LimbSwingRange) {ActorPtr ->LimbSwingDirection = false;}
                
                // Swing forward
                if (ActorPtr ->ArmREntity != nullptr) ActorPtr ->ArmREntity ->RotationOffset.y += LimbSwingRate;
                if (ActorPtr ->ArmLEntity != nullptr) ActorPtr ->ArmLEntity ->RotationOffset.y -= LimbSwingRate;
                if (ActorPtr ->LegREntity != nullptr) ActorPtr ->LegREntity ->RotationOffset.y += LimbSwingRate;
                if (ActorPtr ->LegLEntity != nullptr) ActorPtr ->LegLEntity ->RotationOffset.y -= LimbSwingRate;
                
            } else {
                
                // Check swing reversal
                if (ActorPtr ->ArmREntity ->RotationOffset.y < -LimbSwingRange) {ActorPtr ->LimbSwingDirection = true;}
                
                // Swing backward
                if (ActorPtr ->ArmREntity != nullptr) ActorPtr ->ArmREntity ->RotationOffset.y -= LimbSwingRate;
                if (ActorPtr ->ArmLEntity != nullptr) ActorPtr ->ArmLEntity ->RotationOffset.y += LimbSwingRate;
                if (ActorPtr ->LegREntity != nullptr) ActorPtr ->LegREntity ->RotationOffset.y -= LimbSwingRate;
                if (ActorPtr ->LegLEntity != nullptr) ActorPtr ->LegLEntity ->RotationOffset.y += LimbSwingRate;
                
            }
            
        }
        
        void DeathCycle(Actor* ActorPtr) {
            
        }
        
    }
    
    
    
    //
    // The decision making process starts here
    
    void DecisionPipeline(Actor* ActorPtr) {
        
        Physics::Vector3 Force;
        
        // Get actor transforms
        POSITION Position = ActorPtr ->Position;
        ROTATION Rotation = ActorPtr ->Rotation;
        
        //ActorPtr ->AnimationCurrent = ANIMATION_WALK;
        
        
        // Actor states
        bool IsCurrentlyTravling = false;
        
        //
        // Iterate through the decision list
        if (ActorPtr ->ActionList.size() < 1) return;
        
        for (std::vector<std::string>::iterator it = ActorPtr ->ActionList.begin(); it <= ActorPtr ->ActionList.end(); ++it) {
            
            std::string ActionString = *it;
            
            // Explode the string
            int i=0; std::string Strings[50];
            std::vector<std::string> Array = StringExplode(ActionString, ' ');
            for (std::vector<std::string>::iterator it = Array.begin(); it != Array.end(); ++it) {std::string String = *it; Strings[i] = String; i++;}
            
            
            // Walk toward point
            if (Strings[0] == "walk") {
                
                // Check current state
                if (IsCurrentlyTravling == false) {IsCurrentlyTravling = true;
                    
                    // Check current state
                    if (ActorPtr ->AnimationCurrent != ANIMATION_WALK) {
                        
                        AnimationCycles::ResetCycle(ActorPtr);
                        
                        ActorPtr ->AnimationCurrent = ANIMATION_WALK;
                        
                    }
                    
                    // Destination point
                    //POSITION Destination = POSITION(StringToFloat(Strings[1]), StringToFloat(Strings[2]), StringToFloat(Strings[3]));
                    
                    
                    
                    
                }
                
            }
            
            // Run toward point
            if (Strings[0] == "run") {
                
                // Check current state
                if (IsCurrentlyTravling == false) {IsCurrentlyTravling = true;
                    
                    // Check current state
                    if (ActorPtr ->AnimationCurrent != ANIMATION_WALK) {
                        
                        AnimationCycles::ResetCycle(ActorPtr);
                        
                        ActorPtr ->AnimationCurrent = ANIMATION_WALK;
                        
                    }
                    
                    // Destination point
                    //POSITION Destination = POSITION(StringToFloat(Strings[1]), StringToFloat(Strings[2]), StringToFloat(Strings[3]));
                    
                    
                    
                    
                }
                
            }
            
            /*
            
            // Check travel state
            if (IsCurrentlyWalking == false) {IsCurrentlyWalking = true;
                
                
                
                
                // Point toward which we are moving
                POSITION PointPosition = POSITION(0.0, 0.0, 0.0);
                
                // Get direction toward point
                float PointDirection = AngleBetweenPoints(ActorPtr ->Position, PointPosition);
                
                
                // Walk towards point
                if (DecisionType = "") {
                    
                    // Check current state
                    if (ActorPtr ->AnimationCurrent != ANIMATION_WALK) {
                        
                        AnimationCycles::ResetCycle(ActorPtr);
                        
                        ActorPtr ->AnimationCurrent = ANIMATION_WALK;
                        
                    }
                    
                }
                
                // Run towards point
                if (DecisionType = "") {
                    
                    // Check current state
                    if (ActorPtr ->AnimationCurrent != ANIMATION_RUN) {
                        
                        AnimationCycles::ResetCycle(ActorPtr);
                        
                        ActorPtr ->AnimationCurrent = ANIMATION_RUN;
                        
                    }
                    
                }
                
                
                // Generate a random float
                //float RandomNumber = StringToFloat( IntToString( RandomGeneration::Random(1, 99) ) ) * 0.1f;
                
                // Apply the random number
                //ActorPtr ->Rotation.x += RandomNumber;
                //if (ActorPtr ->Rotation.x >= 360.0f) {ActorPtr ->Rotation.x -= 360.0f;}
                
                // Zero the speed
                //ActorPtr ->AttachedBody ->setLinearVelocity( Physics::Vector3(0.0,0.0,0.0));
                
                
                
            }
            
            
            */
            
            
            
            
        }
        
        
        
        /*
        // Direction
        if (RandomGeneration::Random(1, 250) == 1) {
            
            //AnimationCycles::ResetCycle(ActorPtr);
            //ActorPtr ->AnimationCurrent = ANIMATION_IDLE;
            
            // Generate a random float
            float RandomNumber = StringToFloat( IntToString( RandomGeneration::Random(1, 99) ) ) * 0.1f;
            
            // Apply the random number
            ActorPtr ->Rotation.x += RandomNumber;
            if (ActorPtr ->Rotation.x >= 360.0f) {ActorPtr ->Rotation.x -= 360.0f;}
            
            // Zero the speed
            ActorPtr ->AttachedBody ->setLinearVelocity( Physics::Vector3(0.0,0.0,0.0));
            
        }
        
        
        // Calculate the actors forward angle
        float DirectionOffset = ActorPtr ->DirectionOffset + 29.9f;
        ActorPtr ->Forward.x =  cos( Rotation.x + DirectionOffset ) * 3.5f;
        ActorPtr ->Forward.y = -sin( Rotation.x + DirectionOffset ) * 3.5f;
        //ActorPtr ->Forward.z =  tan( Rotation.y + DirectionOffset ) * 3.85f;
        
        
        
        // Apply force to the actor body
        //ActorPtr ->AttachedBody ->applyForceToCenterOfMass( Force );
        ActorPtr ->AttachedBody ->setLinearVelocity( ActorPtr ->Forward );
        
        
        
        
        if (RandomGeneration::Random(1, 200) == 1) {
            
            AnimationCycles::ResetCycle(ActorPtr);
            ActorPtr ->AnimationCurrent = ANIMATION_IDLE;
            
        }
        
        if (RandomGeneration::Random(1, 300) == 1) {
            
            AnimationCycles::ResetCycle(ActorPtr);
            ActorPtr ->AnimationCurrent = ANIMATION_RUN;
            
        }
        
        if (RandomGeneration::Random(1, 300) == 1) {
            
            AnimationCycles::ResetCycle(ActorPtr);
            ActorPtr ->AnimationCurrent = ANIMATION_WALK;
            
        }
        */
        
        
    }
    
    
}

