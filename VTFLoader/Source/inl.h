
#include <strsafe.h>
#include <fstream>
using namespace std;

enum ImageFormat 
{
	IMAGE_FORMAT_UNKNOWN  = -1,
	IMAGE_FORMAT_RGBA8888 = 0,
	IMAGE_FORMAT_ABGR8888, 
	IMAGE_FORMAT_RGB888, 
	IMAGE_FORMAT_BGR888,
	IMAGE_FORMAT_RGB565, 
	IMAGE_FORMAT_I8,
	IMAGE_FORMAT_IA88,
	IMAGE_FORMAT_P8,
	IMAGE_FORMAT_A8,
	IMAGE_FORMAT_RGB888_BLUESCREEN,
	IMAGE_FORMAT_BGR888_BLUESCREEN,
	IMAGE_FORMAT_ARGB8888,
	IMAGE_FORMAT_BGRA8888,
	IMAGE_FORMAT_DXT1,
	IMAGE_FORMAT_DXT3,
	IMAGE_FORMAT_DXT5,
	IMAGE_FORMAT_BGRX8888,
	IMAGE_FORMAT_BGR565,
	IMAGE_FORMAT_BGRX5551,
	IMAGE_FORMAT_BGRA4444,
	IMAGE_FORMAT_DXT1_ONEBITALPHA,
	IMAGE_FORMAT_BGRA5551,
	IMAGE_FORMAT_UV88,
	IMAGE_FORMAT_UVWQ8888,
	IMAGE_FORMAT_RGBA16161616F,

	NUM_IMAGE_FORMATS
};


struct VTFFileHeader_t
{
	char fileTypeString[4]; // "VTF" Valve texture file
	int version[2]; 		// version[0].version[1]
	int headerSize;
	unsigned short	width;
	unsigned short	height;
	unsigned int	flags;
	unsigned short	numFrames;
	unsigned short	startFrame;
	unsigned char	Padding0[4];					//!< Reflectivity padding (16 byte alignment)
	float		reflectivity[3];					// This is a linear value, right?  Average of all frames?
	unsigned char	Padding1[4];					//!< Reflectivity padding (8 byte packing)
	float			bumpScale;
	ImageFormat		imageFormat;
	unsigned char	numMipLevels;
	unsigned char	lowResImageFormat;
	unsigned char	Padding2[3];
	unsigned char	lowResImageWidth;
	unsigned char	lowResImageHeight;	
	unsigned short	depth;
};
struct ImageFormatInfo_t
{
	char* m_pName;
	int m_NumBytes;
	int m_NumRedBits;
	int m_NumGreeBits;
	int m_NumBlueBits;
	int m_NumAlphaBits;
	bool m_IsCompressed;
};

