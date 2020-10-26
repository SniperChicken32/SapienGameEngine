//
// Render system

struct   VIEWPORT   {int x, y, w, h;};

// Colors
#define  COLOR    glm::vec3
#define  COLORA   glm::vec4

// Translation
#define  POSITION   glm::vec3
#define  ROTATION   glm::vec3
#define  SCALE      glm::vec3

// Render assets
#include "RenderSystem/Assets/Light.h"
#include "RenderSystem/Assets/Camera.h"

#include "RenderSystem/Assets/Shader.h"
#include "RenderSystem/Assets/Material.h"
#include "RenderSystem/Assets/Mesh.h"

// Entity
#include "RenderSystem/Assets/Entity.h"

class RenderSystem {
    
    HWND  WindowHandle;
    HDC   DeviceContext;
    HGLRC RenderContext;
    
    bool FrameRenderComplete;
    bool DoClearFrame;
    bool UsePerspective;
    
    int FrameCounterA, FrameCounterB, FrameCounterC;
    
    VIEWPORT  ViewPort, DisplayPort;
    COLOR     Background, RenderColor;
    
    std::vector<Light*>  LightQueue;
    
    std::vector<Entity*>   EntityList;
    std::vector<Mesh*>     MeshList;
    std::vector<Material*> MaterialList;
    std::vector<Shader*>   ShaderList;
    
    std::vector<Light*>    LightList;
    std::vector<Camera*>   CameraList;
    
    std::vector<Entity*> RecycleList;
    
    std::vector<Entity*> RenderQueueBackground;
    std::vector<Entity*> RenderQueueSkyEarly;
    std::vector<Entity*> RenderQueueSky;
    
    std::vector<Entity*> RenderQueueWorldGeometry;
    std::vector<Entity*> RenderQueueGeometryEarly;
    std::vector<Entity*> RenderQueueGeometry;
    std::vector<Entity*> RenderQueueGeometryLate;
    
    std::vector<Entity*> RenderQueueOverlayEarly;
    std::vector<Entity*> RenderQueueOverlay;
    std::vector<Entity*> RenderQueueForeground;
    
    std::vector<Entity*>* QueueList[10];
    
    Timer*    FrameTimer;
    
    float TimeAlpha, TimeDelta, TimeFrequency;
    float TimeAccumulator;
    float UnitsPerSecond;
    float FrameRateInc, FrameRateDec;
    float FrameFrameRateSec, FrameRateMs, FrameRateDrop, FrameDropPoint, FrameRate;
    float FrameRateMax, FrameRateMin;
    bool  FrameComplete;
    
    float FrameTime, FrameRemainder;
    
    public:
    
    Mesh*     CurrentMesh;
    Material* CurrentMaterial;
    Shader*   CurrentShader;
    Entity*   CurrentSky;
    Camera*   CurrentCamera;
    
    RenderSystem();
    ~RenderSystem();
    
    /** Sets the current rendering target.*/
    GLenum SetRenderTarget(HWND vHandle);
    /** Frees the current rendering target.*/
    void   ReleaseRenderTarget(void);
    
    /** Creates an entity object and returns its pointer.*/
    Entity*       CreateEntity         (double x, double y, double z);
    /** Destroys an entity object.*/
    bool          DestroyEntity        (Entity*);
    
    /** Creates a mesh object and returns its pointer.*/
    Mesh*         CreateMesh           (void);
    /** Destroys a mesh object.*/
    bool          DestroyMesh          (Mesh*);
    /** Finds a mesh object by the given name.*/
    Mesh*         FindMesh             (std::string);
    
    /** Creates a material object and returns its pointer.*/
    Material*     CreateMaterial       (void);
    /** Destroys a material object.*/
    bool          DestroyMaterial      (Material*);
    /** Finds a material object by the given name.*/
    Material*     FindMaterial         (std::string);
    
    /** Creates a shader object and returns its pointer.*/
    Shader*       CreateShader         (void);
    /** Destroys a shader object.*/
    bool          DestroyShader        (Shader*);
    /** Finds a shader object by the given name.*/
    Shader*       FindShader           (std::string);
    
    /** Creates a light object and returns its pointer.*/
    Light*        CreateLight          (void);
    /** Destroys a light object.*/
    bool          DestroyLight         (Light*);
    /** Finds a light object by the given name.*/
    Light*        FindLight            (std::string);
    
    /** Creates a camera object and returns its pointer.*/
    Camera*       CreateCamera         (double x, double y, double z);
    /** Destroys a camera object.*/
    bool          DestroyCamera        (Camera*);
    /** Finds a camera object by the given name.*/
    Camera*       FindCamera           (std::string);
    
    /** Attaches a camera object to the render system.*/
    void          AttachCamera         (Camera*);
    /** Frees any attached camera objects.*/
    void          DetachCamera         (void);
    
    /** Attaches a sky entity to the render system.*/
    void          AttachSky            (Entity* EntityPtr) {
        CurrentSky = EntityPtr;
    }
    /** Frees any attached sky entities.*/
    void          DetachSky            (void) {
        CurrentSky = nullptr;
    }
    
    /** Enables face culling.*/
    void          SetCulling           (bool State)  {if (State) {glEnable(GL_CULL_FACE);} else {glDisable(GL_CULL_FACE);}}
    /** Enables background clearing before drawing.*/
    void          SetFrameClear        (bool State)  {DoClearFrame = State;}
    /** Enables the multi-sampling state.*/
    void          SetMultiSample       (bool State)  {if (State) {glEnable(GL_MULTISAMPLE);} else {glDisable(GL_MULTISAMPLE);}}
    /** Sets the area of the viewport.*/
    void          SetViewport          (int xx, int yy, int ww, int hh) {glViewport(xx,yy,ww,hh); ViewPort.x = xx; ViewPort.x = ww; ViewPort.x = yy; ViewPort.x = hh;}
    /** Sets the area of the viewport.*/
    int           GetViewportWidth(void) {return this ->ViewPort.w;}
    /** Sets the area of the viewport.*/
    int           GetViewportHeight(void) {return this ->ViewPort.h;}
    /** Sets the background color.*/
    void          SetBackgroundColor   (float rr, float gg, float bb) {Background.r = rr; Background.g = gg; Background.b = bb;}
    /** Sets the direction in which the triangles are drawn.
    RENDER_WINDING_CCW  RENDER_WINDING_CW*/
    void          SetTriangleDirection (GLenum Wind) {if (Wind == RENDER_WINDING_CCW) {glFrontFace(GL_CCW);} if (Wind == RENDER_WINDING_CW) {glFrontFace(GL_CW);}}
    /** Sets the face culling direction.
    RENDER_CULL_FRONT  RENDER_CULL_BACK*/
    void          SetCullingDirection  (GLenum Cull) {if (Cull == RENDER_CULL_FRONT) {glCullFace(GL_FRONT);} if (Cull == RENDER_CULL_BACK)  {glCullFace(GL_BACK);}}
    
    /** Draws the current frame using an internal timer to control the frame rate.*/
    bool          SceneRender          (void);
    /** Sets the frame rate for the internal frame timer.
    Default: 120.0*/
    void          SetFrameRate         (float);
    
