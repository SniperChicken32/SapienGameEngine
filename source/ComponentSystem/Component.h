//
// Component object

struct Component {
    
    std::string Name;
    std::string Type;
    
    float Value;
    
    bool IsActive;
    
    Component();
    
    void Update(void);
    
};

Component :: Component() {
    
    Name = "";
    Type = "";
    
    Value = 0.0;
    
    IsActive = false;
    
}

void Component :: Update(void) {
    
    
}

