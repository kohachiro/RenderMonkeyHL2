//=================================================================================================//
// filename: VTFLoaderMain.h                                                            //
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

#include "VTFLoaderPCH.h"
#include "VTFLoaderPlugIn.h"
#include "VTFLoaderMain.h"


//=================================================================================================//
// Accessor for the Kernel
//=================================================================================================//
CVTFLoaderKernel* GetLoaderKernel()
{
   static CVTFLoaderKernel myPlugInKernel;
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
CVTFLoaderKernel::CVTFLoaderKernel() :
   m_hInstance(NULL),
   m_pLoaderPlugIn(NULL)
{
} // End of Constructor for CVTFLoaderKernel


//---------------------------------------------------------
CVTFLoaderKernel::~CVTFLoaderKernel()
{
} // End of Destructor for CVTFLoaderKernel


//---------------------------------------------------------
// Init/Uninitialize
//---------------------------------------------------------
void CVTFLoaderKernel::Init()
{
   m_pLoaderPlugIn = new CVTFLoaderPlugIn();
} // End of Init for CVTFLoaderKernel


//---------------------------------------------------------
void CVTFLoaderKernel::Uninitialize()
{
   m_hInstance = NULL;

   assert(m_pLoaderPlugIn==NULL); // PlugIn must have been freed already
} // End of Uninitialize for CVTFLoaderKernel


//---------------------------------------------------------
// PlugIn
//---------------------------------------------------------
void CVTFLoaderKernel::GetPlugIn( int nIndex, IRmPlugIn **ppPlugIn )
{
   assert(nIndex==0);
   assert(m_pLoaderPlugIn!=NULL);

   *ppPlugIn = m_pLoaderPlugIn;
} // End of GetPlugIn for CVTFLoaderKernel


//---------------------------------------------------------
void CVTFLoaderKernel::FreePlugIn( IRmPlugIn *pPlugIn )
{
   assert(m_pLoaderPlugIn==pPlugIn);
   assert(m_pLoaderPlugIn!=NULL);

   m_pLoaderPlugIn->Uninitialize();
   delete m_pLoaderPlugIn;
   m_pLoaderPlugIn = NULL;
} // End of FreePlugIn for CVTFLoaderKernel


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