    /** Destroys all instances of entity and light objects in the render system.*/
    void          SceneClear           (void);
    
    /** Destroys all loaded object definitions.*/
    void          DestroyAssetCache    (void);
    
    //
    // Advanced
    
    /** Adds an entity to its assigned render queue group.*/
    void          AddToRenderQueues      (Entity*);
    /** Removes an entity from render queue groups.*/
    bool          RemoveFromRenderQueues (Entity*);
    
    /** Refreshes the render queue groups.*/
    void          RefreshRenderQueue   (void);
    /** Refreshes the light objects.*/
    void          RefreshLightQueue    (void);
    /** Refreshes the currently bound rendering asset.*/
    bool          RefreshAssetBindings (Mesh*);
    
    //
    // (Internal functions)
    
    // Render pipeline
    void          RenderPipeline       (void);
    void          RenderEntity         (Entity*, glm::mat4&, int&);
    
    // Entity enable/disable
    void          RenderEnableEntity(Entity*);
    void          RenderDisableEntity(Entity*);
    
    // Draw call
    void          RenderDrawCall       (int&);
    glm::mat4     SetProjectionByCamera(void);
    
    // Matrix operations
    glm::mat4     CalculateModelMatrix         (POSITION& PositionWorld, ROTATION &RotationWorld, SCALE &ScaleWorld, POSITION &PositionModel, ROTATION &RotationModel);
    glm::mat4     CalculatePerspectiveMatrix   (POSITION& Position, ROTATION& Angle, float& vpFoV, float& vpAspect, double& vpNear, double& vpFar);
    glm::mat4     CalculateOrthographicMatrix  (POSITION& Position, ROTATION &Angle, int Width, int Height);
    
};

// Render system singleton
/** Render system singleton object pointer.*/
RenderSystem* Renderer = nullptr;
/** Initiate the render system singleton object.*/
bool InitiateRenderSystem(void) {if (Renderer == nullptr) {Renderer = new RenderSystem(); return true;}return false;}
/** Shutdown the render system singleton object.*/
bool ShutdownRenderSystem(void) {if (Renderer != nullptr) {delete Renderer; Renderer = nullptr; return true;}return false;}

RenderSystem :: RenderSystem() {
    
    // Clear the log
    LogClear();
    LogWrite(strings::RenderSystemInitialize);
    LogWriteLn();
    
    WindowHandle  = NULL;
    DeviceContext = NULL;
    RenderContext = NULL;
    
    FrameRenderComplete = false;
    DoClearFrame        = true;
    UsePerspective      = true;
    
    // Frame count skipper
    FrameCounterA=0;
    FrameCounterB=0;
    FrameCounterC=0;
    
    // Render window port size
    ViewPort.x=0;
    ViewPort.y=0;
    ViewPort.w=0;
    ViewPort.h=0;
    
    // Display port size
    DisplayPort.x=0;
    DisplayPort.y=0;
    DisplayPort.w=0;
    DisplayPort.h=0;
    
    // Background color
    Background.r  = 0.008;
    Background.g  = 0.008;
    Background.b  = 0.018;
    
    // Render color
    RenderColor.r = 1.0;
    RenderColor.g = 1.0;
    RenderColor.b = 1.0;
    
    LightQueue.clear();
    
    EntityList.clear();
    MeshList.clear();
    MaterialList.clear();
    ShaderList.clear();
    
    LightList.clear();
    CameraList.clear();
    
    RecycleList.clear();
    
    // Background render queue groups
    QueueList[0] = &RenderQueueBackground;
    QueueList[1] = &RenderQueueSkyEarly;
    QueueList[2] = &RenderQueueSky;
    
    // Geometry render queue groups
    QueueList[3] = &RenderQueueWorldGeometry;
    QueueList[4] = &RenderQueueGeometryEarly;
    QueueList[5] = &RenderQueueGeometry;
    QueueList[6] = &RenderQueueGeometryLate;
    
    // Foreground render queue groups
    QueueList[7] = &RenderQueueOverlayEarly;
    QueueList[8] = &RenderQueueOverlay;
    QueueList[9] = &RenderQueueForeground;
    
    FrameTimer = new Timer;
    
    // Update timing
    FrameRate       =   60.0;
    FrameRateMax    =   FrameRate + 10.0;
    FrameRateMin    =   FrameRate - 10.0;
    FrameRateMs     = 1000.0 / FrameRate;
    
    TimeAlpha=0.0; TimeDelta=0.0; TimeFrequency=0.0;
    TimeAccumulator=0.0; UnitsPerSecond=0.0;
    FrameRateInc=0.0; FrameRateDec=0.0;
    FrameComplete=0;
    
    FrameTime=0.0; FrameRemainder=0.0;
    
    // Current binding states
    CurrentMesh     = nullptr;
    CurrentMaterial = nullptr;
    CurrentShader   = nullptr;
    CurrentSky      = nullptr;
    CurrentCamera   = nullptr;
    
}
RenderSystem :: ~RenderSystem() {
    
    // Log render system shutdown
    LogWriteLn();
    LogWrite(strings::RenderSystemShutdown);
    
    delete FrameTimer;
    
    // Destroy engine components
    for (std::vector<Camera*>::iterator it = CameraList.begin(); it != CameraList.end(); ++it) {
        Camera* CameraPtr = *it;
        if (CameraPtr != nullptr) {
            delete CameraPtr;
        }
    }
    for (std::vector<Entity*>::iterator it = EntityList.begin(); it != EntityList.end(); ++it) {
        Entity* EntityPtr = *it;
        if (EntityPtr != nullptr) {
            delete EntityPtr;
        }
    }
    for (std::vector<Light*>::iterator it = LightList.begin(); it != LightList.end(); ++it) {
        Light* LightPtr = *it;
        if (LightPtr != nullptr) {
            delete LightPtr;
        }
    }
    for (std::vector<Mesh*>::iterator it = MeshList.begin(); it != MeshList.end(); ++it) {
        Mesh* MeshPtr = *it;
        if (MeshPtr != nullptr) {
            delete MeshPtr;
        }
    }
    for (std::vector<Material*>::iterator it = MaterialList.begin(); it != MaterialList.end(); ++it) {
        Material* MaterialPtr = *it;
        if (MaterialPtr != nullptr) {
            delete MaterialPtr;
        }
    }
    for (std::vector<Shader*>::iterator it = ShaderList.begin(); it != ShaderList.end(); ++it) {
        Shader* ShaderPtr = *it;
        if (ShaderPtr != nullptr) {
            delete ShaderPtr;
        }
    }
    
}

