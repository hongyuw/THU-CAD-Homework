#ifndef FBSURFACE_H
#define  FBSURFACE_H
#pragma once

#include "../external/FKernel/include/FKernelDefines.h"
#include "../external/FKernel/include/FEntity.h"
using namespace FT;

class FBSurface: public FT::FEntity
{
public:
	FBSurface(void);
	~FBSurface(void);

	void Draw(const FT::FRenderOpt& renderOpt);
	FT::F3dPoint SurPoint(int n,int p,QVector<int> U,QVector<QVector<FT::F3dPoint>> P,double u,double v);
	int FindSpan(int n,int p,double u,QVector<int> U);
	void BasisFuns(int i,double u,int p,QVector<int> U,QVector<double> &N);

public:
	QVector<QVector<FT::F3dPoint>> m_pnts;
	QVector<QVector<FT::F3dPoint>> m_surpnts;
	QVector<FT::F3dPoint> m_testpnts;
};

#endif