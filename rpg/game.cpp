#include <ncurses.h>
#include <fstream>
#include <string>
#include <cstring>
#include <ctime>
#include "personagem.h"
#include "personagem.cpp"
#include "game.h"
#include "inimigo.cpp"
#include "inimigo.h"
#include <iostream>
#define MAXTAM 2

using namespace std;

struct Ataque
{
    string nome; // nome do atacante 
    int dano;    // quantidade do ataque
};

struct Fila
{
    int topo;
    Ataque ataques[MAXTAM];
};

void atualizaInfoBox(WINDOW *infoBox, Personagem &personagem)
{
    mvwprintw(infoBox, 4, 1, ("Vida: " + to_string(personagem.vidaAtual) + "  ").c_str()); // atualiza a vida
    wrefresh(infoBox);
}

int rolarD20()
{
    return rand() % 20 + 1;
}
void aplicarSangramentoPersonagem(Personagem &personagem, int dano, int duracao)
{
    personagem.sangramento.danoPorRodada = dano;
    personagem.sangramento.duracao = duracao;
}

void processarSangramentoPersonagem(Personagem &personagem, WINDOW *janela, WINDOW *janelaCombate, WINDOW *infoBox)
{
    if (personagem.sangramento.duracao > 0)
    {
        mvwprintw(janelaCombate, 1, 0, "%s está sangrando e perde %d de vida.", personagem.nome.c_str(), personagem.sangramento.danoPorRodada);
        personagem.vidaAtual -= personagem.sangramento.danoPorRodada;
        atualizaInfoBox(infoBox, personagem);
        personagem.sangramento.duracao--;
    }
}

void aplicarSangramentoInimigo(Inimigo &inimigo, int dano, int duracao)
{
    inimigo.sangramento.danoPorRodada = dano;
    inimigo.sangramento.duracao = duracao;
}

void aplicarSangramentoBoss(Boss &boss, int dano, int duracao)
{
    boss.sangramento.danoPorRodada = dano;
    boss.sangramento.duracao = duracao;
}

void processarSangramentoInimigo(Inimigo &inimigo, WINDOW *janela, WINDOW *janelaCombate, WINDOW *infoBox)
{
    if (inimigo.sangramento.duracao > 0)
    {
        mvwprintw(janelaCombate, 1, 0, "%s está sangrando e perde %d de vida.", inimigo.nome.c_str(), inimigo.sangramento.danoPorRodada);
        inimigo.vida -= inimigo.sangramento.danoPorRodada;
        inimigo.sangramento.duracao--;
    }
}

void processarSangramentoBoss(Boss &boss, WINDOW *janela, WINDOW *janelaCombate, WINDOW *infoBox)
{
    if (boss.sangramento.duracao > 0)
    {
        mvwprintw(janelaCombate, 1, 0, "%s está sangrando e perde %d de vida.", boss.nome.c_str(), boss.sangramento.danoPorRodada);
        boss.vida -= boss.sangramento.danoPorRodada;
        boss.sangramento.duracao--;
    }
}

bool ataqueEspecialPersonagem(Ataque &ataquePersonagem, Personagem &personagem, Arquetipo *arquetipos, Inimigo &inimigo)
{
    int chance = rand() % 100 + 1;
    if (personagem.arquetipo.id == 1)
    {
        if (chance <= 65)
        {
            ataquePersonagem.dano += personagem.arquetipo.danoAtaqueEspecial;

            if (rand() % 2 == 0)
            {
                aplicarSangramentoInimigo(inimigo, rand() % 5 + 1, rand() % 6 + 1);
            }
        }
        return true; //  o ataque especial ocorreu
    }
    else if (personagem.arquetipo.id != 1 && chance <= 20)
    {
        ataquePersonagem.dano += personagem.arquetipo.danoAtaqueEspecial;

        if (rand() % 2 == 0)
        {
            aplicarSangramentoInimigo(inimigo, rand() % 5 + 1, rand() % 6 + 1);
        }
        return true; // o ataque especial ocorreu
    }
    else
    {
        return false; // o ataque especial não ocorreu
    }
}

bool ataqueEspecialPersonagem1(Ataque &ataquePersonagem, Personagem &personagem, Arquetipo *arquetipos, Boss &boss)
{
    int chance = rand() % 100 + 1;
    if (personagem.arquetipo.id == 1)
    {
        if (chance <= 65)
        {
            ataquePersonagem.dano += personagem.arquetipo.danoAtaqueEspecial;

            if (rand() % 2 == 0)
            {
                aplicarSangramentoBoss(boss, rand() % 5 + 1, rand() % 6 + 1);
            }
        }
        return true; // o ataque especial ocorreu
    }
    else if (personagem.arquetipo.id != 1 && chance <= 20)
    {
        ataquePersonagem.dano += personagem.arquetipo.danoAtaqueEspecial;

        if (rand() % 2 == 0)
        {
            aplicarSangramentoBoss(boss, rand() % 5 + 1, rand() % 6 + 1);
        }
        return true; // o ataque especial ocorreu
    }
    else
    {
        return false; // o ataque especial não ocorreu
    }
}

bool ataqueEspecialInimigo(Ataque &ataqueInimigo, Inimigo &inimigo, Personagem &personagem)
{
    int chance = rand() % 100 + 1;
    if (inimigo.arquetipo.id == 1)
    {
        if (chance <= 65)
        {
            ataqueInimigo.dano += inimigo.arquetipo.danoAtaqueEspecial;

            if (rand() % 2 == 0)
            {
                aplicarSangramentoPersonagem(personagem, rand() % 5 + 1, rand() % 6 + 1);
            }
        }
        return true; // o ataque especial ocorreu
    }
    else if (inimigo.arquetipo.id != 1 && chance <= 20)
    {
        ataqueInimigo.dano += inimigo.arquetipo.danoAtaqueEspecial;

        if (rand() % 2 == 0)
        {
            aplicarSangramentoPersonagem(personagem, rand() % 5 + 1, rand() % 6 + 1);
        }
        return true; // o ataque especial ocorreu
    }
    else
    {
        return false; // o ataque especial não ocorreu
    }
}

bool ataqueEspecialBoss(Ataque &ataqueBoss, Boss &boss, Personagem &personagem)
{
    int chance = rand() % 100 + 1;
    if (boss.arquetipo.id == 1)
    {
        if (chance <= 65)
        {
            ataqueBoss.dano += boss.arquetipo.danoAtaqueEspecial;

            if (rand() % 2 == 0)
            {
                aplicarSangramentoPersonagem(personagem, rand() % 5 + 1, rand() % 6 + 1);
            }
        }
        return true; // o ataque especial ocorreu
    }
    else if (boss.arquetipo.id != 1 && chance <= 20)
    {
        ataqueBoss.dano += boss.arquetipo.danoAtaqueEspecial;

        if (rand() % 2 == 0)
        {
            aplicarSangramentoPersonagem(personagem, rand() % 5 + 1, rand() % 6 + 1);
        }
        return true; // o ataque especial ocorreu
    }
    else
    {
        return false; // o ataque especial não ocorreu
    }
}

// funções de inventário

void desenhaInventario(WINDOW* infoBoxInventario, Item* cabeca) {
    werase(infoBoxInventario);  // limpa a janela do inventário
    box(infoBoxInventario, 0, 0); // redesenha a borda

    mvwprintw(infoBoxInventario, 1, 1, "Inventário:");
    int linha = 2;
    Item* atual = cabeca;
    while (atual != nullptr) {
        mvwprintw(infoBoxInventario, linha, 1, "%s x%d", atual->nome.c_str(), atual->quantidade);
        linha++;
        atual = atual->prox;
    }

    wrefresh(infoBoxInventario);  // atualiza a exibição da janela do inventário
}


