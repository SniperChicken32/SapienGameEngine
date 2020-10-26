//
// Application sandbox

#ifndef APPLICATION_SANDBOX_
#define APPLICATION_SANDBOX_

struct Sandbox{
    
    bool Toggle;
    short int SecondCounter;
    short int HalfCounter;
    
    Timer* UpdateTimerSB;
    
    float TimeAlpha, TimeDelta, TimeFrequency;
    float TimeAccumulator;
    float UnitsPerSecond;
    float UpdateRateInc, UpdateRateDec;
    float UpdateRateSec, UpdateRateMs, UpdateRate;
    float UpdateRateMax, UpdateRateMin;
    float UpdateTime, UpdateRemainder;
    bool  UpdateComplete;
    
    float CamSpeed, JumpSpeed;
    bool  JumpState;
    bool  JumpReset;
    
    Sandbox();
    ~Sandbox();
    
    // Creation / destruction
    void Initiate(void);
    void Shutdown(void);
    
    void Create  (void);
    void Destroy (void);
    
    // Main loop
    bool Step    (void);
    
    // Input capturing
    void InputCapture (float);
    
    // Alarms for timing
    void TimerSecond(void);
    void TimerSecondHalf(void);
    void TimerSecondQuarter(void);
    
};

Sandbox :: Sandbox() {
    
    Toggle   = true;
    
    SecondCounter=0;
    HalfCounter=0;
    
    UpdateTimerSB = new Timer();
    
    // Update timing
    UpdateRate       =   4.0;
    UpdateRateMs     = 1000.0 / UpdateRate;
    
    TimeAlpha=0.0; TimeDelta=0.0; TimeFrequency=0.0;
    TimeAccumulator=0.0; UnitsPerSecond=0.0;
    UpdateRateInc=0.0; UpdateRateDec=0.0;
    UpdateComplete=0;
    
    // Camera controller
    CamSpeed  =  1.0;
    JumpSpeed =  2.5;
    JumpState = false;
    
    UpdateTime=0.0; UpdateRemainder=0.0;
    
}
Sandbox :: ~Sandbox() {
    
    delete UpdateTimerSB;
    
}

//
// Application sandbox
void Sandbox :: Initiate(void) {
    
}

void Sandbox :: Shutdown(void) {
    
}

void Sandbox :: Create(void) {
    
    // Load the main scene
    SceneManager::Scene* SceneMain = SceneManager::ResourceManagement::Loaders::LoadScene("data/main");
    
    // Update player controller settings
    if (Renderer ->CurrentCamera != nullptr) {
        CamSpeed  = Renderer ->CurrentCamera ->MoveVelocity;
        JumpSpeed = Renderer ->CurrentCamera ->JumpVelocity;
    }
    
}

void Sandbox :: Destroy(void) {
    
    
}

bool Sandbox :: Step(void) {
    
    bool UpdateComplete  = false;
    
    // ================================
    // Frame timing
    
    TimeDelta = UpdateTimerSB ->Check(true);
    TimeAccumulator += TimeDelta;
    
    // Calculate remaining time difference
    TimeAlpha = UpdateRateMs - TimeDelta;
    
    // Units of movement per second
    UnitsPerSecond = TimeDelta + TimeAlpha;
    
    // Accumulate frame time
    if (TimeAccumulator > UpdateRateMs) {
        
        // Upper limit
        if (TimeAccumulator > 10.0) {TimeAccumulator = 10.0;}
        
        // Drain the accumulator
        while (TimeAccumulator > 0.0) { TimeAccumulator -= UpdateRateMs; }
        
        UpdateTime      = TimeDelta;
        UpdateRemainder = TimeAlpha;
        
        UpdateComplete = true;
        
    }
    
    // ================================
    // Timing system
    if (UpdateComplete) {
        
        // Quarter second has elapsed
        this ->TimerSecondQuarter();
        
        HalfCounter++;
        if (HalfCounter > 1) {HalfCounter=0;
            
            // Half second has elapsed
            this ->TimerSecondHalf();
            
            SecondCounter++;
            if (SecondCounter > 1) {SecondCounter=0;
                
                // Second has elapsed
                this ->TimerSecond();
                
            }
            
        }
        
    }
    
    // Check ESCAPE key exit
    // NOTE: Returning false quits the event loop
    if (InputSystem::Input ->CheckKeyPressed(VK_ESCAPE)) {return false;}
    
    return true;
}


