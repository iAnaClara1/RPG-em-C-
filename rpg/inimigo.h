#ifndef INIMIGO_H
#define INIMIGO_H

#include <string>

struct Inimigo {
    int x;
    int y;
    char simbolo;
    std::string nome;
    int ataque;
    int vida;
    int nivel;
    Sangramento sangramento;
    Arquetipo arquetipo;
};

struct Boss {
    int x;
    int y;
    char simbolo;
    std::string nome;
    int ataque;
    int vida;
    int nivel;
    Sangramento sangramento;
    Arquetipo arquetipo;
};

// Funções que retornam objetos predefinidos de Inimigos
Inimigo criarInimigo();
Boss criarBoss();

#endif // INIMIGO_H
