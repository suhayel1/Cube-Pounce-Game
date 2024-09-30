#include "SystemTypes.h"
#include "ShapeVertices.h"
#include <iostream>
#include <algorithm>

extern Coordinator coordinator;
extern WindowManager windowManager;

void BoundaryCollisionSystem::initialize() {
	this->boundaries = boundaries;

	coordinator.updateSystemComponentSet<BoundaryCollisionSystem>(coordinator.getComponentID<RigidBody>(), true);
}

void BoundaryCollisionSystem::setBounds(int* boundaries) {
	this->boundaries = boundaries;
}

void BoundaryCollisionSystem::update() {
	for (auto& entity : this->entities) {
		auto& rigid = coordinator.getComponent<RigidBody>(entity);
		auto& transf = coordinator.getComponent<Transformation>(entity);

		// check ground collision
		auto& boundaryTransf = coordinator.getComponent<Transformation>(this->boundaries[0]);
		if (transf.translation.y - transf.scale.y / 2 <= boundaryTransf.translation.y + boundaryTransf.scale.y / 2 && rigid.velocity.y <= 0.0f) rigid.velocity.y = 0.0f;

		// check right wall collision
		auto& boundaryTransf_1 = coordinator.getComponent<Transformation>(this->boundaries[1]);
		if (transf.translation.x + transf.scale.x / 2 >= boundaryTransf_1.translation.x - boundaryTransf_1.scale.x / 2 && rigid.velocity.x > 0.0f) rigid.velocity.x = 0.0f;

		// check right wall collision
		auto& boundaryTransf_2 = coordinator.getComponent<Transformation>(this->boundaries[2]);
		if (transf.translation.x - transf.scale.x / 2 <= boundaryTransf_2.translation.x + boundaryTransf_2.scale.x / 2 && rigid.velocity.x < 0.0f) rigid.velocity.x = 0.0f;

		// check front wall collision
		auto& boundaryTransf_3 = coordinator.getComponent<Transformation>(this->boundaries[3]);
		if (transf.translation.z - transf.scale.z / 2 <= boundaryTransf_3.translation.z + boundaryTransf_3.scale.z / 2 && rigid.velocity.z < 0.0f) rigid.velocity.z = 0.0f;

		// check front wall collision
		auto& boundaryTransf_4 = coordinator.getComponent<Transformation>(this->boundaries[4]);
		if (transf.translation.z + transf.scale.z / 2 >= boundaryTransf_4.translation.z - boundaryTransf_4.scale.z / 2 && rigid.velocity.z > 0.0f) rigid.velocity.z = 0.0f;
	}
}

void CameraSystem::initialize() {
	coordinator.updateSystemComponentSet<CameraSystem>(coordinator.getComponentID<Camera>(), true);

	this->mouseInputSystem = coordinator.getSystem<MouseInputSystem>();
}

void CameraSystem::update() {
	auto cameraID = this->entities.begin();
	auto mouseID = this->mouseInputSystem->entities.begin();
	auto& camera = coordinator.getComponent<Camera>(*cameraID);
	auto& mouseInput = coordinator.getComponent<MouseInput>(*mouseID);

	camera.yaw += mouseInput.xOffset;
	camera.pitch += mouseInput.yOffset;

	glm::vec3 front;

	if (camera.freeRoamIsEnabled) {
		if (camera.constrainPitch) {
			if (camera.pitch > 89.0f) camera.pitch = 89.0f;
			if (camera.pitch < -89.0f) camera.pitch = -89.0f;
		}

		front.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
		front.y = sin(glm::radians(camera.pitch));
		front.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));

		camera.front = glm::normalize(front);
		camera.right = glm::normalize(glm::cross(camera.front, camera.worldUp));
		camera.up = glm::normalize(glm::cross(camera.right, camera.front));
	}
	else {
		if (camera.constrainPitch) {
			if (camera.pitch < -89.0f) camera.pitch = -89.0f;
			if (camera.pitch > 1.0f) camera.pitch = 1.0f;
		}
		// rest done in PlayerCameraSystem
	}
}


