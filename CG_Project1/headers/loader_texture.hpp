#include "stb_image.h"

#include <iostream>
#include <glad/glad.h>
#include <vector>

void LoadTexture(const char* fileName, std::vector<GLuint>& textures, unsigned int& textureCount);
