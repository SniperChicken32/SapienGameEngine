//
// Resource management

namespace SceneManager {
    
    // Resource management
    namespace ResourceManagement {
        
        // Asset file loader
        struct AssetLoader {
            
            // In the file being loaded, the asset name is
            // specified in this way:
            //
            // define_asset YourAssetName
            
            // Values are specified in the following way:
            //
            // Name Value
            
            // Data blocks are specified
            // by using the following syntax:
            //
            // [begin] YourBlockName
            //
            // Data...
            //
            // [end]
            
            // The name of the asset
            std::string AssetName;
            
            // Pair of names and values
            std::map<std::string, std::vector<std::string>> AssetData;
            
            // Pair of names and data blocks
            std::map<std::string, std::string> DataBlocks;
            
            bool IsDataLoaded;
            
            std::fstream FileStream;
            
            AssetLoader(std::string FileName) {
                
                AssetName = "";
                
                AssetData.clear();
                DataBlocks.clear();
                
                IsDataLoaded = false;
                
                // Load the file data
                FileStream.open( FileName );
                if (this ->IsAssetLoaded()) {
                    
                    std::string String;
                    
                    // Get first line from the file
                    getline(FileStream, String);
                    if (String == "") return;
                    
                    // Split string by spaces
                    std::vector<std::string> Array = StringExplode(String, ' ');
                    int MaxSz = Array.size();
                    if (MaxSz <= 0) return;
                    
                    // Check asset header
                    if (Array[0] != "define_asset") return;
                    
                    // Set asset name
                    AssetName = Array[1];
                    
                    // Load the data from file
                    while ( getline(FileStream, String) ) {
                        
                        if (String == "") continue;
                        if (StringFind("//", String)) continue;
                        
                        // Split string by spaces
                        std::vector<std::string> ArrayData = StringExplode(String, ' ');
                        int MaxSz = ArrayData.size();
                        if (MaxSz < 1) continue;
                        
                        // Check data block
                        if (ArrayData[0] == "[begin]") {
                            
                            // Get block name
                            std::string BlockName = ArrayData[1];
                            std::string BlockString = "";
                            
                            for (int i=0; i < 512; i++) {
                                
                                getline(FileStream, String);
                                if (String == "[end]") break;
                                
                                BlockString += String + "\n";
                                
                            }
                            
                            DataBlocks.emplace(BlockName, BlockString);
                            
                        }
                        
                        std::vector<std::string> ValueList;
                        
                        for (int i=1; i < MaxSz; i++) {
                            
                            ValueList.push_back(ArrayData[i]);
                            
                        }
                        
                        AssetData.emplace(ArrayData[0], ValueList);
                        
                    }
                    
                    IsDataLoaded = true;
                    
                    FileStream.close();
                    
                }
                
            }
            ~AssetLoader() {
                
            }
            
            // Check if the asset is loaded
            bool IsAssetLoaded(void) {if (IsDataLoaded) return true; if (FileStream.is_open()) return true; else return false;}
            
            std::string GetValueByName(std::string Name, int Index) {
                
                for (std::map<std::string, std::vector<std::string>>::iterator it = AssetData.begin(); it != AssetData.end(); it++) {
                    
                    if (it ->first == Name) {
                        
                        std::vector<std::string> ValueList = it ->second;
                        if (ValueList.size() == 0) return "";
                        
                        if (ValueList.size() < Index) return "";
                        
                        return ValueList[Index];
                        
                    }
                    
                }
                
                return "";
            }
            
            std::string GetDataBlockByName(std::string Name) {
                
                for (std::map<std::string, std::string>::iterator it = DataBlocks.begin(); it != DataBlocks.end(); it++) {
                    
                    if (it ->first == Name) {
                        
                        return it ->second;
                    }
                    
                }
                
                return "";
            }
            
        };
        
        namespace Loaders {
            
