#include "texture.h"

#include "openglapi.h"
#include "stb_image.h"
#include "log.h"

std::string textureTypeToString(TextureType type)
{
	switch (type)
	{
	case TextureType::Diffuse:
		return "diffuse";
	case TextureType::Specular:
		return "specular";
	case TextureType::Normal:
		return "normal";
	case TextureType::Height:
		return "height";
	default:
		LOG_ERROR("Unknown texture type");
		return "";
	}
}

Texture::Texture(TextureType type, const std::string& path)
{
	int width, height, bpp;
	stbi_set_flip_vertically_on_load(true);
	m_localBuffer = stbi_load(path.c_str(), &width, &height, &bpp, 0);
	if (m_localBuffer == nullptr)
	{
		LOG_ERROR("Failed to load texture: {0}", path);
	}

	LOG_INFO("Loaded texture: {} {} {}", path, textureTypeToString(type), bpp);

	m_width = width;
	m_height = height;
	m_bpp = bpp;

	GLCall(glGenTextures(1, &m_rendererId));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererId));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

	if (bpp == 4)
	{
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
				m_localBuffer));
		m_format = TextureFormat::Rgba;
	}
	else if (bpp == 3)
	{
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE,
				m_localBuffer));
		m_format = TextureFormat::Rgb;
	}
	else if (bpp == 1)
	{
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_width, m_height, 0, GL_RED, GL_UNSIGNED_BYTE,
				m_localBuffer));
		m_format = TextureFormat::Red;
	}
	else
	{
		LOG_ERROR("Unknown texture format");
	}

	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	stbi_image_free(m_localBuffer);
}

Texture::Texture(TextureType type, TextureFormat format, const std::string& path)
		: m_type(type), m_format(format), m_filePath(path), m_localBuffer(nullptr), m_width(0), m_height(0), m_bpp(0)
{
	int width, height, bpp;
	stbi_set_flip_vertically_on_load(true);
	m_localBuffer = stbi_load(path.c_str(), &width, &height, &bpp, 0);
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

	uint32_t glType = 0;
	if (format == TextureFormat::Rgba)
	{
		glType = GL_RGBA;
	}
	else if (format == TextureFormat::Rgb)
	{
		glType = GL_RGB;
	}
	else if (format == TextureFormat::Red)
	{
		glType = GL_RED;
	}
	else
	{
		LOG_ERROR("Unknown texture format");
	}

	if (bpp == 4)
	{
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, glType, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
				m_localBuffer));
	}
	else if (bpp == 3)
	{
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, glType, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE,
				m_localBuffer));
	}
	else if (bpp == 1)
	{
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, glType, m_width, m_height, 0, GL_RED, GL_UNSIGNED_BYTE,
				m_localBuffer));
	}
	else
	{
		LOG_ERROR("Unknown texture format");
	}
	GLCall(glGenerateMipmap(GL_TEXTURE_2D));

	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_localBuffer)
	{
		stbi_image_free(m_localBuffer);
	}
}

Texture::Texture(TextureType type, TextureFormat format, int width, int height, int bpp,
		uint8_t* localBuffer)
{
	m_format = format;
	m_localBuffer = localBuffer;
	m_width = width;
	m_height = height;
	m_bpp = bpp;

	GLCall(glGenTextures(1, &m_rendererId));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererId));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

	if (bpp == 4)
	{
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
				m_localBuffer));
		m_format = TextureFormat::Rgba;
	}
	else if (bpp == 3)
	{
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE,
				m_localBuffer));
		m_format = TextureFormat::Rgb;
	}
	else if (bpp == 1)
	{
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_width, m_height, 0, GL_RED, GL_UNSIGNED_BYTE,
				m_localBuffer));
		m_format = TextureFormat::Red;
	}
	else
	{
		LOG_ERROR("Unknown texture format");
	}
	GLCall(glGenerateMipmap(GL_TEXTURE_2D));

	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
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

std::shared_ptr<Texture> Texture::create(TextureType type, TextureFormat format, const std::string& path)
{
	return std::make_shared<Texture>(type, format, path);
}

std::shared_ptr<Texture> Texture::create(TextureType type, TextureFormat format, int width, int height, int bpp,
		uint8_t* localBuffer)
{
	return std::make_shared<Texture>(type, format, width, height, bpp, localBuffer);
}

std::shared_ptr<Texture> Texture::create(TextureType type, const std::string& path)
{
	return std::make_shared<Texture>(type, path);
}