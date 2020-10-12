//
// AI decision processing

namespace ActorAI {
    
    // World ray cast callback
    class MyCallbackClass : public Physics::RaycastCallback {
        
        public: 
        
        virtual Physics::decimal notifyRaycastHit(const Physics::RaycastInfo& info) { 
            
            Physics::Vector3 worldPoint  = info.worldPoint;
            Physics::Vector3 worldNormal = info.worldNormal;
            
            // Display the world hit point coordinates 
            //std::cout << "Hit: " << worldPoint.x << "  " << worldPoint.y << "  " << worldPoint.z << std::endl;
            
            
            /// Fraction distance of the hit point between point1 and point2 of the ray
            /// The hit point "p" is such that p = point1 + hitFraction * (point2 - point1)
            float hitFraction = info.hitFraction;
            
            /// Mesh subpart index that has been hit (only used for triangles mesh and -1 otherwise)
            int meshSubpart = info.meshSubpart;
            
            /// Hit triangle index (only used for triangles mesh and -1 otherwise)
            int triangleIndex = info.triangleIndex;
            
            /// Pointer to the hit collision body
            Physics::CollisionBody* body = info.body;
            
            /// Pointer to the hit collider
            Physics::Collider* collider = info.collider;
            
            
            // Return a fraction of 1.0 to gather all hits 
            return Physics::decimal(1.0); 
        }
        
    };
    
    
    namespace RandomGeneration {
        
        // Generate random number between given range
        int Random(int Min, int Max) {return rand() % Max + Min;}
        
    }
    
    // Path-finding and scene query
    namespace PathFinding {
        
        
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
        
        
        // Returns true if the animation state is already set
        bool SetAnimationState(Actor* ActorPtr, int AnimState) {
            
            // Check current state
            if (ActorPtr ->AnimationCurrent != AnimState) {
                
                // Reset the animation cycle
                AnimationCycles::ResetCycle(ActorPtr);
                ActorPtr ->AnimationCurrent = AnimState;
                
                return false;
            }
            
            return true;
        }
        
    }
    
    
    //
    // The decision making process
    
