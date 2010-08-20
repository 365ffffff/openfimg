/*
 * fimg/raster.c
 *
 * SAMSUNG S3C6410 FIMG-3DSE RASTER ENGINE RELATED FUNCTIONS
 *
 * Copyrights:	2006 by Samsung Electronics Limited (original code)
 *		2010 by Tomasz Figa <tomasz.figa@gmail.com> (new code)
 */

#include "fimg_private.h"

#define FGRA_PIX_SAMP		(0x38000)
#define FGRA_D_OFF_EN		(0x38004)
#define FGRA_D_OFF_FACTOR	(0x38008)
#define FGRA_D_OFF_UNITS	(0x3800c)
#define FGRA_D_OFF_R_IN		(0x38010)
#define FGRA_BFCULL		(0x38014)
#define FGRA_YCLIP		(0x38018)
#define FGRA_LODCTL		(0x3c000)
#define FGRA_XCLIP		(0x3c004)
#define FGRA_PWIDTH		(0x3801c)
#define FGRA_PSIZE_MIN		(0x38020)
#define FGRA_PSIZE_MAX		(0x38024)
#define FGRA_COORDREPLACE	(0x38028)
#define FGRA_LWIDTH		(0x3802c)

#define FGRA_COORDREPLACE_VAL(i)	(1 << (i))

/* TODO: Function inlining */

/*****************************************************************************
 * FUNCTIONS:	fimgSetPixelSamplePos
 * SYNOPSIS:	This function controls whether pixel is sampled at the center
 * 		or corner of the sample
 * PARAMETERS:	[IN] FGL_Sample samp: pixel sampling position
 * 		     Zero	- sample at center
 *		     Non-zero	- sample at left-top corner
 *****************************************************************************/
void fimgSetPixelSamplePos(fimgContext *ctx, int corner)
{
	ctx->rasterizer.samplePos = !!corner;
	fimgWrite(ctx, !!corner, FGRA_PIX_SAMP);
}

/*****************************************************************************
 * FUNCTIONS:	fimgEnableDepthOffset
 * SYNOPSIS:	This function decides to use the depth offset
 *		Note : This function affects polygon, not point and line.
 * PARAMETERS:	[IN] int enable: Non-zero to enable. default value is FG_FALSE
 *****************************************************************************/
void fimgEnableDepthOffset(fimgContext *ctx, int enable)
{
	ctx->rasterizer.dOffEn = !!enable;
	fimgWrite(ctx, !!enable, FGRA_D_OFF_EN);
}

/*****************************************************************************
 * FUNCTIONS:	fimgSetDepthOffsetParam
 * SYNOPSIS:	This function sets depth offset parameters one by one
 *		Note : This function affects polygon, not point and line.
 * PARAMETERS:	[IN] FGL_DepthOffsetParam param: depth offset parameter to be set
 *		[IN] unsigned int value: specified parameter value
 * RETURNS: 	 0 - if successful.
 *		-1 - if an invalid parameter is specified.
 *****************************************************************************/
void fimgSetDepthOffsetParam(fimgContext *ctx, float factor, float units)
{
	ctx->rasterizer.dOffFactor = factor;
	ctx->rasterizer.dOffUnits = units;
	fimgWriteF(ctx, factor, FGRA_D_OFF_FACTOR);
	fimgWriteF(ctx, units, FGRA_D_OFF_UNITS);
}

/*****************************************************************************
 * FUNCTIONS:	fimgSetFaceCullControl
 * SYNOPSIS:	This function controls back-face culling.
 * PARAMETERS:	[IN] FG_BOOL enable: FG_TRUE for using back-face cull.
 *		[IN] FG_BOOL bCW: FG_TRUE for make a clock-wise face front
 *		[IN] fimgCullingFace face: culling face
 *****************************************************************************/
void fimgSetFaceCullEnable(fimgContext *ctx, int enable)
{
	ctx->rasterizer.cull.bits.enable = !!enable;

	fimgWrite(ctx, ctx->rasterizer.cull.val, FGRA_BFCULL);
}

void fimgSetFaceCullControl(fimgContext *ctx, int bCW, fimgCullingFace face)
{
	ctx->rasterizer.cull.bits.clockwise = !!bCW;
	ctx->rasterizer.cull.bits.face = face;

	fimgWrite(ctx, ctx->rasterizer.cull.val, FGRA_BFCULL);
}

/*****************************************************************************
 * FUNCTIONS:	fimgSetYClip
 * SYNOPSIS:	This function sets clipping plan in Y coordinate.
 * PARAMETERS:	[IN] unsigned int ymin: Y clipping min. coordinate.
 *		[IN] unsigned int ymax: Y clipping max. coordinate.
 *****************************************************************************/
void fimgSetYClip(fimgContext *ctx, unsigned int ymin, unsigned int ymax)
{
	ctx->rasterizer.yClip.bits.maxval = ymax;
	ctx->rasterizer.yClip.bits.minval = ymin;

	fimgWrite(ctx, ctx->rasterizer.yClip.val, FGRA_YCLIP);
}

/*****************************************************************************
 * FUNCTIONS:	fimgSetLODControl
 * SYNOPSIS:	This function sets LOD calculation control.
 * PARAMETERS:	[IN] fimgLODControl ctl: lodcon values
 *****************************************************************************/
