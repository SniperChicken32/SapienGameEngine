//
// Shader object

struct Shader {
    
    std::string Name;
    std::string ShaderLanguage;
    
    bool IsShaderLoaded;
    
    unsigned int ProgramID;
    
    // Uniform name locations
    int UniformLocation[32];
    
    // Internal uniform locations
    int ViewProjMatrixLoc;
    int ModelMatrixLoc;
    int NormalMatrixLoc;
    
    // Texture sampler location
    int TextureSamplerLoc;
    
    // Material locations
    int AmbientColorLoc;
    int DiffuseColorLoc;
    int SpecularColorLoc;
    int ShininessLoc;
    int SpecularMulLoc;
    
    // Camera position location
    int CameraPosLoc;
    
    // Light count location
    int LightCountLoc;
    
    // Light list
    int LightCount;
    int LightPosLoc   [MAX_LIGHT_COUNT];
    int LightDirLoc   [MAX_LIGHT_COUNT];
    int LightDiffLoc  [MAX_LIGHT_COUNT];
    int LightSpecLoc  [MAX_LIGHT_COUNT];
    int LightAttenLoc [MAX_LIGHT_COUNT];
    int LightFocusLoc [MAX_LIGHT_COUNT];
    
    Shader();
    ~Shader();
    
    // Set uniform location name
    int  SetUniformName(int Pos, std::string UniformName)  {UniformLocation[Pos] = glGetUniformLocation(ProgramID, UniformName.c_str()); return UniformLocation[Pos];}
    
    // Uniform integers
    void  SetUniform1i(int Pos, int i1)                                {glUniform1i(UniformLocation[Pos], i1);}
    void  SetUniform2i(int Pos, int i1, int   i2)                      {glUniform2i(UniformLocation[Pos], i1, i2);}
    void  SetUniform3i(int Pos, int i1, int   i2, int   i3)            {glUniform3i(UniformLocation[Pos], i1, i2, i3);}
    void  SetUniform4i(int Pos, int i1, int   i2, int   i3, int   i4)  {glUniform4i(UniformLocation[Pos], i1, i2, i3, i4);}
    
    // Uniform unsigned integers
    void  SetUniform1ui(int Pos, unsigned int i1)                                                           {glUniform1ui(UniformLocation[Pos], i1);}
    void  SetUniform2ui(int Pos, unsigned int i1, unsigned int   i2)                                        {glUniform2ui(UniformLocation[Pos], i1, i2);}
    void  SetUniform3ui(int Pos, unsigned int i1, unsigned int   i2, unsigned int   i3)                     {glUniform3ui(UniformLocation[Pos], i1, i2, i3);}
    void  SetUniform4ui(int Pos, unsigned int i1, unsigned int   i2, unsigned int   i3, unsigned int   i4)  {glUniform4ui(UniformLocation[Pos], i1, i2, i3, i4);}
    
    // Uniform floats
    void  SetUniform1f(int Pos, float f1)                                {glUniform1f(UniformLocation[Pos], f1);}
    void  SetUniform2f(int Pos, float f1, float f2)                      {glUniform2f(UniformLocation[Pos], f1, f2);}
    void  SetUniform3f(int Pos, float f1, float f2, float f3)            {glUniform3f(UniformLocation[Pos], f1, f2, f3);}
    void  SetUniform4f(int Pos, float f1, float f2, float f3, float f4)  {glUniform4f(UniformLocation[Pos], f1, f2, f3, f4);}
    
    // Uniform matrix of floats
    void  SetMatrix2f(int Pos, glm::mat2 Matrix)  {glUniformMatrix2fv(UniformLocation[Pos],1, 0, &Matrix[0][0]);}
    void  SetMatrix3f(int Pos, glm::mat3 Matrix)  {glUniformMatrix3fv(UniformLocation[Pos],1, 0, &Matrix[0][0]);}
    void  SetMatrix4f(int Pos, glm::mat4 Matrix)  {glUniformMatrix4fv(UniformLocation[Pos],1, 0, &Matrix[0][0]);}
    
    //
    // Pipeline uniforms
    