            Camera* LoadCamera(std::string FilePath) {
                
                // Load and check the asset
                SceneManager::ResourceManagement::AssetLoader AssetLoader( FilePath );
                if (!AssetLoader.IsAssetLoaded()) {
                    
                    LogWrite(strings::LogStringError + strings::AssetTypeCamera + FilePath);
                    
                    return nullptr;
                }
                
                // Check if the asset is already loaded
                if (Renderer ->FindCamera(AssetLoader.AssetName) != nullptr) return nullptr;
                
                // Create the asset
                Camera* CameraPtr = Renderer ->CreateCamera();
                
                // Name the asset
                CameraPtr ->Name = AssetLoader.AssetName;
                
#ifdef  DEVELOPMENT_MODE_
                LogWrite(strings::LogStringAdd + strings::AssetTypeCamera + AssetLoader.AssetName);
#endif  
                
                // Assign asset values
                if (AssetLoader.GetValueByName("MouseLook", 0) == "1") {CameraPtr ->UseMouseLook = true;} else {CameraPtr ->UseMouseLook = false;}
                if (AssetLoader.GetValueByName("CamHeight", 0) == "1") {CameraPtr ->UseMouseLook = true;} else {CameraPtr ->UseMouseLook = false;}
                
                CameraPtr ->CameraHeight = StringToFloat(AssetLoader.GetValueByName("FoV", 0));
                
                CameraPtr ->vpAspect = StringToFloat(AssetLoader.GetValueByName("Aspect", 0));
                CameraPtr ->vpNear   = StringToFloat(AssetLoader.GetValueByName("Near", 0));
                CameraPtr ->vpFar    = StringToDouble(AssetLoader.GetValueByName("Far", 0));
                
                CameraPtr ->MouseSensitivityYaw   = StringToFloat(AssetLoader.GetValueByName("MouseSensitivityYaw", 0));
                CameraPtr ->MouseSensitivityPitch = StringToFloat(AssetLoader.GetValueByName("MouseSensitivityPitch", 0));
                
                CameraPtr ->MouseLimitPitchMin = StringToFloat(AssetLoader.GetValueByName("MouseLimitPitchMin", 0));
                CameraPtr ->MouseLimitPitchMax = StringToFloat(AssetLoader.GetValueByName("MouseLimitPitchMax", 0));
                
                CameraPtr ->MouseLimitYawMin = StringToFloat(AssetLoader.GetValueByName("MouseLimitYawMin", 0));
                CameraPtr ->MouseLimitYawMax = StringToFloat(AssetLoader.GetValueByName("MouseLimitYawMax", 0));
                
                return CameraPtr;
                
                
                
                // Get asset name
                std::string AssetName = AssetFilePeekHeader(FilePath);
                
                std::string Line;
                std::ifstream FileStream;
                
                FileStream.open( FilePath );
                
                // Check file open
                if (FileStream.is_open()){
                    
                    
                    
                    
                    // Loop through the file
                    while ( getline(FileStream, Line) ) {
                        
                        // Check comment
                        if (StringFind("//", Line)) {continue;}
                        
                        // Explode the string
                        int i=0; std::string Strings[50];
                        std::vector<std::string> Array = StringExplode(Line, ' ');
                        for (std::vector<std::string>::iterator it = Array.begin(); it != Array.end(); ++it) {std::string String = *it; Strings[i] = String; i++;}
                        
                        if (Strings[0] == "MouseLook") {if (Strings[1] == "1") {CameraPtr ->UseMouseLook = true;} else {CameraPtr ->UseMouseLook = false;}}
                        if (Strings[0] == "CamHeight") {CameraPtr ->CameraHeight = StringToFloat(Strings[1]);}
                        
                        if (Strings[0] == "FoV")    {CameraPtr ->vpFoV    = StringToFloat(Strings[1]);}
                        if (Strings[0] == "Aspect") {CameraPtr ->vpAspect = StringToFloat(Strings[1]);}
                        if (Strings[0] == "Near")   {CameraPtr ->vpNear   = StringToDouble(Strings[1]);}
                        if (Strings[0] == "Far")    {CameraPtr ->vpFar    = StringToDouble(Strings[1]);}
                        
                        if (Strings[0] == "MouseSensitivityYaw")   {CameraPtr ->MouseSensitivityYaw   = StringToFloat(Strings[1]);}
                        if (Strings[0] == "MouseSensitivityPitch") {CameraPtr ->MouseSensitivityPitch = StringToFloat(Strings[1]);}
                        
                        if (Strings[0] == "MouseLimitPitchMin") {CameraPtr ->MouseLimitPitchMin = StringToFloat(Strings[1]);}
                        if (Strings[0] == "MouseLimitPitchMax") {CameraPtr ->MouseLimitPitchMax = StringToFloat(Strings[1]);}
                        
                        if (Strings[0] == "MouseLimitYawMin") {CameraPtr ->MouseLimitYawMin = StringToFloat(Strings[1]);}
                        if (Strings[0] == "MouseLimitYawMax") {CameraPtr ->MouseLimitYawMax = StringToFloat(Strings[1]);}
                        
                        // Reset
                        Strings[0]="";
                    }
                    
                    FileStream.close();
                    
                } else {
                    
                    LogWrite(strings::LogStringError + strings::AssetTypeCamera + FilePath);
                    
                    Renderer ->DestroyCamera(CameraPtr);
                    return nullptr;
                }
                
                return CameraPtr;
            }
            
