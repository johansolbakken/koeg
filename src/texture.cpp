#include "texture.h"

#include "openglapi.h"
#include "stb_image.h"
#include "log.h"

Texture::Texture(TextureType type, const std::string& path)
		: m_type(type), m_filePath(path), m_localBuffer(nullptr), m_width(0), m_height(0), m_bpp(0)
{
	int width, height, bpp;
	stbi_set_flip_vertically_on_load(true);
	if (m_type == TextureType::Rgba)
	{
		m_localBuffer = stbi_load(path.c_str(), &width, &height, &bpp, 4);
	}
	else if (m_type == TextureType::Red)
	{
		m_localBuffer = stbi_load(path.c_str(), &width, &height, &bpp, 1);
	}
	else
	{
		LOG_ERROR("Unknown texture type");
	}
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

	if (m_type == TextureType::Rgba)
	{
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
				m_localBuffer));
	}
	else if (m_type == TextureType::Red)
	{
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_width, m_height, 0, GL_RED, GL_UNSIGNED_BYTE, m_localBuffer));
	}
	else
	{
		LOG_ERROR("Unknown texture type");
	}
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