#include "game.hpp"

bool Game::init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow("Project War", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
				{
					printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

bool Game::loadMedia()
{
	//Loading success flag
	bool success = true;
	gTexture.push_back(loadTexture("assets/mainscreen.png"));
	gTexture.push_back(loadTexture("assets/instructions.png"));
	gTexture.push_back(loadTexture("assets/map.png"));
	gTexture.push_back(loadTexture("assets/win.png"));
	gTexture.push_back(loadTexture("assets/lose.png"));
	gTexture.push_back(loadTexture("assets/options.png"));
	gTexture.push_back(loadTexture("assets/pause.png"));
	gTexture.push_back(loadTexture("assets/loading.png"));

	assets = loadTexture("assets/assets.png");
	// gTexture = loadTexture("assets/map.png");
	bgMusic = Mix_LoadMUS("assets/beat.mp3");
	if (assets == NULL)
	{
		printf("Unable to run due to error: %s\n", SDL_GetError());
		success = false;
	}

	if (bgMusic == NULL)
	{
		printf("Unable to load music: %s \n", Mix_GetError());
		success = false;
	}
	return success;
}

void Game::close()
{
	//Free loaded images
	SDL_DestroyTexture(assets);
	assets = NULL;
	for (auto i : gTexture)
		SDL_DestroyTexture(i);

	//Destroy window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;
	Mix_FreeMusic(bgMusic);
	bgMusic = NULL;
	//Quit SDL subsystems
	IMG_Quit();
	Mix_Quit();
	SDL_Quit();
}

SDL_Texture *Game::loadTexture(std::string path)
{
	//The final texture
	SDL_Texture *newTexture = NULL;

	//Load image at specified path
	SDL_Surface *loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}
void Game::run()
{
	bool quit = false;
	SDL_Event e;
	// GameScreen ProjectWar(gRenderer,assets);
	Screens.push_back(new MainScreen(gRenderer, assets));
	Screens.push_back(new Instructions(gRenderer, assets));
	Screens.push_back(new GameScreen(gRenderer, assets));
	Screens.push_back(new Win(gRenderer, assets));
	Screens.push_back(new Lose(gRenderer, assets));
	Screens.push_back(new Options(gRenderer, assets));
	Screens.push_back(new Pause(gRenderer, assets));
	Screens.push_back(new Loading(gRenderer, assets));

	while (!quit)
	{
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{

			//User requests quit
			if (e.type == SDL_QUIT || screenNumber == 8)
			{
				quit = true;
				break;
			}

			if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				int xMouse, yMouse;
				SDL_GetMouseState(&xMouse, &yMouse);
				Screens[screenNumber]->detectClick(xMouse, yMouse);
			}
		}
		if (quit)
			break;
		if (Mix_PlayingMusic() == 0)
		{
			//Play the music
			Mix_PlayMusic(bgMusic, 2);
		}
		SDL_RenderClear(gRenderer);									   //removes everything from renderer
		SDL_RenderCopy(gRenderer, gTexture[screenNumber], NULL, NULL); //Draws background to renderer
		//***********************draw the objects here********************
		screenNumber = Screens[screenNumber]->drawObjects();
		if (screenNumber == 3 || screenNumber == 4 || screenNumber == 0)
		{
			delete (Screens[2]);
			Screens[2] = new GameScreen(gRenderer, assets);
		}
		if (screenNumber == 2)
		{
			delete (Screens[3]);
			delete (Screens[4]);
			delete (Screens[6]);
			Screens[6] = new Pause(gRenderer, assets);
			Screens[3] = new Win(gRenderer, assets);
			Screens[4] = new Lose(gRenderer, assets);
		}
		if (screenNumber == 1 || screenNumber == 5 || screenNumber == 6 || screenNumber == 3 || screenNumber == 4)
		{
			delete Screens[0];
			Screens[0] = new MainScreen(gRenderer, assets);
		}
		if (screenNumber == 0)
		{
			delete (Screens[1]);
			delete (Screens[5]);
			delete (Screens[7]);
			Screens[1] = new Instructions(gRenderer, assets);
			Screens[5] = new Options(gRenderer, assets);
			Screens[7] = new Loading(gRenderer, assets);
		}
		//****************************************************************
		SDL_RenderPresent(gRenderer); //displays the updated renderer

		SDL_Delay(50); //causes sdl engine to delay for specified miliseconds
	}
}
