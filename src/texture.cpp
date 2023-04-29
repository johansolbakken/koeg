#include "texture.h"

#include "openglapi.h"
#include "stb_image.h"
#include "log.h"

Texture::Texture(const std::string& path)
{
	int width, height, bpp;
	stbi_set_flip_vertically_on_load(true);
	m_localBuffer = stbi_load(path.c_str(), &width, &height, &bpp, 4);
	if (m_localBuffer == nullptr)
	{
		LOG_ERROR("Failed to load image");
	}

	m_width = width;
	m_height = height;

	GLCall(glGenTextures(1, &m_rendererId));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererId));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer));
	GLCall(glGenerateMipmap(GL_TEXTURE_2D));

	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_localBuffer)
	{
		stbi_image_free(m_localBuffer);
	}
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_rendererId));
}

void Texture::bind(uint32_t slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererId));
}

void Texture::unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}