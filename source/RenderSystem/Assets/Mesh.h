//
// Mesh object

struct Mesh {
    
    std::string Name;
    
    bool IsBufferLoaded;
    bool IsBufferIndexed;
    int PrimitiveType;
    
    unsigned int BufferAttribute;
    unsigned int BufferVertex;
    unsigned int BufferIndex;
    
    int VertexCount;
    int IndexCount;
    
    Material* AttachedMaterial;
    
    objl::Loader MeshLoader;
    
    // Physics collision mesh
    std::vector<objl::Vector3> PhysicsPositionList;
    std::vector<unsigned int>  PhysicsIndexList;
    
    Mesh();
    ~Mesh();
    
    // Material attachment
    bool  AttachMaterial(Material*);
    void  DetachMaterial(void);
    
};

Mesh :: Mesh() {
    
    // Initial Settings
    Name                 = "";
    IsBufferLoaded       = false;
    IsBufferIndexed      = false;
    PrimitiveType        = MESH_TRIANGLES;
    
    // Vertex buffers
    BufferAttribute=0;
    BufferVertex=0;
    BufferIndex=0;
    VertexCount=0;
    IndexCount=0;
    
    AttachedMaterial = nullptr;
    
}
Mesh :: ~Mesh() {
    
    // Check if the buffer is loaded
    if (IsBufferLoaded) {
        
        // Destroy the buffers
        glDeleteBuffers(1, &BufferVertex);
        glDeleteBuffers(1, &BufferAttribute);
        
        if (IsBufferIndexed) {glDeleteBuffers(1, &BufferIndex);}
        
    }
    
}

bool Mesh :: AttachMaterial(Material* MaterialPtr) {
    if (MaterialPtr == nullptr) {return 0;}
    AttachedMaterial = MaterialPtr;
    return true;
}                  
void Mesh :: DetachMaterial(void) {
    AttachedMaterial = nullptr;
}

