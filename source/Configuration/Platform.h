//
// Platform specific functions

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



#ifndef STRUCT_GENERIC_
#define STRUCT_GENERIC_

// Viewport area
struct   VIEWPORT   {int x, y, w, h;};

// Colors
#define  COLOR    glm::vec3
#define  COLORA   glm::vec4

// Translation
#define  POSITION   glm::vec3
#define  ROTATION   glm::vec3
#define  SCALE      glm::vec3

#endif

#ifndef PLATFORM_FUNCTIONS_
#define PLATFORM_FUNCTIONS_

// General purpose timer
class Timer {
    private:
    
    double TimeFrequency;
    LARGE_INTEGER tLast;
    
    public:
    
    Timer() {
        
        LARGE_INTEGER tFrequency;
        
        // Get precise frequency
        QueryPerformanceFrequency(&tFrequency);
        TimeFrequency = tFrequency.QuadPart / 1000.0;
        
        // Initiate timer
        QueryPerformanceCounter(&tLast);
    }
    ~Timer() {
        
    }
    
    double Check (bool Reset=false)  {
        LARGE_INTEGER tCurrent;
        
        QueryPerformanceCounter(&tCurrent);
        
        double Delta = (tCurrent.QuadPart - tLast.QuadPart) / TimeFrequency;
        
        if (Reset) {tLast = tCurrent;}
        return Delta;
    }
    
    void Reset (void) {QueryPerformanceCounter(&tLast);}
    
};

//
// String functions
bool   StringFind(std::string SubString, std::string String) {
    if (strstr ( String.c_str(), SubString.c_str() ) != NULL) {return 1;}
    return 0;
}
size_t StringLen(std::string String) {
    size_t StrSz = strlen( String.c_str() );
    return StrSz;
}
std::vector<std::string> StringExplode(std::string String, const char Char) {
	
	std::vector<std::string> result;
    std::istringstream iss(String);
    
    for (std::string token; std::getline(iss, token, Char); ) {
        
        if (std::move(token) != "") {
            result.push_back(std::move(token));
        }
        
    }
    
    return result;
}

// Type conversion
std::string FloatToString(float Float) {
    std::stringstream sStream;
    sStream << Float;
    std::string String = sStream.str();
    if (!StringFind(".", String)) {String += ".0";}
    return String;
}
float       StringToFloat(std::string String) {
    float Float;
    std::stringstream(String) >> Float;
    return Float;
}
std::string DoubleToString(double Double) {
    std::stringstream sStream;
    sStream << Double;
    std::string String = sStream.str();
    if (!StringFind(".", String)) {String += ".0";}
    return String;
}
double      StringToDouble(std::string String) {
    double Double;
    std::stringstream(String) >> Double;
    return Double;
}
std::string IntToString(int Integer) {
    std::stringstream sStream;
    sStream << Integer;
    return sStream.str();
}
int         StringToInt(std::string String) {
    int Integer;
    std::stringstream(String) >> Integer;
    return Integer;
}

// Return a filename from a path
std::string GetFileName(std::string FilePath, bool AddExtension) {
    
    char seperator = '/';
    
    // Get last dot position
    std::size_t dotPos = FilePath.rfind('.');
    std::size_t sepPos = FilePath.rfind(seperator);
    if(sepPos != std::string::npos) {
        return FilePath.substr(sepPos + 1, FilePath.size());
    }
    
    return "";
}

// Message box
void Msg(std::string vText) {MessageBox(NULL, vText.c_str(), "Message box", MB_OK);}
void Msg(const char* vText) {MessageBox(NULL, vText, "Message box", MB_OK);}
void Msg(int vText) {
    std::string String = IntToString(vText);
    const char* ConstString = String.c_str();
    MessageBox(NULL, ConstString, "Message box", MB_OK);
}
void Msg(float vText) {
    std::string String = FloatToString(vText);
    const char* ConstString = String.c_str();
    MessageBox(NULL, ConstString, "Message box", MB_OK);
}
void Msg(double vText) {
    std::string String = DoubleToString(vText);
    const char* ConstString = String.c_str();
    MessageBox(NULL, ConstString, "Message box", MB_OK);
}

