//
// Window procedure


LRESULT CALLBACK Procedure(HWND wHnd, UINT messages, WPARAM wParam, LPARAM lParam) {
    
    switch (messages) {
        
        // Capture keys
        case WM_KEYDOWN: if (InputSystem::Input != nullptr) InputSystem::Input ->SetKeyPressed(wParam); break;
        case WM_KEYUP:   if (InputSystem::Input != nullptr) InputSystem::Input ->SetKeyReleased(wParam); break;
        
        // Check focus
        case WM_KILLFOCUS: if (InputSystem::Input != nullptr) InputSystem::Input ->ClearKeys(); break;
        
        // Resize the view port
        case WM_SIZE: 
            RECT WindowRect; GetWindowRect(wHnd, &WindowRect);
            if (Renderer != nullptr) Renderer -> SetViewport(0, 0, WindowRect.right - WindowRect.left, WindowRect.bottom - WindowRect.top);
            break;
        
        // Minimum window size
        case WM_GETMINMAXINFO: {
            
            LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
            
            lpMMI -> ptMinTrackSize.x = 480;
            lpMMI -> ptMinTrackSize.y = 320;
            
            break;
        }
        
        case WM_CREATE: 
            break;
        
        case WM_CLOSE: 
            
            PostQuitMessage(0);
            
            break;
        
        default: 
            return DefWindowProc (wHnd, messages, wParam, lParam);
        
    }
    
    return 0;
}

