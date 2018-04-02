/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * xmodelib.cc
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
 * xmodelib IS PROVIDED BY Calogiuri Enzo Antonio ``AS IS'' AND ANY EXPRESS
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

#include <stdlib.h>
#include <string.h>

#include "xmodelib.h"

#define SWAP(x, y)\
        x = x + y;\
        y = x - y;\
        x = x - y;

VgaXMode::VgaXMode(char FullScreen)
{
	SDL_VideoInfo vinfo;
    Uint32 vmodeflags = 0;
    int i;

	//Set the best resolution mode possible

	vmodeflags = SDL_DOUBLEBUF;

	vinfo = *SDL_GetVideoInfo();

	if (vinfo.hw_available == 0)
		vmodeflags |= SDL_SWSURFACE;
	else
		if (vinfo.blit_sw == 0)
			vmodeflags |= SDL_HWSURFACE;
		else
			vmodeflags |= SDL_SWSURFACE;

	//Enable fullscreen mode if is required

	if (FullScreen)
	{
		vmodeflags |= SDL_FULLSCREEN;

		SDL_ShowCursor(0);
	}

	//Create the 640x480x256 main "window"

	Video_Surface = SDL_SetVideoMode(640, 480, 8, vmodeflags);

	if (Video_Surface == NULL)
		exit(-1);

	//Create the 320x240x256 emulated video mode
	
	Video_Memory = SDL_CreateRGBSurface(vmodeflags, 320, 240, 8, 0, 0, 0, 0);

	if (Video_Memory == NULL)
		exit(-1);

	//Build then Y offset table for speed up video memory read/write 

	PixTab = (unsigned int *)malloc(sizeof(unsigned int) * 240);

	for (i = 0; i < 240; i++)
		PixTab[i] = i * 320;

	//Set internal pointer to video memory

	pVideo_Memory = (unsigned char *)Video_Memory->pixels;
    
    NewFade = 0;
}

VgaXMode::~VgaXMode()
{
	//free resource
	
	SDL_FreeSurface(Video_Surface);
	SDL_FreeSurface(Video_Memory);

	free(PixTab);
}

void VgaXMode::Update()
{
	//Stretch emulated 320x240 surface in main 640x480 window
	
	SDL_Rect srcrect = {0, 0, 320, 240};
    SDL_Rect dstrect = {0, 0, 640, 480};

	SDL_SoftStretch(Video_Memory, &srcrect, Video_Surface, &dstrect);

	if (SDL_MUSTLOCK(Video_Surface))
		SDL_UnlockSurface(Video_Surface);
	else
		SDL_UpdateRect(Video_Surface, 0, 0, 0, 0);
}

void VgaXMode::PutPixel(int x, int y, unsigned char col)
{
	//Put a pixel on screen
	//WARNING!!! No control on input value
	
	pVideo_Memory[PixTab[y] + x] = col;
}

unsigned char VgaXMode::GetPixel(int x, int y)
{
	//Get a pixel from screen
	//WARNING!!! No control on input value
	
	return (pVideo_Memory[PixTab[y] + x]);
}

void VgaXMode::DrawLine(int x1, int y1, int x2, int y2, unsigned char col)
{
	//Draw a line on screen
	//WARNING!!! No control on input value
	
	int dx, dy, cxy, dxy;

	dx = abs(x1 - x2);
    dy = abs(y1 - y2);
    cxy = 0;

	if (dy > dx)
    {
       if (y1 > y2)
       {
          SWAP(y1, y2);
          SWAP(x1, x2);
       }
       
       if (x1 > x2)
          dxy = -1;
       else
          dxy = 1;
          
       for (y1 = y1; y1 < y2; y1++)
       {
           cxy += dx;
           
           if (cxy >= dy)
           {
              x1 += dxy;
              cxy -= dy;
           }
           
           PutPixel(x1, y1, col);
       }
    }
    else
    {
        if (x1 > x2)
        {
           SWAP(x1, x2);
           SWAP(y1, y2);
        }
        
        if (y1 > y2)
           dxy = -1;
        else
           dxy = 1;
           
        for (x1 = x1; x1 < x2; x1++)
        {
            cxy += dy;
            
            if (cxy >= dx)
            {
               y1 += dxy;
               cxy -= dx;
            }
            
            PutPixel(x1, y1, col);
        }
    }
}

void VgaXMode::Clear(void)
{
	//Clear screen
	
	memset(pVideo_Memory, 0, 320 * 240);
}

void VgaXMode::ClearToColor(unsigned char col)
{
	//Clear screen to color col
	
	memset(pVideo_Memory, col, 320 * 240);
}

void VgaXMode::SetColour(unsigned char n, unsigned char r, unsigned char g, unsigned char b)
{
	//Set the color n to r, g, b, value
	
	Palette[n].r = r;
    Palette[n].g = g;
    Palette[n].b = b;
}

void VgaXMode::UpdatePalette(void)
{
	//Enabled palette
	
	SDL_SetColors(Video_Surface, Palette, 0, 256);
}

