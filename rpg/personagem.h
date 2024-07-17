#ifndef PERSONAGEM_H
#define PERSONAGEM_H

using namespace std;

#include <string>

struct Sangramento
{
    int danoPorRodada;
    int duracao;
};

struct Arquetipo
{
    string nome;
    string nomeAtaqueEspecial;
    int danoAtaqueEspecial;
    int id;
};

struct Item
{
    string nome;
    int quantidade;
    Item *ant;
    Item *prox;
};

struct Inventario
{
    Item *cabeca;
};

struct Personagem
{
    string nome;
    int ataque;
    int ataqueAtual;
    int defesa;
    int defesaAtual;
    int vidaAtual;
    int vidaMaxima;
    int exp;
    int nivel;
    string arma;
    string classe;
    int ponto;
    Arquetipo arquetipo;
    Sangramento sangramento;
    Inventario inventario;
};

#endif
