
#include <SDL.h>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"
#include "shader.h"
#include "camera.h"
#include "vboindexer.hpp"
#include "loader_wavefront.hpp"
#include "loader_texture.hpp"

#include <iostream>
#include <vector>



int main(int argc, char** argv)
{
	float screenWidth = 800;
	float screenHeight = 600;

	float centerX = screenWidth / 2.0f;
	float centerY = screenHeight / 2.0f;

	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	SDL_Window* window = SDL_CreateWindow("OpenGL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_OPENGL);
	if (window == nullptr)
	{
		std::cout << "Failed to create SDL Window" << std::endl;
		SDL_Quit();
		return -1;
	}

	SDL_GLContext context = SDL_GL_CreateContext(window);

	SDL_SetRelativeMouseMode(SDL_TRUE);

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		SDL_Quit();
		return -2;
	}

	

	// Added a cube to use as a light
	float lightVerts[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};


	std::vector<float> vertices;
	LoadObj("resources/objects/untitled.obj", vertices);

	std::vector<float> verticesCube;
	LoadObj("resources/objects/box.obj", verticesCube);

	std::vector<float> suz_vbo;
	std::vector<unsigned int> suz_ibo;
	IndexVBO(vertices, suz_vbo, suz_ibo);

	std::vector<float> cube_vbo;
	std::vector<unsigned int> cube_ibo;
	IndexVBO(verticesCube, cube_vbo, cube_ibo);


	// added the new shaders
	Shader gouraudProgram("shaders/gouraudvertexmaterialshader.glsl", "shaders/gouraudfragmentmaterialshader.glsl");
	Shader phongProgram("shaders/phongvertexmaterialshader.glsl", "shaders/phongfragmentmaterialshader.glsl");
	Shader lightCubeProgram("shaders/lightvertex.glsl", "shaders/lightfragment.glsl");

	bool currentShader = true; //default phong


	GLuint vao;
	GLuint vao_cube;

	GLuint vbo;
	glGenBuffers(1, &vbo); // Generate 1 vertex buffer object
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, (suz_vbo.size() + cube_vbo.size()) * sizeof(float), 0, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, suz_vbo.size() * sizeof(float), suz_vbo.data());
	glBufferSubData(GL_ARRAY_BUFFER, suz_vbo.size() * sizeof(float), cube_vbo.size() * sizeof(float), cube_vbo.data());

	GLuint ibo;
	glGenBuffers(1, &ibo); // Generate 1 element buffer object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (suz_ibo.size() + cube_ibo.size()) * sizeof(unsigned int), 0, GL_STATIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, suz_ibo.size() * sizeof(unsigned int), suz_ibo.data());
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, suz_ibo.size() * sizeof(unsigned int), cube_ibo.size() * sizeof(unsigned int), cube_ibo.data());

	glGenVertexArrays(1, &vao);
	glGenVertexArrays(1, &vao_cube);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	phongProgram.setVertexAttribPointer("position", 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	phongProgram.setVertexAttribPointer("texCoord", 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	phongProgram.setVertexAttribPointer("normal", 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	
	gouraudProgram.setVertexAttribPointer("position", 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	gouraudProgram.setVertexAttribPointer("texCoord", 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	gouraudProgram.setVertexAttribPointer("normal", 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glBindVertexArray(vao_cube);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	
	phongProgram.setVertexAttribPointer("position", 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(suz_vbo.size() * sizeof(float)));
	phongProgram.setVertexAttribPointer("texCoord", 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)((suz_vbo.size() * sizeof(float)) + (3 * sizeof(float))));
	phongProgram.setVertexAttribPointer("normal", 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)((suz_vbo.size() * sizeof(float)) + (5 * sizeof(float))));
	
	gouraudProgram.setVertexAttribPointer("position", 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(suz_vbo.size() * sizeof(float)));
	gouraudProgram.setVertexAttribPointer("texCoord", 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)((suz_vbo.size() * sizeof(float)) + (3 * sizeof(float))));
	gouraudProgram.setVertexAttribPointer("normal", 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)((suz_vbo.size() * sizeof(float)) + (5 * sizeof(float))));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	
	

	stbi_set_flip_vertically_on_load(true);

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	LoadTexture("resources/textures/container.jpg");

	GLuint texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	LoadTexture("resources/textures/box_tex.png");


	glBindVertexArray(0);

	// added a vao/vbo for the light 
	GLuint vaoLightCube;
	glGenVertexArrays(1, &vaoLightCube);
	glBindVertexArray(vaoLightCube);

	GLuint vboLightCube;
	glGenBuffers(1, &vboLightCube); // Generate 1 vertex buffer object
	glBindBuffer(GL_ARRAY_BUFFER, vboLightCube);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lightVerts), lightVerts, GL_STATIC_DRAW);


	// added setVertex for the new light shader
	lightCubeProgram.setVertexAttribPointer("aPos", 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glBindVertexArray(0);

	Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);

	glm::mat4 view;
	view = camera.getViewMatrix();

	glm::mat4 model(1.0f);
	model = glm::rotate(model, glm::radians(-20.0f), glm::vec3(1.0, 0.0, 0.0));

	// added a model for the light to follow
	glm::mat4 model2(1.0f);

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(camera.getFov()), screenWidth / screenHeight, 0.1f, 100.0f);


	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	phongProgram.setInt("Texture", 0);
	gouraudProgram.setInt("Texture", 1);


	// added a light
	glm::vec3 lightPos(0.0f, 2.0f, 0.0f);

	//added variables to simulate materials
	glm::vec3 ambientColor(0.2f, 0.1f, 0.0f);
	glm::vec3 diffuseColor(0.8f, 0.4f, 0.0f);
	glm::vec3 specularColor(1.0f, 1.0f, 1.0f);
	float shinyAmount = 1.0f;
	float ambientCoefficient = 1.0f;
	float diffuseCoefficient = 1.0f;
	float specularCoefficient = 1.0f;

	SDL_Event event;

	int start = SDL_GetTicks();
	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrameTime = start; // Time of last frame

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	bool isRunning = true;

	bool print = true;

	while (isRunning) // render loop
	{
		int now = SDL_GetTicks();
		float deltaTime = (now - lastFrameTime) / 1000.0f;
		lastFrameTime = now;

		while (SDL_PollEvent(&event)) // event loop
		{
			if (event.type == SDL_QUIT || SDL_GetKeyboardState(NULL)[SDL_SCANCODE_ESCAPE])
				isRunning = false;

			if (event.type == SDL_MOUSEMOTION)
			{
				camera.processMouseInput(event, deltaTime);
			}

			if (event.type == SDL_MOUSEWHEEL)
			{
				camera.processMouseScroll(event.wheel.y);
			}

			
		}
		const Uint8* keyState = SDL_GetKeyboardState(nullptr);
		camera.processKeyboardInput(keyState, deltaTime);
		//light controls
		if (keyState[SDL_SCANCODE_I]) {
			lightPos = lightPos + glm::vec3(0.0f, 0.0f, -1.0f) * deltaTime;
		}
		if (keyState[SDL_SCANCODE_K]) {
			lightPos = lightPos + glm::vec3(0.0f, 0.0f, 1.0f) * deltaTime;
		}
		if (keyState[SDL_SCANCODE_J]) {
			lightPos = lightPos + glm::vec3(-1.0f, 0.0f, 0.0f) * deltaTime;
		}
		if (keyState[SDL_SCANCODE_L]) {
			lightPos = lightPos + glm::vec3(1.0f, 0.0f, 0.0f) * deltaTime;
		}
		if (keyState[SDL_SCANCODE_U]) {
			lightPos = lightPos + glm::vec3(0.0f, -1.0f, 0.0f) * deltaTime;	
		}
		if (keyState[SDL_SCANCODE_O]) {
			lightPos = lightPos + glm::vec3(0.0f, 1.0f, 0.0f) * deltaTime;
		}

		//shader controls
		if (keyState[SDL_SCANCODE_RETURN]) {
			currentShader = true; //swaps active shader to phong
		}
		if (keyState[SDL_SCANCODE_BACKSPACE]) {
			currentShader = false; //swaps active shader to gouraund
		}
		
		//material controls
		//ambient
		if (keyState[SDL_SCANCODE_KP_1]) {
			if (keyState[SDL_SCANCODE_R]) { //Red
				if (keyState[SDL_SCANCODE_UP]) {
					if (ambientColor.x < 1.0f) ambientColor.x += 0.1f * deltaTime;
				}
				if (keyState[SDL_SCANCODE_DOWN]) {
					if (ambientColor.x > 0.0f) ambientColor.x -= 0.1f * deltaTime;
				}
			}
			if (keyState[SDL_SCANCODE_G]) { //Green
				if (keyState[SDL_SCANCODE_UP]) {
					if (ambientColor.y < 1.0f) ambientColor.y += 0.1f * deltaTime;
				}
				if (keyState[SDL_SCANCODE_DOWN]) {
					if (ambientColor.y > 0.0f) ambientColor.y -= 0.1f * deltaTime;
				}
			}
			if (keyState[SDL_SCANCODE_B]) { //Blue
				if (keyState[SDL_SCANCODE_UP]) {
					if(ambientColor.z < 1.0f) ambientColor.z += 0.1f * deltaTime;
				}
				if (keyState[SDL_SCANCODE_DOWN]) {
					if (ambientColor.z > 0.0f) ambientColor.z -= 0.1f * deltaTime;
				}
			}
			//Coefficient
			if (keyState[SDL_SCANCODE_RIGHT]) {
				if (ambientCoefficient < 1.0f) ambientCoefficient += 0.1f * deltaTime;
			}
			if (keyState[SDL_SCANCODE_LEFT]) {
				if (ambientCoefficient > 0.0f) ambientCoefficient -= 0.1f * deltaTime;
			}
		}
		//difuse
		if (keyState[SDL_SCANCODE_KP_2]) {
			if (keyState[SDL_SCANCODE_R]) { //Red
				if (keyState[SDL_SCANCODE_UP]) {
					if (diffuseColor.x < 1.0f) diffuseColor.x += 0.1f * deltaTime;
				}
				if (keyState[SDL_SCANCODE_DOWN]) {
					if (diffuseColor.x > 0.0f) diffuseColor.x -= 0.1f * deltaTime;
				}
			}
			if (keyState[SDL_SCANCODE_G]) { //Green
				if (keyState[SDL_SCANCODE_UP]) {
					if (diffuseColor.y < 1.0f) diffuseColor.y += 0.1f * deltaTime;
				}
				if (keyState[SDL_SCANCODE_DOWN]) {
					if (diffuseColor.y > 0.0f) diffuseColor.y -= 0.1f * deltaTime;
				}
			}
			if (keyState[SDL_SCANCODE_B]) { //Blue
				if (keyState[SDL_SCANCODE_UP]) {
					if (diffuseColor.z < 1.0f) diffuseColor.z += 0.1f * deltaTime;
				}
				if (keyState[SDL_SCANCODE_DOWN]) {
					if(diffuseColor.z > 0.0f) diffuseColor.z -= 0.1f * deltaTime;
				}
			}
			//Coefficient
			if (keyState[SDL_SCANCODE_RIGHT]) {
				if (diffuseCoefficient < 1.0f) diffuseCoefficient += 0.1f * deltaTime;
			}
			if (keyState[SDL_SCANCODE_LEFT]) {
				if (diffuseCoefficient > 0.0f) diffuseCoefficient -= 0.1f * deltaTime;
			}
		}
		//ambient
		if (keyState[SDL_SCANCODE_KP_3]) {
			if (keyState[SDL_SCANCODE_R]) { //Red
				if (keyState[SDL_SCANCODE_UP]) {
					if (specularColor.x < 1.0f) specularColor.x += 0.1f * deltaTime;
				}
				if (keyState[SDL_SCANCODE_DOWN]) {
					if (specularColor.x > 0.0f) specularColor.x -= 0.1f * deltaTime;
				}
			}
			if (keyState[SDL_SCANCODE_G]) { //Green
				if (keyState[SDL_SCANCODE_UP]) {
					if (specularColor.y < 1.0f) specularColor.y += 0.1f * deltaTime;
				}
				if (keyState[SDL_SCANCODE_DOWN]) {
					if (specularColor.y > 0.0f) specularColor.y -= 0.1f * deltaTime;
				}
			}
			if (keyState[SDL_SCANCODE_B]) { //Blue
				if (keyState[SDL_SCANCODE_UP]) {
					if (specularColor.z < 1.0f) specularColor.z += 0.1f * deltaTime;
				}
				if (keyState[SDL_SCANCODE_DOWN]) {
					if(specularColor.z > 0.0f) specularColor.z -= 0.1f * deltaTime;
				}
			}
			//Coefficient
			if (keyState[SDL_SCANCODE_RIGHT]) {
				if(specularCoefficient < 1.0f) specularCoefficient += 0.1f * deltaTime;
			}
			if (keyState[SDL_SCANCODE_LEFT]) {
				if (specularCoefficient > 0.0f) specularCoefficient -= 0.1f * deltaTime;
			}
		}
		//shining
		if (keyState[SDL_SCANCODE_KP_0]) {
			if (keyState[SDL_SCANCODE_UP]) {
				if(shinyAmount < 32.0f) shinyAmount += 0.1f;
			}
			if (keyState[SDL_SCANCODE_DOWN]) {
				if (shinyAmount > 0.0f) shinyAmount -= 0.1f * deltaTime;
			}
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(vao);

		view = camera.getViewMatrix();
		projection = glm::perspective(glm::radians(camera.getFov()), screenWidth / screenHeight, 0.1f, 100.0f);
		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
		
		//set uniforms for new shaders and commented the ones from the old one
		if (currentShader) {
			phongProgram.use();
			phongProgram.setVec3("lightPos", lightPos);
			phongProgram.setVec3("viewPos", camera.GetPosition());

			phongProgram.setFloat("ambientCoefficient", ambientCoefficient);
			phongProgram.setFloat("diffuseCoefficient", diffuseCoefficient);
			phongProgram.setFloat("specularCoefficient", specularCoefficient);

			phongProgram.setVec3("ambientColor", ambientColor);
			phongProgram.setVec3("diffuseColor", diffuseColor);
			phongProgram.setVec3("specularColor", specularColor);

			phongProgram.setMat4("view", view);
			phongProgram.setMat4("model", model);
			phongProgram.setMat4("projection", projection);

		}
		else
		{
			gouraudProgram.use();
			gouraudProgram.setVec3("lightPos", lightPos);
			gouraudProgram.setVec3("viewPos", camera.GetPosition());

			gouraudProgram.setFloat("ambientCoefficient", ambientCoefficient);
			gouraudProgram.setFloat("diffuseCoefficient", diffuseCoefficient);
			gouraudProgram.setFloat("specularCoefficient", specularCoefficient);
			gouraudProgram.setFloat("shinyAmount", shinyAmount);

			gouraudProgram.setVec3("ambientColor", ambientColor);
			gouraudProgram.setVec3("diffuseColor", diffuseColor);
			gouraudProgram.setVec3("specularColor",specularColor);

			gouraudProgram.setMat4("view", view);
			gouraudProgram.setMat4("model", model);
			gouraudProgram.setMat4("projection", projection);
		}

		//added this in order to use a second vao for the light
		glBindVertexArray(vao);

		// draw suzanne
		glDrawElements(GL_TRIANGLES, suz_ibo.size(), GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);

		//draw second object 
		glBindVertexArray(vao_cube);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture2);


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(10.0f, 0.5f, 0.7f));
		phongProgram.setMat4("model", model);

		glDrawElements(GL_TRIANGLES, cube_ibo.size(), GL_UNSIGNED_INT, (void*)(suz_ibo.size() * sizeof(unsigned int)));


		//drew the light
		lightCubeProgram.use();
		lightCubeProgram.setMat4("projection", projection);
		lightCubeProgram.setMat4("view", view);
		model2 = glm::mat4(1.0f);
		model2 = glm::translate(model2, lightPos);
		model2 = glm::scale(model2, glm::vec3(0.2f)); // a smaller cube
		lightCubeProgram.setMat4("model", model2);

		glBindVertexArray(vaoLightCube);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);


		glBindVertexArray(0);
		


		SDL_GL_SwapWindow(window);
	}
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
