/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * texturelib.cc
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
 * texturelib IS PROVIDED BY Calogiuri Enzo Antonio ``AS IS'' AND ANY EXPRESS
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

#include <stdio.h>
#include <stdlib.h>

#include "texturelib.h"

Texture::Texture(const char *filename)
{
	//Create the class end read the pcs file
  
	FILE *fp;
	short MinX, MinY, MaxX, MaxY;
	unsigned int fsize = 0, NumData;
	unsigned char Data;

	Palette = NULL;
	ImgBuf = NULL;
	XSize = 0;
	YSize = 0;

	fp = fopen(filename, "rb");

	if (fp == NULL)
	{
		printf("No file!\n\r");
		
		return;
	}

	//read pcx header
	fread(Header, 128, 1, fp);

	if (Header[0] != 10)
		return;

	//From header data, build some image info
	Make_Short(Header[4], Header[5], &MinX);
	Make_Short(Header[6], Header[7], &MinY);
	Make_Short(Header[8], Header[9], &MaxX);
	Make_Short(Header[10], Header[11], &MaxY);

	//Now calculate the size of image
	XSize = MaxX - MinX + 1;
	YSize = MaxY - MinY + 1;

	//If size is valid
	if (XSize > 1)
	{
		Palette = (PcxPalette *)malloc(256 * sizeof(PcxPalette));

		//Read palette from end of file
		fseek(fp, -768, SEEK_END);

		fread(Palette, 256 * sizeof(PcxPalette), 1, fp);

		rewind(fp);
		fseek(fp, 128, SEEK_CUR);

		ImgBuf = (unsigned char *)malloc(XSize * YSize);

		//Unpack image
		while (fsize < (XSize * YSize))
		{
			Data = getc(fp);

			if (Data >= 192)
			{
				NumData = Data & 0x3F;
				
				Data = getc(fp);

				while (NumData-- > 0)
					ImgBuf[fsize++] = Data;
			}
			else
				ImgBuf[fsize++] = Data;
		}
	}

	fclose(fp);
}

Texture::~Texture(void)
{
	//Release memory
  
	free(Palette);
	free(ImgBuf);
}

void Texture::Make_Short(unsigned char a, unsigned char b, short *Val)
{
	//Make ad unsigned 16 int from 2 byte
  
	*Val = (a + (b << 8));
}

PcxPalette *Texture::GetPalette(void)
{
	//Return pcx palette
  
	return (Palette);
}

unsigned char *Texture::GetImage(void)
{
	//Return Pcx image data
  
	return (ImgBuf);
}

short Texture::GetXSize(void)
{
	//Return Width of image
  
	return (XSize);
}

short Texture::GetYSize(void)
{
	//Return Height of image
  
	return (YSize);
}