//
// Entity object

struct Entity {
    
    std::string Name;
    std::string ColliderName;
    std::string FilePath;
    
    bool  IsActive;
    bool  IsRendered;
    bool  UseAutoTrack;
    bool  IsStatic;
    bool  IsOverlay;
    
    double RenderDistance;
    
    int RenderQueueGroup;
    
    POSITION  Position;
    POSITION  PositionOffset;
    ROTATION  Rotation;
    ROTATION  RotationOffset;
    SCALE     Scale;
    
    Mesh*               AttachedMesh;
    Physics::RigidBody* AttachedBody;
    
    std::vector<Light*>     LightList;
    
    glm::mat4 ModelMatrix;
    glm::mat4 NormalMatrix;
    
    Entity();
    ~Entity();
    
    // Translation
    void      SetPosition(double x, double y, double z)  {Position.x = x; Position.y = y; Position.z = z;}
    void      SetRotation(float y, float p, float r)     {Rotation.x = y; Rotation.y = p; Rotation.z = r;}
    void      SetScale(float x, float y, float z)        {Scale.x = x; Scale.y = y; Scale.z = z;}
    POSITION  GetPosition(void)  {return Position;}
    ROTATION  GetRotation(void)  {return Rotation;}
    SCALE     GetScale(void)     {return Scale;}
    
    // Settings
    void    SetRenderQueueGroup(int Group)     {RenderQueueGroup = Group;}
    void    SetRenderDistance(double Distance) {RenderDistance = Distance;}
    int     GetRenderQueueGroup(void)          {return RenderQueueGroup;}
    double  GetRenderDistance(void)            {return RenderDistance;}
    void    EnableRender(bool State)        {IsRendered   = State;}
    void    EnableAutoTracking(bool State)  {UseAutoTrack = State;}
    void    EnableOverlay(bool State)       {IsOverlay    = State;}
    
    // Attachment / detachment
    bool  AttachMesh(Mesh* MeshPtr) {
        if (MeshPtr == nullptr) {return false;}
        AttachedMesh = MeshPtr;
        return true;
    }
    void  DetachMesh(void) {
        AttachedMesh  = nullptr;
    }
    
    bool  AddLight(Light* LightPtr) {
        if (LightPtr == nullptr) {return false;}
        LightList.push_back(LightPtr);
        return true;
    }
    bool  RemoveLight(Light* LightPtr) {
        
        Light* LightListPtr;
        for (std::vector<Light*>::iterator it = LightList.begin(); it != LightList.end(); ++it) {
            LightListPtr = *it;
            
            if (LightListPtr == LightPtr) {
                
                LightList.erase(it);
                
                return true;
            }
            
        }
        return false;
    }
    
    bool  AttachBody(Physics::RigidBody* BodyPtr) {
        if (BodyPtr == nullptr) {return false;}
        AttachedBody = BodyPtr;
        return true;
    }
    void  DetachBody(void) {
        AttachedBody = nullptr;
    }
    
    // Update
    void  Update(void);
    void  Activate(void);
    void  Deactivate(void);
    
};

Entity :: Entity() {
    
    Name = "";
    ColliderName = "";
    
    // For saving
    FilePath = "";
    
    // Initial state
    IsActive       = true;
    IsRendered     = true;
    UseAutoTrack   = false;
    IsStatic       = false;
    IsOverlay      = false;
    
    // Render settings
    RenderDistance    = 300.0;
    RenderQueueGroup  = RENDER_QUEUE_GEOMETRY;
    
    // Object position, orientation and scale
    Position = glm::vec3(0.0, 0.0, 0.0);
    PositionOffset = glm::vec3(0.0, 0.0, 0.0);
    Rotation = glm::vec3(0.0, 0.0, 0.0);
    RotationOffset = glm::vec3(0.0, 0.0, 0.0);
    Scale    = glm::vec3(1.0, 1.0, 1.0);
    
    AttachedMesh   = nullptr;
    AttachedBody   = nullptr;
    
    LightList.clear();
    
    ModelMatrix  = glm::mat4(1.0f);
    NormalMatrix = glm::mat4(1.0f);
    
}
Entity :: ~Entity() {
    
    // Destroy the rigid body
    if (AttachedBody != nullptr) {
        
        PhysicsWorld ->destroyRigidBody( AttachedBody );
        AttachedBody = nullptr;
        
    }
    
}

// Update attachments
void Entity :: Update(void) {
    
    // Update our position to the rigid body
    if (AttachedBody != nullptr) {
        
        // Get physical body transform
        Physics::Transform  EntityTransform  = AttachedBody ->getTransform();
        Physics::Quaternion EntityQuaternion = EntityTransform.getOrientation();
        
        // Sync the entity to the physical body`s position
        Physics::Vector3 EntityPosition = EntityTransform.getPosition();
        this ->Position = glm::vec3(EntityPosition.x, EntityPosition.y, EntityPosition.z);
        
        Physics::Vector3 EntityRotation   = EntityQuaternion.getVectorV();
        this ->Rotation = glm::vec3(EntityRotation.x, EntityRotation.y, EntityRotation.z);
        
    }
    
    // Update attachments
    for (std::vector<Light*>::iterator it = LightList.begin(); it != LightList.end(); ++it) {
        Light* LightListPtr = *it;
        
        LightListPtr ->Position = Position;
        LightListPtr ->Rotation = Rotation;
        
    }
    
}

// Activate the entity
void Entity :: Activate(void) {
    
    Light* LightListPtr;
    for (std::vector<Light*>::iterator it = this ->LightList.begin(); it != this ->LightList.end(); ++it) {
        
        LightListPtr = *it;
        
        LightListPtr ->IsActive = true;
        
    }
    
}

// Deactivate the entity
void Entity :: Deactivate(void) {
    
    Light* LightListPtr;
    for (std::vector<Light*>::iterator it = this ->LightList.begin(); it != this ->LightList.end(); ++it) {
        
        LightListPtr = *it;
        
        LightListPtr ->IsActive = false;
        
    }
    
}

