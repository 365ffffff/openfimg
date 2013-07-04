/*
 * libsgl/fglsurface.h
 *
 * SAMSUNG S3C6410 FIMG-3DSE (PROPER) OPENGL ES IMPLEMENTATION
 *
 * Copyrights:	2010 by Tomasz Figa < tomasz.figa at gmail.com >
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _LIBSGL_FGLSURFACE_
#define _LIBSGL_FGLSURFACE_

#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include "types.h"


class FGLContext;

/**
 * Base class representing abstract backing surface (2D buffer).
 * Defines an interface for classes implementing specific surfaces.
 */
class FGLSurface {
protected:
	FGLContext	*ctx;

	virtual bool allocate(FGLContext *ctx) = 0;

public:
	/** GEM handle of the surface. */
	uint32_t	handle;
	/** GEM file descriptor. */
	int		fd;
	/** Offset from start of the GEM object. */
	off_t		offset;
	/** Virtual address of the surface. */
	void		*vaddr;
	/** Size (in bytes) of the surface. */
	size_t		size;

	/** Default constructor creating null surface. */
			FGLSurface() :
				ctx(0),
				handle(0),
				offset(0),
				vaddr(0),
				size(0) {};
	/**
	 * Creates specified surface.
	 * @param p Physical address of the surface.
	 * @param v Virtual address of the surface.
	 * @param s Size of the surface in bytes.
	 */
			FGLSurface(uint32_t h, off_t o,
				   unsigned long s) :
				ctx(0),
				handle(h),
				offset(o),
				vaddr(0),
				size(s) {};
	/** Destroys the surface. */
	virtual		~FGLSurface()
	{
		unbindContext();
		if (fd >= 0)
			close(fd);
	}

	bool bindContext(FGLContext *ctx);
	void unbindContext(void);

	/**
	 * Flushes the surface to memory.
	 * This operation ensures that any writes to the surface has been
	 * finished and written back to the memory. This might include
	 * waiting for native graphics stack, flushing caches, etc.
	 */
	virtual void	flush(void) = 0;
	/**
	 * Locks the surface for exclusive use.
	 * @param usage Flags indicating usage.
	 * @return 0 on success, non-zero on failure.
	 */
	virtual int	lock(int usage = 0) = 0;
	/**
	 * Unlocks the surface.
	 * @return 0 on success, non-zero on failure.
	 */
	virtual int	unlock(void) = 0;

	/**
	 * Checks if the surface is valid.
	 * @return True if the surface is valid, otherwise false.
	 */
	bool	isValid(void) { return handle != 0; };
};

/** A class implementing a surface backed by internally allocated memory. */
class FGLLocalSurface : public FGLSurface {
protected:
	virtual bool allocate(FGLContext *ctx);

public:
	/**
	 * Creates a local surface.
	 * @param size Size of the surface in bytes.
	 */
			FGLLocalSurface(unsigned long size);
	/** Destroys the surface. */
	virtual		~FGLLocalSurface();

	virtual void	flush(void);
	virtual int	lock(int usage = 0);
	virtual int	unlock(void);
};

/** A class implementing a surface backed by external (native) buffer. */
class FGLExternalSurface : public FGLSurface {
public:
	/**
	 * Creates an external surface.
	 * @param v Virtual address of the surface.
	 * @param p Physical address of the surface.
	 * @param s Size of the surface in bytes.
	 */
			FGLExternalSurface(void *v, uint32_t h,
					   off_t o, size_t s);
	/** Destroys the surface. */
	virtual		~FGLExternalSurface();

	virtual void	flush(void);
	virtual int	lock(int usage = 0);
	virtual int	unlock(void);
};

#endif
