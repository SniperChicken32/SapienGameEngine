//
// Material object

struct Material {
    
    std::string Name;
    
    bool IsTextureLoaded;
    bool IsMaterialLoaded;
    bool UseDepthTest;
    bool UseLighting;
    bool UseBlending;
    bool UseSeperateAlpha;
    bool UsePointLineSize;
    
    unsigned int BufferTexture;
    
    int Width, Height, Channels;
    
    COLOR Ambient;
    COLOR Diffuse;
    COLOR Specular;
    float Shininess;
    float SpecularMul;
    
    int DepthFunction;
    
    int TextureFilter;
    float AnisotropyLevel;
    
    int TextureParameter;
    int TextureSlot;
    
    COLORA BlendColor;
    GLenum blSource, blDest;
    GLenum blEquation;
    GLenum blColorSource, blAlphaSource, blColorDest, blAlphaDest;
    
    float SizePoint;
    float SizeLine;
    
    unsigned int ShaderID;
    
    Shader* AttachedShader;
    
    Material();
    ~Material();
    
    // Attach shader
    bool  AttachShader         (Shader*);
    void  DetachShader         (void);
    
    // Colors
    void  SetAmbientColor(COLOR ambient) {Ambient.r  = ambient.r;  Ambient.g  = ambient.g;  Ambient.b  = ambient.b;}
    void  SetDiffuseColor(COLOR diffuse) {Diffuse.r  = diffuse.r;  Diffuse.g  = diffuse.g;  Diffuse.b  = diffuse.b;}
    void  SetSpecularColor(COLOR specular) {Specular.r = specular.r; Specular.g = specular.g; Specular.b = specular.b;}
    void  SetShininess(float shininess) {Shininess  = shininess;}
    
    // Settings
    void  SetDepthFunction(GLenum DepthFunc) {DepthFunction = DepthFunc;}
    void  EnableDepthTesting(bool State)  {if (State == true) {UseDepthTest = true;} else {UseDepthTest = false;}}
    void  EnableLighting(bool State)  {if (State == true) {UseLighting  = true;} else {UseLighting = false;}}
    void  EnableBlending(bool State)  {if (State == true) {UseBlending  = true;} else {UseBlending = false;}}
    void  SetBlendColor(float rr, float gg, float bb, float aa) {BlendColor.r = rr; BlendColor.g = gg; BlendColor.b = bb; BlendColor.a = aa;}
    void  SetBlendEquation(GLenum equ) {blEquation = equ;}
    void  SetBlendFunction(GLenum src, GLenum dst) {UseSeperateAlpha = false; blSource = src; blDest = dst;}
    void  SetBlendFunctionSep(GLenum srcColor, GLenum srcAlpha, GLenum dstColor, GLenum dstAlpha) {UseSeperateAlpha = true; blColorSource = srcColor; blColorDest = dstColor; blAlphaSource = srcAlpha; blAlphaDest = dstAlpha;}
    void  SetEdgeFunction(GLenum Func) {TextureParameter = Func;}
    
};

Material :: Material() {
    
    // Asset name
    Name                 = "";
    
    // States
    IsTextureLoaded      = false;
    IsMaterialLoaded     = false;
    UseDepthTest         = true;
    UseLighting          = true;
    UseBlending          = false;
    UseSeperateAlpha     = false;
    UsePointLineSize     = false;
    
    // Texture data buffer
    BufferTexture        = 0;
    
    // Image variables
    Width                = 0;
    Height               = 0;
    Channels             = 0;
    
    // Colors
    Ambient = glm::vec3(0.01, 0.01, 0.01);
    Diffuse = glm::vec3(0.87, 0.87, 0.87);
    Specular = glm::vec3(0.12, 0.12, 0.12);
    
    // Material effects
    Shininess   = 8.0;
    SpecularMul = 1.0;
    
    // Depth testing
    DepthFunction      = MATERIAL_DEPTH_LESS;
    
    TextureFilter      = 0;
    AnisotropyLevel    = 0.0;
    
    // Texture wrap and slot ID
    TextureParameter   = MATERIAL_WRAP_REPEAT;
    TextureSlot        = GL_TEXTURE0;
    
    // Blending
    BlendColor = glm::vec4(0.0, 0.0, 0.0, 0.0);
    
    blSource        = BLEND_SRC_ALPHA;
    blDest          = BLEND_ONE_MINUS_SRC_ALPHA;
    blEquation      = BLEND_EQUATION_ADD;
    
    blColorSource   = BLEND_SRC_COLOR;
    blAlphaSource   = BLEND_SRC_ALPHA;
    blColorDest     = BLEND_ONE_MINUS_SRC_COLOR;
    blAlphaDest     = BLEND_ONE_MINUS_SRC_ALPHA;
    
    // Point / line size
    SizePoint       = 5.0;
    SizeLine        = 3.0;
    
    ShaderID = 0;
    
    AttachedShader = nullptr;
    
}
Material :: ~Material() {
    
    if (IsTextureLoaded == true) {glDeleteTextures(1, &BufferTexture);}
    
}

bool Material :: AttachShader(Shader* ShaderPtr) {
    if (ShaderPtr == nullptr) {return false;}
    AttachedShader = ShaderPtr;
    return true;
}
void Material :: DetachShader(void) {
    AttachedShader = nullptr;
}


