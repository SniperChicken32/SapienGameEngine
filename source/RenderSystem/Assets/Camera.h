//
// Camera object

struct Camera {
    
    std::string Name;
    
    bool UseMouseLook;
    
    float CameraHeight;
    
    float  vpFoV, vpAspect;
    double vpNear, vpFar;
    
    float MouseResetX, MouseResetY;
    float MouseX, MouseY;
    float MouseDiffX, MouseDiffY;
    float MouseOldX, MouseOldY;
    
    float MouseSensitivityYaw, MouseSensitivityPitch;
    float MouseLimitPitchMin, MouseLimitPitchMax;
    float MouseLimitYawMin, MouseLimitYawMax;
    
    Physics::RigidBody* AttachedBody;
    
    float MoveVelocity;
    float JumpVelocity;
    
    POSITION Position;
    ROTATION Rotation;
    ROTATION Forward;
    
    Camera(double, double, double);
    ~Camera();
    
    /** Attaches a rigid body to the camera.*/
    bool  AttachBody(Physics::RigidBody* BodyPtr) {if (BodyPtr == nullptr) {return false;}AttachedBody = BodyPtr; return true;}
    /** Detaches any attached rigid body.*/
    void  DetachBody(void)          {AttachedBody = nullptr;}
    
    /** Returns the height offset of the camera.*/
    float GetHeight(void)             {return CameraHeight;}
    /** Sets the height offset for the camera.*/
    void  SetHeight(float CamHeight)  {CameraHeight = CamHeight;}
    
    /** Sets the perspective projection.*/
    void  SetPerspective(double fov, double aspct, double nr, double fr) {vpFoV=fov; vpAspect=aspct; vpNear=nr; vpFar=fr;}
    /** Sets the field of view.*/
    void  SetFov(double fov) {vpFoV=fov;}
    /** Sets the aspect.*/
    void  SetAspect(double aspct) {vpAspect=aspct;}
    /** Sets the minimum camera depth.*/
    void  SetNear(double nr) {vpNear=nr;}
    /** Sets the maximum camera depth.*/
    void  SetFar(double fr) {vpFar=fr;}
    
    /** Sets the sensitivity multiplier for mouse movement.*/
    void  SetMouseSensitivity(float yaw, float pitch) {MouseSensitivityYaw = (yaw * 0.00005); MouseSensitivityPitch = (pitch * 0.00005);}
    /** Enables the use of mouse look.*/
    void  EnableMouseLook(bool State) {UseMouseLook = State; CursorSetPosition(MouseResetX, MouseResetY);}
    
    //
    // (Internal functions)
    
    /** Updates the mouse look.*/
    void  MouseLook(void);
    
    /** Update the camera object.*/
    void  Update(void);
    
};

Camera :: Camera(double x=0.0, double y=0.0, double z=0.0) {
    
    UseMouseLook   = false;
    
    // Camera projection
    CameraHeight = 5.5;
    vpFoV        = 60.0;
    vpAspect     = 1.33;
    vpNear       = 0.01;
    vpFar        = 1000000.0;
    
    // Mouse input settings
    MouseSensitivityYaw   =  0.000045;
    MouseSensitivityPitch =  0.00004;
    MouseLimitPitchMin    = -0.027;
    MouseLimitPitchMax    =  0.027;
    MouseLimitYawMin      = -1.0;
    MouseLimitYawMax      =  1.0;
    
    // Mouse position (internal)
    MouseResetX = 450;
    MouseResetY = 450;
    MouseX=0.0; MouseY=0.0;
    MouseDiffX=0.0; MouseDiffY=0.0;
    MouseOldX=0.0; MouseOldY=0.0;
    
    AttachedBody = nullptr;
    
    MoveVelocity = 16.0;
    JumpVelocity = 6.0;
    
    Position = glm::vec3(x, y, z);
    Rotation = glm::vec3(0.0, 0.0, 0.0);
    Forward  = glm::vec3(0.0, 0.0, 0.0);
    
}
Camera :: ~Camera() {
    
}

void Camera :: MouseLook(void) {
    
    // Get and reset mouse position
    glm::vec2 MousePos = CursorGetPosition();
    CursorSetPosition(MouseResetX, MouseResetY);
    
    MouseX = MousePos.x;
	MouseY = MousePos.y;
    
    // Calculate mouse movement
    MouseDiffX += (MouseX - MouseResetX) * MouseSensitivityYaw;
    MouseDiffY -= (MouseY - MouseResetY) * MouseSensitivityPitch;
    
    // Limit pitch
    if (MouseDiffY >= MouseLimitPitchMax) {MouseDiffY = MouseLimitPitchMax;}
    if (MouseDiffY <= MouseLimitPitchMin) {MouseDiffY = MouseLimitPitchMin;}
    
    // Wrap yaw
    if (MouseDiffX >= 0.109655)  {MouseDiffX -= 0.109655;}
    if (MouseDiffX <= 0.109655)  {MouseDiffX += 0.109655;}
    
    // Apply mouse look to camera
    Rotation.x = MouseDiffX;
    Rotation.y = MouseDiffY;
    
    MouseOldX = MouseX;
    MouseOldY = MouseY;
    
}

void Camera :: Update(void) {
    
    // Update our position to the rigid body
    if (this ->AttachedBody != nullptr) {
        
        // Get physical body transform
        Physics::Transform  EntityTransform  = this ->AttachedBody ->getTransform();
        
        // Sync the entity to the physical body`s position
        Physics::Vector3 EntityPosition = EntityTransform.getPosition();
        this ->Position = glm::vec3(EntityPosition.x, EntityPosition.y, EntityPosition.z);
        
    }
    
}