    void DecisionPipeline(Actor* ActorPtr) {
        
        // Get actor transforms
        POSITION Position = ActorPtr ->Position;
        ROTATION Rotation = ActorPtr ->Rotation;
        
        // Low level states
        bool IsCurrentlyTravling = false;
        
        // Generate a random position to walk toward
        if (ActorPtr ->ActionList.size() == 0) { if (RandomGeneration::Random(0, 100) == 1) {
            
            float x = (StringToFloat( IntToString( RandomGeneration::Random(0, 10000) ) ) * 0.0025);
            float y = (StringToFloat( IntToString( RandomGeneration::Random(0, 10000) ) ) * 0.0025);
            
            std::string ActionString = "walk " + FloatToString(x) + " " + FloatToString(y);
            
            ActorPtr ->AddAction( ActionString );
            
        }}
        
        //
        // Iterate through the action list
        if (ActorPtr ->ActionList.size() < 1) return;
        
        for (std::vector<std::string>::iterator it = ActorPtr ->ActionList.begin(); it != ActorPtr ->ActionList.end(); ++it) {
            
            // Minimum distance threshold
            float RayHeight   = 1.7f;
            float MinDistance = 5.0f;
            
            std::string ActionString = *it;
            
            // Explode the string
            int i=0; std::string Strings[50];
            std::vector<std::string> Array = StringExplode(ActionString, ' ');
            for (std::vector<std::string>::iterator it = Array.begin(); it != Array.end(); ++it) {std::string String = *it; Strings[i] = String; i++;}
            
            // Walk toward point
            if (Strings[0] == "walk") {
                
                // Check current state
                if (IsCurrentlyTravling == true) continue;
                
                IsCurrentlyTravling = true;
                
                // Check current state
                AnimationCycles::SetAnimationState(ActorPtr, ANIMATION_WALK);
                
                // Destination point
                POSITION Destination = POSITION(StringToFloat(Strings[1]), StringToFloat(Strings[2]), StringToFloat(Strings[3]));
                
                // Check if we have arrived at our destination
                if (glm::distance( Position, Destination ) < MinDistance) {
                    
                    // Reset the animation cycle
                    AnimationCycles::SetAnimationState(ActorPtr, ANIMATION_IDLE);
                    
                    // Zero the speed
                    ActorPtr ->AttachedBody ->setLinearVelocity( Physics::Vector3(0.0,0.0,0.0));
                    
                    ActorPtr ->ActionList.erase(it);
                    break;
                }
                
                // Point toward which we are moving
                float Angle = AngleBetweenPoints(Position, Destination);
                ActorPtr ->Rotation.x = Angle;
                
                // Get speed and orientation
                float Speed      = ActorPtr ->SpeedWalk;
                float Direction  = Angle + ActorPtr ->DirectionOffset + 29.9f;
                
                // Calculate the forward angle
                ActorPtr ->Forward.x =  cos( Direction ) * Speed;
                ActorPtr ->Forward.y = -sin( Direction ) * Speed;
                
                // Apply force to the actor body
                //ActorPtr ->AttachedBody ->applyForceToCenterOfMass( ActorPtr ->Forward );
                ActorPtr ->AttachedBody ->setLinearVelocity( ActorPtr ->Forward );
                
                
                //
                // Obstacle avoidance
                
                // Find start and end points for the ray
                Physics::Vector3 Start = Physics::Vector3(Position.x, Position.y, RayHeight);
                Physics::Vector3 End   = Physics::Vector3(Destination.x, Destination.y, RayHeight);
                
                // Create the ray
                Physics::Ray ray(Start, End);
                
                // Test casting against a rigid body`s collider
                MyCallbackClass CallbackObject;
                PhysicsWorld ->raycast(ray, &CallbackObject);
                
            }
            
            // Run toward point
            if (Strings[0] == "run") {
                
                // Check current state
                if (IsCurrentlyTravling == true) continue;
                
                IsCurrentlyTravling = true;
                
                // Check current state
                AnimationCycles::SetAnimationState(ActorPtr, ANIMATION_RUN);
                
                // Destination point
                POSITION Destination = POSITION(StringToFloat(Strings[1]), StringToFloat(Strings[2]), StringToFloat(Strings[3]));
                
                // Check if we have arrived at our destination
                if (glm::distance( Position, Destination ) < MinDistance) {
                    
                    // Reset the animation cycle
                    AnimationCycles::SetAnimationState(ActorPtr, ANIMATION_IDLE);
                    
                    // Zero the speed
                    ActorPtr ->AttachedBody ->setLinearVelocity( Physics::Vector3(0.0,0.0,0.0));
                    
                    ActorPtr ->ActionList.erase(it);
                    break;
                }
                
                // Point toward which we are moving
                float Angle = AngleBetweenPoints(Position, Destination);
                ActorPtr ->Rotation.x = Angle;
                
                // Get speed and orientation
                float Speed      = ActorPtr ->SpeedRun;
                float Direction  = Angle + ActorPtr ->DirectionOffset + 29.9f;
                
                // Calculate the forward angle
                ActorPtr ->Forward.x =  cos( Direction ) * Speed;
                ActorPtr ->Forward.y = -sin( Direction ) * Speed;
                
                // Apply force to the actor body
                //ActorPtr ->AttachedBody ->applyForceToCenterOfMass( ActorPtr ->Forward );
                ActorPtr ->AttachedBody ->setLinearVelocity( ActorPtr ->Forward );
                
                
                //
                // Obstacle avoidance
                
                // Find start and end points for the ray
                Physics::Vector3 Start = Physics::Vector3(Position.x, Position.y, Position.z);
                Physics::Vector3 End   = Physics::Vector3(Destination.x, Destination.y, Destination.z);
                
                // Create the ray
                Physics::Ray ray(Start, End);
                
                // Test casting against a rigid body`s collider
                MyCallbackClass CallbackObject;
                PhysicsWorld ->raycast(ray, &CallbackObject);
                
            }
            
        }
        
    }
    
    
}

