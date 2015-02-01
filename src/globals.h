/* globals.h
* Theo Kanning
* Contains all global definitions of functions, macros, and data types
*/
#include "math.h"
#include <iostream>
#include "move_macros.h"

/***** Global Macros *****/

//#define DEBUG //Define debug mode for full assert function

#define PROGRAM_NAME "Chess-AI"
#define AUTHOR	"Theo Kanning"
#define VERSION_NO	1.27

#ifndef DEBUG
        #define ASSERT(x)
#else
#define ASSERT(x) \
               if (! (x)) \
               { \
                  std::cout << "ERROR!! Assert " << #x << " failed\n"; \
                  std::cout << " on line " << __LINE__  << "\n"; \
                  std::cout << " in file " << __FILE__ << "\n";  \
				  system("PAUSE"); \
			   }
#endif

//Returns random 64 bit uint
#define RANDOM_U64()			(((U64)rand() + \
								((U64)rand() << 15) + \
								((U64)rand() << 30) + \
								((U64)rand() << 45) + \
							   (((U64)rand() & 0x0f) << 60)))



#define MAX_PLY						1028 //Maximum depth for searching
#define MAX_SEARCH_DEPTH			64   //Max search depth, arbitrary
#define MAX_MOVE_LIST_LENGTH		218 //Maximum moves in any position
#define INF							100000 //Large enough number to be infinite
#define INVALID						1000001 //Larger than infinity, always out of bounds
#define MATE_SCORE					10000
#define IS_MATE(x)					(abs(x) >= MATE_SCORE - MAX_SEARCH_DEPTH)
#define ADJUST_MATE_SCORE(score,ply)		((score < 0) ? -MATE_SCORE + ply : MATE_SCORE -ply)

#define START_FEN		"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

//#define ON_BOARD_120(x)				((21 <= x && x <= 28) || (31 <= x && x <= 38) || (41 <= x && x <= 48) || (51 <= x && x <= 58) || \
//									 (61 <= x && x <= 68) || (71 <= x && x <= 78) || (81 <= x && x <= 88) || (91 <= x && x <= 98))
#define ON_BOARD_120(x)				(ON_BOARD_64(SQUARE_120_TO_64(x)))
#define ON_BOARD_64(x)				(x >= 0 && x < 64)

#define SQUARE_120_TO_64(x)			(square_120_to_64[x])
#define SQUARE_64_TO_120(x)			(square_64_to_120[x])

#define RANK_FILE_TO_SQUARE_64(r,f)	(f+8*r)
#define GET_RANK_64(x)				(x>>3)
#define GET_FILE_64(x)				(x&7)
#define GET_RANK_120(x)				GET_RANK_64(SQUARE_120_TO_64(x))
#define GET_FILE_120(x)				GET_FILE_64(SQUARE_120_TO_64(x))

#define SET_BIT(x,y)				(x |= (1i64<<y))
#define CLR_BIT(x,y)				(x &= ~(1i64<<y))
#define GET_BIT(x,y)				(x & (1i64<<y))

#define IS_WHITE_PIECE(x)			(is_white[x]) //((x > EMPTY) && (x <= wK)) 
#define IS_BLACK_PIECE(x)			(is_black[x]) //(x > wK) && (x <= bK))
#define COLOR(x)					(color[x])
#define IS_PAWN(x)					(is_pawn[x]) //((x == wP) || (x == bP))
#define IS_KNIGHT(x)				(is_knight[x]) //((x == wN) || (x == bN))
#define IS_BISHOP(x)				(is_bishop[x]) //((x == wB) || (x == bB))
#define IS_ROOK(x)					(is_rook[x]) //((x == wR) || (x == bR))
#define IS_QUEEN(x)					(is_queen[x]) //((x == wQ) || (x == bQ))
#define IS_KING(x)					(is_king[x]) //((x == wK) || (x == bK))

