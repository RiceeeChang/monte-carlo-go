#include "gtp.h"

const char LabelX[]="0ABCDEFGHJ";
/* 
 * Following are commands for Go Text Protocol (GTP)
 *
 * */
const char *KnownCommands[]={
    "protocol_version",
    "name",
    "version",
    "known_command",
    "list_commands",
    "quit",
    "boardsize",
    "clear_board",
    "komi",
    "play",
    "genmove",
    "undo",
    "quit",
    "showboard",
    "final_score"
};

/*
 * This main function is used of the gtp protocol
 * */
void gtp_main(int display) {
    char Input[COMMAND_LENGTH]="";
    char Command[COMMAND_LENGTH]="";
    char Parameter[COMMAND_LENGTH]="";
    char Move[4]="";
    char Color[6]="";
    int ivalue;
    double dvalue;

    int NumCapture[3]={0};// 1:Black, 2: White

    // Board Information
    Board _board;
    Board* board = &_board;
    int time_limit = DEFAULT_TIME;
    int game_record[MAX_HISTORY][BOARD_SIZE];
    int game_length = 0;
    

    if (display==1) {
       gtp_list_commands();
       gtp_showboard(board);
    }

    while (fgets(Input, COMMAND_LENGTH, stdin) != 0) {
        sscanf(Input, "%s", Command);
        if (Command[0]== '#')
            continue;

        if (strcmp(Command, "protocol_version")==0) {
            gtp_protocol_version();
        }
        else if (strcmp(Command, "name")==0) {
            gtp_name();
        }
        else if (strcmp(Command, "version")==0) {
            gtp_version();
        }
        else if (strcmp(Command, "list_commands")==0) {
            gtp_list_commands();
        }
        else if (strcmp(Command, "known_command")==0) {
            sscanf(Input, "known_command %s", Parameter);
            gtp_known_command(Parameter);
        }
        else if (strcmp(Command, "boardsize")==0) {
            sscanf(Input, "boardsize %d", &ivalue);
            gtp_boardsize(ivalue);
        }
        else if (strcmp(Command, "clear_board")==0) {
            gtp_clear_board(board);
            game_length = 0;
        }
        else if (strcmp(Command, "komi")==0) {
            sscanf(Input, "komi %lf", &dvalue);
            gtp_komi(dvalue);
        }
        else if (strcmp(Command, "play")==0) {
            sscanf(Input, "play %s %s", Color, Move);
            gtp_play(Color, Move, board, game_length, game_record);
            game_length++;
            if (display==1) {
                gtp_showboard(board);
            }
        }
        else if (strcmp(Command, "genmove")==0) {
            sscanf(Input, "genmove %s", Color);
            gtp_genmove(board, Color, time_limit, game_length, game_record);
            game_length++;
            if (display==1) {
                gtp_showboard(board);
            }
        }
        else if (strcmp(Command, "quit")==0) {
            break;
        }
        else if (strcmp(Command, "showboard")==0) {
            gtp_showboard(board);
        }
        else if (strcmp(Command, "undo")==0) {
            game_length--;
            gtp_undo(board, game_length, game_record);
            if (display==1) {
                gtp_showboard(board);
            }
        }
        else if (strcmp(Command, "final_score")==0) {
            if (display==1) {
                gtp_showboard(board);
            }
            gtp_final_score(board);
        }
    }
}


void gtp_genmove(Board* board, char Color[], int time_limit, int game_length, int game_record[MAX_HISTORY][BOARD_SIZE]){
    int player = (Color[0]=='b'||Color[0]=='B')?BLACK:WHITE;
    int move_i, move_j;

    int move = genmove(board, player, time_limit);
    record(board, game_record, game_length+1);

    if (move==0) {
       cout << "= PASS" << endl << endl << endl;
    }
    else {
        move_i = move/10;
        move_j = (move%(SIZE+1));
        cout << "= " << LabelX[move_j] << 10-move_i << endl << endl;
    }
}

void gtp_final_score(Board* board) {
    double result;
    result = final_score(board);
    //result -= _komi;
    cout << "= ";
    if (result > 0.0) { // Black win
       cout << "B+" << result << endl << endl<< endl;;
    }if (result < 0.0) { // White win
       cout << "W+" << -result << endl << endl<< endl;;
    }else { // draw
       cout << "0" << endl << endl<< endl;;
    }
}
void gtp_undo(Board* board, int game_length, int game_record[MAX_HISTORY][BOARD_SIZE]) {
    if (game_length!=0) {
        for (int i = 1; i <= BOARD_SIZE; ++i) {
            board->board[i] = game_record[game_length][i];
        }
    }
    cout << "= " << endl << endl;
}
void gtp_showboard(Board* board) {
    for(int i=0; i<BOARD_SIZE; i++){
        switch(board->board[i]){
        case BOUNDARY:
            printf("#");
            break;
        case EMPTY:
            printf(".");
            break;
        case BLACK:
            printf("X");
            break;
        case WHITE:
            printf("O");
            break;
        }
        if( i%(SIZE+1) == SIZE)
            printf("\n");
    }
    printf("\n");
}
void gtp_protocol_version() {
    cout <<"= 2"<<endl<< endl;
}
void gtp_name() {
    cout <<"= TCG-randomGo99" << endl<< endl;
}
void gtp_version() {
    cout << "= 1.02" << endl << endl;
}
void gtp_list_commands(){
    cout <<"= ";
    for (int i = 0 ; i < NUM_GTP_COMMANDS; ++i) {
        cout <<KnownCommands[i] << endl;
    }
    cout << endl;
}
void gtp_known_command(const char Input[]) {
    for (int i = 0 ; i < NUM_GTP_COMMANDS; ++i) {
        if (strcmp(Input, KnownCommands[i])==0) {
            cout << "= true" << endl<< endl;
            return;
        }
    }
    cout << "= false" << endl<< endl;
}
void gtp_boardsize(int size) {
    if (size!=9) {
       cout << "? unacceptable size" << endl<< endl;
    }
    else {
       //_board_size = size;
       cout << "= "<<endl<<endl;
    }
}
void gtp_clear_board(Board* board) {
    board->reset();
    cout << "= "<<endl<<endl;
}
void gtp_komi(double komi) {
    _komi = komi;
    cout << "= "<<endl<<endl;
}
void gtp_play(char Color[], char Move[], Board* board, int game_length, int game_record[MAX_HISTORY][BOARD_SIZE]) {
    int player, move_i, move_j;
    if (Color[0] =='b' || Color[0] == 'B')
        player = BLACK;
    else
        player = WHITE;
    if (strcmp(Move, "PASS") == 0 || strcmp(Move, "pass")==0) {
        record(board, game_record, game_length+1);
    }else {
        // [ABCDEFGHJ][1-9], there is no I in the index.
        Move[0] = toupper(Move[0]);
        move_j = Move[0]-'A'+1;
        if (move_j == 10) move_j = 9;
        move_i = 10-(Move[1]-'0');
        int point = move_i*10+move_i;
        update_board(board, player, point);
        record(board, game_record, game_length+1);
    }
    cout << "= "<<endl<<endl;
}
