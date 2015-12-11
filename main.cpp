// use C++11
#include "main.h"

int main(int argc, char* argv[]) {
    srand(time(NULL));
    /*
    int type = GTP_VERSION;// 1: local version, 2: gtp version
    int display = 0; // 1: display, 2 nodisplay
    if (argc > 1) {
        if (strcmp(argv[1], "-display")==0) {
            display = 1;
        }
        if (strcmp(argv[1], "-nodisplay")==0) {
            display = 0;
        }
    }
    gtp_main(display);*/

    
    Board _board;
    Board* board = &_board;

    int game_record[MAX_HISTORY][BOARD_SIZE] = {0};
    int game_length = 0;
    int player = BLACK;
    int time_limit = DEFAULT_TIME;
    
    clock_t start_t, end_t;
    int result = 0;
    start_t = clock();
    /*for(int i=0; i<30000; i++)
        if(quick_move(board, player, 22))
            result++;*/
    int move = mcts_basic(board, player, time_limit);
    end_t = clock();
    printf("result = %d/30000, time = %d\n", result, end_t - start_t);
    do_move(board, player, move);
    board->show();
    
    system("pause");
    return 0;
}
