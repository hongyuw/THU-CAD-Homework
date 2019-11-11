//#include "../GLKLib/stdafx.h"
#include "GeneratedFiles/ui_mainwindow.h"
#include "../external/FKernel/include/FKernelUtilities.h"
#include "../external/FKernel/include/FView.h"
#include "../external/FKernel/include/FileIO.h"
#include "../external/FKernel/include/DataStructure/FBody.h"
#include "FUtilities.h"
#include "MainDoc.h"
#include "MainView.h"
#include "FSubWindow.h"
#include "MainWindow.h"
#include "FFileDialog.h"
#include "FPolygonLine.h"
#include "FBSpline.h"
#include "FBezier.h"
#include "FBSurface.h"
#include "SketchSketch.h"
#include "FFileIO.h"
#include "FTest.h"
#include "../iDesignCAD/Tools/FPolygonLineTool.h"
#include "../iDesignCAD/Tools/Sketch2DTool.h"
#include "../iDesignCAD/Tools/FBezierTool.h"
#include "../iDesignCAD/Tools/FBSplineTool.h"
#include "../iDesignCAD/Tools/FCutTool.h"
#include <time.h>

FT::FBody*			gBody;			//reconstructed triangular mesh from gLPTdata, for downstream analysis

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#endif
#define new DBG_NEW
#endif  // _DEBUG

FMainWindow::FMainWindow(QWidget* parent)
	:QMainWindow(parent)
{
	//in PrinterCAD project, one view is enough
	m_pTopWnd = m_pRightWnd = m_p3DWnd = NULL;

	//1. Create the main window UI
	m_pUI = new Ui::UiMainWindow;
	m_pUI->setupUi(this);
	this->setAcceptDrops(true);	//allow to drop a file to open
	m_pMdiArea = new QMdiArea;
	setCentralWidget(m_pMdiArea);
	QApplication::setActiveWindow(this);	
	CreateStatusBar();

	//2. Create DOC instance
	m_pDoc = new FMainDoc(this);
	FT::gPDoc = m_pDoc;
	//front sub-window
	m_pFrontWnd = new FSubWindow(this, m_pDoc);
	m_pFrontWnd->setWindowTitle("Front(X/Z)");
	m_pMdiArea->addSubWindow(m_pFrontWnd);
	m_pFrontWnd->setVisible(true);
//	m_pFrontWnd->showMaximized();
	m_pFrontWnd->GetView()->GetKernelView()->SetViewType(FViewType::VIEW_FRONT);
	
	//3. Create the single-slots in main window
	CreateSignalSlots();

	//4. UI initialization
	//m_pParserWnd = NULL;
	//m_pParserWnd = new FParserDkWidget(QString("Parser Window"), this);
	//m_pParserWnd->setFloating(true);
//	m_pParserWnd->show();

	//AllocConsole(); //open the console window
	//freopen("CONOUT$", "w", stdout); //set output available to this console
	//freopen("CONIN$", "r", stdin); //set input avaiable to this console
}

FMainWindow::~FMainWindow()
{

}

void FMainWindow::CreateStatusBar()
{
	//status bar
	m_hintLabel=new QLabel("",this);
	m_hintLabel->setAlignment(Qt::AlignLeft);
	m_hintLabel->setMinimumSize(m_hintLabel->sizeHint());
	this->statusBar()->addPermanentWidget(m_hintLabel,1);
}

