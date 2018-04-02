#ifndef ID_UNWATER_H
#define ID_UNWATER_H

#define YPOSTITLE					2

Texture *Title = NULL;
unsigned char *pTitle;
int tSize;

char Sin[64];

///////////////////////////////////////////////////////////////////////

void Init_UnderWater(void)
{
	unsigned char ind;
	
	Title = new Texture("title.pcx");
	
	if (Title == NULL)
		exit(-1);
		
	for (ind = 0; ind < 64; ind++)
		Sin[ind] = sin((double)ind * M_PI / 16) * 6;
		
	pTitle = Title->GetImage();
	tSize = Title->GetYSize() * 2;
}

void Draw_Underwater_Logo(unsigned char *dest)
{
	static int a;
	register int x, y;
	int b, ind;
	unsigned char col;
	
	a--;
	
	for (y = 0; y < tSize; y++)
		for (x = 0; x < 320; x++)
		{
			b = x - 16 + Sin[(y + a) & 0x3F];
			
			if (b >= 0 && b <= 255)
			{
				ind = ((y - 16 + ((Sin[(x + a) & 0x3F]) >> 1)) << 8) + b;
				
				if (ind >= 0 && ind < 20480)
				{
					col = *(pTitle + ind);
					
					if (col != 0)
					{
						ind = (y + YPOSTITLE) * 320 + (x + 16);
						
						dest[ind] = col;
					}
				}
			}
		}
}

#endif