void insereItem(Item **cabeca, string nome, int quantidade, WINDOW *infoBoxInventario)
{
    // verifica se já existem 5 itens na lista
    int contadorItens = 0;
    Item *atual = *cabeca;
    while (atual != nullptr)
    {
        contadorItens++;
        atual = atual->prox;
    }

    if (contadorItens >= 5)
    {
        mvwprintw(infoBoxInventario, 0, 0, "Inventário cheio!");
        wrefresh(infoBoxInventario); // adicionei para atualizar a janela
        return;
    }

    // cria um novo item e insere no início da lista
    Item *novo = new Item;
    novo->nome = nome;
    novo->quantidade = quantidade;
    novo->ant = nullptr;
    novo->prox = *cabeca;

    if (*cabeca != nullptr)
    {
        (*cabeca)->ant = novo;
    }

    *cabeca = novo;
    desenhaInventario(infoBoxInventario, *cabeca); // atualiza o inventário
}

void removeItem(Item **cabeca, string nome)
{
    Item *atual = *cabeca;

    // procura pelo item com o nome especificado
    while (atual != nullptr && atual->nome != nome)
    {
        atual = atual->prox;
    }

    // se o item foi encontrado, remove-o da lista
    if (atual != nullptr)
    {
        // atualiza os ponteiros dos nós vizinhos
        if (atual->ant != nullptr)
        {
            atual->ant->prox = atual->prox;
        }
        if (atual->prox != nullptr)
        {
            atual->prox->ant = atual->ant;
        }

        // verifica se o item removido é a cabeça da lista
        if (*cabeca == atual)
        {
            *cabeca = atual->prox;
        }

        // libera a memória do item removido
        delete atual;
    }
}

void drawCombatMenu(WINDOW *janelaCombate, Item **cabeca, WINDOW *infoBox, WINDOW *infoBoxInventario, Personagem &personagem)
{
    keypad(janelaCombate, TRUE);
    int largura, altura;
    getmaxyx(janelaCombate, altura, largura);

    int key;
    int choice = 0;

    while (true)
    {
        werase(janelaCombate);
        box(janelaCombate, 0, 0); // desenha a borda

        int numItems = 0;
        int larguraItens = 0;
        Item *atual = *cabeca;

        // conta quantos itens existem na lista e calcula a largura total necessária
        while (atual != nullptr)
        {
            numItems++;
            larguraItens += atual->nome.length() + 3; // espaço extra entre os itens
            atual = atual->prox;
        }

        // verifica se o inventário está vazio
        if (numItems == 0)
        {
            mvwprintw(janelaCombate, 1, 1, "Inventário vazio.");
            wrefresh(janelaCombate);
            key = wgetch(janelaCombate);
            werase(janelaCombate);
            wrefresh(janelaCombate);
            return;
        }

        // define a posição inicial para centralizar os itens horizontalmente
        int startX = (largura - larguraItens) / 2;
        int x = startX;
        int y = 1;

        // reinicia para o início da lista para o próximo ciclo
        atual = *cabeca;

        // desenha os itens do inventário como opções
        for (int i = 0; i < numItems; i++)
        {
            if (atual != nullptr)
            {
                if (i == choice)
                {
                    wattron(janelaCombate, A_STANDOUT);
                }
                mvwprintw(janelaCombate, y, x, "%s", atual->nome.c_str());
                if (i == choice)
                {
                    wattroff(janelaCombate, A_STANDOUT);
                }
                x += atual->nome.length() + 3; // espaço extra entre os itens
                atual = atual->prox;
            }
        }

        wrefresh(janelaCombate);

        key = wgetch(janelaCombate);
        switch (key)
        {
        case KEY_LEFT:
            choice = (choice == 0) ? numItems - 1 : choice - 1;
            break;
        case KEY_RIGHT:
            choice = (choice == numItems - 1) ? 0 : choice + 1;
            break;
        case 10: // enter
            // o item que foi escolhido
            if (choice < numItems && *cabeca != nullptr)
            {
                // encontra o item escolhido
                Item *itemEscolhido = *cabeca;
                for (int i = 0; i < choice; i++)
                {
                    if (itemEscolhido != nullptr)
                    {
                        itemEscolhido = itemEscolhido->prox;
                    }
                }

                if (itemEscolhido != nullptr)
                {
                    if (itemEscolhido->nome == "Poção de Vida")
                    {
                        personagem.vidaAtual += 10;
                        personagem.ponto += 1;
                    }
                    if (itemEscolhido->nome == "Poção de Ataque")
                    {
                        personagem.ataqueAtual += 10;
                        personagem.ponto += 1;
                    }
                    if (itemEscolhido->nome == "Tortinha de Limão")
                    {
                        personagem.vidaAtual += 10;
                        personagem.ataqueAtual += 10;
                        personagem.ponto += 2;
                    }
                    if (itemEscolhido->nome == "Moeda")
                    {
                        personagem.ponto += 5;
                    }

                    mvwprintw(janelaCombate, altura - 2, 1, "Usando item: %s", itemEscolhido->nome.c_str());
                    wrefresh(janelaCombate);
                    removeItem(cabeca, itemEscolhido->nome);
                    desenhaInventario(infoBoxInventario, *cabeca); // Atualiza o inventário
                    key = wgetch(janelaCombate);
                    werase(janelaCombate);
                    wrefresh(janelaCombate);

                    // atualize o infoBox
                    werase(infoBox);
                    box(infoBox, 0, 0);
                    mvwprintw(infoBox, 1, 1, ("Nome: " + personagem.nome).c_str());
                    mvwprintw(infoBox, 2, 1, ("Classe: " + personagem.classe).c_str());
                    mvwprintw(infoBox, 3, 1, ("Ataque: " + to_string(personagem.ataqueAtual)).c_str());
                    mvwprintw(infoBox, 4, 1, ("Vida: " + to_string(personagem.vidaAtual)).c_str());
                    wrefresh(infoBox);

                    // após remover o item, verifica novamente o número de itens
                    numItems = 0;
                    atual = *cabeca;
                    while (atual != nullptr)
                    {
                        numItems++;
                        atual = atual->prox;
                    }

                    // se o inventário ficar vazio, saia do menu
                    if (numItems == 0)
                    {
                        mvwprintw(janelaCombate, 1, 1, "Inventário vazio.");
                        wrefresh(janelaCombate);
                        key = wgetch(janelaCombate); // Espera por qualquer tecla
                        werase(janelaCombate);
                        wrefresh(janelaCombate);
                        return;
                    }

                    // redefine atual para a cabeça da lista após remover o item
                    atual = *cabeca;
                }
            }
            break;

        case 'q': // saída
            key = wgetch(janelaCombate);
            werase(janelaCombate);
            wrefresh(janelaCombate);
            return; // sai do menu de combate
        }
    }
}

// funções de luta
void iniciarLuta(Fila *f)
{
    f->topo = -1;
}

bool filaVazia(Fila *f)
{
    return f->topo == -1;
}

bool filaCheia(Fila *f)
{
    return f->topo == MAXTAM - 1;
}

bool enfileirar(Fila *f, Ataque ataque)
{
    if (filaCheia(f))
    {
        return false;
    }
    f->topo++;
    f->ataques[f->topo] = ataque;
    return true;
}

bool desenfileirar(Fila *f, Ataque *ataque)
{
    if (filaVazia(f))
    {
        return false;
    }
    *ataque = f->ataques[0];
    for (int i = 0; i < f->topo; i++)
    {
        f->ataques[i] = f->ataques[i + 1];
    }
    f->topo--;
    return true;
}