void DamagePowerUpSystem::initialize() {
	coordinator.updateSystemComponentSet<DamagePowerUpSystem>(coordinator.getComponentID<DamagePowerUp>(), true);
}

void DamagePowerUpSystem::setPlayer(int player) {
	this->player = player;
}

void DamagePowerUpSystem::update(float time) {
	std::vector<int> powerUpsToDelete;

	auto& playerTransf = coordinator.getComponent<Transformation>(this->player);

	for (int entity : this->entities) {
		auto& powerupTransf = coordinator.getComponent<Transformation>(entity);

		if (glm::distance(powerupTransf.translation, playerTransf.translation) <= 1.0f) {
			auto& playerDmg = coordinator.getComponent<Damage>(this->player);
			playerDmg.points = 50;

			powerUpsToDelete.push_back(entity);
			break;
		}

		powerupTransf.rotation.y += 0.1f;
		if (powerupTransf.rotation.y >= 360.0f) powerupTransf.rotation.y = 0.0f;

		powerupTransf.translation.y = 0.25f * sin(time);
	}

	for (const auto& id : powerUpsToDelete) coordinator.destroyEntity(id);

	powerUpsToDelete.clear();
}


void KeyInputSystem::initialize() {
	coordinator.updateSystemComponentSet<KeyInputSystem>(coordinator.getComponentID<KeyInput>(), true);
}

void KeyInputSystem::update() {
	auto playerID = this->entities.begin();

	auto& keyInput = coordinator.getComponent<KeyInput>(*playerID);
	auto& transf = coordinator.getComponent<Transformation>(*playerID);
	auto& rigid = coordinator.getComponent<RigidBody>(*playerID);

	GLFWwindow* window = windowManager.getWindow();

	keyInput.w = glfwGetKey(window, GLFW_KEY_W);
	keyInput.a = glfwGetKey(window, GLFW_KEY_A);
	keyInput.s = glfwGetKey(window, GLFW_KEY_S);
	keyInput.d = glfwGetKey(window, GLFW_KEY_D);
	keyInput.space = glfwGetKey(window, GLFW_KEY_SPACE);

	glm::mat4 model(1.0f);
	float rotY = glm::radians(transf.rotation.y);
	glm::vec3 yAxis = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::vec3 frontVel(0.0f);
	glm::vec3 leftVel(0.0f);
	glm::vec3 backVel(0.0f);
	glm::vec3 rightVel(0.0f);
	glm::vec3 upVel(0.0f);

	if (keyInput.w == GLFW_PRESS) frontVel = glm::vec3(glm::rotate(model, rotY, yAxis) * glm::vec4(0.0f, 0.0f, -5.0f, 0.0f));
	if (keyInput.a == GLFW_PRESS) leftVel = glm::vec3(glm::rotate(model, rotY, yAxis) * glm::vec4(-5.0f, 0.0f, 0.0f, 0.0f));
	if (keyInput.s == GLFW_PRESS) backVel = glm::vec3(glm::rotate(model, rotY, yAxis) * glm::vec4(0.0f, 0.0f, 5.0f, 0.0f));
	if (keyInput.d == GLFW_PRESS) rightVel = glm::vec3(glm::rotate(model, rotY, yAxis) * glm::vec4(5.0f, 0.0f, 0.0f, 0.0f));
	if (keyInput.space == GLFW_PRESS && rigid.velocity.y < 0.001f) upVel = glm::vec3(0.0f, 8.0f, 0.0f);
	
	rigid.velocity += frontVel + backVel + rightVel + leftVel + upVel;
}


void PlayerCameraSystem::initialize() {
	coordinator.updateSystemComponentSet<PlayerCameraSystem>(coordinator.getComponentID<Empty>(), true);

	this->playerSystem = coordinator.getSystem<PlayerSystem>();
	this->cameraSystem = coordinator.getSystem<CameraSystem>();
}

