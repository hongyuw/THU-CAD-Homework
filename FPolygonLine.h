#ifndef FPOLYGON_LINE_H
#define  FPOLYGON_LINE_H
#pragma once

#include "../external/FKernel/include/FKernelDefines.h"
#include "../external/FKernel/include/FEntity.h"
using namespace FT;

class FPolygonLine: public FT::FEntity
{
public:
	FPolygonLine(void);
	~FPolygonLine(void);

	void Draw(const FT::FRenderOpt& renderOpt);

public:
	QVector<FT::F3dPoint> m_pnts;
	int finished;
};

#endif