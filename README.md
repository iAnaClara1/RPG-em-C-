# RPG-em-C-
## Descrição do Projeto ✏️
Este projeto é um RPG de terminal desenvolvido como parte do trabalho semestral da disciplina "Estruturas de Dados 1". O objetivo principal é demonstrar a aplicação prática de conceitos fundamentais de estruturas de dados em um ambiente de jogo interativo. Desenvolvido em C++ e utilizando a biblioteca ncurses para a interface de texto, o jogo oferece uma experiência imersiva dentro do terminal.

Neste RPG, os jogadores podem escolher o arquétipo do personagem, gerenciar inventários, explorar mapas, enfrentar inimigos e acessar outras funcionalidades, tudo dentro do terminal. As estruturas de dados implementadas incluem filas e listas duplamente encadeadas, proporcionando uma experiência rica em termos de manipulação de dados.

## Objetivos do Projeto 🎯
Este projeto tem como objetivo:
- Desenvolver habilidades em C++.
- Aprimorar a lógica de programação e resolução de problemas.
- Aplicar conceitos de estruturas de dados em um conceito prático.
- Explorar a biblioteca ncurses para interfaces de texto em jogos.
- Criar um jogo interativo que demonstre a utilização de fila e lista duplamente encadeada.

## Funcionalidades do Jogo 👾
- **Interface de Texto**: A biblioteca ncurses foi utilizada para uma experiência de terminal. O jogo é todo feito em ASCII.
- **Sistema de Combate por Turnos**: Implementado utilizando a estrutura de fila para gerenciar a ordem dos ataques entre o jogador e os inimigos. Nos turnos, ambos podem atacar normalmente, atacar com poderes especiais (com chance de 50% de sangramento no defensor) ou errar o ataque.
- **Inventário**: Implementado utilizando a estrutura de lista duplamente encadeada. É possível coletar itens e escolher quais serão utilizados.

## Demonstração 🕹️
Abaixo, você pode ver um breve vídeo do jogo em ação. O vídeo está acelerado para mostrar todas as funcionalidades dentro do tempo disponível.

https://github.com/user-attachments/assets/23f00066-0683-4d21-ad33-81e55c660e1f

## Instruções para Executar o Jogo 🚀
1. Clone o repositório:
```
$ git clone https://github.com/iAnaClara1/RPG-em-C-.git
```
2. Navegue até o repositório do projeto.
3. Compile o código:
```
g++ -o main main.cpp game.cpp -lncurses
```
4. Execute o jogo:
```
./main
```
## Requisitos ⚙️
- C++11 ou superior.
- Biblioteca ncurses.
- Terminal do Linux. Caso você deseje executar no Windows, recomendo utilizar o Cygwin.

## Contribuições 💙
Contribuições são bem-vindas! Sinta-se à vontade para abrir issues e pull requests com melhorias ou correções.