void luta(WINDOW *janela, WINDOW *janelaCombate, WINDOW *infoBox, Personagem &personagem, Inimigo &inimigo, Fila *f)
{
    iniciarLuta(f);
    srand(time(0));
    inicializarInimigo(inimigo);
    inimigo.sangramento.duracao = 0;
    inimigo.sangramento.danoPorRodada = 0;

    int largura = getmaxx(janelaCombate); // obtém a largura da janela

    while (personagem.vidaAtual > 0 && inimigo.vida > 0)
    {
        bool ocorreuAtaqueEspecialPersonagem = false;
        bool ocorreuAtaqueEspecialInimigo = false;

        wrefresh(janelaCombate);
        processarSangramentoPersonagem(personagem, janela, janelaCombate, infoBox);
        processarSangramentoInimigo(inimigo, janela, janelaCombate, infoBox);

        Ataque ataquePersonagem;
        ataquePersonagem.nome = personagem.nome;
        ataquePersonagem.dano = personagem.ataqueAtual;

        // personagem realiza o ataque
        int rdn = rolarD20();
        if (rdn >= 1 && rdn <= 3)
        {
            wrefresh(janelaCombate);
            ataquePersonagem.dano = 0;
        }
        else
        {
            ocorreuAtaqueEspecialPersonagem = ataqueEspecialPersonagem(ataquePersonagem, personagem, arquetipos, inimigo);
        }

        if (!enfileirar(f, ataquePersonagem))
        {
        }

        Ataque ataqueInimigo;
        ataqueInimigo.nome = inimigo.nome;
        ataqueInimigo.dano = inimigo.ataque; 

        // inimigo realiza o ataque
        rdn = rolarD20();
        if (rdn >= 1 && rdn <= 3)
        {
            wrefresh(janelaCombate);
            ataqueInimigo.dano = 0;
        }
        else
        {
            ocorreuAtaqueEspecialInimigo = ataqueEspecialInimigo(ataqueInimigo, inimigo, personagem);
        }

        if (!enfileirar(f, ataqueInimigo))
        {
        }

        // executa os ataques enfileirados
        while (!filaVazia(f))
        {
            Ataque ataque;
            if (desenfileirar(f, &ataque))
            {
                if (ataque.nome == personagem.nome)
                {
                    // personagem ataca
                    if (ataque.dano > 0)
                    {
                        if (ocorreuAtaqueEspecialPersonagem)
                        {
                            inimigo.vida -= ataque.dano;
                            if (inimigo.vida < 0)
                                inimigo.vida = 0;
                            mvwhline(janelaCombate, 2, 0, ' ', largura); // limpa a linha 2 da janelaCombate com espaços em branco
                            wrefresh(janelaCombate);
                            mvwprintw(janelaCombate, 2, 0, "%s foi atacado com poder especial = %d depois do ataque", inimigo.nome.c_str(), inimigo.vida);
                            wrefresh(janelaCombate);
                            wgetch(janelaCombate);
                        }
                        else
                        {
                            inimigo.vida -= ataque.dano;
                            if (inimigo.vida < 0)
                                inimigo.vida = 0;
                            mvwhline(janelaCombate, 2, 0, ' ', largura); // limpa a linha 2 da janelaCombate com espaços em branco
                            wrefresh(janelaCombate);
                            mvwprintw(janelaCombate, 2, 0, "%s foi atacado = %d depois do ataque", inimigo.nome.c_str(), inimigo.vida);
                            wrefresh(janelaCombate);
                            wgetch(janelaCombate);
                        }

                        if (inimigo.vida <= 0)
                        {
                            werase(janelaCombate);
                            mvwprintw(janelaCombate, 0, 0, "Você derrotou o %s!", inimigo.nome.c_str());
                            wrefresh(janelaCombate);
                            wgetch(janelaCombate);
                            werase(janelaCombate);
                            wrefresh(janelaCombate);
                            break;
                        }
                    }
                    else
                    {
                        mvwhline(janelaCombate, 2, 0, ' ', largura); //limpa a linha 2 da janelaCombate com espaços em branco
                        wrefresh(janelaCombate);
                        mvwprintw(janelaCombate, 2, 0, "%s errou o ataque", personagem.nome.c_str());
                        wrefresh(janelaCombate);
                        wgetch(janelaCombate);
                    }
                }
                else if (ataque.nome == inimigo.nome)
                {
                    // inimigo ataca
                    if (ataque.dano > 0)
                    {
                        if (ocorreuAtaqueEspecialInimigo)
                        {
                            personagem.vidaAtual -= ataque.dano;
                            if (personagem.vidaAtual < 0)
                                personagem.vidaAtual = 0;
                            atualizaInfoBox(infoBox, personagem);
                            mvwhline(janelaCombate, 4, 0, ' ', largura);
                            wrefresh(janelaCombate);
                            mvwprintw(janelaCombate, 4, 0, "%s foi atacado com poder especial = %d depois do ataque", personagem.nome.c_str(), personagem.vidaAtual);
                            wrefresh(janelaCombate);
                            wgetch(janelaCombate);
                        }
                        else
                        {
                            personagem.vidaAtual -= ataque.dano;
                            if (personagem.vidaAtual < 0)
                                personagem.vidaAtual = 0;
                            atualizaInfoBox(infoBox, personagem);
                            mvwhline(janelaCombate, 4, 0, ' ', largura);
                            wrefresh(janelaCombate);
                            mvwprintw(janelaCombate, 4, 0, "%s foi atacado = %d depois do ataque", personagem.nome.c_str(), personagem.vidaAtual);
                            wrefresh(janelaCombate);
                            wgetch(janelaCombate); 
                        }

                        if (personagem.vidaAtual <= 0)
                        {
                            werase(janelaCombate);
                            wrefresh(janelaCombate);
                            mvwprintw(janelaCombate, 3, 0, "Você foi derrotado pelo inimigo!");
                            wrefresh(janelaCombate);
                            wgetch(janelaCombate);
                            werase(janelaCombate);
                            wrefresh(janelaCombate);
                            break;
                        }
                    }
                    else
                    {
                        mvwhline(janelaCombate, 4, 0, ' ', largura);
                        wrefresh(janelaCombate);
                        mvwprintw(janelaCombate, 4, 0, "%s errou o ataque", inimigo.nome.c_str());
                        wrefresh(janelaCombate);
                        wgetch(janelaCombate);
                        werase(janelaCombate);
                        wrefresh(janelaCombate);
                    }
                }
            }
        }
    }
}


