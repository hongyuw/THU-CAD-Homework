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
	glPointSize(10.0);
	glLineWidth(2.0);

	int nsize = m_pnts.count();

	glBegin(GL_POINTS);//GL_LINE_LOOP
	for (int i=0; i<nsize; i++)
	{
		glVertex3d(m_pnts[i].x, m_pnts[i].y, m_pnts[i].z);
	}
	glEnd();

	if (interp == 1)
	{
		double d = 0;
		QVector<double> u_bar;
		u_bar.clear();
		int p = 3;
		int n = nsize - 1;
		//计算d
		for (int i=0;i<n;i++)
		{
			d = d + Distance(m_pnts[i],m_pnts[i+1]);
			u_bar.push_back(0);
		}
		//计算u_bar

		u_bar.push_back(1);
		for (int i=1;i<n;i++)
		{
			u_bar[i] = u_bar[i-1] + Distance(m_pnts[i],m_pnts[i-1])/d;
		}
		//计算U
		QVector<double> U;
		U.clear();
		for (int i=0;i<=p;i++)
		{
			U.push_back(0);
		}

		for (int j=1;j<=n - p;j++)
		{
			U.push_back((u_bar[j]+u_bar[j+1]+u_bar[j+2])/p); //hard code
		}

		for (int i=0;i<=p;i++)
		{
			U.push_back(1);
		}
		//计算A
		QVector<QVector<double>> A;
		A.resize(n+1);
		for (int i=0;i<=n;i++)
		{
			A[i].resize(n+1);
		}

		for (int i=0;i<=n;i++)
		{
			for (int j=0;j<=n;j++)
			{
				A[i][j] = 0;
			}
		}

		QVector<double> N1;
		N1.clear();
		for (int i=0;i<=p;i++)
		{
			N1.push_back(0);
		}

		for(int i=0;i<=n;i++){

			int span1 = FindSpan(n,p,u_bar[i],U);
			BasisFuns(span1,u_bar[i],p,U,N1);

			for (int j=0;j<=p;j++)
			{
				A[i][span1-p+j] = N1[j];
				double test1 = A[i][span1-p+j];
			}
		}
		//计算A逆
		LaGenMatDouble a(nsize,nsize);
		A_la = a;
		for (int i=0;i<=n;i++)
		{
			for (int j=0;j<=n;j++)
			{
				A_la(i,j) = A[i][j];
			}
		}
		//得到新控制点
		NewPnts(nsize);
		//画
		double u = 0;
		double span = 1.0/100.0;
		m_curpnts.clear();
		for (int i=0;i<100;i++)
		{
			m_curpnts.push_back(CurvePoint(n,p,U,m_pnts2,u));
			u = u + span;
		}
		glBegin(GL_LINE_STRIP);
		for (int i=0; i<m_curpnts.size(); i++)
		{
			glVertex3d(m_curpnts[i].x, m_curpnts[i].y, m_curpnts[i].z);
		}
		glVertex3d(m_pnts2.back().x,m_pnts2.back().y,m_pnts2.back().z);
		glEnd();
	}
	else if (nsize >= 3)
	{
		m_curpnts.clear();
		int p = 2;
		int n = nsize - 1;
		double u = 0;
		double span = 1.0/100.0;

		QVector<double> U;
		U.clear();
		U.push_back(0);
		U.push_back(0);
		for (int i=0;i<=nsize - p;i++)
		{
			U.push_back(i*1.0/(nsize-p));
		}
		U.push_back(1);
		U.push_back(1);

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

FT::F3dPoint FBSpline::CurvePoint(int n,int p,QVector<double> U,QVector<FT::F3dPoint> P,double u){

	QVector<double> N1;
	N1.clear();
	for (int i=0;i<=p;i++)
	{
		N1.push_back(0);
	}

	int span = FindSpan(n,p,u,U);
	BasisFuns(span,u,p,U,N1);

	FT::F3dPoint C;
	C.x = 0;
	C.y = 0;
	C.z = 0;

	for (int i=0;i<=p;i++)//gaidong
	{
		C = C + N1[i]*P[span-p+i];
	}

	return C;
}


int FBSpline::FindSpan(int n,int p,double u,QVector<double> U){

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

void FBSpline::BasisFuns(int i,double u,int p,QVector<double> U,QVector<double> &N){

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

double FBSpline::Distance(FT::F3dPoint p1,FT::F3dPoint p2){

	return sqrt((p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y) + (p1.z-p2.z)*(p1.z-p2.z));
}

void FBSpline::NewPnts(int nsize){
	m_pnts2.clear();
	LaGenMatDouble x(nsize,1);	
	LaGenMatDouble y(nsize,1);	
	LaGenMatDouble z(nsize,1);	

	for (int i=0;i<nsize;i++)
	{
		x(i,0)=m_pnts[i].x;
		y(i,0)=m_pnts[i].y;
		z(i,0)=m_pnts[i].z;
	}

	LaGenMatDouble cx(nsize,1);	
	LaQRLinearSolve(A_la,cx,x);
	LaGenMatDouble cy(nsize,1);	
	LaQRLinearSolve(A_la,cy,y);
	LaGenMatDouble cz(nsize,1);
	LaQRLinearSolve(A_la,cz,z);
	for (int i=0;i<nsize;i++)
	{
		m_pnts2.push_back(F3dPoint(cx(i,0),cy(i,0),cz(i,0)));
	}



}