    // Matrices
    void  SetProjectionMatrix(glm::mat4&);
    void  SetModelMatrix(glm::mat4&);
    void  SetNormalMatrix(glm::mat4&);
    
    // Color
    void  SetColorUniforms(COLOR, COLOR, COLOR);
    void  SetColorShininessUniforms(float, float);
    
    // Set the light list and camera position
    void  SetLightQueue(std::vector<Light*>&, POSITION&);
    
};

Shader :: Shader() {
    
    Name           = "";
    ShaderLanguage = "";
    
    IsShaderLoaded = false;
    
    ProgramID = 0;
    
    for (int i=0; i < 32; i++) {UniformLocation[i] = 0;}
    
    ViewProjMatrixLoc=0;
    ModelMatrixLoc=0;
    NormalMatrixLoc=0;
    TextureSamplerLoc=0;
    AmbientColorLoc=0;
    DiffuseColorLoc=0;
    SpecularColorLoc=0;
    ShininessLoc=0;
    CameraPosLoc=0;
    LightCountLoc=0;
    
}
Shader :: ~Shader() {
    
    if (IsShaderLoaded) {
        
        glDeleteProgram( ProgramID );
        
    }
    
}

void Shader :: SetModelMatrix(glm::mat4 &ModelMatrix) {
    
    glUniformMatrix4fv(ModelMatrixLoc, 1, 0, &ModelMatrix[0][0]);
    
}

void Shader :: SetNormalMatrix(glm::mat4 &NormalMatrix) {
    
    glUniformMatrix4fv(NormalMatrixLoc, 1, 0, &NormalMatrix[0][0]);
    
}

void Shader :: SetProjectionMatrix(glm::mat4 &ProjectionMatrix) {
    
    glUniformMatrix4fv(ViewProjMatrixLoc, 1, 0, &ProjectionMatrix[0][0]);
    
}

void Shader :: SetLightQueue(std::vector<Light*> &LightList, POSITION &CameraPos) {
    
    int i=0;
    POSITION Position;
    ROTATION Direction;
    COLOR LightDiffuse;
    COLOR LightSpecular;
    
    // Set camera position
    glUniform3f(CameraPosLoc, CameraPos.x, CameraPos.z, CameraPos.y);
    
    // Send in the lights
    Light* LightPtr;
    for (std::vector<Light*>::iterator it = LightList.begin(); it != LightList.end(); ++it) {
        
        LightPtr = *it;
        
        // Get light data
        Position  = LightPtr ->Position;
        Direction = LightPtr ->Rotation;
        LightDiffuse  = LightPtr ->Diffuse;
        LightSpecular = LightPtr ->Specular;
        
        // Position and orientation
        glUniform3f(LightPosLoc[i], Position.x,  Position.z,  Position.y);
        glUniform3f(LightDirLoc[i], Direction.y, Direction.p, Direction.r);
        
        // Color
        glUniform3f(LightDiffLoc  [i],  LightDiffuse.r,  LightDiffuse.g,  LightDiffuse.b);
        glUniform3f(LightSpecLoc  [i],  LightSpecular.r, LightSpecular.g, LightSpecular.b);
        
        // Attenuation
        glUniform4f(LightAttenLoc [i], LightPtr ->Range, LightPtr ->Constant, LightPtr ->Linear, LightPtr ->Quadratic);
        
        // Focus
        glUniform1f(LightFocusLoc [i], LightPtr ->Focus);
        
        i++;
        if (i >= MAX_LIGHT_COUNT) {break;}
    }
    
    // Number of lights
    glUniform1i(LightCountLoc, i-1);
    
}

void Shader :: SetColorUniforms(COLOR Ambient, COLOR Diffuse, COLOR Specular) {
    
    glUniform3f(AmbientColorLoc,  Ambient.r,  Ambient.g,  Ambient.b);
    glUniform3f(DiffuseColorLoc,  Diffuse.r,  Diffuse.g,  Diffuse.b);
    glUniform3f(SpecularColorLoc, Specular.r, Specular.g, Specular.b);
    
}

void Shader :: SetColorShininessUniforms(float Shininess, float SpecularMul) {
    
    glUniform1f(ShininessLoc, Shininess);
    glUniform1f(SpecularMulLoc, SpecularMul);
    
}

