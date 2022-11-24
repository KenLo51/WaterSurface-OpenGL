#include "AnimatedTexture.h"

AnimatedTexture::AnimatedTexture(const char* folder) {
	loadFiles(folder);
	currFrameTime = 0.0f;
	currFrame = 0;
}

void AnimatedTexture::loadFiles(const char* folder) {
	unsigned int idx = 0;
	for (const auto& entry : std::filesystem::directory_iterator(folder)) {
		std::string texPath = entry.path().string();
		std::cout << "AnimatedTexture loading : " << texPath << std::endl;
		Texture* newTexture = new Texture(texPath.c_str(), 5);
		frames.push_back(newTexture);
		delayTime.push_back(0.033f);
		idx++;
	}
}

void AnimatedTexture::BindAndUse(Shader& shader, const char* uniform, float deltaTime) {
	currFrameTime += deltaTime;
	while (currFrameTime >= delayTime[currFrame]) {
		currFrameTime = currFrameTime - delayTime[currFrame];
		currFrame = (currFrame + 1) % delayTime.size();
	}
	while (currFrameTime < 0) {
		currFrame = (currFrame == 0) ? delayTime.size() - 1 : currFrame - 1;
		currFrameTime = currFrameTime + delayTime[currFrame];
	}
	//std::cout << "draw Frame : " << currFrame << std::endl;
	AnimatedTexture::frames[currFrame]->Bind();
	AnimatedTexture::frames[currFrame]->Use(shader, uniform);
}