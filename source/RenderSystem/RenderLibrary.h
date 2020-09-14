//
// OpenGL base library

#ifndef  GL_BASE_LIBRARY__
#define  GL_BASE_LIBRARY__

namespace RenderLibrary {
    
    // Vertex buffer byte offsets
    // 
    //  Position    0  - 11
    //  Normal      12 - 23
    //  Texture     24 - 32
    struct VERTEX {float x, y, z, nx, ny, nz, u, v;};
    struct INDEX  {unsigned int index;};
    
    const int VertexSz = sizeof(VERTEX);
    const int IndexSz  = sizeof(INDEX);
    
    // Frame buffer
    unsigned int  CreateFrameBuffer(void);
    bool          DestroyFrameBuffer(unsigned int &FrameBufferID);
    
    // Buffers
    unsigned int CreateVertexArray(void);
    unsigned int CreateVertexBuffer(void);
    unsigned int CreateIndexBuffer(void);
    void SetAttributes(int Position, int Normal, int Texture);
    
    template<typename T> bool LoadBufferVertex(unsigned int VertexBufferID, int VertexCount, T* BufferData);
    template<typename T> bool LoadBufferIndex(unsigned int IndexBufferID, int IndexCount, T* BufferData);
    
    bool DestroyBuffer(unsigned int &BufferID);
    
    // Texture buffer
    unsigned int CreateTextureBuffer(void);
    template<typename T> bool LoadBufferTexture(unsigned int BufferID, T* TextureData, int Width, int Height);
    bool DestroyTextureBuffer(unsigned int &BufferID);
    
    // Shader program objects
    unsigned int CompileSource(unsigned int Type, std::string Src);
    unsigned int CreateShaderProgram(std::string VertexScript, std::string FragmentScript);
    bool DestroyShaderProgram(unsigned int &ProgramID);
    
    // Draw vertex objects
    void DrawVertexBuffer(int PrimitiveType, int VertexCount);
    void DrawIndexBuffer(int PrimitiveType, int IndexCount);
    
    
    
    //
    // Implementations
    
    unsigned int CreateFrameBuffer(void) {
        
        unsigned int FrameBuffer;
        
        glGenFramebuffers(1, &FrameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer);
        
        return FrameBuffer;
    }
    bool DestroyFrameBuffer(unsigned int &FrameBufferID) {
        
        if (FrameBufferID == 0) {return false;}
        if (!glIsFramebuffer(FrameBufferID)) {return false;}
        
        glDeleteFramebuffers(1, &FrameBufferID);
        
        FrameBufferID = 0;
        return true;
    }
    
    unsigned int CreateVertexArray(void) {
        
        unsigned int VertexArray;
        
        glGenVertexArrays(1, &VertexArray);
        glBindVertexArray(VertexArray);
        
        return VertexArray;
    }
    unsigned int CreateVertexBuffer(void) {
        
        unsigned int BufferVertex;
        
        glGenBuffers(1, &BufferVertex);
        glBindBuffer(GL_ARRAY_BUFFER, BufferVertex);
        
        return BufferVertex;
    }
    unsigned int CreateIndexBuffer(void) {
        
        unsigned int BufferIndex;
        
        glGenBuffers(1, &BufferIndex);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferIndex);
        
