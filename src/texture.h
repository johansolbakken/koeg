#pragma once

#include <string>

enum class TextureType
{
	Rgba,
	Red
};

class Texture
{
public:
	Texture(TextureType type, const std::string& path);

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


private:
	uint32_t m_rendererId;
	std::string m_filePath;
	uint8_t* m_localBuffer;
	int m_width, m_height, m_bpp;
	TextureType m_type;
};