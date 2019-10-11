#include "FBSpline.h"


FBSpline::FBSpline(void)
{

}


FBSpline::~FBSpline(void)
{
}

void FBSpline::Draw(const FT::FRenderOpt& renderOpt)
{
	glColor3f(1.0, 0.0, 0.0);
	glPointSize(5.0);
	glLineWidth(2.0);

	int nsize = m_pnts.count();

	glBegin(GL_POINTS);//GL_LINE_LOOP
	for (int i=0; i<nsize; i++)
	{
		glVertex3d(m_pnts[i].x, m_pnts[i].y, m_pnts[i].z);
	}
	glEnd();

	if (nsize >= 3)
	{
		m_curpnts.clear();
		int p = 2;
		int n = nsize - 1;
		double u = 0;
		double span = 1.0*(nsize - p)/100.0;

		QVector<int> U;
		U.clear();
		U.push_back(0);
		U.push_back(0);
		for (int i=0;i<=nsize - p;i++)
		{
			U.push_back(i);
		}
		U.push_back(nsize - p);
		U.push_back(nsize - p);

		for (int i=0;i<100;i++)
		{
			m_curpnts.push_back(CurvePoint(n,p,U,m_pnts,u));
			u = u + span;
		}
		glBegin(GL_LINE_STRIP);
		for (int i=0; i<m_curpnts.size(); i++)
		{
			glVertex3d(m_curpnts[i].x, m_curpnts[i].y, m_curpnts[i].z);
		}
		glVertex3d(m_pnts.back().x,m_pnts.back().y,m_pnts.back().z);
		glEnd();
	}




}

FT::F3dPoint FBSpline::CurvePoint(int n,int p,QVector<int> U,QVector<FT::F3dPoint> P,double u){

	QVector<double> N;
	N.clear();
	for (int i=0;i<=p;i++)
	{
		N.push_back(0);
	}

	int span = FindSpan(n,p,u,U);
	BasisFuns(span,u,p,U,N);

	FT::F3dPoint C;
	C.x = 0;
	C.y = 0;
	C.z = 0;

	for (int i=0;i<=p;i++)//gaidong
	{
		C = C + N[i]*P[span-p+i];
	}

	return C;
}


int FBSpline::FindSpan(int n,int p,double u,QVector<int> U){

	if(u == U[n+1]) return n;
	int low = p;
	int high = n+1;
	int mid = (high + low)/2;
	while(u<U[mid] || u>=U[mid+1]){

		if (u<U[mid])
		{
			high = mid;
		}

		else
		{
			low = mid;
		}
			
		mid = (high + low)/2;
	}

	return mid;
}

void FBSpline::BasisFuns(int i,double u,int p,QVector<int> U,QVector<double> &N){

	N[0] = 1.0;
	QVector<double> left;
	QVector<double> right;
	double saved;
	double temp;
	for (int j=0;j<=p;j++)
	{
		left.push_back(0);
		right.push_back(0);
	}
	for (int j=1;j<=p;j++){

		left[j] = u - U[i+1-j];
		right[j] = U[i+j] - u;
		saved = 0.0;
		for (int r=0;r<j;r++)
		{
			temp = N[r]/(right[r+1]+left[j-r]);
			N[r] = saved + right[r+1]*temp;
			saved = left[j-r]*temp;
		}
	N[j] = saved;
	}

}