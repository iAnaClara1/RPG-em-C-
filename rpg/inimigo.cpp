#include "inimigo.h"


Inimigo criarInimigo() {
    Inimigo inimigo;
    inimigo.simbolo = 'X';
    inimigo.ataque = 5;
    inimigo.nome = "Goblin";
    inimigo.arquetipo = arquetipos[rand() % 2];
    return inimigo;
}

Boss criarBoss() {
    Boss boss;
    boss.simbolo = 'B';
    boss.ataque = 12;
    boss.nome = "Olho Torto";
    boss.arquetipo = arquetipos[rand() % 2];
    return boss;
}

void inicializarInimigo(Inimigo &inimigo) {
    inimigo.vida = 5;
    inimigo.ataque = 5;
    inimigo.arquetipo = arquetipos[rand() % 2];
}

void inicializarBoss(Boss &boss) {
    boss.vida = 50;
    boss.ataque = 12;
    boss.arquetipo = arquetipos[rand() % 2];
}
