//=================================================================================================//
// filename: HL2LoaderPlugIn.h                                                          //
//                                                                                                 //
//           ATI Research, Inc.                                                                    //
//           3D Application Research Group                                                         //
//                                                                                                 //
// description: X Loader PlugIn.                                                                   //
//                                                                                                 //
//=================================================================================================//
//   (C) 2004 ATI Research, Inc.  All rights reserved.                                             //
//=================================================================================================//

#include "HL2LoaderPCH.h"
#include "HL2LoaderMain.h"
#include "HL2LoaderPlugIn.h"
#include "Shlwapi.h"

// Vertex declaration
D3DVERTEXELEMENT9 VERTEX_DECL[] =
{
    { 0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_BLENDWEIGHT,  0},
    { 0, 12, D3DDECLTYPE_UBYTE4, D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_BLENDINDICES, 0},
    { 0, 16, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_POSITION, 0},
    { 0, 28, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_NORMAL,   0}, 
    { 0, 40, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_TEXCOORD, 0},
    { 0, 48, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_TANGENT,  0},
	D3DDECL_END()
};

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//..............................................................................................................//
//..............................................................................................................//
//..............................................................................................................//
//
// Geometry loader plug-in implementation
//
//..............................................................................................................//
//..............................................................................................................//
//..............................................................................................................//


//-------------------------------------------------------------------------------------------------//
// Class:       CHL2LoaderPlugIn
// 
// Function:    Constructor
//
//-------------------------------------------------------------------------------------------------//

CHL2LoaderPlugIn::CHL2LoaderPlugIn() : IRmGeometryLoaderPlugIn()
{
   // This plug-in's type is a geometry plug-in, we must explicitly specify it:
   m_plugInDesc.plugInType = RM_PLUGINTYPE_GEOMETRY_LOADER;

   RmPlugInID plugInID = { 0x98b0b59c, 0x70eb, 0x4d1b, { 0xbf, 0xeb, 0xa7, 0xfc, 0x6, 0xbd, 0x58, 0x13 } };
   m_plugInDesc.plugInID   = plugInID;

   // There is only one type of node that this plug-in is associated with by the main application:
   m_plugInDesc.nNumSupportedNodeTypes = 1;
   m_plugInDesc.supportedNodeTypes[0] = RM_MODEL_DATA_TYPE; // Model data node association

   // This plug-in is implemented using 1.0 Wizard version:
   m_plugInDesc.nMajorSDKVersion = RENDERMONKEY_SDK_CURRENT_VERSION_MAJOR;
   m_plugInDesc.nMinorSDKVersion = RENDERMONKEY_SDK_CURRENT_VERSION_MINOR;

   // This plug-in in can be used in effects of all types and thus doesn't care about the
   // graphics API version:
   RmStrCopyT( m_plugInDesc.strRenderAPIVersion, RM_API_AGNOSTIC );

   // The name that the user will see for this plug-in
   RmStrCopyT( m_plugInDesc.strName, _T( "HL2Loader" ) );
   
} // End of Constructor for CHL2LoaderPlugIn

//-------------------------------------------------------------------------------------------------//
// Class:       CHL2LoaderPlugIn
// 
// Function:    ~CHL2LoaderPlugIn
// 
// Description: Destructor
//
//-------------------------------------------------------------------------------------------------//

CHL2LoaderPlugIn::~CHL2LoaderPlugIn()
{
} // End of Destructor for CHL2LoaderPlugIn


//-------------------------------------------------------------------------------------------------//
// Class:       CHL2LoaderPlugIn
// 
// Function:    Init
// 
// Description: Initializes this particular plug-in
//
//-------------------------------------------------------------------------------------------------//
bool CHL2LoaderPlugIn::Init()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   return true;
} // End of Init for CHL2LoaderPlugIn

