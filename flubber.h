#ifndef ID_FLUBBER_H
#define ID_FLUBBER_H

#define SPANS				32
#define SPANMASK			SPANS - 1

struct points {
	float x, z;
};

TIMER *Timer = NULL;
Texture *Flub = NULL;
unsigned char *pFlub;
unsigned int *pPixTable;

long text_offs;
unsigned char *light = NULL, *lut = NULL;
unsigned short *zbuffer = NULL;
unsigned char *Buffer = NULL;

points pts[240][SPANS], nrm[240][SPANS];

///////////////////////////////////////////////////////////////////////

static void DrawSpan(int y, int x1, int tx1, int px1, int z1,
                            int x2, int tx2, int px2, int z2)
{
	int dx, dtx, dpx, dz, texel, lumel;
	long offs;
	register int i;
	
	if (x1 >= x2)
		return;
		
	dx = x2 - x1;
	dtx = (tx2 - tx1) / dx;
	dpx = (px2 - px1) / dx;
	dz = (z2 - z1) / dx;
	
	offs = pPixTable[y] + x1;
	
	for (i = 0; i < dx; i++)
	{
		if (z1 < zbuffer[offs])
		{
			texel = pFlub[(((y + text_offs) << 8) & 0xff00) + ((tx1 >> 16) & 0xff)];
			lumel = light[(px1 >> 16) & 0xff];
			
			Buffer[offs] = lut[(texel << 8) + lumel];
			
			zbuffer[offs] = z1;
		}
		
		px1 += dpx;
		tx1 += dtx;
		z1 += dz;

		offs++;
	}
}

unsigned char Closest_Colour(unsigned char *pal, unsigned char r,
                             unsigned char g, unsigned char b)
{
	long dist = 1 << 30, newDist;
	unsigned char index = 0;
	int i;
	
	for (i = 0; i < 256; i++)
	{
		newDist = (r - pal[i * 3]) * (r - pal[i * 3]) +
				  (g - pal[i * 3 + 1]) * (g - pal[i * 3 + 1]) +
				  (b - pal[i * 3 + 2]) * (b - pal[i * 3 + 2]);
				  
		if (newDist == 0)
			return i;
			
		if (newDist < dist)
		{
			index = i;
			dist = newDist;
		}
	}
	
	return index;
}

void Compute_LUT(unsigned char *pal)
{
	unsigned char r, g, b;
	int i, j;
	
	for (j = 0; j < 256; j++)
	{
		r = pal[j * 3];
   		g = pal[j * 3 + 1];
   		b = pal[j * 3 + 2];
   		
   		for (i = 0; i < 240; i++)
			lut[(j << 8) + i] = Closest_Colour(pal, r * (120 + i) / 360,
			                                        g * (120 + i) / 360,
			                                        b * (120 + i) / 360);
			                                        
		for (i = 0; i < 16; i++)
			lut[(j << 8) + 240 + i] = Closest_Colour(pal, r + (255 - r) * i / 23,
			                                              g + (255 - g) * i / 23,
			                                     	      b + (255 - b) * i / 23);
	}
}

void Build_Light(void)
{
	int i, c;
	
	light = new unsigned char[256];
	
	for (i = 0; i < 256; i++)
	{
		c = abs(255 - i * 2);
		
		if (c > 255)
			c = 255;
			
		light[i] = 255 - c;
	}
}

void Build_Points(void)
{
	int i, j;
	float angle, nx, nz, ca, sa;
	float inorm;
	
	for (i = 0; i < SPANS; i++)
	{
		angle = (float)i * M_PI * 2 / SPANS;
		ca = cos(angle);
		sa = sin(angle);
		
		for (j = 0; j < 240; j++)
		{
			pts[j][i].x = 24.0 * ca;
			pts[j][i].z = 110.0 * sa;
		}
	}
	
	for (i = 0; i < SPANS; i++)
		for (j = 0; j < 240; j++)
		{
			nx = pts[j][(i + 1) & SPANMASK].x - pts[j][(i - 1) & SPANMASK].x;
			nz = pts[j][(i + 1) & SPANMASK].z - pts[j][(i - 1) & SPANMASK].z;
			inorm = 1 / sqrt(nx*nx + nz*nz);
			
			nx *= inorm;
			nz *= inorm;

			nrm[j][i].x = nx;
			nrm[j][i].z = nz;
		}
}

void Init_Flubber(void)
{
	Flub = new Texture("flub.pcx");
	
	if (Flub == NULL)
		exit(-1);
		
	pFlub = Flub->GetImage();
	pPixTable = Video->GetPixTable();
		
	Build_Light();
	
	Buffer = new unsigned char [320*240];
	lut = new unsigned char [256*256];
	
	Compute_LUT((unsigned char *)Flub->GetPalette());
	
	zbuffer = new unsigned short [320*240];
	
	Build_Points();
	
	Timer = new TIMER;
}

void DeInit_Flubber(void)
{
	if (Buffer != NULL)
		delete Buffer;
		
	if (lut != NULL)
		delete lut;
		
	if (zbuffer != NULL)
		delete zbuffer;	
}

static void transmemcpy(unsigned char *dst, unsigned char *src,
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

void Do_Flubber(unsigned char *dest)
{
	long long currentTime;
	register int i, j;
	float angle, ca, sa;
	register float p1, p2, p3, p4;
	long xoffs, tmp;
	int xc[SPANS], zc[SPANS], nc[SPANS];
	
	currentTime = Timer->GetCount();

	memset(zbuffer, 255, (320*240) * sizeof(unsigned short));

	text_offs = currentTime / 21456;
	
	memset(Buffer, 0, 320*240);
	
	p1 = (float)currentTime / 1948613;
	
	p2 = cos((float)currentTime / 3179640);
		 
	p3 = sin((float)currentTime / 2714147);
	
	p4 = cos((float)currentTime / 2945174);
	
	for (i = 0; i < 240; i++)
	{
		angle = p1 + 3 * M_PI * (p2 * cos(-(float)i / 213) * p3 + 1);
				 
		ca = 256 * cos(angle);
		sa = 256 * sin(angle);
		xoffs = (long)(40960 + 10240 * p4 * sin((float)i / 79));
		           					   
		for (j = 0; j < SPANS; j++)
		{
			xc[j] = xoffs + (long)(pts[i][j].x * ca + pts[i][j].z * -sa);
			zc[j] = (long)(pts[i][j].x * sa + pts[i][j].z * ca);
			
			tmp = (long)(nrm[i][j].x * ca + nrm[i][j].z * -sa);
			tmp = 256 - tmp;
			
			nc[j] = (128 << 16) + (tmp << 15);
		}
		
		for (j = 0; j < SPANS; j++)
			DrawSpan(i, xc[j] >> 8, j << 20, nc[j], 49152 + zc[j],
                        xc[(j + 1) & SPANMASK] >> 8, (j + 1) << 20,
                        nc[(j + 1) & SPANMASK], 49152 + zc[(j + 1) & SPANMASK]);
	}
	
	transmemcpy(dest, Buffer, 320*240, 0);
}

#endif