SDL_Color *VgaXMode::GetPalette(void)
{
	//Return a pointer to actual palette
	
	return (Palette);
}

unsigned char *VgaXMode::GetVideoMem(void)
{
	//Return a pointer to video memory
	
	return (pVideo_Memory);
}

void VgaXMode::Blit(unsigned char *data, unsigned int len)
{
	//fill video memory with a buffer
	
	memcpy(pVideo_Memory, data, len);
}

void VgaXMode::BitmapBlit(int desX, int desY, int bitW, int bitH, unsigned char *data)
{
	//Put a bitmap on screen
	
	int desStartX = 0, desStartY = 0;
	int destEndX = bitW;
	register int tmp;
	unsigned char *pvm, *pdata;

	if (desX < 0)
	{
		desStartX = -1 * desX;

		if (desStartX > bitW)
			desStartX = bitW;

		bitW -= desStartX; 		
		
		desX = 0;
	}

	if (desY < 0)
	{
		desStartY = -1 * desY;

		if (desStartY > bitH)
			desStartY = bitH;

		bitH -= desStartY;
		
		desY = 0;
	}

	if ((desX + bitW) > 320)
	{
		tmp = (desX + bitW) - 320;
		bitW -= tmp;
	}

	if ((desY + bitH) > 240)
	{
		tmp = (desY + bitH) - 240;
		bitH -= tmp;
	}

	pvm = pVideo_Memory + (PixTab[desY] + desX);
	pdata = data + ((desStartY * bitW) + desStartX);

	for (tmp = 0; tmp < bitH; tmp++)
	{
		memcpy(pvm, pdata, bitW);		

		pvm += 320;
		pdata += destEndX;
	}
}

unsigned int *VgaXMode::GetPixTable(void)
{
	//Return a pointer to a Y offset table
	
	return (PixTab);
}

void VgaXMode::CheckLock(void)
{
	//Check if video surface must be lock
	
	if(SDL_MUSTLOCK(Video_Surface))
		if (SDL_LockSurface(Video_Surface) < 0)
			exit(-2);
}

void VgaXMode::BlackPalette(void)
{
	//Create an all blacks palette
	
	int i;

	for (i = 0; i < 256; i++)
	{
		FadePalette[i].r = 0;
    	FadePalette[i].g = 0;
    	FadePalette[i].b = 0;

		SetColour(i, 0, 0, 0);
	}

	UpdatePalette();
}

char VgaXMode::FadeIn(unsigned char Speed)
{
	//Fade in
	
	static char end = 0;
	int i;
	char finish = 0;
    
    if (NewFade == 0)
    {
        end = 0;
        
        NewFade = 1;
    }

	if (end != 0)
		return 1;

	for (i = 0; i < 256; i++)
	{
		if (FadePalette[i].r < Palette[i].r)
		{
			if ((int)(FadePalette[i].r + Speed) <= (int)Palette[i].r)			
				FadePalette[i].r += Speed;
			else
				FadePalette[i].r = Palette[i].r;			

			finish = 1;
		}

		if (FadePalette[i].g < Palette[i].g)
		{
			if ((int)(FadePalette[i].g + Speed) <= (int)Palette[i].g)			
				FadePalette[i].g += Speed;
			else
				FadePalette[i].g = Palette[i].g;			

			finish = 1;
		}

		if (FadePalette[i].b < Palette[i].b)
		{
			if ((int)(FadePalette[i].b + Speed) <= (int)Palette[i].b)			
				FadePalette[i].b += Speed;
			else
				FadePalette[i].b = Palette[i].b;

			finish = 1;
		}
			
	}

	if (finish == 0)
	{
		end = 1;

		return 1;
	}

	SDL_SetColors(Video_Surface, FadePalette, 0, 256);	
	
	return 0;
}

char VgaXMode::FadeOut(unsigned char Speed)
{
	//Fade out
	
	static char end = 0;
	int i;
	char finish = 0;
    
    if (NewFade == 0)
    {
        end = 0;
        
        NewFade = 1;
    }

	if (end != 0)
		return 1;

	for (i = 0; i < 256; i++)
	{
		if (Palette[i].r > 0)
		{
			if ((int)(Palette[i].r - Speed) >= 0)			
				Palette[i].r -= Speed;
			else
				Palette[i].r = 0;
			
			finish = 1;
		}

		if (Palette[i].g > 0)
		{
			if ((int)(Palette[i].g - Speed) >= 0)			
				Palette[i].g -= Speed;
			else
				Palette[i].g = 0;
			
			finish = 1;
		}

		if (Palette[i].b > 0)
		{
			if ((int)(Palette[i].b - Speed) >= 0)			
				Palette[i].b -= Speed;
			else
				Palette[i].b = 0;

			finish = 1;
		}
	}

	if (finish == 0)
	{
		end = 1;

		return 1;
	}

	SDL_SetColors(Video_Surface, Palette, 0, 256);

	return 0;
}

void VgaXMode::ResetFade(void)
{
    NewFade = 0;
}