#include "../external/FKernel/include/FKernelUtilities.h"
#include "../FUtilities.h"
#include "../MainDoc.h"
#include "../SketchSketch.h"
#include "../SketchStroke.h"
#include "Sketch2DTool.h"


extern FMainWindow* gWindow;

void Sketch2DTool::Init()
{
	m_pSketch = NULL;
}
Sketch2DTool::Sketch2DTool()
	:FFeatureTool()
{
	Init();
}
Sketch2DTool::Sketch2DTool(FMainDoc* pDoc, FEntity* pEnt, int sx, int sy)
	:FFeatureTool(pDoc, sx, sy)
{
	Init();
	m_pSketch = (SketchSketch*)pEnt;
}
Sketch2DTool::~Sketch2DTool()
{

}

void Sketch2DTool::CreateToolUI()
{
	//	if(!m_pCurve)	return;
	//m_pUI = new QDialog((QWidget*)gWindow);
	//m_pUI->setWindowTitle("Edit");
	//m_pUI->setAutoFillBackground(true);
	//m_pUI->setWindowModality(Qt::NonModal);
	//m_pUI->move(m_sx, m_sy);
	//m_pUI->setFixedWidth(200);
	//m_pUI->setFixedHeight(200);

	//	if(m_pCurve)
	//		m_pCurve->Curve()->SetShowUserPoint(true);

	//m_pUI->show();
}

int Sketch2DTool::ProcessEvent(FEventType eventType, const FPickEvent& pe)
{
	if (!m_pSketch)
	{
		m_pSketch = new SketchSketch;
		m_pDoc->AddDispEnt(m_pSketch);
	}
	if (!m_pDoc)	return 0;
	if (eventType==MOUSE_BUTTON_DOWN && pe.nFlags==MK_LBUTTON && m_iStep==0)	//left button down, start of mouse drag
	{   
		m_iStep = 1;
		F3dPoint tPnt;
		SketchStroke* stroke = new SketchStroke;
		pe.view->Screen2WCS(pe.x, pe.y, tPnt);
		stroke->m_pnts.push_back(tPnt);
		m_pSketch->m_strokes.push_back(stroke);

	}
	else if (eventType==MOUSE_MOVE && pe.nFlags==MK_LBUTTON && m_iStep==1)	//left button drag, mouse dragging
	{
		F3dPoint tPnt, tCV;
		pe.view->Screen2WCS(pe.x, pe.y, tPnt);
		m_pSketch->m_strokes.back()->m_pnts.push_back(tPnt);

	}
	else if (eventType==MOUSE_BUTTON_UP && pe.nFlags==MK_LBUTTON && m_iStep==1)	//left button up, end of mouse drag
	{	
		
		//m_pSketch->m_pnts.clear();
		m_iStep=0;
	}
	else if (eventType==MOUSE_BUTTON_UP && pe.nFlags==MK_RBUTTON)	//exit edit mode
	{
		m_pSketch->Detect();
		m_iStep = 0;
	}
	return 1;
}