GLenum RenderSystem :: SetRenderTarget(HWND wHndl) {
    
    int iFormat;
    std::string gcVendor, gcRenderer, gcExtensions, gcVersion, Line;
    
    // Set the window handle and get the device context
    WindowHandle = wHndl;
    HDC hDC = GetDC(wHndl);
    DeviceContext = hDC;
    
    // Get display size
    DisplayPort.w = GetDeviceCaps(DeviceContext, HORZRES);
    DisplayPort.h = GetDeviceCaps(DeviceContext, VERTRES);
    
    // Pixel format descriptor
    PIXELFORMATDESCRIPTOR pfd;
    ZeroMemory (&pfd, sizeof (pfd));
    pfd.nSize       = sizeof (pfd);
    pfd.nVersion    = 1;
    pfd.dwFlags     = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType  = PFD_TYPE_RGBA;
    pfd.cColorBits  = 32;
    pfd.cDepthBits  = 16;
    pfd.iLayerType  = PFD_MAIN_PLANE;
    
    // Setup pixel format
    iFormat = ChoosePixelFormat(DeviceContext, &pfd);
    SetPixelFormat(DeviceContext, iFormat, &pfd);
    
    HGLRC hRC = wglCreateContext(hDC);
    RenderContext = hRC;
    
    // Create the render context and set the target
    wglMakeCurrent(DeviceContext, hRC);
    
    // Initiate glew after setting the render target
    GLenum ErrorString = glewInit();
    
#ifdef  LOG_RENDER_DETAILS_
    // Get hardware details
    const char* gcVendorConst     = (const char*)glGetString(GL_VENDOR);
    const char* gcRendererConst   = (const char*)glGetString(GL_RENDERER);
    const char* gcExtensionsConst = (const char*)glGetString(GL_EXTENSIONS);
    const char* gcVersionConst    = (const char*)glGetString(GL_VERSION);
    
    gcVendor     = std::string(gcVendorConst);
    gcRenderer   = std::string(gcRendererConst);
    gcExtensions = std::string(gcExtensionsConst);
    gcVersion    = std::string(gcVersionConst);
    
    // Set window viewport
    RECT WindowRect; GetWindowRect(wHndl, &WindowRect);
    this ->SetViewport(0, 0, WindowRect.right - WindowRect.left, WindowRect.bottom - WindowRect.top);
    
    // Log details
    LogWrite("== Hardware details =="); Line = "" + gcRenderer;
    LogWrite(Line);
    LogWriteLn();
    
    std::string DetailStringHead = "  - ";
    std::string DetailStringEqu  = " = ";
    
    Line = " Device details"; LogWrite(Line);
    Line = DetailStringHead + "Name   " + DetailStringEqu + gcVendor;  LogWrite(Line);
    Line = DetailStringHead + "Version" + DetailStringEqu + gcVersion; LogWrite(Line);
    LogWriteLn();
    
    Line = " Colors"; LogWrite(Line);
    Line = DetailStringHead + "Color" + DetailStringEqu + IntToString(pfd.cColorBits) + " bit"; LogWrite(Line);
    Line = DetailStringHead + "Depth" + DetailStringEqu + IntToString(pfd.cDepthBits) + " bit"; LogWrite(Line);
    LogWriteLn();
#endif
    
    return ErrorString;
}
void RenderSystem :: ReleaseRenderTarget(void) {
    
    // Null the current contexts
    wglMakeCurrent (NULL, NULL);
    wglDeleteContext(RenderContext);
    
    // Release the device context
    ReleaseDC(WindowHandle, DeviceContext);
    
}


Entity*  RenderSystem :: CreateEntity(double x=0.0, double y=0.0, double z=0.0) {
    
    Entity* EntityPtr = nullptr;
    
#ifdef  CONSOLE_CHECK_ALLOCATIONS_
    std::cout << "+ Entity" << std::endl;
#endif
    
    // Create an entity
    EntityPtr = new Entity;
    EntityPtr -> SetPosition(x, y, z);
    EntityList.push_back(EntityPtr);
    
    this ->AddToRenderQueues(EntityPtr);
    
    return EntityPtr;
}
bool     RenderSystem :: DestroyEntity(Entity* EntityPtr) {
    if (EntityPtr == nullptr) {return false;}
    
    this ->RemoveFromRenderQueues(EntityPtr);
    
    for (std::vector<Entity*>::iterator it = EntityList.begin(); it != EntityList.end(); ++it) {
        
        Entity* EntityListPtr = *it;
        
        if (EntityPtr == EntityListPtr) {
            
            // Clear lights
            for (std::vector<Light*>::iterator itb = EntityPtr ->LightList.begin(); itb != EntityPtr ->LightList.end(); ++itb) {
                
                Light* LightPtr = *itb;
                
                this ->DestroyLight(LightPtr);
                
            }
            
            EntityList.erase(it);
            
#ifdef  CONSOLE_CHECK_ALLOCATIONS_
            std::cout << "- Entity" << std::endl;
#endif
            
            delete EntityPtr;
            
            return true;
        }
        
    }
    
    return false;
}

Mesh*  RenderSystem :: CreateMesh(void) {
    
#ifdef  CONSOLE_CHECK_ALLOCATIONS_
    std::cout << "+ Mesh" << std::endl;
#endif
    
    Mesh* MeshPtr = new Mesh();
    MeshList.push_back(MeshPtr);
    
    return MeshPtr;
}
bool   RenderSystem :: DestroyMesh(Mesh* MeshPtr) {
    
#ifdef  CONSOLE_CHECK_ALLOCATIONS_
    std::cout << "- Mesh" << std::endl;
#endif
    
    for (std::vector<Mesh*>::iterator it = MeshList.begin(); it != MeshList.end(); ++it) {
        
        Mesh* MeshListPtr = *it;
        
        if (MeshPtr == MeshListPtr) {
            
            MeshList.erase(it);
            
            delete MeshPtr;
            
            return true;
        }
        
    }
    
    return false;
}
Mesh*  RenderSystem :: FindMesh(std::string AssetName) {
    for (std::vector<Mesh*>::iterator it = MeshList.begin(); it != MeshList.end(); ++it) {
        Mesh* MeshListPtr = *it;
        if (MeshListPtr ->Name == AssetName) {return MeshListPtr;}
    }
    return nullptr;
}

Material*  RenderSystem :: CreateMaterial(void) {
    
#ifdef  CONSOLE_CHECK_ALLOCATIONS_
    std::cout << "+ Material" << std::endl;
#endif
    
    Material* MaterialPtr = new Material();
    MaterialList.push_back(MaterialPtr);
    
    return MaterialPtr;
}
bool       RenderSystem :: DestroyMaterial(Material* MaterialPtr) {
    
#ifdef  CONSOLE_CHECK_ALLOCATIONS_
    std::cout << "- Material" << std::endl;
#endif
    
    for (std::vector<Material*>::iterator it = MaterialList.begin(); it != MaterialList.end(); ++it) {
        
        Material* MaterialListPtr = *it;
        
        if (MaterialPtr == MaterialListPtr) {
            
            MaterialList.erase(it);
            
            delete MaterialPtr;
            
            return true;
        }
        
    }
    return false;
}
Material*  RenderSystem :: FindMaterial(std::string AssetName) {
    for (std::vector<Material*>::iterator it = MaterialList.begin(); it != MaterialList.end(); ++it) {
        Material* MaterialListPtr = *it;
        if (MaterialListPtr ->Name == AssetName) {return MaterialListPtr;}
    }
    return nullptr;
}

