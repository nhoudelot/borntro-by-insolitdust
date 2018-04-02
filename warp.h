#ifndef ID_WARP_H
#define ID_WARP_H

typedef struct {
	int dx, dy;
} DecXY;

DecXY Sph[120][160];
Texture *WarpImg = NULL;
unsigned char *pWarpImg;

void Init_Warp_Table(void)
{
	int i, j;
	double f, d;
	unsigned short w, z;
	
	for (j = 0; j < 120; j++)
		for (i = 0; i < 160; i++)
		{
			f = pow(i * 1.2 / 320, 2);
			d = log(1 + j / (240 / 3.0)) / (3 * f + 1) * 240 / 2;
			w = (unsigned short)d;
			
			f = pow(j * 1.5 / 320, 2);
			d = log(1 + i / (320 / 3.0)) / (3 * f + 1) * 320 / 2;
			z = (unsigned short)d;
			
			Sph[j][i].dx = w;
			Sph[j][i].dy = z;
		}
}

static unsigned char GetWarpTexturePixel(int x, int y)
{
	return (pWarpImg[(y << 8) + x]);
}

void Do_Warp(unsigned char *dest)
{
	static float alpha = 0, beta = 0, dz = 0, dw = 0;
	int decz = 0, decw = 0;
	register int ddx, ddy, i, j;
	unsigned char *p1, *p2, *p3, *p4;
	
	alpha += 0.16;
    beta += 0.332;
    dz += sin(alpha + beta) * 2 + cos(beta) + 0.4;
    dw += cos(beta - alpha) * 3 + sin(alpha) + 0.2;
    decz = (int)(dz);
    decw = (int)(dw);
    
    for (j = 0; j < 120; j++)
    {
		p1 = dest + 160 + 320 * (120 - j);
        p2 = p1;
        p3 = dest + 160 + 320 * (120 + j);
        p4 = p3;
        
        for (i = 0; i < 160; i++)
        {
			ddx = Sph[j][i].dx;
            ddy = Sph[j][i].dy;
            
            *p1 --= GetWarpTexturePixel((-ddx + decz) & 255, (-ddy + decw) & 255);
             
            *p2 ++= GetWarpTexturePixel((-ddx + decz) & 255, (ddy + decw) & 255);
            
            *p3 --= GetWarpTexturePixel((ddx + decz) & 255, (-ddy + decw) & 255);
             
            *p4 ++= GetWarpTexturePixel((ddx + decz) & 255, (ddy + decw) & 255);
		}
	}
}

void Init_Warp(void)
{
	PcxPalette *pal;
	short ind;
	
	WarpImg = new Texture("back.pcx");
	
	if (WarpImg == NULL)
		exit(-1);
		
	pal = WarpImg->GetPalette();
	
	for (ind = 0; ind < 256; ind++)
	{
		Video->SetColour(ind, pal->r, pal->g, pal->b);
		
		pal++;
	}
	
	Video->UpdatePalette();
	
	pWarpImg = WarpImg->GetImage();
	
	Init_Warp_Table();
}

#endif

