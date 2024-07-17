#include "game.h"  // Inclua este cabeçalho para ter acesso a drawNewGame
#include <ncurses.h>
#include <cstring>

void drawControls() {
    clear();
    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    std::string controls[] = {
        "Controles:",
        "- Utilize WASD para se mover com o personagem",
        "- Pressione 'Enter' para selecionar",
        "- Pressione 'b' para utilizar itens e lutar",
        "- Pressione 'i' para abrir o inventário",
        "- Pressione 'q' para sair"
    };

    int numControls = sizeof(controls) / sizeof(controls[0]);
    int startRow = (rows - numControls) / 2;

    for (int i = 0; i < numControls; i++) {
        int startCol = (cols - controls[i].length()) / 2;
        mvprintw(startRow + i, startCol, controls[i].c_str());
    }

    getch();
    clear();
}

void drawExitMessage() {
    clear();
    printw("Saindo do jogo...");
    getch(); // Espera o usuário digitar algo antes de retornar
    clear();
}

int showMainMenu() {
    initscr(); // Inicia o ncurses
    noecho(); // Não mostra as teclas pressionadas
    curs_set(0); // Esconde o cursor
    keypad(stdscr, TRUE); // Habilita a leitura de teclas especiais

    int cols, rows;
    int choice = 0;
    const char *options[] = {"Jogar", "Controles", "Sair"};
    int key;

    while (true) {
        clear();
        getmaxyx(stdscr, rows, cols); // Obtém as dimensões da tela
        for (int i = 0; i < 3; i++) {
            if (i == choice) {
                attron(A_STANDOUT); // Destaca a opção escolhida
            }
            mvprintw(rows / 2 + i, (cols - strlen(options[i])) / 2, "%s", options[i]);
            if (i == choice) {
                attroff(A_STANDOUT);
            }
        }
        refresh();

        key = getch(); // Obtém uma tecla
        switch(key) {
            case KEY_UP:
                choice = (choice == 0) ? 2 : choice - 1;
                break;
            case KEY_DOWN:
                choice = (choice == 2) ? 0 : choice + 1;
                break;
            case 10: // Enter
                switch(choice) {
                    case 0: // New game
                        roleEditor();  // Chamada para iniciar novo jogo
                        break;
                    case 1: // Controls
                        drawControls();
                        break;
                    case 2: // Exit
                        drawExitMessage();
                        endwin();
                        return choice;
                }
                break;
            case 'q': // Saída
                endwin();
                return -1;
        }
    }
}

int main() {
    initscr();  // Inicia o modo ncurses
    showMainMenu();
    endwin();  // Encerra o modo ncurses
    return 0;
}