Shader*  RenderSystem :: CreateShader(void) {
    
#ifdef  CONSOLE_CHECK_ALLOCATIONS_
    std::cout << "+ Shader" << std::endl;
#endif
    
    Shader* ShaderPtr = new Shader();
    ShaderList.push_back(ShaderPtr);
    
    return ShaderPtr;
}
bool     RenderSystem :: DestroyShader(Shader* ShaderPtr) {
    
#ifdef  CONSOLE_CHECK_ALLOCATIONS_
    std::cout << "-Shader" << std::endl;
#endif
    
    for (std::vector<Shader*>::iterator it = ShaderList.begin(); it != ShaderList.end(); ++it) {
        
        Shader* ShaderListPtr = *it;
        
        if (ShaderPtr == ShaderListPtr) {
            
            ShaderList.erase(it);
            
            delete ShaderPtr;
            
            return true;
        }
        
    }
    
    return false;
}
Shader*  RenderSystem :: FindShader(std::string AssetName) {
    for (std::vector<Shader*>::iterator it = ShaderList.begin(); it != ShaderList.end(); ++it) {
        Shader* ShaderListPtr = *it;
        if (ShaderListPtr ->Name == AssetName) {return ShaderListPtr;}
    }
    return nullptr;
}

Light*  RenderSystem :: CreateLight(void) {
    
#ifdef  CONSOLE_CHECK_ALLOCATIONS_
    std::cout << "+ Light" << std::endl;
#endif
    
    Light* LightPtr = new Light();
    LightList.push_back(LightPtr);
    
    return LightPtr;
}
bool    RenderSystem :: DestroyLight(Light* LightPtr) {
    
#ifdef  CONSOLE_CHECK_ALLOCATIONS_
    std::cout << "- Light" << std::endl;
#endif
    
    for (std::vector<Light*>::iterator it = LightList.begin(); it != LightList.end(); ++it) {
        
        Light* LightListPtr = *it;
        
        if (LightPtr == LightListPtr) {
            
            LightList.erase(it);
            
            delete LightPtr;
            
            return true;
        }
        
    }
    
    this ->RefreshLightQueue();
    
    return false;
}
Light*  RenderSystem :: FindLight(std::string AssetName) {
    for (std::vector<Light*>::iterator it = LightList.begin(); it != LightList.end(); ++it) {
        Light* LightListPtr = *it;
        if (LightListPtr ->Name == AssetName) {return LightListPtr;}
    }
    return nullptr;
}

Camera*  RenderSystem :: CreateCamera(double x=0.0, double y=0.0, double z=0.0) {
    
#ifdef  CONSOLE_CHECK_ALLOCATIONS_
    std::cout << "+ Camera" << std::endl;
#endif
    
    Camera* CamPtr = new Camera(x, y, z);
    CameraList.push_back(CamPtr);
    
    return CamPtr;
}
bool     RenderSystem :: DestroyCamera(Camera* CameraPtr) {
    
#ifdef  CONSOLE_CHECK_ALLOCATIONS_
    std::cout << "- Camera" << std::endl;
#endif
    
    for (std::vector<Camera*>::iterator it = CameraList.begin(); it != CameraList.end(); ++it) {
        
        Camera* CameraListPtr = *it;
        
        if (CameraPtr == CameraListPtr) {
            
            CameraList.erase(it);
            
            delete CameraPtr;
            
            return true;
        }
        
    }
    
    return false;
}
Camera*  RenderSystem :: FindCamera(std::string AssetName) {
    for (std::vector<Camera*>::iterator it = CameraList.begin(); it != CameraList.end(); ++it) {
        Camera* CameraPtr = *it;
        if (CameraPtr ->Name == AssetName) {return CameraPtr;}
    }
    return nullptr;
}


void   RenderSystem :: AttachCamera(Camera* NewCameraPtr) {CurrentCamera = NewCameraPtr;}
void   RenderSystem :: DetachCamera(void) {CurrentCamera = nullptr;}

// Render queue
void RenderSystem :: AddToRenderQueues(Entity* EntityPtr) {
    
    int RenderQueueGroup = EntityPtr ->RenderQueueGroup;
    
    switch (RenderQueueGroup) {
        
        case RENDER_QUEUE_WORLD_GEOMETRY: RenderQueueWorldGeometry.push_back(EntityPtr); break;
        case RENDER_QUEUE_GEOMETRY_EARLY: RenderQueueGeometryEarly.push_back(EntityPtr); break;
        default: 
        case RENDER_QUEUE_GEOMETRY:       RenderQueueGeometry.push_back(EntityPtr); break;
        case RENDER_QUEUE_GEOMETRY_LATE:  RenderQueueGeometryLate.push_back(EntityPtr); break;
        case RENDER_QUEUE_BACKGROUND:     RenderQueueBackground.push_back(EntityPtr); break;
        case RENDER_QUEUE_SKY_EARLY:      RenderQueueSkyEarly.push_back(EntityPtr); break;
        case RENDER_QUEUE_SKY:            RenderQueueSky.push_back(EntityPtr); break;
        case RENDER_QUEUE_OVERLAY_EARLY:  RenderQueueOverlayEarly.push_back(EntityPtr); break;
        case RENDER_QUEUE_OVERLAY:        RenderQueueOverlay.push_back(EntityPtr); break;
        case RENDER_QUEUE_FOREGROUND:     RenderQueueForeground.push_back(EntityPtr); break;
        
    }
    
}
bool RenderSystem :: RemoveFromRenderQueues(Entity* EntityPtr) {
    
    for (std::vector<Entity*>::iterator it = RenderQueueGeometry.begin(); it != RenderQueueGeometry.end(); ++it) {
        Entity* EntityListPtr = *it;
        
        if (EntityPtr == EntityListPtr) {RenderQueueGeometry.erase(it); return true;}
        
    }
    for (std::vector<Entity*>::iterator it = RenderQueueGeometryEarly.begin(); it != RenderQueueGeometryEarly.end(); ++it) {
        Entity* EntityListPtr = *it;
        
        if (EntityPtr == EntityListPtr) {RenderQueueGeometryEarly.erase(it); return true;}
        
    }
    for (std::vector<Entity*>::iterator it = RenderQueueGeometryLate.begin(); it != RenderQueueGeometryLate.end(); ++it) {
        Entity* EntityListPtr = *it;
        
        if (EntityPtr == EntityListPtr) {RenderQueueGeometryLate.erase(it); return true;}
        
    }
    for (std::vector<Entity*>::iterator it = RenderQueueWorldGeometry.begin(); it != RenderQueueWorldGeometry.end(); ++it) {
        Entity* EntityListPtr = *it;
        
        if (EntityPtr == EntityListPtr) {RenderQueueWorldGeometry.erase(it); return true;}
        
    }
    for (std::vector<Entity*>::iterator it = RenderQueueOverlay.begin(); it != RenderQueueOverlay.end(); ++it) {
        Entity* EntityListPtr = *it;
        
        if (EntityPtr == EntityListPtr) {RenderQueueOverlay.erase(it); return true;}
        
    }
    for (std::vector<Entity*>::iterator it = RenderQueueOverlayEarly.begin(); it != RenderQueueOverlayEarly.end(); ++it) {
        Entity* EntityListPtr = *it;
        
        if (EntityPtr == EntityListPtr) {RenderQueueOverlayEarly.erase(it); return true;}
        
    }
    for (std::vector<Entity*>::iterator it = RenderQueueSky.begin(); it != RenderQueueSky.end(); ++it) {
        Entity* EntityListPtr = *it;
        
        if (EntityPtr == EntityListPtr) {RenderQueueSky.erase(it); return true;}
        
    }
    for (std::vector<Entity*>::iterator it = RenderQueueSkyEarly.begin(); it != RenderQueueSkyEarly.end(); ++it) {
        Entity* EntityListPtr = *it;
        
        if (EntityPtr == EntityListPtr) {RenderQueueSkyEarly.erase(it); return true;}
        
    }
    for (std::vector<Entity*>::iterator it = RenderQueueForeground.begin(); it != RenderQueueForeground.end(); ++it) {
        Entity* EntityListPtr = *it;
        
        if (EntityPtr == EntityListPtr) {RenderQueueForeground.erase(it); return true;}
        
    }
    for (std::vector<Entity*>::iterator it = RenderQueueBackground.begin(); it != RenderQueueBackground.end(); ++it) {
        Entity* EntityListPtr = *it;
        
        if (EntityPtr == EntityListPtr) {RenderQueueBackground.erase(it); return true;}
        
    }
    
    return false;
}

