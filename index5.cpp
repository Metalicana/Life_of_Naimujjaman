#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <map>
#include <algorithm>
#include <math.h>
#include <string>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>
#define ekhon e.key.keysym.sym
const int SW = 1280;
const int SH = 720;
const unsigned int totalscorenum = 10;
const int walking = 9;
const int button_animation = 7;
const int coin_animation = 36;
const int button_height = 119;
const int button_width = 323;
const int coin_width = 32;
const int coin_height = 32;
const int play_button_x =450,play_button_y = 350;
const int quit_button_x = 450,quit_button_y = 550;
const int hof_button_x = 450,hof_button_y = 450;
const int fps = 8;
const int car_height = 200;
const int car_width = 90;
const int bus_height = 200;
const int bus_width = 90;
const int dd_height = 200;
const int dd_width = 90;
const int cng_height = 150;
const int cng_width = 90;
const int bike_height =120;
const int bike_width = 50;
const int beka_cng_height = cng_width;
const int beka_cng_width = cng_height;
const int beka_car_height = car_width;
const int beka_car_width = car_height;
const int beka_dd_height = dd_width;
const int beka_dd_width = dd_height;
const int beka_bus_width = bus_height;
const int beka_bus_height = bus_width;
bool in_scoresave = false;
bool played_once = false;
SDL_Rect going_up[walking];
SDL_Rect going_down[walking];
SDL_Rect going_left[walking];
SDL_Rect going_right[walking];
SDL_Rect play_button_rect;
SDL_Rect quit_button_rect;
SDL_Rect hof_button_rect;
SDL_Rect button_shadow_array[button_animation];
SDL_Rect vehichle_up[4];
SDL_Rect vehichle_down[4];
SDL_Rect vehichle_left[4];
SDL_Rect vehichle_right[4];
SDL_Rect bike_up_rect,bike_down_rect;
SDL_Rect coin_rect[coin_animation];
SDL_Rect logo_rect;
SDL_Rect road_rect;
SDL_Rect side_walk_rect,side_walk_rect_2;
SDL_Rect chips_rect,frooto_rect;
SDL_Rect scorename_rect[totalscorenum];
SDL_Rect score_rect[totalscorenum];
SDL_Rect currentscore_rect;
SDL_Rect scoresave_text_rect, scoresave_name_rect;
SDL_Rect hof_bg_rect;
TTF_Font *main_font;
int tempscore = 0;
char tempscore_string[9] = {0};
std::string tempname = "";
class texture_jinish
{
  public:
    texture_jinish();//constructor
    ~texture_jinish();//destructor
    bool loadFromFile(std:: string path);//loads images from
    bool RasteriseText(std::string text);//Rasterises text
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

texture_jinish skeleton,play_button,quit_button,hof_button,button_shadow,logo,road,side_walk,side_walk_2;//textures
texture_jinish gari_up,bus_up,dotola_bus_up,cng_up;
texture_jinish gari_down,bus_down,dotola_bus_down,cng_down;
texture_jinish gari_left,gari_right,bus_left,bus_right,dotola_bus_left,dotola_bus_right,cng_left,cng_right;
texture_jinish chips,frooto,coin;
texture_jinish scorename[10],score[10],currentscore,scoresave_text,scoresave_name;
texture_jinish bike_up,bike_down;
texture_jinish hof_bg;
timer clock_release,clock_move,snacks,beka,tera;
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
bool texture_jinish::RasteriseText(std::string text)
{
  free();//free prexesting texture
  SDL_Color Black = {0, 0, 0};
  SDL_Texture *newTexture = NULL;
  if (text == "") {
    text = " ";
  }
  SDL_Surface *loadedSurface = TTF_RenderText_Blended(main_font, text.c_str(), Black);

  if(loadedSurface == NULL)printf( "Unable to load font %s! SDL_TTF Error: %s\n", text.c_str(), TTF_GetError() );
  else
  {
    newTexture = SDL_CreateTextureFromSurface(main_renderer,loadedSurface);
    if(newTexture == NULL)printf( "Unable to create texture from %s! SDL Error: %s\n", text.c_str(), SDL_GetError() );
    else
    {
      SDL_QueryTexture(newTexture, NULL, NULL, &mWidth, &mHeight);
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
}
//shortcuts below
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

void padded_itoa(int a, char arr[])
{
  for (int i = 0; i < 8; i++) {
    arr[i] = 48;
  }

  for (int j = 0; j < 8; j++) {
    arr[j] = (a % 10) + 48;
    a /= 10;
  }

  arr[8] = 0;

  std::reverse(arr, arr + 8);

}

void updatescore (int tempscore, char tempname[])
{
  unsigned int i = 0, j = 0, k = 0, l = 0;

  FILE *readscore = fopen("savedata/scores.sav", "r");

  char updatescorename_array[totalscorenum + 1][11] = {0};
  char updatescore_array[totalscorenum + 1][10] = {0};
  int updatescore_int_array[totalscorenum + 1] = {0};
  updatescore_int_array[totalscorenum] = tempscore;

  while (i < totalscorenum) {
    fgets(updatescorename_array[i], 12, readscore);
    updatescorename_array[i][strcspn(updatescorename_array[i], "\n")] = 0;
    fgets(updatescore_array[i], 10, readscore);
    updatescore_array[i][strcspn(updatescore_array[i], "\n")] = 0;
    updatescore_int_array[i] = atoi(updatescore_array[i]);
    i++;
  }

  fclose(readscore);

  if (tempscore < updatescore_int_array[totalscorenum - 1]) {
    return;
  }

  std::sort(updatescore_int_array, (updatescore_int_array + totalscorenum + 1), std::greater<int>());

  for (k = (totalscorenum - 1); k >= 0; k--) {
    if (tempscore == updatescore_int_array[k]) {
      for (l = totalscorenum - 2; l > k; l--) {
        strcpy(updatescorename_array[l + 1], updatescorename_array[l]);
      }
      strcpy(updatescorename_array[k], tempname);
      break;
    }
  }

  FILE *edited_scoreboard = fopen("savedata/scores.sav", "w");

  while (j < totalscorenum) {
    //printf("Scanned name: %s\nScanned score: %08d\n", updatescorename_array[j], updatescore_int_array[j]);
    fprintf(edited_scoreboard, "%s\n%08d\n", updatescorename_array[j], updatescore_int_array[j]);
    j++;
  }

  fclose(edited_scoreboard);

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
        else
        {
          if (TTF_Init() == -1)
          {
            printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
            s = false;
          }
        }
      }
    }
  }
  return s;
}
bool loadscores()
{

  bool s = true;
  unsigned int i = 0;

  FILE *scoreboard = fopen("savedata/scores.sav", "r");

  char scorename_array[totalscorenum + 1][11] = {0};
  char score_array[totalscorenum + 1][10] = {0};

  while (i < totalscorenum) {
    fgets(scorename_array[i], 12, scoreboard);
    scorename_array[i][strcspn(scorename_array[i], "\n")] = 0;
    fgets(score_array[i], 10, scoreboard);
    score_array[i][strcspn(score_array[i], "\n")] = 0;
    //fscanf(scoreboard, "%s %s", scorename_array[i], score_array[i]);
    //printf("scorename scanned: %s\n", scorename_array[i]);
    i++;
  }

  fclose(scoreboard);

  for (unsigned int j = 0; j < totalscorenum; j++) {
    s = s & scorename[j].RasteriseText(scorename_array[j]);
    s = s & score[j].RasteriseText(score_array[j]);
  }

  for (unsigned int score_rect_initialiser = 0; score_rect_initialiser < totalscorenum; score_rect_initialiser++) {
    score_rect[score_rect_initialiser].x = 0;
    score_rect[score_rect_initialiser].y = 0;
    score_rect[score_rect_initialiser].w = 250;
    score_rect[score_rect_initialiser].h = 50;
  }

  for (unsigned int scorename_rect_initialiser = 0; scorename_rect_initialiser < totalscorenum; scorename_rect_initialiser++) {
    scorename_rect[scorename_rect_initialiser].x = 0;
    scorename_rect[scorename_rect_initialiser].y = 0;
    scorename_rect[scorename_rect_initialiser].w = scorename[scorename_rect_initialiser].getW();
    scorename_rect[scorename_rect_initialiser].h = scorename[scorename_rect_initialiser].getH();
  }

  return s;

}
bool is_colliding(int x1,int y1,int x2,int y2,int h,int w)
{
  int x_1_left = x1;int x_1_right = x1+90;
  int y_1_left = y1+110;int y_1_right = y1;
  int x_2_left = x2;int x_2_right = x2+w;
  int y_2_left = y2+h;int y_2_right = y2;
  int x,y;
  x = std::max(x_1_left, x_2_left);
  int xx = std::min(x_1_right,x_2_right);
  if(x > xx )return false;
  y = std::min(y_1_left,y_2_left);
  int yy = std::max(y_1_right,y_2_right);
  if(y < yy)return false;
  return true;
}
bool hover(int mx,int my,int btn_x,int btn_y,int btn_height,int btn_width)
{
  if(mx >= btn_x && mx <= btn_x + btn_width && my >= btn_y && my <= btn_y + btn_height)return 1;
  return 0;
}
bool loadMedia()
{
  bool s = true;
  s = s & skeleton.loadFromFile("skeleton.png");
  s = s & logo.loadFromFile("logo.png");
  s = s & road.loadFromFile("road.png");
  s = s & play_button.loadFromFile("play_btn.png");
  s = s & quit_button.loadFromFile("quit_btn.png");
  s = s & hof_button.loadFromFile("hof_btn.png");
  s = s & button_shadow.loadFromFile("btn_shadow_sprite.png");
  s = s & side_walk.loadFromFile("sidewalk_left.png");
  s = s & side_walk_2.loadFromFile("sidewalk_right.png");
  s = s & gari_up.loadFromFile("car1.png");
  s = s & bus_up.loadFromFile("bus1.png");
  s = s & dotola_bus_up.loadFromFile("dd1.png");
  s = s & cng_up.loadFromFile("cng1.png");
  s = s & gari_down.loadFromFile("car2.png");
  s = s & bus_down.loadFromFile("bus2.png");
  s = s & dotola_bus_down.loadFromFile("dd2.png");
  s = s & cng_down.loadFromFile("cng2.png");
  s = s & chips.loadFromFile("chips.png");
  s = s & frooto.loadFromFile("frooto.png");
  s = s & bike_up.loadFromFile("bike1.png");
  s = s & bike_down.loadFromFile("bike2.png");
  s = s & coin.loadFromFile("coinsprites/coinspritesheet.png");
  s = s & hof_bg.loadFromFile("hof_bg.png");
  s = s & gari_left.loadFromFile("car4.png");
  s = s & gari_right.loadFromFile("car3.png");
  s = s & bus_left.loadFromFile("bus4.png");
  s = s & bus_right.loadFromFile("bus3.png");
  s = s & dotola_bus_left.loadFromFile("dd4.png");
  s = s & dotola_bus_right.loadFromFile("dd3.png");
  s = s & cng_left.loadFromFile("cng4.png");
  s = s & cng_right.loadFromFile("cng3.png");
  main_font = TTF_OpenFont("main_font.ttf", 48);

  play_button_rect.x = 0;play_button_rect.y = 0;play_button_rect.h = 119;play_button_rect.w = 363;
  quit_button_rect.x = 0;quit_button_rect.y = 0;quit_button_rect.h = 119;quit_button_rect.w = 363;
  hof_button_rect.x = 0;hof_button_rect.y = 0;hof_button_rect.h = 119;hof_button_rect.w = 363;
  road_rect.x = 0;road_rect.y = 0;road_rect.h =1440;road_rect.w = 600;
  logo_rect.x = 150;logo_rect.y = 48;logo_rect.h =243-48;logo_rect.w = 488-150;
  side_walk_rect.x =0,side_walk_rect.y=0,side_walk_rect.h=1440,side_walk_rect.w=170;
  side_walk_rect_2.x =0,side_walk_rect_2.y=0,side_walk_rect_2.h=1440,side_walk_rect_2.w=170;
  chips_rect.x = 0,chips_rect.y = 0,chips_rect.w =48,chips_rect.h=60;
  frooto_rect.x = 0,frooto_rect.y = 0,frooto_rect.w = 24,frooto_rect.h = 60;
  currentscore_rect.x = 0,currentscore_rect.y = 0,currentscore_rect.w = 250,currentscore_rect.h = 50;
  hof_bg_rect.x = 0,hof_bg_rect.y = 0,hof_bg_rect.w = 1280,hof_bg_rect.h = 720;

        int hx = 0,hy = 0,inter = 90;
        int hh=110,ww=90;
        for(int w=0;w<walking;w++)
        {
          going_up[w].x = hx;
          going_up[w].y = hy;
          going_up[w].h = hh;
          going_up[w].w = ww;
          hx += inter;
        }
        hx = 0,hy=110,inter=90;
        hh=110,ww=90;
        for(int w=0;w<walking;w++)
        {
          going_left[w].x = hx;
          going_left[w].y = hy;
          going_left[w].h = hh;
          going_left[w].w = ww;
          hx += inter;
        }
        hx = 0,hy=220,inter=90;
        hh=110,ww=90;
        for(int w=0;w<walking;w++)
        {
          going_down[w].x = hx;
          going_down[w].y = hy;
          going_down[w].h = hh;
          going_down[w].w = ww;
          hx += inter;
        }
        hx = 0,hy=330,inter=90;
        hh=110,ww=90;
        for(int w=0;w<walking;w++)
        {
          going_right[w].x = hx;
          going_right[w].y = hy;
          going_right[w].h = hh;
          going_right[w].w = ww;
          hx += inter;
        }
        hx = 0,hy=0,inter=363,hh=119,ww=360;
        for(int w=0;w<button_animation;w++)
        {
          button_shadow_array[w].x = hx;
          button_shadow_array[w].y = hy;
          button_shadow_array[w].h = hh;
          button_shadow_array[w].w = ww;
          hx += inter;
        }
        hx = 0,hy=0,inter=coin_width,hh=coin_height,ww=coin_width;
        for(int w=0;w<coin_animation;w++)
        {
          coin_rect[w].x = hx;
          coin_rect[w].y = hy;
          coin_rect[w].h = coin_height;
          coin_rect[w].w = coin_width;
          hx += inter;
        }

      vehichle_up[0].x = vehichle_up[0].y = 0;vehichle_up[0].h = car_height;vehichle_up[0].w=car_width;
      vehichle_up[1].x = vehichle_up[1].y = 0;vehichle_up[1].h = bus_height;vehichle_up[1].w=bus_width;
      vehichle_up[2].x = vehichle_up[2].y = 0;vehichle_up[2].h = dd_height;vehichle_up[2].w=dd_width;
      vehichle_up[3].x = vehichle_up[3].y = 0;vehichle_up[3].h = cng_height;vehichle_up[3].w=cng_width;
      vehichle_down[0].x = vehichle_down[0].y = 0;vehichle_down[0].h = car_height;vehichle_down[0].w=car_width;
      vehichle_down[1].x = vehichle_down[1].y = 0;vehichle_down[1].h = bus_height;vehichle_down[1].w=bus_width;
      vehichle_down[2].x = vehichle_down[2].y = 0;vehichle_down[2].h = dd_height;vehichle_down[2].w=dd_width;
      vehichle_down[3].x = vehichle_down[3].y = 0;vehichle_down[3].h = cng_height;vehichle_down[3].w=cng_width;
      vehichle_left[0].x = vehichle_left[0].y = 0;vehichle_left[0].w = car_height;vehichle_left[0].h=car_width;
      vehichle_left[1].x = vehichle_left[1].y = 0;vehichle_left[1].w = bus_height;vehichle_left[1].h=bus_width;
      vehichle_left[2].x = vehichle_left[2].y = 0;vehichle_left[2].w = dd_height;vehichle_left[2].h=dd_width;
      vehichle_left[3].x = vehichle_left[3].y = 0;vehichle_left[3].w = cng_height;vehichle_left[3].h=cng_width;
      vehichle_right[0].x = vehichle_right[0].y = 0;vehichle_right[0].w = car_height;vehichle_right[0].h=car_width;
      vehichle_right[1].x = vehichle_right[1].y = 0;vehichle_right[1].w = bus_height;vehichle_right[1].h=bus_width;
      vehichle_right[2].x = vehichle_right[2].y = 0;vehichle_right[2].w = dd_height;vehichle_right[2].h=dd_width;
      vehichle_right[3].x = vehichle_right[3].y = 0;vehichle_right[3].w = cng_height;vehichle_right[3].h=cng_width;
      bike_up_rect.x = 0,bike_up_rect.y=0,bike_up_rect.h=bike_height,bike_up_rect.w=bike_width;
      bike_down_rect.x = 0,bike_down_rect.y=0,bike_down_rect.h=bike_height,bike_down_rect.w = bike_width;

  return s;
}
void close()
{
  play_button.free();quit_button.free();hof_button.free();button_shadow.free();logo.free();road.free();side_walk.free();side_walk_2.free();
  gari_up.free();bus_up.free();dotola_bus_up.free();cng_up.free();
  gari_down.free();bus_down.free();dotola_bus_down.free();cng_down.free();
  skeleton.free();
  hof_bg.free();
  for (unsigned int scorequit_idx = 0; scorequit_idx < totalscorenum; scorequit_idx++) {
    score[scorequit_idx].free();
    scorename[scorequit_idx].free();
  }
  SDL_DestroyWindow(main_window);
  SDL_DestroyRenderer(main_renderer);
  main_window = NULL;
  main_renderer = NULL;
  TTF_Quit();
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
      begin:

      SDL_Event e;

      int ident=0,f=0,p_f=6,q_f=6,h_f=6;
      int side_walk_y_1 = 0,side_walk_y_2=-1440;
      int side_walk_y_3 = 0,side_walk_y_4 = -1440;
      int character_x = 15,character_y=600;
      int road_x = 340,road_y_1 = 0,road_y_2=-1440;
      int mx,my;
      bool moto = false;
      int speed = 4;
      for (int tempscore_string_initialiser = 0; tempscore_string_initialiser < 8; tempscore_string_initialiser++)
      {
        tempscore_string[tempscore_string_initialiser] = 48;
      }
      bool quit = false;
      bool in_game=false;
      bool in_scoreboard = false;
      bool marker_up[4][8] = {0};
      bool marker_down[4][8] = {0};
      bool marker_left[4][8] = {0};
      bool marker_right[4][8] = {0};
      bool marker_snacks[2][16] = {0};
      bool marker_coin[16] = {0};
      bool scores_open = false;
      bool rerender_text = false;
      bool savenow = false;
      bool game_paused = false;
      bool loadstate = true;
      bool write_state_tofile = true;
      int vehicle_variant_selector;
      int snack_variant_selector;
      int snack = 0;
      int car_up = 0,car_down=0,car_left=0,car_right=0;
      int stamina = 1280;
      int stamina_blow = 100;
      bool bike_up_stat = 0;
      bool bike_down_stat = 0;
      int ypos_bike_up = 920;
      int ypos_bike_down = -200;
      int xpos_bike;
      int bike_direction_selector;
      int bike_lane_selector;
      bool midFrame=false,topFrame=true,bottomFrame=false;
      int coins = 0;
      int xpos_up[8] = {450,450,450,450,450,450,450,450};
      int ypos_up[8] = {920,920,920,920,920,920,920,920};
      int xpos_down[8] = {750,750,750,750,750,750,750,750};
      int ypos_down[8] = {-400,-400,-400,-400,-400,-400,-400,-400};
      int xpos_left[8] = {-100,-100,-100,-100,-100,-100,-100,-100};
      int xpos_right[8] = {1100,1100,1100,1100,1100,1100,1100,1100};
      int y_left=-100,y_right=10;
      int ypos_snack[16] = {720,720,720,720,720,720,720,720,720,720,720,720,720,720,720,720};
      int xpos_snack[16];
      int ypos_coin[16] = {720,720,720,720,720,720,720,720,720,720,720,720,720,720,720,720};
      int xpos_coin[16];
      int c_f = 0;
      int fff;
      bool right_permit=1;
      bool left_permit=1;

      int paused_tempscore;
      int paused_side_walk_y_1;
      int paused_side_walk_y_2;
      int paused_side_walk_y_3;
      int paused_side_walk_y_4;
      int paused_character_x;
      int paused_character_y;
      int paused_road_x;
      int paused_road_y_1;
      int paused_road_y_2;
      int paused_speed;
      bool paused_marker_up[4][8] = {0};
      bool paused_marker_down[4][8] = {0};
      bool paused_marker_left[4][8] = {0};
      bool paused_marker_right[4][8] = {0};
      bool paused_marker_snacks[2][16] = {0};
      bool paused_marker_coin[16] = {0};
      int paused_vehicle_variant_selector;
      int paused_snack_variant_selector;
      int paused_snack;
      int paused_coins;
      int paused_car_up;
      int paused_car_down;
      int paused_car_left;
      int paused_car_right;
      bool paused_midFrame, paused_topFrame, paused_bottomFrame;
      int paused_stamina;
      int paused_stamina_blow;
      bool paused_bike_up_stat;
      bool paused_bike_down_stat;
      int paused_ypos_bike_up;
      int paused_ypos_bike_down;
      int paused_xpos_up[8] = {0};
      int paused_xpos_down[8] = {0};
      int paused_xpos_left[8] = {0};
      int paused_xpos_right[8] = {0};
      int paused_ypos_up[8] = {0};
      int paused_ypos_down[8] = {0};
      int paused_y_left;
      int paused_y_right;
      int paused_xpos_snack[16] = {0};
      int paused_ypos_snack[16] = {0};
      int paused_xpos_coin[16] = {0};
      int paused_ypos_coin[16] = {0};
      //int paused_fff;
      bool paused_left_permit;
      bool paused_right_permit;

      const Uint8 *state = SDL_GetKeyboardState(NULL);
      while(!quit)
      {
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_MOUSEBUTTONDOWN)
            {
              SDL_GetMouseState(&mx,&my);
              if((hover(mx,my,play_button_x,play_button_y,button_height,button_width)) && !in_game && !in_scoreboard && !in_scoresave)
              {
                clock_release.start();
                clock_move.start();
                snacks.start();
                beka.start();
                tera.start();
                in_game = true;
              }
              else if((hover(mx,my,quit_button_x,quit_button_y,button_height,button_width)) && !in_game && !in_scoreboard && !in_scoresave)
              {
                quit = true;
              }
              else if((hover(mx,my,hof_button_x,hof_button_y,button_height,button_width)) && !in_game && !in_scoreboard && !in_scoresave)
              {
                in_scoreboard = true;
              }

            }
            else if(e.type == SDL_KEYDOWN)
            {
              if( e.key.keysym.sym == SDLK_BACKSPACE && tempname.length() > 0 && !in_game && in_scoresave)
  						{
  							//lop off character
  							tempname.pop_back();
  							rerender_text = true;
  						}
              else if(e.key.keysym.sym == SDLK_RETURN && tempname.length() > 0 && !in_game && in_scoresave)
              {
                savenow = true;
              }
              else if(e.key.keysym.sym == SDLK_ESCAPE && in_game)
              {
                if (!game_paused) {
                  game_paused = true;
                }
                else {
                  game_paused = false;
                }
              }
            }

            else if( e.type == SDL_TEXTINPUT && e.key.keysym.sym != SDLK_RETURN)
  					{
  						if(tempname.length() <= 9 && !in_game && in_scoresave)
  						{
  							tempname += e.text.text;
  							rerender_text = true;
  						}
  					}

            else if(e.type == SDL_QUIT)
            {
              quit = true;
            }
        }
        if(in_game)
        {
          //If we start playing the game, this will show
          SDL_SetRenderDrawColor( main_renderer, 0xFF, 0xFF, 0xFF, 0xFF );
          SDL_RenderClear( main_renderer );
          srand(time(NULL));
        //  printf("%d %d \n", clock_release.getTicks(), clock_move.getTicks());
         if(car_up >= 8)car_up = 0;
         if(car_down >= 8)car_down = 0;
         if(car_left >= 8)car_left = 0;
         if(car_right >= 8)car_right = 0;
         if(snack >= 16)snack = 0;
         if(coins >= 16)coins = 0;

         if(!game_paused)
         {
           if (loadstate) {
              loadstate = false;
              FILE *readstate = fopen("savedata/state.sav", "r");
              if (readstate != NULL) {
                fscanf(readstate, "%d", &tempscore);
                padded_itoa(tempscore, tempscore_string);
                fscanf(readstate, "%d", &side_walk_y_1);
                fscanf(readstate, "%d", &side_walk_y_2);
                fscanf(readstate, "%d", &side_walk_y_3);
                fscanf(readstate, "%d", &side_walk_y_4);
                fscanf(readstate, "%d", &character_x);
                fscanf(readstate, "%d", &character_y);
                fscanf(readstate, "%d", &road_x);
                fscanf(readstate, "%d", &road_y_1);
                fscanf(readstate, "%d", &road_y_2);
                fscanf(readstate, "%d", &speed);
                for (int marker_vehicle_variant_idx = 0; marker_vehicle_variant_idx < 4; marker_vehicle_variant_idx++) {
                  for (int marker_vehicle_queue_idx = 0; marker_vehicle_queue_idx < 8; marker_vehicle_queue_idx++) {
                    fscanf(readstate, "%d", &marker_up[marker_vehicle_variant_idx][marker_vehicle_queue_idx]);
                    fscanf(readstate, "%d", &marker_down[marker_vehicle_variant_idx][marker_vehicle_queue_idx]);
                    fscanf(readstate, "%d", &marker_left[marker_vehicle_variant_idx][marker_vehicle_queue_idx]);
                    fscanf(readstate, "%d", &marker_right[marker_vehicle_variant_idx][marker_vehicle_queue_idx]);
                  }
                }
                for (int marker_consumable_idx = 0; marker_consumable_idx < 16; marker_consumable_idx++) {
                  for (int marker_snack_variant = 0; marker_snack_variant < 2; marker_snack_variant++) {
                    fscanf(readstate, "%d", &marker_snacks[marker_snack_variant][marker_consumable_idx]);
                  }
                  fscanf(readstate, "%d", &marker_coin[marker_consumable_idx]);
                }
                fscanf(readstate, "%d", &vehicle_variant_selector);
                fscanf(readstate, "%d", &snack_variant_selector);
                fscanf(readstate, "%d", &snack);
                fscanf(readstate, "%d", &coins);
                fscanf(readstate, "%d", &car_up);
                fscanf(readstate, "%d", &car_down);
                fscanf(readstate, "%d", &car_left);
                fscanf(readstate, "%d", &car_right);
                fscanf(readstate, "%d", &midFrame);
                fscanf(readstate, "%d", &topFrame);
                fscanf(readstate, "%d", &bottomFrame);
                fscanf(readstate, "%d", &stamina);
                fscanf(readstate, "%d", &stamina_blow);
                fscanf(readstate, "%d", &bike_up_stat);
                fscanf(readstate, "%d", &bike_down_stat);
                fscanf(readstate, "%d", &ypos_bike_up);
                fscanf(readstate, "%d", &ypos_bike_down);
                for (int xypos_vehicle_idx = 0; xypos_vehicle_idx < 8; xypos_vehicle_idx++) {
                  fscanf(readstate, "%d", &xpos_up[xypos_vehicle_idx]);
                  fscanf(readstate, "%d", &xpos_down[xypos_vehicle_idx]);
                  fscanf(readstate, "%d", &xpos_left[xypos_vehicle_idx]);
                  fscanf(readstate, "%d", &xpos_right[xypos_vehicle_idx]);
                  fscanf(readstate, "%d", &ypos_up[xypos_vehicle_idx]);
                  fscanf(readstate, "%d", &ypos_down[xypos_vehicle_idx]);
                }
                fscanf(readstate, "%d", &y_left);
                fscanf(readstate, "%d", &y_right);
                for (int xypos_consumable_idx = 0; xypos_consumable_idx < 16; xypos_consumable_idx++) {
                  fscanf(readstate, "%d", &xpos_snack[xypos_consumable_idx]);
                  fscanf(readstate, "%d", &ypos_snack[xypos_consumable_idx]);
                  fscanf(readstate, "%d", &xpos_coin[xypos_consumable_idx]);
                  fscanf(readstate, "%d", &ypos_coin[xypos_consumable_idx]);
                }
                fscanf(readstate, "%d", &left_permit);
                fscanf(readstate, "%d", &right_permit);
              }
              else {
                printf("No savestate found\n");
              }

              fclose(readstate);
           }
         fff = std::max(road_y_1,road_y_2);
         if(topFrame)
         {
            y_right = fff+150;
            y_left = fff-200;
         }
         else if(midFrame)
         {
           y_left = fff-200;
           y_right =fff+150;
         }
         else if(bottomFrame)
         {
           y_left = fff-200;
           y_right = fff+150;
         }
         else
         {
           y_left = 1080;
           y_right = 1080;
           for(int w=0;w<8;w++)
           {
             if(marker_left[0][w]||marker_left[1][w]||marker_left[2][w]||marker_left[3][w])
             {
                 marker_left[0][w]=0;
                 marker_left[1][w]=0;
                 marker_left[2][w]=0;
                 marker_left[3][w]=0;
                 xpos_left[w]=0;
             }
             if(marker_right[0][w] || marker_right[1][w] || marker_right[2][w] || marker_right[3][w])
             {

                 marker_right[0][w]=0;
                 marker_right[1][w]=0;
                 marker_right[2][w]=0;
                 marker_right[3][w]=0;
                 xpos_right[w]=1080;

             }
           }
         }
         //printf("%d %d\n",y_left,y_right);
         if(fff >= 0 && fff<=180)
         {
           topFrame = true;
           midFrame = false;
           bottomFrame = false;
         }
         else if(fff >= 180 && fff <= 720)
         {
           topFrame = false;
           midFrame = true;
           bottomFrame = false;
         }
         else if(fff >= 720 && fff <= 1080)
         {
           topFrame = false;
           midFrame = false;
           bottomFrame = true;
         }
         else
         {
           topFrame = false;
           midFrame = false;
           bottomFrame = false;
         }

         if(clock_release.getTicks()%6000 < 15 && moto && !bike_up_stat && !bike_down_stat)
         {
           bike_direction_selector = rand()%2;
           if(bike_direction_selector == 0)
           {
             bike_up_stat = 1;
             bike_down_stat = 0;
             bike_lane_selector = rand()%3;
             if(bike_lane_selector == 0)
             {
               xpos_bike = 180;
             }
             else if(bike_lane_selector == 1)
             {
              xpos_bike = 600;
             }
             else
             {
              xpos_bike  = 1000;
             }
           }
           else
           {
             bike_up_stat = 0;
             bike_down_stat = 1;
             bike_lane_selector = rand()%3;
             if(bike_lane_selector == 0)
             {
               xpos_bike = 200;
             }
             else if(bike_lane_selector == 1)
             {
              xpos_bike = 600;
             }
             else
             {
              xpos_bike  = 1000;
             }
           }
         }

         c_f++;
         if(c_f >= 36)c_f = 0;

         if(clock_release.getTicks()%1000 <= 20 && rand()%15<10)
         {
           srand(time(NULL));
           vehicle_variant_selector = rand()%4;
           if(!marker_up[0][car_up]&&!marker_up[1][car_up]&&!marker_up[2][car_up]&&!marker_up[3][car_up])
           {
             if(car_up> 0)
             {
               if(marker_up[0][car_up-1]|| marker_up[1][car_up-1]|| marker_up[2][car_up-1]||marker_up[3][car_up-1])
               {
                 if(ypos_up[car_up-1]> 200)marker_up[vehicle_variant_selector][car_up++]=1;
               }
               else marker_up[vehicle_variant_selector][car_up++]=1;
             }
             else
             {
               if(marker_up[0][7]|| marker_up[1][7]|| marker_up[2][7]||marker_up[3][7])
               {
                 if(ypos_up[7]> 200)marker_up[vehicle_variant_selector][car_up++]=1;
               }
               else marker_up[vehicle_variant_selector][car_up++]=1;
             }

           }


         }

         if(rand()%10 < 8 && snacks.getTicks()%1000 <= 20 && character_y <= 360 && state[SDL_SCANCODE_UP])
         {
           srand(time(NULL));
           snack_variant_selector = rand()%2;
           if(!marker_snacks[0][snack] && !marker_snacks[1][snack])
           {
             marker_snacks[snack_variant_selector][snack++]=1;
             ypos_snack[snack-1] = character_y - 400 - rand()%300;
             xpos_snack[snack-1]  = 300 +  rand()%600;
           }
           if(!marker_coin[coins])
           {
             marker_coin[coins++] = 1;
             ypos_coin[coins-1] = character_y - 200 - rand()%450;
             xpos_coin[coins-1] = 200 + rand()%700;
           }
         }
         srand(time(NULL));
         if(clock_release.getTicks()%1000 <= 20 && rand()%15<10)
         {
           srand(time(NULL));
           vehicle_variant_selector = (clock_move.getTicks()+rand())%4;
           if(!marker_down[0][car_down]&&!marker_down[1][car_down]&&!marker_down[2][car_down]&&!marker_down[3][car_down])
           {
             if(car_down> 0)
             {
               if(marker_down[0][car_down-1]|| marker_down[1][car_down-1]|| marker_down[2][car_down-1]||marker_down[3][car_down-1])
               {
                 if(ypos_down[car_down-1]> 200)marker_down[vehicle_variant_selector][car_down++]=1;
               }
               else marker_down[vehicle_variant_selector][car_down++]=1;
             }
             else
             {
               if(marker_down[0][7]|| marker_down[1][7]|| marker_down[2][7]||marker_down[3][7])
               {
                 if(ypos_down[7]> 200)marker_down[vehicle_variant_selector][car_down++]=1;
               }
               else  marker_down[vehicle_variant_selector][car_down++]=1;
             }

           }
         }
         //SDL_Delay(5);
         for(int w=0;w<8;w++)
         {
           int h1=0,w1=0,h2=0,w2=0;

           for(int q=0;q<4;q++)
           {
             if(marker_up[q][w])
             {
               if(q == 0)
               {
                 h1 = car_height;
                 w1 = car_width;
               }
               else if(q == 1)
               {
                 h1 = bus_height;
                 w1 = bus_width;
               }
               else if(q==2)
               {
                 h1 = dd_height;
                 w1 = dd_width;
               }
               else
               {
                 h1 = cng_height;
                 w1 = cng_width;
               }
               break;
             }
           }
           for(int q=0;q<4;q++)
           {
             if(marker_down[q][w])
             {
               if(q == 0)
               {
                 h2 = car_height;
                 w2 = car_width;
               }
               else if(q == 1)
               {
                 h2 = bus_height;
                 w2 = bus_width;
               }
               else if(q==2)
               {
                 h2 = dd_height;
                 w2 = dd_width;
               }
               else
               {
                 h2 = cng_height;
                 w2 = cng_width;
               }
               break;
             }
           }

           if(is_colliding(character_x,character_y,xpos_up[w],ypos_up[w],h1,w1) || is_colliding(character_x,character_y,xpos_down[w],ypos_down[w],h2,w2))
           {
             character_x=600;
             stamina-=stamina_blow;
             //SDL_Delay(50);
           }
           h1=0,w1=0,w2=0,h2=0;
           for(int q=0;q<4;q++)
           {
             if(marker_right[q][w])
             {
               if(q == 0)
               {
                 w1 = car_height;
                 h1 = car_width;
               }
               else if(q == 1)
               {
                 w1 = bus_height;
                 h1 = bus_width;
               }
               else if(q==2)
               {
                 w1 = dd_height;
                 h1 = dd_width;
               }
               else
               {
                 w1 = cng_height;
                 h1 = cng_width;
               }
               break;
             }
           }
           for(int q=0;q<4;q++)
           {
             if(marker_left[q][w])
             {
               if(q == 0)
               {
                 w2 = car_height;
                 h2 = car_width;
               }
               else if(q == 1)
               {
                 w2 = bus_height;
                 h2 = bus_width;
               }
               else if(q==2)
               {
                 w2 = dd_height;
                 h2 = dd_width;
               }
               else
               {
                 w2 = cng_height;
                 h2 = cng_width;
               }
               break;
             }
           }
           if(is_colliding(character_x,character_y,xpos_right[w],y_right,h1,w1) || is_colliding(character_x,character_y,xpos_left[w],y_left,h2,w2))
           {
             character_x=600;
             stamina-=stamina_blow;
             //SDL_Delay(50);
           }
         }
         if(bike_up_stat)
         {
           if(is_colliding(character_x,character_y,xpos_bike,ypos_bike_up,bike_height,bike_width))
           {
             character_x = 600;
             stamina -= 3*stamina_blow;
           }
         }
         else if(bike_down_stat)
         {
           if(is_colliding(character_x,character_y,xpos_bike,ypos_bike_down,bike_height,bike_width))
           {
             character_x = 600;
             stamina -= 3*stamina_blow;
           }
         }
         for(int w=0;w<16;w++)
         {
           if(marker_snacks[0][w])
           {
             if(is_colliding(character_x,character_y,xpos_snack[w],ypos_snack[w],chips_rect.h,chips_rect.w))
             {
               stamina+=100;
               if (stamina > 1280) {
                 stamina = 1280;
               }
               xpos_snack[w]=0;
               ypos_snack[w]=15000;

             }
           }
           else if(marker_snacks[1][w])
           {
             if(is_colliding(character_x,character_y,xpos_snack[w],ypos_snack[w],frooto_rect.h,frooto_rect.w))
             {
               xpos_snack[w]=0;
               ypos_snack[w]=15000;
               stamina+=100;
               if (stamina > 1280) {
                 stamina = 1280;
               }
             }
           }
           if(marker_coin[w])
           {
             if(is_colliding(character_x,character_y,xpos_coin[w],ypos_coin[w],coin_height,coin_width))
             {
               xpos_coin[w]=0;
               ypos_coin[w]=15000;
               tempscore += 100;
             }
           }
         }
         if(stamina<0)
         {
           in_game = false;
           in_scoresave = true;
           goto begin;
         }
         stamina_blow = 100 + 20*(tempscore/1000);
         speed  = tempscore/8000 + 3;
         if(tempscore > 8000)
         {
           moto = 1;
         }
         if(1)
         {
           for(int w=0;w<8;w++)
           {
             if(marker_up[0][w]||marker_up[1][w]||marker_up[2][w]||marker_up[3][w])
             {
               if(ypos_up[w]>= y_left && ypos_up[w]<=820)left_permit=0;
               if(ypos_up[w]>=y_right && ypos_up[w] <= 820)right_permit=0;
               ypos_up[w]-=(fps + fps/4 + (speed-2)*state[SDL_SCANCODE_UP] );
             }
             if(marker_down[0][w]||marker_down[1][w]||marker_down[2][w]||marker_down[3][w])
             {
               if(ypos_down[w]<=y_left && ypos_down[w]>-300)left_permit=0;
               if(ypos_down[w]<=y_right && ypos_down[w]>-300)right_permit=0;
               ypos_down[w]+=(fps +fps/4 + (speed+1)*state[SDL_SCANCODE_UP]);
             }
           }
           if(bike_up_stat)ypos_bike_up -= fps*2;
           if(bike_down_stat)ypos_bike_down += fps*2;
         }
         for(int w=0;w<8;w++)
         {

           if(marker_left[0][w]||marker_left[1][w]||marker_left[2][w]||marker_left[3][w])
           {
            // printf("FCK YOUUUUU\n");
             xpos_left[w]+=2*fps;//- fps*state[SDL_SCANCODE_UP]);
           }
           if(marker_right[0][w]||marker_right[1][w]||marker_right[2][w]||marker_right[3][w])
           {
             xpos_right[w]-=2*fps;
           }
         }
         if(left_permit)
         {
           srand(time(NULL));
           vehicle_variant_selector = rand()%4;
           if(tera.getTicks() >= 1500)
           {
             marker_left[vehicle_variant_selector][car_left++]=1;
             left_permit=0;
             tera.stop();
             tera.start();
            // printf("%d\n",tera.getTicks());
           }
         }
         else left_permit=1;
         if(right_permit)
         {
           srand(time(NULL));
           vehicle_variant_selector = rand()%4;
           if(beka.getTicks() >= 1500)
           {
             marker_right[vehicle_variant_selector][car_right++]=1;
             right_permit = 0;
             beka.stop();
             beka.start();
           }
         }
         else right_permit = 1;
         if(bike_up_stat)
         {
           if(character_y - ypos_bike_up > 720)
           {
             ypos_bike_up = 920;
             bike_up_stat = 0;
           }
         }
         if(bike_down_stat)
         {
          if(ypos_bike_down - character_y > 720)
          {
            ypos_bike_down = -200;
            bike_down_stat = 0;
          }
         }
         for(int w=0;w<16;w++)
         {
           if(ypos_snack[w]-character_y> 720)
           {
             marker_snacks[0][w]=0;
             marker_snacks[1][w]=0;
           }
           if(ypos_coin[w] - character_y > 720)
           {
             marker_coin[w] = 0;
           }
         }
         for(int w=0;w<8;w++)
         {
           if(marker_up[0][w]||marker_up[1][w]||marker_up[2][w]||marker_up[3][w])
           {
             if(character_y-ypos_up[w] > 720)
             {
               marker_up[0][w]=0;
               marker_up[1][w]=0;
               marker_up[2][w]=0;
               marker_up[3][w]=0;
               ypos_up[w]=1080;
             }
           }
           if(marker_down[0][w] || marker_down[1][w] || marker_down[2][w] || marker_down[3][w])
           {
             if(ypos_down[w] - character_y > 720)
             {
               marker_down[0][w]=0;
               marker_down[1][w]=0;
               marker_down[2][w]=0;
               marker_down[3][w]=0;
               ypos_down[w]=-720;
             }
           }
           if(marker_left[0][w]||marker_left[1][w]||marker_left[2][w]||marker_left[3][w])
           {
             if(xpos_left[w]>1500)
             {
               marker_left[0][w]=0;
               marker_left[1][w]=0;
               marker_left[2][w]=0;
               marker_left[3][w]=0;
               xpos_left[w]=-100;
             }
           }
           if(marker_right[0][w] || marker_right[1][w] || marker_right[2][w] || marker_right[3][w])
           {
             if(xpos_right[w]<-200)
             {
               marker_right[0][w]=0;
               marker_right[1][w]=0;
               marker_right[2][w]=0;
               marker_right[3][w]=0;
               xpos_right[w]=1100;
             }
           }
         }
          if(road_y_1 >= 1440)road_y_1=-1440;
          if(road_y_2 >= 1440)road_y_2 = -1440;
          if(side_walk_y_1 >= 1440)side_walk_y_1=-1440;
          if(side_walk_y_2 >= 1440)side_walk_y_2=-1440;
          if(side_walk_y_3 >= 1440)side_walk_y_3=-1440;
          if(side_walk_y_4 >= 1440)side_walk_y_4=-1440;
        }
          road.render(road_x,road_y_1,&road_rect);
          road.render(road_x,road_y_2,&road_rect);//two road tiling one after another creating an illusion of continuity
          side_walk.render(road_x-170,side_walk_y_1,&side_walk_rect);
          side_walk.render(road_x - 170,side_walk_y_2,&side_walk_rect);
          side_walk_2.render(road_x+600,side_walk_y_3,&side_walk_rect_2);
          side_walk_2.render(road_x+600,side_walk_y_4,&side_walk_rect_2);
          currentscore.RasteriseText(tempscore_string);
          currentscore_rect.w = currentscore.getW();
          currentscore.render(900, 100, &currentscore_rect);
          if(character_x < 0)character_x = 0;
          if(character_x > 1280)character_x = 1280;
          if(character_y < 0)character_y = 0;
          if(character_y > 720)character_y = 720;
          for(int w=0;w<16;w++)
          {
            if(marker_snacks[0][w])chips.render(xpos_snack[w],ypos_snack[w],&chips_rect);
            else if(marker_snacks[1][w])frooto.render(xpos_snack[w],ypos_snack[w],&frooto_rect);
            if(marker_coin[w])coin.render(xpos_coin[w],ypos_coin[w],coin_rect+c_f);
          }
          if(ident == 0)skeleton.render(character_x,character_y,going_up+f);
          else if(ident == 1)skeleton.render(character_x,character_y,going_down+f);
          else if(ident == 2)skeleton.render(character_x,character_y,going_left+f);
          else if(ident == 3)skeleton.render(character_x,character_y,going_right+f);
          for(int w=0;w<8;w++)
          {
            if(marker_up[0][w]== 1)gari_up.render(450,ypos_up[w],&vehichle_up[0]);
            else if(marker_up[1][w]==1)bus_up.render(450,ypos_up[w],&vehichle_up[1]);
            else if(marker_up[2][w]==1)dotola_bus_up.render(450,ypos_up[w],&vehichle_up[2]);
            else if(marker_up[3][w]==1)cng_up.render(450,ypos_up[w],&vehichle_up[3]);
            if(marker_left[0][w]== 1)gari_left.render(xpos_left[w],y_left,&vehichle_left[0]);
            else if(marker_left[1][w]==1)bus_left.render(xpos_left[w],y_left,&vehichle_left[1]);
            else if(marker_left[2][w]==1)dotola_bus_left.render(xpos_left[w],y_left,&vehichle_left[2]);
            else if(marker_left[3][w]==1)cng_left.render(xpos_left[w],y_left,&vehichle_left[3]);
          }
          for(int w=0;w<8;w++)
          {
            if(marker_down[0][w]== 1)gari_down.render(750,ypos_down[w],&vehichle_down[0]);
            else if(marker_down[1][w]==1)bus_down.render(750,ypos_down[w],&vehichle_down[1]);
            else if(marker_down[2][w]==1)dotola_bus_down.render(750,ypos_down[w],&vehichle_down[2]);
            else if(marker_down[3][w]==1)cng_down.render(750,ypos_down[w],&vehichle_down[3]);
            if(marker_right[0][w]== 1)gari_right.render(xpos_right[w],y_right,&vehichle_right[0]);
            else if(marker_right[1][w]==1)bus_right.render(xpos_right[w],y_right,&vehichle_right[1]);
            else if(marker_right[2][w]==1)dotola_bus_right.render(xpos_right[w],y_right,&vehichle_right[2]);
            else if(marker_right[3][w]==1)cng_right.render(xpos_right[w],y_right,&vehichle_right[3]);
          }
          if(bike_up_stat)bike_up.render(xpos_bike,ypos_bike_up,&bike_up_rect);
          if(bike_down_stat)bike_down.render(xpos_bike,ypos_bike_down,&bike_down_rect);
          SDL_Rect stam = {0,0,stamina,15};
          if (stamina >= 800) {
            SDL_SetRenderDrawColor( main_renderer, 0x85, 0xDD, 0x88, 0xFF ); //Colour code #85dd88
            SDL_RenderFillRect( main_renderer, &stam );
            SDL_RenderPresent(main_renderer);
          }
          else if (stamina < 800 && stamina >= 400) {
            SDL_SetRenderDrawColor( main_renderer, 0xDD, 0xB5, 0x85, 0xFF ); //Colour code #ddb585
            SDL_RenderFillRect( main_renderer, &stam );
            SDL_RenderPresent(main_renderer);
          }
          else {
            SDL_SetRenderDrawColor( main_renderer, 0xDD, 0x85, 0x85, 0xFF ); //Colour code #dd8585
            SDL_RenderFillRect( main_renderer, &stam );
            SDL_RenderPresent(main_renderer);
          }
          if(!game_paused)
          {
          if(state[SDL_SCANCODE_UP] ||state[SDL_SCANCODE_LEFT]||state[SDL_SCANCODE_RIGHT]||state[SDL_SCANCODE_DOWN])//if we're playing the game, then these conditions apply
          {
            if(state[SDL_SCANCODE_UP] )
            {
              stamina--;
              ident=0;
              if(character_y <= 360)
              {
                tempscore++;
                //printf("%d\n", tempscore);
                padded_itoa(tempscore, tempscore_string);
                road_y_1+=fps;
                road_y_2 +=fps;
                side_walk_y_1 += fps;
                side_walk_y_2 += fps;
                side_walk_y_3 += fps;
                side_walk_y_4 += fps;
                for(int w=0;w<16;w++)
                {
                  if(marker_snacks[0][w] || marker_snacks[1][w])ypos_snack[w]+=fps;
                  if(marker_coin[w])ypos_coin[w]+=fps;
                }
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
              stamina--;
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
              stamina--;
              ident=2;
              character_x-=fps;
            }
            else if(state[SDL_SCANCODE_RIGHT])
            {
              stamina--;
              ident=3;
              character_x+=fps;
            }
            f++;
            if(f >= 9)f=0;
          }
        }
          if (game_paused) {
            paused_tempscore = tempscore;
            paused_side_walk_y_1 = side_walk_y_1;
            paused_side_walk_y_2 = side_walk_y_2;
            paused_side_walk_y_3 = side_walk_y_3;
            paused_side_walk_y_4 = side_walk_y_4;
            paused_character_x = character_x;
            paused_character_y = character_y;
            paused_road_x = road_x;
            paused_road_y_1 = road_y_1;
            paused_road_y_2 = road_y_2;
            paused_speed = speed;
            for (int paused_marker_vehicle_variant_idx = 0; paused_marker_vehicle_variant_idx < 4; paused_marker_vehicle_variant_idx++) {
              for (int paused_marker_vehicle_queue_idx = 0; paused_marker_vehicle_queue_idx < 8; paused_marker_vehicle_queue_idx++) {
                paused_marker_up[paused_marker_vehicle_variant_idx][paused_marker_vehicle_queue_idx] = marker_up[paused_marker_vehicle_variant_idx][paused_marker_vehicle_queue_idx];
                paused_marker_down[paused_marker_vehicle_variant_idx][paused_marker_vehicle_queue_idx] = marker_down[paused_marker_vehicle_variant_idx][paused_marker_vehicle_queue_idx];
                paused_marker_left[paused_marker_vehicle_variant_idx][paused_marker_vehicle_queue_idx] = marker_left[paused_marker_vehicle_variant_idx][paused_marker_vehicle_queue_idx];
                paused_marker_right[paused_marker_vehicle_variant_idx][paused_marker_vehicle_queue_idx] = marker_right[paused_marker_vehicle_variant_idx][paused_marker_vehicle_queue_idx];
              }
            }
            for (int paused_marker_consumable_idx = 0; paused_marker_consumable_idx < 16; paused_marker_consumable_idx++) {
              for (int paused_marker_snack_variant = 0; paused_marker_snack_variant < 2; paused_marker_snack_variant++) {
                paused_marker_snacks[paused_marker_snack_variant][paused_marker_consumable_idx] = marker_snacks[paused_marker_snack_variant][paused_marker_consumable_idx];
              }
              paused_marker_coin[paused_marker_consumable_idx] = marker_coin[paused_marker_consumable_idx];
            }
            paused_vehicle_variant_selector = vehicle_variant_selector;
            paused_snack_variant_selector = snack_variant_selector;
            paused_snack = snack;
            paused_coins = coins;
            paused_car_up = car_up;
            paused_car_down = car_down;
            paused_car_left = car_left;
            paused_car_right = car_right;
            paused_midFrame = midFrame;
            paused_topFrame = topFrame;
            paused_bottomFrame = bottomFrame;
            paused_stamina = stamina;
            paused_stamina_blow = stamina_blow;
            paused_bike_up_stat = bike_up_stat;
            paused_bike_down_stat = bike_down_stat;
            paused_ypos_bike_up = ypos_bike_up;
            paused_ypos_bike_down = ypos_bike_down;
            for (int paused_xypos_vehicle_idx = 0; paused_xypos_vehicle_idx < 8; paused_xypos_vehicle_idx++) {
              paused_xpos_up[paused_xypos_vehicle_idx] = xpos_up[paused_xypos_vehicle_idx];
              paused_xpos_down[paused_xypos_vehicle_idx] = xpos_down[paused_xypos_vehicle_idx];
              paused_xpos_left[paused_xypos_vehicle_idx] = xpos_left[paused_xypos_vehicle_idx];
              paused_xpos_right[paused_xypos_vehicle_idx] = xpos_right[paused_xypos_vehicle_idx];
              paused_ypos_up[paused_xypos_vehicle_idx] = ypos_up[paused_xypos_vehicle_idx];
              paused_ypos_down[paused_xypos_vehicle_idx] = ypos_down[paused_xypos_vehicle_idx];
            }
            paused_y_left = y_left;
            paused_y_right = y_right;
            for (int paused_xypos_consumable_idx = 0; paused_xypos_consumable_idx < 16; paused_xypos_consumable_idx++) {
              paused_xpos_snack[paused_xypos_consumable_idx] = xpos_snack[paused_xypos_consumable_idx];
              paused_ypos_snack[paused_xypos_consumable_idx] = ypos_snack[paused_xypos_consumable_idx];
              paused_xpos_coin[paused_xypos_consumable_idx] = xpos_coin[paused_xypos_consumable_idx];
              paused_ypos_coin[paused_xypos_consumable_idx] = ypos_coin[paused_xypos_consumable_idx];
            }
            //paused_fff = fff;
            paused_left_permit = left_permit;
            paused_right_permit = right_permit;

            if (write_state_tofile) {
              FILE *writestate = fopen("savedata/state.sav", "w");

              fprintf(writestate, "%d\n", paused_tempscore);
              fprintf(writestate, "%d\n", paused_side_walk_y_1);
              fprintf(writestate, "%d\n", paused_side_walk_y_2);
              fprintf(writestate, "%d\n", paused_side_walk_y_3);
              fprintf(writestate, "%d\n", paused_side_walk_y_4);
              fprintf(writestate, "%d\n", paused_character_x);
              fprintf(writestate, "%d\n", paused_character_y);
              fprintf(writestate, "%d\n", paused_road_x);
              fprintf(writestate, "%d\n", paused_road_y_1);
              fprintf(writestate, "%d\n", paused_road_y_2);
              fprintf(writestate, "%d\n", paused_speed);
              for (int paused_marker_vehicle_variant_idx = 0; paused_marker_vehicle_variant_idx < 4; paused_marker_vehicle_variant_idx++) {
                for (int paused_marker_vehicle_queue_idx = 0; paused_marker_vehicle_queue_idx < 8; paused_marker_vehicle_queue_idx++) {
                  fprintf(writestate, "%d\n", paused_marker_up[paused_marker_vehicle_variant_idx][paused_marker_vehicle_queue_idx]);
                  fprintf(writestate, "%d\n", paused_marker_down[paused_marker_vehicle_variant_idx][paused_marker_vehicle_queue_idx]);
                  fprintf(writestate, "%d\n", paused_marker_left[paused_marker_vehicle_variant_idx][paused_marker_vehicle_queue_idx]);
                  fprintf(writestate, "%d\n", paused_marker_right[paused_marker_vehicle_variant_idx][paused_marker_vehicle_queue_idx]);
                }
              }
              for (int paused_marker_consumable_idx = 0; paused_marker_consumable_idx < 16; paused_marker_consumable_idx++) {
                for (int paused_marker_snack_variant = 0; paused_marker_snack_variant < 2; paused_marker_snack_variant++) {
                  fprintf(writestate, "%d\n", paused_marker_snacks[paused_marker_snack_variant][paused_marker_consumable_idx]);
                }
                fprintf(writestate, "%d\n", paused_marker_coin[paused_marker_consumable_idx]);
              }
              fprintf(writestate, "%d\n", paused_vehicle_variant_selector);
              fprintf(writestate, "%d\n", paused_snack_variant_selector);
              fprintf(writestate, "%d\n", paused_snack);
              fprintf(writestate, "%d\n", paused_coins);
              fprintf(writestate, "%d\n", paused_car_up);
              fprintf(writestate, "%d\n", paused_car_down);
              fprintf(writestate, "%d\n", paused_car_left);
              fprintf(writestate, "%d\n", paused_car_right);
              fprintf(writestate, "%d\n", paused_midFrame);
              fprintf(writestate, "%d\n", paused_topFrame);
              fprintf(writestate, "%d\n", paused_bottomFrame);
              fprintf(writestate, "%d\n", paused_stamina);
              fprintf(writestate, "%d\n", paused_stamina_blow);
              fprintf(writestate, "%d\n", paused_bike_up_stat);
              fprintf(writestate, "%d\n", paused_bike_down_stat);
              fprintf(writestate, "%d\n", paused_ypos_bike_up);
              fprintf(writestate, "%d\n", paused_ypos_bike_down);
              for (int paused_xypos_vehicle_idx = 0; paused_xypos_vehicle_idx < 8; paused_xypos_vehicle_idx++) {
                fprintf(writestate, "%d\n", paused_xpos_up[paused_xypos_vehicle_idx]);
                fprintf(writestate, "%d\n", paused_xpos_down[paused_xypos_vehicle_idx]);
                fprintf(writestate, "%d\n", paused_xpos_left[paused_xypos_vehicle_idx]);
                fprintf(writestate, "%d\n", paused_xpos_right[paused_xypos_vehicle_idx]);
                fprintf(writestate, "%d\n", paused_ypos_up[paused_xypos_vehicle_idx]);
                fprintf(writestate, "%d\n", paused_ypos_down[paused_xypos_vehicle_idx]);
              }
              fprintf(writestate, "%d\n", paused_y_left);
              fprintf(writestate, "%d\n", paused_y_right);
              for (int paused_xypos_consumable_idx = 0; paused_xypos_consumable_idx < 16; paused_xypos_consumable_idx++) {
                fprintf(writestate, "%d\n", paused_xpos_snack[paused_xypos_consumable_idx]);
                fprintf(writestate, "%d\n", paused_ypos_snack[paused_xypos_consumable_idx]);
                fprintf(writestate, "%d\n", paused_xpos_coin[paused_xypos_consumable_idx]);
                fprintf(writestate, "%d\n", paused_ypos_coin[paused_xypos_consumable_idx]);
              }
              fprintf(writestate, "%d\n", paused_left_permit);
              fprintf(writestate, "%d\n", paused_right_permit);

              fclose(writestate);
            }

            printf("Saved state\n");
            game_paused = false;
            loadstate = true;
            in_game = false;

          }
          SDL_Delay(10);


        }
        else if(in_scoreboard)
        {
          SDL_SetRenderDrawColor( main_renderer, 0xFF, 0xFF, 0xFF, 0xFF );
          SDL_RenderClear( main_renderer );

          hof_bg.render(0, 0, &hof_bg_rect);

          if (!scores_open)
          {
            if (!loadscores())
            {
              printf("Error loading score!\n");
            }
            else
            {
              scores_open = true;
            }
          }

          int score_render_x = 950, score_render_initial_y = 144;
          int scorename_render_x = 150, scorename_render_initial_y = 144;

          for (unsigned int score_render_idx = 0; score_render_idx < totalscorenum; score_render_idx++, scorename_render_initial_y += 50, score_render_initial_y += 50) {
            scorename[score_render_idx].render(scorename_render_x, scorename_render_initial_y, &scorename_rect[score_render_idx]);
            score[score_render_idx].render(score_render_x, score_render_initial_y, &score_rect[score_render_idx]);
          }

          SDL_RenderPresent(main_renderer);
        }
        else if(in_scoresave)
        {
          SDL_StartTextInput();

          scoresave_text.RasteriseText("Enter your name (Max 10 characters):");
          scoresave_text_rect.x = 0;scoresave_text_rect.y = 0;scoresave_text_rect.w = scoresave_text.getW();scoresave_text_rect.h = scoresave_text.getH();

          SDL_SetRenderDrawColor( main_renderer, 0xFF, 0xFF, 0xFF, 0xFF );
          SDL_RenderClear( main_renderer );

          if(rerender_text)
  				{
  					//Text is not empty
  					if( tempname != "" )
  					{
  						//Render new text
              scoresave_name.RasteriseText(tempname.c_str());
              scoresave_name_rect.x = 0;scoresave_name_rect.y = 0;scoresave_name_rect.w = scoresave_name.getW();scoresave_name_rect.h = scoresave_name.getH();
  					}
  					//Text is empty
  					else
  					{
  						//Render space texture
  						scoresave_name.RasteriseText(" ");
  					}
  				}

          scoresave_text.render(100, 200, &scoresave_text_rect);
          scoresave_name.render(100, 275, &scoresave_name_rect);

          SDL_RenderPresent(main_renderer);

          if (savenow) {
            savenow = false;
            SDL_StopTextInput();
            updatescore(tempscore, &tempname[0]);
            tempscore = 0;
            tempname = "";
            scoresave_name.RasteriseText(tempname.c_str());
            tempscore_string[9] = {0};
            for (int tempscore_string_initialiser = 0; tempscore_string_initialiser < 8; tempscore_string_initialiser++) {
              tempscore_string[tempscore_string_initialiser] = 48;
            }
            in_scoresave = false;
          }

        }
        else
        {
          SDL_GetMouseState(&mx,&my);//current mouce coordinates
          if(hover(mx,my,play_button_x,play_button_y,button_height,button_width))
          {
            SDL_Delay(50);
            p_f--;//if we hover over play button then run frames for play animation
            if(p_f< 0)p_f = 0;
            q_f++;
            h_f++;
            if(q_f >= 6)q_f = 6;
            if(h_f >= 6)h_f = 6;
          }
          else if(hover(mx,my,quit_button_x,quit_button_y,button_height,button_width))
          {
            SDL_Delay(50);
            q_f--;//if we hover over quit button then run frames for quit animation
            if(q_f < 0)q_f = 0;
            p_f++;
            h_f++;
            if(p_f >= 6)p_f=6;
            if(h_f >= 6)h_f=6;
          }
          else if(hover(mx,my,hof_button_x,hof_button_y,button_height,button_width))
          {
            SDL_Delay(50);
            h_f--;//if we hover over quit button then run frames for quit animation
            if(h_f < 0)h_f = 0;
            p_f++;
            q_f++;
            if(p_f >= 6)p_f=6;
            if(q_f >= 6)q_f=6;
          }
          else
          {
            p_f++;//we want to increment both frames to the last to bring animation of button to ground state
            q_f++;
            h_f++;
            SDL_Delay(50);
            if(p_f>= 6)p_f = 6;
            if(q_f >= 6)q_f = 6;
            if(h_f >= 6)h_f=6;
          }
          //This is the menu windown
          SDL_SetRenderDrawColor( main_renderer, 0xFF, 0xFF, 0xFF, 0xFF );
          SDL_RenderClear( main_renderer );
          button_shadow.render(quit_button_x,quit_button_y,button_shadow_array+q_f);
          quit_button.render(quit_button_x,quit_button_y,&quit_button_rect);//quit button render
          button_shadow.render(play_button_x,play_button_y,button_shadow_array+p_f);
          play_button.render(play_button_x,play_button_y,&play_button_rect);//play button render
          button_shadow.render(hof_button_x,hof_button_y,button_shadow_array+h_f);
          hof_button.render(hof_button_x,hof_button_y,&hof_button_rect);
          logo.render(470,100,&logo_rect);//logo render
          SDL_RenderPresent(main_renderer);
        }
      }
    }
  }
  close();
  return 0;
}
