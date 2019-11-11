#pragma once
#include "C:\program3\iDesignCAD-STU\External\fkernel\include\ffeaturetool.h"\

#include "../SketchSketch.h"
#include "../SketchStroke.h"

class FPlane;
class FMainDoc;
class FBSpline;

using namespace FT;
using namespace std;

class FCutTool :
	public FFeatureTool
{
public:
	FCutTool();
	FCutTool(FMainDoc* pDoc, FEntity* pEnt, int sx=0, int sy=0);
	~FCutTool();


	void	Init();
	virtual int		ProcessEvent(FEventType eventType, const FPickEvent& pe);
	virtual void	CreateToolUI();
	void    Cutopreation();
protected:
	FMainDoc*			m_pDoc;
	int					m_iNum;			///operation steps flag used in event handle func.
	int					m_iKeyState;	///key status, 0 default, no key is pressed;1 when Ctrl is pressed;
	SketchStroke*		m_pStroke;	
private:
	SketchSketch*		m_pSketch;
	FT::FPlane      *       m_pPlane;
};