//-------------------------------------------------------------------------------------------------//
// Class:       CHL2LoaderPlugIn
// 
// Function:    Uninitialize
// 
// Description: Performs uninitialization of the plug-in specific data
//
//-------------------------------------------------------------------------------------------------//

void CHL2LoaderPlugIn::Uninitialize()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
} // End of Uninitialize for CHL2LoaderPlugIn


//-------------------------------------------------------------------------------------------------//
// Class:       CHL2LoaderPlugIn
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
int CHL2LoaderPlugIn::MessageHandler(   int nMessageID,
                                                   int nMessageData,
                                                   int nMessageParameter /* = 0 */,
                                                   const RmPlugInID* pRmPlugInID /* = NULL */ )
{
   AFX_MANAGE_STATE( AfxGetStaticModuleState() );
   return RM_OK;
} // End of MessageHandler for CHL2LoaderPlugIn



//-------------------------------------------------------------------------------------------------//
// Class:       CHL2LoaderPlugIn
// 
// Function:    GetSupportedExtensions
// 
// Description: This method returns a list of file extensions that are supported by this geometry
//              loader plug-in for importing geometry from. These values are used to control the
//              display of files in the FileOpen dialog for model load. Note that number of entries
//              for 'formatDescriptions' and 'formatExtensions' must match.
//              
//              Eg. For X file, extension = "*.X"
//                              text      = "X Studio File (*.X)"
//              
//-------------------------------------------------------------------------------------------------//
void CHL2LoaderPlugIn::GetSupportedExtensions( RmLinkedList<RmStringT> &formatDescriptions,
                                               RmLinkedList<RmStringT> &formatExtensions )
{
   formatDescriptions.push_back(_T("HL2 File (*.mdl)"));
   formatExtensions.push_back(_T("*.mdl"));
} // End of GetSupportedExtensions for CHL2LoaderPlugIn


//-------------------------------------------------------------------------------------------------//
// Class:       CHL2LoaderPlugIn
// 
// Function:    CanLoadGeometry
// 
// Description: Method checks whether this plug-in can load geometry in the specified file
//              based on its own criteria. Returns true if it can load the file and false 
//              otherwise.
//
//-------------------------------------------------------------------------------------------------//
bool CHL2LoaderPlugIn::CanLoadGeometry( const RM_TCHAR *strFileName )
{
   //----------------------------------------------------------------------
   // We want to use RenderMonkey specific file open methods to ensure that 
   // we are always opening file using either unicode or ascii-specific 
   // methods using the application settings automatically:
   //
   //----------------------------------------------------------------------
   //FILE *pFile = NULL;
   //RmFileOpenT( &pFile, strFileName, _T("rb") );
   //if ( pFile == NULL )
   //{
   //   return false;
   //} // End if
	//RmFileClose( pFile );
	bool bResult = false;
	WCHAR tmpStringName[MAX_PATH];
	StringCchCopy( tmpStringName, MAX_PATH, strFileName );
	RM_TCHAR* pch = wcsrchr( tmpStringName, L'\.' );
	if( 0 == wcscmp( pch, L".mdl" ) )
		bResult = true;
	return bResult;
} // End of CanLoadGeometry for CHL2LoaderPlugIn