// Set the frame rate for the render system
void RenderSystem :: SetFrameRate(float NewFrameRate) {
    FrameRate    = NewFrameRate;
    FrameRateMax = NewFrameRate;
    FrameRateMs  = 1000.0 / FrameRate;
    
    UnitsPerSecond = FrameRateMs;
    FrameRateDrop  = std::round(FrameRateMs / FrameDropPoint);
}

// Clear the scene of entities
void RenderSystem :: SceneClear(void) {
    
    // Dereference associated components
    this ->CurrentSky      = nullptr;
    this ->CurrentShader   = nullptr;
    this ->CurrentMaterial = nullptr;
    this ->CurrentMesh     = nullptr;
    
    // Destroy scene objects
    for (std::vector<Entity*>::iterator it = EntityList.begin(); it != EntityList.end(); ++it) {
        Entity* EntityPtr = *it;
        if (EntityPtr != nullptr) {
            delete EntityPtr;
        }
    }
    for (std::vector<Light*>::iterator it = LightList.begin(); it != LightList.end(); ++it) {
        Light* LightPtr = *it;
        if (LightPtr != nullptr) {
            
            // Only destroy instantiated lights
            if (LightPtr ->Name == "") {
                
                delete LightPtr;
                
            }
            
        }
    }
    
    // Clear scene lists
    EntityList.clear();
    LightList.clear();
    LightQueue.clear();
    
    RenderQueueBackground.clear();
    RenderQueueSkyEarly.clear();
    RenderQueueSky.clear();
    
    RenderQueueWorldGeometry.clear();
    RenderQueueGeometryEarly.clear();
    RenderQueueGeometry.clear();
    RenderQueueGeometryLate.clear();
    
    RenderQueueOverlayEarly.clear();
    RenderQueueOverlay.clear();
    RenderQueueForeground.clear();
    
}

// Clear the asset cache data
void RenderSystem :: DestroyAssetCache(void) {
    
    // Clear instances first
    this ->SceneClear();
    
    // Dereference associated components
    this ->CurrentCamera = nullptr;
    
    // Destroy engine components
    for (std::vector<Camera*>::iterator it = CameraList.begin(); it != CameraList.end(); ++it) {
        Camera* CameraPtr = *it;
        if (CameraPtr != nullptr) {
            delete CameraPtr;
            CameraPtr = nullptr;
        }
    }
    for (std::vector<Mesh*>::iterator it = MeshList.begin(); it != MeshList.end(); ++it) {
        Mesh* MeshPtr = *it;
        if (MeshPtr != nullptr) {
            delete MeshPtr;
            MeshPtr = nullptr;
        }
    }
    for (std::vector<Material*>::iterator it = MaterialList.begin(); it != MaterialList.end(); ++it) {
        Material* MaterialPtr = *it;
        if (MaterialPtr != nullptr) {
            delete MaterialPtr;
            MaterialPtr = nullptr;
        }
    }
    for (std::vector<Shader*>::iterator it = ShaderList.begin(); it != ShaderList.end(); ++it) {
        Shader* ShaderPtr = *it;
        if (ShaderPtr != nullptr) {
            delete ShaderPtr;
            ShaderPtr = nullptr;
        }
    }
    
    CameraList.clear();
    MeshList.clear();
    MaterialList.clear();
    ShaderList.clear();
    
}



//
// Refresh functions
void RenderSystem :: RefreshRenderQueue(void) {
    
    // Clear render lists
    RenderQueueBackground.clear();
    RenderQueueSkyEarly.clear();
    RenderQueueSky.clear();
    
    RenderQueueWorldGeometry.clear();
    RenderQueueGeometryEarly.clear();
    RenderQueueGeometry.clear();
    RenderQueueGeometryLate.clear();
    
    RenderQueueOverlayEarly.clear();
    RenderQueueOverlay.clear();
    RenderQueueForeground.clear();
    
    // Update objects and refresh render lists
    for (std::vector<Entity*>::iterator it = EntityList.begin(); it != EntityList.end(); ++it) {
        
        Entity*EntityPtr = *it;
        int RenderQueueGroup = EntityPtr ->RenderQueueGroup;
        
        // Select a render queue group
        switch (RenderQueueGroup) {
            
            case RENDER_QUEUE_WORLD_GEOMETRY: RenderQueueWorldGeometry.push_back(EntityPtr); break;
            case RENDER_QUEUE_GEOMETRY_EARLY: RenderQueueGeometryEarly.push_back(EntityPtr); break;
            default: 
            case RENDER_QUEUE_GEOMETRY:       RenderQueueGeometry     .push_back(EntityPtr); break;
            case RENDER_QUEUE_GEOMETRY_LATE:  RenderQueueGeometryLate .push_back(EntityPtr); break;
            case RENDER_QUEUE_BACKGROUND:     RenderQueueBackground   .push_back(EntityPtr); break;
            case RENDER_QUEUE_SKY_EARLY:      RenderQueueSkyEarly     .push_back(EntityPtr); break;
            case RENDER_QUEUE_SKY:            RenderQueueSky          .push_back(EntityPtr); break;
            case RENDER_QUEUE_OVERLAY_EARLY:  RenderQueueOverlayEarly .push_back(EntityPtr); break;
            case RENDER_QUEUE_OVERLAY:        RenderQueueOverlay      .push_back(EntityPtr); break;
            case RENDER_QUEUE_FOREGROUND:     RenderQueueForeground   .push_back(EntityPtr); break;
            
        }
        
    }
    
}

