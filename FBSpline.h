#ifndef FBSPLINE_H
#define  FBSPLINE_H
#pragma once

#include "../external/FKernel/include/FKernelDefines.h"
#include "../external/FKernel/include/FEntity.h"
using namespace FT;

class FBSpline: public FT::FEntity
{
public:
	FBSpline(void);
	~FBSpline(void);

	void Draw(const FT::FRenderOpt& renderOpt);
	FT::F3dPoint CurvePoint(int n,int p,QVector<int> U,QVector<FT::F3dPoint> P,double u);
	int FindSpan(int n,int p,double u,QVector<int> U);
	void BasisFuns(int i,double u,int p,QVector<int> U,QVector<double> &N);
public:
	QVector<FT::F3dPoint> m_pnts;
	QVector<FT::F3dPoint> m_curpnts;
	int select;
};

#endif