void luta2(WINDOW *janela, WINDOW *janelaCombate, WINDOW *infoBox, Personagem &personagem, Boss &boss, Fila *f)
{
    iniciarLuta(f);
    srand(time(0));
    inicializarBoss(boss);
    boss.sangramento.duracao = 0;
    boss.sangramento.danoPorRodada = 0;

    while (personagem.vidaAtual > 0 && boss.vida > 0)
    {
        bool ocorreuAtaqueEspecialPersonagem1 = false;
        bool ocorreuAtaqueEspecialBoss = false;

        wrefresh(janelaCombate);
        processarSangramentoPersonagem(personagem, janela, janelaCombate, infoBox);
        processarSangramentoBoss(boss, janela, janelaCombate, infoBox);

        Ataque ataquePersonagem;
        ataquePersonagem.nome = personagem.nome;
        ataquePersonagem.dano = personagem.ataqueAtual;

        // personagem realiza o ataque
        int rdn = rolarD20();
        if (rdn >= 1 && rdn <= 3)
        {
            wrefresh(janelaCombate);
            ataquePersonagem.dano = 0;
        }
        else
        {
            ocorreuAtaqueEspecialPersonagem1 = ataqueEspecialPersonagem1(ataquePersonagem, personagem, arquetipos, boss);
        }

        // enfileira o ataque do personagem
        if (!enfileirar(f, ataquePersonagem))
        {
        }

        Ataque ataqueBoss;
        ataqueBoss.nome = boss.nome;
        ataqueBoss.dano = boss.ataque;
        // boss realiza o ataque
        rdn = rolarD20();
        if (rdn >= 1 && rdn <= 3)
        {
            wrefresh(janelaCombate);
            ataqueBoss.dano = 0;
        }
        else
        {
            ocorreuAtaqueEspecialBoss = ataqueEspecialBoss(ataqueBoss, boss, personagem);
        }

        // enfileira o ataque do boss
        if (!enfileirar(f, ataqueBoss))
        {
        }

        // executa os ataques enfileirados
        while (!filaVazia(f))
        {
            Ataque ataque;
            if (desenfileirar(f, &ataque))
            {
                if (ataque.nome == personagem.nome)
                {
                    // personagem ataca
                    if (ataque.dano > 0)
                    {
                        if (ocorreuAtaqueEspecialPersonagem1)
                        {
                            boss.vida -= ataque.dano;
                            if (boss.vida < 0)
                                boss.vida = 0;
                            mvwprintw(janelaCombate, 2, 0, "%s foi atacado com poder especial e agora tem %d de vida", boss.nome.c_str(), boss.vida);
                            wrefresh(janelaCombate);
                            wgetch(janelaCombate);
                        }
                        else
                        {
                            boss.vida -= ataque.dano;
                            if (boss.vida < 0)
                                boss.vida = 0;
                            mvwprintw(janelaCombate, 2, 0, "%s foi atacado e agora tem %d de vida", boss.nome.c_str(), boss.vida);
                            wrefresh(janelaCombate);
                            wgetch(janelaCombate);
                        }

                        if (boss.vida <= 0)
                        {
                            mvwprintw(janelaCombate, 3, 0, "Você derrotou %s!", boss.nome.c_str());
                            wrefresh(janelaCombate);
                            werase(janelaCombate);
                            wrefresh(janelaCombate);
                            break;
                        }
                    }
                    else
                    {
                        werase(janelaCombate);
                        mvwprintw(janelaCombate, 2, 0, "%s errou o ataque", personagem.nome.c_str());
                        wrefresh(janelaCombate);
                        wgetch(janelaCombate); 
                    }
                }
                else if (ataque.nome == boss.nome)
                {
                    // Boss ataca
                    if (ataque.dano > 0)
                    {
                        if (ocorreuAtaqueEspecialBoss)
                        {
                            personagem.vidaAtual -= ataque.dano;
                            if (personagem.vidaAtual < 0)
                                personagem.vidaAtual = 0;
                            atualizaInfoBox(infoBox, personagem);
                            mvwprintw(janelaCombate, 4, 0, "%s foi atacado com poder especial e agora tem %d de vida", personagem.nome.c_str(), personagem.vidaAtual);
                            wrefresh(janelaCombate);
                            wgetch(janelaCombate);
                        }
                        else
                        {
                            personagem.vidaAtual -= ataque.dano;
                            if (personagem.vidaAtual < 0)
                                personagem.vidaAtual = 0;
                            atualizaInfoBox(infoBox, personagem);
                            mvwprintw(janelaCombate, 4, 0, "%s foi atacado e agora tem %d de vida", personagem.nome.c_str(), personagem.vidaAtual);
                            wrefresh(janelaCombate);
                            wgetch(janelaCombate);
                        }

                        if (personagem.vidaAtual <= 0)
                        {
                            mvwprintw(janelaCombate, 3, 0, "Você foi derrotado pelo %s!", boss.nome.c_str());
                            wrefresh(janelaCombate);
                            werase(janelaCombate);
                            wrefresh(janelaCombate);
                            break;
                        }
                    }
                    else
                    {
                        mvwprintw(janelaCombate, 4, 0, "%s errou o ataque", boss.nome.c_str());
                        wrefresh(janelaCombate);
                        wgetch(janelaCombate);
                    }
                }
            }
        }
    }
}

