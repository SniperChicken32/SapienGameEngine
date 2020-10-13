//
// Application layer

#include "EntryPoint.h"
#include "Procedure.h"

// Sandbox layer
#include "Sandbox.h"

//
// Windows application entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    
    // Handle to the window and command console
    HWND wHnd, cHnd;
    
    MSG messages;
    
    // Frame timer
    Timer* UpdateTimer;
    
    // Update timing
    double UpdateRate       =   PHYSICS_UPDATE_RATE;
    double UpdateRateMax    =   UpdateRate + 10.0;
    double UpdateRateMin    =   UpdateRate - 10.0;
    double UpdateRateMs     = 1000.0 / UpdateRate;
    double UpdateRateSec    =    1.0 / UpdateRate;
    
    double TimeDelta=0.0;
    double TimeAlpha=0.0;
    double TimeAccumulator=0.0;
    double UnitsPerSecond=UpdateRateMs;
    double UpdateComplete=0;
    double UpdateRemainder=0.0;
    
    // Window class
    WNDCLASSEX wClassEx;
    ZeroMemory((void*)&wClassEx, sizeof(wClassEx));
    
    LPCSTR WindowTitle       = "Render window";
    LPCSTR ClassName         = "FrameWindow";
    wClassEx.lpszClassName   = ClassName;
    wClassEx.cbSize          = sizeof(WNDCLASSEX);
    wClassEx.style           = CS_OWNDC;
    wClassEx.lpfnWndProc     = Procedure;
    wClassEx.cbClsExtra      = 0;
    wClassEx.cbWndExtra      = 0;
    wClassEx.hInstance       = hInstance;
    wClassEx.lpszMenuName    = NULL;
    wClassEx.hCursor         = LoadCursor(NULL, IDC_ARROW);
    wClassEx.hIcon           = LoadIcon(NULL, IDI_APPLICATION);
    wClassEx.hIconSm         = LoadIcon(NULL, IDI_APPLICATION);;
    wClassEx.hbrBackground   = (HBRUSH)GetStockObject(BLACK_BRUSH);
    
    if (!RegisterClassEx(&wClassEx)) {MessageBox(NULL, "Failed to register the window class.", "Error", MB_OK); return 0;}
    
    // Create window
    wHnd = CreateWindowEx(0, ClassName, WindowTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
    ShowWindow(wHnd, SW_SHOW);
    
    // Get console window handle
    cHnd = GetConsoleWindow();
    
#ifdef  SHOW_CONSOLE_
    
    ShowWindow(cHnd, SW_SHOW);
    
	SetWindowPos(cHnd, 0, 0, 0, 640, 480, SWP_NOSIZE | SWP_NOZORDER);
    
#else
    ShowWindow(cHnd, SW_HIDE);
#endif
    
    // Get display size
    HDC hDC = GetDC(wHnd);
    int DisplayWidth  = GetDeviceCaps(hDC, HORZRES);
    int DisplayHeight = GetDeviceCaps(hDC, VERTRES);
    
    VIEWPORT wArea;
    ZeroMemory((void*)&wArea, sizeof(wArea));
    
#ifdef DEVELOPMENT_MODE_
    // Smaller window for development
    wArea.w = 1024;
    wArea.h = 800;
#else
    // Figure a decent window size
    wArea.w = DisplayWidth  - 11;
    wArea.h = DisplayHeight - 81;
#endif
    
    // Place the window
    wArea.x = (DisplayWidth  / 2) - (wArea.w / 2);
    wArea.y = (DisplayHeight / 2) - (wArea.h / 2);
    SetWindowPos(wHnd, NULL, wArea.x, wArea.y, wArea.w, wArea.h, SWP_SHOWWINDOW);
    
    //
    // Initiate physics support
    PhysicsCommon = new Physics::PhysicsCommon();
    PhysicsWorld  = PhysicsCommon ->createPhysicsWorld();
    PhysicsWorld ->setNbIterationsPositionSolver( ITERATER_POSITION_SOLVER );
    PhysicsWorld ->setNbIterationsVelocitySolver( ITERATER_VELOCITY_SOLVER );
    PhysicsWorld ->enableSleeping(true);
    // Gravity
    Physics::Vector3 GravityVec = Physics::Vector3(GRAVITY_VECTOR);
    PhysicsWorld ->setGravity(GravityVec);
    // Debug rendering
    PhysicsWorld ->setIsDebugRenderingEnabled(false);
    
    // Create and register our custom event listener
    SceneManager::PhysicsManagement::EventListener listener;
    PhysicsWorld ->setEventListener(&listener);
    
    
    //
    // Initiate input system and scene manager
    InputSystem::InitiateInputSystem();
    SceneManager::InitiateSceneSystem();
    
    //
    // Initiate render system
    InitiateRenderSystem();
    // Setup render system
    Renderer ->SetTriangleDirection(RENDER_WINDING_CCW);
    Renderer ->SetMultiSample(true);
    
    Renderer ->SetFrameClear(false);
    //Renderer ->SetBackgroundColor( 1.0, 1.0, 1.0 );
    
    Renderer ->SetCulling(true);
    Renderer ->SetCullingDirection(RENDER_CULL_BACK);
    
    Renderer ->SetFrameRate(RENDER_FRAME_RATE);
    
    
    //
    // Initiate actor AI
    ActorAI::InitiateAI();
    
    
    // Initiate sandbox and a universal timer
    SandboxApp = new Sandbox();
    UpdateTimer = new Timer;
    
    
    // Set the render target
    Renderer ->SetRenderTarget(wHnd);
    
    // Initiate sandbox
    SandboxApp ->Initiate();
    SandboxApp ->Create();
    
    // Initiate random number generator
    srand( time(NULL) );
    
    //
    // Main loop
    bool IsActive = true;
    while (IsActive) {
        
        // Check window messages
        if (PeekMessage(&messages, NULL, 0, 0, PM_REMOVE)) {
            
            TranslateMessage(&messages);
            DispatchMessage(&messages);
            
        }
        
        // Capture input
        SandboxApp ->InputCapture( UnitsPerSecond );
        
        // Forward step the sandbox
        if (!SandboxApp ->Step()) IsActive = false;
        
        // Reset key states
        InputSystem::Input ->ClearKeysPressed();
        
        
        // ================================
        // Frame timer
        
        // Query the timer
        TimeDelta = UpdateTimer ->Check(true);
        TimeAccumulator += TimeDelta;
        
        // Calculate remaining time difference
        TimeAlpha = UpdateRateMs - TimeDelta;
        
        // Units of movement per second
        UnitsPerSecond = TimeDelta * 1000.0;
        
        // Check accumulated time
        if (TimeAccumulator > UpdateRateMs) {
            
            // Upper limit
            if (TimeAccumulator > 100.0) {TimeAccumulator = 100.0;}
            
            // Drain the accumulator
            while (TimeAccumulator > 0.0) {
                
                // Update the physics simulation
                PhysicsWorld ->update(UpdateRateSec);
                
                TimeAccumulator -= UpdateRateMs;
                
            }
            
        }
        
        //
        // Render the scene (built-in frame timer)
        if (Renderer ->SceneRender()) {
            
            // Update AI actors
            ActorAI::AI ->UpdateActors();
            
        }
        
    }
    
    // Shutdown sandbox
    SandboxApp ->Destroy();
    SandboxApp ->Shutdown();
    
    
    // Release the rendering target
    Renderer ->ReleaseRenderTarget();
    
    // Close the window
    DestroyWindow(wHnd);
    
    
    // Shutdown sandbox and the timer
    delete SandboxApp;
    delete UpdateTimer;
    
    
    // Clear all scene graphs
    SceneManager::SceneDestroy();
    
    // Shutdown scene manager and input system
    SceneManager::ShutdownSceneSystem();
    InputSystem::ShutdownInputSystem();
    
    
    // Shutdown actorAI
    ActorAI::ShutdownAI();
    
    
    // Release singletons
    ShutdownRenderSystem();
    
    // Destroy physics colliders
    SceneManager::PhysicsManagement::DestroyAllProxies();
    
    // Shutdown physics system
    PhysicsCommon ->destroyPhysicsWorld(PhysicsWorld);
    delete PhysicsCommon;
    
    return messages.wParam;
}

