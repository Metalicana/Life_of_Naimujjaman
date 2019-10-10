#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <map>
#include <string>
#include <time.h>
#include <stdlib.h>
#define ekhon e.key.keysym.sym
const int SW = 1280;
const int SH = 720;
const int walking = 9;
const int button_animation = 7;
const int button_height = 119;
const int button_width = 323;
const int play_button_x =450,play_button_y = 400;
const int quit_button_x = 450,quit_button_y = 500;
const int fps = 10;

SDL_Rect going_up[walking];
SDL_Rect going_down[walking];
SDL_Rect going_left[walking];
SDL_Rect going_right[walking];
SDL_Rect play_button_array[button_animation];
SDL_Rect quit_button_array[button_animation];
SDL_Rect vehichle_up[8];
SDL_Rect vehichle_down[8];
SDL_Rect logo_rect;
SDL_Rect road_rect;
SDL_Rect side_walk_rect,side_walk_rect_2;
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
    int getW();//fetches the width
    int getH();//fetches the height
  private:
    SDL_Texture *mTexture;//main texture
    int mWidth,mHeight;//main width and height

};
class timer
{
public:
  timer();
  void start();
  void stop();
  void pause();
  void unpause();
  Uint32 getTicks();
  bool is_started();
  bool is_paused();
private:
  Uint32 mStartTicks;
  Uint32 mPausedTicks;
  bool mPaused,mStarted;
};

