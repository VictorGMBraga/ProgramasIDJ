/*
 * SDLBase.cpp
 *
 *  Created on: 16/04/2013
 *      Author: Victor
 */

#include "SDLBase.h"

SDL_Surface* SDLBase::screen = NULL;

void SDLBase::inicializaSDL()
{
	SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_TIMER);
	screen = SDL_SetVideoMode(800, 600, 32, SDL_SWSURFACE);
	SDL_WM_SetCaption("Trabalho 2 10/0126481", "Trabalho 2 10/0126481");
}

SDL_Surface* SDLBase::getScreen()
{
	return screen;
}

SDL_Surface* SDLBase::loadImage(string arquivo)
{
	SDL_Surface *surface = 0;
	SDL_Surface *surface_aux  = 0;

	// Carrega a imagem
	surface = IMG_Load(arquivo.c_str());

	// Verifica se h� transpar�ncia na imagem e otimiza ela p/ o tamanho da tela
	if (surface->format->Amask != 0)
		surface_aux = SDL_DisplayFormatAlpha(surface);
	else
		surface_aux = SDL_DisplayFormat(surface);

	// Limpa a surface n�o otimizada
	SDL_FreeSurface(surface);

	// Retorna a surface otimizada
	return surface_aux;
}



void SDLBase::renderSurface(SDL_Surface* surface, SDL_Rect* clip = NULL, SDL_Rect* dst = NULL)
{
	// surface � a superf�cie que ser� colada
	// clip � o ret�ngulo da superf�cie a ser colado
	// screen � a superf�cie destino
	// dst � ret�ngulo onde a superf�cie ser� colada

	// "Cola" a surface na tela (screen)
	SDL_BlitSurface(surface, clip, screen, dst);
}

void SDLBase::atualizarTela()
{
	// Atualiza a tela com as altera��es
	SDL_Flip(screen);
}