            Light* LoadLight(std::string FilePath) {
                
                // Load and check the asset
                SceneManager::ResourceManagement::AssetLoader AssetLoader( FilePath );
                if (!AssetLoader.IsAssetLoaded()) {
                    
                    LogWrite(strings::LogStringError + strings::AssetTypeLight + FilePath);
                    
                    return nullptr;
                }
                
                // Check if the asset is already loaded
                if (Renderer ->FindLight(AssetLoader.AssetName) != nullptr) return nullptr;
                
                // Create the asset
                Light* LightPtr = Renderer ->CreateLight();
                
                // Name the asset
                LightPtr ->Name = AssetLoader.AssetName;
                LightPtr ->OriginalName = "";
                
#ifdef  DEVELOPMENT_MODE_
                LogWrite(strings::LogStringAdd + strings::AssetTypeLight + AssetLoader.AssetName);
#endif  
                
                // Assign asset values
                
                std::string Type = AssetLoader.GetValueByName("Type", 0);
                if (Type == "POINT")       {LightPtr ->LightType = 0;}
                if (Type == "DIRECTIONAL") {LightPtr ->LightType = 1;}
                if (Type == "SPOT")        {LightPtr ->LightType = 2;}
                
                LightPtr ->Diffuse.r = StringToFloat(AssetLoader.GetValueByName("Diffuse", 0));
                LightPtr ->Diffuse.g = StringToFloat(AssetLoader.GetValueByName("Diffuse", 1));
                LightPtr ->Diffuse.b = StringToFloat(AssetLoader.GetValueByName("Diffuse", 2));
                
                LightPtr ->Specular.r = StringToFloat(AssetLoader.GetValueByName("Specular", 0));
                LightPtr ->Specular.g = StringToFloat(AssetLoader.GetValueByName("Specular", 1));
                LightPtr ->Specular.b = StringToFloat(AssetLoader.GetValueByName("Specular", 2));
                
                LightPtr ->Range      = StringToFloat(AssetLoader.GetValueByName("Range", 0));
                LightPtr ->SetIntensity(StringToFloat(AssetLoader.GetValueByName("Intensity", 0)));
                
                //LightPtr ->Constant   = StringToFloat(AssetLoader.GetValueByName("Constant", 0));
                //LightPtr ->Linear     = StringToFloat(AssetLoader.GetValueByName("Linear", 0));
                //LightPtr ->Quadratic  = StringToFloat(AssetLoader.GetValueByName("Quadratic", 0));
                LightPtr ->Focus      = StringToFloat(AssetLoader.GetValueByName("Focus", 0));
                
                return LightPtr;
            }
            
            Mesh* LoadMesh(std::string FilePath) {
                
                // Create the asset
                Mesh* MeshPtr = Renderer ->CreateMesh();
                
                // Load wavefront file
                if (MeshPtr ->MeshLoader.LoadFile(FilePath)) {
                    
                    // Check if the asset is already loaded
                    if (Renderer ->FindMesh(MeshPtr ->MeshLoader.LoadedMeshes[0].MeshName) != nullptr) {
                        
                        Renderer ->DestroyMesh(MeshPtr);
                        return nullptr;
                    }
                    
                    // Set the asset name
                    MeshPtr ->Name = MeshPtr ->MeshLoader.LoadedMeshes[0].MeshName;
                    
#ifdef  DEVELOPMENT_MODE_
                    LogWrite(strings::LogStringAdd + strings::AssetTypeMesh + MeshPtr ->Name);
#endif
                    
                } else {
                    
                    LogWrite(strings::LogStringError + strings::AssetTypeMesh + FilePath);
                    
                    Renderer ->DestroyMesh(MeshPtr);
                    return nullptr;
                }
                
                // Get buffer sizes
                MeshPtr ->VertexCount = MeshPtr ->MeshLoader.LoadedVertices.size();
                MeshPtr ->IndexCount  = MeshPtr ->MeshLoader.LoadedIndices.size();
                
                // Check if a mesh was already loaded
                if (MeshPtr ->IsBufferLoaded) {
                    
                    // Destroy the buffers
                    
                    glDeleteBuffers(1, &MeshPtr ->BufferVertex);
                    if (MeshPtr ->IsBufferIndexed) {glDeleteBuffers(1, &MeshPtr ->BufferIndex);}
                    
                    glDeleteBuffers(1, &MeshPtr ->BufferAttribute);
                    
                    MeshPtr ->IsBufferIndexed = false;
                    MeshPtr ->IsBufferLoaded  = false;
                }
                
                // Draw triangles by default
                MeshPtr ->PrimitiveType = MESH_TRIANGLES;
                
                //
                // First, generate an attribute objects
                MeshPtr ->BufferAttribute = RenderLibrary::CreateVertexArray();
                
                // Generate vertex buffer
                MeshPtr ->BufferVertex    = RenderLibrary::CreateVertexBuffer();
                
                // Load the vertex buffer
                RenderLibrary::LoadBufferVertex(MeshPtr ->BufferVertex, MeshPtr ->VertexCount, &MeshPtr ->MeshLoader.LoadedVertices[0]);
                
                // Check if we are indexed
                if (MeshPtr ->IndexCount > 0) {
                    
                    // Generate an index buffer
                    MeshPtr ->BufferIndex     = RenderLibrary::CreateIndexBuffer();
                    MeshPtr ->IsBufferIndexed = true;
                    
                    // Load the index buffer
                    RenderLibrary::LoadBufferIndex(MeshPtr ->BufferIndex, MeshPtr ->IndexCount, &MeshPtr ->MeshLoader.LoadedIndices[0]);
                    
                }
                
                // set the vertex attribute offsets
                RenderLibrary::SetAttributes(0, 12, 24);
                
                MeshPtr ->IsBufferLoaded  = true;
                
#ifdef  LOG_GL_ERRORS_
                RenderLibrary::LogGLErrors(strings::AssetTypeMesh);
#endif
                
                return MeshPtr;
            }
            
