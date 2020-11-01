//
// Scene management

#ifndef SCENE_MANAGEMENT_
#define SCENE_MANAGEMENT_

/** Peeks into an asset file and returns the header name.*/
std::string AssetFilePeekHeader(std::string FileName) {
    
    std::string Line;
    std::ifstream FileStream;
    
    FileStream.open( FileName );
    
    if (FileStream.is_open()) {
        
        getline(FileStream, Line);
        
        // Explode the string by spaces
        std::vector<std::string> Array = StringExplode(Line, ' ');
        if (Array.size() < 1) return "";
        
        FileStream.close();
        
        // Return asset header name
        return Array[1];
    }
    
    return "";
}

// Scene object manager
#include "SceneSystem.h"

// Resource loader and management
#include "ResourceManager.h"

// Physics proxy manager
#include "PhysicsManager.h"

// Scene file parsers
#include "Parsers.h"

#endif

