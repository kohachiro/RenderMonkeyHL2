//=================================================================================================//
// filename: VTFLoaderPlugIn.h                                                          //
//                                                                                                 //
//           ATI Research, Inc.                                                                    //
//           3D Application Research Group                                                         //
//                                                                                                 //
// description: Image Loader PlugIn.                                                                   //
//                                                                                                 //
//=================================================================================================//
//   (C) 2004 ATI Research, Inc.  All rights reserved.                                             //
//=================================================================================================//

#include "VTFLoaderPCH.h"
#include "VTFLoaderMain.h"
#include "VTFLoaderPlugIn.h"
#include "Shlwapi.h"

#include "inl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//..............................................................................................................//
//..............................................................................................................//
//..............................................................................................................//
//
// Texture loader plug-in implementation
//
//..............................................................................................................//
//..............................................................................................................//
//..............................................................................................................//


//-------------------------------------------------------------------------------------------------//
// Class:       CVTFLoaderPlugIn
// 
// Function:    Constructor
//
//-------------------------------------------------------------------------------------------------//

CVTFLoaderPlugIn::CVTFLoaderPlugIn() : IRmTextureLoaderPlugIn()
{
   // This plug-in's type is a Texture plug-in, we must explicitly specify it:
   m_plugInDesc.plugInType = RM_PLUGINTYPE_TEXTURE_LOADER;

   RmPlugInID plugInID = { 0xf51438ea, 0xea7d, 0x4be8, { 0xb6, 0x5, 0x8a, 0x21, 0xa0, 0x81, 0xc9, 0x8d } };
   m_plugInDesc.plugInID   = plugInID;

   // There is only one type of node that this plug-in is associated with by the main application:
   m_plugInDesc.nNumSupportedNodeTypes = 0;

   // This plug-in is implemented using 1.0 Wizard version:
   m_plugInDesc.nMajorSDKVersion = RENDERMONKEY_SDK_CURRENT_VERSION_MAJOR;
   m_plugInDesc.nMinorSDKVersion = RENDERMONKEY_SDK_CURRENT_VERSION_MINOR;

   // This plug-in in can be used in effects of all types and thus doesn't care about the
   // graphics API version:
   RmStrCopyT( m_plugInDesc.strRenderAPIVersion, RM_API_AGNOSTIC );

   // The name that the user will see for this plug-in
   RmStrCopyT( m_plugInDesc.strName, _T( "VTFLoader" ) );
   
} // End of Constructor for CVTFLoaderPlugIn

//-------------------------------------------------------------------------------------------------//
// Class:       CVTFLoaderPlugIn
// 
// Function:    ~CVTFLoaderPlugIn
// 
// Description: Destructor
//
//-------------------------------------------------------------------------------------------------//

CVTFLoaderPlugIn::~CVTFLoaderPlugIn()
{
} // End of Destructor for CVTFLoaderPlugIn


//-------------------------------------------------------------------------------------------------//
// Class:       CVTFLoaderPlugIn
// 
// Function:    Init
// 
// Description: Initializes this particular plug-in
//
//-------------------------------------------------------------------------------------------------//
bool CVTFLoaderPlugIn::Init()
{
   return true;
} // End of Init for CVTFLoaderPlugIn

//-------------------------------------------------------------------------------------------------//
// Class:       CVTFLoaderPlugIn
// 
// Function:    Uninitialize
// 
// Description: Performs uninitialization of the plug-in specific data
//
//-------------------------------------------------------------------------------------------------//

void CVTFLoaderPlugIn::Uninitialize()
{
} // End of Uninitialize for CVTFLoaderPlugIn


//-------------------------------------------------------------------------------------------------//
// Class:       CVTFLoaderPlugIn
// 
// Function:    MessageHandler
// 
// Description: RenderMonkey main message handling entry point. RenderMonkey communicates
//              all of its events with the loaded plug-ins via messages. See 
//              Wizard/Include/Core/RmDefines.h for more details on each message.
//
// Returns:     RM_OK if the message was successfully handled and RM_EFAIL otherwise.
//
//-------------------------------------------------------------------------------------------------//
int CVTFLoaderPlugIn::MessageHandler( int nMessageID,
                                                int nMessageData,
                                                int nMessageParameter /* = 0 */,
                                                const RmPlugInID* pRmPlugInID /* = NULL */)
{
   return RM_OK;
} // End of MessageHandler for CVTFLoaderPlugIn



