#include "FBSurface.h"


FBSurface::FBSurface(void)
{

}


FBSurface::~FBSurface(void)
{
}

void FBSurface::Draw(const FT::FRenderOpt& renderOpt)
{
	glColor3f(1.0, 0.0, 0.0);
	glPointSize(5.0);
	glLineWidth(2.0);
	int pntsize = 4;//4*4 control
	int nsize = pntsize;
	int surpntsize = 40;//100*100 surf
	m_pnts.resize(pntsize);
	for (int i=0; i<pntsize; i++)
	{
		m_pnts[i].resize(pntsize);
	}

	for (int i=0; i<pntsize; i++)
	{
		for (int j=0; j<pntsize; j++)
		{
			m_pnts[i][j]=F3dPoint(i*10,j*10,5*abs(i-1));
		}

	}
	m_pnts[3][3] = F3dPoint(30,30,40);
	m_pnts[1][1] = F3dPoint(10,10,20);
	m_pnts[0][1] = F3dPoint(-10,10,40);
	glBegin(GL_POINTS);
		for (int i=0; i<4; i++)
		{
			for (int j=0; j<4; j++)
			{
			   glVertex3d(m_pnts[i][j].x, m_pnts[i][j].y, m_pnts[i][j].z);
			}

		}
	glEnd();

	m_surpnts.resize(surpntsize);
	for (int i=0; i<surpntsize; i++)
	{
		m_surpnts[i].resize(surpntsize);
	}
	int p = 2;
	int n = nsize - 1;
	double u = 0;
	double v = 0;
	double span = 1.0*(nsize - p)/surpntsize;

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

	for (int i=0;i<surpntsize;i++)
	{
		for (int j=0;j<surpntsize;j++)
		{
			m_surpnts[i][j] = SurPoint(n,p,U,m_pnts,u,v);
		//	m_testpnts.push_back(SurPoint(n,p,U,m_pnts,u,v));
			v = v + span;
		}
		u = u + span;
		v = 0;
	}

	for (int i=0;i<surpntsize-1;i++)
	{
		m_testpnts.clear();
			for (int j=0;j<surpntsize;j++)
			{

				m_testpnts.push_back(m_surpnts[i][j]);
				m_testpnts.push_back(m_surpnts[i+1][j]);
			}

		glBegin(GL_TRIANGLE_STRIP);
		for (int k=0; k<m_testpnts.size(); k++)
		{
			glVertex3d(m_testpnts[k].x, m_testpnts[k].y, m_testpnts[k].z);
		}
		glEnd();

	}
	//if (m_surpnts[i][0].z-m_surpnts[i+1][0].z>0)
	//{
	//	for (int j=0;j<surpntsize;j++)
	//	{

	//		m_testpnts.push_back(m_surpnts[i][j]);
	//		m_testpnts.push_back(m_surpnts[i+1][j]);
	//	}

	//}
	//else{
	//	for (int j=0;j<surpntsize;j++)
	//	{
	//		m_testpnts.push_back(m_surpnts[i+1][j]);
	//		m_testpnts.push_back(m_surpnts[i][j]);
	//		
	//	}

	//}


	//m_testpnts.clear();

	//m_testpnts.push_back(m_surpnts[0][0]);
	//m_testpnts.push_back(m_surpnts[1][0]);
	//m_testpnts.push_back(m_surpnts[0][1]);
	//m_testpnts.push_back(m_surpnts[1][1]);
	//m_testpnts.push_back(m_surpnts[0][2]);
	//m_testpnts.push_back(m_surpnts[1][2]);
	//glBegin(GL_TRIANGLE_STRIP);
	//for (int k=0; k<m_testpnts.size(); k++)
	//{
	//	glVertex3d(m_testpnts[k].x, m_testpnts[k].y, m_testpnts[k].z);
	//}
	//glEnd();
	//glBegin(GL_POINTS);
	//for (int i=0; i<surpntsize; i++)
	//{
	//	for (int j=0; j<surpntsize; j++)
	//	{
	//		glVertex3d(m_surpnts[i][j].x, m_surpnts[i][j].y, m_surpnts[i][j].z);
	//	}

	//}
	//glEnd();
}

FT::F3dPoint FBSurface::SurPoint(int n,int p,QVector<int> U,QVector<QVector<FT::F3dPoint>> P,double u,double v){

	QVector<double> Nu;
	Nu.clear();
	for (int i=0;i<=p;i++)
	{
		Nu.push_back(0);
	}

	QVector<double> Nv;
	Nv.clear();
	for (int i=0;i<=p;i++)
	{
		Nv.push_back(0);
	}

	int uspan = FindSpan(n,p,u,U);
	BasisFuns(uspan,u,p,U,Nu);

	int vspan = FindSpan(n,p,v,U);
	BasisFuns(vspan,v,p,U,Nv);

	FT::F3dPoint C;
	C.x = 0;
	C.y = 0;
	C.z = 0;

	for (int i=0;i<=p;i++)//Èý¸ö¾ØÕóÏà³Ë
	{
		for (int j=0;j<=p;j++)
		{
			C = C + Nu[i]*P[uspan-p+i][vspan-p+j]*Nv[j];
		}
		
	}

	return C;
}


int FBSurface::FindSpan(int n,int p,double u,QVector<int> U){

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

void FBSurface::BasisFuns(int i,double u,int p,QVector<int> U,QVector<double> &N){

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