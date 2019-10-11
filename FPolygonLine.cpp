#include "FPolygonLine.h"


FPolygonLine::FPolygonLine(void)
{

}


FPolygonLine::~FPolygonLine(void)
{
}

void FPolygonLine::Draw(const FT::FRenderOpt& renderOpt)
{
	glColor3f(1.0, 0.0, 0.0);
	glPointSize(5.0);
	glLineWidth(2.0);

	int nsize = m_pnts.count();

	//glBegin(GL_POINTS);//GL_LINE_LOOP
	//for (int i=0; i<nsize; i++)
	//{
	//	glVertex3d(m_pnts[i].x, m_pnts[i].y, m_pnts[i].z);
	//}
	//glEnd();
	//GL_LINE_LOOP
	if(finished){
		glBegin(GL_LINE_LOOP);
		for (int i=0; i<nsize; i++)
		{
			glVertex3d(m_pnts[i].x, m_pnts[i].y, m_pnts[i].z);
		}
		glEnd();
	}


	else
	{
		glBegin(GL_LINE_STRIP);
		for (int i=0; i<nsize; i++)
		{
			glVertex3d(m_pnts[i].x, m_pnts[i].y, m_pnts[i].z);
		}
		glEnd();

	}

}
