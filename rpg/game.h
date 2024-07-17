#ifndef GAME_H
#define GAME_H

#include "personagem.h"

void roleEditor();
void drawNewGame(Personagem& novoPersonagem, Inventario &inventario, Item &item);
void drawNewGame2(Personagem& novoPersonagem, Inventario &inventario, Item &item);
void aplicarSangramentoPersonagem(Personagem& personagem, int dano, int duracao);
char verificaInimigo(int y, int x, int& inimigoY, int& inimigoX, int distancia);


#endif
