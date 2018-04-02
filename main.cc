#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <math.h>
#include <unistd.h>

#include "texturelib.h"
#include "xmodelib.h"
#include "idscroller.h"
#include "timerlib.h"

SDL_TimerID MyTimer;					//ID of my timer
VgaXMode *Video;
IDScroller *Font = NULL;
unsigned char *DBuf;
static Mix_Music *module = 0;
static bool OkAudio = true;
int Effect = 0;

char FULLSCREEN = 1;

///////////////////////////////////////////////////////////////////////

#include "warp.h"
#include "unwater.h"
#include "flubber.h"

void SetUp_Video(void)
{
  //Enable video
  Video = new VgaXMode(FULLSCREEN);
  DBuf = Video->GetVideoMem();
  
  SDL_WM_SetCaption("InsolitDust BoRnTrO 2012", "borntro");
  
  Font = new IDScroller("ID_Font_002_32x31.pcx", Video, 32, 31);
  
  Font->YPosScroller = 208;
  
  Font->CreateLongString("THIS IS THE FIRST INSOLIT DUST PRODUCTION! THIS SIMPLE BORNTRO AS THE PURPOSE OF PROMOTING THE BIRTH OF A NEW OLDSCHOOL GROUP! ALL GRAPHICS YOU SEE IS IN 320X240 PIXELS AT 256 COLORS. FOR SOURCECODE OF THIS BORNTRO AND MANY OTHER EFFECT PLEASE VISIT INSOLITDUST.SOURCEFORGE.NET! AND REMEMBER, 320X240X256 COLORS ARE ENOUGH FOR EVERYTHING... 3D HARDWARE SUCKS!! P.S. THE MUSIC IS 'THE NADA ONE' BY ANDREAS VIKLUND FROM MODARCHIVE.COM. ESC TO QUIT...");
}

Uint32 Demo(Uint32 interval, void *flagp)
{
  static unsigned long long Counter = 0;
  
  Video->CheckLock();  
  
  switch (Effect)
  {
    case 		0:
      if (Video->FadeIn(2))
		Effect++;
      
      break;
      
    case		1:
      Counter++;
      
      if (Counter >= 45)
      {
		  Effect++;
		  
		  Counter = 0;

		  Mix_VolumeMusic(128);
		  
		  Mix_PlayMusic(module, -1);
      }
      
      break;
      
    case		2:
      if (Video->FadeOut(2))
      {
		Effect++;
		
		Counter = 0;
		
		memset(DBuf, 0, 320 * 240);
	  }
      
      break;
      
    case		3:    
		if (Counter == 0)
		{
			
			
			Init_Warp();
			
			Counter++;
		}
		
		if (Counter > 0 && Counter <= 5)
		{
			Video->ClearToColor(255);
			
			Counter++;
		}
		else
		{
			Video->Clear();
			
			Do_Warp(DBuf);
			
			Effect++;
			Counter = 0;
		}
			
		break;
		
	case		4:
		if (Counter == 0)
			Init_UnderWater();
		
		Counter++;
		
		Video->Clear();
		
		Do_Warp(DBuf);
		
		if (Counter > 40 && Counter <= 45)
		{
			Video->ClearToColor(255);
			
			Video->Update();
			
			return interval;
		}		
		
		if (Counter > 46)
			Do_Flubber(DBuf);
			
		if (Counter > 210 && Counter <= 215)
		{
			Video->ClearToColor(255);
			
			Video->Update();
			
			return interval;
		}		
			
		if (Counter > 216)
		    Draw_Underwater_Logo(DBuf);
		
		if (Counter > 560 && Counter <= 565)
		{
			Video->ClearToColor(255);
			
			Video->Update();
			
			return interval;
		}
		
		if (Counter > 566)
		{
			Font->PutTransString(16, 110, "OLDSCHOOL", 0);
			Font->PutTransString(0, 141, "PRODUCTION", 0);
		}
		
		if (Counter > 585 && Counter <= 590)
		{
			Video->ClearToColor(255);
			
			Video->Update();
			
			return interval;
		}
		
		if (Counter > 591)
		  if (Font->TransparentScroll(4, 0))
		    Font->ResetColumn();
		
		break;
  }
  
  Video->Update();

  return interval;
}

void MainLoop(void)
{
  char done = 0;
  SDL_Event event;

  while (!done)
  {
    usleep(15000);
		
    if (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
	case SDL_KEYDOWN:
	  if (event.key.keysym.sym == SDLK_ESCAPE)
	    done = 1;
	  break;

	case SDL_QUIT:
	  done = 1;
	  break;
      }
    }
  }
}

void Close_Audio(void)
{
  if (OkAudio == false)
    return;
  
  if (Mix_PlayingMusic())
  {
    Mix_FadeOutMusic(500);
    
    while (Video->FadeOut(2) == 0);
  }
  
  if (module)
  {
    Mix_FreeMusic(module);
    
    module = 0;
  }
  
  Mix_CloseAudio();
}

void MyExit(void)
{
  Close_Audio();
	
  SDL_Quit(); 
}

void Init_IDLogo(void)
{
  Texture *IDLogo;
  PcxPalette *tmppal;
  int ind;
  
  IDLogo = new Texture("Logo_ID.pcx");
  
  if (IDLogo == NULL)
    exit(-1);
  
  Video->BlackPalette();
  
  tmppal = IDLogo->GetPalette();

  for (ind = 0; ind < 256; ind++)
  {
    Video->SetColour(ind, tmppal->r, tmppal->g, tmppal->b);

    tmppal++;
  }

  Video->Blit(IDLogo->GetImage(), 320 * 240);
  
  delete IDLogo;
}

int main(int argc, char *argv[])
{  
  int flags=0;
  
  if (argc > 1)
  {
	  if (!strcmp("-h", argv[1]) || !strcmp("-help", argv[1]) ||
          !strcmp("--help", argv[1]) || !strcmp("-?", argv[1]) ||
          !strcmp("help", argv[1]))
      {
		  printf("\n\r");
		  printf("BoRnTrO - (c) Insolit Dust 2012\n\r");
		  printf("Calogiuri Enzo Antonio\n\n");		  
		  
		  printf(" -h : help (this)\n");
		  printf(" -window : run in window (not fullscreen)\n\n");
		  
		  exit(0);
	  }
	  
	  if (!strcmp("-window", argv[1]))
		FULLSCREEN = 0;
  }
	
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
  {
    printf("Couldn't initialize SDL! Error:%s\n\r", SDL_GetError());

    exit(-1);
  }
  
  atexit(MyExit);
  
  if (Mix_OpenAudio(22050, AUDIO_S16, 2, 512) < 0)
    OkAudio = false;
  else
	module = Mix_LoadMUS("music.mod");
  
  SetUp_Video();
  
  Init_Flubber();
  
  Init_IDLogo();  

  SDL_Delay(2000); 

  Video->Update();  

  MyTimer = SDL_AddTimer(40, Demo,  &flags);

  MainLoop();
  
  printf("\n\r");
  printf("BoRnTrO - (c) Insolit Dust 2012\n\r");
  printf("Please visit insolitdust.sourceforge.net\n\r");
  printf("\n\r");
	
  return 0;
}
