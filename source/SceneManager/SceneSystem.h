//
// Scene management system

#ifndef SCENE_SYSTEM_
#define SCENE_SYSTEM_

namespace SceneManager {
    
    struct Scene {
        
        std::string Name;
        std::string Path;
        
        std::vector<std::string> LocationsFiles;
        std::vector<std::string> DefinitionsFiles;
        
        std::vector<Entity*> EntityList;
        std::vector<Actor*>  ActorList;
        
        Scene() {
            
            Name = "";
            Path = "";
            
            EntityList.clear();
            ActorList.clear();
            
        }
        
        void AddEntity(Entity* EntityPtr) {EntityList.push_back(EntityPtr);}
        bool RemoveEntity(Entity* EntityPtr) {
            
            for (std::vector<Entity*>::iterator it = EntityList.begin(); it != EntityList.end(); ++it) {
                Entity* EntityListPtr = *it;
                if (EntityPtr == EntityListPtr) {
                    
                    EntityList.erase(it);
                    
                    return true;
                }
            }
            
            return false;
        }
        bool DestroyEntity(Entity* EntityPtr) {
            
            for (std::vector<Entity*>::iterator it = EntityList.begin(); it != EntityList.end(); ++it) {
                Entity* EntityListPtr = *it;
                if (EntityPtr == EntityListPtr) {
                    
                    EntityList.erase(it);
                    
                    Renderer ->DestroyEntity( EntityPtr );
                    
                    return true;
                }
            }
            
            return false;
        }
        
        void AddActor(Actor* ActorPtr) {ActorList.push_back(ActorPtr);}
        bool RemoveActor(Actor* ActorPtr) {
            
            for (std::vector<Actor*>::iterator it = ActorList.begin(); it != ActorList.end(); ++it) {
                Actor* ActorListPtr = *it;
                if (ActorPtr == ActorListPtr) {
                    
                    ActorList.erase(it);
                    
                    return true;
                }
            }
            
            return false;
        }
        bool DestroyActor(Actor* ActorPtr) {
            
            for (std::vector<Actor*>::iterator it = ActorList.begin(); it != ActorList.end(); ++it) {
                Actor* ActorListPtr = *it;
                if (ActorPtr == ActorListPtr) {
                    
                    ActorList.erase(it);
                    
                    ActorAI::AI ->DestroyActor( ActorPtr );
                    
                    return true;
                }
            }
            
            return false;
        }
        
        void Activate(void) {
            
            // Entities
            for (std::vector<Entity*>::iterator it = EntityList.begin(); it != EntityList.end(); ++it) {
                
                Entity* EntityPtr = *it;
                
                EntityPtr ->IsActive   = true;
                EntityPtr ->IsRendered = true;
                
                // Lights
                for (std::vector<Light*>::iterator it = EntityPtr ->LightList.begin(); it != EntityPtr ->LightList.end(); ++it) {
                    
                    Light* LightPtr = *it;
                    
                    LightPtr ->IsActive = true;
                    
                }
                
            }
            
            // Actors
            for (std::vector<Actor*>::iterator it = ActorList.begin(); it != ActorList.end(); ++it) {
                
                Actor* ActorPtr = *it;
                
                ActorPtr ->IsActive = true;
                
                ActorPtr ->HeadEntity ->IsActive    = true;
                ActorPtr ->HeadEntity ->IsRendered  = true;
                
                ActorPtr ->TorsoEntity ->IsActive   = true;
                ActorPtr ->TorsoEntity ->IsRendered = true;
                
                ActorPtr ->ArmREntity ->IsActive    = true;
                ActorPtr ->ArmREntity ->IsRendered  = true;
                
                ActorPtr ->ArmLEntity ->IsActive    = true;
                ActorPtr ->ArmLEntity ->IsRendered  = true;
                
                ActorPtr ->LegREntity ->IsActive    = true;
                ActorPtr ->LegREntity ->IsRendered  = true;
                
                ActorPtr ->LegLEntity ->IsActive    = true;
                ActorPtr ->LegLEntity ->IsRendered  = true;
                
            }
            
        }
        void Deactivate(void) {
            
            // Entities
            for (std::vector<Entity*>::iterator it = EntityList.begin(); it != EntityList.end(); ++it) {
                
                Entity* EntityPtr = *it;
                
                EntityPtr ->IsActive   = false;
                EntityPtr ->IsRendered = false;
                
                // Lights
                for (std::vector<Light*>::iterator it = EntityPtr ->LightList.begin(); it != EntityPtr ->LightList.end(); ++it) {
                    
                    Light* LightPtr = *it;
                    
                    LightPtr ->IsActive = false;
                    
                }
                
            }
            
            // Actors
            for (std::vector<Actor*>::iterator it = ActorList.begin(); it != ActorList.end(); ++it) {
                
                Actor* ActorPtr = *it;
                
                ActorPtr ->IsActive = false;
                
                ActorPtr ->HeadEntity ->IsActive    = false;
                ActorPtr ->HeadEntity ->IsRendered  = false;
                
                ActorPtr ->TorsoEntity ->IsActive   = false;
                ActorPtr ->TorsoEntity ->IsRendered = false;
                
                ActorPtr ->ArmREntity ->IsActive    = false;
                ActorPtr ->ArmREntity ->IsRendered  = false;
                
                ActorPtr ->ArmLEntity ->IsActive    = false;
                ActorPtr ->ArmLEntity ->IsRendered  = false;
                
                ActorPtr ->LegREntity ->IsActive    = false;
                ActorPtr ->LegREntity ->IsRendered  = false;
                
                ActorPtr ->LegLEntity ->IsActive    = false;
                ActorPtr ->LegLEntity ->IsRendered  = false;
                
            }
            
        }
        