//Late move reduction macros
#define CAN_REDUCE(move)			(!IS_CAPTURE(move) && !IS_PROMOTION(move)) //Returns 1 if move is not a promotion or capture
#define REDUCTION_LIMIT				3 //Minimum depth to consider reductions
#define LATE_MOVE_NUM				5 //First move number to consider reducing, 6th move = 5
#define LATE_MOVE_REDUCTION			1 //Number of ply to shorten late move searches

/***** Global structures and typedefs *****/
typedef unsigned long long U64; //64 bit integer

enum PIECE_NAME_ENUM
{
	EMPTY,wP,wN,wB,wR,wQ,wK,bP,bN,bB,bR,bQ,bK
};

enum COLOR_ENUM
{
	WHITE,BLACK,BOTH //BOTH is used for bitboards
};

enum SPACE_ENUM //Space indices using 10x12 board
{
	A1 = 21, B1, C1, D1, E1, F1, G1, H1,
	A2 = 31, B2, C2, D2, E2, F2, G2, H2,
	A3 = 41, B3, C3, D3, E3, F3, G3, H3,
	A4 = 51, B4, C4, D4, E4, F4, G4, H4,
	A5 = 61, B5, C5, D5, E5, F5, G5, H5,
	A6 = 71, B6, C6, D6, E6, F6, G6, H6,
	A7 = 81, B7, C7, D7, E7, F7, G7, H7,
	A8 = 91, B8, C8, D8, E8, F8, G8, H8, OFF_BOARD,NO_SQUARE
};

enum RANK_ENUM
{
	RANK_1,RANK_2,RANK_3,RANK_4,RANK_5,RANK_6,RANK_7,RANK_8,NO_RANK
};

enum FILE_ENUM
{
	FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, NO_FILE
};

enum CASTLE_ENUM
{
	WK_CASTLE = 1, WQ_CASTLE = 2, BK_CASTLE = 4, BQ_CASTLE = 8
};

typedef struct
{
	int move; //32 bit move stores all necessary data 
	int score;
}MOVE_STRUCT;

typedef struct
{
	MOVE_STRUCT list[MAX_MOVE_LIST_LENGTH];
	int num;
}MOVE_LIST_STRUCT;

typedef struct
{
	int move_counter;
	int ep;
	int castle;
	int move_num;
	U64 hash; //Hash before move

}UNDO_STRUCT;

typedef struct
{
	UNDO_STRUCT list[MAX_PLY];
	int num;
}UNDO_LIST_STRUCT;

typedef struct
{
	MOVE_STRUCT list[MAX_SEARCH_DEPTH];
	int num;
	int in_pv_line;
}PV_LIST_STRUCT;

typedef struct
{
	int start_time;
	int stop_time;
	int time_set;
	int end_early; //Enable ending early if using normal clock
	int stopped;
	int nodes;

	int depth;
	int null_available;

	int age; //Number of irreversible moves made
	int hash_hits;

	int quit;
	
}SEARCH_INFO_STRUCT;

typedef struct
{
	U64 hash;
	int depth;
	int flag;
	int eval;
	int age; //number of irreversible moves made
	int move;
}HASH_ENTRY_STRUCT;

typedef struct
{
	int num_entries;
	HASH_ENTRY_STRUCT *table;
}HASH_TABLE_STRUCT;

typedef struct
{
	int board_array120[120];
	int board_array64[64]; 

	int ep; //120 index
	int side;
	int castle_rights;

	int hply; //total moves taken so far
	int move_counter; //100 move counter

	int piece_num[13]; //Stores the number of each type of piece
	int piece_list120[13][10]; //[Piece][instance] contains index of pieces on board, empty cells are 0, which is off the 120 board

	int phase; //Calculated at beginning of eval function

	int white_big_material; //Non-pawn material
	int white_pawn_material;

	int black_big_material;
	int black_pawn_material;

	int middle_piece_square_score;
	int end_piece_square_score;

	int eval_score; //Overall evaluation score

	U64 pawn_bitboards[3]; //One for white, black, and both
	U64 piece_bitboards[13]; //One for each piece type
	U64 side_bitboards[3]; //One for white, black, and both

	UNDO_LIST_STRUCT undo_list;

	int the_killers[MAX_SEARCH_DEPTH][2];

	int history[120][120];
	int history_max;

	U64 hash_key;
	U64 pawn_hash_key;
	int age; //number of irreversible moves made

} BOARD_STRUCT;


