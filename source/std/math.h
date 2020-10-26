//
// 3D math functions

const float PI = 3.14159;

/** Generates a random number between the given range.*/
int Random(int Min, int Max) {return rand() % Max + Min;}

/** Returns the angle between points.*/
float AngleBetweenPoints(glm::vec3 VecA, glm::vec3 VecB) {
    float x1 = VecA.x;
    float y1 = VecA.y;
    //float z1 = VecA.z;
    
    float x2 = VecB.x;
    float y2 = VecB.y;
    //float z2 = VecB.z;
    
    float xo = x2 - x1;
    float yo = y2 - y1;
    //float zo = z2 - z1;
    
    return -std::atan2(yo, xo) * 180.0 / PI;
    
    //float angle  = VectorDotProduct(aX, aY, aZ, bX, bY, bZ);
    //angle       /= (VectorMagnitude(aX, aY, aZ) * VectorMagnitude(bX, bY, bZ));
    //return acosf(angle);
}

/** Converts radians to degrees.*/
double RadToDeg(double vRadian) {return ( vRadian * (180 / PI));}

/** Converts degrees to radians.*/
double DegToRad(double vDegree) {return ((vDegree * 180) / PI); }