void roleEditor()
{
    int attack;
    int health;
    int currentHealth;
    string weapon;
    int point;

    int cols, rows;
    getmaxyx(stdscr, rows, cols);
    char key = -1;
    int chosen = 0;
    int firstline = (rows * 0.1);

    string nameJogador;

    while (key != 10)
    { // 10 = enter
        clear();
        getmaxyx(stdscr, rows, cols);

        mvaddstr(firstline, (cols / 2) - 36, "      ::::::::       :::            :::        ::::::::       ::::::::       ::::::::::          ");
        mvaddstr(firstline + 1, (cols / 2) - 36, "    :+:    :+:      :+:          :+: :+:     :+:    :+:     :+:    :+:      :+:              :+: ");
        mvaddstr(firstline + 2, (cols / 2) - 36, "   +:+             +:+         +:+   +:+    +:+            +:+             +:+                   ");
        mvaddstr(firstline + 3, (cols / 2) - 36, "  +#+             +#+        +#++:++#++:   +#++:++#++     +#++:++#++      +#++:++#               ");
        mvaddstr(firstline + 4, (cols / 2) - 36, " +#+             +#+        +#+     +#+          +#+            +#+      +#+                     ");
        mvaddstr(firstline + 5, (cols / 2) - 36, "#+#    #+#      #+#        #+#     #+#   #+#    #+#     #+#    #+#      #+#              #+#     ");
        mvaddstr(firstline + 6, (cols / 2) - 36, "########       ########## ###     ###    ########       ########       ##########");

        // recognize arrow keys
        if (key == 4 || key == 3)
        { // LEFT or UP
            (chosen == 0) ? chosen = 2 : chosen--;
        }
        if (key == 5 || key == 2)
        { // RIGHT or DOWN
            (chosen == 2) ? chosen = 0 : chosen++;
        }

        // draw role logos
        if (chosen == 0)
        {
            attron(A_STANDOUT);
        }
        mvaddstr(firstline + 8, (cols / 2) - 29, "             ");
        mvaddstr(firstline + 9, (cols / 2) - 29, "   ^^        ");
        mvaddstr(firstline + 10, (cols / 2) - 29, " < ** >      ");
        mvaddstr(firstline + 11, (cols / 2) - 29, "   ||   '    ");
        mvaddstr(firstline + 12, (cols / 2) - 29, "   || ' __ ' ");
        mvaddstr(firstline + 13, (cols / 2) - 29, "   ||  /*.\\  ");
        mvaddstr(firstline + 14, (cols / 2) - 29, "   ||  \\__/  ");
        mvaddstr(firstline + 15, (cols / 2) - 29, "   || '      ");
        mvaddstr(firstline + 16, (cols / 2) - 29, "   ||    '   ");
        mvaddstr(firstline + 17, (cols / 2) - 29, "             ");
        if (chosen == 0)
        {
            attroff(A_STANDOUT);
            // draw description
            attron(A_BOLD);
            mvaddstr(firstline + 20, (cols / 2) - 20, "Mago");
            attroff(A_BOLD);
            mvaddstr(firstline + 22, (cols / 2) - 20, "Muito inteligente. Sua aprendizagem é impressionante.");
            mvaddstr(firstline + 24, (cols / 2) - 20, "Nivelamento:           150%");
            mvaddstr(firstline + 25, (cols / 2) - 20, "Pontos de habilidade:  10");
        }
        if (chosen == 1)
        {
            attron(A_STANDOUT);
        }
        mvaddstr(firstline + 8, (cols / 2) - 5, "  __________  ");
        mvaddstr(firstline + 9, (cols / 2) - 5, " |    ||    | ");
        mvaddstr(firstline + 10, (cols / 2) - 5, " |____||____| ");
        mvaddstr(firstline + 11, (cols / 2) - 5, " |____  ____| ");
        mvaddstr(firstline + 12, (cols / 2) - 5, " \\    ||    / ");
        mvaddstr(firstline + 13, (cols / 2) - 5, "  \\   ||   /  ");
        mvaddstr(firstline + 14, (cols / 2) - 5, "   \\  ||  /   ");
        mvaddstr(firstline + 15, (cols / 2) - 5, "    \\ || /    ");
        mvaddstr(firstline + 16, (cols / 2) - 5, "     \\||/     ");
        mvaddstr(firstline + 17, (cols / 2) - 5, "      \\/      ");
        if (chosen == 1)
        {
            attroff(A_STANDOUT);
            // draw description
            attron(A_BOLD);
            mvaddstr(firstline + 20, (cols / 2) - 20, "Paladino");
            attroff(A_BOLD);
            mvaddstr(firstline + 22, (cols / 2) - 20, "Forte e corajoso. Este é o paladino.");
            mvaddstr(firstline + 24, (cols / 2) - 20, "Nivelamento:           100%");
            mvaddstr(firstline + 25, (cols / 2) - 20, "Pontos de habilidade:  30");
        }
        if (chosen == 2)
        {
            attron(A_STANDOUT);
        }
        mvaddstr(firstline + 8, (cols / 2) + 17, "     (.      ");
        mvaddstr(firstline + 9, (cols / 2) + 17, "     |\\      ");
        mvaddstr(firstline + 10, (cols / 2) + 17, "     | \\     ");
        mvaddstr(firstline + 11, (cols / 2) + 17, "     | .\\    ");
        mvaddstr(firstline + 12, (cols / 2) + 17, " >---|--|--> ");
        mvaddstr(firstline + 13, (cols / 2) + 17, "     | ./    ");
        mvaddstr(firstline + 14, (cols / 2) + 17, "     | /     ");
        mvaddstr(firstline + 15, (cols / 2) + 17, "     |/      ");
        mvaddstr(firstline + 16, (cols / 2) + 17, "     (.      ");
        mvaddstr(firstline + 17, (cols / 2) + 17, "             ");
        if (chosen == 2)
        {
            attroff(A_STANDOUT);
            // draw description
            attron(A_BOLD);
            mvaddstr(firstline + 20, (cols / 2) - 20, "Elfo");
            attroff(A_BOLD);
            mvaddstr(firstline + 22, (cols / 2) - 20, "Universal e determinado. Todo inimigo deveria correr.");
            mvaddstr(firstline + 24, (cols / 2) - 20, "Nivelamento:           125%");
            mvaddstr(firstline + 25, (cols / 2) - 20, "Pontos de habilidade:  15");
        }
        key = getch();
    }

    // role selected, now select points
    key = -1;
    string name;
    int pointsLeft;
    int leveling;
    int pointsHealth = 0;
    int pointsAttack = 0;
    if (chosen == 0)
    {
        name = "Mago";
        pointsLeft = 20;
        leveling = 150;
    }
    if (chosen == 1)
    {
        name = "Paladino";
        pointsLeft = 30;
        leveling = 100;
    }
    if (chosen == 2)
    {
        name = "Elfo";
        pointsLeft = 15;
        leveling = 125;
    }
    chosen = 0;
    while (key != 10)
    {
        // clean under logos
        for (int i = 0; i < 10; i++)
        {
            mvaddstr(firstline + 21 + i, (cols / 2) - 36, "                                                                       ");
        }

        // recognize arrow keys
        if (key == 3)
        { // UP
            (chosen == 0) ? chosen = 1 : chosen--;
        }
        if (key == 2)
        { // DOWN
            (chosen == 1) ? chosen = 0 : chosen++;
        }
        if (key == 4)
        { // LEFT
            if (chosen == 0 && pointsHealth > 0)
            {
                pointsHealth--;
                pointsLeft++;
            }
            if (chosen == 1 && pointsAttack > 0)
            {
                pointsAttack--;
                pointsLeft++;
            }
        }
        if (key == 5)
        { // RIGHT
            if (chosen == 0 && pointsLeft > 0)
            {
                pointsHealth++;
                pointsLeft--;
            }
            if (chosen == 1 && pointsLeft > 0)
            {
                pointsAttack++;
                pointsLeft--;
            }
        }

        // show point bar
        mvaddstr(firstline + 20, (cols / 2) - 20, name.c_str());
        mvaddstr(firstline + 22, (cols / 2) - 20, "Pontos restantes:    ");
        mvprintw(firstline + 22, (cols / 2) - 2, to_string(pointsLeft).c_str());

        if (chosen == 0)
        {
            attron(A_STANDOUT);
        }
        mvaddstr(firstline + 24, (cols / 2) - 20, "Health:  ");
        mvprintw(firstline + 24, (cols / 2) - 10, to_string(pointsHealth).c_str());
        if (chosen == 0)
        {
            attroff(A_STANDOUT);
        }

        if (chosen == 1)
        {
            attron(A_STANDOUT);
        }
        mvaddstr(firstline + 25, (cols / 2) - 20, "Ataque:  ");
        mvprintw(firstline + 25, (cols / 2) - 10, to_string(pointsAttack).c_str());
        if (chosen == 1)
        {
            attroff(A_STANDOUT);
        }

        key = getch();
    }
    attack = pointsAttack;
    health = pointsHealth;
    currentHealth = health;
    weapon = "Padrão";

    // final screen
    key = -1;
    while (key != 10)
    {
        // clean under logos
        for (int i = 0; i < 10; i++)
        {
            mvaddstr(firstline + 21 + i, (cols / 2) - 36, "                                                                       ");
        }

        mvaddstr(firstline + 20, (cols / 2) - 6, "BEM-VINDO(A)");
        mvaddstr(firstline + 22, (cols / 2) - 9, name.c_str());

        // draw role logos
        if (name == "Mago")
        {
            mvaddstr(firstline + 8, (cols / 2) - 29, "             ");
            mvaddstr(firstline + 9, (cols / 2) - 29, "   ^^        ");
            mvaddstr(firstline + 10, (cols / 2) - 29, " < ** >      ");
            mvaddstr(firstline + 11, (cols / 2) - 29, "   ||   '    ");
            mvaddstr(firstline + 12, (cols / 2) - 29, "   || ' __ ' ");
            mvaddstr(firstline + 13, (cols / 2) - 29, "   ||  /*.\\  ");
            mvaddstr(firstline + 14, (cols / 2) - 29, "   ||  \\__/  ");
            mvaddstr(firstline + 15, (cols / 2) - 29, "   || '      ");
            mvaddstr(firstline + 16, (cols / 2) - 29, "   ||    '   ");
            mvaddstr(firstline + 17, (cols / 2) - 29, "             ");
        }
        if (name == "Paladino")
        {
            mvaddstr(firstline + 8, (cols / 2) - 5, "  __________  ");
            mvaddstr(firstline + 9, (cols / 2) - 5, " |    ||    | ");
            mvaddstr(firstline + 10, (cols / 2) - 5, " |____||____| ");
            mvaddstr(firstline + 11, (cols / 2) - 5, " |____  ____| ");
            mvaddstr(firstline + 12, (cols / 2) - 5, " \\    ||    / ");
            mvaddstr(firstline + 13, (cols / 2) - 5, "  \\   ||   /  ");
            mvaddstr(firstline + 14, (cols / 2) - 5, "   \\  ||  /   ");
            mvaddstr(firstline + 15, (cols / 2) - 5, "    \\ || /    ");
            mvaddstr(firstline + 16, (cols / 2) - 5, "     \\||/     ");
            mvaddstr(firstline + 17, (cols / 2) - 5, "      \\/      ");
        }
        if (name == "Elfo")
        {
            mvaddstr(firstline + 8, (cols / 2) + 17, "     (.      ");
            mvaddstr(firstline + 9, (cols / 2) + 17, "     |\\      ");
            mvaddstr(firstline + 10, (cols / 2) + 17, "     | \\     ");
            mvaddstr(firstline + 11, (cols / 2) + 17, "     | .\\    ");
            mvaddstr(firstline + 12, (cols / 2) + 17, " >---|--|--> ");
            mvaddstr(firstline + 13, (cols / 2) + 17, "     | ./    ");
            mvaddstr(firstline + 14, (cols / 2) + 17, "     | /     ");
            mvaddstr(firstline + 15, (cols / 2) + 17, "     |/      ");
            mvaddstr(firstline + 16, (cols / 2) + 17, "     (.      ");
            mvaddstr(firstline + 17, (cols / 2) + 17, "             ");
        }
        mvaddstr(firstline + 23, (cols / 2) - 20, "Saúde:");
        mvprintw(firstline + 23, (cols / 2) - 12, to_string(health).c_str());
        mvaddstr(firstline + 24, (cols / 2) - 20, "Ataque:");
        mvprintw(firstline + 24, (cols / 2) - 12, to_string(attack).c_str());
        mvaddstr(firstline + 25, (cols / 2) - 20, "Leveling:  ");
        mvprintw(firstline + 25, (cols / 2) - 10, to_string(leveling).c_str());
        mvaddstr(firstline + 26, (cols / 2) - 20, "Arma:");
        mvprintw(firstline + 26, (cols / 2) - 12, weapon.c_str());

        // nome do jogador e permitir a edição
        mvprintw(firstline + 28, (cols / 2) - 20, "Digite o nome do jogador: %s", nameJogador.c_str());
        char playerName[50];
        echo(); // habilita a exibição do que é digitado
        getnstr(playerName, 50);

        // atualizar o nome do jogador com o que foi digitado
        nameJogador = playerName;

        key = getch();
        noecho(); // desabilita a exibição do que é digitado
    }

    Personagem novoPersonagem = inicializarPersonagem(nameJogador, attack, health, weapon, name, point);
    if (novoPersonagem.classe == arquetipos[0].nome)
    {
        novoPersonagem.arquetipo.nomeAtaqueEspecial = arquetipos[0].nomeAtaqueEspecial;
        novoPersonagem.arquetipo.danoAtaqueEspecial = arquetipos[0].danoAtaqueEspecial;
        novoPersonagem.arquetipo.id = arquetipos[0].id;
    }
    else if (novoPersonagem.classe == arquetipos[1].nome)
    {
        novoPersonagem.arquetipo.nomeAtaqueEspecial = arquetipos[1].nomeAtaqueEspecial;
        novoPersonagem.arquetipo.danoAtaqueEspecial = arquetipos[1].danoAtaqueEspecial;
        novoPersonagem.arquetipo.id = arquetipos[1].id;
    }
    else if (novoPersonagem.classe == arquetipos[2].nome)
    {
        novoPersonagem.arquetipo.nomeAtaqueEspecial = arquetipos[2].nomeAtaqueEspecial;
        novoPersonagem.arquetipo.danoAtaqueEspecial = arquetipos[2].danoAtaqueEspecial;
        novoPersonagem.arquetipo.id = arquetipos[2].id;
    }

    Sangramento sangramento;
    novoPersonagem.sangramento.duracao = 0;

    Inventario inventario;
    Item item;

    drawNewGame(novoPersonagem, inventario, item);
}