void FMainWindow::CreateSignalSlots()
{
	//menu File
	connect(m_pUI->action_new, SIGNAL(triggered()), this, SLOT(onFileNew()));
	connect(m_pUI->action_open, SIGNAL(triggered()), this, SLOT(onFileOpen()));
	connect(m_pUI->action_open, SIGNAL(triggered()), this, SLOT(onFileSave()));
	connect(m_pUI->action_quit, SIGNAL(triggered()), this, SLOT(onFileClose()));

	//menu View
	connect(m_pUI->action_topview, SIGNAL(triggered()), this, SLOT(onViewTop()));
	connect(m_pUI->action_bottomview, SIGNAL(triggered()), this, SLOT(onViewBottom()));
	connect(m_pUI->action_frontview, SIGNAL(triggered()), this, SLOT(onViewFront()));
	connect(m_pUI->action_backview, SIGNAL(triggered()), this, SLOT(onViewBack()));
	connect(m_pUI->action_leftview, SIGNAL(triggered()), this, SLOT(onViewLeft()));
	connect(m_pUI->action_rightview, SIGNAL(triggered()), this, SLOT(onViewRight()));
	connect(m_pUI->action_zoomin, SIGNAL(triggered()), this, SLOT(onViewZoomIn()));
	connect(m_pUI->action_zoomout, SIGNAL(triggered()), this, SLOT(onViewZoomOut()));
	connect(m_pUI->action_zoomall, SIGNAL(triggered()), this, SLOT(onViewZoomAll()));
	connect(m_pUI->action_orbit, SIGNAL(triggered()), this, SLOT(onViewOrbit()));
	connect(m_pUI->action_pan, SIGNAL(triggered()), this, SLOT(onViewPan()));
	connect(m_pUI->action_smoothdisp, SIGNAL(triggered()), this, SLOT(onViewSmooth()));
	connect(m_pUI->action_meshdisp, SIGNAL(triggered()), this, SLOT(onViewMesh()));

	//menu Test
	connect(m_pUI->actionTestTest, SIGNAL(triggered()), this, SLOT(onTestTest()));
	connect(m_pUI->actionSketch2D_HW, SIGNAL(triggered()), this, SLOT(onSketch2D_HW()));
	connect(m_pUI->actionTest1_HW, SIGNAL(triggered()), this, SLOT(onTest1_HW()));

	//menu Homework
	connect(m_pUI->actionHelloworld, SIGNAL(triggered()), this, SLOT(onHelloworld()));
	connect(m_pUI->actionBezier, SIGNAL(triggered()), this, SLOT(onBezier()));
	connect(m_pUI->actionBSpline, SIGNAL(triggered()), this, SLOT(onBSpline()));
	connect(m_pUI->actionBSplineSurface, SIGNAL(triggered()), this, SLOT(onBSplineSurface()));
	connect(m_pUI->actionreadObj, SIGNAL(triggered()), this, SLOT(onreadObj()));
	
}

void FMainWindow::dragEnterEvent(QDragEnterEvent* e)
{
	QString jcdMimeType = "application/x-qt-windows-mime;value=\"FileNameW\"";
	if ( e->mimeData()->hasFormat(jcdMimeType) )
	{
		e->acceptProposedAction();
	}
	QMainWindow::dragEnterEvent(e);
}

void FMainWindow::dropEvent(QDropEvent* e)
{
	QString jcdMimeType = "application/x-qt-windows-mime;value=\"FileNameW\"";
	QByteArray filenameBytes;
	QString filename;

	if (e->mimeData()->hasFormat(jcdMimeType))
	{
		filenameBytes=e->mimeData()->data(jcdMimeType);
		filename=QTextCodec::codecForName("UTF-16")->toUnicode(filenameBytes);
		OpenFile(filename);
	}
	QMainWindow::dropEvent(e);
}

FSubWindow* FMainWindow::GetCurrentWnd()
{
	if (m_pFrontWnd && m_pFrontWnd->isActiveWindow())
		return m_pFrontWnd;
	if (m_pTopWnd && m_pTopWnd->isActiveWindow())
		return m_pTopWnd;
	if (m_pRightWnd && m_pRightWnd->isActiveWindow())
		return m_pRightWnd;
	if (m_p3DWnd && m_p3DWnd->isActiveWindow())
		return m_p3DWnd;

	return 0;
}

bool FMainWindow::OpenFile(QString fname)
{
	FT::FFreeBody* pFBody(NULL);
	QString fExt = FT::gGetExtFromName(fname).toUpper();
	if (fExt=="OBJ")
		pFBody = gImportOBJFile(fname.toLocal8Bit().data());
	else if (fExt=="STL")
		pFBody = gImportSTLFile(fname.toLocal8Bit().data());
	else 
	{
		QMessageBox::warning(
			QApplication::activeWindow(),QObject::tr("Caution"),QObject::tr(" File extension is not supported in current version!"));
	}
	if(!pFBody)	return false;
	m_pDoc->AddDispEnt((FEntity*)pFBody);	
	return true;
}

bool FMainWindow::onFileNew()
{
	if (!m_pDoc)
		return false;
	m_pDoc->OnNewDocument();
	m_pDoc->UpdateAllViews();
	return true;
}

bool FMainWindow::onFileClose()
{
	if (m_pMdiArea)
	{
		m_pMdiArea->closeAllSubWindows();
	}
	return true;
}

