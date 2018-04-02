/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * terturelib.h
 * Copyright (C) Calogiuri Enzo Antonio 2011 <Insolit Dust>
 * 
Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name ``Calogiuri Enzo Antonio'' nor the name of any other
 *    contributor may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 * 
 * terturelib IS PROVIDED BY Calogiuri Enzo Antonio ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL Calogiuri Enzo Antonio OR ANY OTHER CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef TEXTURELIB_H
#define TEXTURELIB_H

//Single color structure
typedef struct
{
	unsigned char r,g,b;
} PcxPalette;

class Texture
{
	private:
		unsigned char *ImgBuf;		//Internal pointer to image
		PcxPalette *Palette;		//Internal pointer to palette
		unsigned char Header[128];	//Header of pcx file
		short XSize, YSize;		//Image dimensions

		void Make_Short(unsigned char a, unsigned char b, short *Val);	//Make a short into from 2 byte

	public:
		Texture(const char *filename);		//Constructor
		~Texture(void);				//Destructor

		PcxPalette *GetPalette(void);		//Return a pointer to pcx palette
		unsigned char *GetImage(void);		//Return a pointer to image data
		short GetXSize(void);			//Return Width of image
		short GetYSize(void);			//Return Height of image
};

#endif