        return BufferIndex;
    }
    
    template<typename T> bool LoadBufferVertex(unsigned int VertexBufferID, int VertexCount, T* BufferData) {
        
        glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
        glBufferData(GL_ARRAY_BUFFER, VertexCount * VertexSz, &BufferData[0], GL_STATIC_DRAW);
        
        return true;
    }
    template<typename T> bool LoadBufferIndex(unsigned int IndexBufferID, int IndexCount, T* BufferData) {
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexCount * IndexSz, &BufferData[0], GL_STATIC_DRAW);
        
        return true;
    }
    
    void SetAttributes(int Position, int Normal, int Texture) {
        
        // Enable vertex attribute arrays
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        
        // Define the attributes
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VertexSz, (void*) Position);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VertexSz, (void*) Normal);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, VertexSz, (void*) Texture);
        
    }
    bool DestroyBuffer(unsigned int &BufferID) {
        
        if (BufferID == 0) {return false;}
        glDeleteBuffers(1, &BufferID);
        
        BufferID = 0;
        return true;
    }
    
    unsigned int CreateTextureBuffer(void) {
        
        unsigned int BufferTexture;
        
        glGenTextures(1, &BufferTexture);
        glBindTexture(GL_TEXTURE_2D, BufferTexture);
        
        return BufferTexture;
    }
    template<typename T> bool LoadBufferTexture(unsigned int BufferID, T* TextureData, int Width, int Height) {
        
        glBindTexture(GL_TEXTURE_2D, BufferID);
        
        glTexImage2D(GL_TEXTURE_2D, 0, PIXEL_LAYOUT_INTERNAL_, Width, Height, 0, PIXEL_LAYOUT_, GL_UNSIGNED_BYTE, (void*)TextureData);
        
    }
    bool DestroyTextureBuffer(unsigned int &BufferID) {
         
         if (BufferID == 0) {return false;}
         glDeleteTextures(1, &BufferID);
         
         BufferID = 0;
         return true;
    }
    
    unsigned int CompileSource(unsigned int Type, std::string Script) {
        
        unsigned int ShaderID = glCreateShader(Type);
        const char*  SourceScript = Script.c_str();
        
        // Compile source script
        glShaderSource(ShaderID, 1, &SourceScript, nullptr);
        glCompileShader(ShaderID);
        
        // Check compile status
        int vResult;
        glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &vResult);
        if (!vResult) {return 0;}
        
        return ShaderID;
    }
    unsigned int CreateShaderProgram(std::string VertexScript, std::string FragmentScript) {
        
        // Compile the scripts into a shader program
        unsigned int vs = CompileSource(GL_VERTEX_SHADER,   VertexScript);
        unsigned int fs = CompileSource(GL_FRAGMENT_SHADER, FragmentScript);
        
        // Check compiled shaders
        if ( (vs==0) || (fs==0) ) {return 0;}
        
        // Create a shader program
        unsigned int ProgramID = glCreateProgram();
        
        // Attach shaders to program
        glAttachShader(ProgramID, vs);
        glAttachShader(ProgramID, fs);
        
        // Link the program`s shaders
        glLinkProgram(ProgramID);
        
        // Finished, detach and destroy remaining data
        glDetachShader(ProgramID, vs);
        glDetachShader(ProgramID, fs);
        
        glDeleteShader(vs);
        glDeleteShader(fs);
        
        return ProgramID;
    }
    bool DestroyShaderProgram(unsigned int &ProgramID) {
        
        if (ProgramID == 0) {return false;}
        glDeleteProgram(ProgramID);
        
        ProgramID = 0;
        return true;
    }
    
    void DrawBufferVertex(int PrimitiveType, int VertexCount) {
        
        glDrawArrays(PrimitiveType, 0, VertexCount);
        
    }
    void DrawBufferIndex(int PrimitiveType, int IndexCount) {
        
        glDrawElements(PrimitiveType, IndexCount, GL_UNSIGNED_INT, 0);
        
    }
    
    // Log openGL errors
    std::vector<std::string> LogGLErrors(std::string ErrorLocation) {
        
        GLenum glError;
        std::string ErrorMsg = ErrorLocation;
        std::vector<std::string> ErrorList;
        
        // Get any GL error
        glError = glGetError();
        
        // While there are errors
        while (glError != GL_NO_ERROR) {
            
            switch(glError) {
                
                case GL_INVALID_OPERATION:             ErrorMsg+="INVALID_OPERATION"; break;
                case GL_INVALID_ENUM:                  ErrorMsg+="INVALID_ENUM";  break;
                case GL_INVALID_VALUE:                 ErrorMsg+="INVALID_VALUE"; break;
                case GL_OUT_OF_MEMORY:                 ErrorMsg+="OUT_OF_MEMORY"; break;
                case GL_INVALID_FRAMEBUFFER_OPERATION: ErrorMsg+="INVALID_FRAMEBUFFER_OPERATION"; break;
                default :                              ErrorMsg+="UNKNOWN_ERROR"; break;
                
            }
            
            LogWrite(ErrorMsg);
            LogWriteLn();
            
            ErrorList.push_back( ErrorMsg );
            
            // Get the next error
            glError = glGetError();
        }
        
        return ErrorList;
    }
    
}

#endif





