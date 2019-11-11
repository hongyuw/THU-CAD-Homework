#ifndef SKETCH2DTOOL_H
#define SKETCH2DTOOL_H

#include "../external/FKernel/include/FKernelDefines.h"
#include "../external/FKernel/include/FFeatureTool.h"

class FMainDoc;
class SketchSketch;
using namespace FT;

class Sketch2DTool:public FFeatureTool
{
	Q_OBJECT
public:
	Sketch2DTool();
	Sketch2DTool(FMainDoc* pDoc, FEntity* pEnt, int sx=0, int sy=0);
	~Sketch2DTool();

	void	Init();
	virtual int		ProcessEvent(FEventType eventType, const FPickEvent& pe);
	virtual void	CreateToolUI();
protected:
	SketchSketch*  m_pSketch;			///
	/*!
	\brief	record key's information
	\li 0 --- no key is active
	\li 1 --- "Ctrl" key is active
	\li 2 --- "Shift" key is active 	*/	
	uint			m_iKeyState;
};
#endif