void fimgSetLODControl(fimgContext *ctx, fimgLODControl ctl)
{
	fimgWrite(ctx, ctl.val, FGRA_LODCTL);
}

/*****************************************************************************
 * FUNCTIONS:	fimgSetXClip
 * SYNOPSIS:	This function sets clipping plan in X coordinate.
 * PARAMETERS:	[IN] unsigned int xmin: X clipping min. coordinate.
 *		[IN] unsigned int xmax: X clipping max. coordinate.
 *****************************************************************************/
void fimgSetXClip(fimgContext *ctx, unsigned int xmin, unsigned int xmax)
{
	ctx->rasterizer.xClip.bits.maxval = xmax;
	ctx->rasterizer.xClip.bits.minval = xmin;

	fimgWrite(ctx, ctx->rasterizer.xClip.val, FGRA_XCLIP);
}

/*****************************************************************************
 * FUNCTIONS:	fimgSetPointWidth
 * SYNOPSIS:	This function sets point width register.
 * PARAMETERS:	[IN] float pWidth: Point width.
 *****************************************************************************/
void fimgSetPointWidth(fimgContext *ctx, float pWidth)
{
	ctx->rasterizer.pointWidth = pWidth;
	fimgWriteF(ctx, pWidth, FGRA_PWIDTH);
}

/*****************************************************************************
 * FUNCTIONS:	fimgSetMinimumPointWidth
 * SYNOPSIS:	This function sets minimum point width register.
 * PARAMETERS:	[IN] float pWidthMin: Minimum point width.
 *****************************************************************************/
void fimgSetMinimumPointWidth(fimgContext *ctx, float pWidthMin)
{
	ctx->rasterizer.pointWidthMin = pWidthMin;
	fimgWriteF(ctx, pWidthMin, FGRA_PSIZE_MIN);
}

/*****************************************************************************
 * FUNCTIONS:	fimgSetMaximumPointWidth
 * SYNOPSIS:	This function sets maximum point width register.
 * PARAMETERS:	[IN] float pWidthMax: Maximum point width.
 *****************************************************************************/
void fimgSetMaximumPointWidth(fimgContext *ctx, float pWidthMax)
{
	ctx->rasterizer.pointWidthMax = pWidthMax;
	fimgWriteF(ctx, pWidthMax, FGRA_PSIZE_MAX);
}

/*****************************************************************************
 * FUNCTIONS:	fimgSetCoordReplace
 * SYNOPSIS:	This function is used only in point sprite rendering.
 *		Only one bit chooses generated texture coordinate for point sprite.
 * PARAMETERS:	[IN] unsigned int coordReplaceNum :
 *		     Attribute number for texture coord. of point sprite.
 *****************************************************************************/
void fimgSetCoordReplace(fimgContext *ctx, unsigned int coordReplaceNum)
{
	ctx->rasterizer.spriteCoordAttrib = FGRA_COORDREPLACE_VAL(coordReplaceNum);
	fimgWrite(ctx, FGRA_COORDREPLACE_VAL(coordReplaceNum), FGRA_COORDREPLACE);
}

/*****************************************************************************
 * FUNCTIONS:	fimgSetLineWidth
 * SYNOPSIS:	This function sets line width register.
 * PARAMETERS:	[IN] float lWidth: Line width.
 *****************************************************************************/
void fimgSetLineWidth(fimgContext *ctx, float lWidth)
{
	ctx->rasterizer.lineWidth = lWidth;
	fimgWriteF(ctx, lWidth, FGRA_LWIDTH);
}

void fimgCreateRasterizerContext(fimgContext *ctx)
{
	ctx->rasterizer.pointWidth = 1.0f;
	ctx->rasterizer.pointWidthMin = 1.0f;
	ctx->rasterizer.pointWidthMax = 2048.0f;
	ctx->rasterizer.lineWidth = 1.0f;
}

void fimgRestoreRasterizerState(fimgContext *ctx)
{
	fimgWrite(ctx, ctx->rasterizer.samplePos, FGRA_PIX_SAMP);
	fimgWrite(ctx, ctx->rasterizer.dOffEn, FGRA_D_OFF_EN);
	fimgWriteF(ctx, ctx->rasterizer.dOffFactor, FGRA_D_OFF_FACTOR);
	fimgWriteF(ctx, ctx->rasterizer.dOffUnits, FGRA_D_OFF_UNITS);
	fimgWrite(ctx, ctx->rasterizer.cull.val, FGRA_BFCULL);
	fimgWrite(ctx, ctx->rasterizer.yClip.val, FGRA_YCLIP);
	fimgWrite(ctx, ctx->rasterizer.xClip.val, FGRA_XCLIP);
	fimgWriteF(ctx, ctx->rasterizer.pointWidth, FGRA_PWIDTH);
	fimgWriteF(ctx, ctx->rasterizer.pointWidthMin, FGRA_PSIZE_MIN);
	fimgWriteF(ctx, ctx->rasterizer.pointWidthMax, FGRA_PSIZE_MAX);
	fimgWrite(ctx, ctx->rasterizer.spriteCoordAttrib, FGRA_COORDREPLACE);
	fimgWriteF(ctx, ctx->rasterizer.lineWidth, FGRA_LWIDTH);
}