/*
 * StateManager.cpp
 *
 *  Created on: 13/06/2013
 *      Author: Victor
 */

#include "StateManager.h"

StateManager::StateManager()
{
	inputManager = NULL;
	stack = 0;

	// Inicializa a SDL
	SDLBase::loadSDL();

	timer = new Timer();
	timer->start();
	dt = 0;

	currentState = new StateMenu();
	currentState->load();

	// D� o primeiro update na InputManager
	inputManager->getInstance().update();
}

StateManager::~StateManager()
{
	SDLBase::unloadSDL();
}

void StateManager::run()
{
	bool quit = false;

	while(!quit)
	{
		// Calculo de Timer e dt
		//dt = timer->getTime();
		dt = 2;
		timer->start();

		// Atualiza��o da InputManager � deve ser chamado somente aqui
		inputManager->getInstance().update();

		// STATE MANAGER
		switch(currentState->update(dt))
		{
			case STATE:
				// Nothing to see here
			break;
			case STATE_MENU:
				stack = currentState->unload();
				currentState = new StateMenu();
				currentState->load(stack);
			break;
			case STATE_GAME:
				stack = currentState->unload();
				currentState = new StateGame();
				currentState->load(stack);
			break;
			case STATE_WINLOSE:
				stack = currentState->unload();
				currentState = new StateWinLose();
				currentState->load(stack);
			break;
			case STATE_QUIT:
				quit = true;
			break;
		}

		// Atualiza��o dos objetos na tela
		currentState->render();

		// Atualizar a tela
		SDLBase::atualizarTela();

		// Delay
		//SDL_Delay(13);
	}
}

