//
// Scene manager front-end API

#ifndef SCENE_MANAGEMENT_API_
#define SCENE_MANAGEMENT_API_

#include "SceneManager/SceneSystem.h"
#include "SceneManager/ResourceManager.h"
#include "SceneManager/PhysicsManager.h"
#include "SceneManager/Parsers.h"

//
// Scene management system
namespace SceneManager {
    
    // Load a ".scene" file
    Scene* SceneLoad(std::string FilePath) {
        
        // Load and check the asset
        SceneManager::ResourceManagement::AssetLoader AssetLoader( FilePath + strings::ExtScene );
        if (!AssetLoader.IsAssetLoaded()) {
            
            LogWrite(strings::LogStringError + strings::AssetTypeScene + FilePath);
            
            return nullptr;
        }
        
        // If the asset already exists, add the scene to the current scene graph
        Scene* ScenePtr = SceneMgr ->FindScene( AssetLoader.AssetName );
        if (ScenePtr == nullptr) {
            
            // Create the asset
            ScenePtr = SceneMgr ->CreateScene();
            
            // Name the asset
            ScenePtr ->Name = AssetLoader.AssetName;
            
        }
        
        
        LogWrite(strings::AddSymbol + strings::SpaceSingle + strings::AssetTypeScene + AssetLoader.AssetName);
        
        //
        // Process scene definitions
        for (std::map<std::string, std::vector<std::string>>::iterator it = AssetLoader.AssetData.begin(); it != AssetLoader.AssetData.end(); it++) {
            
            // Get the path to the definition/location file
            std::string FilePath = it ->first;
            
            if (StringFind(strings::ExtDefinition, FilePath)) {
                
                SceneManager::AssetParsers::LoadDefinitions( FilePath );
                ScenePtr ->DefinitionsFiles.push_back( FilePath );
                
            }
            
        }
        
        //
        // Process scene entity instances
        for (std::map<std::string, std::vector<std::string>>::iterator it = AssetLoader.AssetData.begin(); it != AssetLoader.AssetData.end(); it++) {
            
            // Get the path to the definition/location file
            std::string FilePath = it ->first;
            
            if (StringFind(strings::ExtLocation, FilePath)) {
                
                SceneManager::AssetParsers::LoadLocations( ScenePtr, FilePath );
                ScenePtr ->LocationsFiles.push_back( FilePath );
                
            }
            
        }
        
        return ScenePtr;
    }
    
    // Clear the scene specified or nullptr to clear all scenes
    void SceneDestroy(Scene* ScenePtr=nullptr) {
        
        // If no scene is specified, clear all scenes
        if (ScenePtr == nullptr) {
            
            SceneManager::SceneMgr ->ClearAllScenes();
            
            return;
        }
        
        // Destroy the scene
        SceneManager::SceneMgr ->DestroyScene( ScenePtr );
        
    }
    
    // Save the current scene
    bool SceneSave(Scene* ScenePtr) {
        
        // Get asset name and file path
        std::string AssetName = ScenePtr ->Name;
        std::string AssetPath = ScenePtr ->Path;
        
        std::string FilePath;
        for (std::vector<std::string>::iterator it = ScenePtr ->LocationsFiles.begin(); it != ScenePtr ->LocationsFiles.end(); it++) {
            
            FilePath = *it;
            
        }
        
        std::ofstream FileStream;
        FileStream.open( FilePath );
        if (FileStream.is_open()) {
            
            //
            // Save object locations
            
            for (std::vector<Entity*>::iterator it = ScenePtr ->EntityList.begin(); it != ScenePtr ->EntityList.end(); it++) {
                
                Entity* EntityPtr = *it;
                
                POSITION Position = EntityPtr ->Position;
                std::string xx = DoubleToString(Position.x);
                std::string yy = DoubleToString(Position.y);
                std::string zz = DoubleToString(Position.z);
                
                ROTATION Rotation = EntityPtr ->Rotation;
                std::string yaw   = DoubleToString(Rotation.y);
                std::string pitch = DoubleToString(Rotation.p);
                std::string roll  = DoubleToString(Rotation.r);
                
                ROTATION Scale = EntityPtr ->Scale;
                std::string width  = DoubleToString(Scale.y);
                std::string height = DoubleToString(Scale.p);
                std::string depth  = DoubleToString(Scale.r);
                
                std::string Translation = strings::SpaceDouble;
                
                std::string ColliderName = strings::SpaceDouble + strings::SpaceSingle + EntityPtr ->ColliderName + strings::SpaceSingle;
                std::string Mass    = FloatToString(EntityPtr ->AttachedBody ->getMass()) + strings::SpaceSingle;
                std::string DampLin = FloatToString(EntityPtr ->AttachedBody ->getLinearDamping()) + strings::SpaceSingle;
                std::string DampAng = FloatToString(EntityPtr ->AttachedBody ->getAngularDamping()) + strings::SpaceDouble;
                
                std::string Attachemnts = strings::SpaceDouble;
                
                // Translation string
                Translation += xx + strings::SpaceSingle + yy + strings::SpaceSingle + zz + strings::SpaceSingle;
                Translation += yaw + strings::SpaceSingle + pitch + strings::SpaceSingle + roll + strings::SpaceSingle;
                Translation += width + strings::SpaceSingle + height + strings::SpaceSingle + depth + strings::SpaceSingle;
                
                // Get name
                std::string Name = EntityPtr ->AttachedMesh ->Name + strings::SpaceDouble;
                
                // Add light attachments
                for (std::vector<Light*>::iterator it = EntityPtr ->LightList.begin(); it != EntityPtr ->LightList.end(); ++it) {
                    
                    Light* LightPtr = *it;
                    
                    std::string LightName = LightPtr ->OriginalName;
                    
                    Attachemnts += LightName + strings::SpaceSingle;
                    
                }Attachemnts += strings::SpaceSingle;
                
                // Consolidate string and write to the file
                FileStream << Name << Translation << ColliderName << Mass << DampLin << DampAng << Attachemnts << std::endl;
                
            }
            
            for (std::vector<Actor*>::iterator it = ScenePtr ->ActorList.begin(); it != ScenePtr ->ActorList.end(); it++) {
                
                Actor* ActorPtr = *it;
                std::string Name = ActorPtr ->OriginalName;
                
                POSITION Position = ActorPtr ->Position;
                std::string xx = DoubleToString(Position.x);
                std::string yy = DoubleToString(Position.y);
                std::string zz = DoubleToString(Position.z);
                
                ROTATION Rotation = ActorPtr ->Rotation;
                std::string yaw   = DoubleToString(Rotation.y);
                std::string pitch = DoubleToString(Rotation.p);
                std::string roll  = DoubleToString(Rotation.r);
                
                std::string Translation = strings::SpaceDouble;
                Translation += xx + strings::SpaceSingle + yy + strings::SpaceSingle + zz + strings::SpaceSingle;
                Translation += yaw + strings::SpaceSingle + pitch + strings::SpaceSingle + roll + strings::SpaceDouble;
                
                std::string ColliderName = strings::SpaceDouble + strings::SpaceSingle + ActorPtr ->ColliderName + strings::SpaceSingle;
                
                // Consolidate string and write to the file
                FileStream << Name << Translation << ColliderName << std::endl;
                
            }
            
            FileStream.close();
        }
        
        return true;
    }
    
}

#endif

