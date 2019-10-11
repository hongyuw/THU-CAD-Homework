#include "../external/FKernel/include/FKernelUtilities.h"
#include "../FUtilities.h"
#include "../MainDoc.h"
#include "../FBezier.h"
#include "FBezierTool.h"

extern FMainWindow* gWindow;

void FBezierTool::Init()
{
	m_pBezier = NULL;
}
FBezierTool::FBezierTool()
	:FFeatureTool()
{
	Init();
}
FBezierTool::FBezierTool(FMainDoc* pDoc, FEntity* pEnt, int sx, int sy)
	:FFeatureTool(pDoc, sx, sy)
{
	Init();
	m_pBezier = (FBezier*)pEnt;
}
FBezierTool::~FBezierTool()
{

}

void FBezierTool::CreateToolUI()
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

int FBezierTool::ProcessEvent(FEventType eventType, const FPickEvent& pe)
{
	if (!m_pBezier)
	{
		m_pBezier = new FBezier;
		m_pDoc->AddDispEnt(m_pBezier);
	}
	if (!m_pDoc)	return 0;
	if (eventType==MOUSE_BUTTON_DOWN && pe.nFlags==MK_LBUTTON && m_iStep==0)	//left button down, start of mouse drag
	{   
		//if (!m_pBezier)
		//{
		//	m_pBezier = new FBezier;
		//	m_pDoc->AddDispEnt(m_pBezier);
		//}
		m_iStep = 1;
		gSetHintMessage(QObject::tr("Test1_HW_Lmouse"));
		F3dPoint tPnt;
		pe.view->Screen2WCS(pe.x, pe.y, tPnt);


		m_pBezier->m_pnts.push_back(tPnt);
		/******/
	//	
	}
	else if (eventType==MOUSE_MOVE && pe.nFlags==MK_LBUTTON && m_iStep==1)	//left button drag, mouse dragging
	{
		gSetHintMessage(QObject::tr("Test1_HW_LmouseDrag"));		
		//F3dPoint tPnt, tCV;
		//pe.view->Screen2WCS(pe.x, pe.y, tPnt);

		//if(!m_pBezier->m_pnts.isEmpty()){		
		//	m_pBezier->m_pnts.pop_back();
		//}
	 //   m_pBezier->m_pnts.push_back(tPnt);
	}
	else if (eventType==MOUSE_BUTTON_UP && pe.nFlags==MK_LBUTTON && m_iStep==1)	//left button up, end of mouse drag
	{
		m_iStep=0;
	}
	else if (eventType==MOUSE_BUTTON_UP && pe.nFlags==MK_RBUTTON)	//exit edit mode
	{
		m_iStep = 0;
	}
	return 1;
}
