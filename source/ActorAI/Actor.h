//
// Actor component

struct Actor {
    
    std::string Name;
    std::string OriginalName;
    
    // States
    bool IsActive;
    bool DoAnimationStates;
    
    Physics::RigidBody* AttachedBody;
    
    std::string ColliderName;
    
    // Attachments
    float LimbSwingRangeWalk;
    float LimbSwingRateWalk;
    float LimbSwingRangeRun;
    float LimbSwingRateRun;
    
    bool LimbSwingDirection;
    
    std::string HeadName;
    std::string TorsoName;
    std::string ArmRName;
    std::string ArmLName;
    std::string LegRName;
    std::string LegLName;
    
    int AnimationCurrent;
    int AnimationIdle;
    int AnimationWalk;
    int AnimationRun;
    int AnimationDeath;
    
    glm::vec3 HeadOffset;
    glm::vec3 TorsoOffset;
    glm::vec3 ArmROffset;
    glm::vec3 ArmLOffset;
    glm::vec3 LegROffset;
    glm::vec3 LegLOffset;
    
    Entity* HeadEntity;
    Entity* TorsoEntity;
    Entity* ArmREntity;
    Entity* ArmLEntity;
    Entity* LegREntity;
    Entity* LegLEntity;
    
    POSITION Position;
    ROTATION Rotation;
    Physics::Vector3 Forward;
    
    float DirectionOffset;
    
    // AI state
    int State;
    
    // Decision task list
    std::vector<std::string> ActionList;
    
    Actor();
    ~Actor();
    
    // Update this actor
    void Update(void);
    
    // Decision processing
    void AddAction(std::string);
    
};

Actor :: Actor() {
    
    Name = "";
    
    IsActive = false;
    DoAnimationStates = false;
    
    AttachedBody = nullptr;
    
    ColliderName = "";
    
    LimbSwingRangeWalk = 0.0;
    LimbSwingRateWalk  = 0.0;
    LimbSwingRangeRun  = 0.0;
    LimbSwingRateRun   = 0.0;
    
    LimbSwingDirection = false;
    
    HeadName = "";
    TorsoName = "";
    ArmRName = "";
    ArmLName = "";
    LegRName = "";
    LegLName = "";
    
    AnimationCurrent = ANIMATION_NONE;
    AnimationIdle = ANIMATION_NONE;
    AnimationWalk = ANIMATION_NONE;
    AnimationRun = ANIMATION_NONE;
    AnimationDeath = ANIMATION_NONE;
    
    HeadOffset  = glm::vec3(0.0, 0.0, 0.0);
    TorsoOffset = glm::vec3(0.0, 0.0, 0.0);
    ArmROffset  = glm::vec3(0.0, 0.0, 0.0);
    ArmLOffset  = glm::vec3(0.0, 0.0, 0.0);
    LegROffset  = glm::vec3(0.0, 0.0, 0.0);
    LegLOffset  = glm::vec3(0.0, 0.0, 0.0);
    
    HeadEntity = nullptr;
    TorsoEntity = nullptr;
    ArmREntity = nullptr;
    ArmLEntity = nullptr;
    LegREntity = nullptr;
    LegLEntity = nullptr;
    
    Position = POSITION(0.0, 0.0, 0.0);
    Rotation = ROTATION(0.0, 0.0, 0.0);
    Forward  = Physics::Vector3(0.0, 0.0, 0.0);
    
    DirectionOffset = 0.0;
    
    // Idle state
    State = STATE_IDLE;
    
    ActionList.clear();
    
}
Actor :: ~Actor() {
    
    // Destroy the rigid body
    if (AttachedBody != nullptr) {
        
        PhysicsWorld ->destroyRigidBody( AttachedBody );
        AttachedBody = nullptr;
        
    }
    
}

void Actor :: Update(void) {
    
    // Update our position to the rigid body
    if (AttachedBody != nullptr) {
        
        // Get physical body transform
        Physics::Transform  EntityTransform  = AttachedBody ->getTransform();
        Physics::Quaternion EntityQuaternion = EntityTransform.getOrientation();
        
        // Sync the actor to the physical body`s position
        Physics::Vector3 EntityPosition = EntityTransform.getPosition();
        this ->Position = glm::vec3(EntityPosition.x, EntityPosition.y, EntityPosition.z);
        
        //Physics::Vector3 EntityRotation = EntityQuaternion. ();
        //this ->Rotation = glm::vec3(EntityRotation.x, EntityRotation.y, EntityRotation.z);
        
    }
    
    // Update entity translation
    if (this ->HeadEntity != nullptr) {
        this ->HeadEntity ->Position = this ->Position + this->HeadOffset;
        this ->HeadEntity ->Rotation = this ->Rotation;
    }
    if (this ->TorsoEntity != nullptr) {
        this ->TorsoEntity ->Position = this ->Position + this->TorsoOffset;
        this ->TorsoEntity ->Rotation = this ->Rotation;
    }
    if (this ->ArmREntity != nullptr) {
        this ->ArmREntity ->Position = this ->Position + this->ArmROffset;
        this ->ArmREntity ->Rotation = this ->Rotation;
    }
    if (this ->ArmLEntity != nullptr) {
        this ->ArmLEntity ->Position = this ->Position + this->ArmLOffset;
        this ->ArmLEntity ->Rotation = this ->Rotation;
    }
    if (this ->LegREntity != nullptr) {
        this ->LegREntity  ->Position = this ->Position + this->LegROffset;
        this ->LegREntity  ->Rotation = this ->Rotation;
    }
    if (this ->LegLEntity != nullptr) {
        this ->LegLEntity  ->Position = this ->Position + this->LegLOffset;
        this ->LegLEntity  ->Rotation = this ->Rotation;
    }
    
}

void Actor :: AddAction(std::string ActionString) {
    
    ActionList.push_back( ActionString );
    
}