//-------------------------------------------------------------------------------------------------//
// Class:       CHL2LoaderPlugIn
// 
// Function:    LoadGeometry
// 
// Description: This method gets called by the main application to read in geometry data from
//              a particular file ('strFileName') into the specified model data container
//              ('pModelContainer')
//
//-------------------------------------------------------------------------------------------------//
bool CHL2LoaderPlugIn::LoadGeometry( const RM_TCHAR       *strFileName, 
                                                RmMeshModelContainer *pModelContainer )
{
   //----------------------------------------------------------------------
   // Actual loading code goes here
   //----------------------------------------------------------------------
    HRESULT hr;
	// Store the device pointer
	IDirect3DDevice9 *pd3dDevice = RmGfxGetD3DDevice();

	WCHAR vvdwstr[MAX_PATH];
	WCHAR vtxwstr[MAX_PATH];
	WCHAR mdlwstr[MAX_PATH];

	char vvdstr[MAX_PATH];
	char vtxstr[MAX_PATH];
	char mdlstr[MAX_PATH];

	int vvdFileSize;
	int vtxFileSize;
	int mdlFileSize;
	
	WCHAR tmpStringName[MAX_PATH];
	StringCchCopy( tmpStringName, MAX_PATH, strFileName );
	WCHAR* pch = wcsrchr( tmpStringName, L'\.' );
	*pch='\0';

	StringCchCopy( vvdwstr, MAX_PATH, tmpStringName );
	StringCchCopy( vtxwstr, MAX_PATH, tmpStringName );
	StringCchCopy( mdlwstr, MAX_PATH, tmpStringName );

	StringCchCat( vvdwstr, MAX_PATH, L".vvd" );
	StringCchCat( vtxwstr, MAX_PATH, L".dx90.vtx" );
	StringCchCat( mdlwstr, MAX_PATH, L".mdl" );
    
	WideCharToMultiByte( CP_ACP, 0, vvdwstr, -1, vvdstr, MAX_PATH, NULL, NULL );
	WideCharToMultiByte( CP_ACP, 0, vtxwstr, -1, vtxstr, MAX_PATH, NULL, NULL );
	WideCharToMultiByte( CP_ACP, 0, mdlwstr, -1, mdlstr, MAX_PATH, NULL, NULL );

	ifstream vvdFile( vvdstr,ios::binary );
	ifstream vtxFile( vtxstr,ios::binary );
	ifstream mdlFile( mdlstr,ios::binary );

	vvdFile.seekg(0,ios::end);
	vtxFile.seekg(0,ios::end);
	mdlFile.seekg(0,ios::end);

	vvdFileSize   =   vvdFile.tellg();
	vtxFileSize   =   vtxFile.tellg();
	mdlFileSize   =   mdlFile.tellg();

	vvdFile.seekg(0,ios::beg);
	vtxFile.seekg(0,ios::beg);
	mdlFile.seekg(0,ios::beg);

	char *m_pVvdMem = new char[vvdFileSize];
	char *m_pVtxMem = new char[vtxFileSize];
	char *m_pMdlMem = new char[mdlFileSize];
	vvdFile.read(m_pVvdMem,vvdFileSize);
	vtxFile.read(m_pVtxMem,vtxFileSize);
	mdlFile.read(m_pMdlMem,mdlFileSize);

	vertexFileHeader_t* m_pVvdFileHeader=(vertexFileHeader_t *)m_pVvdMem;
	FileHeader_t*	 m_pVtxFileHeader=(FileHeader_t *)m_pVtxMem;
	studiohdr_t*	 m_pMdlFileHeader=(studiohdr_t *)m_pMdlMem;
	unsigned short    m_iLod=0;
	if (m_pVvdFileHeader->numFixups)
	{
		vertexFileHeader_t *	pNewVvdHdr;
		pNewVvdHdr = new vertexFileHeader_t[vvdFileSize];
		Studio_LoadVertexes( m_pVvdFileHeader, pNewVvdHdr, 0, true );
		free( m_pVvdFileHeader );
		m_pVvdFileHeader = pNewVvdHdr;
	}
	// check mdl header
	if (m_pMdlFileHeader->id != IDSTUDIOHEADER)
	{
		getRmApp()->OutputText( _T(".mdl File id error") );
		return false;
	}
	if (m_pMdlFileHeader->version != STUDIO_VERSION)
	{
		getRmApp()->OutputText( _T(".mdl File version error") );
		return false;
	}

	// check vtx header
	if (m_pVtxFileHeader->version != OPTIMIZED_MODEL_FILE_VERSION)
	{
		getRmApp()->OutputText( _T(".vtd File version error") );
		return false;
	}
	if (m_pVtxFileHeader->checkSum != m_pMdlFileHeader->checksum)
	{
		getRmApp()->OutputText( _T(".vtd File checksum error") );
		return false;
	}

	// check vvd header
	if (m_pVvdFileHeader->id != MODEL_VERTEX_FILE_ID)
	{
		getRmApp()->OutputText( _T(".vvd File id error") );
		return false;
	}
	if (m_pVvdFileHeader->version != MODEL_VERTEX_FILE_VERSION)
	{
		getRmApp()->OutputText( _T(".vvd File version error") );
		return false;
	}
	if (m_pVvdFileHeader->checksum != m_pMdlFileHeader->checksum)
	{
		getRmApp()->OutputText( _T(".vvd File checksum error") );
		return false;
	}

	m_pMdlFileHeader->pVertexBase = (void *)m_pVvdFileHeader;
	m_pMdlFileHeader->pIndexBase  = (void *)m_pVtxFileHeader;

	BodyPartHeader_t* pBodyPart = m_pVtxFileHeader->pBodyPart(0);
	ModelHeader_t*  pModel=pBodyPart->pModel(0);
	ModelLODHeader_t* pLod = pModel->pLOD(m_iLod);

	mstudiobodyparts_t* pStudioBodyPart = m_pMdlFileHeader->pBodypart(0);
	mstudiomodel_t* pStudioModel= pStudioBodyPart->pModel(0);
	int numVerts=0;
	int numIndices=0;
	for (int k=0;k<pStudioModel->nummeshes;k++)
	{
		MeshHeader_t* pMesh = pLod->pMesh(k);
		mstudiomesh_t* pStudioMesh = pStudioModel->pMesh( k );
		for (int j=0;j<pMesh->numStripGroups;j++)
		{
			StripGroupHeader_t* pStripGroup = pMesh->pStripGroup(j);
			numVerts+=pStripGroup->numVerts;
			numIndices+=pStripGroup->numIndices;
		}
	}

	RmMeshModel *pRmMesh = new RmMeshModel();
	pModelContainer->AddMeshModel(pRmMesh);
	pRmMesh->GetVertexArray()->SetNumVertices( numVerts, false );
	pRmMesh->GetVertexArray()->CreateVertexElementArray( RM_DECLUSAGE_BLENDWEIGHT, 0, RMVSDT_FLOAT3 );
	pRmMesh->GetVertexArray()->CreateVertexElementArray( RM_DECLUSAGE_BLENDINDICES, 0, RMVSDT_UBYTE4 );
	pRmMesh->GetVertexArray()->CreateVertexElementArray( RM_DECLUSAGE_POSITION, 0, RMVSDT_FLOAT3 );
	pRmMesh->GetVertexArray()->CreateVertexElementArray( RM_DECLUSAGE_NORMAL, 0, RMVSDT_FLOAT3 );
	pRmMesh->GetVertexArray()->CreateVertexElementArray( RM_DECLUSAGE_TEXCOORD, 0, RMVSDT_FLOAT2 );
	pRmMesh->GetVertexArray()->CreateVertexElementArray( RM_DECLUSAGE_TANGENT, 0, RMVSDT_FLOAT4 );

	RmVertexElementArray *pBlendWeightElementArray  = pRmMesh->GetVertexArray()->GetVertexElementArray( RM_DECLUSAGE_BLENDWEIGHT, 0, RMVSDT_FLOAT3 );
	RmVertexElementArray *pBlendIndicesElementArray  = pRmMesh->GetVertexArray()->GetVertexElementArray( RM_DECLUSAGE_BLENDINDICES, 0, RMVSDT_UBYTE4 );
	RmVertexElementArray *pPositionElementArray  = pRmMesh->GetVertexArray()->GetVertexElementArray( RM_DECLUSAGE_POSITION, 0, RMVSDT_FLOAT3 );
	RmVertexElementArray *pNormalElementArray = pRmMesh->GetVertexArray()->GetVertexElementArray(RM_DECLUSAGE_NORMAL,0,RMVSDT_FLOAT3);
	RmVertexElementArray *pTexCoordElementArray = pRmMesh->GetVertexArray()->GetVertexElementArray(RM_DECLUSAGE_TEXCOORD,0,RMVSDT_FLOAT2);
	RmVertexElementArray *pTangentElementArray = pRmMesh->GetVertexArray()->GetVertexElementArray(RM_DECLUSAGE_TANGENT,0,RMVSDT_FLOAT4);

	unsigned short indexOffset=0;
	unsigned short iVertex=0;
	unsigned short iIndex=0;
	unsigned short iSubset = 0;	

	for (int k=0;k<pStudioModel->nummeshes;k++)
	{
		MeshHeader_t* pMesh = pLod->pMesh(k);
		mstudiomesh_t* pStudioMesh = pStudioModel->pMesh( k );
		for (int j=0;j<pMesh->numStripGroups;j++)
		{
			StripGroupHeader_t* pStripGroup = pMesh->pStripGroup(j);
			for (int i=0;i<pStripGroup->numVerts;i++)
			{
				float *pBoneWeight  = ( float* ) pBlendWeightElementArray->GetBuffer( iVertex );
				float *pBoneIndices = ( float* ) pBlendIndicesElementArray->GetBuffer( iVertex );
				float *pPosition    = ( float* ) pPositionElementArray->GetBuffer( iVertex );
				float *pNormal      = ( float* ) pNormalElementArray->GetBuffer( iVertex );
				float *pTexCoord    = ( float* ) pTexCoordElementArray->GetBuffer( iVertex );
				float *pTangent     = ( float* ) pTangentElementArray->GetBuffer( iVertex );

				float *pVertex = ( float* ) m_pVvdFileHeader->pVertex( pStudioMesh->vertexoffset+pStripGroup->pVertex(i)->origMeshVertID );
				pBoneWeight[0]=	pVertex[0];
				pBoneWeight[1]=	pVertex[1];
				pBoneWeight[2]=	pVertex[2];
				pBoneIndices[0]=pVertex[3];
				pPosition[0]  =	pVertex[4];
				pPosition[1]  =	pVertex[5];
				pPosition[2]  =	pVertex[6];
				pNormal[0]    =	pVertex[7];
				pNormal[1]    =	pVertex[8];
				pNormal[2]    =	pVertex[9];
				pTexCoord[0]  = pVertex[10];
				pTexCoord[1]  = pVertex[11];

				float *pTan = ( float* ) m_pVvdFileHeader->pTangent(pStripGroup->pVertex(i)->origMeshVertID );
				pTangent[0] = pTan[0];
				pTangent[1] = pTan[1];
				pTangent[2] = pTan[2];
				pTangent[3] = pTan[3];
				iVertex++;
			}
			pRmMesh->SetPrimitiveType( RM_PRIMITIVETYPE_TRIANGLELIST );
			pRmMesh->SetNumIndices( numIndices );
			RM_DWORD *pIndices = pRmMesh->GetIndices();
			for (int i=0;i<pStripGroup->numIndices;i+=3)
			{
				pIndices[iIndex]=indexOffset + *pStripGroup->pIndex(i);
				pIndices[iIndex+1]=indexOffset + *pStripGroup->pIndex(i+1);
				pIndices[iIndex+2]=indexOffset + *pStripGroup->pIndex(i+2);
				iIndex+=3;
			}
			indexOffset=iVertex;
		}
	}


	getRmApp()->OutputText( _T("\nLoad '%s' into RenderMonkey.\n"), vvdwstr );
	getRmApp()->OutputText( _T("Load '%s' into RenderMonkey.\n"), vtxwstr );
	getRmApp()->OutputText( _T("Load '%s' into RenderMonkey.\n"), mdlwstr );

	return true;
} // End of LoadGeometry for CHL2LoaderPlugIn



