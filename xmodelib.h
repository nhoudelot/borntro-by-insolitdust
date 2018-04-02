/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * xmodelib.h
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

#ifndef XMODELIB_H
#define XMODELIB_H

#include <SDL/SDL.h>

class VgaXMode
{
	private:
		SDL_Surface *Video_Surface;			//Primary video surface
		SDL_Surface *Video_Memory;			//320x240 surface
		unsigned int *PixTab;				//Precalculated Y offset table
		unsigned char *pVideo_Memory;		//Internal pointer to video memory
		SDL_Color Palette[256];				//Main palette
		SDL_Color FadePalette[256];			//Palette used for fading
        char NewFade;                       //Uset to reset new fade effect

	public:
		VgaXMode(char FullScreen);			//Constructor, FullScreen = 1 to go in fullscreen mode
		~VgaXMode();						//Destructor

		//Show video memory on screen
		void Update();
		//Put a pixel on screen
        void PutPixel(int x, int y, unsigned char col);
		//Read a pixel from screen
        unsigned char GetPixel(int x, int y);
		//Draw a line on screen
        void DrawLine(int x1, int y1, int x2, int y2, unsigned char col);
		//Clear screen to color 0
        void Clear(void);
		//Clear screen to color col
        void ClearToColor(unsigned char col);
		//Set a color into palette
        void SetColour(unsigned char n, unsigned char r, unsigned char g, unsigned char b);
		//Update the video palette
        void UpdatePalette(void);
		//Get a pointer to the actual palette
        SDL_Color *GetPalette(void);
		//Get a pointer to video memory
        unsigned char *GetVideoMem(void);
		//Copy a buffer into video memory
        void Blit(unsigned char *data, unsigned int len);
		//Put a bitmap into video memory
		void BitmapBlit(int desX, int desY, int bitW, int bitH, unsigned char *data);
		//Get a pointer to the precalculated Y offset table
		unsigned int *GetPixTable(void);
		//Check if screen must lock
		void CheckLock(void);
		//Set the actual palette to all backs
		void BlackPalette(void);
		//Fade in from black palette to actual palette
		char FadeIn(unsigned char Speed);
		//Fade out from actual palette to black palette
		char FadeOut(unsigned char Speed);
        //Reset Fade functions to use another fade effect
        void ResetFade(void);
};

#endif