//
// Input capturing
void Sandbox :: InputCapture(float CurrentUnitsPerSecond) {
    
    if (InputSystem::Input ->CheckKeyPressed(VK_F5)) {
        
        SceneManager::Scene* ScenePtr = SceneManager::SceneMgr ->FindScene("ExampleScene");
        if (ScenePtr == nullptr) return;
        
        SceneManager::ResourceManagement::SceneSave( ScenePtr );
        
    }
    
    // Toggle mouse look
    if (InputSystem::Input ->CheckKeyPressed(VK_SPACE)) {
        
        Toggle = !Toggle;
        
        if (Renderer ->CurrentCamera != nullptr) {
            
            if (Toggle) {
                
                ShowCursor(true);
                Renderer ->CurrentCamera ->EnableMouseLook(false);
                
                Renderer ->CurrentCamera ->AttachedBody ->enableGravity(false);
                
            } else {
                
                ShowCursor(false);
                Renderer ->CurrentCamera ->EnableMouseLook(true);
                
                //Renderer ->CurrentCamera ->AttachedBody ->enableGravity(true);
                Renderer ->CurrentCamera ->AttachedBody ->enableGravity(false);
                
            }
            
        }
        
    }
    
    // Check camera pointer
    if (Renderer ->CurrentCamera != nullptr) {
        
        // Check camera physical body controller
        if (Renderer ->CurrentCamera ->AttachedBody != nullptr) {
            
            // Get current camera body
            Physics::RigidBody* CurrentBody = Renderer ->CurrentCamera ->AttachedBody;
            
            // Get speed according to time
            float CurrentSpeed = CamSpeed  * CurrentUnitsPerSecond;
            
            ROTATION Direction = {0.0, 0.0, 0.0};
            
            // Forward facing vector
            POSITION Forward = Renderer ->CurrentCamera ->Forward;
            
            // Directional input
            if (InputSystem::Input ->CheckKeyCurrent(VK_W))  {
                
                Direction.x += Forward.x * CurrentSpeed;
                Direction.y += Forward.y * CurrentSpeed;
                
            }
            if (InputSystem::Input ->CheckKeyCurrent(VK_S))  {
                
                Direction.x -= Forward.x * CurrentSpeed;
                Direction.y -= Forward.y * CurrentSpeed;
                
            }
            if (InputSystem::Input ->CheckKeyCurrent(VK_A))  {
                
                ROTATION Up = {0.0, 0.0, -CurrentSpeed};
                
                Direction += glm::cross(Up, Forward);
                
            }
            if (InputSystem::Input ->CheckKeyCurrent(VK_D))  {
                
                ROTATION Up = {0.0, 0.0, CurrentSpeed};
                
                Direction += glm::cross(Up, Forward);
                
            }
            
            if (InputSystem::Input ->CheckKeyCurrent(VK_R))  {
                
                Direction += ROTATION(0.0, 0.0, CurrentSpeed);
                
            }
            
            if (InputSystem::Input ->CheckKeyCurrent(VK_F))  {
                
                Direction -= ROTATION(0.0, 0.0, CurrentSpeed);
                
            }
            
            // Apply force to camera body
            Physics::Vector3 Force = Physics::Vector3(Direction.x, Direction.y, Direction.z);
            CurrentBody ->applyForceToCenterOfMass( Force );
            
        }
        
    }
    
    
    
    
    
    //
    
    // TESTING... SCENE RESET/SWITCH
    
    //
    
    
    if (InputSystem::Input ->CheckKeyPressed(VK_I)) {
        
        SceneManager::Scene* ScenePtr = SceneManager::SceneMgr ->FindScene("ExampleScene");
        if (ScenePtr == nullptr) return;
        
        ScenePtr ->Activate();
        
    }
    
    if (InputSystem::Input ->CheckKeyPressed(VK_K)) {
        
        SceneManager::Scene* ScenePtr = SceneManager::SceneMgr ->FindScene("ExampleScene");
        if (ScenePtr == nullptr) return;
        
        ScenePtr ->Deactivate();
        
    }
    
}


//
// Timing
void Sandbox :: TimerSecond(void) {
    
}

void Sandbox :: TimerSecondHalf(void) {
    
}

void Sandbox :: TimerSecondQuarter(void) {
    
    
}


// Sandbox singleton
Sandbox* SandboxApp = nullptr;

#endif

