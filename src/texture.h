#pragma once

#include <string>

enum class TextureFormat
{
	Rgba,
	Red
};

enum class TextureType
{
	Diffuse,
	Specular,
	Normal,
	Height
};

std::string textureTypeToString(TextureType type);

class Texture
{
public:
	Texture(TextureType type, TextureFormat format, const std::string& path);

	Texture(TextureType type, TextureFormat format, int width, int height, int bpp,
			uint8_t* localBuffer);

	~Texture();

	void bind(uint32_t slot = 0) const;

	void unbind() const;

	inline int width() const
	{
		return m_width;
	}

	inline int height() const
	{
		return m_height;
	}

	inline TextureFormat format() const
	{
		return m_format;
	}

	inline TextureType type() const
	{
		return m_type;
	}

	static std::shared_ptr<Texture> create(TextureType type, TextureFormat format, const std::string& path);

	static std::shared_ptr<Texture> create(TextureType type, TextureFormat format, int width, int height, int bpp,
			uint8_t* localBuffer);


private:
	uint32_t m_rendererId;
	std::string m_filePath;
	uint8_t* m_localBuffer;
	int m_width, m_height, m_bpp;
	TextureFormat m_format;
	TextureType m_type;
};