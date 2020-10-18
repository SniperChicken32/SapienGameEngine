//
// Platform specific functions

#include "Strings.h"

#ifndef STRUCT_GENERIC_
#define STRUCT_GENERIC_

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
std::string GetFileName(std::string FilePath) {
    
    char seperator = '/';
    
    // Get last dot position
    //std::size_t dotPos = FilePath.rfind('.');
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
    
    std::string Line;
    std::ifstream FileStream;
    
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