            Material* LoadMaterial(std::string FilePath) {
                
                // Load and check the asset
                SceneManager::ResourceManagement::AssetLoader AssetLoader( FilePath );
                if (!AssetLoader.IsAssetLoaded()) {
                    
                    LogWrite(strings::LogStringError + strings::AssetTypeMaterial + FilePath);
                    
                    return nullptr;
                }
                
                // Check if the asset is already loaded
                if (Renderer ->FindMaterial(AssetLoader.AssetName) != nullptr) return nullptr;
                
                // Create the asset
                Material* MaterialPtr = Renderer ->CreateMaterial();
                
                // Name the asset
                MaterialPtr ->Name = AssetLoader.AssetName;
                
#ifdef  DEVELOPMENT_MODE_
                LogWrite(strings::LogStringAdd + strings::AssetTypeMaterial + AssetLoader.AssetName);
#endif  
                
                // Assign asset values
                
                std::string FilterType = AssetLoader.GetValueByName("filter", 0);
                MaterialPtr ->TextureFilter = MATERIAL_FILTER_NONE;
                if (FilterType == "bilinear")  MaterialPtr ->TextureFilter = MATERIAL_FILTER_LINEAR;
                if (FilterType == "trilinear") MaterialPtr ->TextureFilter = MATERIAL_FILTER_TRILINEAR;
                if (FilterType == "aniso")     MaterialPtr ->TextureFilter = MATERIAL_FILTER_ANISOTROPIC;
                
                MaterialPtr ->Ambient.r = StringToFloat(AssetLoader.GetValueByName("ambient", 0));
                MaterialPtr ->Ambient.g = StringToFloat(AssetLoader.GetValueByName("ambient", 1));
                MaterialPtr ->Ambient.b = StringToFloat(AssetLoader.GetValueByName("ambient", 2));
                
                MaterialPtr ->Diffuse.r = StringToFloat(AssetLoader.GetValueByName("diffuse", 0));
                MaterialPtr ->Diffuse.g = StringToFloat(AssetLoader.GetValueByName("diffuse", 1));
                MaterialPtr ->Diffuse.b = StringToFloat(AssetLoader.GetValueByName("diffuse", 2));
                
                MaterialPtr ->Specular.r = StringToFloat(AssetLoader.GetValueByName("specular", 0));
                MaterialPtr ->Specular.g = StringToFloat(AssetLoader.GetValueByName("specular", 1));
                MaterialPtr ->Specular.b = StringToFloat(AssetLoader.GetValueByName("specular", 2));
                
                MaterialPtr ->Shininess   = StringToFloat(AssetLoader.GetValueByName("shininess", 0));
                MaterialPtr ->SpecularMul = StringToFloat(AssetLoader.GetValueByName("specularmul", 0));
                
                //MaterialPtr ->
                
                
                for (std::map<std::string, std::vector<std::string>>::iterator it = AssetLoader.AssetData.begin(); it != AssetLoader.AssetData.end(); it++) {
                    
                    //it ->first;
                    //it ->second;
                    
                }
                
                AssetLoader.GetValueByName("enable", 0);
                
                // "depth_test" MaterialPtr ->UseDepthTest = true;
                // "lighting"   MaterialPtr ->UseLighting  = true;
                // "blending"   MaterialPtr ->UseBlending  = true;
                
                
                
                AssetLoader.GetValueByName("disable", 0);
                
                //"depth_test" MaterialPtr ->UseDepthTest = false;
                //"lighting"   MaterialPtr ->UseLighting  = false;
                //"blending"   MaterialPtr ->UseBlending  = false;
                
                std::string TexturePath = AssetLoader.GetValueByName("texture", 0);
                
                
                // Texture dimensions
                int Width=0;
                int Height=0;
                int Channels=0;
                
                //
                // Load texture
                unsigned char* TextureData = nullptr;
                TextureData = stbi_load(TexturePath.c_str(), &Width, &Height, &Channels, 3);
                
                // Check texture loaded
                if (TextureData == nullptr) {
                    
                    LogWrite(strings::SpaceDouble + strings::LogStringError + TexturePath);
                    LogWriteLn();
                    
                    Renderer ->DestroyMaterial(MaterialPtr);
                    return nullptr;
                }
                
                // Check if a texture was already loaded
                if (MaterialPtr ->IsTextureLoaded) {
                    glDeleteTextures(1, &MaterialPtr ->BufferTexture);
                    MaterialPtr ->IsTextureLoaded = false;
                }
                
                // Generate texture
                MaterialPtr ->BufferTexture = RenderLibrary::CreateTextureBuffer();
                
                switch (MaterialPtr ->TextureFilter) {
                    
                    case MATERIAL_FILTER_ANISOTROPIC:
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                        
                        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &MaterialPtr ->AnisotropyLevel);
                        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, MaterialPtr ->AnisotropyLevel);
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
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, MaterialPtr ->TextureParameter);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, MaterialPtr ->TextureParameter);
                
