#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


// empty class for future
// main idea: post processing uses custom frame buffers and using them for post processing shaders
// making FBO(Frame Buffer Object) should be main part of code in constructor(or not constructor, idk now)
// MAIN RULES FOR EXISTING FRAME BUFFERS::
// 1) At least one buffer (color, depth or stencil) must be connected.
// 2) At least one color attachment must be present.
// 3) All connections must also be completed(provided with allocated memory).
// 4) Each buffer must have the same number of samples(now idk what`s this).
class PostProcessing {
public:
	// IDs of buffers
	GLuint FBO_ID;
	GLuint TBO_ID;
	GLuint RBO_ID;

	// Constructor
	PostProcessing();
};