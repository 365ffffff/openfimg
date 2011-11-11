/**
 * libsgl/fglimage.h
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

#ifndef _FGLIMAGE_H_
#define _FGLIMAGE_H_

struct FGLSurface;

struct FGLImage {
	uint32_t pixelFormat;
	uint32_t stride;
	uint32_t height;
	void *buffer;
	FGLSurface *surface;
	bool swapNeeded;
	bool isARGB;
	bool terminated;
	uint32_t refCount;

	FGLImage() : buffer(0), surface(0), terminated(false), refCount(0) {}
	virtual ~FGLImage() {}

	virtual bool isValid() = 0;

	void connect()
	{
		++refCount;
	}

	void disconnect()
	{
		if(--refCount == 0 && terminated)
			delete this;
	}

	void terminate()
	{
		terminated = true;
		if (!refCount)
			delete this;
	}
};

#endif /* _FGLIMAGE_H_ */