                // Load the texture data
                RenderLibrary::LoadBufferTexture(MaterialPtr ->BufferTexture, TextureData, Width, Height);
                
                // Generate mipmaps
                glGenerateMipmap(GL_TEXTURE_2D);
                
                // Free the image data
                stbi_image_free(TextureData);
                
#ifdef  LOG_GL_ERRORS_
                RenderLibrary::LogGLErrors(strings::AssetTypeMaterial);
#endif
                
                MaterialPtr ->IsMaterialLoaded = true;
                MaterialPtr ->IsTextureLoaded = true;
                
                
                
                return MaterialPtr;
                
                
                
                /*
                
                // Create the asset
                Material* MaterialPtr = Renderer ->CreateMaterial();
                
                int i=0;
                std::string Line;
                std::ifstream FileStream;
                std::vector<std::string> FileData;
                
                int Width=0;
                int Height=0;
                int Channels=0;
                
                MaterialPtr ->IsMaterialLoaded = false;
                MaterialPtr ->IsTextureLoaded = false;
                
                std::string TexturePath = "";
                
                // Get asset name
                std::string AssetName = AssetFilePeekHeader( FilePath );
                MaterialPtr ->Name = AssetName;
                
                FileStream.open(FilePath);
                
                if (FileStream.is_open()) {
                    
#ifdef  DEVELOPMENT_MODE_
                    LogWrite(strings::LogStringAdd + strings::AssetTypeMaterial + MaterialPtr ->Name);
#endif
                
                // Check enable/disable
                if (Strings[0] == "enable") {
                    
                    if (Strings[1] == "depth_test") {MaterialPtr ->UseDepthTest = true; continue;}
                    if (Strings[1] == "lighting")   {MaterialPtr ->UseLighting  = true; continue;}
                    if (Strings[1] == "blending")   {MaterialPtr ->UseBlending  = true; continue;}
                    
                }
                
                if (Strings[0] == "disable") {
                    
                    if (Strings[1] == "depth_test") {MaterialPtr ->UseDepthTest = false; continue;}
                    if (Strings[1] == "lighting")   {MaterialPtr ->UseLighting  = false; continue;}
                    if (Strings[1] == "blending")   {MaterialPtr ->UseBlending  = false; continue;}
                    
                }
                
                // Color
                if (Strings[0] == "ambient")  {MaterialPtr ->Ambient.r = StringToFloat(Strings[1]);  MaterialPtr ->Ambient.g = StringToFloat(Strings[2]);  MaterialPtr ->Ambient.b = StringToFloat(Strings[3]);continue;}
                if (Strings[0] == "diffuse")  {MaterialPtr ->Diffuse.r = StringToFloat(Strings[1]);  MaterialPtr ->Diffuse.g = StringToFloat(Strings[2]);  MaterialPtr ->Diffuse.b = StringToFloat(Strings[3]);continue;}
                if (Strings[0] == "specular") {MaterialPtr ->Specular.r = StringToFloat(Strings[1]); MaterialPtr ->Specular.g = StringToFloat(Strings[2]); MaterialPtr ->Specular.b = StringToFloat(Strings[3]);continue;}
                
                // Specular intensity
                if (Strings[0] == "shininess")   {MaterialPtr ->Shininess = StringToFloat(Strings[1]);continue;}
                if (Strings[0] == "specularmul") {MaterialPtr ->SpecularMul = StringToFloat(Strings[1]);continue;}
                
                // Texture filtering
                if (Strings[0] == "filter") {
                    
                    if (Strings[1] == "none")      {MaterialPtr ->TextureFilter = 0;continue;}
                    if (Strings[1] == "linear")    {MaterialPtr ->TextureFilter = 1;continue;}
                    if (Strings[1] == "bilinear")  {MaterialPtr ->TextureFilter = 2;continue;}
                    if (Strings[1] == "aniso")     {MaterialPtr ->TextureFilter = 3;continue;}
                    
                    continue;
                }
                
                if (Strings[0] == "blend") {
                    
                    if (Strings[1] == "color") {
                        MaterialPtr ->BlendColor.r = StringToFloat(Strings[2]);
                        MaterialPtr ->BlendColor.g = StringToFloat(Strings[3]);
                        MaterialPtr ->BlendColor.b = StringToFloat(Strings[4]);
                        MaterialPtr ->BlendColor.a = StringToFloat(Strings[5]);
                        continue;
                    }
                    
                    if (Strings[1] == "equation") {MaterialPtr ->blEquation = StringToInt(Strings[2]);continue;}
                    
                    if (Strings[1] == "function") {
                        MaterialPtr ->UseSeperateAlpha = false;
                        MaterialPtr ->blSource = StringToInt(Strings[2]);
                        MaterialPtr ->blDest   = StringToInt(Strings[3]);
                        continue;
                    }
                    
                    if (Strings[1] == "function_separate") {
                        MaterialPtr ->UseSeperateAlpha = true;
                        MaterialPtr ->blColorSource = StringToInt(Strings[2]);
                        MaterialPtr ->blAlphaSource = StringToInt(Strings[3]);
                        MaterialPtr ->blColorDest   = StringToInt(Strings[4]);
                        MaterialPtr ->blAlphaDest   = StringToInt(Strings[5]);
                        continue;
                    }
                    
                }
                
                // Texture
                if (Strings[0] == "texture") {
                    
                    TexturePath = Strings[1];
                    
                    continue;
                }
                
                
                
                
                
                //
                // Load texture
                unsigned char* TextureData = nullptr;
                TextureData = stbi_load(TexturePath.c_str(), &Width, &Height, &Channels, 3);
                
                // Check texture loaded
                if (TextureData == nullptr) {
                    
                    LogWrite(strings::SpaceDouble + strings::LogStringError + TexturePath);
                    LogWriteLn();
                    
                    Renderer ->DestroyMaterial(MaterialPtr);
                    return nullptr;
                }
                
                // Check if a texture was already loaded
                if (MaterialPtr ->IsTextureLoaded) {
                    glDeleteTextures(1, &MaterialPtr ->BufferTexture);
                    MaterialPtr ->IsTextureLoaded = false;
                }
                
                // Generate texture
                MaterialPtr ->BufferTexture = RenderLibrary::CreateTextureBuffer();
                
                switch (MaterialPtr ->TextureFilter) {
                    
                    case MATERIAL_FILTER_ANISOTROPIC:
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                        
                        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &MaterialPtr ->AnisotropyLevel);
                        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, MaterialPtr ->AnisotropyLevel);
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
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, MaterialPtr ->TextureParameter);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, MaterialPtr ->TextureParameter);
                
                // Load the texture data
                RenderLibrary::LoadBufferTexture(MaterialPtr ->BufferTexture, TextureData, Width, Height);
                
                // Generate mipmaps
                glGenerateMipmap(GL_TEXTURE_2D);
                
                // Free the image data
                stbi_image_free(TextureData);
                
#ifdef  LOG_GL_ERRORS_
                RenderLibrary::LogGLErrors(strings::AssetTypeMaterial);
#endif
                
                MaterialPtr ->IsMaterialLoaded = true;
                MaterialPtr ->IsTextureLoaded = true;
                return MaterialPtr;
                */
                
            }
            
            Shader* LoadShader(std::string FilePath) {
                
                // Load and check the asset
                SceneManager::ResourceManagement::AssetLoader AssetLoader( FilePath );
                if (!AssetLoader.IsAssetLoaded()) {
                    
                    LogWrite(strings::LogStringError + strings::AssetTypeShader + FilePath);
                    
                    return nullptr;
                }
                
                // Check if the asset is already loaded
                if (Renderer ->FindShader(AssetLoader.AssetName) != nullptr) return nullptr;
                
                // Create the asset
                Shader* ShaderPtr = Renderer ->CreateShader();
                
                // Name the asset
                ShaderPtr ->Name = AssetLoader.AssetName;
                
#ifdef  DEVELOPMENT_MODE_
                LogWrite(strings::LogStringAdd + strings::AssetTypeShader + AssetLoader.AssetName);
#endif  
                
                // Assign asset values
                
                std::string ViewProjMatrixName = AssetLoader.GetValueByName("define_matrices", 0);
                std::string ModelMatrixName    = AssetLoader.GetValueByName("define_matrices", 1);
                std::string NormalMatrixName   = AssetLoader.GetValueByName("define_matrices", 2);
                
                std::string AmbientColorName   = AssetLoader.GetValueByName("define_material", 0);
                std::string DiffuseColorName   = AssetLoader.GetValueByName("define_material", 1);
                std::string SpecularColorName  = AssetLoader.GetValueByName("define_material", 2);
                std::string ShininessName      = AssetLoader.GetValueByName("define_material", 3);
                std::string SpecularMulName    = AssetLoader.GetValueByName("define_material", 4);
                
                std::string TextureSamplerName = AssetLoader.GetValueByName("define_textures", 0);
                
                std::string CameraPosName   = AssetLoader.GetValueByName("define_camera", 0);
                std::string LightCountName  = AssetLoader.GetValueByName("define_lights", 0);
                
                std::string VertexCode    = AssetLoader.GetDataBlockByName("VertexShader");
                std::string FragmentCode  = AssetLoader.GetDataBlockByName("FragmentShader");
                
                
                
                // Check if a program is already loaded
                if (ShaderPtr ->IsShaderLoaded) {
                    glDeleteProgram( ShaderPtr ->ProgramID );
                    ShaderPtr ->IsShaderLoaded = false;
                }
                
                //
                // Create and compile the shader program
                ShaderPtr ->ProgramID = RenderLibrary::CreateShaderProgram( VertexCode, FragmentCode );
                
                // Check shader creation
                if (!ShaderPtr ->ProgramID) {
                    
                    LogWrite(strings::ErrorStringHeader + "Shader compilation failed.");
                    LogWriteLn();
                    
                    Renderer ->DestroyShader(ShaderPtr);
                    return nullptr;
                }
                
                
                // Setup uniform matrix locations
                ShaderPtr ->ViewProjMatrixLoc = glGetUniformLocation(ShaderPtr ->ProgramID, ViewProjMatrixName.c_str());
                ShaderPtr ->ModelMatrixLoc    = glGetUniformLocation(ShaderPtr ->ProgramID, ModelMatrixName.c_str());
                ShaderPtr ->NormalMatrixLoc   = glGetUniformLocation(ShaderPtr ->ProgramID, NormalMatrixName.c_str());
                
                // Texture samplers
                ShaderPtr ->TextureSamplerLoc = glGetUniformLocation(ShaderPtr ->ProgramID, TextureSamplerName.c_str());
                
                // Setup uniform color locations
                ShaderPtr ->AmbientColorLoc   = glGetUniformLocation(ShaderPtr ->ProgramID, AmbientColorName.c_str());
                ShaderPtr ->DiffuseColorLoc   = glGetUniformLocation(ShaderPtr ->ProgramID, DiffuseColorName.c_str());
                ShaderPtr ->SpecularColorLoc  = glGetUniformLocation(ShaderPtr ->ProgramID, SpecularColorName.c_str());
                ShaderPtr ->ShininessLoc      = glGetUniformLocation(ShaderPtr ->ProgramID, ShininessName.c_str());
                ShaderPtr ->SpecularMulLoc    = glGetUniformLocation(ShaderPtr ->ProgramID, SpecularMulName.c_str());
                
                // Lighting
                ShaderPtr ->CameraPosLoc      = glGetUniformLocation(ShaderPtr ->ProgramID, CameraPosName.c_str());
                ShaderPtr ->LightCountLoc     = glGetUniformLocation(ShaderPtr ->ProgramID, LightCountName.c_str());
                
                // Initiate light structure
                for (int i=0; i < MAX_LIGHT_COUNT; i++) {
                    
                    std::string StringCounter = "lights[" + IntToString(i) + "].";
                    
                    std::string PositionConst    = StringCounter + "Position";
                    std::string DirectionConst   = StringCounter + "Direction";
                    std::string DiffuseConst     = StringCounter + "Diffuse";
                    std::string SpecularConst    = StringCounter + "Specular";
                    std::string AttenuationConst = StringCounter + "Attenuation";
                    std::string FocusConst       = StringCounter + "Focus";
                    
                    ShaderPtr ->LightPosLoc   [i] = glGetUniformLocation(ShaderPtr ->ProgramID, PositionConst.c_str());
                    ShaderPtr ->LightDirLoc   [i] = glGetUniformLocation(ShaderPtr ->ProgramID, DirectionConst.c_str());
                    ShaderPtr ->LightDiffLoc  [i] = glGetUniformLocation(ShaderPtr ->ProgramID, DiffuseConst.c_str());
                    ShaderPtr ->LightSpecLoc  [i] = glGetUniformLocation(ShaderPtr ->ProgramID, SpecularConst.c_str());
                    ShaderPtr ->LightAttenLoc [i] = glGetUniformLocation(ShaderPtr ->ProgramID, AttenuationConst.c_str());
                    ShaderPtr ->LightFocusLoc [i] = glGetUniformLocation(ShaderPtr ->ProgramID, FocusConst.c_str());
                    
                }
                
#ifdef  LOG_GL_ERRORS_
                RenderLibrary::LogGLErrors(strings::AssetTypeShader);
#endif
                
                ShaderPtr ->IsShaderLoaded = true;
                
                return ShaderPtr;
            }
            
            Actor* LoadActor(std::string FilePath) {
                
                // Load and check the asset
                SceneManager::ResourceManagement::AssetLoader AssetLoader( FilePath );
                if (!AssetLoader.IsAssetLoaded()) {
                    
                    LogWrite(strings::LogStringError + strings::AssetTypeActor + FilePath);
                    
                    return nullptr;
                }
                
                // Check if the asset is already loaded
                if (ActorAI::AI ->FindActor(AssetLoader.AssetName) != nullptr) return nullptr;
                
                // Create the asset
                Actor* ActorPtr = ActorAI::AI ->CreateActor();
                
                // Name the asset
                ActorPtr ->Name         = AssetLoader.AssetName;
                ActorPtr ->OriginalName = "";
                
#ifdef  DEVELOPMENT_MODE_
                LogWrite(strings::LogStringAdd + strings::AssetTypeActor + AssetLoader.AssetName);
#endif  
                
                // Get asset values
                
                // Mesh names
                ActorPtr ->HeadName  = AssetLoader.GetValueByName("Head", 0);
                ActorPtr ->TorsoName = AssetLoader.GetValueByName("Torso", 0);
                ActorPtr ->ArmLName  = AssetLoader.GetValueByName("ArmR", 0);
                ActorPtr ->ArmRName  = AssetLoader.GetValueByName("ArmL", 0);
                ActorPtr ->LegLName  = AssetLoader.GetValueByName("LegR", 0);
                ActorPtr ->LegRName  = AssetLoader.GetValueByName("LegL", 0);
                
                // Use of animation cycles
                if (AssetLoader.GetValueByName("DoAnimationStates", 0) == "true") ActorPtr ->DoAnimationStates = true;
                
                ActorPtr ->DirectionOffset = StringToFloat(AssetLoader.GetValueByName("DirectionOffset", 0));
                
                // Animation states
                ActorPtr ->AnimationIdle  = StringToInt(AssetLoader.GetValueByName("AnimationIdle", 0));
                ActorPtr ->AnimationWalk  = StringToInt(AssetLoader.GetValueByName("AnimationWalk", 0));
                ActorPtr ->AnimationRun   = StringToInt(AssetLoader.GetValueByName("AnimationRun", 0));
                ActorPtr ->AnimationDeath = StringToInt(AssetLoader.GetValueByName("AnimationDeath", 0));
                
                // Animation limits
                ActorPtr ->LimbSwingRangeWalk = StringToFloat(AssetLoader.GetValueByName("LimbSwingRangeWalk", 0));
                ActorPtr ->LimbSwingRateWalk  = StringToFloat(AssetLoader.GetValueByName("LimbSwingRateWalk", 0));
                ActorPtr ->LimbSwingRangeRun = StringToFloat(AssetLoader.GetValueByName("LimbSwingRangeRun", 0));
                ActorPtr ->LimbSwingRateRun  = StringToFloat(AssetLoader.GetValueByName("LimbSwingRateRun", 0));
                
                ActorPtr ->SpeedWalk = StringToFloat(AssetLoader.GetValueByName("Speed", 0));
                ActorPtr ->SpeedRun  = StringToFloat(AssetLoader.GetValueByName("Speed", 1));
                
                
                // Attachment offsets
                
                std::string Name;
                float OffsetX, OffsetY, OffsetZ;
                
                Name = "HeadOffset";
                OffsetX = StringToFloat(AssetLoader.GetValueByName(Name, 0));
                OffsetY = StringToFloat(AssetLoader.GetValueByName(Name, 1));
                OffsetZ = StringToFloat(AssetLoader.GetValueByName(Name, 2));
                ActorPtr ->HeadOffset  = glm::vec3(OffsetX, OffsetY, OffsetZ);
                
                Name = "TorsoOffset";
                OffsetX = StringToFloat(AssetLoader.GetValueByName(Name, 0));
                OffsetY = StringToFloat(AssetLoader.GetValueByName(Name, 1));
                OffsetZ = StringToFloat(AssetLoader.GetValueByName(Name, 2));
                ActorPtr ->TorsoOffset = glm::vec3(OffsetX, OffsetY, OffsetZ);
                
                Name = "ArmROffset";
                OffsetX = StringToFloat(AssetLoader.GetValueByName(Name, 0));
                OffsetY = StringToFloat(AssetLoader.GetValueByName(Name, 1));
                OffsetZ = StringToFloat(AssetLoader.GetValueByName(Name, 2));
                ActorPtr ->ArmROffset  = glm::vec3(OffsetX, OffsetY, OffsetZ);
                
                Name = "ArmLOffset";
                OffsetX = StringToFloat(AssetLoader.GetValueByName(Name, 0));
                OffsetY = StringToFloat(AssetLoader.GetValueByName(Name, 1));
                OffsetZ = StringToFloat(AssetLoader.GetValueByName(Name, 2));
                ActorPtr ->ArmLOffset  = glm::vec3(OffsetX, OffsetY, OffsetZ);
                
                Name = "LegROffset";
                OffsetX = StringToFloat(AssetLoader.GetValueByName(Name, 0));
                OffsetY = StringToFloat(AssetLoader.GetValueByName(Name, 1));
                OffsetZ = StringToFloat(AssetLoader.GetValueByName(Name, 2));
                ActorPtr ->LegROffset  = glm::vec3(OffsetX, OffsetY, OffsetZ);
                
                Name = "LegLOffset";
                OffsetX = StringToFloat(AssetLoader.GetValueByName(Name, 0));
                OffsetY = StringToFloat(AssetLoader.GetValueByName(Name, 1));
                OffsetZ = StringToFloat(AssetLoader.GetValueByName(Name, 2));
                ActorPtr ->LegLOffset  = glm::vec3(OffsetX, OffsetY, OffsetZ);
                
                return ActorPtr;
            }
            
        }
        
    }
    
    
}