void gameOverScreen()
{
    int cols, rows;
    getmaxyx(stdscr, rows, cols);
    clear();

    string gameOverText[] = {
        " #####     #    #           # #######     #####  #           # ####### #####   ",
        "#     #   # #   ##         ## #          #     #  #         #  #       #    #  ",
        "#        #   #  # #       # # #          #     #   #       #   #       #   #   ",
        "#  #### #     # #  #     #  # #####      #     #    #     #    #####   # ##    ",
        "#     # ####### #   #   #   # #          #     #     #   #     #       #   #   ",
        "#     # #     # #    # #    # #          #     #      # #      #       #    #  ",
        " #####  #     # #     #     # ########    #####        #       ####### #     # "};

    int start_y = (rows - 7) / 2;
    int start_x = (cols - 60) / 2;

    for (int i = 0; i < 7; ++i)
    {
        mvaddstr(start_y + i, start_x, gameOverText[i].c_str());
    }

    mvaddstr(start_y + 10, start_x, "Pressione 'q' para sair...");

    refresh();
    getch();
    endwin();
}

void victoryScreen(Personagem &personagem)
{
    int cols, rows;
    getmaxyx(stdscr, rows, cols);
    clear();

    string victoryText[] = {
        "#           # #  #####  #####  #####  #####   #     #  #",
        " #         #  # #     #   #   #     # #    #   #   #   #",
        "  #       #   # #         #   #     # #   #     # #    #",
        "   #     #    # #         #   #     # # ##       #     #",
        "    #   #     # #         #   #     # #   #      #     #",
        "     # #      # #     #   #   #     # #    #     #      ",
        "      #       #  #####    #    #####  #     #    #     #"};

    int start_y = (rows - 7) / 2;
    int start_x = (cols - 60) / 2;

    for (int i = 0; i < 7; ++i)
    {
        mvaddstr(start_y + i, start_x, victoryText[i].c_str());
    }
    mvprintw(start_y + 9, start_x, ("Nome: " + personagem.nome).c_str());
    mvprintw(start_y + 10, start_x, ("Classe: " + personagem.classe).c_str());
    mvprintw(start_y + 11, start_x, ("Ataque: " + to_string(personagem.ataqueAtual)).c_str());
    mvprintw(start_y + 12, start_x, ("Vida: " + to_string(personagem.vidaAtual)).c_str());
    mvprintw(start_y + 13, start_x, ("Pontos: " + to_string(personagem.ponto)).c_str());

    mvprintw(start_y + 15, start_x, "Pressione 'q' para sair...");

    refresh();
    getch();
    endwin();
}

