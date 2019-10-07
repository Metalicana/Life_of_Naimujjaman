#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#define ekhon e.key.keysym.sym
const int SW = 1280;
const int SH = 720;
const int walking = 9;
const int button_animation = 7;
const int button_height = 119;
const int button_width = 323;
const int play_button_x =450,play_button_y = 400;
const int quit_button_x = 450,quit_button_y = 500;

SDL_Rect going_up[walking];
SDL_Rect going_down[walking];
SDL_Rect going_left[walking];
SDL_Rect going_right[walking];
SDL_Rect play_button_array[button_animation];
SDL_Rect quit_button_array[button_animation];
SDL_Rect logo_rect;
SDL_Rect road_rect;
class texture_jinish
{
  public:
    texture_jinish();//constructor
    ~texture_jinish();//destructor
    bool loadFromFile(std:: string path);//loads images from pc
    void free();//deallocates stuff
    void setColor(Uint8 red, Uint8 green, Uint8 blue);//color modulation
    void setBlendMode(SDL_BlendMode blending);//blending
    void setAlpha(Uint8 alpha);//alpha modulation
    void render(int x,int y, SDL_Rect *clip =NULL);//texture rendering on a point
    int getW();
    int getH();
  private:
    SDL_Texture *mTexture;
    int mWidth,mHeight;

};
texture_jinish skeleton,play_button,quit_button,logo,road;
bool init();//Initialization
bool loadMedia();//loads media
void close();//memory saving before closing
SDL_Window *main_window = NULL;
SDL_Renderer *main_renderer = NULL;
texture_jinish::texture_jinish()
{
  mTexture = NULL;
  mWidth = 0,mHeight = 0;
}
texture_jinish::~texture_jinish()
{
  free();
}
bool texture_jinish::loadFromFile(std::string path)
{
  free();//free prexesting texture
  SDL_Texture *newTexture = NULL;
  SDL_Surface *loadedSurface = IMG_Load(path.c_str());
  if(loadedSurface == NULL)printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
  else
  {
    SDL_SetColorKey(loadedSurface,SDL_TRUE,SDL_MapRGB(loadedSurface->format,0,0xFF,0xFF));
    newTexture = SDL_CreateTextureFromSurface(main_renderer,loadedSurface);
    if(newTexture == NULL)printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
    else
    {
      mWidth = loadedSurface->w;
      mHeight = loadedSurface->h;
    }
    SDL_FreeSurface(loadedSurface);
  }
  mTexture = newTexture;
  return newTexture != NULL;
}
void texture_jinish::free()
{
  if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}//shortcuts below