static ImageFormatInfo_t g_ImageFormatInfo[] =
{
	{ "IMAGE_FORMAT_RGBA8888",	4, 8, 8, 8, 8, false }, // IMAGE_FORMAT_RGBA8888,
	{ "IMAGE_FORMAT_ABGR8888",	4, 8, 8, 8, 8, false }, // IMAGE_FORMAT_ABGR8888, 
	{ "IMAGE_FORMAT_RGB888",	3, 8, 8, 8, 0, false }, // IMAGE_FORMAT_RGB888,
	{ "IMAGE_FORMAT_BGR888",	3, 8, 8, 8, 0, false }, // IMAGE_FORMAT_BGR888,
	{ "IMAGE_FORMAT_RGB565",	2, 5, 6, 5, 0, false }, // IMAGE_FORMAT_RGB565, 
	{ "IMAGE_FORMAT_I8",		1, 0, 0, 0, 0, false }, // IMAGE_FORMAT_I8,
	{ "IMAGE_FORMAT_IA88",		2, 0, 0, 0, 8, false }, // IMAGE_FORMAT_IA88
	{ "IMAGE_FORMAT_P8",		1, 0, 0, 0, 0, false }, // IMAGE_FORMAT_P8
	{ "IMAGE_FORMAT_A8",		1, 0, 0, 0, 8, false }, // IMAGE_FORMAT_A8
	{ "IMAGE_FORMAT_RGB888_BLUESCREEN", 3, 8, 8, 8, 0, false },	// IMAGE_FORMAT_RGB888_BLUESCREEN
	{ "IMAGE_FORMAT_BGR888_BLUESCREEN", 3, 8, 8, 8, 0, false },	// IMAGE_FORMAT_BGR888_BLUESCREEN
	{ "IMAGE_FORMAT_ARGB8888",	4, 8, 8, 8, 8, false }, // IMAGE_FORMAT_ARGB8888
	{ "IMAGE_FORMAT_BGRA8888",	4, 8, 8, 8, 8, false }, // IMAGE_FORMAT_BGRA8888
	{ "IMAGE_FORMAT_DXT1",		0, 0, 0, 0, 0, true }, // IMAGE_FORMAT_DXT1
	{ "IMAGE_FORMAT_DXT3",		0, 0, 0, 0, 8, true }, // IMAGE_FORMAT_DXT3
	{ "IMAGE_FORMAT_DXT5",		0, 0, 0, 0, 8, true }, // IMAGE_FORMAT_DXT5
	{ "IMAGE_FORMAT_BGRX8888",	4, 8, 8, 8, 0, false }, // IMAGE_FORMAT_BGRX8888
	{ "IMAGE_FORMAT_BGR565",	2, 5, 6, 5, 0, false }, // IMAGE_FORMAT_BGR565
	{ "IMAGE_FORMAT_BGRX5551",	2, 5, 5, 5, 0, false }, // IMAGE_FORMAT_BGRX5551
	{ "IMAGE_FORMAT_BGRA4444",	2, 4, 4, 4, 4, false },	 // IMAGE_FORMAT_BGRA4444
	{ "IMAGE_FORMAT_DXT1_ONEBITALPHA",		0, 0, 0, 0, 0, true }, // IMAGE_FORMAT_DXT1_ONEBITALPHA
	{ "IMAGE_FORMAT_BGRA5551",	2, 5, 5, 5, 1, false }, // IMAGE_FORMAT_BGRA5551
	{ "IMAGE_FORMAT_UV88",	    2, 8, 8, 0, 0, false }, // IMAGE_FORMAT_UV88
	{ "IMAGE_FORMAT_UVWQ8888",	    4, 8, 8, 8, 8, false }, // IMAGE_FORMAT_UV88
	{ "IMAGE_FORMAT_RGBA16161616F",	    8, 16, 16, 16, 16, false }, // IMAGE_FORMAT_RGBA16161616F
};
inline ImageFormatInfo_t const& ImageFormatInfo( ImageFormat fmt )
{
	assert( fmt < NUM_IMAGE_FORMATS );
	return g_ImageFormatInfo[fmt];
}
inline int SizeInBytes( ImageFormat fmt )
{
	return ImageFormatInfo(fmt).m_NumBytes;
}
inline int GetMemRequired( int width, int height, ImageFormat imageFormat, bool mipmap )
{
	if( !mipmap )
	{
		if( imageFormat == IMAGE_FORMAT_DXT1 ||
			imageFormat == IMAGE_FORMAT_DXT3 ||
			imageFormat == IMAGE_FORMAT_DXT5 )
		{
			assert( ( width < 4 ) || !( width % 4 ) );
			assert( ( height < 4 ) || !( height % 4 ) );
			if( width < 4 && width > 0 )
			{
				width = 4;
			}
			if( height < 4 && height > 0 )
			{
				height = 4;
			}
			int numBlocks = ( width * height ) >> 4;
			switch( imageFormat )
			{
			case IMAGE_FORMAT_DXT1:
				return numBlocks * 8;
				break;
			case IMAGE_FORMAT_DXT3:
			case IMAGE_FORMAT_DXT5:
				return numBlocks * 16;
				break;
			default:
				assert( 0 );
				return 0;
				break;
			}
		}
		else
		{
			return width * height * SizeInBytes(imageFormat);
		}
	}
	else
	{
		int memSize = 0;

		while( 1 )
		{
			memSize += GetMemRequired( width, height, imageFormat, false );
			if( width == 1 && height == 1 )
			{
				break;
			}
			width >>= 1;
			height >>= 1;
			if( width < 1 )
			{
				width = 1;
			}
			if( height < 1 )
			{
				height = 1;
			}
		}

		return memSize;
	}
}