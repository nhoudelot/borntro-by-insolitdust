/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * idscroller.cc
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

#include "idscroller.h"

IDScroller :: IDScroller(const char *fontname, VgaXMode *Video,
                         unsigned char FW, unsigned char FH)
{
	if (Video == NULL)
		return;
	
	Font = new Texture(fontname);
	Pal = NULL;
	cVideo = NULL;
	VMem = NULL;
	FMem = NULL;
	FontW = 0;
	FontH = 0;
	LongString = NULL;
	YPosScroller = 0;
	Column = 0;

	if (Font != NULL)
	{
		Pal = Font->GetPalette();		

		cVideo = Video;
		
		VMem = cVideo->GetVideoMem();
		FMem = Font->GetImage();

		FontW = FW;
		FontH = FH;
	}
}

IDScroller :: ~IDScroller()
{
	if (Font != NULL)
	{
		delete Font;

		if (LongString != NULL)
			delete LongString;
	}
}

void IDScroller :: Grab_Letter(unsigned char num, unsigned char *dest)
{
	unsigned char *start;
	unsigned char *fnt = FMem;
	register int x, y, pos = 0;
	
	if (FMem == NULL)
		return;

	fnt += ((num - 32) * FontW);
	start = fnt;

	for (y = 0; y < FontH; y++)
	{
		for (x = 0; x < FontW; x++)
			dest[pos++] = *fnt++;

		fnt = start + (Font->GetXSize() * (y + 1));
	}
}

void IDScroller :: Blit(short Col)
{
	register int i;
	register unsigned char *buf = VMem + (320 * YPosScroller);
	register unsigned char *dest = LongString + Col;

	for (i = 0; i < FontH; i++)
	{
		memcpy(buf, dest, 320);

		buf += 320;
		dest += Width;
	}
}

void IDScroller :: transmemcpy(unsigned char *dst, unsigned char *src,
                               int len, unsigned char trans)
{
	register int i;

	for (i = 0; i < len; i++)
	{
		if (*src != trans)
			*dst = *src;

		src++;
		dst++;
	}
}

void IDScroller :: TransparentBlit(short Col, unsigned char BackColor)
{
	register int i;
	register unsigned char *buf = VMem + (320 * YPosScroller);
	register unsigned char *dest = LongString + Col;

	for (i = 0; i < FontH; i++)
	{
		transmemcpy(buf, dest, 320, BackColor);

		buf += 320;
		dest += Width;
	}
}

void IDScroller :: CreateLongString(const char *text)
{
	int emptychars = 320 / FontW;
	int reallen = strlen(text) + (emptychars * 2);
	int i, y, x, pos = 0;
	unsigned char *tmp, *start;
	char *linea;
	
	if (LongString != NULL)
		delete LongString;

	tmp = new unsigned char [FontW * FontH];
	Width = FontW * reallen;

	linea = new char [reallen];

	for (i = 0; i < emptychars; i++)
		linea[pos++] = ' ';

	for (i = 0; i < strlen(text); i++)
		linea[pos++] = text[i];

	for (i = 0; i < emptychars; i++)
		linea[pos++] = ' ';

	LongString = new unsigned char [(FontW * reallen) * FontH];

	for (i = 0; i < reallen; i++)
	{
		pos = 0;

		Grab_Letter(linea[i], tmp);

		start = LongString + (i * FontW);

		for (y = 0; y < FontH; y++)
		{
			for(x = 0; x < FontW; x++)
				*start++ = tmp[pos++];

			start = LongString + (i * FontW) + ((FontW * reallen) * (y + 1));
		}
	}

	delete tmp;
	delete linea;
}

char IDScroller :: Show(short Col)
{
	if (LongString == NULL)
		return 1;
	
	if (Col < Width - 320)
	{
		cVideo->CheckLock();

		Blit(Col);

		cVideo->Update();

		Column = Col;

		return 0;
	}
	else
		return 1;
}

char IDScroller :: TransparentShow(short Col, unsigned char BackColor)
{
	if (LongString == NULL)
		return 1;
	
	if (Col < Width - 320)
	{
		cVideo->CheckLock();

		TransparentBlit(Col, BackColor);

		cVideo->Update();

		Column = Col;

		return 0;
	}
	else
		return 1;
}

char IDScroller :: Scroll(unsigned char Increment)
{
	if (LongString == NULL)
		return 1;
	
	if (Column < Width - 320)
	{
		cVideo->CheckLock();

		Blit(Column);

		cVideo->Update();

		Column += Increment;

		return 0;
	}
	else
		return 1;
}

char IDScroller :: TransparentScroll(unsigned char Increment,
                                     unsigned char BackColor)
{
	if (LongString == NULL)
		return 1;	
	
	if (Column < Width - 320)
	{
		//cVideo->CheckLock();

		TransparentBlit(Column, BackColor);

		//cVideo->Update();

		Column += Increment;

		return 0;
	}
	else
		return 1;
}

void IDScroller :: ResetColumn(void)
{
	Column = 0;
}

PcxPalette *IDScroller :: ReturnFontPalette(void)
{
	return Pal;
}

void IDScroller :: PutTransString(short x, short y, const char *text, unsigned char trans)
{
  int i, lx, ly, pos, px, py;
  unsigned char *tmp;
  
  tmp = new unsigned char [FontW * FontH];
  
  for (i = 0; i < strlen(text); i++)
  {
    pos = 0;
    
    Grab_Letter(text[i], tmp);
    
    lx = x + (i * FontW);
    ly = y;
    
    for (py = ly; py < ly + FontH; py++)
      for (px = lx; px < lx + FontW; px++)
      {
		if (tmp[pos] != trans)
			cVideo->PutPixel(px, py, tmp[pos]);
	
		pos++;
      }
  }
  
  delete tmp;
}
