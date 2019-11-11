#ifndef FBSPLINE_H
#define  FBSPLINE_H
#pragma once



#include "../external/FKernel/include/FKernelDefines.h"
#include "../external/FKernel/include/FEntity.h"

#include "lapack++.h"
using namespace FT;

class FBSpline: public FT::FEntity
{
public:
	FBSpline(void);
	~FBSpline(void);

	void Draw(const FT::FRenderOpt& renderOpt);
	FT::F3dPoint CurvePoint(int n,int p,QVector<double> U,QVector<FT::F3dPoint> P,double u);
	int FindSpan(int n,int p,double u,QVector<double> U);
	void BasisFuns(int i,double u,int p,QVector<double> U,QVector<double> &N);
	double Distance(FT::F3dPoint p1,FT::F3dPoint p2);
	void NewPnts(int nsize);
public:
	QVector<FT::F3dPoint> m_pnts;
	QVector<FT::F3dPoint> m_pnts2;
	QVector<FT::F3dPoint> m_curpnts;
	int select;
	int interp;
	LaGenMatDouble A_la;
};

#endif