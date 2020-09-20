//
// Actor based AI system

#define ANIMATION_NONE   0
#define ANIMATION_IDLE   1
#define ANIMATION_WALK   2
#define ANIMATION_RUN    3
#define ANIMATION_DEATH  4

#define  STATE_SLEEP   0
#define  STATE_IDLE    1
#define  STATE_SEARCH  2
#define  STATE_ATTACK  3
#define  STATE_FLEE    4

#define  DECISION_NO_DECISION     0
#define  DECISION_WALK_TO_POINT   1
#define  DECISION_RUN_TO_POINT    2

#include "ActorAI/Actor.h"
#include "ActorAI/CoreAI.h"

namespace ActorAI {
    
    // AI controller
    class ActorSystem {
        
        std::vector<Actor*> ActorList;
        
        public:
        
        ActorSystem();
        ~ActorSystem();
        
        // Actor management
        Actor*  CreateActor  (double, double, double);
        bool    DestroyActor (Actor* ActorPtr);
        Actor*  FindActor    (std::string AssetName);
        
        // Animation
        void AnimationStep(Actor*);
        
        // Update actors
        void UpdateActors(void);
        
    };
    
    ActorSystem* AI = nullptr;
    bool InitiateAI(void) {if (AI == nullptr) {AI = new ActorSystem(); return true;}return false;}
    bool ShutdownAI(void) {if (AI != nullptr) {delete AI; AI = nullptr; return true;}return false;}
    
    ActorSystem :: ActorSystem() {
        
        ActorList.clear();
        
    }
    ActorSystem :: ~ActorSystem() {
        
    }
    
    // Actor objects
    Actor* ActorSystem :: CreateActor(double x=0.0, double y=0.0, double z=0.0) {
        
        Actor* ActorPtr = new Actor();
        ActorPtr ->Position = glm::vec3(x, y, z);
        
        ActorList.push_back(ActorPtr);
        
        return ActorPtr;
    }
    bool   ActorSystem :: DestroyActor(Actor* ActorPtr) {
        
        for (std::vector<Actor*>::iterator it = ActorList.begin(); it != ActorList.end(); ++it) {
            Actor* ActorListPtr = *it;
            
            if (ActorPtr == ActorListPtr) {
                
                ActorList.erase(it);
                
                delete ActorPtr;
                
                return true;
            }
        }
        
        return false;
    }
    Actor* ActorSystem :: FindActor(std::string AssetName) {
        
        for (std::vector<Actor*>::iterator it = ActorList.begin(); it != ActorList.end(); ++it) {
            Actor* ActorPtr = *it;
            
            if (ActorPtr ->Name == AssetName) {
                
                return ActorPtr;
                
            }
            
        }
        
        return nullptr;
    }
    
    // Animation
    void ActorSystem :: AnimationStep(Actor* ActorPtr) {    
        
        //
        // Animation cycles
        
        if (ActorPtr ->AnimationCurrent == ANIMATION_IDLE) {AnimationCycles::IdleCycle(ActorPtr);}
        if (ActorPtr ->AnimationCurrent == ANIMATION_WALK) {AnimationCycles::WalkCycle(ActorPtr);}
        if (ActorPtr ->AnimationCurrent == ANIMATION_RUN) {AnimationCycles::RunCycle(ActorPtr);}
        if (ActorPtr ->AnimationCurrent == ANIMATION_DEATH) {AnimationCycles::DeathCycle(ActorPtr);}
        
    }
    
    // Update actors
    void ActorSystem :: UpdateActors(void) {
        
        for (std::vector<Actor*>::iterator it = ActorList.begin(); it != ActorList.end(); ++it) {
            Actor* ActorPtr = *it;
            
            if (ActorPtr ->IsActive) {
                
                // Decision making pipeline
                ActorAI::DecisionPipeline( ActorPtr );
                
                // Cycle the animations
                this ->AnimationStep(ActorPtr);
                
                // Call update on this actor
                ActorPtr ->Update();
                
            }
            
        }
        
    }
    
}


