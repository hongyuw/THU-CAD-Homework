/*	
\brief	Edit a freeform curve, which usually are imported from library.
\author: Jacky
\date    2014-07-07
*/
#ifndef FT_BEZIER_TOOL_H
#define FT_BEZIER_TOOL_H

#include "../external/FKernel/include/FKernelDefines.h"
#include "../external/FKernel/include/FFeatureTool.h"

class FMainDoc;
class FBezier;
using namespace FT;

class FBezierTool:public FFeatureTool
{
	Q_OBJECT
public:
	FBezierTool();
	FBezierTool(FMainDoc* pDoc, FEntity* pEnt, int sx=0, int sy=0);
	~FBezierTool();

	void	Init();
	virtual int		ProcessEvent(FEventType eventType, const FPickEvent& pe);
	virtual void	CreateToolUI();
protected:
	FBezier*  m_pBezier;			///
	/*!
	\brief	record key's information
	\li 0 --- no key is active
	\li 1 --- "Ctrl" key is active
	\li 2 --- "Shift" key is active 	*/	
	uint			m_iKeyState;
};
#endif
