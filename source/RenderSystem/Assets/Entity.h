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
    
    std::map<std::string, float> ComponentList;
    
    glm::mat4 ModelMatrix;
    glm::mat4 NormalMatrix;
    
    Entity();
    ~Entity();
    
    //
    // Translation
    
    /** Sets the position of this entity object.*/
    void      SetPosition(double x, double y, double z)  {Position.x = x; Position.y = y; Position.z = z;}
    /** Sets the orientation of this entity object.*/
    void      SetRotation(float y, float p, float r)     {Rotation.x = y; Rotation.y = p; Rotation.z = r;}
    /** Sets the scale of this entity object.*/
    void      SetScale(float x, float y, float z)        {Scale.x = x; Scale.y = y; Scale.z = z;}
    /** Returns the position of this entity object.*/
    POSITION  GetPosition(void)  {return Position;}
    /** Returns the orientation of this entity object.*/
    ROTATION  GetRotation(void)  {return Rotation;}
    /** Returns the position of this entity object.*/
    SCALE     GetScale(void)     {return Scale;}
    
    //
    // Components
    
    /** Returns whether a component with the given name exists.*/
    bool CheckComponent(std::string Name) {
        
        for (std::map<std::string, float>::iterator it = ComponentList.begin(); it != ComponentList.end(); ++it) {
            
            std::string listName  = it->first;
            //float       listValue = it->second;
            
            if (Name == listName) return true;
            
        }
        
        return false;
    }
    /** Adds a component of the given name with the given value.*/
    bool AddComponent(std::string Name, float Value=0.0) {
        
        // Check if the component already exists
        if (this ->CheckComponent(Name)) return false;
        
        ComponentList.emplace(Name, Value);
        return true;
    }
    /** Removes a component with the given name.*/
    bool RemoveComponent(std::string Name) {
        
        for (std::map<std::string, float>::iterator it = ComponentList.begin(); it != ComponentList.end(); ++it) {
            
            std::string listName  = it->first;
            
            if (Name == listName) {
                
                ComponentList.erase(it);
                
                return true;
            }
            
        }
        
        return false;
    }
    /** Sets a components value.*/
    bool SetComponentValue(std::string Name, float Value) {
        
        for (std::map<std::string, float>::iterator it = ComponentList.begin(); it != ComponentList.end(); ++it) {
            
            std::string listName  = it->first;
            
            if (Name == listName) {
                
                it->second = Value;
                
                return true;
            }
            
        }
        
        return false;
    }
    /** Returns a components value.*/
    float GetComponentValue(std::string Name) {
        
        for (std::map<std::string, float>::iterator it = ComponentList.begin(); it != ComponentList.end(); ++it) {
            
            std::string listName  = it->first;
            
            if (Name == listName) {
                
                float       listValue = it->second;
                
                return listValue;
            }
            
        }
        
        return -1.0f;
    }
    
    //
    // Settings
    
    /** Sets the render queue group to which this entity will be assigned.*/
    void    SetRenderQueueGroup(int Group)     {RenderQueueGroup = Group;}
    /** Returns the render queue group from which this entity was assigned.*/
    int     GetRenderQueueGroup(void)          {return RenderQueueGroup;}
    /** Sets the maximum render distance.*/
    void    SetRenderDistance(double Distance) {RenderDistance = Distance;}
    /** Returns the maximum render distance.*/
    double  GetRenderDistance(void)            {return RenderDistance;}
    
    /** Sets whether this entity is rendered.*/
    void    SetRenderState(bool State)        {IsRendered   = State;}
    /** Sets whether this object tracks the cameras position.*/
    void    SetAutoTrackingState(bool State)  {UseAutoTrack = State;}
    /** Sets whether this entity will render as a 2D overlay.*/
    void    SetOverlayState(bool State)       {IsOverlay    = State;}
    
    //
    // Attachment / detachment
    
    /** Attaches a mesh to this entity object.*/
    bool  AttachMesh(Mesh* MeshPtr) {
        if (MeshPtr == nullptr) {return false;}
        AttachedMesh = MeshPtr;
        return true;
    }
    /** Detaches a mesh from this entity object.*/
    void  DetachMesh(void) {
        AttachedMesh  = nullptr;
    }
    
    /** Adds a light to this entity object.*/
    bool  AddLight(Light* LightPtr) {
        if (LightPtr == nullptr) {return false;}
        LightList.push_back(LightPtr);
        return true;
    }
    /** Removes a light from this entity object.*/
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
    
    /** Attaches a rigid body to this entity object.*/
    bool  AttachBody(Physics::RigidBody* BodyPtr) {
        if (BodyPtr == nullptr) {return false;}
        AttachedBody = BodyPtr;
        return true;
    }
    /** Detaches a rigid body from this entity object.*/
    void  DetachBody(void) {
        AttachedBody = nullptr;
    }
    
    //
    // (Internal functions)
    
    /** Updates this entity object.*/
    void  Update(void);
    
    /** Activates this entity object and all of its attachments.*/
    void  Activate(void);
    /** Deactivates this entity object and all of its attachments.*/
    void  Deactivate(void);
    
};

Entity :: Entity() {
    
    // Name of the entity
    Name = "";
    
    // Name of the collider used for this entity
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
    ComponentList.clear();
    
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

void Entity :: Activate(void) {
    
    Light* LightListPtr;
    for (std::vector<Light*>::iterator it = this ->LightList.begin(); it != this ->LightList.end(); ++it) {
        
        LightListPtr = *it;
        
        LightListPtr ->IsActive = true;
        
    }
    
}

void Entity :: Deactivate(void) {
    
    Light* LightListPtr;
    for (std::vector<Light*>::iterator it = this ->LightList.begin(); it != this ->LightList.end(); ++it) {
        
        LightListPtr = *it;
        
        LightListPtr ->IsActive = false;
        
    }
    
}