//-------------------------------------------------------------------------------------------------//
// Class:       CVTFLoaderPlugIn
// 
// Function:    GetSupportedExtensions
// 
// Description: This method returns a list of file extensions that are supported by this Texture
//              loader plug-in for importing Texture from. These values are used to control the
//              display of files in the FileOpen dialog for model load. Note that number of entries
//              for 'formatDescriptions' and 'formatExtensions' must match.
//              
//              Eg. For X file, extension = "*.X"
//                              text      = "X Studio File (*.X)"
//              
//-------------------------------------------------------------------------------------------------//
void CVTFLoaderPlugIn::GetSupportedExtensions( RmTextureType textureType,
                                                   RmLinkedList<RmStringT> &formatDescriptions,
                                                   RmLinkedList<RmStringT> &formatExtensions )
{
   // Add your code for your format's extension

   formatDescriptions.push_back(_T("HL2 Textrue File (*.vtf)"));
   formatExtensions.push_back(_T("*.vtf"));
} // End of GetSupportedExtensions for CVTFLoaderPlugIn


//-------------------------------------------------------------------------------------------------//
// Class:       CVTFLoaderPlugIn
// 
// Function:    CanLoadTexture
// 
// Description: Method checks whether this plug-in can load Texture in the specified file
//              based on its own criteria. Returns true if it can load the file and false 
//              otherwise.
//
//-------------------------------------------------------------------------------------------------//
bool CVTFLoaderPlugIn::CanLoadTexture( RmTextureType textureType,
                                           const RM_TCHAR *strFileName )
{
   // Add your check if file is loadable here
	bool bResult = false;
	WCHAR tmpStringName[MAX_PATH];
	StringCchCopy( tmpStringName, MAX_PATH, strFileName );
	RM_TCHAR* pch = wcsrchr( tmpStringName, L'\.' );
	if( 0 == wcscmp( pch, L".vtf" ) )
		bResult = true;
	return bResult;
} // End of CanLoadTexture for CVTFLoaderPlugIn


