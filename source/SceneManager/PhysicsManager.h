//
// Physics resource management

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
        
        
        // Collider object
        struct CollisionManafold {
            
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
            
            CollisionManafold() {
                
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
            ~CollisionManafold() {
                
            }
            
        };
        std::vector<CollisionManafold*> CollisionList;
        
        // Find collision object
        CollisionManafold* FindCollider(std::string AssetName) {
            
            CollisionManafold* CollisionPtr;
            for (std::vector<CollisionManafold*>::iterator it = CollisionList.begin(); it != CollisionList.end(); ++it) {
                CollisionPtr = *it;
                if (CollisionPtr ->AssetName == AssetName) {return CollisionPtr;}
            }
            return nullptr;
        }
        
        // Destroy ALL colliders in the list
        void DestroyColliders(void) {
            
            CollisionManafold* CollisionPtr;
            for (std::vector<CollisionManafold*>::iterator it = CollisionList.begin(); it != CollisionList.end(); ++it) {
                CollisionPtr = *it;
                delete CollisionPtr;
            }
            CollisionList.clear();
        }
        
        // Create collider objects
        CollisionManafold* AddBoxCollider(std::string AssetName, short int AssetIndex, float x, float y, float z) {
            
            Physics::Vector3 BoxScale   = Physics::Vector3(x, y, z);
            Physics::BoxShape* Collider = PhysicsCommon ->createBoxShape(BoxScale);
            
            CollisionManafold* CollisionMfold = new CollisionManafold();
            
            CollisionMfold ->AssetName = AssetName;
            CollisionMfold ->AssetIndex = AssetIndex;
            CollisionMfold ->AssetType = 0;
            
            CollisionMfold ->CollisionBox = Collider;
            
            CollisionList.push_back(CollisionMfold);
            
            return CollisionMfold;
        }
        CollisionManafold* AddSphereCollider(std::string AssetName, short int AssetIndex, float Radius) {
            
            Physics::SphereShape* Collider = PhysicsCommon ->createSphereShape(Radius);
            
            CollisionManafold* CollisionMfold = new CollisionManafold();
            
            CollisionMfold ->AssetName = AssetName;
            CollisionMfold ->AssetIndex = AssetIndex;
            CollisionMfold ->AssetType = 1;
            
            CollisionMfold ->CollisionSphere = Collider;
            
            CollisionList.push_back(CollisionMfold);
            
            return CollisionMfold;
        }
        CollisionManafold* AddCapsuleCollider(std::string AssetName, short int AssetIndex, float Radius, float Length) {
            
            Physics::CapsuleShape* Collider = PhysicsCommon ->createCapsuleShape(Radius, Length);
            
            CollisionManafold* CollisionMfold = new CollisionManafold();
            
            CollisionMfold ->AssetName = AssetName;
            CollisionMfold ->AssetIndex = AssetIndex;
            CollisionMfold ->AssetType = 2;
            
            CollisionMfold ->CollisionCapsule = Collider;
            
            CollisionList.push_back(CollisionMfold);
            
            return CollisionMfold;
        }
        
        // Create a rigid body
        Physics::RigidBody* CreateRigidBody(double x=0.0, double y=0.0, double z=0.0) {
            
            Physics::Vector3    Position   = Physics::Vector3(x, y, z);
            Physics::Quaternion Quaternion = Physics::Quaternion::identity();
            
            Physics::Transform BodyTransform = Physics::Transform(Position, Quaternion);
            Physics::RigidBody* RigidBody = PhysicsWorld ->createRigidBody(BodyTransform);
            
            return RigidBody;
        }
        
    }
    
    
}