/***** Global Functions *****/
//board
extern void Init_Board(BOARD_STRUCT *board);
extern void Update_Piece_Lists(BOARD_STRUCT *board);
extern void Update_Bitboards(BOARD_STRUCT *board);
extern void Add_To_Piecelists(int piece, int index120, BOARD_STRUCT *board);
extern void Remove_From_Piecelists(int piece, int index120, BOARD_STRUCT *board);
extern void Parse_Fen(char *fen, BOARD_STRUCT *board);
extern void Clear_Undo_List(BOARD_STRUCT *board);
extern int  Is_Threefold_Repetition(BOARD_STRUCT *board);
extern int Is_Repetition(BOARD_STRUCT *board);
extern int Is_Material_Draw(BOARD_STRUCT *board);
extern void Check_Board(BOARD_STRUCT *board);
extern void Print_Board(BOARD_STRUCT *board);
extern void Print_Bitboard(U64 bb);

//data
extern int square_120_to_64[120];
extern int square_64_to_120[64];
extern char* file_names;
extern char* rank_names;
extern char* piece_names;
extern int is_white[13];
extern int is_black[13];
extern int color[13];
extern int is_pawn[13];
extern int is_knight[13];
extern int is_bishop[13];
extern int is_rook[13];
extern int is_queen[13];
extern int is_king[13];
extern const U64 rank_masks[8];
extern const U64 file_masks[8];
extern int futility_margins[4];
extern int piece_values[13];
extern int passed_pawn_rank_bonus[8];
extern short middle_piece_square_tables[13][64];
extern short end_piece_square_tables[13][64];

//eval
extern int Evaluate_Board(BOARD_STRUCT *board);
extern int Get_Board_Piece_Square_Score(BOARD_STRUCT *board);
extern int Get_Piece_Square_Score(int index64, int piece, float phase);
extern int Get_Pawn_Eval_Score(BOARD_STRUCT *board);
extern int Get_King_Safety_Score(BOARD_STRUCT *board);
extern void Init_Pawn_Masks(void);

//hashkeys
extern void Init_Hashkeys(void);
extern void Init_Hash_Table(void);
extern void Compute_Hash(BOARD_STRUCT *board);
extern void Add_Hash_Entry(HASH_ENTRY_STRUCT *hash_ptr, int ply, SEARCH_INFO_STRUCT *info);
extern void Remove_Hash_Entry(U64 hash);
extern int  Get_Hash_Entry(U64 hash, int alpha, int beta, int depth, int ply, int * hash_move);
extern void Fill_Hash_Entry(int age, int depth, int eval, int flag, U64 hash, int move, HASH_ENTRY_STRUCT *hash_ptr);

//history
extern void Add_History_Move(int move, BOARD_STRUCT *board);
extern void Find_History_Moves(MOVE_LIST_STRUCT *move_list, BOARD_STRUCT *board);
extern void Clear_History_Data(BOARD_STRUCT *board);

//input
extern int Get_Time_Ms(void);
extern void ReadInput(SEARCH_INFO_STRUCT *info);

//killers
extern void Find_Killer_Moves(MOVE_LIST_STRUCT *move_list, BOARD_STRUCT *board);
extern void Add_Killer_Move(int move, BOARD_STRUCT *board);

//magic_data
extern const U64 R_Magic[64];
extern const U64 B_Magic[64];
extern const int R_Bits[64];
extern const int B_Bits[64];
extern const U64 R_Occ[64];
extern const U64 B_Occ[64];
extern int count_1s(U64 b);
extern const int BitTable[64];
extern int pop_1st_bit(U64 *bb);
extern int transform(U64 b, U64 magic, int bits);
extern U64 magicMovesRook[64][4096];
extern U64 magicMovesBishop[64][512];
extern const U64 magicMovesKnight[64];
extern const U64 magicMovesKing[64];
extern void Generate_Occupancy_Masks(void);
extern void Generate_King_Knight_Attack_Masks(void);
extern void Generate_Magic_Numbers(void);
extern void Generate_Magic_Moves(void);

