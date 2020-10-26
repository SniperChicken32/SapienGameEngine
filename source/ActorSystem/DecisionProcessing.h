//
// AI decision processing

namespace ActorAI {
    
    // Path-finding and scene query
    namespace PathFinding {
        
        
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
        if (ActorPtr ->ActionList.size() == 0) { if (Random(0, 100) == 1) {
            
            float x = (StringToFloat( IntToString( Random(0, 10000) ) ) * 0.0025);
            float y = (StringToFloat( IntToString( Random(0, 10000) ) ) * 0.0025);
            
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
                //MyCallbackClass CallbackObject;
                //PhysicsWorld ->raycast(ray, &CallbackObject);
                
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
                //MyCallbackClass CallbackObject;
                //PhysicsWorld ->raycast(ray, &CallbackObject);
                
            }
            
        }
        
    }
    
    
}