bool FMainWindow::onFileOpen()
{
	if (!m_pDoc)	return false;
	FFileDialog fileDlg(this,tr("Open File"),"Mesh/Point file(*.obj *.stl *.out *.lpt);;All files (*.*)",false);
	QString filename = fileDlg.GetFileNameAsOpen();

	if (!filename.isEmpty())
	{
		if (OpenFile(filename))
		{
			m_pDoc->UpdateAllViews();
			m_pFrontWnd->GetView()->GetKernelView()->ZoomAll();
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

bool FMainWindow::onFileSave()
{
	return true;
}

void FMainWindow::onSketch2D_HW()
{
	//gSetHintMessage(QObject::tr("Sketch2D_HW"));
	SketchSketch* pSketch(NULL);
	pSketch = new SketchSketch;

	m_pDoc->AddDispEnt(pSketch);
	m_pDoc->PushTool(new Sketch2DTool(m_pDoc, pSketch));
}

void FMainWindow::onTest1_HW()
{
	//gSetHintMessage(QObject::tr("Test1_HW"));
	FTest test;
	test.CreateFreeCurve(m_pDoc);
//	m_pDoc->PushTool(new FCutTool(m_pDoc,));
	//m_pDoc->PushTool(new FPolygonLineTool(m_pDoc, pPolygon));
	//FPolygonLine* pPolygon(NULL);
	//pPolygon = new FPolygonLine;
	//
	//m_pDoc->AddDispEnt(pPolygon);
	//m_pDoc->PushTool(new FPolygonLineTool(m_pDoc, pPolygon));

	//FBSpline* pPolygon(NULL);
	//pPolygon = new FBSpline;

	//m_pDoc->AddDispEnt(pPolygon);
	//m_pDoc->PushTool(new FBSplineTool(m_pDoc, pPolygon));
	//FPolygonLineTool* pTool = new FPolygonLineTool(m_pDoc, 0);
	//m_pDoc->PopTool();
	//   m_pDoc->PushTool(pTool);
}

//Homework
void FMainWindow::onHelloworld()
{
	QMessageBox::warning(QApplication::activeWindow(),QObject::tr("Caution"),
	QObject::tr("Hello CAD!"), QMessageBox::Ok | QMessageBox::Cancel | QMessageBox::Abort);
	//FPolygonLine* pPolygon(NULL);
	//pPolygon = new FPolygonLine;
	//m_pDoc->AddDispEnt(pPolygon);
	//m_pDoc->PushTool(new FPolygonLineTool(m_pDoc, pPolygon));
}

void FMainWindow::onBezier()
{
	gSetHintMessage(QObject::tr("Test1_HW"));

	FBezier* pBezier(NULL);
	pBezier = new FBezier;

	m_pDoc->AddDispEnt(pBezier);
	m_pDoc->PushTool(new FBezierTool(m_pDoc, pBezier));
}

void FMainWindow::onBSpline()
{
	FBSpline* pPolygon(NULL);
	pPolygon = new FBSpline;
	m_pDoc->AddDispEnt(pPolygon);
	m_pDoc->PushTool(new FBSplineTool(m_pDoc, pPolygon));
}

void FMainWindow::onBSplineSurface()
{
	FBSurface* pPolygon(NULL);
	pPolygon = new FBSurface;

	m_pDoc->AddDispEnt(pPolygon);
}

void FMainWindow::onreadObj()
{
	//FTest test;
	//test.CreateFreeCurve(m_pDoc);

	FFileIO* pFio(NULL);
	pFio = new FFileIO();
	FT::FMesh* pFmesh(NULL);
	pFmesh = new FT::FMesh;
	pFmesh = pFio->ImportOBJ("../hw/bunny.obj");
	FFreeBody *pFbody = new FFreeBody(FT::FEuler::CreateBodyFromMesh(pFmesh));
 	m_pDoc->AddDispEnt(pFbody);
 	m_pDoc->PushTool(new FCutTool(m_pDoc,pFbody));
}

void FMainWindow::onViewTop()
{
	GetCurrentWnd()->GetView()->setWindowTitle(QObject::tr("Top(X/Y)"));
	GetCurrentWnd()->GetView()->GetKernelView()->SetViewType(FT::VIEW_TOP);
	GetCurrentWnd()->GetView()->update();
}

void FMainWindow::onViewBottom()
{
	GetCurrentWnd()->GetView()->setWindowTitle(QObject::tr("Bottom(X/Y)"));
	GetCurrentWnd()->GetView()->GetKernelView()->SetViewType(FT::VIEW_BOTTOM);
	GetCurrentWnd()->GetView()->update();
}

void FMainWindow::onViewFront()
{
	GetCurrentWnd()->GetView()->setWindowTitle(QObject::tr("Front(X/Z)"));
	GetCurrentWnd()->GetView()->GetKernelView()->SetViewType(FT::VIEW_FRONT);
	GetCurrentWnd()->GetView()->update();
}

void FMainWindow::onViewBack()
{
	GetCurrentWnd()->GetView()->setWindowTitle(QObject::tr("Front(X/Z)"));
	GetCurrentWnd()->GetView()->GetKernelView()->SetViewType(FT::VIEW_BACK);
	GetCurrentWnd()->GetView()->update();
}

void FMainWindow::onViewLeft()
{
	GetCurrentWnd()->GetView()->setWindowTitle(QObject::tr("Front(Y/Z)"));
	GetCurrentWnd()->GetView()->GetKernelView()->SetViewType(FT::VIEW_LEFT);
	GetCurrentWnd()->GetView()->update();
}

void FMainWindow::onViewRight()
{
	GetCurrentWnd()->GetView()->setWindowTitle(QObject::tr("Front(Y/Z)"));
	GetCurrentWnd()->GetView()->GetKernelView()->SetViewType(FT::VIEW_RIGHT);
	GetCurrentWnd()->GetView()->update();
}

void FMainWindow::onViewZoomIn()
{
	GetCurrentWnd()->GetView()->GetKernelView()->Zoom(1.5);
	GetCurrentWnd()->GetView()->update();
}

void FMainWindow::onViewZoomOut()
{
	GetCurrentWnd()->GetView()->GetKernelView()->Zoom(0.75);
	GetCurrentWnd()->GetView()->update();
}

void FMainWindow::onViewZoomAll()
{
	GetCurrentWnd()->GetView()->GetKernelView()->ZoomAll();
	GetCurrentWnd()->GetView()->update();
}

void FMainWindow::onViewSmooth()
{
	GetCurrentWnd()->GetView()->GetKernelView()->SetDispMode(FDispMode::SMOOTH_MODE);
	GetCurrentWnd()->GetView()->update();
	gSetHintMessage(QObject::tr("Smooth display"));
}

void FMainWindow::onViewMesh()
{
	GetCurrentWnd()->GetView()->GetKernelView()->SetDispMode(FDispMode::MESH_MODE);
	GetCurrentWnd()->GetView()->update();
	gSetHintMessage(QObject::tr("Mesh display"));
}

void FMainWindow::onViewPan()
{
	GetCurrentWnd()->GetView()->GetKernelView()->SetCameraType(FCameraType::PAN_CAM);
	GetCurrentWnd()->GetView()->update();
	gSetHintMessage(QObject::tr("Drag any mouse to translate model"));
}

void FMainWindow::onViewOrbit()
{
	GetCurrentWnd()->GetView()->GetKernelView()->SetCameraType(FCameraType::ORBIT_CAM);
	GetCurrentWnd()->GetView()->update();
	gSetHintMessage(QObject::tr("Drag any mouse to rotate model"));
}

void FMainWindow::onTestTest()
{
	QMessageBox::warning(QApplication::activeWindow(),QObject::tr("Caution"),
		QObject::tr("Just a test program!"), QMessageBox::Ok | QMessageBox::Cancel | QMessageBox::Abort);
	//FPolygonLine* pPolygon(NULL);
	//pPolygon = new FPolygonLine;
	//m_pDoc->AddDispEnt(pPolygon);
	//m_pDoc->PushTool(new FPolygonLineTool(m_pDoc, pPolygon));
}


void FMainWindow::resizeEvent(QResizeEvent* e)
{
	QSize size = e->size();
	if (m_pMdiArea)
	{
		m_pMdiArea->showMaximized();
	}
	if (m_pFrontWnd)
	{
		m_pFrontWnd->showMaximized();
	}
}

void FMainWindow::UpdateAllViews()
{
	if (m_pFrontWnd && !m_pFrontWnd->isHidden())		
		m_pFrontWnd->Update();
	if (m_pTopWnd && !m_pTopWnd->isHidden())
		m_pTopWnd->Update();
	if (m_pRightWnd && !m_pRightWnd->isHidden())
		m_pRightWnd->Update();
	if (m_p3DWnd && !m_p3DWnd->isHidden())
		m_p3DWnd->Update();
}