void PlayerCameraSystem::update() {
	auto cameraID = this->cameraSystem->entities.begin();
	auto playerID = this->playerSystem->entities.begin();

	auto& camera = coordinator.getComponent<Camera>(*cameraID);
	auto& player = coordinator.getComponent<Transformation>(*playerID);

	if (!camera.freeRoamIsEnabled) {
		float radius = 8.0f;
		
		camera.position.x = player.translation.x - radius * cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
		camera.position.y = player.translation.y - radius * sin(glm::radians(camera.pitch));
		camera.position.z = player.translation.z - radius * sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
		
		camera.front = glm::normalize(player.translation - camera.position);
		camera.right = glm::normalize(glm::cross(camera.front, camera.worldUp));
		camera.up = glm::normalize(glm::cross(camera.right, camera.front));
	}
}

void PlayerSystem::initialize() {
	coordinator.updateSystemComponentSet<PlayerSystem>(coordinator.getComponentID<Damage>(), true);
	coordinator.updateSystemComponentSet<PlayerSystem>(coordinator.getComponentID<Health>(), true);
	coordinator.updateSystemComponentSet<PlayerSystem>(coordinator.getComponentID<Score>(), true);
	coordinator.updateSystemComponentSet<PlayerSystem>(coordinator.getComponentID<Transformation>(), true);

	this->mouseInputSystem = coordinator.getSystem<MouseInputSystem>();
}

void PlayerSystem::update() {
	auto playerID = this->entities.begin();
	auto mouseID = this->mouseInputSystem->entities.begin();
	auto& player = coordinator.getComponent<Transformation>(*playerID);
	auto& mouseInput = coordinator.getComponent<MouseInput>(*mouseID);

	player.rotation.y -= mouseInput.xOffset;
}


void PlayerCollisionSystem::initialize() {
	coordinator.updateSystemComponentSet<PlayerCollisionSystem>(coordinator.getComponentID<Enemy>(), true);
}

void PlayerCollisionSystem::setPlayer(int player) {
	this->player = player;
}

void PlayerCollisionSystem::update() {
	auto& playerTransf = coordinator.getComponent<Transformation>(this->player);
	auto& playerRigid = coordinator.getComponent<RigidBody>(this->player);

	glm::vec2 playerXZpos(playerTransf.translation.x, playerTransf.translation.z);

	std::vector<int> enemiesToDelete;

	for (int enemyID : this->entities) {
		auto& enemyTransf = coordinator.getComponent<Transformation>(enemyID);

		glm::vec2 enemyXZpos(enemyTransf.translation.x, enemyTransf.translation.z);
		
		if (playerTransf.translation.y <= enemyTransf.translation.y + enemyTransf.scale.y && glm::distance(playerXZpos, enemyXZpos) < enemyTransf.scale.x && playerRigid.velocity.y < 0.0f) {
			playerRigid.velocity.y = 8.0f;

			auto& enemy = coordinator.getComponent<Enemy>(enemyID);
			auto& playerDmg = coordinator.getComponent<Damage>(this->player);
			auto& enemyHealth = coordinator.getComponent<Health>(enemyID);

			enemyHealth.points -= playerDmg.points;
			if (enemyHealth.points <= 0) enemiesToDelete.push_back(enemyID);

		}
	}

	for (const auto& id : enemiesToDelete) coordinator.destroyEntity(id);

	enemiesToDelete.clear();

}


void MouseInputSystem::initialize() {
	coordinator.updateSystemComponentSet<MouseInputSystem>(coordinator.getComponentID<MouseInput>(), true);
}

void MouseInputSystem::update() {
	double xposIn, yposIn;
	glfwGetCursorPos(windowManager.getWindow(), &xposIn, &yposIn);

	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	for (int entityID : this->entities) {

		auto& mouse = coordinator.getComponent<MouseInput>(entityID);

		if (mouse.firstMouse)
		{
			mouse.lastX = xpos;
			mouse.lastY = ypos;
			mouse.firstMouse = false;
		}

		mouse.xOffset = xpos - mouse.lastX;
		mouse.yOffset = mouse.lastY - ypos; // reversed since y-coordinates go from bottom to top

		mouse.lastX = xpos;
		mouse.lastY = ypos;

		mouse.xOffset *= mouse.sensitivity;
		mouse.yOffset *= mouse.sensitivity;
	}
}