        // Clear and destroy all objects in the scene
        void Clear(void) {
            
            // Clear entities
            for (std::vector<Entity*>::iterator it = EntityList.begin(); it != EntityList.end(); ++it) {
                
                Entity* EntityPtr = *it;
                
                Renderer ->DestroyEntity( EntityPtr );
                
            }
            
            // Clear actors
            for (std::vector<Actor*>::iterator it = ActorList.begin(); it != ActorList.end(); ++it) {
                
                Actor* ActorPtr = *it;
                
                ActorAI::AI ->DestroyActor( ActorPtr );
                
            }
            
            EntityList.clear();
            ActorList.clear();
            
            
        }
        
        // Query the scene
        int GetEntityNumber(void) {return EntityList.size();}
        int GetActorNumber(void) {return ActorList.size();}
        
    };
    
    class SceneSystem {
        
        std::vector<Scene*> SceneList;
        
        public:
        
        SceneSystem();
        ~SceneSystem();
        
        Scene* CreateScene(void);
        Scene* FindScene(std::string SceneName);
        bool DestroyScene(Scene* ScenePtr);
        
        void DestroyAllScenes(void);
        void ClearAllScenes(void);
        
    };
    
    SceneSystem* SceneMgr = nullptr;
    bool InitiateSceneSystem(void) {if (SceneMgr == nullptr) {SceneMgr = new SceneSystem(); return true;}return false;}
    bool ShutdownSceneSystem(void) {if (SceneMgr != nullptr) {delete SceneMgr; SceneMgr = nullptr; return true;}return false;}
    
    SceneSystem :: SceneSystem() {
        
        SceneList.clear();
        
    }
    SceneSystem :: ~SceneSystem() {
        
        for (std::vector<Scene*>::iterator it = SceneList.begin(); it != SceneList.end(); ++it) {
            
            Scene* ScenePtr = *it;
            
            delete ScenePtr;
            
        }
        
    }
    
    Scene* SceneSystem :: CreateScene(void) {
        Scene* ScenePtr = new Scene();
        SceneList.push_back(ScenePtr);
        return ScenePtr;
    }
    Scene* SceneSystem :: FindScene(std::string SceneName) {
        
        for (std::vector<Scene*>::iterator it = SceneList.begin(); it != SceneList.end(); ++it) {
            Scene* ScenePtr = *it;
            if (ScenePtr ->Name == SceneName) {
                
                return ScenePtr;
            }
        }
        
        return nullptr;
    }
    bool   SceneSystem :: DestroyScene(Scene* ScenePtr) {
        
        for (std::vector<Scene*>::iterator it = SceneList.begin(); it != SceneList.end(); ++it) {
            Scene* SceneListPtr = *it;
            if (ScenePtr == SceneListPtr) {
                
                ScenePtr ->Clear();
                
                SceneList.erase(it);
                
                delete ScenePtr;
                
                return true;
            }
        }
        
        return false;
    }
    
    void   SceneSystem :: DestroyAllScenes(void) {
        
        for (std::vector<Scene*>::iterator it = SceneList.begin(); it != SceneList.end(); ++it) {
            
            Scene* ScenePtr = *it;
            
            delete ScenePtr;
            
        }
        
        SceneList.clear();
        
    }
    
    void   SceneSystem :: ClearAllScenes(void) {
        
        for (std::vector<Scene*>::iterator it = SceneList.begin(); it != SceneList.end(); ++it) {
            
            Scene* ScenePtr = *it;
            
            ScenePtr ->Clear();
            
        }
        
    }
    
    
}

#endif

