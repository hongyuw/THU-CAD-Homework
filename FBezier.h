#ifndef FBEZIER_H
#define  FBEZIER_H
#pragma once

#include "../external/FKernel/include/FKernelDefines.h"
#include "../external/FKernel/include/FEntity.h"
using namespace FT;

class FBezier: public FT::FEntity
{
public:
	FBezier(void);
	~FBezier(void);

	void Draw(const FT::FRenderOpt& renderOpt);
	QVector<FT::F3dPoint> CalculateCur(QVector<FT::F3dPoint> m_pnt);

public:
	QVector<FT::F3dPoint> m_pnts;
	QVector<FT::F3dPoint> m_curpnts;
};

#endif