#include "personagem.h"

using namespace std;

Arquetipo arquetipos[3] = {
    {"Mago", "Golpe Poderoso", 10, 1},
    {"Paladino", "Explosão Mágica", 10, 2},
    {"Elfo", "Ataque Furtivo", 20, 3},
};

Personagem inicializarPersonagem(const string &nome, int ataque, int vidaMaxima, const string &arma, const string &classe, int ponto)
{
    Personagem novoPersonagem;
    novoPersonagem.nome = nome;
    novoPersonagem.ataque = ataque;
    novoPersonagem.ataqueAtual = ataque;
    novoPersonagem.defesa = 0;
    novoPersonagem.vidaMaxima = vidaMaxima;
    novoPersonagem.vidaAtual = vidaMaxima;
    novoPersonagem.arma = arma;
    novoPersonagem.classe = classe;
    novoPersonagem.ponto = ponto;
    return novoPersonagem;
}
