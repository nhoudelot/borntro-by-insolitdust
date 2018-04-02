/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * idscroller.h
 * Copyright (C) Calogiuri Enzo Antonio 2012 <Insolit Dust>
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
 * idscroller IS PROVIDED BY Calogiuri Enzo Antonio ``AS IS'' AND ANY EXPRESS
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

#ifndef IDSCROLLER_H
#define IDSCROLLER_H

#include "xmodelib.h"
#include "texturelib.h"

class IDScroller
{
	private:
		VgaXMode *cVideo;
		Texture *Font;
		PcxPalette *Pal;
		int Width;
		unsigned char *VMem;
		unsigned char *FMem;
		unsigned char *LongString;
		unsigned char FontW, FontH;
		short Column;

		void Grab_Letter(unsigned char num, unsigned char *dest);
		void Blit(short Col);
		void transmemcpy(unsigned char *dst, unsigned char *src, int len, unsigned char trans);
		void TransparentBlit(short Col, unsigned char BackColor);

	public:
		short YPosScroller;
		
		IDScroller(const char *fontname, VgaXMode *Video, unsigned char FW, unsigned char FH);
		~IDScroller();

		void CreateLongString(const char *text);
		char Show(short Col);
		char TransparentShow(short Col, unsigned char BackColor);
		char Scroll(unsigned char Increment);
		char TransparentScroll(unsigned char Increment, unsigned char BackColor);
		void ResetColumn(void);
		PcxPalette *ReturnFontPalette(void);
		void PutTransString(short x, short y, const char *text, unsigned char trans);
};

#endif
