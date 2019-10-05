#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
const int SW = 1920;
const int SH = 1080;
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
int texture_jinish::getHeight()
{
  return mHeight;
}
int texture_jinish::getWidth()
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
    if(main_window =NULL)
    {
      printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			s = false;
    }
    else
    {
      main_renderer = SDL_CreateRenderer(main_window,-1,SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
      if(main_renderer = NULL)
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
  return s;
}
void close()
{
  SDL_DestroyWindow(main_window);
  SDL_DestroyRenderer(main_renderer);
  main_window = NULL;
  main_renderer = NULL;
  IMG_Quit();
  SDL_Quit();
}
int main()
{
  return 0;
}