void drawNewGame(Personagem &novoPersonagem, Inventario &inventario, Item &item)
{
    initscr();            // inicializa a tela ncurses
    noecho();             // não mostra os caracteres digitados
    cbreak();             // desativa o buffer de linha
    keypad(stdscr, TRUE); // habilita a captura de teclas especiais

    int cols, rows;
    getmaxyx(stdscr, rows, cols);

    // lendo o mapa do arquivo
    ifstream file("mapa.txt");
    if (!file)
    {
        endwin();
        printf("Erro ao abrir o arquivo de mapa!\n");
        return;
    }

    string linha;
    string map[100]; // array de strings para o mapa, com no máximo 100 linhas
    int numLinhas = 0;
    while (getline(file, linha) && numLinhas < 100)
    {
        map[numLinhas++] = linha;
    }
    file.close();

    int altura = numLinhas;
    int largura = map[0].length(); // supondo que todas as linhas têm a mesma largura

    // verificar se o terminal é grande o suficiente
    int maxRows, maxCols;
    getmaxyx(stdscr, maxRows, maxCols);

    // adicionar mensagens de depuração
    endwin();
    printf("Dimensões do terminal: %d linhas, %d colunas\n", maxRows, maxCols);
    printf("Dimensões do mapa: %d linhas, %d colunas\n", altura, largura);

    if (maxRows < altura + 5 || maxCols < largura)
    { // +5 para a janela de combate
        printf("O terminal não é grande o suficiente para exibir o mapa e as mensagens de combate.\n");
        return;
    }

    // iniciar ncurses novamente após a depuração
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    Item *cabeca = nullptr;

    // adiciona inimigo
    Inimigo inimigo = criarInimigo();
    map[5][30] = inimigo.simbolo;
    map[5][60] = inimigo.simbolo;
    map[30][83] = inimigo.simbolo;
    map[22][56] = inimigo.simbolo;

    map[8][30] = 'T';
    map[9][75] = 'T';
    map[28][150] = '|';
    map[25][23] = 'T';
    map[40][55] = 'T';

    // criar uma janela para o mapa
    WINDOW *janela = newwin(altura, largura, 0, 0);
    keypad(janela, TRUE);

    // criar uma janela para as mensagens de combate
    WINDOW *janelaCombate = newwin(5, largura, altura, 0);

    int y = 1, x = 15; // coordenadas iniciais do personagem
    char direcaoPersonagem = '>';

    // desenhar o mapa e o personagem
    for (int i = 0; i < altura; i++)
    {
        for (int j = 0; j < largura; j++)
        {
            mvwaddch(janela, i, j, map[i][j]);
        }
    }
    mvwaddch(janela, y, x, direcaoPersonagem);
    wrefresh(janela);

    // box com informações do personagem
    WINDOW *infoBox = newwin(10, 30, 1, largura + 2); // Cria um box ao lado do mapa
    box(infoBox, 0, 0);
    mvwprintw(infoBox, 1, 1, ("Nome: " + novoPersonagem.nome).c_str());
    mvwprintw(infoBox, 2, 1, ("Classe: " + novoPersonagem.classe).c_str());
    mvwprintw(infoBox, 3, 1, ("Ataque: " + to_string(novoPersonagem.ataqueAtual)).c_str());
    mvwprintw(infoBox, 4, 1, ("Vida: " + to_string(novoPersonagem.vidaAtual)).c_str());
    wrefresh(infoBox);

    // criar a janela para o inventário
    WINDOW *infoBoxInventario = newwin(10, 30, 12, largura + 2); // posiciona abaixo do infoBox
    box(infoBoxInventario, 0, 0);
    mvwprintw(infoBoxInventario, 1, 1, "Inventário:");

    bool teste = true;
    int qtdInimigos = 0;
    int qtdChave = 0;

    while (teste != false)
    {
        int saida = wgetch(janela);
        if (saida == 'q')
        {
            teste = 2; // saída imediata do jogo
            break;
        }

        // função para verificar se há inimigos em uma distância de ataque
        auto verificaInimigo = [&](int y, int x, int &inimigoY, int &inimigoX, int distancia)
        {
            for (int dy = -distancia; dy <= distancia; ++dy)
            {
                for (int dx = -distancia; dx <= distancia; ++dx)
                {
                    int ny = y + dy, nx = x + dx;
                    if (ny >= 0 && ny < altura && nx >= 0 && nx < largura && map[ny][nx] == 'X')
                    {
                        inimigoY = ny;
                        inimigoX = nx;
                        return true;
                    }
                }
            }
            return false;
        };

        auto verificaBoss = [&](int y, int x, int &bossY, int &bossX, int distancia)
        {
            for (int dy = -distancia; dy <= distancia; ++dy)
            {
                for (int dx = -distancia; dx <= distancia; ++dx)
                {
                    int ny = y + dy, nx = x + dx;
                    if (ny >= 0 && ny < altura && nx >= 0 && nx < largura && map[ny][nx] == 'X')
                    {
                        bossY = ny;
                        bossX = nx;
                        return true;
                    }
                }
            }
            return false;
        };

        int distanciaAtaque = 1; // ajuste a distância de ataque conforme necessário

        int ch;
        while ((ch = wgetch(janela)) != 'q')
        {
            if (ch == 'i')
            {
                drawCombatMenu(janelaCombate, &cabeca, infoBox, infoBoxInventario, novoPersonagem);
            }

            int newY = y, newX = x;
            char novaDirecaoPersonagem = direcaoPersonagem;

            switch (ch)
            {
            case 'w':
                newY--;
                novaDirecaoPersonagem = '^';
                break;
            case 's':
                newY++;
                novaDirecaoPersonagem = 'v';
                break;
            case 'a':
                newX--;
                novaDirecaoPersonagem = '<';
                break;
            case 'd':
                newX++;
                novaDirecaoPersonagem = '>';
                break;
            }

            int inimigoY, inimigoX;
            if (verificaInimigo(newY, newX, inimigoY, inimigoX, distanciaAtaque))
            {
                mvwprintw(janelaCombate, 0, 0, "Encontrou um inimigo! Pressione b para lutar: ");
                wrefresh(janelaCombate);
                int atacar = wgetch(janelaCombate);
                if (atacar == 'b')
                {
                    // luta
                    Fila f;
                    luta(janela, janelaCombate, infoBox, novoPersonagem, inimigo, &f);
                    if (inimigo.vida <= 0)
                    {
                        map[inimigoY][inimigoX] = ' '; // remove o inimigo do mapa
                        // redesenha o mapa
                        for (int i = 0; i < altura; i++)
                        {
                            for (int j = 0; j < largura; j++)
                            {
                                mvwaddch(janela, i, j, map[i][j]);
                            }
                        }
                        mvwaddch(janela, y, x, direcaoPersonagem);
                        wrefresh(janela);
                        qtdInimigos++;

                        // verifica drop de chave
                        if (qtdInimigos < 4 && qtdChave == 0) // verifica se há menos de 4 inimigos derrotados e se o jogador não possui nenhuma chave
                        {
                            if (rand() % 100 < 15) // 15% de chance de dropar uma chave
                            {
                                string nomeItem = "Chave";
                                int quantidade = 1;
                                mvwprintw(janelaCombate, 0, 0, "Você encontrou um item! Pegar? (b)");
                                wrefresh(janelaCombate);
                                int escolha = wgetch(janelaCombate);
                                if (escolha == 'b')
                                {
                                    map[newY][newX] = ' ';
                                    mvwaddch(janela, newY, newX, ' ');
                                    werase(janelaCombate);
                                    wrefresh(janelaCombate);
                                    mvwprintw(janelaCombate, 0, 0, "Você pegou uma chave!");
                                    wrefresh(janela);
                                    wgetch(janelaCombate);
                                    werase(janelaCombate);
                                    wrefresh(janela);
                                    qtdChave++;
                                }
                                werase(janelaCombate);
                                wrefresh(janelaCombate);
                                mvwaddch(janela, y, x, direcaoPersonagem);
                                wrefresh(janela);
                            }
                        }
                        else if (qtdInimigos == 4 && qtdChave == 0) // verifica se exatamente 4 inimigos foram derrotados e se o jogador não possui nenhuma chave
                        {
                            string nomeItem = "Chave";
                            int quantidade = 1;
                            mvwprintw(janelaCombate, 0, 0, "Você encontrou um item! Pegar? (b)");
                            wrefresh(janelaCombate);
                            int escolha = wgetch(janelaCombate);
                            if (escolha == 'b')
                            {
                                map[newY][newX] = ' ';
                                mvwaddch(janela, newY, newX, ' ');
                                werase(janelaCombate);
                                wrefresh(janelaCombate);
                                wrefresh(janela);
                                qtdChave++;
                            }
                            werase(janelaCombate);
                            wrefresh(janelaCombate);
                            mvwaddch(janela, y, x, direcaoPersonagem);
                            wrefresh(janela);
                        }
                    }
                    if (novoPersonagem.vidaAtual <= 0)
                    {
                        gameOverScreen();
                    }
                }
                werase(janelaCombate);
                wrefresh(janelaCombate);
            }

            if (newY >= 0 && newY < altura && newX >= 0 && newX < largura)
            {
                if (map[newY][newX] == 'T')
                {
                    int chance = rand() % 100;
                    string nomeItem;
                    int quantidade = 1;

                    if (chance < 10)
                    { // 10% de chance de dropar uma moeda
                        nomeItem = "Moeda";
                    }
                    else
                    {
                        // 90% de chance dividida igualmente entre os três itens restantes
                        int itemIndex = rand() % 3;
                        switch (itemIndex)
                        {
                        case 0:
                            nomeItem = "Poção de Vida";
                            break;
                        case 1:
                            nomeItem = "Poção de Ataque";
                            break;
                        case 2:
                            nomeItem = "Tortinha de Limão";
                            break;
                        }
                    }

                    mvwprintw(janelaCombate, 0, 0, "Você encontrou um item! Pegar? (b)");
                    wrefresh(janelaCombate);
                    int escolha = wgetch(janelaCombate);
                    if (escolha == 'b')
                    {
                        insereItem(&cabeca, nomeItem, quantidade, infoBoxInventario);
                        map[newY][newX] = ' ';
                        mvwaddch(janela, newY, newX, ' ');
                        werase(janelaCombate);
                        wrefresh(janelaCombate);
                        wrefresh(janela);
                    }
                    werase(janelaCombate);
                    wrefresh(janelaCombate);
                }
                else if (map[newY][newX] == ' ')
                {
                    mvwaddch(janela, y, x, ' ');
                    y = newY;
                    x = newX;
                    direcaoPersonagem = novaDirecaoPersonagem; // atualiza a direção do personagem
                    mvwaddch(janela, y, x, direcaoPersonagem);
                    wrefresh(janela);
                }
            }

            if (newY >= 0 && newY < altura && newX >= 0 && newX < largura)
            {
                if (map[newY][newX] == '|')
                {
                    mvwprintw(janelaCombate, 0, 0, "Pressione 'b' para abrir a porta.");
                    wrefresh(janelaCombate);
                    int abrir = wgetch(janelaCombate);
                    if (abrir == 'b')
                    {
                        if (qtdChave = !1)
                        {
                            werase(janelaCombate);
                            wrefresh(janelaCombate);
                            mvwprintw(janelaCombate, 0, 0, "Você tem que pegar a chave para abrir a porta!");
                            wrefresh(janelaCombate);
                        }
                        else
                        {
                            map[newY][newX] = ' ';
                            mvwaddch(janela, newY, newX, ' ');
                            werase(janelaCombate);
                            wrefresh(janelaCombate);
                            wrefresh(janela);
                            drawNewGame2(novoPersonagem, inventario, item);
                        }
                    }
                }
                else if (map[newY][newX] == ' ')
                {
                    mvwaddch(janela, y, x, ' ');
                    y = newY;
                    x = newX;
                    direcaoPersonagem = novaDirecaoPersonagem; // atualiza a direção do personagem
                    mvwaddch(janela, y, x, direcaoPersonagem);
                    wrefresh(janela);
                }
            }
        }
    }
}

