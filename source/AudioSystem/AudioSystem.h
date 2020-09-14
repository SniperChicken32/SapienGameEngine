//
// Audio system

#ifndef  AUDIO_SYSTEM__
#define  AUDIO_SYSTEM__

struct Sound {
    
    std::string AssetName;
    
    Sound() {
        
        AssetName = "";
        
    }
    
};

struct SoundScenery {
    
    Sound* SoundPtr;
    
    POSITION Position;
    
    float Volume;
    float Range;
    float Fade;
    
    SoundScenery() {
        
        SoundPtr = nullptr;
        
        Position = glm::vec3(0.0, 0.0, 0.0);
        
        Volume = 0.0;
        Range  = 0.0;
        Fade   = 0.0;
        
    }
    
};


class AudioSystem {
    
    std::vector<Sound*> SoundList;
    std::vector<Sound*> SoundSceneryList;
    
    public:
    
    AudioSystem() {
        
    }
    
    ~AudioSystem() {
        
    }
    
    
};


#endif

