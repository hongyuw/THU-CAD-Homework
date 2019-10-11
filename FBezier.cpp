#include "FBezier.h"


FBezier::FBezier(void)
{

}


FBezier::~FBezier(void)
{
}

void FBezier::Draw(const FT::FRenderOpt& renderOpt)
{
	glColor3f(1.0, 0.0, 0.0);
	glPointSize(10.0);
	glLineWidth(2.0);

	int nsize = m_pnts.count();
	glBegin(GL_POINTS);
	for (int i=0; i<nsize; i++)
	{
		glVertex3d(m_pnts[i].x, m_pnts[i].y, m_pnts[i].z);
	}
	glEnd();

	if (m_pnts.size()>1)
	{	
		m_curpnts = CalculateCur(m_pnts);
	}

	glBegin(GL_LINE_STRIP);
	for (int i=0; i<m_curpnts.size(); i++)
	{
		glVertex3d(m_curpnts[i].x, m_curpnts[i].y, m_curpnts[i].z);
	}
	glEnd();
}

QVector<FT::F3dPoint> FBezier::CalculateCur(QVector<FT::F3dPoint> m_pnt){
	double x,y,z;	
	int m = 100;//计算最终曲线上的100个点,连成一条线;
	QVector<FT::F3dPoint> curpnts;
	curpnts.resize(m);
	//两条辅助曲线；
	QVector<FT::F3dPoint> cur1;
	cur1.resize(m);
	QVector<FT::F3dPoint> cur2;
	cur2.resize(m);
	//两条辅助曲线控制点；
	QVector<FT::F3dPoint> pnts1;
	pnts1.resize(m_pnt.size()-1);
	QVector<FT::F3dPoint> pnts2;
	pnts2.resize(m_pnt.size()-1);

	if(m_pnt.size()==2){
		for(double t=1;t<=m;t++){
			x = t/m*(m_pnt[1].x - m_pnt[0].x) + m_pnt[0].x;
			y = t/m*(m_pnt[1].y - m_pnt[0].y) + m_pnt[0].y;
			z = t/m*(m_pnt[1].z - m_pnt[0].z) + m_pnt[0].z;
			curpnts[t-1]=F3dPoint(x,y,z);
		}

		return curpnts;
	}

	else{
		for(int i = 0;i<m_pnt.size()-1;i++){
			pnts1[i] = m_pnt[i];
		}
		for(int i = 1;i<m_pnt.size();i++){
			pnts2[i-1] = m_pnt[i];
		}

		cur1 = CalculateCur(pnts1);
		cur2 = CalculateCur(pnts2);

		for(double t=1;t<=m;t++){
			x = t/m*(cur2[t-1].x - cur1[t-1].x) + cur1[t-1].x;
   		    y = t/m*(cur2[t-1].y - cur1[t-1].y) + cur1[t-1].y;
		    z = t/m*(cur2[t-1].z - cur1[t-1].z) + cur1[t-1].z;
			curpnts[t-1]=F3dPoint(x,y,z);
		}
		return curpnts;
	}
}
