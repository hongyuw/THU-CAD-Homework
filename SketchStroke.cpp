#include "SketchStroke.h"


SketchStroke::SketchStroke(void)
{

}


SketchStroke::~SketchStroke(void)
{
}

void SketchStroke::Draw(const FT::FRenderOpt& renderOpt)
{
	glColor3f(1.0, 0.0, 0.0);
	glPointSize(10.0);
	glLineWidth(2.0);

	glBegin(GL_LINE_STRIP);
	for (int i=0; i< m_pnts.count(); i++){
		glVertex3d(m_pnts[i].x, m_pnts[i].y, m_pnts[i].z);
	}
	glEnd();
	
	glBegin(GL_POINTS);
	for (int i=0; i< m_vpnts.count(); i++){
		glVertex3d(m_vpnts[i].x, m_vpnts[i].y, m_vpnts[i].z);
	}
	glEnd();
}

void SketchStroke::Detect(){
	m_vpnts.clear();
	double S = DetermineResampleSpacing(m_pnts);
	QVector<FT::F3dPoint> resampled = ResamplePoints(m_pnts,S);
	m_vpnts = Getcorners(resampled);
}

double SketchStroke::DetermineResampleSpacing(QVector<FT::F3dPoint> points){
	double topleftx,toprightx,toplefty,toprighty;
	QVector<double> xvec,zvec;
	xvec.clear();
	zvec.clear();
	for (int i = 0;i<points.size();i++)
	{
		xvec.append(points[i].x);
		zvec.append(points[i].z);
	}

	double max_x=points[0].x,max_z=points[0].z,min_x=points[0].x,min_z=points[0].z;
	for (int i = 0;i<points.size();i++)
	{
		if (xvec[i]>max_x){
			max_x=xvec[i];
		}
		if (xvec[i]<min_x){
			min_x=xvec[i];
		}
		if (zvec[i]>max_z){
			max_z=zvec[i];
		}
		if (zvec[i]<min_z){
			min_z=zvec[i];
		}
		
	}
	return Distance(F3dPoint(max_x,0,min_z),F3dPoint(min_x,0,max_z))/40;

}
QVector<FT::F3dPoint> SketchStroke::ResamplePoints(QVector<FT::F3dPoint> points, double S){
	
	double D = 0;
	double d;
    QVector<FT::F3dPoint> resampled;
	F3dPoint q;
	q.y = 0;
	resampled.append(points[0]);
	for (int i = 1;i<points.size();i++)
	{
		d = Distance(points[i-1],points[i]);
		if(D+d>=S){
			q.x = points[i-1].x + ((S-D)/d) * (points[i].x - points[i-1].x);
			q.z = points[i-1].z + ((S-D)/d) * (points[i].z - points[i-1].z);
			resampled.append(q);
			points.insert(i,q);
			D = 0;
		
		}

		else{
			D = D + d;
		}

	}
	
	return resampled;

}
QVector<FT::F3dPoint> SketchStroke::Getcorners(QVector<FT::F3dPoint> points){
	    
	QVector<FT::F3dPoint> corners;
    QVector<int> corners_ind;
	corners.append(points[0]);
	corners_ind.append(0);

	int w = 3;
	QVector<double> straws;

	for (int i = w;i<points.size()-3;i++)
	{
		straws.append(Distance(points[i-w],points[i+w]));
	}
	
	double sum = 0;

	for (int i = 0;i<straws.size();i++)
	{
		sum += straws[i];
	}

	double medval = sum/straws.size();
	double t = medval * 0.95;

	for (int i = 0;i<straws.size();i++)
	{
	    if (straws[i]<t)
	    {
			double localmin = straws[i];
			int localminindex = i+w;
			i++;
			while (i<straws.size() && straws[i]<t)
			{
				if (straws[i]<localmin)
				{
					localmin = straws[i];
					localminindex = i+w;
				}
				i++;
			}

			corners.append(points[localminindex]);
			corners_ind.append(localminindex);
	    }
	}
	corners.append(points.back());
	corners_ind.append(points.size()-1);

	corners = PostPorcesscorners(points,corners,corners_ind);
	return corners;
}


double SketchStroke::Distance(FT::F3dPoint point1,FT::F3dPoint point2){

	double dx,dz;
	dx = point1.x - point2.x;
	dz = point1.z - point2.z;

	return sqrt(dx*dx+dz*dz);
}

QVector<FT::F3dPoint> SketchStroke::PostPorcesscorners(QVector<FT::F3dPoint> points, QVector<FT::F3dPoint> corners, QVector<int> corners_ind){
	//insert

	for (int i = 1;i<corners.size();i++)
	{
		int c1 = corners_ind[i-1];
		int c2 = corners_ind[i];
		if (!IsLine(points,c1,c2))
		{
			int w = 3;
			QVector<double> straws;

			for (int j = w;j<c2-c1-3;j++)
			{
				straws.append(Distance(points[j-w],points[j+w]));
			}

			double lmin = straws[0];
			int lminind = c1+w;

			for (int j = 0;j<straws.size();j++)
			{
				if (straws[j]<lmin)
				{
					lmin = straws[j];
					lminind = j + c1 +w;
				}
			}
			corners.insert(i,points[lminind]);
			corners_ind.insert(i,lminind);
		}
	}

	//delete
    for (int i = 1;i<corners.size()-1;i++)
    {
         if (IsLine(points,corners_ind[i-1],corners_ind[i+1]))
         {
			 corners.remove(i);
			 corners_ind.remove(i);
			 i = i-1;
         }
    }
	return corners;
}

bool SketchStroke::IsLine(QVector<FT::F3dPoint> points, int a, int b){
	
	double threshhold = 0.95;
	double dis = Distance(points[a],points[b]);
	double pdis = PathDistance(points,a,b);
	if (dis/pdis<threshhold)
	{
		return false;
	}
	return true;
}

double SketchStroke::PathDistance(QVector<FT::F3dPoint> points, int a, int b){
	double d = 0;
	for (int i = a;i<b-1;i++)
	{
		d += Distance(points[i],points[i+1]);
	}
	
	return d;
}
