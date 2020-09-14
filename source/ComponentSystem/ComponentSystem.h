//
// Component system

#include "ComponentSystem/Component.h"

namespace ComponentSystem {
    
    class ComponentSystem {
        
        std::vector<Component*> ComponentList;
        
        public:
        
        ComponentSystem();
        ~ComponentSystem();
        
        // Component management
        Component*  CreateComponent  (std::string);
        bool        DestroyComponent (Component*);
        Component*  FindComponent    (std::string);
        
        // Update Components
        void UpdateComponents(void);
        
    };
    
    ComponentSystem* ComponentMgr = nullptr;
    bool InitiateComponentSystem(void) {if (ComponentMgr == nullptr) {ComponentMgr = new ComponentSystem(); return true;}return false;}
    bool ShutdownComponentSystem(void) {if (ComponentMgr != nullptr) {delete ComponentMgr; ComponentMgr = nullptr; return true;}return false;}
    
    ComponentSystem :: ComponentSystem() {
        
        ComponentList.clear();
        
    }
    ComponentSystem :: ~ComponentSystem() {
        
    }
    
    // Actor objects
    Component* ComponentSystem :: CreateComponent(std::string ComponentName) {
        
        Component* ComponentPtr = new Component();
        ComponentPtr ->Name = ComponentName;
        
        ComponentList.push_back(ComponentPtr);
        
        return ComponentPtr;
    }
    bool       ComponentSystem :: DestroyComponent(Component* ComponentPtr) {
        
        for (std::vector<Component*>::iterator it = ComponentList.begin(); it != ComponentList.end(); ++it) {
            Component* ComponentListPtr = *it;
            
            if (ComponentPtr == ComponentListPtr) {
                
                ComponentList.erase(it);
                
                delete ComponentPtr;
                
                return true;
            }
        }
        
        return false;
    }
    Component* ComponentSystem :: FindComponent(std::string AssetName) {
        
        for (std::vector<Component*>::iterator it = ComponentList.begin(); it != ComponentList.end(); ++it) {
            Component* ComponentPtr = *it;
            
            if (ComponentPtr ->Name == AssetName) {
                
                return ComponentPtr;
                
            }
            
        }
        
        return nullptr;
    }
    
    // Update Components
    void ComponentSystem :: UpdateComponents(void) {
        
        for (std::vector<Component*>::iterator it = ComponentList.begin(); it != ComponentList.end(); ++it) {
            Component* ComponentPtr = *it;
            
            if (ComponentPtr ->IsActive) {
                
                ComponentPtr ->Update();
                
            }
            
        }
        
    }
    
}


