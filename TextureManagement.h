#ifndef TEXTUREMANAGEMENT_H_INCLUDED
#define TEXTUREMANAGEMENT_H_INCLUDED

using namespace std;

GLuint loadTexture(string filename)
	{
		GLuint texture;
		SDL_Surface *surface; 
  
		if((surface = IMG_Load(filename.c_str()))) 
		{
			glEnable(GL_TEXTURE_2D);
			/* GLU will convert textures to POT before sending to GL */
			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR);
			SDL_PixelFormat *format = surface->format;

			if(format->Amask)
			{ 
				gluBuild2DMipmaps(GL_TEXTURE_2D, 4, surface->w, surface->h, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
			}
			else
			{
				gluBuild2DMipmaps(GL_TEXTURE_2D, 3, surface->w, surface->h, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
			}
		}
		else 
		{
			fprintf(stderr, "SDL could not load %s.\n%s\n", filename.c_str(), SDL_GetError());
		}

		if(surface)
		{
			SDL_FreeSurface(surface);
		}
		else
		{
			return -1;
		}
		return texture;
	}
#endif