texture_jinish skeleton,play_button,quit_button,logo,road,side_walk,side_walk_2,bahon;//textures
timer clock_release,clock_move;
bool init();//Initialization
bool loadMedia();//loads media
void close();//memory saving before closing
SDL_Window *main_window = NULL;//main window
SDL_Renderer *main_renderer = NULL;//main renderer
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
timer::timer()
{
  mStartTicks = 0,mPausedTicks=0,mPaused=false,mStarted=false;
}
void timer::start()
{
  mStarted=true,mPaused=false,mStartTicks=SDL_GetTicks(),mPausedTicks=0;
}
void timer::stop()
{
  mStarted = false;
  mPaused = false;
  mStartTicks = 0;
  mPausedTicks=0;
}
void timer::pause()
{
  if(mStarted && !mPaused)
  {
    mPaused = true;
    mPausedTicks = SDL_GetTicks()-mStartTicks;
    mStartTicks=0;
  }
}
void timer::unpause()
{
  if(mStarted && mPaused)
  {
    mPaused = false;
    mStartTicks = SDL_GetTicks()-mPausedTicks;
    mPausedTicks = 0;
  }
}
Uint32 timer::getTicks()
{
  Uint32 time = 0;
  if(mStarted)
  {
    if(mPaused)
    {
      time = mPausedTicks;
    }
    else
    {
      time = SDL_GetTicks()-mStartTicks;
    }
  }
  return time;
}
bool timer::is_started()
{
  return mStarted;
}
bool timer::is_paused()
{
  return mPaused;
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
bool is_colliding(int x1,int y1,int x2,int y2)
{
  int x_1_left = x1;int x_1_right = x1+90;
  int y_1_left = y1+110;int y_1_right = y1;
  int x_2_left = x2;int x_2_right = x2+90;
  int y_2_left = y2+200;int y_2_right = y2;
  int x,y;
  x = std::max(x_1_left, x_2_left);
  int xx = std::min(x_1_right,x_2_right);
  if(x > xx )return false;
  y = std::min(y_1_left,y_2_left);
  int yy = std::max(y_1_right,y_2_right);
  if(y < yy)return false;
  return true;
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
    if(!play_button.loadFromFile("play_btn_sprite.png") || !quit_button.loadFromFile("quit_btn_sprite.png")||!bahon.loadFromFile("car.png") )
    {
        printf( "Failed to load play button texture!\n" );
    		s = false;
    }
    else
    {
      if(!side_walk.loadFromFile("sidewalk_left.png") || !side_walk_2.loadFromFile("sidewalk_right.png"))
      {
        printf( "Failed to load play button texture!\n" );
    		s = false;
      }
      else
      {
        road_rect.x = 0;road_rect.y = 0;road_rect.h =1440;road_rect.w = 600;
        logo_rect.x = 150;logo_rect.y = 48;logo_rect.h =243-48;logo_rect.w = 488-150;
        side_walk_rect.x =0,side_walk_rect.y=0,side_walk_rect.h=1440,side_walk_rect.w=170;
        side_walk_rect_2.x =0,side_walk_rect_2.y=0,side_walk_rect_2.h=1440,side_walk_rect_2.w=170;
        int hx = 0,hy = 0,inter = 90;
        int hh=110,ww=90;
        for(int w=0;w<walking;w++)
        {
          going_up[w].x = hx;
          going_up[w].y = hy;
          going_up[w].h=hh;
          going_up[w].w = ww;
          hx += inter;
        }
        hx = 0,hy=110,inter=90;
        hh=110,ww=90;
        for(int w=0;w<walking;w++)
        {
          going_left[w].x = hx;
          going_left[w].y = hy;
          going_left[w].h=hh;
          going_left[w].w = ww;
          hx += inter;
        }
        hx = 0,hy=220,inter=90;
        hh=110,ww=90;
        for(int w=0;w<walking;w++)
        {
          going_down[w].x = hx;
          going_down[w].y = hy;
          going_down[w].h=hh;
          going_down[w].w = ww;
          hx += inter;
        }
        hx = 0,hy=330,inter=90;
        hh=110,ww=90;
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
      for(int w=0;w<8;w++)
      {
        vehichle_up[w].x = 0,vehichle_up[w].y=0,vehichle_up[w].w=90,vehichle_up[w].h=200;
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
int main(int argc,char *argv[])
{
  if(!init())printf("Error\n");
  else
  {
    if(!loadMedia())printf("Error\n");
    else
    {
      SDL_Event e;
      int ident=0,f=0,p_f=6,q_f=6;
      int side_walk_y_1 = 0,side_walk_y_2=-1440;
      int side_walk_y_3 = 0,side_walk_y_4 = -1440;
      int character_x = 15,character_y=600;
      int road_x = 340,road_y_1 = 0,road_y_2=-1440;
      int mx,my;
      bool quit = false;
      bool menu=false;
      bool marker_up[8];
      int car_up = 0;
      for(int w=0;w<8;w++)marker_up[w]=0;
      int ypos_up[8];
      for(int w=0;w<8;w++)ypos_up[w]=920;

      while(!quit)
      {
        const Uint8 *state = SDL_GetKeyboardState(NULL);
        while(SDL_PollEvent(&e))
        {
          if(e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)
          {
            if(menu)//if we're playing the game, then these conditions apply
            {
              if(state[SDL_SCANCODE_UP])
              {
                ident=0;
                if(character_y <= 360)
                {
                  road_y_1+=fps;
                  road_y_2 +=fps;
                  side_walk_y_1 += fps;
                  side_walk_y_2 += fps;
                  side_walk_y_3 += fps;
                  side_walk_y_4 += fps;
                }
                else character_y -= fps;
                if(state[SDL_SCANCODE_LEFT])
                {
                  ident=2;
                  character_x-=fps;
                }
                else if(state[SDL_SCANCODE_RIGHT])
                {
                  ident=3;
                  character_x+=fps;
                }

              }
              else if(state[SDL_SCANCODE_DOWN])
              {
                character_y += fps;
                if(character_y >= 680)character_y=680;
                ident =1;
                if(state[SDL_SCANCODE_LEFT])
                {
                  ident=2;
                  character_x-=fps;
                }
                else if(state[SDL_SCANCODE_RIGHT])
                {
                  ident=3;
                  character_x+=fps;
                }
              }
              else if(state[SDL_SCANCODE_LEFT])
              {
                ident=2;
                character_x-=fps;
              }
              else if(state[SDL_SCANCODE_RIGHT])
              {
                ident=3;
                character_x+=fps;
              }
              f++;
              if(f >= 9)f=0;
            }
          }

          if(e.type == SDL_MOUSEBUTTONDOWN)
          {
            SDL_GetMouseState(&mx,&my);
            if(mx >= play_button_x && mx <= play_button_x+ button_width && my >= play_button_y && my <= play_button_y + button_height)
            {
              clock_release.start();
              clock_move.start();
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
          //If we start playing the game, this will show
          SDL_SetRenderDrawColor( main_renderer, 0xFF, 0xFF, 0xFF, 0xFF );
  				SDL_RenderClear( main_renderer );
          srand(time(NULL));
           if(car_up >= 8)car_up = 0;
           if(clock_release.getTicks()%1200 == 0 && rand()%15<10)
           {
             marker_up[car_up++]=1;
           }
           for(int w=0;w<8;w++)
           {
             if(is_colliding(character_x,character_y,450,ypos_up[w]))
             {
               menu = false;
             }
           }
           if(clock_move.getTicks()%25==0 || clock_move.getTicks()%25 ==  1)
           {
             for(int w=0;w<8;w++)
             {
               if(marker_up[w])
               {
                 ypos_up[w]-=1;
               }
             }
           }
           //SDL_Delay(1);
           for(int w=0;w<8;w++)
           {
             if(marker_up[w]==1)
             {
               if(character_y-ypos_up[w] > 720)
               {
                 marker_up[w]=0;
                 ypos_up[w]=920;
               }
             }
           }
          if(road_y_1 >= 1440)road_y_1=-1440;
          if(road_y_2 >= 1440)road_y_2 = -1440;
          if(side_walk_y_1 >= 1440)side_walk_y_1=-1440;
          if(side_walk_y_2 >= 1440)side_walk_y_2=-1440;
          if(side_walk_y_3 >= 1440)side_walk_y_3=-1440;
          if(side_walk_y_4 >= 1440)side_walk_y_4=-1440;
          road.render(road_x,road_y_1,&road_rect);
          road.render(road_x,road_y_2,&road_rect);//two road tiling one after another creating an illusion of continuity
          side_walk.render(road_x-170,side_walk_y_1,&side_walk_rect);
          side_walk.render(road_x - 170,side_walk_y_2,&side_walk_rect);
          side_walk_2.render(road_x+600,side_walk_y_3,&side_walk_rect_2);
          side_walk_2.render(road_x+600,side_walk_y_4,&side_walk_rect_2);
          if(character_x < 0)character_x = 0;
          if(character_x > 1280)character_x = 1280;
          if(character_y < 0)character_y = 0;
          if(character_y > 720)character_y = 720;
          if(ident == 0)skeleton.render(character_x,character_y,going_up+f);
          else if(ident == 1)skeleton.render(character_x,character_y,going_down+f);
          else if(ident == 2)skeleton.render(character_x,character_y,going_left+f);
          else if(ident == 3)skeleton.render(character_x,character_y,going_right+f);
          for(int w=0;w<8;w++)
          {
            if(marker_up[w]== 1)bahon.render(450,ypos_up[w],&vehichle_up[w]);
          }
          SDL_RenderPresent(main_renderer);
        }
        else
        {
          SDL_GetMouseState(&mx,&my);//current mouce coordinates
          if(mx >= play_button_x && mx <= play_button_x+ button_width && my >= play_button_y && my <= play_button_y + button_height)
          {
            SDL_Delay(50);
            p_f--;//if we hover over play button then run frames for play animation
            if(p_f< 0)p_f = 0;
            q_f++;
            if(q_f >= 6)q_f = 6;
          }
          else if(mx >= quit_button_x && mx <= quit_button_x + button_width && my >= quit_button_y && my <= quit_button_y + button_height)
          {
            SDL_Delay(50);
            q_f--;//if we hover over quit button then run frames for quit animation
            if(q_f < 0)q_f = 0;
            p_f++;
            if(p_f >= 6)p_f=6;
          }
          else
          {
            p_f++;//we want to increment both frames to the last to bring animation of button to ground state
            q_f++;
            SDL_Delay(50);
            if(p_f>= 6)p_f = 6;
            if(q_f >= 6)q_f = 6;
          }
          //This is the menu windown
          SDL_SetRenderDrawColor( main_renderer, 0xFF, 0xFF, 0xFF, 0xFF );
          SDL_RenderClear( main_renderer );
          quit_button.render(quit_button_x,quit_button_y,quit_button_array+q_f);//quit button render
          play_button.render(play_button_x,play_button_y,play_button_array+p_f);//play button render
          logo.render(450,150,&logo_rect);//logo render
          SDL_RenderPresent(main_renderer);
        }
      }
    }
  }
  close();
  return 0;
}
