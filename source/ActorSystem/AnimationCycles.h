//
// Animation cycles

namespace ActorAI {
    
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
    
}