//-------------------------------------------------------------------------------------------------//
// Load 2D Texture from the given file described by the specified file name                 //
//-------------------------------------------------------------------------------------------------//
bool CVTFLoaderPlugIn::LoadTexture( const RM_TCHAR    *strFileName, 
                                        RmTexture* pTexture,
                                        RmPixelFormatType format )
{	
	// Add your loading code here
	ifstream inFile( strFileName,ios::binary );
	inFile.seekg(0,ios::end);
	int fileSize   =   inFile.tellg();
	inFile.seekg(0,ios::beg);
	char *pMem = new char[fileSize];
	inFile.read(pMem,fileSize);
	VTFFileHeader_t* pVtf=(VTFFileHeader_t *)pMem;
	
	//getRmApp()->OutputText( _T("\nmidmap=%d type=%d w=%d h=%d\n"), pVtf->numMipLevels,pVtf->lowResImageFormat, pVtf->lowResImageWidth,pVtf->lowResImageHeight );
	int memRequired = GetMemRequired( pVtf->width, pVtf->height, pVtf->imageFormat, false );
	int memOffset = GetMemRequired( pVtf->width, pVtf->height, pVtf->imageFormat, true ) - memRequired;
	int ThumbnailBufferSize = GetMemRequired( pVtf->lowResImageWidth, pVtf->lowResImageHeight, (ImageFormat)pVtf->lowResImageFormat, false );
	int ImageDataOffset = pVtf->headerSize + ThumbnailBufferSize;
	unsigned char* pSrc  = (unsigned char*)(((unsigned char*)pVtf) + ImageDataOffset + memOffset );	
	Rm2DTexture *pRm2DTexture=(Rm2DTexture *)pTexture;
	bool isCopy=false;
	switch (pVtf->imageFormat)
	{
		case IMAGE_FORMAT_RGBA8888:
			format=RM_PIXELFORMAT_A8R8G8B8;
			isCopy=false;
			break;
		case IMAGE_FORMAT_ABGR8888:
			format=RM_PIXELFORMAT_A8R8G8B8;
			isCopy=false;
			break;
		case IMAGE_FORMAT_RGB888:
			format=RM_PIXELFORMAT_X8R8G8B8;
			isCopy=false;
			break;
		case IMAGE_FORMAT_BGR888:
			format=RM_PIXELFORMAT_X8R8G8B8;
			isCopy=false;
			break;
		case IMAGE_FORMAT_RGB565:
			format=RM_PIXELFORMAT_R5G6B5;
			isCopy=false;
			break;
		case IMAGE_FORMAT_I8:
			format=RM_PIXELFORMAT_L8;//?
			isCopy=true;
			break;
		case IMAGE_FORMAT_IA88:
			format=RM_PIXELFORMAT_A8L8;//?
			isCopy=true;
			break;
		case IMAGE_FORMAT_P8:
			format=RM_PIXELFORMAT_P8;
			isCopy=true;
			break;
		case IMAGE_FORMAT_A8:
			format=RM_PIXELFORMAT_A8;
			isCopy=true;
			break;
		case IMAGE_FORMAT_RGB888_BLUESCREEN:
			format=RM_PIXELFORMAT_X8R8G8B8;
			isCopy=false;
			break;			
		case IMAGE_FORMAT_ARGB8888:
			format=RM_PIXELFORMAT_A8R8G8B8;
			isCopy=false;
			break;
		case IMAGE_FORMAT_BGRA8888:
			format=RM_PIXELFORMAT_A8R8G8B8;
			isCopy=true;
			break;
		case IMAGE_FORMAT_DXT1:
			format=RM_PIXELFORMAT_DXT1;
			isCopy=true;
			break;
		case IMAGE_FORMAT_DXT3:
			format=RM_PIXELFORMAT_DXT3;
			isCopy=true;
			break;
		case IMAGE_FORMAT_DXT5:
			format=RM_PIXELFORMAT_DXT5;
			isCopy=true;
			break;
		case IMAGE_FORMAT_BGRX8888:
			format=RM_PIXELFORMAT_X8R8G8B8;
			isCopy=true;
			break;
		case IMAGE_FORMAT_BGR565:
			format=RM_PIXELFORMAT_R5G6B5;
			isCopy=true;
			break;
		case IMAGE_FORMAT_BGRX5551:
			format=RM_PIXELFORMAT_A1R5G5B5;
			isCopy=true;
			break;
		case IMAGE_FORMAT_BGRA4444:
			format=RM_PIXELFORMAT_A4R4G4B4;
			isCopy=true;
			break;
		case IMAGE_FORMAT_DXT1_ONEBITALPHA:
			format=RM_PIXELFORMAT_DXT1;
			isCopy=true;
			break;
		case IMAGE_FORMAT_BGRA5551:
			format=RM_PIXELFORMAT_A1R5G5B5;
			isCopy=true;
			break;
		case IMAGE_FORMAT_UV88:
			format=RM_PIXELFORMAT_V8U8;
			isCopy=true;
			break;
		case IMAGE_FORMAT_UVWQ8888:
			format=RM_PIXELFORMAT_Q8W8V8U8;
			isCopy=true;
			break;
		case IMAGE_FORMAT_RGBA16161616F:
			format=RM_PIXELFORMAT_A16B16G16R16F;
			isCopy=true;
			break;
		default:
			getRmApp()->OutputText( _T("\nThe VTF file format not support.\n") );
			return false;
	}
	pRm2DTexture->Create(format,pVtf->width,pVtf->height,1);
	unsigned char* pDest=pRm2DTexture->GetMipmap(0)->GetBuffer();
	if (isCopy)
	{
		memcpy( pDest, pSrc, memRequired );
	}
	else
	{
		if (pVtf->imageFormat==IMAGE_FORMAT_RGBA8888)
		{//RM_PIXELFORMAT_A8R8G8B8 BGRA
			int size=4*pVtf->width*pVtf->height;
			for ( int i = 0;i < size;i += 4 )
			{
				memcpy( pDest+i+0, pSrc+i+2, 1 );
				memcpy( pDest+i+1, pSrc+i+1, 1 );
				memcpy( pDest+i+2, pSrc+i+0, 1 );
				memcpy( pDest+i+3, pSrc+i+3, 1 );
			}
		}
		else if (pVtf->imageFormat==IMAGE_FORMAT_ABGR8888)
		{//RM_PIXELFORMAT_A8R8G8B8 BGRA
			int size=4*pVtf->width*pVtf->height;
			for ( int i = 0;i < size;i += 4 )
			{
				memcpy( pDest+i+0, pSrc+i+1, 1 );
				memcpy( pDest+i+1, pSrc+i+2, 1 );
				memcpy( pDest+i+2, pSrc+i+3, 1 );
				memcpy( pDest+i+3, pSrc+i+0, 1 );
			}
		}
		else if (pVtf->imageFormat==IMAGE_FORMAT_RGB888||pVtf->imageFormat==IMAGE_FORMAT_RGB888_BLUESCREEN)
		{//RM_PIXELFORMAT_X8R8G8B8 BGRX
			int size=4*pVtf->width*pVtf->height;
			int j = 0;
			for ( int i = 0;i < size;i+=4 )
			{
				memcpy( pDest+i+0, pSrc+j+2, 1 );
				memcpy( pDest+i+1, pSrc+j+1, 1 );
				memcpy( pDest+i+2, pSrc+j+0, 1 );
				memcpy( pDest+i+3, pSrc+j+0, 1 );
				j+=3;
			}
		}
		else if (pVtf->imageFormat==IMAGE_FORMAT_BGR888||pVtf->imageFormat==IMAGE_FORMAT_BGR888_BLUESCREEN)
		{//RM_PIXELFORMAT_X8R8G8B8 BGRX
			int size=4*pVtf->width*pVtf->height;
			int j = 0;
			for ( int i = 0;i < size;i+=4 )
			{
				memcpy( pDest+i+0, pSrc+j+0, 1 );
				memcpy( pDest+i+1, pSrc+j+1, 1 );
				memcpy( pDest+i+2, pSrc+j+2, 1 );
				memcpy( pDest+i+3, pSrc+j+0, 1 );
				j+=3;
			}
		}
		else if (pVtf->imageFormat==IMAGE_FORMAT_RGB565)
		{//RM_PIXELFORMAT_R5G6B5 BGR
			int size=2*pVtf->width*pVtf->height;
			unsigned short src,dest,R,G,B;
			for ( int i = 0;i < size;i+=2 )
			{
				src=*(unsigned short*)(pSrc+i);
				R = (src&0xF800)>>11;
				G = src&0x07E0;
				B = src&0x1F;
				dest=(B<<11)+G+R;
				memcpy( pDest+i, &dest, 2 );
			}
		}
		else if (pVtf->imageFormat==IMAGE_FORMAT_ARGB8888)
		{//RM_PIXELFORMAT_A8R8G8B8 BGRA
			int size=4*pVtf->width*pVtf->height;
			for ( int i = 0;i < size;i += 4 )
			{
				memcpy( pDest+i+0, pSrc+i+1, 1 );
				memcpy( pDest+i+1, pSrc+i+0, 1 );
				memcpy( pDest+i+2, pSrc+i+3, 1 );
				memcpy( pDest+i+3, pSrc+i+2, 1 );
			}
		}
		else if (pVtf->imageFormat==IMAGE_FORMAT_BGRX8888)
		{//RM_PIXELFORMAT_A8R8G8B8 BGRA
			int size=4*pVtf->width*pVtf->height;
			for ( int i = 0;i < size;i += 4 )
			{
				memcpy( pDest+i+0, pSrc+i+1, 1 );
				memcpy( pDest+i+1, pSrc+i+0, 1 );
				memcpy( pDest+i+2, pSrc+i+3, 1 );
				memcpy( pDest+i+3, pSrc+i+2, 1 );
			}
		}
	}
	delete pMem;
	inFile.close();
	return true;
} // End of LoadTexture for CVTFLoaderPlugIn