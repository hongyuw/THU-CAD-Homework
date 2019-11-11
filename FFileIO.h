/*	
\brief	Import various files.
\author: Jacky
\date    2014-04
*/
#ifndef F_FILEIOHW_H
#define F_FILEIOHW_H

#include "../external/FKernel/include/FKernelDefines.h"
#include "../external/FKernel/include/FKernelHeader.h"

namespace FT{
	class FBody;

	class FFileIO
	{
	public:
		FFileIO();
		~FFileIO();
		/////////////////////////////// Public methods ///////////////////////////////////////////
		void AddEnts(FEntity* pEnt){m_ents.append(pEnt);}

		/////////////////////////////// Static methods ///////////////////////////////////////////
		static FMesh*	ImportOBJ(char* fname);	//full path file name, with .obj extension
		//	   bool		ExportOBJ(char* fname);	//export m_ents to obj file, with full path and .obj extension

		//	   bool		ExportSTL(char* fname);
		//static bool		ExportFT(FDoc* pDoc, char* fname);	//export display entity to a feature file, e.g. test.ft
		//static FMesh*	ImportSTL(char* fname);
		//static void		ExportSecurity(char* fname, bool bExpired=false);
		//static QStringList ImportSecurity(char* fname);
	protected:
		//static FMesh* ImportSTL_TXT(char* fname);
		//static FMesh* ImportSTL_BIN(char* fname);
		//       bool		ExportProduct2OBJ(char* fname);	///export a product to a single OBJ file
				/*!
				\brief	export a single 3D feature to obj file.
				\param	[i/o]pFile	file pointer
				\param	[i]p3D		exported 3D feature
				\param	[i]index	entity number exported yet
				\param	[i/o]offset	vertex offset
				\return	true if exported successfully. */	
			   //bool		ExportFeature2OBJ(FILE* pFile, F3DFeature* p3D, int& offset, int index);	//export m_ents to obj file, with full path and .obj extension		

	protected:
		PFEntityList		m_ents;	//list of entities for file IO
	};
}
#endif