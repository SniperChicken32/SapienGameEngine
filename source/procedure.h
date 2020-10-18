//
// Windows callback procedure

namespace ProcedureCallbacks{
    
    void WindowCreate(void) {
        
    }
    
    void WindowClose(void) {
        
        PostQuitMessage(0);
        
    }
    
    
    void KeyPressed(WPARAM wParam) {
        
        if (InputSystem::Input != nullptr) InputSystem::Input ->SetKeyPressed(wParam);
        
    }
    
    void KeyReleased(WPARAM wParam) {
        
        if (InputSystem::Input != nullptr) InputSystem::Input ->SetKeyReleased(wParam);
        
    }
    
    void ChangeFocus(void) {
        
        if (InputSystem::Input != nullptr) InputSystem::Input ->ClearKeys();
        
    }
    
    void Resize(HWND wHnd) {
        
        RECT WindowRect;
        GetWindowRect(wHnd, &WindowRect);
        
        if (Renderer != nullptr) Renderer -> SetViewport(0, 0, WindowRect.right - WindowRect.left, WindowRect.bottom - WindowRect.top);
        
    }
    
    void MinMaxInfo(LPARAM lParam) {
        
        LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
        
        // Set minimum window size
        lpMMI -> ptMinTrackSize.x = 480;
        lpMMI -> ptMinTrackSize.y = 320;
        
    }
    
    
}


LRESULT CALLBACK Procedure(HWND wHnd, UINT messages, WPARAM wParam, LPARAM lParam) {
    
    switch (messages) {
        
        case WM_KEYDOWN: ProcedureCallbacks::KeyPressed(wParam); break;
        case WM_KEYUP:   ProcedureCallbacks::KeyReleased(wParam); break;
        
        case WM_KILLFOCUS: ProcedureCallbacks::ChangeFocus(); break;
        case WM_SIZE: ProcedureCallbacks::Resize(wHnd); break;
        
        case WM_GETMINMAXINFO: ProcedureCallbacks::MinMaxInfo(lParam); break;
        
        case WM_CREATE: ProcedureCallbacks::WindowCreate(); break;
        
        case WM_CLOSE: ProcedureCallbacks::WindowClose(); break;
        
        default: return DefWindowProc (wHnd, messages, wParam, lParam);
        
    }
    
    return 0;
}

