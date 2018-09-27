#include "FrameworkPCH.h"
#include "TextureData.h"

TextureData::TextureData(const std::string& path)
{
	// Load image at specified path
	std::string fullPath{ path };
	SDL_Surface* pLoadedSurface = IMG_Load(fullPath.c_str());
	if (pLoadedSurface == nullptr)
	{
		std::cerr << "TextureData::CreateFromImage, error when calling IMG_Load: " << SDL_GetError() << std::endl;
		return;
	}
	CreateFromSurface(pLoadedSurface);

	// Free loaded surface
	SDL_FreeSurface(pLoadedSurface);
}

TextureData::TextureData(const std::string & text, const std::string & fontPath, int ptSize, const glm::vec4 & txtColor)
{
	// Create font
	TTF_Font* pFont = TTF_OpenFont(fontPath.c_str(), ptSize);

	if (pFont == nullptr)
	{
		std::cerr << "TextureData::CreateFromString, error when calling TTF_OpenFont: " << TTF_GetError() << std::endl;
	}

	// Render text surface
	SDL_Color textColor{};
	textColor.r = Uint8(txtColor.r * 255);
	textColor.g = Uint8(txtColor.g * 255);
	textColor.b = Uint8(txtColor.b * 255);
	textColor.a = Uint8(txtColor.a * 255);

	SDL_Surface* pLoadedSurface = TTF_RenderText_Blended(pFont, text.c_str(), textColor);
	if (pLoadedSurface == nullptr)
	{
		std::cerr << "TextureData::CreateFromString, error when calling TTF_RenderText_Blended: " << TTF_GetError() << std::endl;
	}

	// Copy to video memory
	CreateFromSurface(pLoadedSurface);

	// Free loaded surface
	SDL_FreeSurface(pLoadedSurface);

	//Free font
	TTF_CloseFont(pFont);
}

TextureData::~TextureData()
{
	glDeleteTextures(1, &m_Id);
}

const GLuint & TextureData::GetTextureId()
{
	return m_Id;
}

const glm::vec2 & TextureData::GetTextureSize()
{
	return m_Size;
}

void TextureData::CreateFromSurface(SDL_Surface * pSurface)
{
	//Get image dimensions
	m_Size.x = float(pSurface->w);
	m_Size.y = float(pSurface->h);

	// Get pixel format information and translate to OpenGl format
	GLenum pixelFormat{ GL_RGB };
	switch (pSurface->format->BytesPerPixel)
	{
	case 3:
		if (pSurface->format->Rmask == 0x000000ff)
		{
			pixelFormat = GL_RGB;
		}
		else
		{
			pixelFormat = GL_BGR;
		}
		break;
	case 4:
		if (pSurface->format->Rmask == 0x000000ff)
		{
			pixelFormat = GL_RGBA;
		}
		else
		{
			pixelFormat = GL_BGRA;
		}
		break;
	default:
		std::cerr << "TextureData::CreateFromSurface, unknow pixel format, BytesPerPixel: " << pSurface->format->BytesPerPixel << "\nUse 32 bit or 24 bit images.\n";
		return;
	}

	//Generate an array of textures.  We only want one texture (one element array), so trick
	//it by treating "texture" as array of length one.
	glGenTextures(1, &m_Id);

	//Select (bind) the texture we just generated as the current 2D texture OpenGL is using/modifying.
	//All subsequent changes to OpenGL's texturing state for 2D textures will affect this texture.
	glBindTexture(GL_TEXTURE_2D, m_Id);
	// check for errors. Can happen if a texture is created while a static pointer is being initialized, even before the call to the main function.
	GLenum e = glGetError();
	if (e != GL_NO_ERROR)
	{
		std::cerr << "TextureData::CreateFromSurface, error binding textures, Error id = " << e << '\n';
		std::cerr << "Can happen if a texture is created before performing the initialization code (e.g. a static TextureData object).\n";
		std::cerr << "There might be a white rectangle instead of the image.\n";
	}

	// Specify the texture's data.  
	// This function is a bit tricky, and it's hard to find helpful documentation. 
	// A summary:
	//    GL_TEXTURE_2D:    The currently bound 2D texture (i.e. the one we just made)
	//                0:    The mipmap level.  0, since we want to update the base level mipmap image (i.e., the image itself,
	//                         not cached smaller copies)
	//          GL_RGBA:    Specifies the number of color components in the texture.
	//                     This is how OpenGL will store the texture internally (kinda)--
	//                     It's essentially the texture's type.
	//       surface->w:    The width of the texture
	//       surface->h:    The height of the texture
	//                0:    The border.  Don't worry about this if you're just starting.
	//      pixelFormat:    The format that the *data* is in--NOT the texture! 
	// GL_UNSIGNED_BYTE:    The type the data is in.  In SDL, the data is stored as an array of bytes, with each channel
	//                         getting one byte.  This is fairly typical--it means that the image can store, for each channel,
	//                         any value that fits in one byte (so 0 through 255).  These values are to be interpreted as
	//                         *unsigned* values (since 0x00 should be dark and 0xFF should be bright).
	//  surface->pixels:    The actual data.  As above, SDL's array of bytes.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pSurface->w, pSurface->h, 0, pixelFormat, GL_UNSIGNED_BYTE, pSurface->pixels);

	// Set the minification and magnification filters.  In this case, when the texture is minified (i.e., the texture's pixels (texels) are
	// *smaller* than the screen pixels you're seeing them on, linearly filter them (i.e. blend them together).  This blends four texels for
	// each sample--which is not very much.  Mipmapping can give better results.  Find a texturing tutorial that discusses these issues
	// further.  Conversely, when the texture is magnified (i.e., the texture's texels are *larger* than the screen pixels you're seeing
	// them on), linearly filter them.  Qualitatively, this causes "blown up" (overmagnified) textures to look blurry instead of blocky.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}