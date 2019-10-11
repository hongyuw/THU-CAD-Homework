#ifndef SKETCH_STROKE_H
#define SKETCH_STROKE_H
#pragma once

#include "../external/FKernel/include/FKernelDefines.h"
#include "../external/FKernel/include/FEntity.h"
using namespace FT;

class SketchStroke: public FT::FEntity
{
public:
	SketchStroke(void);
	~SketchStroke(void);

	void Draw(const FT::FRenderOpt& renderOpt);

	void Detect();
    double DetermineResampleSpacing(QVector<FT::F3dPoint> points);
	QVector<FT::F3dPoint> ResamplePoints(QVector<FT::F3dPoint> points, double S);
	QVector<FT::F3dPoint> Getcorners(QVector<FT::F3dPoint> points);
	double Distance(FT::F3dPoint point1,FT::F3dPoint point2);

	QVector<FT::F3dPoint> PostPorcesscorners(QVector<FT::F3dPoint> points, QVector<FT::F3dPoint> corners, QVector<int> corners_ind);
	bool IsLine(QVector<FT::F3dPoint> points, int a, int b);
	double PathDistance(QVector<FT::F3dPoint> points, int a, int b);

public:
	QVector<FT::F3dPoint> m_pnts;
	QVector<FT::F3dPoint> m_vpnts;

};

#endif