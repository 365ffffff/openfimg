/*
 * fimg/fimg.h
 *
 * SAMSUNG S3C6410 FIMG-3DSE LOW LEVEL DEFINITIONS (PUBLIC PART)
 *
 * Copyrights:	2010 by Tomasz Figa <tomasz.figa@gmail.com>
 */

#ifndef _FIMG_H_
#define _FIMG_H_

#ifdef __cplusplus
extern "C" {
#endif

//=============================================================================

struct _fimgContext;
typedef struct _fimgContext fimgContext;

/*
 * Global block
 */

/* Type definitions */
typedef union {
	unsigned int val;
	struct {
		unsigned		:13;
		unsigned ccache0	:1;
		unsigned		:1;
		unsigned pf0		:1;
		unsigned		:3;
		unsigned ps0		:1;
		unsigned		:1;
		unsigned ra		:1;
		unsigned tse		:1;
		unsigned pe		:1;
		unsigned		:3;
		unsigned vs		:1;
		unsigned vc		:1;
		unsigned hvf		:1;
		unsigned hi		:1;
		unsigned host_fifo	:1;
	} bits;
} fimgPipelineStatus;

typedef union {
	unsigned int val;
	struct {
		unsigned major		:8;
		unsigned minor		:8;
		unsigned revision	:8;
		unsigned		:8;
	} bits;
} fimgVersion;

/* Functions */
fimgPipelineStatus fimgGetPipelineStatus(fimgContext *ctx);
int fimgFlush(fimgContext *ctx/*, fimgPipelineStatus pipelineFlags*/);
int fimgInvalidateFlushCache(fimgContext *ctx,
			     unsigned int vtcclear, unsigned int tcclear,
			     unsigned int ccflush, unsigned int zcflush);
void fimgSoftReset(fimgContext *ctx);
fimgVersion fimgGetVersion(fimgContext *ctx);
unsigned int fimgGetInterrupt(fimgContext *ctx);
void fimgClearInterrupt(fimgContext *ctx);
void fimgEnableInterrupt(fimgContext *ctx);
void fimgDisableInterrupt(fimgContext *ctx);
void fimgSetInterruptBlock(fimgContext *ctx, fimgPipelineStatus pipeMask);
void fimgSetInterruptState(fimgContext *ctx, fimgPipelineStatus status);
fimgPipelineStatus fimgGetInterruptState(fimgContext *ctx);

/*
 * Host interface
 */

#define FIMG_ATTRIB_NUM		10

/* Type definitions */
typedef union {
	unsigned int val;
	struct {
		unsigned envb		:1;
		unsigned		:5;
		unsigned idxtype	:2;
		unsigned		:7;
		unsigned autoinc	:1;
		unsigned		:11;
		unsigned envc		:1;
		unsigned numoutattrib	:4;
	} bits;
} fimgHInterface;

#define FGHI_NUMCOMP(i)		((i) - 1)

typedef enum {
	FGHI_ATTRIB_DT_BYTE = 0,
	FGHI_ATTRIB_DT_SHORT,
	FGHI_ATTRIB_DT_INT,
	FGHI_ATTRIB_DT_FIXED,
	FGHI_ATTRIB_DT_UBYTE,
	FGHI_ATTRIB_DT_USHORT,
	FGHI_ATTRIB_DT_UINT,
	FGHI_ATTRIB_DT_FLOAT,
	FGHI_ATTRIB_DT_NBYTE,
	FGHI_ATTRIB_DT_NSHORT,
	FGHI_ATTRIB_DT_NINT,
	FGHI_ATTRIB_DT_NFIXED,
	FGHI_ATTRIB_DT_NUBYTE,
	FGHI_ATTRIB_DT_NUSHORT,
	FGHI_ATTRIB_DT_NUINT,
	FGHI_ATTRIB_DT_HALF_FLOAT
} fimgHostDataType;

/* Functions */
unsigned int fimgGetNumEmptyFIFOSlots(fimgContext *ctx);
void fimgSendToFIFO(fimgContext *ctx, unsigned int count, const void *buffer);
void fimgDrawNonIndexArrays(fimgContext *ctx,
			    unsigned int numVertices,
			    const void **ppvData,
			    unsigned int *pStride);
void fimgDrawArraysUByteIndex(fimgContext *ctx,
			      unsigned int numVertices,
			      const void **ppvData,
			      unsigned int *pStride,
			      const unsigned char *idx);
void fimgDrawArraysUShortIndex(fimgContext *ctx,
			       unsigned int numVertices,
			       const void **ppvData,
			       unsigned int *pStride,
			       const unsigned short *idx);
void fimgSetHInterface(fimgContext *ctx, fimgHInterface HI);
void fimgSetIndexOffset(fimgContext *ctx, unsigned int offset);
void fimgSetVtxBufferAddr(fimgContext *ctx, unsigned int addr);
void fimgSendToVtxBuffer(fimgContext *ctx, unsigned int data);
void fimgSetAttribute(fimgContext *ctx,
		      unsigned int idx,
		      unsigned int type,
		      unsigned int numComp);
void fimgSetVtxBufAttrib(fimgContext *ctx,
			 unsigned char idx, unsigned short base,
			 unsigned char stride, unsigned short range);
void fimgSetAttribCount(fimgContext *ctx, unsigned char count);

/*
 * Primitive Engine
 */

/* Type definitions */
typedef enum {
	FGPE_TRIANGLES		= (1 << 7),
	FGPE_TRIANGLE_FAN	= (1 << 6),
	FGPE_TRIANGLE_STRIP	= (1 << 5),
	FGPE_LINES		= (1 << 4),
	FGPE_LINE_LOOP		= (1 << 3),
	FGPE_LINE_STRIP		= (1 << 2),
	FGPE_POINTS		= (1 << 1),
	FGPE_POINT_SPRITE	= (1 << 0)
} fimgPrimitiveType;

/* Functions */
void fimgSetVertexContext(fimgContext *ctx,
			  unsigned int type,
			  unsigned int count);
void fimgSetViewportParams(fimgContext *ctx,
			   float x0, float y0,
			   float px, float py);
void fimgSetDepthRange(fimgContext *ctx,
		       float n, float f);

/*
 * Raster engine
 */

/* Type definitions */
typedef enum {
	FGRA_BFCULL_FACE_BACK = 0,
	FGRA_BFCULL_FACE_FRONT,
	FGRA_BFCULL_FACE_BOTH = 3
} fimgCullingFace;

typedef union {
	unsigned int val;
	struct {
		unsigned		:8;
		struct {
			unsigned ddy	:1;
			unsigned ddx	:1;
			unsigned lod	:1;
		} coef[8];
	} bits;
} fimgLODControl;

/* Functions */
void fimgSetPixelSamplePos(fimgContext *ctx, int corner);
void fimgEnableDepthOffset(fimgContext *ctx, int enable);
void fimgSetDepthOffsetParam(fimgContext *ctx, float factor, float units);
void fimgSetFaceCullEnable(fimgContext *ctx, int enable);
void fimgSetFaceCullControl(fimgContext *ctx, int bCW,fimgCullingFace face);
void fimgSetYClip(fimgContext *ctx, unsigned int ymin, unsigned int ymax);
void fimgSetLODControl(fimgContext *ctx, fimgLODControl ctl);
void fimgSetXClip(fimgContext *ctx, unsigned int xmin, unsigned int xmax);
void fimgSetPointWidth(fimgContext *ctx, float pWidth);
void fimgSetMinimumPointWidth(fimgContext *ctx, float pWidthMin);
void fimgSetMaximumPointWidth(fimgContext *ctx, float pWidthMax);
void fimgSetCoordReplace(fimgContext *ctx, unsigned int coordReplaceNum);
void fimgSetLineWidth(fimgContext *ctx, float lWidth);

/*
 * Shaders
 */

/* Vertex shader functions */
int fimgLoadVShader(fimgContext *ctx,
		    const unsigned int *pShaderCode, unsigned int numAttribs);
/* Pixel shader functions */
int fimgLoadPShader(fimgContext *ctx,
		    const unsigned int *pShaderCode, unsigned int numAttribs);

/*
 * Texture engine
 */

#define FGTU_MAX_MIPMAP_LEVEL	11

/* Type definitions */
enum {
	FGTU_TSTA_TYPE_2D = 1,
	FGTU_TSTA_TYPE_CUBE,
	FGTU_TSTA_TYPE_3D
};

enum {
	FGTU_TSTA_TEX_EXP_DUP = 0,
	FGTU_TSTA_TEX_EXP_ZERO
};

enum {
	FGTU_TSTA_AFORMAT_ARGB = 0,
	FGTU_TSTA_AFORMAT_RGBA
};

enum {
	FGTU_TSTA_PAL_TEX_FORMAT_1555 = 0,
	FGTU_TSTA_PAL_TEX_FORMAT_565,
	FGTU_TSTA_PAL_TEX_FORMAT_4444,
	FGTU_TSTA_PAL_TEX_FORMAT_8888
};

enum {
	FGTU_TSTA_TEXTURE_FORMAT_1555 = 0,
	FGTU_TSTA_TEXTURE_FORMAT_565,
	FGTU_TSTA_TEXTURE_FORMAT_4444,
	FGTU_TSTA_TEXTURE_FORMAT_DEPTHCOMP16,
	FGTU_TSTA_TEXTURE_FORMAT_88,
	FGTU_TSTA_TEXTURE_FORMAT_8,
	FGTU_TSTA_TEXTURE_FORMAT_8888,
	FGTU_TSTA_TEXTURE_FORMAT_1BPP,
	FGTU_TSTA_TEXTURE_FORMAT_2BPP,
	FGTU_TSTA_TEXTURE_FORMAT_4BPP,
	FGTU_TSTA_TEXTURE_FORMAT_8BPP,
	FGTU_TSTA_TEXTURE_FORMAT_S3TC,
	FGTU_TSTA_TEXTURE_FORMAT_Y1VY0U,
	FGTU_TSTA_TEXTURE_FORMAT_VY1UY0,
	FGTU_TSTA_TEXTURE_FORMAT_Y1UY0V,
	FGTU_TSTA_TEXTURE_FORMAT_UY1VY0
};

enum {
	FGTU_TSTA_ADDR_MODE_REPEAT = 0,
	FGTU_TSTA_ADDR_MODE_FLIP,
	FGTU_TSTA_ADDR_MODE_CLAMP
};

enum {
	FGTU_TSTA_TEX_COOR_PARAM = 0,
	FGTU_TSTA_TEX_COOR_NON_PARAM
};

enum {
	FGTU_TSTA_MIPMAP_DISABLED = 0,
	FGTU_TSTA_MIPMAP_NEAREST,
	FGTU_TSTA_MIPMAP_LINEAR
};

enum {
	FGTU_VTSTA_MOD_REPEAT = 0,
	FGTU_VTSTA_MOD_FLIP,
	FGTU_VTSTA_MOD_CLAMP
};

typedef union {
	unsigned int val;
	struct {
		unsigned		:3;
		unsigned type		:2;
		unsigned		:4;
		unsigned clrKeySel	:1;
		unsigned clrKeyEn	:1;
		unsigned texExp		:1;
		unsigned alphaFmt	:1;
		unsigned paletteFmt	:2;
		unsigned textureFmt	:5;
		unsigned uAddrMode	:2;
		unsigned vAddrMode	:2;
		unsigned pAddrMode	:2;
		unsigned		:1;
		unsigned texCoordSys	:1;
		unsigned magFilter	:1;
		unsigned minFilter	:1;
		unsigned useMipmap	:2;
	} bits;
} fimgTexControl;

typedef struct {
	fimgTexControl	ctrl;
	unsigned int	mipmapLvl;
	unsigned int 	uUSize;
	unsigned int 	uVSize;
	unsigned int 	uPSize;
} fimgTexUnitParams;

typedef union {
	unsigned int val;
	struct {
		unsigned		:20;
		unsigned umod		:2;
		unsigned vmod		:2;
		unsigned usize		:4;
		unsigned vsize		:4;
	} bits;
} fimgVtxTexControl;

/* Functions */
void fimgSetTexUnitParams(fimgContext *ctx, unsigned int unit,
			  fimgTexUnitParams *params);
void fimgSetTexStatusParams(fimgContext *ctx, unsigned int unit,
			    fimgTexControl params);
void fimgSetTexUSize(fimgContext *ctx, unsigned int unit,
		     unsigned int uSize);
void fimgSetTexVSize(fimgContext *ctx, unsigned int unit,
		     unsigned int vSize);
void fimgSetTexPSize(fimgContext *ctx, unsigned int unit,
		     unsigned int pSize);
unsigned int fimgCalculateMipmapOffset(fimgContext *ctx, unsigned int unit,
				       unsigned int uSize, unsigned int vSize,
				       unsigned int maxLevel);
unsigned int fimgCalculateMipmapOffsetYUV(fimgContext *ctx, unsigned int unit,
					  unsigned int uSize, unsigned int vSize,
					  unsigned int maxLevel);
unsigned int fimgCalculateMipmapOffsetS3TC(fimgContext *ctx, unsigned int unit,
					   unsigned int uSize, unsigned int vSize,
					   unsigned int maxLevel);
void fimgSetTexMipmapLevel(fimgContext *ctx, unsigned int unit,
			   int min, int max);
void fimgSetTexBaseAddr(fimgContext *ctx, unsigned int unit,
			unsigned int addr);
void fimgSetTexColorKey(fimgContext *ctx, unsigned int unit,
			unsigned char r, unsigned char g, unsigned char b);
void fimgSetTexColorKeyYUV(fimgContext *ctx, unsigned char u, unsigned char v);
void fimgSetTexColorKeyMask(fimgContext *ctx, unsigned char bitsToMask);
void fimgSetTexPaletteAddr(fimgContext *ctx, unsigned char addr);
void fimgSetTexPaletteEntry(fimgContext *ctx, unsigned int entry);
void fimgSetVtxTexUnitParams(fimgContext *ctx, unsigned int unit,
			     fimgVtxTexControl vts);
void fimgSetVtxTexBaseAddr(fimgContext *ctx, unsigned int unit,
			   unsigned int addr);

/*
 * OpenGL 1.1 compatibility
 */

void fimgLoadMatrix(fimgContext *ctx, unsigned int matrix, float *pData);

/*
 * Per-fragment unit
 */

/* Type definitions */
typedef enum {
	FGPF_TEST_MODE_NEVER = 0,
	FGPF_TEST_MODE_ALWAYS,
	FGPF_TEST_MODE_LESS,
	FGPF_TEST_MODE_LEQUAL,
	FGPF_TEST_MODE_EQUAL,
	FGPF_TEST_MODE_GREATER,
	FGPF_TEST_MODE_GEQUAL,
	FGPF_TEST_MODE_NOTEQUAL
} fimgTestMode;

/**
 *	WORKAROUND
 *	Inconsistent with fimgTestMode due to hardware bug
 */
typedef enum {
	FGPF_STENCIL_MODE_NEVER = 0,
	FGPF_STENCIL_MODE_ALWAYS,
	FGPF_STENCIL_MODE_GREATER,
	FGPF_STENCIL_MODE_GEQUAL,
	FGPF_STENCIL_MODE_EQUAL,
	FGPF_STENCIL_MODE_LESS,
	FGPF_STENCIL_MODE_LEQUAL,
	FGPF_STENCIL_MODE_NOTEQUAL
} fimgStencilMode;

typedef enum {
	FGPF_TEST_ACTION_KEEP = 0,
	FGPF_TEST_ACTION_ZERO,
	FGPF_TEST_ACTION_REPLACE,
	FGPF_TEST_ACTION_INCR,
	FGPF_TEST_ACTION_DECR,
	FGPF_TEST_ACTION_INVERT,
	FGPF_TEST_ACTION_INCR_WRAP,
	FGPF_TEST_ACTION_DECR_WRAP
} fimgTestAction;

typedef enum {
	FGPF_BLEND_EQUATION_ADD = 0,
	FGPF_BLEND_EQUATION_SUB,
	FGPF_BLEND_EQUATION_REVSUB,
	FGPF_BLEND_EQUATION_MIN,
	FGPF_BLEND_EQUATION_MAX
} fimgBlendEquation;

typedef enum {
	FGPF_BLEND_FUNC_ZERO = 0,
	FGPF_BLEND_FUNC_ONE,
	FGPF_BLEND_FUNC_SRC_COLOR,
	FGPF_BLEND_FUNC_ONE_MINUS_SRC_COLOR,
	FGPF_BLEND_FUNC_DST_COLOR,
	FGPF_BLEND_FUNC_ONE_MINUS_DST_COLOR,
	FGPF_BLEND_FUNC_SRC_ALPHA,
	FGPF_BLEND_FUNC_ONE_MINUS_SRC_ALPHA,
	FGPF_BLEND_FUNC_DST_ALPHA,
	FGPF_BLEND_FUNC_ONE_MINUS_DST_ALPHA,
	FGPF_BLEND_FUNC_CONST_COLOR,
	FGPF_BLEND_FUNC_ONE_MINUS_CONST_COLOR,
	FGPF_BLEND_FUNC_CONST_ALPHA,
	FGPF_BLEND_FUNC_ONE_MINUS_CONST_ALPHA,
	FGPF_BLEND_FUNC_SRC_ALPHA_SATURATE
} fimgBlendFunction;

typedef enum {
	FGPF_LOGOP_CLEAR = 0,
	FGPF_LOGOP_AND,
	FGPF_LOGOP_AND_REVERSE,
	FGPF_LOGOP_COPY,
	FGPF_LOGOP_AND_INVERTED,
	FGPF_LOGOP_NOOP,
	FGPF_LOGOP_XOR,
	FGPF_LOGOP_OR,
	FGPF_LOGOP_NOR,
	FGPF_LOGOP_EQUIV,
	FGPF_LOGOP_INVERT,
	FGPF_LOGOP_OR_REVERSE,
	FGPF_LOGOP_COPY_INVERTED,
	FGPF_LOGOP_OR_INVERTED,
	FGPF_LOGOP_NAND,
	FGPF_LOGOP_SET
} fimgLogicalOperation;

typedef enum {
	FGPF_COLOR_MODE_555 = 0,
	FGPF_COLOR_MODE_565,
	FGPF_COLOR_MODE_4444,
	FGPF_COLOR_MODE_1555,
	FGPF_COLOR_MODE_0888,
	FGPF_COLOR_MODE_8888
} fimgColorMode;

/* Functions */
void fimgSetScissorParams(fimgContext *ctx,
			  unsigned int xMax, unsigned int xMin,
			  unsigned int yMax, unsigned int yMin);
void fimgSetScissorEnable(fimgContext *ctx, int enable);
void fimgSetAlphaParams(fimgContext *ctx, unsigned int refAlpha,
			fimgTestMode mode);
void fimgSetAlphaEnable(fimgContext *ctx, int enable);
void fimgSetFrontStencilFunc(fimgContext *ctx, fimgStencilMode mode,
			     unsigned char ref, unsigned char mask);
void fimgSetFrontStencilOp(fimgContext *ctx, fimgTestAction sfail,
			   fimgTestAction dpfail, fimgTestAction dppass);
void fimgSetBackStencilFunc(fimgContext *ctx, fimgStencilMode mode,
			    unsigned char ref, unsigned char mask);
void fimgSetBackStencilOp(fimgContext *ctx, fimgTestAction sfail,
			  fimgTestAction dpfail, fimgTestAction dppass);
void fimgSetStencilEnable(fimgContext *ctx, int enable);
void fimgSetDepthParams(fimgContext *ctx, fimgTestMode mode);
void fimgSetDepthEnable(fimgContext *ctx, int enable);
void fimgSetBlendEquation(fimgContext *ctx,
			  fimgBlendEquation alpha, fimgBlendEquation color);
void fimgSetBlendFunc(fimgContext *ctx,
		      fimgBlendFunction srcAlpha, fimgBlendFunction srcColor,
		      fimgBlendFunction dstAlpha, fimgBlendFunction dstColor);
void fimgSetBlendFuncRGB565(fimgContext *ctx,
			    fimgBlendFunction srcAlpha, fimgBlendFunction srcColor,
			    fimgBlendFunction dstAlpha, fimgBlendFunction dstColor);
void fimgSetBlendEnable(fimgContext *ctx, int enable);
void fimgSetBlendColor(fimgContext *ctx, unsigned int blendColor);
void fimgSetDitherEnable(fimgContext *ctx, int enable);
void fimgSetLogicalOpParams(fimgContext *ctx, fimgLogicalOperation alpha,
			    fimgLogicalOperation color);
void fimgSetLogicalOpEnable(fimgContext *ctx, int enable);
void fimgSetColorBufWriteMask(fimgContext *ctx, int r, int g, int b, int a);
void fimgSetStencilBufWriteMask(fimgContext *ctx, int back, unsigned int mask);
void fimgSetZBufWriteMask(fimgContext *ctx, int enable);
void fimgSetFrameBufParams(fimgContext *ctx,
			   int opaqueAlpha, unsigned int thresholdAlpha,
			   unsigned int constAlpha, fimgColorMode format);
void fimgSetZBufBaseAddr(fimgContext *ctx, unsigned int addr);
void fimgSetColorBufBaseAddr(fimgContext *ctx, unsigned int addr);
void fimgSetFrameBufWidth(fimgContext *ctx, unsigned int width);

/*
 * OS support
 */

void *fimgAllocMemory(fimgContext *ctx,
		      unsigned long *size, unsigned long *paddr);
void fimgFreeMemory(fimgContext *ctx,
		    void *vaddr, unsigned long paddr, unsigned long size);
fimgContext *fimgCreateContext(void);
void fimgDestroyContext(fimgContext *ctx);
void fimgRestoreContext(fimgContext *ctx);

//=============================================================================

#ifdef __cplusplus
}
#endif

#endif /* _FIMG_H_ */