void RenderSystem :: RefreshLightQueue(void) {
    
    double RenderDistance;
    POSITION CamPos;
    
    // Get camera position
    if (CurrentCamera != nullptr) {
        
        CamPos = CurrentCamera ->Position;
        
    } else {
        
        CamPos.x=0.0;
        CamPos.y=0.0;
        CamPos.z=0.0;
        
    }
    
    // Compile a new list of lights
    LightQueue.clear();
    
    Light* LightPtr; for (std::vector<Light*>::iterator it = LightList.begin(); it != LightList.end(); ++it) {
        
        LightPtr = *it;
        POSITION LightPos = LightPtr ->Position;
        
        glm::vec3 PointA = glm::vec3( CamPos.x, CamPos.y, CamPos.z);
        glm::vec3 PointB = glm::vec3( LightPos.x, LightPos.y, LightPos.z);
        
        RenderDistance = glm::distance(PointA, PointB);
        
        if (RenderDistance < LightPtr ->RenderDistance) {
            
            // Check light state
            if (LightPtr ->GetState()) {
                
                // Add light to render queue
                LightQueue.push_back(LightPtr);
                
                // Check MAX light count
                if (LightQueue.size() >= MAX_LIGHT_COUNT) {break;}
                
            }
            
        }
        
        
        
    }
    
}

bool RenderSystem :: RefreshAssetBindings(Mesh* MeshPtr) {
    
    // Check current entity`s asset bindings
    if (MeshPtr != nullptr) {
        
        // Compare with the current asset
        if (CurrentMesh != MeshPtr) {
            
            // Check the new asset state
            if (MeshPtr ->IsBufferLoaded) {
                
                // Bind the new asset
                CurrentMesh = MeshPtr;
                
                // Select the vertex attribute object
                glBindVertexArray( CurrentMesh ->BufferAttribute );
                
            }else{return false;}
            
        }
        
    } else {return false;}
    
    if (CurrentMesh ->AttachedMaterial != nullptr) {
        
        // Compare with the current asset
        if (CurrentMaterial != CurrentMesh ->AttachedMaterial) {
            
            // Check the new asset state
            if (CurrentMesh ->AttachedMaterial ->IsTextureLoaded) {
                
                // Bind the new asset
                CurrentMaterial = CurrentMesh ->AttachedMaterial;
                
                // Select the texture data buffer
                glBindTexture(GL_TEXTURE_2D, CurrentMaterial ->BufferTexture);
                
                // Select texture slot
                glActiveTexture( CurrentMaterial ->TextureSlot );
                
                // Update material states
                if (CurrentMaterial ->UsePointLineSize) {
                    
                    glPointSize(CurrentMaterial ->SizePoint);
                    glLineWidth(CurrentMaterial ->SizeLine);
                    
                }
                if (CurrentMaterial ->UseDepthTest) {
                    
                    glEnable(GL_DEPTH_TEST);
                    glDepthFunc(CurrentMaterial ->DepthFunction);
                    
                } else {
                    
                    glDisable(GL_DEPTH_TEST);
                    
                }
                if (CurrentMaterial ->UseBlending) {
                    
                    glEnable(GL_BLEND);
                    
                    glBlendColor(CurrentMaterial ->BlendColor.r, CurrentMaterial ->BlendColor.g, CurrentMaterial ->BlendColor.b, CurrentMaterial ->BlendColor.a);
                    
                    glBlendEquation(CurrentMaterial ->blEquation);
                    
                    if (CurrentMaterial ->UseSeperateAlpha) {
                        
                        glBlendFuncSeparate(CurrentMaterial ->blColorSource, CurrentMaterial ->blColorDest, CurrentMaterial ->blAlphaSource, CurrentMaterial ->blAlphaDest);
                        
                    } else {
                        
                        glBlendFunc(CurrentMaterial ->blSource, CurrentMaterial ->blDest);
                        
                    }
                    
                } else {
                    
                    glDisable(GL_BLEND);
                    
                }
                
            }else{
                
                // Bind the new asset
                CurrentMaterial = CurrentMesh ->AttachedMaterial;
                
                // Clear bindings
                glBindTexture(GL_TEXTURE_2D, 0);
                glActiveTexture( 0 );
                
            }
            
        }
        
    } else {return false;}
    
    if (CurrentMaterial ->AttachedShader != nullptr) {
        
        // Compare with the current asset
        if (CurrentShader != CurrentMaterial ->AttachedShader) {
            
            // Check the new asset state
            if (CurrentMaterial ->AttachedShader ->IsShaderLoaded) {
                
                // Bind the new asset
                CurrentShader = CurrentMaterial ->AttachedShader;
                
                glUseProgram(CurrentShader ->ProgramID);
                
                // Assign texture slots
                int TextureSlot = CurrentMaterial ->TextureSlot - GL_TEXTURE0;
                glUniform1i(CurrentShader ->TextureSamplerLoc, TextureSlot);
                
            }else{return false;}
            
        }
        
        // Update color uniforms
        CurrentShader ->SetColorUniforms(CurrentMaterial ->Ambient, CurrentMaterial ->Diffuse, CurrentMaterial ->Specular);
        CurrentShader ->SetColorShininessUniforms(CurrentMaterial ->Shininess, CurrentMaterial ->SpecularMul);
        
    } else {return false;}
    
    return true;
}


//
// Render the scene (internal timer)
bool RenderSystem :: SceneRender(void) {
    
    //
    // Built in timer
    
    bool RenderThisFrame = false;
    
    // ================================
    // Frame timing
    
    // Query frame timer
    TimeDelta = this ->FrameTimer ->Check(true);
    TimeAccumulator += TimeDelta;
    
    // Calculate remaining time difference
    TimeAlpha = FrameRateMs - TimeDelta;
    
    // Units of movement per second
    UnitsPerSecond = TimeDelta * 1000.0;
    
    // Check accumulated frame time
    if (TimeAccumulator > FrameRateMs) {
        
        // Upper limit
        if (TimeAccumulator > 10.0) {TimeAccumulator = 10.0;}
        
        // Drain the accumulator
        while (TimeAccumulator > 0.0) { TimeAccumulator -= FrameRateMs; }
        
        RenderThisFrame = true;
        
    }
    
    // Frame complete
    if (FrameRenderComplete) {FrameRenderComplete = false;
        
        FrameTime      = TimeDelta;
        FrameRemainder = TimeAlpha;
        
#ifdef  CONSOLE_RENDER_TIMING_
        // DEBUG output
        std::cout << " RenderSystem > " << std::round(FrameRate);
        std::cout << "   " << FrameTime;
        std::cout << "   " << FrameRemainder;
        std::cout << "     " << FrameTime + FrameRemainder;
        std::cout << std::endl;
#endif
        
    }
    
    // Check if we need to drop frames
    if (FrameTime > FrameRateDrop) {
        
        // Drop frames
        if (FrameRate > FrameRateMin) {FrameRate -= FrameRateDec;}
        
        FrameRateMs     = 1000.0 / FrameRate;
        FrameRateDrop   = FrameRateMs / FrameDropPoint;
        
    } else {
        
        // Restore frames
        if (FrameRate < FrameRateMax) {FrameRate += FrameRateInc;}
        
        FrameRateMs     = 1000.0 / FrameRate;
        FrameRateDrop   = FrameRateMs / FrameDropPoint;
    }
    
    
    // ================================
    // Render
    
    // Check whether to render the frame
    if (!RenderThisFrame) return false;
    
    // Begin the render pipeline
    this ->RenderPipeline();
    
#ifdef  LOG_GL_ERRORS_
    RenderLibrary::LogGLErrors("RenderSystem");
#endif
    
    return true;
}


