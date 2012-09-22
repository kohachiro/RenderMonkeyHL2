//=================================================================================================//
// filename: VTFLoaderMain.h                                                            //
//                                                                                                 //
//           ATI Research, Inc.                                                                    //
//           3D Application Research Group                                                         //
//                                                                                                 //
// description: Declaration file for RenderMonkey's plug-in DLL entry points and the geometry      //
//              plug-in itself.                                                                    //
//                                                                                                 //
//=================================================================================================//
//   (C) 2004 ATI Research, Inc.  All rights reserved.                                             //
//=================================================================================================//

#ifndef _VTFLoader_Main_H_
#define _VTFLoader_Main_H_

//=================================================================================================//
// Forward Declaration
//=================================================================================================//
class CVTFLoader;
class CVTFLoaderPlugIn;

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

class CVTFLoaderKernel
{
public :
   CVTFLoaderKernel();
   virtual ~CVTFLoaderKernel();

   // Initializes the kernel instance:
   void Init();

   // Perform any uninitialization code for the kernel (and thus the plug-ins that it
   // manages). Note that this method gets called after the plug-ins were supposed
   // to be freed by the application. 
   void Uninitialize();

   // Retrieve the DLL instance for this DLL:
   HINSTANCE GetPlugInInstance() const       {        return m_hInstance;     };

   // Retrieve the number of plug-ins that this kernel manages (1 in our case, since it's a single plug-in):
   int GetNumPlugIns()           const       {        return 1;               };

   // Retrieve a plug-in specified by the index and store it in the interface pointer provided.
   // This method is used by the main application to access individual plug-ins from each DLL:
   void GetPlugIn( int nIndex, IRmPlugIn **ppPlugIn );

   // This method gets called to perform any plug-in specific uninitialization code and then
   // delete the actual plug-in from memory:
   void FreePlugIn( IRmPlugIn *pPlugIn );

private :

   HINSTANCE         m_hInstance;

   // In this particular example there is only a single plug-in to manage. 
   // However, it is easy to extend the approach to manage multiple plug-ins
   // by the same kernel. 
   CVTFLoaderPlugIn *m_pLoaderPlugIn;       

}; // End of CVTFLoaderKernel class declaration

//===========================================//
// Singleton accessor for the plug-in kernel //
//===========================================//
CVTFLoaderKernel* GetLoaderKernel();

#endif // _VTFLoader_Main_H_
