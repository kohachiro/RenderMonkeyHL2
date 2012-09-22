//=================================================================================================//
// filename: HL2LoaderMain.h                                                            //
//                                                                                                 //
//           ATI Research, Inc.                                                                    //
//           3D Application Research Group                                                         //
//                                                                                                 //
// description: Implementation file for RenderMonkey's plug-in DLL entry points and the geometry   //
//           plug-in itself.                                                                       //
//                                                                                                 //
//=================================================================================================//
//   (C) 2004 ATI Research, Inc.  All rights reserved.                                             //
//=================================================================================================//

#include "HL2LoaderPCH.h"
#include "HL2LoaderPlugIn.h"
#include "HL2LoaderMain.h"


//=================================================================================================//
// Accessor for the Kernel
//=================================================================================================//
CHL2LoaderKernel* GetLoaderKernel()
{
   static CHL2LoaderKernel myPlugInKernel;
   return &myPlugInKernel;
} // End of GetLoaderKernel


//..............................................................................................................//
//..............................................................................................................//
//..............................................................................................................//
//
// X Loader's plug-in management kernel. This class can be used as an illustration for managing multiple      
// plug-ins in single DLL.                                                                                        
//
//..............................................................................................................//
//..............................................................................................................//
//..............................................................................................................//

//---------------------------------------------------------
// Constructor/Destructor
//---------------------------------------------------------
CHL2LoaderKernel::CHL2LoaderKernel() :
   m_hInstance(NULL),
   m_pLoaderPlugIn(NULL)
{
} // End of Constructor for CHL2LoaderKernel


//---------------------------------------------------------
CHL2LoaderKernel::~CHL2LoaderKernel()
{
} // End of Destructor for CHL2LoaderKernel


//---------------------------------------------------------
// Init/Uninitialize
//---------------------------------------------------------
void CHL2LoaderKernel::Init()
{
   m_pLoaderPlugIn = new CHL2LoaderPlugIn();
} // End of Init for CHL2LoaderKernel


//---------------------------------------------------------
void CHL2LoaderKernel::Uninitialize()
{
   m_hInstance = NULL;

   assert(m_pLoaderPlugIn==NULL); // PlugIn must have been freed already
} // End of Uninitialize for CHL2LoaderKernel


//---------------------------------------------------------
// PlugIn
//---------------------------------------------------------
void CHL2LoaderKernel::GetPlugIn( int nIndex, IRmPlugIn **ppPlugIn )
{
   assert(nIndex==0);
   assert(m_pLoaderPlugIn!=NULL);

   *ppPlugIn = m_pLoaderPlugIn;
} // End of GetPlugIn for CHL2LoaderKernel


//---------------------------------------------------------
void CHL2LoaderKernel::FreePlugIn( IRmPlugIn *pPlugIn )
{
   assert(m_pLoaderPlugIn==pPlugIn);
   assert(m_pLoaderPlugIn!=NULL);

   m_pLoaderPlugIn->Uninitialize();
   delete m_pLoaderPlugIn;
   m_pLoaderPlugIn = NULL;
} // End of FreePlugIn for CHL2LoaderKernel


//=================================================================================================//
// Entry point for New PlugIn Architecture
//=================================================================================================//
bool RmInitPlugInDLL()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

   GetLoaderKernel()->Init();

   return true;
} // End of RmInitPlugInDLL

//-------------------------------------------------------------------------------------------------//
int  RmGetNumPlugIns()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

   int num = GetLoaderKernel()->GetNumPlugIns();
   return num;
} // End of RmGetNumPlugIns


//-------------------------------------------------------------------------------------------------//
void RmGetPlugIn( int nIndex, IRmPlugIn **ppPlugIn )
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

   GetLoaderKernel()->GetPlugIn(nIndex,ppPlugIn);
} // End of RmGetPlugIn


//-------------------------------------------------------------------------------------------------//
void RmFreePlugIn( IRmPlugIn *pPlugIn )
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

   GetLoaderKernel()->FreePlugIn(pPlugIn);
} // End of RmFreePlugIn


//-------------------------------------------------------------------------------------------------//
void RmUninitializePlugInDLL()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

   GetLoaderKernel()->Uninitialize();
} // End of RmUninitializePlugInDLL
