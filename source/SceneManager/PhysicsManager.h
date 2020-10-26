//
// Physics proxy resource management

namespace SceneManager {
    
    /** Physics resource management.*/
    namespace PhysicsManagement {
        
        /** Custom event listener class.*/
        class EventListener : public Physics::EventListener {
            
            // Override the onContact() method 
            virtual void onContact(const CollisionCallback::CallbackData& callbackData) override {
                
                // For each contact pair
                for (unsigned int i = 0; i < callbackData.getNbContactPairs(); i++) {
                    
                    // Get the contact pair
                    CollisionCallback::ContactPair contactPair = callbackData.getContactPair(i);
                    
                    // For each contact point of the contact pair
                    for (unsigned int u = 0; u < contactPair.getNbContactPoints(); u++) {
                        
                        // Get the collision contact point in world space
                        CollisionCallback::ContactPoint contactPoint = contactPair.getContactPoint(u);
                        Physics::Vector3 worldPoint = contactPair.getCollider1() ->getLocalToWorldTransform() * contactPoint.getLocalPointOnCollider1();
                        
                        // Get colliders involved in collision
                        Physics::Collider* ColliderA = contactPair.getCollider1();
                        Physics::Collider* ColliderB = contactPair.getCollider2();
                        
                        // Get rigid bodies involved in collision
                        //const Physics::RigidBody& RigidBodyA = (const Physics::RigidBody*) ColliderA ->getUserData();
                        //const Physics::RigidBody& RigidBodyB = (const Physics::RigidBody*) ColliderB ->getUserData();
                        
                        //Physics::Transform Transform;
                        //Physics::Vector3 PositionA;
                        //Physics::Vector3 PositionB;
                        
                        //Transform = RigidBodyA ->getTransform();
                        //PositionA = Transform.getPosition();
                        
                        //Transform = RigidBodyB ->getTransform();
                        //PositionB = Transform.getPosition();
                        
                        //if (worldPoint.z < 1.5) continue;
                        
                        //std::cout << "Body A: " << PositionA.x << " " << PositionA.y << " " << PositionA.z << " " << std::endl;
                        //std::cout << "Body B: " << PositionB.x << " " << PositionB.y << " " << PositionB.z << " " << std::endl;
                        
                        
                    }
                    
                }
                
            }
            
        };
        
        /** Custom ray cast callback class.*/
        class MyCallbackClass : public Physics::RaycastCallback {
            
            public: 
            
            virtual Physics::decimal notifyRaycastHit(const Physics::RaycastInfo& info) { 
                
                Physics::Vector3 worldPoint  = info.worldPoint;
                Physics::Vector3 worldNormal = info.worldNormal;
                
                // Display the world hit point coordinates 
                //std::cout << "Hit: " << worldPoint.x << "  " << worldPoint.y << "  " << worldPoint.z << std::endl;
                
                
                /// Fraction distance of the hit point between point1 and point2 of the ray
                /// The hit point "p" is such that p = point1 + hitFraction * (point2 - point1)
                float hitFraction = info.hitFraction;
                
                /// Mesh subpart index that has been hit (only used for triangles mesh and -1 otherwise)
                int meshSubpart = info.meshSubpart;
                
                /// Hit triangle index (only used for triangles mesh and -1 otherwise)
                int triangleIndex = info.triangleIndex;
                
                /// Pointer to the hit collision body
                Physics::CollisionBody* body = info.body;
                
                /// Pointer to the hit collider
                Physics::Collider* collider = info.collider;
                
                
                // Return a fraction of 1.0 to gather all hits 
                return Physics::decimal(1.0); 
            }
            
        };
        
        // Test casting against a rigid body`s collider
        //MyCallbackClass CallbackObject;
        //PhysicsWorld ->raycast(ray, &CallbackObject);
        
        
        
        /** Collider proxy object.*/
        struct CollisionProxy {
            
            std::string AssetName;
            short int   AssetIndex;
            short int   AssetType;
            
            // Collider objects
            Physics::BoxShape*     CollisionBox;
            Physics::SphereShape*  CollisionSphere;
            Physics::CapsuleShape* CollisionCapsule;
            
            // Collision material
            float Bounciness;
            float FrictionCoefficient;
            float RollingResistance;
            float MassDensity;
            
            CollisionProxy() {
                
                AssetName = "";
                AssetIndex = 0;
                AssetType = 0;
                
                CollisionBox = nullptr;
                CollisionSphere = nullptr;
                CollisionCapsule = nullptr;
                
                Bounciness = 0.0;
                FrictionCoefficient = 0.0;
                RollingResistance = 0.0;
                MassDensity = 0.0;
                
            }
            ~CollisionProxy() {
                
            }
            
        };
        
