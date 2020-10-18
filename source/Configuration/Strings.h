//
// Constant strings

namespace strings {
    
    // String optimizations
    const std::string SpaceSingle       = " ";
    const std::string SpaceDouble       = SpaceSingle + SpaceSingle;
    const std::string DashSymbol        = "-";
    const std::string AddSymbol         = "+";
    const std::string FailSymbol        = "!";
    const std::string EqualSymbol       = "=";
    const std::string StreamSymbol      = "~";
    const std::string ColonSymbol       = ":";
    const std::string PeriodSymbol      = ".";
    const std::string ArrowLeft         = "<";
    const std::string ArrowRight        = ">";
    const std::string BracketLeft       = "[";
    const std::string BracketRight      = "]";
    
    // File extention names
    const std::string ExtActor           = PeriodSymbol + "actor";
    const std::string ExtScene           = PeriodSymbol + "scene";
    const std::string ExtDefinition      = PeriodSymbol + "definitions";
    const std::string ExtLocation        = PeriodSymbol + "locations";
    const std::string ExtComponent       = PeriodSymbol + "component";
    const std::string ExtPhysics         = PeriodSymbol + "physics";
    const std::string ExtCamera          = PeriodSymbol + "camera";
    const std::string ExtLight           = PeriodSymbol + "light";
    const std::string ExtEmitter         = PeriodSymbol + "emitter";
    const std::string ExtMesh            = PeriodSymbol + "mesh";
    const std::string ExtObj             = PeriodSymbol + "obj";
    const std::string ExtMaterial        = PeriodSymbol + "material";
    const std::string ExtShader          = PeriodSymbol + "shader";
    
    // Asset header definition string
    const std::string AssetIdentifier    = "define_asset";
    
    // Log asset names
    const std::string AssetTypeActor     = "Actor"      + ColonSymbol + SpaceSingle;
    const std::string AssetTypeCamera    = "Camera"     + ColonSymbol + SpaceSingle;
    const std::string AssetTypeLight     = "Light"      + ColonSymbol + SpaceSingle;
    const std::string AssetTypeEmitter   = "Emitter"    + ColonSymbol + SpaceSingle;
    const std::string AssetTypeMaterial  = "Material"   + ColonSymbol + SpaceSingle;
    const std::string AssetTypeMesh      = "Mesh"       + ColonSymbol + SpaceSingle;
    const std::string AssetTypeShader    = "Shader"     + ColonSymbol + SpaceSingle;
    const std::string AssetTypeBody      = "Body"       + ColonSymbol + SpaceSingle;
    const std::string AssetTypeComponent = "Component"  + ColonSymbol + SpaceSingle;
    const std::string AssetTypeScene     = "Scene"      + ColonSymbol + SpaceSingle;
    const std::string AssetTypeSky       = "Sky"        + ColonSymbol + SpaceSingle;
    const std::string AssetTypeCollider  = "Collider"   + ColonSymbol + SpaceSingle;
    
    // Shader program headers
    const std::string VertShaderHeader  = BracketLeft + "vertex_shader" + BracketRight;
    const std::string FragShaderHeader  = BracketLeft + "fragment_shader" + BracketRight;
    
    // Log strings
    const std::string RenderSystemInitialize  = ArrowLeft + "RenderSystem initialized" + ArrowRight;
    const std::string RenderSystemShutdown    = ArrowLeft + "RenderSystem shutting down" + ArrowRight;
    
    // Log status symbols
    const std::string LogStringLoad         = SpaceDouble + AddSymbol + SpaceSingle;
    const std::string LogStringFail         = SpaceDouble + FailSymbol + SpaceSingle;
    const std::string LogStringAdd          = SpaceDouble + SpaceDouble + AddSymbol + SpaceSingle;
    const std::string LogStringLink         = SpaceDouble + SpaceDouble + EqualSymbol + SpaceSingle;
    const std::string LogStringStream       = SpaceDouble + SpaceDouble + StreamSymbol + SpaceSingle;
    const std::string LogStringError        = SpaceDouble + SpaceDouble + FailSymbol + SpaceSingle;
    const std::string ErrorStringHeader     = SpaceDouble + SpaceDouble + SpaceDouble + DashSymbol + SpaceSingle;
    
    const std::string AssetResolution       = SpaceSingle + ArrowRight + SpaceSingle;
    
    const std::string MissingCorrupt        = " - Missing or corrupt.";
    const std::string NotFound              = " - Not found";
    
    
}

