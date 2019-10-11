#ifndef FSKETCHSKETCH_H
#define  FSKETCHSKETCH_H
#pragma once

#include "../external/FKernel/include/FKernelDefines.h"
#include "../external/FKernel/include/FEntity.h"
#include "SketchStroke.h"

using namespace FT;

class SketchSketch: public FT::FEntity
{
public:
	SketchSketch(void);
	~SketchSketch(void);

	void Draw(const FT::FRenderOpt& renderOpt);

	void Detect();

public:
	QVector<SketchStroke*>	m_strokes;	
	 
};

#endif