/*
bool Material :: LoadAsset(std::string FilePath) {
    
    int i=0;
    std::string Line;
    std::ifstream FileStream;
    std::vector<std::string> FileData;
    
    int Width=0;
    int Height=0;
    int Channels=0;
    
    IsMaterialLoaded = false;
    IsTextureLoaded = false;
    
    std::string TexturePath = "";
    
    // Get asset name
    AssetName = AssetFilePeekHeader(FilePath);
    if (AssetName == "") {
        
        LogWrite(strings::LogStringAdd + strings::AssetTypeMaterial + AssetName + strings::SpaceSingle + strings::MissingCorrupt);
        
        return false;
    }
    
    FileStream.open(FilePath);
    
    if (FileStream.is_open()) {
        
#ifdef  DEVELOPMENT_MODE_
        LogWrite(strings::LogStringAdd + strings::AssetTypeMaterial + AssetName);
#endif
        
        while (getline(FileStream, Line)) {
            
            if (Line == "") {continue;}
            
            if (StringFind("//", Line)) {continue;}
            
            // Explode the string
            int i=0; std::string Strings[50];
            std::vector<std::string> Array = StringExplode(Line, ' ');
            for (std::vector<std::string>::iterator ita = Array.begin(); ita != Array.end(); ++ita) 
            {std::string String = *ita; Strings[i] = String; i++;}
            
            // Check enable/disable
            if (Strings[0] == "enable") {
                
                if (Strings[1] == "depth_test") {UseDepthTest = true; continue;}
                if (Strings[1] == "lighting")   {UseLighting  = true; continue;}
                if (Strings[1] == "blending")   {UseBlending  = true; continue;}
                
            }
            
            if (Strings[0] == "disable") {
                
                if (Strings[1] == "depth_test") {UseDepthTest = false; continue;}
                if (Strings[1] == "lighting")   {UseLighting  = false; continue;}
                if (Strings[1] == "blending")   {UseBlending  = false; continue;}
                
            }
            
            // Color
            if (Strings[0] == "ambient")  {Ambient.r = StringToFloat(Strings[1]); Ambient.g = StringToFloat(Strings[2]); Ambient.b = StringToFloat(Strings[3]);continue;}
            if (Strings[0] == "diffuse")  {Diffuse.r = StringToFloat(Strings[1]); Diffuse.g = StringToFloat(Strings[2]); Diffuse.b = StringToFloat(Strings[3]);continue;}
            if (Strings[0] == "specular") {Specular.r = StringToFloat(Strings[1]); Specular.g = StringToFloat(Strings[2]); Specular.b = StringToFloat(Strings[3]);continue;}
            
            // Specular intensity
            if (Strings[0] == "shininess") {Shininess = StringToFloat(Strings[1]);continue;}
            if (Strings[0] == "specularmul") {SpecularMul = StringToFloat(Strings[1]);continue;}
            
            // Texture filtering
            if (Strings[0] == "filter") {
                
                if (Strings[1] == "none")      {TextureFilter = 0;continue;}
                if (Strings[1] == "linear")    {TextureFilter = 1;continue;}
                if (Strings[1] == "bilinear")  {TextureFilter = 2;continue;}
                if (Strings[1] == "aniso")     {TextureFilter = 3;continue;}
                
                continue;
            }
            
            if (Strings[0] == "blend") {
                
                if (Strings[1] == "color") {
                    BlendColor.r = StringToFloat(Strings[2]); BlendColor.g = StringToFloat(Strings[3]);
                    BlendColor.b = StringToFloat(Strings[4]); BlendColor.a = StringToFloat(Strings[5]);
                    continue;
                }
                
                if (Strings[1] == "equation") {blEquation = StringToInt(Strings[2]);continue;}
                
                if (Strings[1] == "function") {
                    UseSeperateAlpha = false;
                    blSource = StringToInt(Strings[2]);
                    blDest   = StringToInt(Strings[3]);
                    continue;
                }
                
                if (Strings[1] == "function_separate") {
                    UseSeperateAlpha = true;
                    blColorSource = StringToInt(Strings[2]);
                    blAlphaSource = StringToInt(Strings[3]);
                    blColorDest   = StringToInt(Strings[4]);
                    blAlphaDest   = StringToInt(Strings[5]);
                    continue;
                }
                
            }
            
            // Texture
            if (Strings[0] == "texture") {
                
                TexturePath = Strings[1];
                
                continue;
            }
            
        }
        
        FileStream.close();
    } else {
        
        LogWrite(strings::LogStringError + strings::AssetTypeMaterial + FilePath);
        
        return false;
    }
    
    //
    // Load texture
    unsigned char* TextureData = nullptr;
    TextureData = stbi_load(TexturePath.c_str(), &Width, &Height, &Channels, 3);
    
    // Check texture loaded
    if (TextureData == nullptr) {
        
        LogWrite(strings::SpaceDouble + strings::LogStringError + TexturePath);
        LogWriteLn();
        
        return false;
    }
    
    // Check if a texture was already loaded
    if (IsTextureLoaded) {
        glDeleteTextures(1, &BufferTexture);
        IsTextureLoaded = false;
    }
    
    // Generate texture
    BufferTexture = RenderLibrary::CreateTextureBuffer();
    
    switch (TextureFilter) {
        
        case MATERIAL_FILTER_ANISOTROPIC:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            
            glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &AnisotropyLevel);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, AnisotropyLevel);
            break;
            
        case MATERIAL_FILTER_TRILINEAR:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            break;
            
        case MATERIAL_FILTER_LINEAR:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            break;
            
        default: 
        case MATERIAL_FILTER_NONE:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            break;
        
    }
    
    // Texture wrap
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, TextureParameter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, TextureParameter);
    
    // Load the texture data
    RenderLibrary::LoadBufferTexture(BufferTexture, TextureData, Width, Height);
    
    // Generate mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);
    
    // Free the image data
    stbi_image_free(TextureData);
    
#ifdef  LOG_GL_ERRORS_
    RenderLibrary::LogGLErrors(strings::AssetTypeMaterial);
#endif
    
    IsMaterialLoaded = true;
    IsTextureLoaded = true;
    return true;
}

*/