        /** Collider proxy object management system.*/
        class ColliderSystem {
            
            std::vector<CollisionProxy*> CollisionList;
            
            public:
            
            ColliderSystem() {
                
                CollisionList.clear();
                
            }
            
            /** Creates a box collision proxy object and returns its pointer.*/
            CollisionProxy* CreateBoxProxy(std::string AssetName, short int AssetIndex, float x, float y, float z) {
                
                Physics::Vector3 BoxScale   = Physics::Vector3(x, y, z);
                Physics::BoxShape* Collider = PhysicsCommon ->createBoxShape(BoxScale);
                
                CollisionProxy* CollisionProxyPtr = new CollisionProxy();
                
                CollisionProxyPtr ->AssetName = AssetName;
                CollisionProxyPtr ->AssetIndex = AssetIndex;
                CollisionProxyPtr ->AssetType = 0;
                
                CollisionProxyPtr ->CollisionBox = Collider;
                
                this ->CollisionList.push_back(CollisionProxyPtr);
                
                return CollisionProxyPtr;
            }
            /** Creates a sphere collision proxy object and returns its pointer.*/
            CollisionProxy* CreateSphereProxy(std::string AssetName, short int AssetIndex, float Radius) {
                
                Physics::SphereShape* Collider = PhysicsCommon ->createSphereShape(Radius);
                
                CollisionProxy* CollisionProxyPtr = new CollisionProxy();
                
                CollisionProxyPtr ->AssetName = AssetName;
                CollisionProxyPtr ->AssetIndex = AssetIndex;
                CollisionProxyPtr ->AssetType = 1;
                
                CollisionProxyPtr ->CollisionSphere = Collider;
                
                this ->CollisionList.push_back(CollisionProxyPtr);
                
                return CollisionProxyPtr;
            }
            /** Creates a capsule collision proxy object and returns its pointer.*/
            CollisionProxy* CreateCapsuleProxy(std::string AssetName, short int AssetIndex, float Radius, float Length) {
                
                Physics::CapsuleShape* Collider = PhysicsCommon ->createCapsuleShape(Radius, Length);
                
                CollisionProxy* CollisionProxyPtr = new CollisionProxy();
                
                CollisionProxyPtr ->AssetName = AssetName;
                CollisionProxyPtr ->AssetIndex = AssetIndex;
                CollisionProxyPtr ->AssetType = 2;
                
                CollisionProxyPtr ->CollisionCapsule = Collider;
                
                this ->CollisionList.push_back(CollisionProxyPtr);
                
                return CollisionProxyPtr;
            }
            
            /** Finds a collision proxy with the given name and returns its pointer.*/
            CollisionProxy* FindProxy(std::string AssetName) {
                
                CollisionProxy* CollisionPtr;
                for (std::vector<CollisionProxy*>::iterator it = this ->CollisionList.begin(); it != this ->CollisionList.end(); ++it) {
                    CollisionPtr = *it;
                    if (CollisionPtr ->AssetName == AssetName) {return CollisionPtr;}
                }
                return nullptr;
            }
            
            /** Destroys all proxy objects in this manager.*/
            void DestroyAllProxies(void) {
                
                CollisionProxy* CollisionPtr;
                for (std::vector<CollisionProxy*>::iterator it = this ->CollisionList.begin(); it != this ->CollisionList.end(); ++it) {
                    CollisionPtr = *it;
                    delete CollisionPtr;
                }
                this ->CollisionList.clear();
            }
            
            
        };
        
        /** Collision system singleton object pointer.*/
        ColliderSystem* ColliderMgr = nullptr;
        /** Initiate the Collision system singleton object.*/
        bool InitiateColliderSystem(void) {if (ColliderMgr == nullptr) {ColliderMgr = new ColliderSystem(); return true;}return false;}
        /** Shutdown the Collision system singleton object.*/
        bool ShutdownColliderSystem(void) {if (ColliderMgr != nullptr) {delete ColliderMgr; ColliderMgr = nullptr; return true;}return false;}
        
        /** Create a rigid body at the specified position.*/
        Physics::RigidBody* CreateRigidBody(double x=0.0, double y=0.0, double z=0.0) {
            
            Physics::Vector3    Position   = Physics::Vector3(x, y, z);
            Physics::Quaternion Quaternion = Physics::Quaternion::identity();
            
            Physics::Transform BodyTransform = Physics::Transform(Position, Quaternion);
            Physics::RigidBody* RigidBody = PhysicsWorld ->createRigidBody(BodyTransform);
            
            return RigidBody;
        }
        
    }
    
    
}

