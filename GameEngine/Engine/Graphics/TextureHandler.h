#ifndef TEXTUREHANDLER_H
#define TEXTUREHANDLER_H

#include <memory>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include "../Core/Debug.h"
#include <gl/glew.h>

struct Texture
{
	GLuint textureID = 0;
	int width, height = 0;
	std::string textureName = "";
};

class TextureHandler
{
public:
	// disable copy/move constructors
	TextureHandler(const TextureHandler&) = delete;
	TextureHandler(TextureHandler&&) = delete;
	TextureHandler& operator=(const TextureHandler&) = delete;
	TextureHandler& operator=(TextureHandler&&) = delete;

	static TextureHandler* GetInstance();
	void OnDestroy();

	void CreateTexture(const std::string& textureName_, const std::string& textureFilePath_);
	const GLuint GetTexture(const std::string& textureName_);
	const Texture* GetTextureData(const std::string& textureName_);

private:
	TextureHandler();
	~TextureHandler();
	static std::unique_ptr<TextureHandler> textureInstance;
	friend std::default_delete<TextureHandler>;
	static std::vector<Texture*> textures;
};

#endif // !TEXTUREHANDLER_H

