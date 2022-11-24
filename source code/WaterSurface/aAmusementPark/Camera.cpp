#include "Camera.h"

const char Camera::ORTHOGRAPHIC_PROJECTION = 1;
const char Camera::PRESPECTIVE_PROJECTION = 2;

// Camera constructor
Camera::Camera(float width, float height, glm::vec3 defaultPos, glm::vec3 defaultOri) {
	// store variables
	Camera::position_vec3 = defaultPos;
	Camera::orientation_vec3 = glm::normalize(defaultOri);
	Camera::width = width;
	Camera::height = height;
}

// Camera destructor
Camera::~Camera() {
}

// init projection size
void Camera::setSize(float width, float height) {
	// store variables
	Camera::width = width;
	Camera::height = height;

	// update projection matrix
	if (Camera::projectionMode == Camera::PRESPECTIVE_PROJECTION) {
		Camera::projection_mat4 = glm::perspective(Camera::fov, (float)(Camera::width / Camera::height), Camera::near, Camera::far);
	}
	if (Camera::projectionMode == Camera::ORTHOGRAPHIC_PROJECTION) {
		Camera::projection_mat4 = glm::ortho(-(float)Camera::width / 2.0f * Camera::fov, (float)Camera::width / 2.0f * Camera::fov,
			-(float)Camera::height / 2.0f * Camera::fov, (float)Camera::height / 2.0f * Camera::fov,
			near, far);
	}
	updateMatrix();
}

// init perspective projection
void Camera::perspectiveInit(float FOV, float near, float far) {
	// store variables
	Camera::fov = FOV;
	Camera::near = near;
	Camera::far = far;
	Camera::projectionMode = Camera::PRESPECTIVE_PROJECTION;
	// generate perspective matrix
	Camera::projection_mat4 = glm::perspective(Camera::fov, (float)(Camera::width / Camera::height), Camera::near, Camera::far);
}

// init orthographic projection
void Camera::orthographicInit(float scale, float near, float far) {
	// store variables
	Camera::fov = scale;
	Camera::near = near;
	Camera::far = far;
	Camera::projectionMode = Camera::ORTHOGRAPHIC_PROJECTION;
	// generate perspective matrix
	Camera::projection_mat4 = glm::ortho(-(float)Camera::width / 2.0f * Camera::fov, (float)Camera::width / 2.0f * Camera::fov,
		-(float)Camera::height / 2.0f * Camera::fov, (float)Camera::height / 2.0f * Camera::fov,
		near, far);
}

// move the camera
void Camera::move(glm::vec3 moveVec3) {
	Camera::position_vec3 += moveVec3.x * glm::normalize(glm::cross(Camera::orientation_vec3, Camera::Up));
	Camera::position_vec3 += moveVec3.y * Camera::Up;
	Camera::position_vec3 += moveVec3.z * glm::normalize(Camera::orientation_vec3 * glm::vec3(1.0f, 0.0f, 1.0f));
}

// rotate the camera
void Camera::rotate(glm::vec2 rotateVec2) {
	glm::vec3 newOrientation;
	float rad = glm::clamp(glm::angle(Camera::orientation_vec3, Up) + glm::radians(rotateVec2.x), 0.1570f, 2.9845f) - glm::radians(90.0f);
	newOrientation = glm::rotate(Camera::orientation_vec3 * glm::vec3(1.0f, 0.0f, 1.0f), -rad, glm::normalize(glm::cross(Camera::orientation_vec3, Up)));
	newOrientation = glm::rotate(newOrientation, glm::radians(-rotateVec2.y), Up);

	Camera::orientation_vec3 = glm::normalize(newOrientation);
}

glm::mat4 Camera::updateMatrix() {
	glm::mat4 view = glm::lookAt(Camera::position_vec3, Camera::position_vec3 + Camera::orientation_vec3, Up);
	transform_mat4 = Camera::projection_mat4 * view;
	return transform_mat4;
}