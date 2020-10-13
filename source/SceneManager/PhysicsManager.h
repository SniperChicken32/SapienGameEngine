//
// Physics proxy resource management

namespace SceneManager {
    
    // Physics resource management
    namespace PhysicsManagement {
        
        // Custom event listener class
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
        
        
        
        // Collider proxy object
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
        
        // Collider proxy list
        std::vector<CollisionProxy*> CollisionList;
        
        // Create collider proxy
        CollisionProxy* CreateBoxProxy(std::string AssetName, short int AssetIndex, float x, float y, float z) {
            
            Physics::Vector3 BoxScale   = Physics::Vector3(x, y, z);
            Physics::BoxShape* Collider = PhysicsCommon ->createBoxShape(BoxScale);
            
            CollisionProxy* CollisionProxyPtr = new CollisionProxy();
            
            CollisionProxyPtr ->AssetName = AssetName;
            CollisionProxyPtr ->AssetIndex = AssetIndex;
            CollisionProxyPtr ->AssetType = 0;
            
            CollisionProxyPtr ->CollisionBox = Collider;
            
            CollisionList.push_back(CollisionProxyPtr);
            
            return CollisionProxyPtr;
        }
        CollisionProxy* CreateSphereProxy(std::string AssetName, short int AssetIndex, float Radius) {
            
            Physics::SphereShape* Collider = PhysicsCommon ->createSphereShape(Radius);
            
            CollisionProxy* CollisionProxyPtr = new CollisionProxy();
            
            CollisionProxyPtr ->AssetName = AssetName;
            CollisionProxyPtr ->AssetIndex = AssetIndex;
            CollisionProxyPtr ->AssetType = 1;
            
            CollisionProxyPtr ->CollisionSphere = Collider;
            
            CollisionList.push_back(CollisionProxyPtr);
            
            return CollisionProxyPtr;
        }
        CollisionProxy* CreateCapsuleProxy(std::string AssetName, short int AssetIndex, float Radius, float Length) {
            
            Physics::CapsuleShape* Collider = PhysicsCommon ->createCapsuleShape(Radius, Length);
            
            CollisionProxy* CollisionProxyPtr = new CollisionProxy();
            
            CollisionProxyPtr ->AssetName = AssetName;
            CollisionProxyPtr ->AssetIndex = AssetIndex;
            CollisionProxyPtr ->AssetType = 2;
            
            CollisionProxyPtr ->CollisionCapsule = Collider;
            
            CollisionList.push_back(CollisionProxyPtr);
            
            return CollisionProxyPtr;
        }
        
        // Find collider proxy
        CollisionProxy* FindProxy(std::string AssetName) {
            
            CollisionProxy* CollisionPtr;
            for (std::vector<CollisionProxy*>::iterator it = CollisionList.begin(); it != CollisionList.end(); ++it) {
                CollisionPtr = *it;
                if (CollisionPtr ->AssetName == AssetName) {return CollisionPtr;}
            }
            return nullptr;
        }
        
        // Destroy ALL collider proxies
        void DestroyAllProxies(void) {
            
            CollisionProxy* CollisionPtr;
            for (std::vector<CollisionProxy*>::iterator it = CollisionList.begin(); it != CollisionList.end(); ++it) {
                CollisionPtr = *it;
                delete CollisionPtr;
            }
            CollisionList.clear();
        }
        
        
        
        // Create a rigid body at the specified position
        Physics::RigidBody* CreateRigidBody(double x=0.0, double y=0.0, double z=0.0) {
            
            Physics::Vector3    Position   = Physics::Vector3(x, y, z);
            Physics::Quaternion Quaternion = Physics::Quaternion::identity();
            
            Physics::Transform BodyTransform = Physics::Transform(Position, Quaternion);
            Physics::RigidBody* RigidBody = PhysicsWorld ->createRigidBody(BodyTransform);
            
            return RigidBody;
        }
        
    }
    
    
}