void texture_jinish::setColor(Uint8 red,Uint8 blue, Uint8 green)
{
  SDL_SetTextureColorMod(mTexture,red,green,blue);
}
void texture_jinish::setBlendMode(SDL_BlendMode blending)
{
  SDL_SetTextureBlendMode(mTexture,blending);
}
void texture_jinish::setAlpha(Uint8 alpha)
{
  SDL_SetTextureAlphaMod(mTexture,alpha);
}
void texture_jinish::render(int x,int y,SDL_Rect* clip)
{
  SDL_Rect renderQuad = {x,y,mWidth,mHeight};
  if(clip != NULL)
  {
    renderQuad.w = clip->w;
    renderQuad.h = clip->h;
  }
  SDL_RenderCopy(main_renderer,mTexture,clip,&renderQuad);
}
int texture_jinish::getH()
{
  return mHeight;
}
int texture_jinish::getW()
{
  return mWidth;
}
bool init()
{
  bool s = true;
  if(SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
    s = false;
  }
  else
  {
    if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"1"))
    {
      printf( "Warning: Linear texture filtering not enabled!" );
    }
    main_window = SDL_CreateWindow("Life of Naimujjaman",0,0,SW,SH,SDL_WINDOW_SHOWN);
    if(main_window ==NULL)
    {
      printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			s = false;
    }
    else
    {
      main_renderer = SDL_CreateRenderer(main_window,-1,SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
      if(main_renderer == NULL)
      {
        printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				s = false;
      }
      else
      {
        SDL_SetRenderDrawColor(main_renderer,0xFF,0xFF,0xFF,0xFF);
        int imgFlags = IMG_INIT_PNG;
        if(!(IMG_Init(imgFlags) && imgFlags))
        {
          printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					s = false;
        }

      }
    }
  }
  return s;
}
bool loadMedia()
{
  bool s = true;
  if(!skeleton.loadFromFile("skeleton.png") || !logo.loadFromFile("logo.png") || !road.loadFromFile("road.png"))
  {
    printf( "Failed to load walking animation texture!\n" );
		s = false;
  }
  else
  {
    if(!play_button.loadFromFile("play_btn_sprite.png") || !quit_button.loadFromFile("quit_btn_sprite.png") )
    {
        printf( "Failed to load play button texture!\n" );
    		s = false;
    }
    else
    {
      road_rect.x = 0;
      road_rect.y = 0;
      road_rect.h =720;
      road_rect.w = 600;
      logo_rect.x = 150;
      logo_rect.y = 48;
      logo_rect.h =243-48;
      logo_rect.w = 488-150;
      int hx = 17,hy = 15,inter = 64;
      int hh=46,ww=30;
      for(int w=0;w<walking;w++)
      {
        going_up[w].x = hx;
        going_up[w].y = hy;
        going_up[w].h=hh;
        going_up[w].w = ww;
        hx += inter;
      }
      hx = 19,hy=79,inter=64;
      hh=47,ww=24;
      for(int w=0;w<walking;w++)
      {
        going_left[w].x = hx;
        going_left[w].y = hy;
        going_left[w].h=hh;
        going_left[w].w = ww;
        hx += inter;
      }
      hx = 17,hy=143,inter=64;
      hh=47,ww=30;
      for(int w=0;w<walking;w++)
      {
        going_down[w].x = hx;
        going_down[w].y = hy;
        going_down[w].h=hh;
        going_down[w].w = ww;
        hx += inter;
      }
      hx = 19,hy=207,inter=64;
      hh=47,ww=24;
      for(int w=0;w<walking;w++)
      {
        going_right[w].x = hx;
        going_right[w].y = hy;
        going_right[w].h=hh;
        going_right[w].w = ww;
        hx += inter;
      }
      hx = 0,hy=0,inter=363,hh=119,ww=360;
      for(int w=0;w<button_animation;w++)
      {
        play_button_array[w].x = hx;
        play_button_array[w].y = hy;
        play_button_array[w].h=hh;
        play_button_array[w].w = ww;
        hx += inter;
      }
      hx = 0,hy=0,inter=363,hh=119,ww=360;
      for(int w=0;w<button_animation;w++)
      {
        quit_button_array[w].x = hx;
        quit_button_array[w].y = hy;
        quit_button_array[w].h=hh;
        quit_button_array[w].w = ww;
        hx += inter;
      }
    }

  }
  return s;
}
void close()
{
  skeleton.free();
  SDL_DestroyWindow(main_window);
  SDL_DestroyRenderer(main_renderer);
  main_window = NULL;
  main_renderer = NULL;
  IMG_Quit();
  SDL_Quit();
}
int main()
{
  if(!init())printf("Error\n");
  else
  {
    if(!loadMedia())printf("Error\n");
    else
    {
      SDL_Event e;
      int ident=0,f=0,p_f=6,q_f=6;
      int character_x = 390,character_y=600;
      int road_x = 340,road_y_1 = 0,road_y_2=-720;
      //bool switch_it = false;
      int mx,my;
      bool quit = false;
      bool menu=false;
      while(!quit)
      {
        while(SDL_PollEvent(&e))
        {
          if(e.type == SDL_KEYDOWN)
          {
            if(menu)
            {
              f++;
              if(f >= 9)f=0;
              if(ekhon == SDLK_UP)
              {
                ident=0;
                road_y_1+=10;
                road_y_2 +=10;

              }
              else if(ekhon == SDLK_LEFT)
              {
                ident=2;
                character_x-=7;
                //if(x < 0)x=0;
              }
              else if(ekhon == SDLK_RIGHT)
              {
                ident=3;
                character_x+=7;
                //if(x > 1920)y=1920;
              }
              else f=0;
            }
          }
          else if(e.type == SDL_MOUSEBUTTONDOWN)
          {
            SDL_GetMouseState(&mx,&my);
            if(mx >= play_button_x && mx <= play_button_x+ button_width && my >= play_button_y && my <= play_button_y + button_height)
            {
              menu = true;
            }
            else if(mx >= quit_button_x && mx <= quit_button_x + button_width && my >= quit_button_y && my <= quit_button_y + button_height)
            {
              quit = true;
            }
          }
          else if(e.type == SDL_QUIT)
          {
            quit = true;
          }

        }
        if(menu)
        {
          SDL_SetRenderDrawColor( main_renderer, 0xFF, 0xFF, 0xFF, 0xFF );
  				SDL_RenderClear( main_renderer );
          if(road_y_1 >= 720)road_y_1=-720;
          if(road_y_2 >= 720)road_y_2 = -720;
          road.render(road_x,road_y_1,&road_rect);
          road.render(road_x,road_y_2,&road_rect);
          if(ident == 0)skeleton.render(character_x,character_y,going_up+f);
          else if(ident == 1)skeleton.render(character_x,character_y,going_down+f);
          else if(ident == 2)skeleton.render(character_x,character_y,going_left+f);
          else if(ident == 3)skeleton.render(character_x,character_y,going_right+f);
          SDL_RenderPresent(main_renderer);
        }
        else
        {
          SDL_GetMouseState(&mx,&my);
          if(mx >= play_button_x && mx <= play_button_x+ button_width && my >= play_button_y && my <= play_button_y + button_height)
          {
            SDL_Delay(50);
            p_f--;
            if(p_f< 0)p_f = 0;
            q_f++;
            if(q_f >= 6)q_f = 6;
          }
          else if(mx >= quit_button_x && mx <= quit_button_x + button_width && my >= quit_button_y && my <= quit_button_y + button_height)
          {
            SDL_Delay(50);
            q_f--;
            if(q_f < 0)q_f = 0;
            p_f++;
            if(p_f >= 6)p_f=6;
          }
          else
          {
            p_f++;
            q_f++;
            SDL_Delay(50);
            if(p_f>= 6)p_f = 6;
            if(q_f >= 6)q_f = 6;
          }
          SDL_SetRenderDrawColor( main_renderer, 0xFF, 0xFF, 0xFF, 0xFF );
          SDL_RenderClear( main_renderer );
          quit_button.render(quit_button_x,quit_button_y,quit_button_array+q_f);
          play_button.render(play_button_x,play_button_y,play_button_array+p_f);
          logo.render(450,150,&logo_rect);
          SDL_RenderPresent(main_renderer);
        }
      }
    }
  }
  close();
  return 0;
}
