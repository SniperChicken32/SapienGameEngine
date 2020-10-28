//
// Scene file string parsers and loaders

namespace SceneManager {
    
    // Back-end scene file parsing
    namespace AssetParsers {
        
        // Scene string parsers
        namespace Parsers {
            
            // Define an asset from a string
            void ParseDefinitionString(std::string DefinitionString) {
                
                // Explode the string
                int i=0; std::string Strings[50];
                std::vector<std::string> Array = StringExplode(DefinitionString, ' ');
                for (std::vector<std::string>::iterator it = Array.begin(); it != Array.end(); ++it) 
                {std::string String = *it; Strings[i] = String; i++;}
                
                // Check blank string
                if (Strings[0] == "") {return;}
                if (StringFind("//", Strings[0])) {return;}
                
                //
                // Asset definitions
                
                // Collision
                if (Strings[0] == "define_collider") {
                    
                    // Check if the asset is already loaded
                    if (SceneManager::PhysicsManagement::ColliderMgr ->FindProxy( Strings[1] ) != nullptr) return;
                    
                    std::string AssetName  = Strings[1];
                    short int   AssetIndex = StringToInt(Strings[2]);
                    std::string AssetType  = Strings[3];
                    
                    if (AssetType == "box") {
                        
                        float xx = StringToFloat(Strings[4]);
                        float yy = StringToFloat(Strings[5]);
                        float zz = StringToFloat(Strings[6]);
                        
                        SceneManager::PhysicsManagement::ColliderMgr ->CreateBoxProxy(AssetName, AssetIndex, xx, yy, zz);
                        
#ifdef  DEVELOPMENT_MODE_
                        LogWrite(strings::LogStringAdd + strings::AssetTypeCollider + Strings[1]);
#endif
                        
                        return;
                    }
                    
                    if (AssetType == "sphere") {
                        
                        float Radius = StringToFloat(Strings[4]);
                        
                        SceneManager::PhysicsManagement::ColliderMgr ->CreateSphereProxy(AssetName, AssetIndex, Radius);
                        
#ifdef  DEVELOPMENT_MODE_
                        LogWrite(strings::LogStringAdd + strings::AssetTypeCollider + Strings[1]);
#endif
                        
                        return;
                    }
                    
                    if (AssetType == "capsule") {
                        
                        float Radius = StringToFloat(Strings[4]);
                        float Height = StringToFloat(Strings[5]);
                        
                        SceneManager::PhysicsManagement::ColliderMgr ->CreateCapsuleProxy(AssetName, AssetIndex, Radius, Height);
                        
#ifdef  DEVELOPMENT_MODE_
                        LogWrite(strings::LogStringAdd + strings::AssetTypeCollider + Strings[1]);
#endif
                        
                        return;
                    }
                    
                    // Incorrect collider type
                    LogWrite(strings::LogStringError + strings::AssetTypeCollider + AssetName + " :: Incorrect collider type");
                    
                    return;
                }
                
                // Render pipeline objects
                // Mesh > Material > Shader
                if (Strings[0] == "define_mesh") {
                    
                    ResourceManagement::LoadMesh( Strings[1] );
                    
                    return;
                }
                if (Strings[0] == "define_material") {
                    
                    ResourceManagement::LoadMaterial( Strings[1] );
                    
                    return;
                }
                if (Strings[0] == "define_shader") {
                    
                    ResourceManagement::LoadShader( Strings[1] );
                    
                    return;
                }
                if (Strings[0] == "link") {
                    
                    // Find required assets for linking
                    Mesh*     MeshPtr     = Renderer ->FindMesh( Strings[1] );
                    Material* MaterialPtr = Renderer ->FindMaterial( Strings[2] );
                    Shader*   ShaderPtr   = Renderer ->FindShader( Strings[3] );
                    
                    if (MeshPtr != nullptr) {
                        
                        if (MaterialPtr != nullptr) {
                            
                            // Material > mesh
                            MeshPtr ->AttachMaterial(MaterialPtr);
                            
                            if (ShaderPtr != nullptr) {
                                
                                // Shader > material
                                MaterialPtr ->AttachShader(ShaderPtr);
                                
#ifdef  DEVELOPMENT_MODE_
                                LogWrite(strings::LogStringAdd + "Link: " + Strings[1] + strings::AssetResolution + Strings[2] + strings::AssetResolution + Strings[3]);
#endif
                                
                                return;
                            }
                            
                            // Shader link failed
                            LogWrite(strings::LogStringError + "Link failed: " + Strings[3]);
                            
                            return;
                        }
                        
                        // Material link failed
                        LogWrite(strings::LogStringError + "Link failed: " + Strings[2]);
                        
                        return;
                    }
                    
                    // Mesh link failed
                    LogWrite(strings::LogStringError + "Link failed: " + Strings[1]);
                    
                    return;
                }
                
                // Advanced renderable objects
                if (Strings[0] == "define_emitter") {
                    
                    return;
                }
                if (Strings[0] == "define_light") {
                    
                    ResourceManagement::LoadLight( Strings[1] );
                    
                    return;
                }
                if (Strings[0] == "define_camera") {
                    
                    Camera* CameraPtr = ResourceManagement::LoadCamera( Strings[1] );
                    if (CameraPtr == nullptr) return;
                    
                    // Bind the camera to the render system
                    Renderer ->CurrentCamera = CameraPtr;
                    
                    return;
                }
                if (Strings[0] == "define_camera_controller") {
                    
                    if (Renderer ->CurrentCamera == nullptr) {
                        
                        LogWrite(strings::LogStringFail + strings::ColonSymbol + ": Must define a camera before camera physics");
                        
                        return;
                    }
                    
                    // Check if the camera already has physics
                    if (Renderer ->CurrentCamera ->AttachedBody != nullptr) return;
                    
#ifdef  LOG_DETAILS_
                    LogWrite(strings::LogStringAdd + "camera_controller");
#endif
                    
                    // Camera starting position
                    float xx = StringToFloat(Strings[1]);
                    float yy = StringToFloat(Strings[2]);
                    float zz = StringToFloat(Strings[3]);
                    
                    // Collider scale
                    SCALE ColliderScale;
                    ColliderScale.x = StringToFloat(Strings[4]);
                    ColliderScale.y = StringToFloat(Strings[5]);
                    ColliderScale.z = StringToFloat(Strings[6]);
                    
                    // Mass, damping
                    float Mass        = StringToFloat(Strings[7]);
                    float LinearDamp  = StringToFloat(Strings[8]);
                    float AngularDamp = StringToFloat(Strings[9]);
                    
                    // Velocities
                    float MoveVelocity = StringToFloat(Strings[10]) * 0.01;
                    float JumpVelocity = StringToFloat(Strings[11]) * 0.1;
                    
                    
                    //
                    // Apply settings to camera object
                    Camera* CurrentCamera = Renderer ->CurrentCamera;
                    CurrentCamera ->MoveVelocity = MoveVelocity;
                    CurrentCamera ->JumpVelocity = JumpVelocity;
                    
                    // Set the camera height offset
                    CurrentCamera ->CameraHeight = ColliderScale.z;
                    
                    // Construct a rigid body
                    Physics::Transform BodyTransform = Physics::Transform(Physics::Vector3(xx, yy, zz), Physics::Quaternion::identity());
                    Physics::RigidBody* RigidBody = PhysicsWorld ->createRigidBody(BodyTransform);
                    
                    // Attach the body to the current camera
                    CurrentCamera ->AttachedBody = RigidBody;
                    
                    // Setup the rigid body
                    RigidBody ->setType( Physics::BodyType::DYNAMIC );
                    RigidBody ->enableGravity       (true);
                    RigidBody ->setIsAllowedToSleep (false);
                    
                    RigidBody ->setMass( Mass );
                    RigidBody ->setLinearDamping( LinearDamp );
                    RigidBody ->setAngularDamping( AngularDamp );
                    
                    
                    
                    //
                    // Construct a box collider
                    Physics::Transform ColliderTransform = Physics::Transform::identity();
                    
                    Physics::Vector3 ScaleVector = Physics::Vector3(ColliderScale.x, ColliderScale.y, ColliderScale.z);
                    Physics::BoxShape* ColliderShape = PhysicsCommon ->createBoxShape( ScaleVector );
                    
                    Physics::Collider* Collider; 
                    Collider = RigidBody ->addCollider(ColliderShape, ColliderTransform);
                    
                    // Setup collider material
                    //Physics::Material& MaterialPtr = Collider ->getMaterial();
                    
                    //MaterialPtr.setBounciness(0.0);
                    //MaterialPtr.setFrictionCoefficient(0.0);
                    //MaterialPtr.setRollingResistance
                    //MaterialPtr.setMassDensity
                    
                    //RigidBody ->updateLocalCenterOfMassFromColliders();
                    //RigidBody ->updateLocalInertiaTensorFromColliders();
                    //RigidBody ->updateMassFromColliders();
                    
                    return;
                }
                
                // Special effects
                if (Strings[0] == "define_sky") {
                    
                    // Check if a sky has already been assigned
                    if (Renderer ->CurrentSky != nullptr) return;
                    
                    //
                    // Build entity
                    Entity* EntityPtr = Renderer ->CreateEntity(0.0, 0.0, 0.0);
                    
                    // Bind the sky to the render system
                    Renderer ->AttachSky( EntityPtr );
                    
                    // Sky orientation
                    EntityPtr ->Rotation.y =  180.0;
                    EntityPtr ->Rotation.p =    0.0;
                    EntityPtr ->Rotation.r =    0.0;
                    
                    // Sky scale
                    float SkyScale = 1000000.0;
                    EntityPtr ->Scale.x  = SkyScale;
                    EntityPtr ->Scale.y  = SkyScale;
                    EntityPtr ->Scale.z  = SkyScale;
                    
                    EntityPtr ->SetRenderQueueGroup(RENDER_QUEUE_SKY_EARLY);
                    EntityPtr ->SetRenderDistance( SkyScale );
                    
                    // Find mesh asset
                    Mesh* MeshPtr = Renderer ->FindMesh( Strings[1] );
                    if (MeshPtr != nullptr) {
                        
                        // Attach the mesh asset
                        EntityPtr ->AttachMesh( MeshPtr );
                        
#ifdef  DEVELOPMENT_MODE_
                        LogWrite(strings::LogStringAdd + strings::AssetTypeSky + Strings[1]);
#endif
                        
                        return;
                    }
                    
#ifdef  DEVELOPMENT_MODE_
                    LogWrite(strings::LogStringError + strings::AssetTypeSky + Strings[1]);
#endif
                    
                    Renderer ->DestroyEntity( EntityPtr );
                    return;
                }
                if (Strings[0] == "define_fog") {
                    
                }
                
                // Actor
                if (Strings[0] == "define_actor") {
                    
                    // Find the asset by name
                    std::string ActorAsset = AssetFilePeekHeader( Strings[1] );
                    Actor* ActorPtr = ActorAI::AI ->FindActor( ActorAsset );
                    
                    if (ActorPtr == nullptr) {
                        
                        ActorPtr = ActorAI::AI ->CreateActor();
                        ResourceManagement::LoadActor( Strings[1] );
                        
                    }
                    
                    return;
                }
                
            }
            
