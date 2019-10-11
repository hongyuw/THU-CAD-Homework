#include "SketchSketch.h"

SketchSketch::SketchSketch(void)
{

}


SketchSketch::~SketchSketch(void)
{
}

void SketchSketch::Draw(const FT::FRenderOpt& renderOpt)
{
	glColor3f(1.0, 0.0, 0.0);
	glPointSize(5.0);
	glLineWidth(2.0);

	int ssize = m_strokes.count();
	for (int j=0;j<ssize;j++){
	    m_strokes[j]->Draw(renderOpt);

	}
}

void SketchSketch::Detect(){

	for (int i = 0;i<m_strokes.count();i++)
	{
		m_strokes[i]->Detect();
	}
}