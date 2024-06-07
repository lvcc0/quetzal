#include "post_processing.h"

// Maybe we need to put here width and height of screen, but idk
PostProcessing::PostProcessing()
{
	glGenFramebuffers(1, &FBO_ID);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO_ID);

	glGenTextures(1, &TBO_ID);
	glBindTexture(GL_TEXTURE_2D, TBO_ID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TBO_ID, 0);

	glGenRenderbuffers(1, &RBO_ID);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO_ID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO_ID);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "ERROR::FRAMEBUFFER WASN`T COMPLETED::ID " << FBO_ID << std::endl;
	}
}