//
// Render pipeline

void RenderSystem :: RenderPipeline(void) {
    
    int DrawCallCount = 0;
    glm::vec3 PointA = glm::vec3(0.0, 0.0, 0.0);
    
    // Set the camera projection
    glm::mat4 ProjectionMatrix = this ->SetProjectionByCamera();
    
    // Frame skipping (for low level updates)
    FrameCounterA++; if (FrameCounterA > 30) {FrameCounterA=0;
        
        // Update light list
        this ->RefreshLightQueue();
        
    }
    FrameCounterB++; if (FrameCounterB > 60) {FrameCounterB=0;
        
        
    }
    FrameCounterC++; if (FrameCounterC > 120) {FrameCounterC=0;
        
        
        // Update sky to cameras position
        if (CurrentSky != nullptr) {
            
            if (CurrentCamera != nullptr) {
                
                CurrentSky ->Position.x = CurrentCamera ->Position.x;
                CurrentSky ->Position.y = CurrentCamera ->Position.y;
                CurrentSky ->Position.z = CurrentCamera ->Position.z;
                
            }
            
        }
        
    }
    
    // Check to clear the frame
    if (DoClearFrame) {
        
        // Clear background to a specific color
        glClearColor(Background.r, Background.g, Background.b, 1.0f);
        
        // Clear buffer bits
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
    }
    
    // Check camera state
    if (CurrentCamera != nullptr) {
        
        // Update the current camera
        CurrentCamera ->Update();
        
        // Get camera position
        PointA = glm::vec3(CurrentCamera ->Position.x, CurrentCamera ->Position.z, CurrentCamera ->Position.y);
        
    } else {
        
        // Default camera position
        PointA = glm::vec3(0.0, 0.0, 0.0);
        
    }
    
    
    //
    // Loop through the render queue groups
    for (int i=0; i < 10; i++) {
        
        // Clear the depth buffer before the next queue is drawn
        glClear(GL_DEPTH_BUFFER_BIT);
        
        // Get the pointer to the next queue group
        std::vector<Entity*> ListPtr = *QueueList[i];
        
        // Render the entities in the queue
        for (std::vector<Entity*>::iterator it = ListPtr.begin(); it != ListPtr.end(); ++it) {
            
            Entity* CurrentEntity = *it;
            
            // Get current entity position
            glm::vec3 PointB = glm::vec3(CurrentEntity ->Position.x, CurrentEntity ->Position.z, CurrentEntity ->Position.y);
            
            // Check entity distance from camera
            double CurrentDistance = glm::distance( PointB, PointA);
            
            if (CurrentEntity ->RenderDistance > CurrentDistance) {
                
                //
                // Render the entity
                
                this ->RenderEntity( CurrentEntity, ProjectionMatrix, DrawCallCount );
                
            } else {
                
                //
                // Deactivate the entity
                
                if (CurrentEntity ->IsActive) {
                    
                    CurrentEntity ->IsActive = false;
                    
                    CurrentEntity ->Deactivate();
                    
                }
                
            }
            
            
        }
        
    }
    
    // Turn over the frame buffer
    SwapBuffers(DeviceContext);
    
    FrameRenderComplete = true;
    
}
void RenderSystem :: RenderEntity(Entity* CurrentEntity, glm::mat4 &ProjectionMatrix, int &DrawCallCount ) {
    
    //
    // Entity within range
    
    // Update the entity
    CurrentEntity ->Update();
    
    // Check entity state
    if (!CurrentEntity ->IsRendered) {return;}
    
    if (!CurrentEntity ->IsActive) {
        
        CurrentEntity ->Activate();
        
        CurrentEntity ->IsActive = true;
        
    }
    
    
    //
    // Check current entity`s asset bindings
    if (!RefreshAssetBindings(CurrentEntity ->AttachedMesh)) return;
    
    
    // Check perspective type
    if (CurrentEntity ->IsOverlay) {
        
        // Set orthographic projection
        float  vpFov    =   60.0;
        float  vpAspect =    1.33;
        double vpNear   =    1.0;
        double vpFar    = 1000.0;
        
        //
        // No camera, provide default projection
        POSITION CamPos = CurrentCamera ->Position;
        
        ROTATION CamAngle;
        CamAngle.y = CamPos.x + cos( DegToRad(   0.0 ) );
        CamAngle.p = CamPos.y + sin( DegToRad(   0.0 ) );
        CamAngle.r = CamPos.z + tan( DegToRad(   0.0 ) );
        
        // Calculate the view and projection matrices
        glm::mat4 OrthographicMatrix = CalculateOrthographicMatrix(CamPos, CamAngle, ViewPort.w, ViewPort.h);
        
        CurrentShader ->SetProjectionMatrix( OrthographicMatrix );
        
    } else {
        
        // Set perspective projection
        CurrentShader ->SetProjectionMatrix(ProjectionMatrix);
        
    }
    
    // Check camera auto tracking
    if (CurrentEntity ->UseAutoTrack) {
        
        // Auto track the cameras position
        CurrentEntity ->Rotation.y = AngleBetweenPoints(CurrentEntity ->Position, CurrentCamera ->Position);
        
    }
    
    // Check material lighting
    if (CurrentMaterial ->UseLighting) {
        
        // Use the current light list
        CurrentShader ->SetLightQueue(LightQueue, CurrentCamera ->Position);
        
    } else {
        
        // Create a dummy light list to cancel lighting
        std::vector<Light*> DummyList;
        
        CurrentShader ->SetLightQueue(DummyList,  CurrentCamera ->Position);
        
    }
    
    // Check if entity is not static
    if (!CurrentEntity ->IsStatic) {
        
        // Dynamic entities update every frame
        // Static entities will update rarely
        
        glm::mat4 ModelMatrix_;
        glm::mat4 NormalMatrix_;
        
        // Calculate the model matrix
        ModelMatrix_  = CalculateModelMatrix(CurrentEntity ->Position, CurrentEntity ->Rotation, CurrentEntity ->Scale, CurrentEntity ->PositionOffset, CurrentEntity ->RotationOffset);
        
        // Calculate the normal matrix
        if ((CurrentEntity ->Scale.x == CurrentEntity ->Scale.y) && (CurrentEntity ->Scale.y == CurrentEntity ->Scale.z)) {
            
            NormalMatrix_ = CurrentEntity ->ModelMatrix;
            
        } else {
            
            NormalMatrix_ = glm::transpose( glm::inverse( CurrentEntity ->ModelMatrix ) );
            
        }
        
        // Set the current model matrix
        CurrentEntity ->ModelMatrix  = ModelMatrix_;
        CurrentEntity ->NormalMatrix = NormalMatrix_;
        
        // Send the matrix into the shader
        CurrentShader ->SetModelMatrix(ModelMatrix_);
        CurrentShader ->SetNormalMatrix(NormalMatrix_);
        
    }
    
    
    //
    // Make the draw call
    this ->RenderDrawCall( DrawCallCount );
    
}

