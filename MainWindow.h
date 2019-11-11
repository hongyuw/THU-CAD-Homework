/*!	
\brief	Create the main window UI, i.e. menus, toolbar, status bar, etc.
		Handle events from these UIs.
\author: jacky
\date    2014-03
*/
//////////////////////////////////////////////////////////////////////////
#ifndef F_MAINWINDOW_H
#define F_MAINWINDOW_H

#include <QtGui>

namespace Ui {
	class UiMainWindow;
}
namespace FT{
	class FBody;
}

class FMainDoc;
class FSubWindow;
class SurfacePointCloud;
class SurfacePreprocess;
class FParserDkWidget;

class FMainWindow :public QMainWindow
{
	Q_OBJECT
public:
	FMainWindow(QWidget* parent=NULL);
	~FMainWindow();

	/////////////////////////////// Accessing methods ///////////////////////////////////////////
	FMainDoc*	GetMainDoc(){return m_pDoc;}
	QMdiArea*	GetMdiArea(){return m_pMdiArea;}
	FSubWindow*	GetFrontWnd(){return m_pFrontWnd;}
	FSubWindow*	GetTopWnd(){return m_pTopWnd;}
	FSubWindow*	GetRightWnd(){return m_pRightWnd;}
	FSubWindow*	Get3DWnd(){return m_p3DWnd;}
	FSubWindow* GetCurrentWnd();
	QLabel*		GetHintLabel(){return m_hintLabel;}
	FParserDkWidget* GetParserWidget(){return m_pParserWnd;}

	/////////////////////////////// Public methods ///////////////////////////////////////////
	void		CreateStatusBar();
	void		CreateSignalSlots();
	void		UpdateAllViews();
	bool		OpenFile(QString fname);

	virtual void	resizeEvent(QResizeEvent* e);
	virtual void	dragEnterEvent(QDragEnterEvent * e);
	virtual void	dropEvent(QDropEvent* e);
public slots:
	//File
	bool		onFileNew();
	bool		onFileOpen();
	bool		onFileSave();
	bool		onFileClose();
	//View
	void		onViewTop();
	void		onViewBottom();
	void		onViewFront();
	void		onViewBack();
	void		onViewLeft();
	void		onViewRight();
	void		onViewZoomIn();
	void		onViewZoomOut();
	void		onViewZoomAll();
	void		onViewPan();
	void		onViewOrbit();
	void		onViewSmooth();
	void		onViewMesh();
	void		onSketch2D_HW();
	void		onTest1_HW();
	//Test menu
	void		onTestTest();
	//Homework menu
	void        onHelloworld();
	void        onBezier();
	void        onBSpline();
	void	    onBSplineSurface();
	void        onreadObj();
private:
	Ui::UiMainWindow*	m_pUI;		///UI pointer
	QMdiArea*			m_pMdiArea;	///multiple documents area handle
	FSubWindow*			m_pFrontWnd, *m_pTopWnd, *m_pRightWnd, *m_p3DWnd;		//four views
	FMainDoc*			m_pDoc;		///Doc pointer
	QLabel*				m_hintLabel;	///label display hint message in status bar
	FParserDkWidget*	m_pParserWnd;	///parser window

};

#endif

