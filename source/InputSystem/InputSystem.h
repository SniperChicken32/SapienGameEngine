//
// Input capture and processing system
#ifndef INPUT_SYSTEM_
#define INPUT_SYSTEM_

#include "Keys.h"

void      CursorSetPosition(int xx, int yy) {SetCursorPos( xx, yy );}
glm::vec2 CursorGetPosition(void) {
    POINT Pos;
    glm::vec2 MousePos;
    
    GetCursorPos(&Pos);
    
    MousePos.x = Pos.x;
    MousePos.y = Pos.y;
    
    return MousePos;
}

namespace InputSystem {

    class InputSystem{
        private:
        
        bool KeyPressed  [256];
        bool KeyReleased [256];
        bool KeyCurrent  [256];
        
        public:
        InputSystem(void)    {for (int i=0; i <= 255; i++) {KeyPressed[i] = 0; KeyReleased[i] = 0; KeyCurrent[i] = 0;}}
        ~InputSystem(void)   {}
        
        // Set key state
        void  SetKeyPressed(int keyid)  {KeyPressed[keyid] = true;  KeyReleased[keyid] = false; KeyCurrent[keyid] = true;}
        void  SetKeyReleased(int keyid) {KeyPressed[keyid] = false; KeyReleased[keyid] = true;  KeyCurrent[keyid] = false;}
        
        // Check if a key is set
        bool  CheckKeyPressed(int keyid)  {return KeyPressed [keyid];}
        bool  CheckKeyReleased(int keyid) {return KeyReleased[keyid];}
        bool  CheckKeyCurrent(int keyid)  {return KeyCurrent [keyid];}
        
        // Clear key state
        void  ClearKey(int keyid)         {KeyPressed[keyid] = false; KeyReleased[keyid] = false; KeyCurrent[keyid] = false;}
        void  ClearKeyPressed(int keyid)  {KeyPressed[keyid] = false;}
        void  ClearKeyReleased(int keyid) {KeyReleased[keyid] = false;}
        void  ClearKeyCurrent(int keyid)  {KeyCurrent[keyid] = false;}
        
        // Clear all key states
        void  ClearKeys(void)         {for (int i=0; i <= 255; i++) {KeyPressed[i] = 0; KeyReleased[i] = 0; KeyCurrent[i] = 0;}}
        void  ClearKeysPressed(void)  {for (int i=0; i <= 255; i++) {KeyPressed[i] = 0;}}
        void  ClearKeysReleased(void) {for (int i=0; i <= 255; i++) {KeyReleased[i] = 0;}}
        void  ClearKeysCurrent(void)  {for (int i=0; i <= 255; i++) {KeyCurrent[i] = 0;}}
        
    };
    
    /** Input system singleton object pointer.*/
    InputSystem* Input = nullptr;
    /** Initiate the input system singleton object.*/
    bool InitiateInputSystem(void) {if (Input == nullptr) {Input = new InputSystem(); return true;}return false;}
    /** Shutdown the input system singleton object.*/
    bool ShutdownInputSystem(void) {if (Input != nullptr) {delete Input; Input = nullptr; return true;}return false;}
    
}

#endif

