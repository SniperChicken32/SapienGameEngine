//
// Resource management

#ifndef RESOURCE_MANAGER_
#define RESOURCE_MANAGER_

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
                        std::vector<std::string> Array = StringExplode(String, ' ');
                        int MaxSz = Array.size();
                        if (MaxSz < 1) continue;
                        
                        // Check data block
                        if (Array[0] == "[begin]") {
                            
                            // Get block name
                            std::string BlockName = Array[1];
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
                            
                            ValueList.push_back(Array[i]);
                            
                        }
                        
                        AssetData.emplace(Array[0], ValueList);
                        
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
                
                int i=0;
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
    
    // Physics resource management
    namespace PhysicsManagement {
        
        // Collider object
        struct CollisionManafold {
            
            std::string AssetName;
            short int   AssetType;
            
            // Collider objects
            Physics::BoxShape*     CollisionBox;
            Physics::SphereShape*  CollisionSphere;
            Physics::CapsuleShape* CollisionCapsule;
            
            // Collision material
            float Bounciness;
            float FrictionCoefficient;
            float RollingResistance;
            float MassDensity;
            
            CollisionManafold() {
                
                AssetName = "";
                AssetType = 0;
                
                CollisionBox = nullptr;
                CollisionSphere = nullptr;
                CollisionCapsule = nullptr;
                
                Bounciness = 0.0;
                FrictionCoefficient = 0.0;
                RollingResistance = 0.0;
                MassDensity = 0.0;
                
            }
            ~CollisionManafold() {
                
            }
            
        };
        std::vector<CollisionManafold*> CollisionList;
        
        // Find collision object
        CollisionManafold* FindCollider(std::string AssetName) {
            
            CollisionManafold* CollisionPtr;
            for (std::vector<CollisionManafold*>::iterator it = CollisionList.begin(); it != CollisionList.end(); ++it) {
                CollisionPtr = *it;
                if (CollisionPtr ->AssetName == AssetName) {return CollisionPtr;}
            }
            return nullptr;
        }
        
        // Destroy ALL colliders in the list
        void DestroyColliders(void) {
            
            CollisionManafold* CollisionPtr;
            for (std::vector<CollisionManafold*>::iterator it = CollisionList.begin(); it != CollisionList.end(); ++it) {
                CollisionPtr = *it;
                delete CollisionPtr;
            }
            CollisionList.clear();
        }
        
        // Create collider objects
        CollisionManafold* AddBoxCollider(std::string AssetName, float x, float y, float z) {
            
            Physics::Vector3 BoxScale   = Physics::Vector3(x, y, z);
            Physics::BoxShape* Collider = PhysicsCommon ->createBoxShape(BoxScale);
            
            CollisionManafold* CollisionMfold = new CollisionManafold();
            
            CollisionMfold ->AssetName = AssetName;
            CollisionMfold ->AssetType = 0;
            CollisionMfold ->CollisionBox = Collider;
            
            CollisionList.push_back(CollisionMfold);
            
            return CollisionMfold;
        }
        CollisionManafold* AddSphereCollider(std::string AssetName, float Radius) {
            
            Physics::SphereShape* Collider = PhysicsCommon ->createSphereShape(Radius);
            
            CollisionManafold* CollisionMfold = new CollisionManafold();
            
            CollisionMfold ->AssetName = AssetName;
            CollisionMfold ->AssetType = 1;
            CollisionMfold ->CollisionSphere = Collider;
            
            CollisionList.push_back(CollisionMfold);
            
            return CollisionMfold;
        }
        CollisionManafold* AddCapsuleCollider(std::string AssetName, float Radius, float Length) {
            
            Physics::CapsuleShape* Collider = PhysicsCommon ->createCapsuleShape(Radius, Length);
            
            CollisionManafold* CollisionMfold = new CollisionManafold();
            
            CollisionMfold ->AssetName = AssetName;
            CollisionMfold ->AssetType = 2;
            CollisionMfold ->CollisionCapsule = Collider;
            
            CollisionList.push_back(CollisionMfold);
            
            return CollisionMfold;
        }
        
        // Create a rigid body
        Physics::RigidBody* CreateRigidBody(double x=0.0, double y=0.0, double z=0.0) {
            
            Physics::Vector3    Position   = Physics::Vector3(x, y, z);
            Physics::Quaternion Quaternion = Physics::Quaternion::identity();
            
            Physics::Transform BodyTransform = Physics::Transform(Position, Quaternion);
            Physics::RigidBody* RigidBody = PhysicsWorld ->createRigidBody(BodyTransform);
            
            return RigidBody;
        }
        
    }
    
    // Asset file parsers
    namespace AssetParsers {
        
        //
        // String parsers
        namespace Parsers {
            
            // Define an asset from a string
            void ParseDefinitionString(std::string DefinitionString) {
                
                // Explode the string
                int i=0; std::string Strings[50];
                std::vector<std::string> Array = StringExplode(DefinitionString, ' ');
                for (std::vector<std::string>::iterator it = Array.begin(); it != Array.end(); ++it) 
                {std::string String = *it; Strings[i] = String; i++;}
                
                // Check blank string
                if (Strings[0] == "") {return;}
                if (StringFind("//", Strings[0])) {return;}
                
                //
                // Asset definitions
                
                // Collision
                if (Strings[0] == "define_collider") {
                    
                    // Check if the asset is already loaded
                    if (PhysicsManagement::FindCollider( Strings[1] ) != nullptr) return;
                    
                    std::string AssetPath = Strings[1];
                    std::string AssetType = Strings[2];
                    
                    if (AssetType == "box") {
                        
                        float xx = StringToFloat(Strings[3]);
                        float yy = StringToFloat(Strings[4]);
                        float zz = StringToFloat(Strings[5]);
                        
                        PhysicsManagement::AddBoxCollider(Strings[1], xx, yy, zz);
                        
#ifdef  DEVELOPMENT_MODE_
                        LogWrite(strings::LogStringAdd + strings::AssetTypeCollider + Strings[1]);
#endif
                        
                        return;
                    }
                    
                    if (AssetType == "sphere") {
                        
                        float Radius = StringToFloat(Strings[3]);
                        
                        PhysicsManagement::AddSphereCollider(Strings[1], Radius);
                        
#ifdef  DEVELOPMENT_MODE_
                        LogWrite(strings::LogStringAdd + strings::AssetTypeCollider + Strings[1]);
#endif
                        
                        return;
                    }
                    
                    if (AssetType == "capsule") {
                        
                        float Radius = StringToFloat(Strings[3]);
                        float Height = StringToFloat(Strings[4]);
                        
                        PhysicsManagement::AddCapsuleCollider(Strings[1], Radius, Height);
                        
#ifdef  DEVELOPMENT_MODE_
                        LogWrite(strings::LogStringAdd + strings::AssetTypeCollider + Strings[1]);
#endif
                        
                        return;
                    }
                    
                    // Incorrect collider type
                    LogWrite(strings::LogStringError + strings::AssetTypeCollider + Strings[1] + " :: Incorrect collider type");
                    
                    return;
                }
                
                // Render pipeline objects
                // Mesh > Material > Shader
                if (Strings[0] == "define_mesh") {
                    
                    ResourceManagement::Loaders::LoadMesh( Strings[1] );
                    
                    return;
                }
                if (Strings[0] == "define_material") {
                    
                    ResourceManagement::Loaders::LoadMaterial( Strings[1] );
                    
                    return;
                }
                if (Strings[0] == "define_shader") {
                    
                    ResourceManagement::Loaders::LoadShader( Strings[1] );
                    
                    return;
                }
                if (Strings[0] == "link") {
                    
                    // Find required assets for linking
                    Mesh*     MeshPtr     = Renderer ->FindMesh( Strings[1] );
                    Material* MaterialPtr = Renderer ->FindMaterial( Strings[2] );
                    Shader*   ShaderPtr   = Renderer ->FindShader( Strings[3] );
                    
                    if (MeshPtr != nullptr) {
                        
                        if (MaterialPtr != nullptr) {
                            
                            // Material > mesh
                            MeshPtr ->AttachMaterial(MaterialPtr);
                            
                            if (ShaderPtr != nullptr) {
                                
                                // Shader > material
                                MaterialPtr ->AttachShader(ShaderPtr);
                                
#ifdef  DEVELOPMENT_MODE_
                                LogWrite(strings::LogStringAdd + "Link: " + Strings[1] + strings::AssetResolution + Strings[2] + strings::AssetResolution + Strings[3]);
#endif
                                
                                return;
                            }
                            
                            // Shader link failed
                            LogWrite(strings::LogStringError + "Link failed: " + Strings[3]);
                            
                            return;
                        }
                        
                        // Material link failed
                        LogWrite(strings::LogStringError + "Link failed: " + Strings[2]);
                        
                        return;
                    }
                    
                    // Mesh link failed
                    LogWrite(strings::LogStringError + "Link failed: " + Strings[1]);
                    
                    return;
                }
                
                // Advanced renderable objects
                if (Strings[0] == "define_emitter") {
                    
                    return;
                }
                if (Strings[0] == "define_light") {
                    
                    ResourceManagement::Loaders::LoadLight( Strings[1] );
                    
                    return;
                }
                if (Strings[0] == "define_camera") {
                    
                    Camera* CameraPtr = ResourceManagement::Loaders::LoadCamera( Strings[1] );
                    if (CameraPtr == nullptr) return;
                    
                    // Bind the camera to the render system
                    Renderer ->CurrentCamera = CameraPtr;
                    
                    return;
                }
                if (Strings[0] == "define_camera_controller") {
                    
                    if (Renderer ->CurrentCamera == nullptr) {
                        
                        LogWrite(strings::LogStringFail + strings::ColonSymbol + ": Must define a camera before camera physics");
                        
                        return;
                    }
                    
                    // Check if the camera already has physics
                    if (Renderer ->CurrentCamera ->AttachedBody != nullptr) return;
                    
#ifdef  LOG_DETAILS_
                    LogWrite(strings::LogStringAdd + "camera_controller");
#endif
                    
                    // Camera starting position
                    float xx = StringToFloat(Strings[1]);
                    float yy = StringToFloat(Strings[2]);
                    float zz = StringToFloat(Strings[3]);
                    
                    // Collider scale
                    SCALE ColliderScale;
                    ColliderScale.x = StringToFloat(Strings[4]);
                    ColliderScale.y = StringToFloat(Strings[5]);
                    ColliderScale.z = StringToFloat(Strings[6]);
                    
                    // Mass, damping
                    float Mass        = StringToFloat(Strings[7]);
                    float LinearDamp  = StringToFloat(Strings[8]);
                    float AngularDamp = StringToFloat(Strings[9]);
                    
                    // Velocities
                    float MoveVelocity = StringToFloat(Strings[10]) * 0.01;
                    float JumpVelocity = StringToFloat(Strings[11]) * 0.1;
                    
                    
                    //
                    // Apply settings to camera object
                    Camera* CurrentCamera = Renderer ->CurrentCamera;
                    CurrentCamera ->MoveVelocity = MoveVelocity;
                    CurrentCamera ->JumpVelocity = JumpVelocity;
                    
                    // Set the camera height offset
                    CurrentCamera ->CameraHeight = ColliderScale.z;
                    
                    // Construct a rigid body
                    Physics::Transform BodyTransform = Physics::Transform(Physics::Vector3(xx, yy, zz), Physics::Quaternion::identity());
                    Physics::RigidBody* RigidBody = PhysicsWorld ->createRigidBody(BodyTransform);
                    
                    // Attach the body to the current camera
                    CurrentCamera ->AttachedBody = RigidBody;
                    
                    // Setup the rigid body
                    RigidBody ->setType( Physics::BodyType::DYNAMIC );
                    RigidBody ->enableGravity       (true);
                    RigidBody ->setIsAllowedToSleep (false);
                    
                    RigidBody ->setMass( Mass );
                    RigidBody ->setLinearDamping( LinearDamp );
                    RigidBody ->setAngularDamping( AngularDamp );
                    
                    
                    
                    //
                    // Construct a box collider
                    Physics::Transform ColliderTransform = Physics::Transform::identity();
                    
                    Physics::Vector3 ScaleVector = Physics::Vector3(ColliderScale.x, ColliderScale.y, ColliderScale.z);
                    Physics::BoxShape* ColliderShape = PhysicsCommon ->createBoxShape( ScaleVector );
                    
                    Physics::Collider* Collider; 
                    Collider = RigidBody ->addCollider(ColliderShape, ColliderTransform);
                    
                    // Setup collider material
                    //Physics::Material& MaterialPtr = Collider ->getMaterial();
                    
                    //MaterialPtr.setBounciness(0.0);
                    //MaterialPtr.setFrictionCoefficient(0.0);
                    //MaterialPtr.setRollingResistance
                    //MaterialPtr.setMassDensity
                    
                    //RigidBody ->updateLocalCenterOfMassFromColliders();
                    //RigidBody ->updateLocalInertiaTensorFromColliders();
                    //RigidBody ->updateMassFromColliders();
                    
                    return;
                }
                
                // Special effects
                if (Strings[0] == "define_sky") {
                    
                    // Check if a sky has already been assigned
                    if (Renderer ->CurrentSky != nullptr) return;
                    
                    //
                    // Build entity
                    Entity* EntityPtr = Renderer ->CreateEntity(0.0, 0.0, 0.0);
                    
                    // Bind the sky to the render system
                    Renderer ->AttachSky( EntityPtr );
                    
                    // Sky orientation
                    EntityPtr ->Rotation.y =  180.0;
                    EntityPtr ->Rotation.p =    0.0;
                    EntityPtr ->Rotation.r =    0.0;
                    
                    // Sky scale
                    float SkyScale = 1000000.0;
                    EntityPtr ->Scale.x  = SkyScale;
                    EntityPtr ->Scale.y  = SkyScale;
                    EntityPtr ->Scale.z  = SkyScale;
                    
                    EntityPtr ->SetRenderQueueGroup(RENDER_QUEUE_SKY_EARLY);
                    EntityPtr ->SetRenderDistance( SkyScale );
                    
                    // Find mesh asset
                    Mesh* MeshPtr = Renderer ->FindMesh( Strings[1] );
                    if (MeshPtr != nullptr) {
                        
                        // Attach the mesh asset
                        EntityPtr ->AttachMesh( MeshPtr );
                        
#ifdef  DEVELOPMENT_MODE_
                        LogWrite(strings::LogStringAdd + strings::AssetTypeSky + Strings[1]);
#endif
                        
                        return;
                    }
                    
#ifdef  DEVELOPMENT_MODE_
                    LogWrite(strings::LogStringError + strings::AssetTypeSky + Strings[1]);
#endif
                    
                    Renderer ->DestroyEntity( EntityPtr );
                    return;
                }
                if (Strings[0] == "define_fog") {
                    
                }
                
                // Actor
                if (Strings[0] == "define_actor") {
                    
                    // Find the asset by name
                    std::string ActorAsset = AssetFilePeekHeader( Strings[1] );
                    Actor* ActorPtr = ActorAI::AI ->FindActor( ActorAsset );
                    
                    if (ActorPtr == nullptr) {
                        
                        ActorPtr = ActorAI::AI ->CreateActor();
                        ResourceManagement::Loaders::LoadActor( Strings[1] );
                        
                    }
                    
                    return;
                }
                
            }
            
            // Place an entity instance from a string
            void ParseLocationString(Scene* ScenePointer, std::string FilePath, std::string LocationString) {
                
#ifdef  LOG_DETAILS_
                // Log the entity location
                std::string LogString = strings::LogStringAdd;
#endif
                
                // Explode the string
                int i=0; std::string Strings[50];
                std::vector<std::string> Array = StringExplode(LocationString, ' ');
                for (std::vector<std::string>::iterator it = Array.begin(); it != Array.end(); ++it) 
                {std::string String = *it; Strings[i] = String; i++;}
                
                // Check blank string
                if (Strings[0] == "") {return;}
                if (StringFind("//", Strings[0])) {return;}
                
                //
                // Get position, orientation
                
                POSITION Position;
                Position.x = StringToDouble(Strings[1]);
                Position.y = StringToDouble(Strings[2]);
                Position.z = StringToDouble(Strings[3]);
                
                ROTATION Rotation;
                Rotation.y = StringToFloat(Strings[4]);
                Rotation.p = StringToFloat(Strings[5]);
                Rotation.r = StringToFloat(Strings[6]);
                
                
                
                //
                // Check assets for entity type
                
                // Check meshes
                Mesh* MeshPtr = Renderer ->FindMesh( Strings[0] );
                if (MeshPtr != nullptr) {
                    
                    // Get the scale for the entity
                    SCALE Scale;
                    Scale.x = StringToFloat(Strings[7]);
                    Scale.y = StringToFloat(Strings[8]);
                    Scale.z = StringToFloat(Strings[9]);
                    
                    // Get entity physics
                    std::string Collider = Strings[10];
                    float Mass     = StringToFloat( Strings[11] );
                    float LinDamp  = StringToFloat( Strings[12] );
                    float AngDamp  = StringToFloat( Strings[13] );
                    
                    
#ifdef  LOG_DETAILS_
                    LogString += Strings[0] + strings::SpaceDouble;
                    LogString += Strings[1] + strings::SpaceSingle + Strings[2] + strings::SpaceSingle + Strings[3] + strings::SpaceDouble;
#endif
                    
                    //
                    // Build entity
                    Entity* EntityPtr = Renderer ->CreateEntity(Position.x, Position.y, Position.z);
                    
                    EntityPtr ->Rotation = Rotation;
                    EntityPtr ->Scale    = Scale;
                    
                    // Attach the mesh asset
                    EntityPtr ->AttachMesh( MeshPtr );
                    EntityPtr ->ColliderName = Collider;
                    
                    // Remember the original file path
                    EntityPtr ->FilePath = FilePath;
                    
                    // Add the entity to the scene graph
                    ScenePointer ->AddEntity( EntityPtr );
                    
                    
                    //
                    // Setup entity physics
                    
                    // Construct a new rigid body
                    Physics::RigidBody* RigidBody = PhysicsManagement::CreateRigidBody(Position.x, Position.y, Position.z);
                    EntityPtr ->AttachedBody = RigidBody;
                    
                    RigidBody ->setIsAllowedToSleep(true);
                    
                    // Find the collision object
                    PhysicsManagement::CollisionManafold* CollisionMfold = PhysicsManagement::FindCollider( Collider );
                    if (CollisionMfold == nullptr) return;
                    
                    // Create the collision object
                    Physics::Transform  Trans = Physics::Transform(Physics::Vector3(0.0, 0.0, 0.0), Physics::Quaternion::identity());
                    
                    // Check body type dynamic/static
                    if (Mass <= 0.0) {
                        
#ifdef  LOG_DETAILS_
                        LogString += "static";
#endif
                        
                        // Static
                        RigidBody ->setType( Physics::BodyType::STATIC );
                        
                        RigidBody ->setMass( Mass );
                        
                        RigidBody ->setLinearDamping ( LinDamp );
                        RigidBody ->setAngularDamping( AngDamp );
                        
                    } else {
                        
#ifdef  LOG_DETAILS_
                        LogString += "dynamic";
#endif
                        
                        // Dynamic
                        RigidBody ->setType( Physics::BodyType::DYNAMIC );
                        
                        RigidBody ->setMass( Mass );
                        
                        RigidBody ->setLinearDamping ( LinDamp );
                        RigidBody ->setAngularDamping( AngDamp );
                        
                    }
                    
                    RigidBody ->updateLocalInertiaTensorFromColliders();
                    //RigidBody ->updateMassPropertiesFromColliders();
                    
                    //
                    // Check collider type
                    // Type 0 - box
                    if (CollisionMfold ->AssetType == 0) {RigidBody ->addCollider( CollisionMfold ->CollisionBox, Trans );}
                    // Type 1 - sphere
                    if (CollisionMfold ->AssetType == 1) {RigidBody ->addCollider( CollisionMfold ->CollisionSphere, Trans );}
                    // Type 2 - capsule
                    if (CollisionMfold ->AssetType == 2) {RigidBody ->addCollider( CollisionMfold ->CollisionCapsule, Trans );}
                    
                    
                    //
                    // Process attachments
                    for (i=0; i<32; i++) {
                        
                        std::string AttachmentString = Strings[14 + i];
                        
                        if (AttachmentString == "") continue;
                        
                        //
                        // Find the asset by name
                        
                        // Light
                        Light* LightPtr = Renderer ->FindLight( AttachmentString );
                        if (LightPtr != nullptr) {
                            
#ifdef  LOG_DETAILS_
                            LogString += strings::SpaceDouble + strings::AddSymbol + AttachmentString + strings::SpaceDouble;
#endif
                            
                            // Create new asset
                            Light* LightNew = Renderer ->CreateLight();
                            EntityPtr ->AddLight( LightNew );
                            LightNew ->OriginalName = LightPtr ->Name;
                            
                            //
                            // Copy the asset data
                            
                            LightNew ->Position = Position;
                            LightNew ->Rotation = Rotation;
                            
                            LightNew ->IsActive        = true;
                            LightNew ->LightType       = LightPtr ->LightType;
                            
                            LightNew ->Diffuse         = LightPtr ->Diffuse;
                            LightNew ->Specular        = LightPtr ->Specular;
                            
                            LightNew ->RenderDistance  = LightPtr ->RenderDistance;
                            
                            LightNew ->Range           = LightPtr ->Range;
                            LightNew ->Intensity       = LightPtr ->Intensity;
                            
                            LightNew ->Constant        = LightPtr ->Constant;
                            LightNew ->Linear          = LightPtr ->Linear;
                            LightNew ->Quadratic       = LightPtr ->Quadratic;
                            LightNew ->Focus           = LightPtr ->Focus;
                            
                            continue;
                        }
                        
                    }
                    
                    
                    
#ifdef  LOG_DETAILS_
                    LogWrite(LogString);
#endif
                    
                    return;
                }
                
                // Check actors
                Actor* ActorPtr = ActorAI::AI ->FindActor( Strings[0] );
                if (ActorPtr != nullptr) {
                    
                    // Create an actor object
                    Actor* NewActorPtr = ActorAI::AI ->CreateActor(Position.x, Position.y, Position.z);
                    NewActorPtr ->IsActive = true;
                    
                    NewActorPtr ->Name = "";
                    NewActorPtr ->OriginalName = ActorPtr ->Name;
                    
                    // Add the actor to the scene
                    ScenePointer ->AddActor( NewActorPtr );
                    
                    //
                    // Copy asset settings
                    NewActorPtr ->DoAnimationStates  = ActorPtr ->DoAnimationStates;
                    NewActorPtr ->LimbSwingRangeWalk = ActorPtr ->LimbSwingRangeWalk;
                    NewActorPtr ->LimbSwingRateWalk  = ActorPtr ->LimbSwingRateWalk;
                    NewActorPtr ->LimbSwingRangeRun  = ActorPtr ->LimbSwingRangeRun;
                    NewActorPtr ->LimbSwingRateRun   = ActorPtr ->LimbSwingRateRun;
                    
                    NewActorPtr ->AnimationCurrent = ANIMATION_NONE;
                    NewActorPtr ->AnimationIdle    = ActorPtr ->AnimationIdle;
                    NewActorPtr ->AnimationWalk    = ActorPtr ->AnimationWalk;
                    NewActorPtr ->AnimationRun     = ActorPtr ->AnimationRun;
                    NewActorPtr ->AnimationDeath   = ActorPtr ->AnimationDeath;
                    
                    //
                    // Find the mesh assets
                    Mesh* HeadMesh  = Renderer ->FindMesh(ActorPtr ->HeadName);
                    Mesh* TorsoMesh = Renderer ->FindMesh(ActorPtr ->TorsoName);
                    Mesh* ArmRMesh  = Renderer ->FindMesh(ActorPtr ->ArmRName);
                    Mesh* ArmLMesh  = Renderer ->FindMesh(ActorPtr ->ArmLName);
                    Mesh* LegRMesh  = Renderer ->FindMesh(ActorPtr ->LegRName);
                    Mesh* LegLMesh  = Renderer ->FindMesh(ActorPtr ->LegLName);
                    
                    //
                    // Create entities
                    NewActorPtr ->HeadEntity  = Renderer ->CreateEntity();
                    NewActorPtr ->TorsoEntity = Renderer ->CreateEntity();
                    NewActorPtr ->ArmREntity  = Renderer ->CreateEntity();
                    NewActorPtr ->ArmLEntity  = Renderer ->CreateEntity();
                    NewActorPtr ->LegREntity  = Renderer ->CreateEntity();
                    NewActorPtr ->LegLEntity  = Renderer ->CreateEntity();
                    
                    // Set entity names
                    NewActorPtr ->HeadEntity ->Name  = "Head";
                    NewActorPtr ->TorsoEntity ->Name = "Torso";
                    NewActorPtr ->ArmREntity ->Name  = "ArmR";
                    NewActorPtr ->ArmLEntity ->Name  = "ArmL";
                    NewActorPtr ->LegREntity ->Name  = "LegR";
                    NewActorPtr ->LegLEntity ->Name  = "LegL";
                    
                    // Set translation offsets
                    NewActorPtr ->HeadEntity ->PositionOffset  = ActorPtr ->HeadOffset;
                    NewActorPtr ->TorsoEntity ->PositionOffset = ActorPtr ->TorsoOffset;
                    NewActorPtr ->ArmREntity ->PositionOffset  = ActorPtr ->ArmROffset;
                    NewActorPtr ->ArmLEntity ->PositionOffset  = ActorPtr ->ArmLOffset;
                    NewActorPtr ->LegREntity ->PositionOffset  = ActorPtr ->LegROffset;
                    NewActorPtr ->LegLEntity ->PositionOffset  = ActorPtr ->LegLOffset;
                    
                    // Attach meshes
                    NewActorPtr ->HeadEntity ->AttachMesh(HeadMesh);
                    NewActorPtr ->TorsoEntity ->AttachMesh(TorsoMesh);
                    NewActorPtr ->ArmREntity ->AttachMesh(ArmRMesh);
                    NewActorPtr ->ArmLEntity ->AttachMesh(ArmLMesh);
                    NewActorPtr ->LegREntity ->AttachMesh(LegRMesh);
                    NewActorPtr ->LegLEntity ->AttachMesh(LegLMesh);
                    
                    
                    // Get entity physics
                    std::string Collider = Strings[7];
                    
                    
                    //
                    // Setup entity physics
                    
                    // Construct a new rigid body
                    Physics::RigidBody* RigidBody = PhysicsManagement::CreateRigidBody(Position.x, Position.y, Position.z);
                    NewActorPtr ->AttachedBody = RigidBody;
                    NewActorPtr ->ColliderName = Collider;
                    
                    RigidBody ->setIsAllowedToSleep(true);
                    
                    // Find the collision object
                    PhysicsManagement::CollisionManafold* CollisionMfold = PhysicsManagement::FindCollider( Collider );
                    if (CollisionMfold == nullptr) return;
                    
                    // Create the collision object
                    Physics::Transform  Transform = Physics::Transform(Physics::Vector3(0.0, 0.0, 0.0), Physics::Quaternion::identity());
                    
                    // Dynamic
                    RigidBody ->setType( Physics::BodyType::DYNAMIC );
                    
                    RigidBody ->setMass( 0.85 );
                    
                    RigidBody ->setLinearDamping ( 0.5 );
                    RigidBody ->setAngularDamping( 0.0 );
                    
                    
                    RigidBody ->updateLocalInertiaTensorFromColliders();
                    //RigidBody ->updateMassPropertiesFromColliders();
                    
                    //
                    // Check collider type
                    // Type 0 - box
                    if (CollisionMfold ->AssetType == 0) {RigidBody ->addCollider( CollisionMfold ->CollisionBox, Transform );}
                    // Type 1 - sphere
                    if (CollisionMfold ->AssetType == 1) {RigidBody ->addCollider( CollisionMfold ->CollisionSphere, Transform );}
                    // Type 2 - capsule
                    if (CollisionMfold ->AssetType == 2) {RigidBody ->addCollider( CollisionMfold ->CollisionCapsule, Transform );}
                    
#ifdef  LOG_DETAILS_
                    LogString += Strings[0] + strings::SpaceDouble;
                    LogString += Strings[1] + strings::SpaceSingle + Strings[2] + strings::SpaceSingle + Strings[3] + strings::SpaceDouble;
                    LogWrite(LogString);
#endif
                    
                    return;
                }
                
#ifdef  LOG_DETAILS_
                LogString = strings::LogStringError + Strings[0] + strings::SpaceDouble;
                LogWrite(LogString);
#endif
                
            }
            
        }
        
        //
        // Load ".definitions" file
        bool LoadDefinitions(std::string PathName) {
            
            std::ifstream FileStream;
            FileStream.open( PathName );
            
            if (FileStream.is_open()) {
                
#ifdef  DEVELOPMENT_MODE_
                LogWrite("");
                LogWrite(strings::LogStringLoad + "Definition: " + PathName);
#endif
                std::string Line;
                
                // Loop through the file
                while (getline(FileStream, Line)) {
                    
                    if (Line == "") continue;
                    if (StringFind("//", Line)) continue;
                    
                    Parsers::ParseDefinitionString(Line);
                    
                }
                
                // Close the file
                FileStream.close();
                
            } else {LogWrite(strings::LogStringFail + PathName); return false;}
            
            return true;
        }
        
        // Load ".locations" file
        bool LoadLocations(Scene* ScenePointer, std::string PathName) {
            
            // Save file path
            ScenePointer ->Path = PathName;
            
            std::ifstream FileLocs;
            FileLocs.open( PathName );
            
            if (FileLocs.is_open()) {
                
#ifdef  DEVELOPMENT_MODE_
                LogWrite("");
                LogWrite(strings::LogStringLoad + "Locations: " + PathName);
#endif
                
                // Loop through the file
                
                std::string Line;
                
                while (getline(FileLocs, Line)) {
                    
                    if (Line == "") continue;
                    if (StringFind("//", Line)) continue;
                    
                    Parsers::ParseLocationString(ScenePointer, PathName, Line);
                    
                }
                
                FileLocs.close();
                
            } else {LogWrite(strings::LogStringFail + PathName); return false;}
            
            return false;
        }
        
    }
    
}

#endif

