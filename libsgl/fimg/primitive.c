/*
 * fimg/primitive.c
 *
 * SAMSUNG S3C6410 FIMG-3DSE PRIMITIVE ENGINE RELATED FUNCTIONS
 *
 * Copyrights:	2005 by Samsung Electronics Limited (original code)
 *		2010 by Tomasz Figa <tomasz.figa@gmail.com> (new code)
 */

#include "fimg_private.h"

#define PRIMITIVE_OFFSET		0x30000

#define FGPE_VERTEX_CONTEXT		(0x30000)
#define FGPE_VIEWPORT_OX		(0x30004)
#define FGPE_VIEWPORT_OY		(0x30008)
#define FGPE_VIEWPORT_HALF_PX		(0x3000c)
#define FGPE_VIEWPORT_HALF_PY		(0x30010)
#define FGPE_DEPTHRANGE_HALF_F_SUB_N	(0x30014)
#define FGPE_DEPTHRANGE_HALF_F_ADD_N	(0x30018)

/*
 * Functions
 * TODO: Function inlining
 */
/*****************************************************************************
 * FUNCTIONS:	fimgSetVertexContext
 * SYNOPSIS:	This function specifies the type of primitive to be drawn
 *		and the number of input attributes
 * PARAMETERS:	[IN] pVtx: the pointer of FGL_Vertex strucutre
 *****************************************************************************/
void fimgSetVertexContext(fimgContext *ctx, unsigned int type, unsigned int count)
{
	ctx->primitive.vctx.bits.type = type; // See fimgPrimitiveType enum
	ctx->primitive.vctx.bits.vsOut = count - 1; // Without position

	ctx->numAttribs = count;

	fimgWrite(ctx, ctx->primitive.vctx.val, FGPE_VERTEX_CONTEXT);
}

/*****************************************************************************
 * FUNCTIONS:	fimgSetViewportParams
 * SYNOPSIS:	This function specifies the viewport parameters.
 * PARAMETERS:	[IN] bYFlip: Not zero if you want y-flipped window coordindate
 *		     zero if otherwise
 *		[IN] x0, y0: origin of viewport in window coordindate system
 *		[IN] px, py: width and height of viewport in terms of pixel
 *		[IN] H: height of window in terms of pixel
 *****************************************************************************/
void fimgSetViewportParams(fimgContext *ctx, float x0, float y0, float px, float py)
{
	// local variable declaration
	float half_px = px * 0.5f;
	float half_py = py * 0.5f;

	// ox: x-coordinate of viewport center
	float ox = x0 + half_px;
	// oy: y-coordindate of viewport center
	float oy = y0 + half_py;

	ctx->primitive.ox = ox;
	ctx->primitive.oy = oy;
	ctx->primitive.halfPX = half_px;
	ctx->primitive.halfPY = half_py;

	fimgWriteF(ctx, ox, FGPE_VIEWPORT_OX);
	fimgWriteF(ctx, oy, FGPE_VIEWPORT_OY);
	fimgWriteF(ctx, half_px, FGPE_VIEWPORT_HALF_PX);
	fimgWriteF(ctx, half_py, FGPE_VIEWPORT_HALF_PY);
}

/*****************************************************************************
 * FUNCTIONS:	fimgSetDepthRange
 * SYNOPSIS:	This function defines an encoding for z-coordinate that's performed
 *		during the viewport transformation.
 * PARAMETERS:	[IN] n: near value ( n should be in [0, 1])
 *		[IN] f: far value (f should be in [0, 1])
 *****************************************************************************/
void fimgSetDepthRange(fimgContext *ctx, float n, float f)
{
	float half_distance = (f - n) * 0.5f;
	float center = (f + n) * 0.5f;

	ctx->primitive.halfDistance = half_distance;
	ctx->primitive.center = center;

	fimgWriteF(ctx, half_distance, FGPE_DEPTHRANGE_HALF_F_SUB_N);
	fimgWriteF(ctx, center, FGPE_DEPTHRANGE_HALF_F_ADD_N);
}

void fimgCreatePrimitiveContext(fimgContext *ctx)
{
	ctx->primitive.halfDistance = 0.5f;
	ctx->primitive.center = 0.5f;
}

void fimgRestorePrimitiveState(fimgContext *ctx)
{
	fimgWrite(ctx, ctx->primitive.vctx.val, FGPE_VERTEX_CONTEXT);
	fimgWriteF(ctx, ctx->primitive.ox, FGPE_VIEWPORT_OX);
	fimgWriteF(ctx, ctx->primitive.oy, FGPE_VIEWPORT_OY);
	fimgWriteF(ctx, ctx->primitive.halfPX, FGPE_VIEWPORT_HALF_PX);
	fimgWriteF(ctx, ctx->primitive.halfPY, FGPE_VIEWPORT_HALF_PY);
	fimgWriteF(ctx, ctx->primitive.halfDistance, FGPE_DEPTHRANGE_HALF_F_SUB_N);
	fimgWriteF(ctx, ctx->primitive.center, FGPE_DEPTHRANGE_HALF_F_ADD_N);
}