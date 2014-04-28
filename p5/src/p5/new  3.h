	bool TriangleBody::checkBary(SphereBody& body1, TriangleBody& body2)
	{
	Vector3 AB, AC, A, B, C, norm, sphereposition;
	
	A = body2.vertices[0];
	B = body2.vertices[1];
	C = body2.vertices[2];
	
	AB = B - A;
	AC = C - A;
	
	norm = cross(AB, AC);
	
	spherePosition = body1.position;
	  
    // edge 1, result needed to compute v
    vector AP =spherePosition - A;
    vector ABxAP = cross(AB, AP);
    float v_num;
    if ((v_num = dot(N, ABxAP)) < 0)
        return false; // P is on the left side
 
    // edge 2
    vector BP = spherePosition - B;
    vector BC = C - B;
    vector BCxBP = cross(BC, BP);
    if (Dot(N, BCxBP) < 0)
       return false; // P is on the left side
 
    // edge 3, needed to compute u
    vector CP = spherePosition - C;
    // we have computed AC already so we can avoid computing CA by
    // inverting the vectors in the cross product:
    // Cross(CA, CP) = cross(CP, AC);
    vector CAxCP = cross(CP, AC);
    float u_num;
    if ((u_num = dot(N, CAxCP)) < 0)
        return false; // P is on the left side;
    // compute barycentric coordinates
    float denom = dot(N, N); // ABxAC.N where N = ABxAC
    u = u_num / denom;
    v = v_num / denom;
 
    return true; // this ray hits the triangle
}