void drawNewGame2(Personagem &novoPersonagem, Inventario &inventario, Item &item)
{
    initscr();            // Inicializa a tela ncurses
    noecho();             // Não mostra os caracteres digitados
    cbreak();             // Desativa o buffer de linha
    keypad(stdscr, TRUE); // Habilita a captura de teclas especiais

    int cols, rows;
    getmaxyx(stdscr, rows, cols);

    // Lendo o mapa do arquivo
    ifstream file("mapa2.txt");
    if (!file)
    {
        endwin();
        printf("Erro ao abrir o arquivo de mapa!\n");
        return;
    }

    string linha;
    string map[100]; // Array de strings para o mapa, com no máximo 100 linhas
    int numLinhas = 0;
    while (getline(file, linha) && numLinhas < 100)
    {
        map[numLinhas++] = linha;
    }
    file.close();

    int altura = numLinhas;
    int largura = map[0].length(); // Supondo que todas as linhas têm a mesma largura

    // Verificar se o terminal é grande o suficiente
    int maxRows, maxCols;
    getmaxyx(stdscr, maxRows, maxCols);

    // Adicionar mensagens de depuração
    endwin();
    printf("Dimensões do terminal: %d linhas, %d colunas\n", maxRows, maxCols);
    printf("Dimensões do mapa: %d linhas, %d colunas\n", altura, largura);

    if (maxRows < altura + 5 || maxCols < largura)
    { // +5 para a janela de combate
        printf("O terminal não é grande o suficiente para exibir o mapa e as mensagens de combate.\n");
        return;
    }

    // Iniciar ncurses novamente após a depuração
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    Item *cabeca = nullptr;

    // adiciona um inimigo
    Boss boss = criarBoss();
    map[20][80] = boss.simbolo;

    // criar uma janela para o mapa
    WINDOW *janela = newwin(altura, largura, 0, 0);
    keypad(janela, TRUE);

    // criar uma janela para as mensagens de combate
    WINDOW *janelaCombate = newwin(5, largura, altura, 0);

    int y1 = 30, x1 = 80; // coordenadas iniciais do personagem
    char direcaoPersonagem = '^';

    // desenhar o mapa e o personagem
    for (int i = 0; i < altura; i++)
    {
        for (int j = 0; j < largura; j++)
        {
            mvwaddch(janela, i, j, map[i][j]);
        }
    }
    mvwaddch(janela, y1, x1, direcaoPersonagem);
    wrefresh(janela);

    // box com informações do personagem
    WINDOW *infoBox = newwin(10, 30, 1, largura + 2); //cCria um box ao lado do mapa
    box(infoBox, 0, 0);
    mvwprintw(infoBox, 1, 1, ("Nome: " + novoPersonagem.nome).c_str());
    mvwprintw(infoBox, 2, 1, ("Classe: " + novoPersonagem.classe).c_str());
    mvwprintw(infoBox, 3, 1, ("Ataque: " + to_string(novoPersonagem.ataqueAtual)).c_str());
    mvwprintw(infoBox, 4, 1, ("Vida: " + to_string(novoPersonagem.vidaAtual)).c_str());
    wrefresh(infoBox);

    // criar a janela para o inventário
    WINDOW *infoBoxInventario = newwin(10, 30, 12, largura + 2); // posiciona abaixo do infoBox
    box(infoBoxInventario, 0, 0);
    mvwprintw(infoBoxInventario, 1, 1, "Inventário:");

    bool teste = true;
    int qtdBoss = 0;

    auto verificaBoss = [&](int y1, int x1, int &bossY1, int &bossX1, int distancia)
    {
        for (int dy1 = -distancia; dy1 <= distancia; ++dy1)
        {
            for (int dx1 = -distancia; dx1 <= distancia; ++dx1)
            {
                int ny1 = y1 + dy1, nx1 = x1 + dx1;
                if (ny1 >= 0 && ny1 < altura && nx1 >= 0 && nx1 < largura && map[ny1][nx1] == 'B')
                {
                    bossY1 = ny1;
                    bossX1 = nx1;
                    return true;
                }
            }
        }
        return false;
    };

    int distanciaAtaque = 1; // ajuste a distância de ataque conforme necessário

    while (teste)
    {
        int ch = wgetch(janela);
        if (ch == 'q')
        {
            teste = false; // saída imediata do jogo
            break;
        }

        int newY1 = y1, newX1 = x1;
        char novaDirecaoPersonagem = direcaoPersonagem;

        switch (ch)
        {
        case 'w':
            newY1--;
            novaDirecaoPersonagem = '^';
            break;
        case 's':
            newY1++;
            novaDirecaoPersonagem = 'v';
            break;
        case 'a':
            newX1--;
            novaDirecaoPersonagem = '<';
            break;
        case 'd':
            newX1++;
            novaDirecaoPersonagem = '>';
            break;
        }

        int bossY1, bossX1;
        if (verificaBoss(newY1, newX1, bossY1, bossX1, distanciaAtaque))
        {
            mvwprintw(janelaCombate, 0, 0, "Encontrou o boss! Pressione b para lutar: ");
            wrefresh(janelaCombate);
            int atacar = wgetch(janelaCombate);
            if (atacar == 'b')
            {
                Fila f;
                luta2(janela, janelaCombate, infoBox, novoPersonagem, boss, &f);
                if (boss.vida <= 0)
                {
                    map[bossY1][bossX1] = ' '; // remove o inimigo do mapa
                    // redesenha o mapa
                    for (int i = 0; i < altura; i++)
                    {
                        for (int j = 0; j < largura; j++)
                        {
                            mvwaddch(janela, i, j, map[i][j]);
                        }
                    }
                    mvwaddch(janela, y1, x1, direcaoPersonagem);
                    wrefresh(janela);
                    qtdBoss++;
                    victoryScreen(novoPersonagem);
                }
                else if (novoPersonagem.vidaAtual <= 0)
                {
                    gameOverScreen();
                }
            }
            werase(janelaCombate);
            wrefresh(janelaCombate);
        }
        else
        {
            if (map[newY1][newX1] == ' ') // verifica se a nova posição é um espaço vazio
            {
                mvwaddch(janela, y1, x1, ' '); // remove o personagem da posição antiga
                y1 = newY1;
                x1 = newX1;
                direcaoPersonagem = novaDirecaoPersonagem;
                mvwaddch(janela, y1, x1, direcaoPersonagem); // adiciona o personagem na nova posição
                wrefresh(janela);
            }
        }
    }
    endwin();
}