void EnemyMovementSystem::initialize() {
	this->player = player;

	coordinator.updateSystemComponentSet<EnemyMovementSystem>(coordinator.getComponentID<Enemy>(), true);
}

void EnemyMovementSystem::setPlayer(int player) {
	this->player = player;
}

void EnemyMovementSystem::update() {
	auto& playerTransf = coordinator.getComponent<Transformation>(this->player);

	for (int enemyID : this->entities) {
		auto& enemyTransf = coordinator.getComponent<Transformation>(enemyID);
		if (enemyTransf.translation.y < 0.001) {
			auto& rigidBody = coordinator.getComponent<RigidBody>(enemyID);

			rigidBody.velocity = 5.0f * glm::normalize(playerTransf.translation - enemyTransf.translation);
			rigidBody.velocity.y = 0.0f;
		}
	}
}


void EnemyCollisionSystem::initialize() {
	coordinator.updateSystemComponentSet<EnemyCollisionSystem>(coordinator.getComponentID<Enemy>(), true);
}

void EnemyCollisionSystem::update() {
	std::set<int>::iterator itr1;
	std::set<int>::iterator itr2;

	for (itr1 = this->entities.begin(); itr1 != this->entities.end(); itr1++) {
		for (itr2 = this->entities.begin(); itr2 != this->entities.end(); itr2++) {
			if (itr2 == itr1) continue;
			auto& rigid_1 = coordinator.getComponent<RigidBody>(*itr1);
			auto& transf_1 = coordinator.getComponent<Transformation>(*itr1);

			auto& rigid_2 = coordinator.getComponent<RigidBody>(*itr2);
			auto& transf_2 = coordinator.getComponent<Transformation>(*itr2);

			glm::vec3 moveDir(glm::normalize(rigid_1.velocity));

			if (glm::distance(transf_1.translation + transf_1.scale.x / 2 * moveDir, transf_2.translation) <= (transf_2.scale.x / 2 + 0.1)) rigid_1.velocity = glm::vec3(0.0f, rigid_1.velocity.y, 0.0f);
		}
	}
}


void PhysicsSystem::initialize() {
	coordinator.updateSystemComponentSet<PhysicsSystem>(coordinator.getComponentID<Gravitational>(), true);
	coordinator.updateSystemComponentSet<PhysicsSystem>(coordinator.getComponentID<RigidBody>(), true);
	coordinator.updateSystemComponentSet<PhysicsSystem>(coordinator.getComponentID<Transformation>(), true);
}

void PhysicsSystem::update(float timeStep) {
	for (int entityID : this->entities) {
		auto& rigidBody = coordinator.getComponent<RigidBody>(entityID);
		auto& transform = coordinator.getComponent<Transformation>(entityID);
		auto& gravity = coordinator.getComponent<Gravitational>(entityID);
		
		transform.translation += rigidBody.velocity * timeStep;
		rigidBody.velocity += gravity.force * timeStep;
		rigidBody.velocity.x = 0.0f;
		rigidBody.velocity.z = 0.0f;
	}
}


void RenderSystem::initialize() {
	coordinator.updateSystemComponentSet<RenderSystem>(coordinator.getComponentID<Shape>(), true);
	coordinator.updateSystemComponentSet<RenderSystem>(coordinator.getComponentID<Texture>(), true);
	coordinator.updateSystemComponentSet<RenderSystem>(coordinator.getComponentID<Transformation>(), true);

	this->textures["Textures/player.jpg"] = 0;
	this->textures["Textures/enemy.jpg"] = 0;
	this->textures["Textures/boundary.png"] = 0;
	this->textures["Textures/dmg_power-up.jpg"] = 0;

	this->playerShader = Shader("Shaders/boundary_vert.glsl", "Shaders/boundary_frag.glsl");		// temporarily using same shader files. plan on creating unique shader files
	this->boundaryShader = Shader("Shaders/boundary_vert.glsl", "Shaders/boundary_frag.glsl");		// in order to add custom lighting effects for each entity type
	this->enemyShader = Shader("Shaders/boundary_vert.glsl", "Shaders/boundary_frag.glsl");
	this->dmgPowerUpShader = Shader("Shaders/boundary_vert.glsl", "Shaders/boundary_frag.glsl");

	this->cameraSystem = coordinator.getSystem<CameraSystem>();
}

