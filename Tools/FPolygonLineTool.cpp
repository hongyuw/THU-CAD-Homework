#include "../external/FKernel/include/FKernelUtilities.h"
#include "../FUtilities.h"
#include "../MainDoc.h"
#include "../FPolygonLine.h"
#include "FPolygonLineTool.h"

extern FMainWindow* gWindow;

void FPolygonLineTool::Init()
{
	m_pPolyline = NULL;
}
FPolygonLineTool::FPolygonLineTool()
	:FFeatureTool()
{
	Init();
}
FPolygonLineTool::FPolygonLineTool(FMainDoc* pDoc, FEntity* pEnt, int sx, int sy)
	:FFeatureTool(pDoc, sx, sy)
{
	Init();
	m_pPolyline = (FPolygonLine*)pEnt;
}
FPolygonLineTool::~FPolygonLineTool()
{

}

void FPolygonLineTool::CreateToolUI()
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

int FPolygonLineTool::ProcessEvent(FEventType eventType, const FPickEvent& pe)
{
	if (!m_pPolyline)
	{
		m_pPolyline = new FPolygonLine;
		m_pDoc->AddDispEnt(m_pPolyline);
	}
	if (!m_pDoc)	return 0;
	if (eventType==MOUSE_BUTTON_DOWN && pe.nFlags==MK_LBUTTON && m_iStep==0)	//left button down, start of mouse drag
	{   
		//if (!m_pPolyline)
		//{
		//	m_pPolyline = new FPolygonLine;
		//	m_pDoc->AddDispEnt(m_pPolyline);
		//}
		m_iStep = 1;
		m_pPolyline->finished = 0;

		gSetHintMessage(QObject::tr("Test1_HW_Lmouse"));
		F3dPoint tPnt;
		pe.view->Screen2WCS(pe.x, pe.y, tPnt);
		

		m_pPolyline->m_pnts.push_back(tPnt);
		/******/
	//	
	}
	else if (eventType==MOUSE_MOVE && pe.nFlags==MK_LBUTTON && m_iStep==1)	//left button drag, mouse dragging
	{	
		//F3dPoint tPnt, tCV;
		//pe.view->Screen2WCS(pe.x, pe.y, tPnt);

		//if(!m_pPolyline->m_pnts.isEmpty()){		
		//	m_pPolyline->m_pnts.pop_back();
		//}
	 //   m_pPolyline->m_pnts.push_back(tPnt);
	}
	else if (eventType==MOUSE_BUTTON_UP && pe.nFlags==MK_LBUTTON && m_iStep==1)	//left button up, end of mouse drag
	{
		m_iStep=0;
	}
	else if (eventType==MOUSE_BUTTON_UP && pe.nFlags==MK_RBUTTON)	//exit edit mode
	{
		m_iStep = 0;
		m_pPolyline->finished = 1;
	}
	return 1;
}
