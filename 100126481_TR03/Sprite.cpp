/*
 * Sprite.cpp
 *
 *  Created on: 16/04/2013
 *      Author: Victor
 */

#include "Sprite.h"

Sprite::Sprite(string arquivo)
{
	surface = NULL;
	surfaceRotoZoom = NULL;
	angle = 0;
	scaleX = 1;
	scaleY = 1;
	load(arquivo);
}

Sprite::~Sprite()
{
	if (surface != NULL)
		SDL_FreeSurface(surface);
}

void Sprite::load(string arquivo)
{
	// Limpa a surface se n�o vazia
	if (surface != NULL)
		SDL_FreeSurface(surface);

	// Carrega a imagem na surface
	surface = SDLBase::loadImage(arquivo);

	// Define o ret�ngulo de clipping (mostrando a imagem toda)
	clipRect.x = 0;
	clipRect.y = 0;
	clipRect.w = surface->w;
	clipRect.h = surface->h;
}

void Sprite::clip(int x, int y, int w, int h)
{
	// Define o ret�ngulo de clipping
	clipRect.x = x;
	clipRect.y = y;
	clipRect.w = w;
	clipRect.h = h;

	// Caso o clip seja	alterado e a
	// surfaceRotoZoom n�o seja nula, devemos
	// recalcular as transforma��es. Para isso, basta
	// chamar o m�todo rotoZoom com os par�metros
	// atuais e a vari�vel force == true.
	if(surfaceRotoZoom)
		rotoZoom(angle, scaleX, scaleY, true);
}

SDL_Rect* Sprite::getClip()
{
	return &clipRect;
}

void Sprite::render(int x, int y)
{
	SDL_Rect dst;

	// Temos 2 surfaces diferentes em
	// cada Sprite, temos que decidir qual mostrar na tela.
	// Para isso, basta checar	se a surfaceRotoZoom existe.
	// Se existir, mostramos ela, sen�o mostramos a normal.
	if(surfaceRotoZoom)
	{
		// Ao mostrar na tela a surfaceRotoZoom
		// ser� necess�rio fazer um ajuste de posi��o,
		// pois a rota��o da imagem n�o � feita �pelo
		// centro�. Para isso basta calcular a diferen�a
		// entre o ponto central do cliprect e o ponto
		// central da surfaceRotoZoom e adicionar esse
		// valor � posi��o na hora de mostrar na tela.
		Sint16 xOffset = clipRect.w/2 - surfaceRotoZoom->w/2;
		Sint16 yOffset = clipRect.h/2 - surfaceRotoZoom->h/2;

		dst.x = x + xOffset;
		dst.y = y + yOffset;
		dst.w = 0;
		dst.h = 0;

		// Cria um novo clipRect de acordo com o tamanho da surfaceRotoZoom.
		// N�o sei se a solu��o adequada, mas funciona.
		SDL_Rect newClipRect = clipRect;
		newClipRect.w = surfaceRotoZoom->w;
		newClipRect.h = surfaceRotoZoom->h;

		SDLBase::renderSurface(surfaceRotoZoom, &newClipRect, &dst);
	}
	else
	{
		dst.x = x;
		dst.y = y;
		dst.w = 0;
		dst.h = 0;

		SDLBase::renderSurface(surface, &clipRect, &dst);
	}
}

int Sprite::getWidth()
{
	return surface->w;
}

int Sprite::getHeight()
{
	return surface->h;
}

void Sprite::restore()
{
	// Deleta a surfaceRotoZoom.
	if (surfaceRotoZoom != NULL)
		SDL_FreeSurface(surfaceRotoZoom);
}

void Sprite::rotoZoom(float angle, float scaleX, float scaleY, bool force)
{
	// Checa se � necess�rio fazer alguma altera��o na imagem, ou seja, se angle,
	// scalex ou scaley s�o diferentes dos atuais. Caso sejam, � necess�rio
	// rotacionar. Se force == true, a rota��o tamb�m deve ser feita, independente
	// dos par�metros passados.
	if(this->angle != angle || this->scaleX != scaleX || this->scaleY != scaleY || force)
	{
		// Libera a surfaceRotoZoom anterior, caso houver.
		if(surfaceRotoZoom)
			SDL_FreeSurface(surfaceRotoZoom);

		// Atribui os par�metros angulo, scalex e scaley.
		this->angle = angle;
		this->scaleX = scaleX;
		this->scaleY = scaleY;

		// O m�todo blit torna indefinidos os valores de w e h do Rect. Para
		// resolver isso, basta fazer uma c�pia do rect e passar essa copia para o
		// m�todo clip.
		SDL_Rect newClipRect = clipRect;

		// Confere se qualquer um dos valores do cliprect foi alterado, ou seja, se a
		// surface usa clip. Caso use, � necess�rio �clipar� a surface antes de alter�-la.
		if(clipRect.x != 0 || clipRect.y != 0 || clipRect.w != surface->w || clipRect.h != surface->h)
		{
			// TODO Ver se isso d� leak de mem�ria.
			surfaceRotoZoom = SDLBase::clip(surface, &newClipRect);
			surfaceRotoZoom = SDLBase::rotoZoom(surfaceRotoZoom, angle, scaleX, scaleY);
		}
		else
			surfaceRotoZoom = SDLBase::rotoZoom(surface, angle, scaleX, scaleY);
	}
}