            // Place an entity instance from a string
            void ParseLocationString(Scene* ScenePointer, std::string FilePath, std::string LocationString) {
                
#ifdef  LOG_DETAILS_
                // Log the entity location
                std::string LogString = strings::LogStringAdd;
#endif
                
                // Explode the string
                int i=0; std::string Strings[50];
                std::vector<std::string> Array = StringExplode(LocationString, ' ');
                for (std::vector<std::string>::iterator it = Array.begin(); it != Array.end(); ++it) 
                {std::string String = *it; Strings[i] = String; i++;}
                
                // Check blank string
                if (Strings[0] == "") {return;}
                if (StringFind("//", Strings[0])) {return;}
                
                //
                // Get position, orientation
                
                POSITION Position;
                Position.x = StringToDouble(Strings[1]);
                Position.y = StringToDouble(Strings[2]);
                Position.z = StringToDouble(Strings[3]);
                
                ROTATION Rotation;
                Rotation.y = StringToFloat(Strings[4]);
                Rotation.p = StringToFloat(Strings[5]);
                Rotation.r = StringToFloat(Strings[6]);
                
                
                
                //
                // Check assets for entity type
                
                // Check meshes
                Mesh* MeshPtr = Renderer ->FindMesh( Strings[0] );
                if (MeshPtr != nullptr) {
                    
                    // Get the scale for the entity
                    SCALE Scale;
                    Scale.x = StringToFloat(Strings[7]);
                    Scale.y = StringToFloat(Strings[8]);
                    Scale.z = StringToFloat(Strings[9]);
                    
                    // Get entity physics
                    std::string Collider = Strings[10];
                    float Mass     = StringToFloat( Strings[11] );
                    float LinDamp  = StringToFloat( Strings[12] );
                    float AngDamp  = StringToFloat( Strings[13] );
                    
                    
#ifdef  LOG_DETAILS_
                    LogString += Strings[0] + strings::SpaceDouble;
                    LogString += Strings[1] + strings::SpaceSingle + Strings[2] + strings::SpaceSingle + Strings[3] + strings::SpaceDouble;
#endif
                    
                    //
                    // Build entity
                    Entity* EntityPtr = Renderer ->CreateEntity(Position.x, Position.y, Position.z);
                    
                    EntityPtr ->Rotation = Rotation;
                    EntityPtr ->Scale    = Scale;
                    
                    // Attach the mesh asset
                    EntityPtr ->AttachMesh( MeshPtr );
                    EntityPtr ->ColliderName = Collider;
                    
                    // Remember the original file path
                    EntityPtr ->FilePath = FilePath;
                    
                    // Add the entity to the scene graph
                    ScenePointer ->AddEntity( EntityPtr );
                    
                    
                    //
                    // Setup entity physics
                    
                    // Construct a new rigid body
                    Physics::RigidBody* RigidBody = PhysicsManagement::CreateRigidBody(Position.x, Position.y, Position.z);
                    EntityPtr ->AttachedBody = RigidBody;
                    
                    RigidBody ->setIsAllowedToSleep(true);
                    
                    // Find the collision object
                    PhysicsManagement::CollisionProxy* CollisionProxyPtr = SceneManager::PhysicsManagement::ColliderMgr ->FindProxy( Collider );
                    if (CollisionProxyPtr == nullptr) return;
                    
                    // Create the collision object
                    Physics::Transform  Trans = Physics::Transform(Physics::Vector3(0.0, 0.0, 0.0), Physics::Quaternion::identity());
                    
                    
                    //
                    // Check collider type
                    Physics::Collider* ColliderPtr = nullptr;
                    
                    // Type 0 - box
                    if (CollisionProxyPtr ->AssetType == 0) {ColliderPtr = RigidBody ->addCollider( CollisionProxyPtr ->CollisionBox, Trans );}
                    // Type 1 - sphere
                    if (CollisionProxyPtr ->AssetType == 1) {ColliderPtr = RigidBody ->addCollider( CollisionProxyPtr ->CollisionSphere, Trans );}
                    // Type 2 - capsule
                    if (CollisionProxyPtr ->AssetType == 2) {ColliderPtr = RigidBody ->addCollider( CollisionProxyPtr ->CollisionCapsule, Trans );}
                    
                    // Check body type dynamic/static
                    if (Mass <= 0.0) {
                        
#ifdef  LOG_DETAILS_
                        LogString += "static";
#endif
                        
                        // Static
                        RigidBody ->setType( Physics::BodyType::STATIC );
                        
                        RigidBody ->setMass( Mass );
                        
                        RigidBody ->setLinearDamping ( LinDamp );
                        RigidBody ->setAngularDamping( AngDamp );
                        
                    } else {
                        
#ifdef  LOG_DETAILS_
                        LogString += "dynamic";
#endif
                        
                        // Dynamic
                        RigidBody ->setType( Physics::BodyType::DYNAMIC );
                        
                        RigidBody ->setMass( Mass );
                        
                        RigidBody ->setLinearDamping ( LinDamp );
                        RigidBody ->setAngularDamping( AngDamp );
                        
                        RigidBody ->updateLocalInertiaTensorFromColliders();
                        //RigidBody ->updateMassPropertiesFromColliders();
                        
                    }
                    
                    // Add a pointer to the rigid body
                    ColliderPtr ->setUserData( RigidBody );
                    
                    //
                    // Process attachments
                    for (i=0; i<32; i++) {
                        
                        std::string AttachmentString = Strings[14 + i];
                        
                        if (AttachmentString == "") continue;
                        
                        //
                        // Find the asset by name
                        
                        // Light
                        Light* LightPtr = Renderer ->FindLight( AttachmentString );
                        if (LightPtr != nullptr) {
                            
#ifdef  LOG_DETAILS_
                            LogString += strings::SpaceDouble + strings::AddSymbol + AttachmentString + strings::SpaceDouble;
#endif
                            
                            // Create new asset
                            Light* LightNew = Renderer ->CreateLight();
                            EntityPtr ->AddLight( LightNew );
                            LightNew ->OriginalName = LightPtr ->Name;
                            
                            //
                            // Copy the asset data
                            
                            LightNew ->Position = Position;
                            LightNew ->Rotation = Rotation;
                            
                            LightNew ->IsActive        = true;
                            LightNew ->LightType       = LightPtr ->LightType;
                            
                            LightNew ->Diffuse         = LightPtr ->Diffuse;
                            LightNew ->Specular        = LightPtr ->Specular;
                            
                            LightNew ->RenderDistance  = LightPtr ->RenderDistance;
                            
                            LightNew ->Range           = LightPtr ->Range;
                            LightNew ->Intensity       = LightPtr ->Intensity;
                            
                            LightNew ->Constant        = LightPtr ->Constant;
                            LightNew ->Linear          = LightPtr ->Linear;
                            LightNew ->Quadratic       = LightPtr ->Quadratic;
                            LightNew ->Focus           = LightPtr ->Focus;
                            
                            continue;
                        }
                        
                    }
                    
                    
                    
#ifdef  LOG_DETAILS_
                    LogWrite(LogString);
#endif
                    
                    return;
                }
                
                // Check actors
                Actor* ActorPtr = ActorAI::AI ->FindActor( Strings[0] );
                if (ActorPtr != nullptr) {
                    
                    // Create an actor object
                    Actor* NewActorPtr = ActorAI::AI ->CreateActor(Position.x, Position.y, Position.z);
                    NewActorPtr ->IsActive = true;
                    
                    NewActorPtr ->Name = "";
                    NewActorPtr ->OriginalName = ActorPtr ->Name;
                    
                    // Add the actor to the scene
                    ScenePointer ->AddActor( NewActorPtr );
                    
                    //
                    // Copy asset settings
                    NewActorPtr ->DoAnimationStates  = ActorPtr ->DoAnimationStates;
                    NewActorPtr ->LimbSwingRangeWalk = ActorPtr ->LimbSwingRangeWalk;
                    NewActorPtr ->LimbSwingRateWalk  = ActorPtr ->LimbSwingRateWalk;
                    NewActorPtr ->LimbSwingRangeRun  = ActorPtr ->LimbSwingRangeRun;
                    NewActorPtr ->LimbSwingRateRun   = ActorPtr ->LimbSwingRateRun;
                    
                    NewActorPtr ->AnimationCurrent = ANIMATION_NONE;
                    NewActorPtr ->AnimationIdle    = ActorPtr ->AnimationIdle;
                    NewActorPtr ->AnimationWalk    = ActorPtr ->AnimationWalk;
                    NewActorPtr ->AnimationRun     = ActorPtr ->AnimationRun;
                    NewActorPtr ->AnimationDeath   = ActorPtr ->AnimationDeath;
                    
                    NewActorPtr ->SpeedWalk = ActorPtr ->SpeedWalk;
                    NewActorPtr ->SpeedRun  = ActorPtr ->SpeedRun;
                    
                    //
                    // Find the mesh assets
                    Mesh* HeadMesh  = Renderer ->FindMesh(ActorPtr ->HeadName);
                    Mesh* TorsoMesh = Renderer ->FindMesh(ActorPtr ->TorsoName);
                    Mesh* ArmRMesh  = Renderer ->FindMesh(ActorPtr ->ArmRName);
                    Mesh* ArmLMesh  = Renderer ->FindMesh(ActorPtr ->ArmLName);
                    Mesh* LegRMesh  = Renderer ->FindMesh(ActorPtr ->LegRName);
                    Mesh* LegLMesh  = Renderer ->FindMesh(ActorPtr ->LegLName);
                    
                    
                    //
                    // Create entities
                    NewActorPtr ->HeadEntity  = Renderer ->CreateEntity();
                    NewActorPtr ->TorsoEntity = Renderer ->CreateEntity();
                    NewActorPtr ->ArmREntity  = Renderer ->CreateEntity();
                    NewActorPtr ->ArmLEntity  = Renderer ->CreateEntity();
                    NewActorPtr ->LegREntity  = Renderer ->CreateEntity();
                    NewActorPtr ->LegLEntity  = Renderer ->CreateEntity();
                    
                    // Set entity names
                    NewActorPtr ->HeadEntity ->Name  = "Head";
                    NewActorPtr ->TorsoEntity ->Name = "Torso";
                    NewActorPtr ->ArmREntity ->Name  = "ArmR";
                    NewActorPtr ->ArmLEntity ->Name  = "ArmL";
                    NewActorPtr ->LegREntity ->Name  = "LegR";
                    NewActorPtr ->LegLEntity ->Name  = "LegL";
                    
                    // Set translation offsets
                    NewActorPtr ->HeadEntity ->PositionOffset  = ActorPtr ->HeadOffset;
                    NewActorPtr ->TorsoEntity ->PositionOffset = ActorPtr ->TorsoOffset;
                    NewActorPtr ->ArmREntity ->PositionOffset  = ActorPtr ->ArmROffset;
                    NewActorPtr ->ArmLEntity ->PositionOffset  = ActorPtr ->ArmLOffset;
                    NewActorPtr ->LegREntity ->PositionOffset  = ActorPtr ->LegROffset;
                    NewActorPtr ->LegLEntity ->PositionOffset  = ActorPtr ->LegLOffset;
                    
                    // Attach meshes
                    NewActorPtr ->HeadEntity ->AttachMesh(HeadMesh);
                    NewActorPtr ->TorsoEntity ->AttachMesh(TorsoMesh);
                    NewActorPtr ->ArmREntity ->AttachMesh(ArmRMesh);
                    NewActorPtr ->ArmLEntity ->AttachMesh(ArmLMesh);
                    NewActorPtr ->LegREntity ->AttachMesh(LegRMesh);
                    NewActorPtr ->LegLEntity ->AttachMesh(LegLMesh);
                    
                    
                    //
                    // Set initial health
                    
                    float InitialHealth = 100.0f;
                    
                    NewActorPtr ->HeadEntity  ->AddComponent("health", InitialHealth);
                    NewActorPtr ->TorsoEntity ->AddComponent("health", InitialHealth);
                    NewActorPtr ->ArmREntity  ->AddComponent("health", InitialHealth);
                    NewActorPtr ->ArmLEntity  ->AddComponent("health", InitialHealth);
                    NewActorPtr ->LegREntity  ->AddComponent("health", InitialHealth);
                    NewActorPtr ->LegLEntity  ->AddComponent("health", InitialHealth);
                    
                    
                    // Get entity physics
                    std::string Collider = Strings[7];
                    
                    
                    //
                    // Setup entity physics
                    
                    // Construct a new rigid body
                    Physics::RigidBody* RigidBody = PhysicsManagement::CreateRigidBody(Position.x, Position.y, Position.z);
                    NewActorPtr ->AttachedBody = RigidBody;
                    NewActorPtr ->ColliderName = Collider;
                    
                    RigidBody ->setIsAllowedToSleep(true);
                    
                    // Find the collision object
                    PhysicsManagement::CollisionProxy* CollisionProxyPtr = SceneManager::PhysicsManagement::ColliderMgr ->FindProxy( Collider );
                    if (CollisionProxyPtr == nullptr) return;
                    
                    // Create the collision object
                    Physics::Transform  Transform = Physics::Transform(Physics::Vector3(0.0, 0.0, 0.0), Physics::Quaternion::identity());
                    
                    // Dynamic
                    RigidBody ->setType( Physics::BodyType::DYNAMIC );
                    
                    RigidBody ->setMass( 0.85 );
                    
                    RigidBody ->setLinearDamping ( 0.5 );
                    RigidBody ->setAngularDamping( 0.0 );
                    
                    
                    //
                    // Check collider type
                    Physics::Collider* ColliderPtr;
                    
                    // Type 0 - box
                    if (CollisionProxyPtr ->AssetType == 0) {ColliderPtr = RigidBody ->addCollider( CollisionProxyPtr ->CollisionBox, Transform );}
                    // Type 1 - sphere
                    if (CollisionProxyPtr ->AssetType == 1) {ColliderPtr = RigidBody ->addCollider( CollisionProxyPtr ->CollisionSphere, Transform );}
                    // Type 2 - capsule
                    if (CollisionProxyPtr ->AssetType == 2) {ColliderPtr = RigidBody ->addCollider( CollisionProxyPtr ->CollisionCapsule, Transform );}
                    
                    RigidBody ->updateLocalInertiaTensorFromColliders();
                    //RigidBody ->updateMassPropertiesFromColliders();
                    
                    // Add a pointer to the rigid body
                    ColliderPtr ->setUserData( RigidBody );
                    
#ifdef  LOG_DETAILS_
                    LogString += Strings[0] + strings::SpaceDouble;
                    LogString += Strings[1] + strings::SpaceSingle + Strings[2] + strings::SpaceSingle + Strings[3] + strings::SpaceDouble;
                    LogWrite(LogString);
#endif
                    
                    return;
                }
                
                
                //
                // Terrain generation
                if (Strings[0] == "generate"){
                    
                    std::string MeshPlatform     = Strings[1];
                    std::string ColliderPlatform = Strings[2];
                    
                    int Width   = StringToInt(Strings[3]);
                    int Height  = StringToInt(Strings[4]);
                    int SizeMul = StringToInt(Strings[5]);
                    
                    // Find the mesh by name
                    MeshPtr = Renderer ->FindMesh( MeshPlatform );
                    if (MeshPtr == nullptr) {Msg("Mesh not found"); return;}
                    
                    
                    
                    POSITION Position = POSITION(0.0, 0.0, 0.0);
                    
                    ROTATION Rotation = ROTATION(0.0, 0.0, 0.0);
                    
                    SCALE    Scale = SCALE(10.0, 10.0, 10.0);
                    
                    
                    
                    // Find the collision asset
                    SceneManager::PhysicsManagement::CollisionProxy* CollisionProxyPtr = SceneManager::PhysicsManagement::ColliderMgr ->FindProxy( ColliderPlatform );
                    if (CollisionProxyPtr == nullptr) {Msg("Collision proxy not found"); return;}
                    
                    // Get starting point
                    Position.x -= StringToDouble(IntToString(Width * SizeMul))  / 2.0f;
                    Position.y -= StringToDouble(IntToString(Height * SizeMul)) / 2.0f;
                    
                    
                    
                    //
                    // Generate platform area
                    
                    for (int i=0; i < Width; i++) {
                        
                        double AreaWidth = StringToDouble(IntToString(i * SizeMul));
                        
                        
                        for (int u=0; u < Height; u++) {
                            
                            double AreaHeight = StringToDouble(IntToString(u * SizeMul));
                            
                            
                            // Random depth offset
                            //float DepthMul = 10.01f;
                            //float Depth = StringToFloat(IntToString(Random(1, 100))) * DepthMul;
                            float Depth = 0.0f;
                            
                            // Generated object position
                            POSITION FinalPosition = glm::vec3(Position.x + AreaWidth, Position.y + AreaHeight, Position.z + Depth);
                            
                            //Rotation.x = (StringToFloat(IntToString(Random(1, 100))) * 0.1f) - (StringToFloat(IntToString(Random(1, 100))) * 0.1f);
                            //Rotation.y = (StringToFloat(IntToString(Random(1, 100))) * 0.1f) - (StringToFloat(IntToString(Random(1, 100))) * 0.1f);
                            //Rotation.z = (StringToFloat(IntToString(Random(1, 100))) * 0.1f) - (StringToFloat(IntToString(Random(1, 100))) * 0.1f);
                            //if (Rotation.x > 360.0) Rotation.x=0.0;
                            //if (Rotation.y > 360.0) Rotation.y=0.0;
                            //if (Rotation.z > 360.0) Rotation.z=0.0;
                            
                            
                            //
                            // Construct an entity
                            Entity* EntityPtr = Renderer ->CreateEntity(0.0, 0.0, 0.0);
                            
                            EntityPtr ->Rotation = Rotation;
                            EntityPtr ->Scale    = Scale;
                            
                            EntityPtr ->SetRenderDistance(1000.0);
                            
                            // Attach the mesh asset
                            EntityPtr ->AttachMesh( MeshPtr );
                            EntityPtr ->ColliderName = ColliderPlatform;
                            
                            // Remember the original file path
                            EntityPtr ->FilePath = FilePath;
                            
                            // Add the entity to the scene graph
                            ScenePointer ->AddEntity( EntityPtr );
                            
                            
                            
                            
                            
                            //
                            // Construct a rigid body
                            Physics::Vector3    Position = Physics::Vector3(FinalPosition.x, FinalPosition.y, FinalPosition.z);
                            Physics::Quaternion Quat     = Physics::Quaternion::identity();
                            
                            float Angle = 1.0;
                            ROTATION Axis = Rotation;
                            
                            // Calculate the quaternion
                            float qx = Axis.x * sin( Angle/2.0 );
                            float qy = Axis.y * sin( Angle/2.0 );
                            float qz = Axis.z * sin( Angle/2.0 );
                            float qw = cos( Angle/2.0 );
                            
                            Quat.setAllValues(qx, qy, qz, qw);
                            
                            
                            Physics::Transform BodyTransform = Physics::Transform(Position, Quat);
                            Physics::RigidBody* RigidBody = PhysicsWorld ->createRigidBody(BodyTransform);
                            
                            
                            // Attach the body to the entity
                            EntityPtr ->AttachedBody = RigidBody;
                            
                            
                            //Physics::RigidBody* RigidBody = PhysicsManagement::CreateRigidBody( FinalPosition.x, FinalPosition.y, FinalPosition.z );
                            
                            // Get physical body transform
                            //Physics::Transform  EntityTransform  = AttachedBody ->getTransform();
                            //Physics::Quaternion EntityQuaternion = EntityTransform.getOrientation();
                            
                            // Sync the entity to the physical body`s position
                            //Physics::Vector3 EntityPosition = EntityTransform.getPosition();
                            //this ->Position = glm::vec3(EntityPosition.x, EntityPosition.y, EntityPosition.z);
                            
                            //Physics::Vector3 EntityRotation   = EntityQuaternion.getVectorV();
                            //this ->Rotation = glm::vec3(EntityRotation.x, EntityRotation.y, EntityRotation.z);
                            
                            
                            
                            
                            
                            
                            //
                            // Create the collision object
                            Physics::Transform CollisionOffset = Physics::Transform(Physics::Vector3(0.0, 0.0, 0.0), Physics::Quaternion::identity());
                            
                            //
                            // Check collider type
                            Physics::Collider* ColliderPtr = nullptr;
                            
                            // Type 0 - box
                            if (CollisionProxyPtr ->AssetType == 0) {ColliderPtr = RigidBody ->addCollider( CollisionProxyPtr ->CollisionBox, CollisionOffset );}
                            // Type 1 - sphere
                            if (CollisionProxyPtr ->AssetType == 1) {ColliderPtr = RigidBody ->addCollider( CollisionProxyPtr ->CollisionSphere, CollisionOffset );}
                            // Type 2 - capsule
                            if (CollisionProxyPtr ->AssetType == 2) {ColliderPtr = RigidBody ->addCollider( CollisionProxyPtr ->CollisionCapsule, CollisionOffset );}
                            
                            // Static mass
                            RigidBody ->setType( Physics::BodyType::STATIC );
                            RigidBody ->setMass( 0.0 );
                            RigidBody ->setLinearDamping ( 0.0 );
                            RigidBody ->setAngularDamping( 0.0 );
                            
                            
                        }
                        
                    }
                    
                    
                    
                    
                    
                    
                    
                    
                    return;
                }
                
                
#ifdef  LOG_DETAILS_
                LogString = strings::LogStringError + Strings[0] + strings::SpaceDouble;
                LogWrite(LogString);
#endif
                
            }
            
        }
        
        
        // Load object definitions
        bool LoadDefinitions(std::string PathName) {
            
            std::ifstream FileStream;
            FileStream.open( PathName );
            
            if (FileStream.is_open()) {
                
#ifdef  DEVELOPMENT_MODE_
                LogWrite("");
                LogWrite(strings::LogStringLoad + "Definition: " + PathName);
#endif
                std::string Line;
                
                while (getline(FileStream, Line)) {
                    
                    if (Line == "") continue;
                    if (StringFind("//", Line)) continue;
                    
                    Parsers::ParseDefinitionString(Line);
                    
                }
                
                FileStream.close();
                
            } else {LogWrite(strings::LogStringFail + PathName); return false;}
            
            return true;
        }
        
        // Load object locations into a scene
        bool LoadLocations(Scene* ScenePointer, std::string PathName) {
            
            // Save scene file path
            ScenePointer ->Path = PathName;
            
            std::ifstream FileLocs;
            FileLocs.open( PathName );
            
            if (FileLocs.is_open()) {
                
#ifdef  DEVELOPMENT_MODE_
                LogWrite("");
                LogWrite(strings::LogStringLoad + "Locations: " + PathName);
#endif
                
                std::string Line;
                
                while (getline(FileLocs, Line)) {
                    
                    if (Line == "") continue;
                    if (StringFind("//", Line)) continue;
                    
                    Parsers::ParseLocationString(ScenePointer, PathName, Line);
                    
                }
                
                FileLocs.close();
                
            } else {LogWrite(strings::LogStringFail + PathName); return false;}
            
            return false;
        }
        
    }
    
}