void RenderSystem::setShaders() {
	for (auto& entity : this->entities) {
		auto& shape = coordinator.getComponent<Shape>(entity);
		auto& texture = coordinator.getComponent<Texture>(entity);

		if (texture.fileName->find("boundary") != std::string::npos) shape.shader = &this->boundaryShader;
		else if (texture.fileName->find("player") != std::string::npos) shape.shader = &this->playerShader;
		else if (texture.fileName->find("enemy") != std::string::npos) shape.shader = &this->enemyShader;
		else if (texture.fileName->find("dmg_power-up") != std::string::npos) shape.shader = &this->dmgPowerUpShader;
	}
}

void RenderSystem::setVertexData(unsigned int& vao, unsigned int& vbo, std::vector<float>& vertices) {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void RenderSystem::setTextureData(std::string texture) {
	// load and create a texture 
		// -------------------------
	glGenTextures(1, &this->textures[texture]);
	glBindTexture(GL_TEXTURE_2D, this->textures[texture]); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	unsigned int textureID = this->textures[texture];
	this->textureParams[textureID] = { 0, 0, 0 };
	unsigned char* data = stbi_load(texture.c_str(), &this->textureParams[textureID][0], &this->textureParams[textureID][1], &this->textureParams[textureID][2], 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->textureParams[textureID][0], this->textureParams[textureID][1], 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

void RenderSystem::setRenderData() {
	this->setVertexData(this->cubeVAO, this->cubeVBO, cubeVertices);

	for (auto& texture : this->textures) this->setTextureData(texture.first);
}

glm::mat4 RenderSystem::getFullTranformation(int entity) {
	auto& transf = coordinator.getComponent<Transformation>(entity);

	// translation matrix
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), transf.translation);

	// rotation matrix with quaternion
	glm::vec3 rotationInRadians = glm::vec3(glm::radians(transf.rotation.x), glm::radians(transf.rotation.y), glm::radians(transf.rotation.z));
	glm::quat quaternion = glm::quat(rotationInRadians);
	glm::mat4 rotationMatrix = glm::toMat4(quaternion);

	// scale matrix (uniform scaling for now)
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), transf.scale);

	// update model matrix
	return translationMatrix * rotationMatrix * scaleMatrix;
}

void RenderSystem::update() {
	auto cameraID = this->cameraSystem->entities.begin();
	auto& camera = coordinator.getComponent<Camera>(*cameraID);

	glClearColor(0.68f, 0.85f, 0.9f, 1.0f); // light blue
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto const& entity : this->entities) {
		auto& texture = coordinator.getComponent<Texture>(entity);
		auto& shape = coordinator.getComponent<Shape>(entity);

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->textures[*texture.fileName]);

		shape.shader->use();

		glm::mat4 view = glm::lookAt(camera.position, camera.position + camera.front, camera.up);
		this->boundaryShader.setMat4("view", view);

		glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)1080 / (float)720, 0.1f, 100.0f);
		this->boundaryShader.setMat4("projection", projection);

		glBindVertexArray(this->cubeVAO);

		// create transformations
		glm::mat4 model = this->getFullTranformation(entity);
		this->boundaryShader.setMat4("model", model);

		// render box
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);
	}
}

void RenderSystem::shutDown() {
	glDeleteVertexArrays(1, &this->cubeVAO);

	glDeleteBuffers(1, &this->cubeVBO);
}