// Mouse functions
void      CursorSetPosition(int xx, int yy) {
    SetCursorPos( xx, yy );
}
glm::vec2 CursorGetPosition(void) {
    POINT Pos;
    glm::vec2 MousePos;
    
    GetCursorPos(&Pos);
    
    MousePos.x = Pos.x;
    MousePos.y = Pos.y;
    
    return MousePos;
}

// 3D math
const float PI = 3.14159;

float AngleBetweenPoints(glm::vec3 VecA, glm::vec3 VecB) {
    float x1 = VecA.x;
    float y1 = VecA.y;
    //float z1 = VecA.z;
    
    float x2 = VecB.x;
    float y2 = VecB.y;
    //float z2 = VecB.z;
    
    float xo = x2 - x1;
    float yo = y2 - y1;
    //float zo = z2 - z1;
    
    return -std::atan2(yo, xo) * 180.0 / PI;
    
    //float angle  = VectorDotProduct(aX, aY, aZ, bX, bY, bZ);
    //angle       /= (VectorMagnitude(aX, aY, aZ) * VectorMagnitude(bX, bY, bZ));
    //return acosf(angle);
}

// Degree / radian conversion
double RadToDeg(double vRadian) {return ( vRadian * (180 / PI));}
double DegToRad(double vDegree) {return ((vDegree * 180) / PI); }

// Logging
void LogWrite(std::string LogString) {
    std::ofstream  FileID;
    
    FileID.open(EVENT_LOG_FILENAME, std::ifstream::app);
    FileID << LogString << "\n";
    
    FileID.close();
}
void LogWriteLn(void) {
    std::ofstream  FileID;
    
    FileID.open(EVENT_LOG_FILENAME, std::ifstream::app);
    FileID << "\n";
    
    FileID.close();
}
void LogClear(void) {
    
    std::ifstream  FileID;
    bool FileExists=false;
    
    FileID.open(EVENT_LOG_FILENAME, std::ifstream::in);
    if (FileID.good()) {FileExists=true;}
    FileID.close();
    
    if (FileExists) {remove(EVENT_LOG_FILENAME.c_str());}
    
}

void FileWriteLine(std::string Path, std::string String) {
    std::ofstream  FileID;
    
    FileID.open(Path, std::ifstream::out);
    FileID << String << "\n";
    
    FileID.close();
}

// Get an asset file`s header name
std::string AssetFilePeekHeader(std::string FileName) {
    
    int i;
    std::string Line;
    std::ifstream FileStream;
    std::vector<std::string> Array;
    
    FileStream.open( FileName );
    
    // Check file opened
    if (FileStream.is_open()) {
        
        getline(FileStream, Line);
        
        // Explode the string
        int i=0; std::string Strings[25];
        std::vector<std::string> Array = StringExplode(Line, ' ');
        for (std::vector<std::string>::iterator it = Array.begin(); it != Array.end(); ++it) {std::string String = *it; Strings[i] = String; i++;}
        
        FileStream.close();
        
        // Return asset header name
        return Strings[1];
        
    }
    
    return "";
}

// Get a wavefront model file`s name
std::string ObjFilePeekHeader(std::string FileName) {
    
    int i;
    std::string Line;
    std::ifstream FileStream;
    
    FileStream.open( FileName );
    
    // Check file opened
    if (FileStream.is_open()) {
        
        while(getline(FileStream, Line)) {
            
            // Explode the string
            int i=0; std::string Strings[25];
            std::vector<std::string> Array = StringExplode(Line, ' ');
            for (std::vector<std::string>::iterator it = Array.begin(); it != Array.end(); ++it) {std::string String = *it; Strings[i] = String; i++;}
            
            if (Strings[0] == "g") {
                
                return Strings[1];
            }
            
        }
        
        FileStream.close();
    }
    
    return "";
}

#endif


