#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <stdio.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

class Camera {
public:
	static const char ORTHOGRAPHIC_PROJECTION;
	static const char PRESPECTIVE_PROJECTION;
public:
	glm::vec3 position_vec3;
	glm::vec3 orientation_vec3 = glm::vec3(0.0f, 0.0f, -1.0f);

	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
	
	glm::mat4 view_mat4 = glm::mat4(1.0f);
	glm::mat4 projection_mat4 = glm::mat4(1.0f);

	float fov=90.0f, near=0.1f, far=100.0f;
	unsigned char projectionMode = 0;
public:
	glm::mat4 transform_mat4 = glm::mat4(1.0f);
	float width, height;

	// Camera constructor
	Camera(	float width, float height,
			glm::vec3 defaultPos = glm::vec3(0.0f),
			glm::vec3 defaultOri = glm::vec3(0.0f, 0.0f, -1.0f));
	// Camera destructor
	~Camera();

	// init projection size
	void setSize(float width, float height);
	// init perspective projection
	void perspectiveInit(float FOV, float near, float far);
	// init orthographic projection
	void orthographicInit(float scale, float near, float far);

	// move the camera
	void move(glm::vec3 moveVec3);
	// rotate the camera
	void rotate(glm::vec2 rotateVec2);

	// update camera transform matrix
	glm::mat4 updateMatrix();
};

#endif