//magic_movegen
extern U64 Rook_Attacks(U64 occ, int sq);
extern U64 Bishop_Attacks(U64 occ, int sq);
extern void Magic_Generate_Moves(BOARD_STRUCT *board, MOVE_LIST_STRUCT *list);

//makemove
extern int Make_Move(int move_num, BOARD_STRUCT *board);
extern void Take_Move(BOARD_STRUCT *board);
extern int Make_Null_Move(BOARD_STRUCT *board);
extern void Take_Null_Move(BOARD_STRUCT *board);
extern int Is_Checking_Move(int move_num, BOARD_STRUCT *board);
extern void Print_Move(MOVE_STRUCT *move);
extern char* UCI_Move_String(MOVE_STRUCT *move);


//movegen
extern void Generate_Moves(BOARD_STRUCT*, MOVE_LIST_STRUCT*);
extern int Under_Attack(int target120, int side, BOARD_STRUCT *board);
extern int In_Check(int side, BOARD_STRUCT *board);

//movelist
extern void Sort_Moves(MOVE_LIST_STRUCT *move_list);
extern void Copy_Move(MOVE_STRUCT *move1, MOVE_STRUCT *move2);
extern int Get_Capture_Moves(MOVE_LIST_STRUCT *move_list); 
extern void Get_Next_Move(int num, MOVE_LIST_STRUCT *move_list); 
extern void Get_Next_Capture_Move(int num, MOVE_LIST_STRUCT *move_list);
extern void Add_Move(MOVE_LIST_STRUCT *move_list, int from, int to, int piece, int capture, int special, int score, BOARD_STRUCT *board);
extern int Movelists_Identical(MOVE_LIST_STRUCT *ptr1, MOVE_LIST_STRUCT *ptr2);
extern void Print_Move_List(MOVE_LIST_STRUCT *move_list);

//perft
extern int Perft_Test(char *fen, int depth, BOARD_STRUCT *board);
extern int Search(BOARD_STRUCT *board, int depth);
extern int Magic_Perft_Test(char *fen, int depth, BOARD_STRUCT *board);
extern int Magic_Search(BOARD_STRUCT *board, int depth);

//pv_table
extern void Clear_PV_List(PV_LIST_STRUCT *pv);
extern int Find_PV_Move(int move_num, MOVE_LIST_STRUCT *move_list);
extern void Print_PV_List(PV_LIST_STRUCT *pv_list);
extern void Get_PV_Line(int depth, PV_LIST_STRUCT *pv_list, BOARD_STRUCT *board);

//search
extern int Iterative_Deepening(int depth, BOARD_STRUCT *board, SEARCH_INFO_STRUCT *info);
extern int Alpha_Beta(int alpha, int beta, int depth, int is_pv, BOARD_STRUCT *board, SEARCH_INFO_STRUCT *info);
extern int Quiescent_Search(int alpha, int beta, BOARD_STRUCT *board, SEARCH_INFO_STRUCT *info);
extern int Search_Position(BOARD_STRUCT *board, SEARCH_INFO_STRUCT *info);
extern int Draw_Error_Found(int move, BOARD_STRUCT *board);

//tuning
extern int king_end_piece_square_tuning_values[8];
extern int pawn_end_piece_square_tuning_values[7];
extern float passed_pawn_tuning_parameters[3];
extern void Set_King_End_Values(void);
extern void Set_Pawn_End_Values(void);
extern void Set_Passed_Pawn_Rank_Bonuses(void);

//uci
extern void Parse_Go(char* line, SEARCH_INFO_STRUCT *info, BOARD_STRUCT *board);
extern void Parse_Position(char* lineIn, BOARD_STRUCT *board);
extern int Parse_Move(char *ptrChar, BOARD_STRUCT *board);
extern void Uci_Loop(BOARD_STRUCT *pos, SEARCH_INFO_STRUCT *info);



