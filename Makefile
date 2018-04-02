borntro: main.cc xmodelib.cc texturelib.cpp idscroller.cc timerlib.cpp    
	@echo Compiling Insolit Dust BoRnTrO...
	g++ -o borntro -w -g -s -O3 -march=native main.cc xmodelib.cc texturelib.cpp idscroller.cc timerlib.cpp `sdl-config --static-libs --cflags` -lSDL_mixer
				
	@echo Build finished. Run ./borntro
