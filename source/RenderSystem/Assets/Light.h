//
// Light object

struct Light {
    
    std::string Name;
    std::string OriginalName;
    
    int LightType;
    
    bool IsActive;
    
    COLOR Diffuse;
    COLOR Specular;
	
	double RenderDistance;
	
	float Range;
    float Intensity;
    
    float Constant;
    float Linear;
    float Quadratic;
    float Focus;
    
    POSITION Position;
    ROTATION Rotation;
    
    Light();
    ~Light();
    
    // Asset naming
    void         SetAssetName(std::string NewName) {Name = NewName;}
    std::string  GetAssetName(void) {return Name;}
    
    // Activate / deactivate
    void  SetState(bool State) {IsActive = State;}
    bool  GetState(void) {return IsActive;}
    
    // Light type
    void  SetLightType(int Type) {if (Type >= 0) {LightType = Type;}}
    
    // Position
    void      SetPosition(POSITION Pos) {Position.x = Pos.x; Position.y = Pos.y; Position.z = Pos.z;}
    void      SetPosition(double x, double y, double z) {Position.x = x; Position.y = y; Position.z = z;}
    POSITION  GetPosition(void) {return Position;}
    
    // Orientation
    void      SetRotation(float y, float p, float r) {Rotation.x = y; Rotation.y = p; Rotation.z = r;}
    ROTATION  GetRotation(void) {return Rotation;}
    
    // Render distance
    void    SetRenderDistance(double Dist) {RenderDistance = Dist;}
    double  GetRenderDistance(void) {return RenderDistance;}
    
    // Color
    void   SetDiffuse(float red, float green, float blue) {Diffuse.x  = red; Diffuse.y  = green; Diffuse.z  = blue;}
    void   SetSpecular(float red, float green, float blue) {Specular.x = red; Specular.y = green; Specular.z = blue;}
    COLOR  GetDiffuse(void) {return Diffuse;}
    COLOR  GetSpecular(void) {return Specular;}
    
    // Intensity & focus
    void   SetFocus(float focus) {Focus = focus;}
    float  GetFocus(void)        {return Focus;}
    void   SetIntensity(float intensity) {
        
        Intensity = intensity;
        
        // Initiate intensity values
        Constant  = 0.01;
        Linear    = 0.8;
        Quadratic = 0.5;
        
        // Calculate intensity
        Constant   -= (Intensity * 0.87);
        Linear     -= (Intensity * 0.87);
        Quadratic  -= (Intensity * 0.087);
        
        // Cap lower limits
        if (Constant  < 0.0) {Constant  = 0.0;}
        if (Linear    < 0.0) {Linear    = 0.0;}
        if (Quadratic < 0.0) {Quadratic = 0.0;}
        
    }
    float  GetIntensity(void)            {return Intensity;}
    
    // Attenuation
    void  SetAttenuation(float range, float constant, float linear, float quadratic) {Range = range; Constant = constant; Linear = linear; Quadratic = quadratic;}
    void  SetRange(float range) {Range = range;}
    void  SetConst(float constant) {Constant = constant;}
    void  SetLinear(float linear) {Linear = linear;}
    void  SetQuad(float quadratic) {Quadratic = quadratic;}
    float  GetRange(void) {return Range;}
    float  GetConstant(void) {return Constant;}
    float  GetLinear(void) {return Linear;}
    float  GetQuad(void) {return Quadratic;}
    
};

Light :: Light() {
    
    IsActive = false;
    Name = "";
    OriginalName = "";
    RenderDistance = 200.0;
    
    // Type of light
    // 0 = point  1 = directional  2 = spot
    LightType = 0;
    
    // Diffuse color
    Diffuse = glm::vec3(0.87, 0.87, 0.87);
    
    // Specular color
    Specular = glm::vec3(0.0, 0.0, 0.0);
    
	// Attenuation
    Range     = 15.0;
    Intensity =  0.1;
    Focus     =  2.0;
    
    // Initiate intensity values
    Constant  = 0.01;
    Linear    = 0.8;
    Quadratic = 0.5;
    
    // Position offset & direction
    Position = glm::vec3(0.0, 0.0, 0.0);
    Rotation = glm::vec3(0.0, 0.0, 0.0);
    
    this ->SetIntensity( Intensity );
    
}
Light :: ~Light() {
    
}

