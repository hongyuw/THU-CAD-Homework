/*	
\brief	Edit a freeform curve, which usually are imported from library.
\author: Jacky
\date    2014-07-07
*/
#ifndef FT_BSPLINE_TOOL_H
#define FT_BSPLINE_TOOL_H

#include "../external/FKernel/include/FKernelDefines.h"
#include "../external/FKernel/include/FFeatureTool.h"

class FMainDoc;
class FBSpline;
using namespace FT;

class FBSplineTool:public FFeatureTool
{
	//Q_OBJECT
public:
	FBSplineTool();
	FBSplineTool(FMainDoc* pDoc, FEntity* pEnt, int sx=0, int sy=0);
	~FBSplineTool();

	void	Init();
	virtual int		ProcessEvent(FEventType eventType, const FPickEvent& pe);
	virtual void	CreateToolUI();
protected:
	FBSpline*  m_pPolyline;			///
	/*!
	\brief	record key's information
	\li 0 --- no key is active
	\li 1 --- "Ctrl" key is active
	\li 2 --- "Shift" key is active 	*/	
	uint			m_iKeyState;
};
#endif