//
// Rasterisation calls
void RenderSystem :: RenderDrawCall(int &DrawCallCount) {
    
    // Check buffer indexing before the draw call is made
    if (CurrentMesh ->IsBufferIndexed) {
        
        // Draw the indexed locations referencing the vertex buffer
        RenderLibrary::DrawBufferIndex( CurrentMesh ->PrimitiveType, CurrentMesh ->IndexCount );
        
    } else {
        
        // No indexing, draw the raw vertex buffer (slower)
        RenderLibrary::DrawBufferVertex( CurrentMesh ->PrimitiveType, CurrentMesh ->VertexCount );
        
    }
    
    DrawCallCount++;
    
}

// Set the projection for rendering
glm::mat4 RenderSystem :: SetProjectionByCamera(void) {
    
    POSITION CamPos;
    ROTATION CamAngle;
    ROTATION Forward;
    
    // Update current camera
    if (CurrentCamera != nullptr) {
        
        if (CurrentCamera ->UseMouseLook) {CurrentCamera ->MouseLook();}
        
        Physics::Vector3 Position = Physics::Vector3(0.0, 0.0, 0.0);
        
        if (CurrentCamera ->AttachedBody != nullptr) {
            
            Physics::Transform CameraTransform = CurrentCamera ->AttachedBody ->getTransform();
            Position = CameraTransform.getPosition();
            
        } else {
            
            Position = Physics::Vector3(CurrentCamera ->Position.x,
                                        CurrentCamera ->Position.y,
                                        CurrentCamera ->Position.z);
            
        }
        
        // Get camera world height
        double CamHeight = Position.z + CurrentCamera ->CameraHeight;
        
        // Camera position
        CamPos.x = Position.x;
        CamPos.y = CamHeight;
        CamPos.z = Position.y;
        
        // Set cameras forward angle
        Forward.x = cos( DegToRad( CurrentCamera ->Rotation.x ) );
        Forward.z = tan( DegToRad( CurrentCamera ->Rotation.y ) );
        Forward.y = sin( DegToRad( CurrentCamera ->Rotation.x ) );
        
        // Calculate camera angle
        CamAngle.x = Position.x + Forward.x;
        CamAngle.z = CamHeight  + Forward.z;
        CamAngle.y = Position.y + Forward.y;
        
        CurrentCamera ->Forward.x = Forward.x;
        CurrentCamera ->Forward.y = Forward.y;
        CurrentCamera ->Forward.z = Forward.z;
        
        // Check projection mode
        if (this ->UsePerspective) {
            
            // Calculate the view and projection matrices
            return CalculatePerspectiveMatrix(CamPos,
                                              CamAngle,
                                              CurrentCamera ->vpFoV,
                                              CurrentCamera ->vpAspect,
                                              CurrentCamera ->vpNear,
                                              CurrentCamera ->vpFar);
            
        } else {
            
            // Calculate the view and projection matrices
            return CalculateOrthographicMatrix(CamPos,
                                               CamAngle,
                                               Renderer ->GetViewportWidth(),
                                               Renderer ->GetViewportHeight());
            
        }
        
    }
    
    //
    // No camera, provide default projection
    CamPos.x = 0.0;
    CamPos.y = 0.0;
    CamPos.z = 0.0;
    
    CamAngle.x = CamPos.x + cos( DegToRad( 0.0 ) );
    CamAngle.y = CamPos.y + sin( DegToRad( 0.0 ) );
    CamAngle.z = CamPos.z + tan( DegToRad( 0.0 ) );
    
    float  vpFov    = 60.0;
    float  vpAspect = 1.33;
    double vpNear   = 1.0;
    double vpFar    = 10000.0;
    
    // Calculate the view and projection matrices
    glm::mat4 ProjectionMatrix = CalculatePerspectiveMatrix(CamPos, CamAngle, vpFov, vpAspect, vpNear, vpFar);
    
    return ProjectionMatrix;
}

// Matrix functions
glm::mat4 RenderSystem :: CalculateModelMatrix(POSITION &PositionWorld, ROTATION &RotationWorld, SCALE &ScaleWorld, POSITION &PositionModel, ROTATION &RotationModel) {
    
    // World translation
    glm::mat4 WorldTranslation = glm::translate(glm::mat4(1.0f), glm::vec3( PositionWorld.x, PositionWorld.z, PositionWorld.y ));
    
    // World rotation
    glm::mat4 
    WorldRotation = glm::rotate(glm::mat4(1.0f), glm::radians( 0.0f ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
    WorldRotation = glm::rotate(WorldRotation, RotationWorld.x, glm::vec3( 0.0f, 1.0f, 0.0f ) );
    WorldRotation = glm::rotate(WorldRotation, RotationWorld.y, glm::vec3( 1.0f, 0.0f, 0.0f ) );
    WorldRotation = glm::rotate(WorldRotation, RotationWorld.z, glm::vec3( 0.0f, 0.0f, 1.0f ) );
    
    // World scale
    glm::mat4 WorldScale = glm::scale(glm::mat4(1.0f), glm::vec3( ScaleWorld.x, ScaleWorld.y, ScaleWorld.z ));
    
    // Model translation
    glm::mat4 ModelTranslation = glm::translate(glm::mat4(1.0f), glm::vec3( PositionModel.x, PositionModel.z, PositionModel.y ));
    
    // Model rotation
    glm::mat4 
    ModelRotation = glm::rotate(glm::mat4(1.0f), glm::radians( 0.0f ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
    ModelRotation = glm::rotate(ModelRotation, RotationModel.x, glm::vec3( 0.0f, 1.0f, 0.0f ) );
    ModelRotation = glm::rotate(ModelRotation, RotationModel.y, glm::vec3( 1.0f, 0.0f, 0.0f ) );
    ModelRotation = glm::rotate(ModelRotation, RotationModel.z, glm::vec3( 0.0f, 0.0f, 1.0f ) );
    
    return (WorldTranslation * WorldScale * WorldRotation) * (ModelTranslation * ModelRotation);
    
}
glm::mat4 RenderSystem :: CalculatePerspectiveMatrix(POSITION &Position, ROTATION &Angle, float &vpFoV, float &vpAspect, double &vpNear, double &vpFar) {
    
    glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
    
    glm::mat4 View = glm::lookAt(glm::vec3(Position.x, Position.y, Position.z), glm::vec3(Angle.x, Angle.z, Angle.y), Up);
    glm::mat4 Proj = glm::perspective(glm::radians( (double)vpFoV ), (double)vpAspect, vpNear, vpFar);
    
    glm::mat4 ViewProj = (Proj * View);
    
    return ViewProj;
}
glm::mat4 RenderSystem :: CalculateOrthographicMatrix(POSITION &Position, ROTATION &Angle, int Width, int Height) {
    
    glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
    
    glm::mat4 View = glm::lookAt(glm::vec3(Position.x, Position.y, Position.z), glm::vec3(Angle.x, Angle.y, Angle.z), Up);
    glm::mat4 Ortho = glm::ortho(0.0f, (float)Width,
                                 0.0f, (float)Height);
    
    glm::mat4 ViewProj = (Ortho * View);
    return ViewProj;
}

