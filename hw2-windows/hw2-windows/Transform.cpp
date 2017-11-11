// Transform.cpp: implementation of the Transform class.

// Note: when you construct a matrix using mat4() or mat3(), it will be COLUMN-MAJOR
// Keep this in mind in readfile.cpp and display.cpp
// See FAQ for more details or if you're having problems.

#include "Transform.h"

// TODO remove test matrix
void print3x3Matrix(mat3 mPrintMe)
{
	std::cout << mPrintMe[0][0] << " ";
	std::cout << mPrintMe[1][0] << " ";
	std::cout << mPrintMe[2][0] << "\n";
	std::cout << mPrintMe[0][1] << " ";
	std::cout << mPrintMe[1][1] << " ";
	std::cout << mPrintMe[2][1] << "\n";
	std::cout << mPrintMe[0][2] << " ";
	std::cout << mPrintMe[1][2] << " ";
	std::cout << mPrintMe[2][2] << " ";

	return;
}

// TODO remove test matrix
void print4x4Matrix(mat4 mPrintMe)
{
	std::cout << mPrintMe[0][0] << " ";
	std::cout << mPrintMe[1][0] << " ";
	std::cout << mPrintMe[2][0] << " ";
	std::cout << mPrintMe[3][0] << "\n";
	std::cout << mPrintMe[0][1] << " ";
	std::cout << mPrintMe[1][1] << " ";
	std::cout << mPrintMe[2][1] << " ";
	std::cout << mPrintMe[3][1] << "\n";
	std::cout << mPrintMe[0][2] << " ";
	std::cout << mPrintMe[1][2] << " ";
	std::cout << mPrintMe[2][2] << " ";
	std::cout << mPrintMe[3][2] << "\n";
	std::cout << mPrintMe[0][3] << " ";
	std::cout << mPrintMe[1][3] << " ";
	std::cout << mPrintMe[2][3] << " ";
	std::cout << mPrintMe[3][3] << "\n";
	return;
}


// Helper rotation function.  Please implement this.  
mat3 Transform::rotate(const float degrees, const vec3& axis) 
{
	float radians = degrees*pi / 180.0;
	vec3 axis_normal = normalize(axis);

	glm::mat3 mRotation(0); // rotation matrix to return
	glm::mat3 mIdentity(1.0);
	glm::mat3 mAxisxAxisTranspose(axis_normal.x * axis_normal.x, axis_normal.x * axis_normal.y, axis_normal.x * axis_normal.z,
							axis_normal.x * axis_normal.y, axis_normal.y * axis_normal.y, axis_normal.y * axis_normal.z,
							axis_normal.x * axis_normal.z, axis_normal.y * axis_normal.z, axis_normal.z * axis_normal.z);
	glm::mat3 mAxisDualMatrix(0,	 axis_normal.z, -axis_normal.y,
							-axis_normal.z,	  0, axis_normal.x,
							axis_normal.y,	 -axis_normal.x, 0);

	// Calculate rotation matrix
	mRotation = (mIdentity * cos(radians))
				+ ((1 - cos(radians)) * mAxisxAxisTranspose)
				+ (sin(radians) * mAxisDualMatrix);

  // You will change this return call
  return mRotation;
}

void Transform::left(float degrees, vec3& eye, vec3& up) 
{
	vec3 vUpNorm = normalize(up);
	eye = (Transform::rotate(degrees, vUpNorm)) * eye;
}

void Transform::up(float degrees, vec3& eye, vec3& up) 
{
	vec3 vEyeNorm = normalize(eye);
	vec3 vUpNorm = normalize(up);
	vec3 vUpRotAxis = cross(vEyeNorm, vUpNorm);

	eye = (Transform::rotate(degrees, vUpRotAxis)) * eye;
	up = (Transform::rotate(degrees, vUpRotAxis)) * up;
}

mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up) 
{
	// Create coordinate frame for camera
// TODO need?	vec3 w = normalize(eye - center);
	vec3 w = normalize(eye);
	vec3 u = normalize(cross(up, w));
	vec3 v = cross(w, u);

	// Define Rotation Matrix
	mat4 mCameraRotation(u.x, v.x, w.x, 0,
		u.y, v.y, w.y, 0,
		u.z, v.z, w.z, 0,
		0, 0, 0, 1);

	// Define translation Matrix
	mat4 mCameraTranslation(1);
	mCameraTranslation[3][0] = -eye.x;
	mCameraTranslation[3][1] = -eye.y;
	mCameraTranslation[3][2] = -eye.z;

	// Apply translation for camera (eye) location
	mat4 mM = mCameraRotation * mCameraTranslation;

	return mM;
}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
    mat4 ret;

    // New, to implement the perspective transform as well.  
	float d = 1 / (tan((fovy *pi) / (2*180)));
	float A = -(zFar + zNear) / (zFar - zNear);
	float B = -(2 * zFar * zNear) / (zFar - zNear);

	ret[0][0] = d/aspect;
	ret[1][1] = d;
	ret[2][2] = A;
	ret[3][2] = B;
	ret[2][3] = -1;
	ret[3][3] = 0;

	return ret;
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz) 
{
    mat4 ret = mat4(1);

	// Implement scaling 
	ret[0][0] = sx;
	ret[1][1] = sy;
	ret[2][2] = sz;
		
    return ret;
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz) 
{
    mat4 ret = mat4(1);

    // Implement translation 
	ret[3][0] = tx;
	ret[3][1] = ty;
	ret[3][2] = tz;

    return ret;
}

// To normalize the up direction and construct a coordinate frame.  
// As discussed in the lecture.  May be relevant to create a properly 
// orthogonal and normalized up. 
// This function is provided as a helper, in case you want to use it. 
// Using this function (in readfile.cpp or display.cpp) is optional.  

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec) 
{
    vec3 x = glm::cross(up,zvec); 
    vec3 y = glm::cross(zvec,x); 
    vec3 ret = glm::normalize(y); 
    return ret; 
}


Transform::Transform()
{

}

Transform::~Transform()
{

}
