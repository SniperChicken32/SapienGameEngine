//
// AI decision processing

#define  STATE_SLEEP   0
#define  STATE_IDLE    1
#define  STATE_SEARCH  2
#define  STATE_ATTACK  3
#define  STATE_FLEE    4

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
    // The decision making process starts with this function
    
    void DecisionPipeline(Actor* ActorPtr) {
        
        Physics::Vector3 Force;
        
        // Get actor transforms
        POSITION Position = ActorPtr ->Position;
        ROTATION Rotation = ActorPtr ->Rotation;
        
        // Get the actors forward angle
        ROTATION Forward;
        Forward.x = cos( DegToRad( ActorPtr ->Rotation.x ) + 90.0 ) * 50.0;
        Forward.y = sin( DegToRad( ActorPtr ->Rotation.y ) - 0.0 ) * 50.0;
        Forward.z = tan( DegToRad( ActorPtr ->Rotation.z ) );
        
        if (RandomGeneration::Random(1, 250) == 1) {
            
            AnimationCycles::ResetCycle(ActorPtr);
            ActorPtr ->AnimationCurrent = ANIMATION_IDLE;
            
        }
        
        if (RandomGeneration::Random(1, 350) == 1) {
            
            AnimationCycles::ResetCycle(ActorPtr);
            ActorPtr ->AnimationCurrent = ANIMATION_RUN;
            
        }
        
        if (RandomGeneration::Random(1, 350) == 1) {
            
            AnimationCycles::ResetCycle(ActorPtr);
            ActorPtr ->AnimationCurrent = ANIMATION_WALK;
            
        }
        
        // Apply force to the actor body
        //Force = Physics::Vector3(Forward.x, Forward.z, Forward.y);
        //ActorPtr ->AttachedBody ->applyForceToCenterOfMass( Force );
        
        // Apply rotational torque to the actor body
        Physics::Vector3 Torque = Physics::Vector3( 80.0, 0.0, 0.0 );
        ActorPtr ->AttachedBody ->applyTorque( Torque );
        
    }
    
    
}

