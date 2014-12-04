/* movegen.c
* Contains all functions for generating a movelist given a board pointer
* Thep Kanning 12/2/14
*/

#include "globals.h"

void Generate_Moves(BOARD_STRUCT *board, MOVE_LIST_STRUCT *movelist)
{
	int i, j, from120, to120, rank, file, piece, capture, slide_num;
	const int side = board->side; //Store side to move
	ASSERT((side == WHITE) || (side == BLACK));

	
	if (side == WHITE)
	{
		/***************************************/
		/**************** PAWNS ****************/
		/***************************************/

		//Loop through each pawn and test for every kind of move, separate loops for white and black
		piece = wP;
		for (i = 0; i < board->piece_num[wP]; i++)
		{
			//Get pawn index120
			from120 = board->piece_list120[wP][i];
			ASSERT(ON_BOARD_120(from120));

			/***** Pushes and Promotions *****/
			to120 = from120 + 10;
			ASSERT(ON_BOARD_120(to120));

			if (board->board_array120[to120] == EMPTY) //If next space is empty, this also means it is on the board
			{
				/***** Single Pushes and Promotions *****/
				//Add as a promotion if this leads to rank 8
				if (GET_RANK_120(to120) == RANK_8)
				{
					Add_Move(movelist, from120, to120, wP, 0, QUEEN_PROMOTE, QUEEN_PROMOTE_SCORE);
					Add_Move(movelist, from120, to120, wP, 0, KNIGHT_PROMOTE, KNIGHT_PROMOTE_SCORE);
				}
				else //If not a promotion
				{
					Add_Move(movelist, from120, to120, wP, 0, NOT_SPECIAL, 0);
				}
				
				/***** Double Pushes *****/
				to120 = from120 + 20;
				ASSERT(ON_BOARD_120(to120));

				//Same as before, first space in front already established as empty
				if ((GET_RANK_120(from120) == RANK_2) && (board->board_array120[to120] == EMPTY)) //If pawn is on starting rank, and second square is also empty
				{
					Add_Move(movelist, from120, to120, wP, 0, NOT_SPECIAL, 0);
				}
			}

			/***** Normal and EP Captures *****/
			//Check first target square
			to120 = from120 + 9;
			if (ON_BOARD_120(to120)) //If target square is on board
			{
				if (IS_BLACK_PIECE(board->board_array120[to120])) //If target has a piece that can be taken
				{
					capture = board->board_array120[to120];
					Add_Move(movelist, from120, to120, wP, capture, NOT_SPECIAL, GET_MMVLVA_SCORE(capture, wP));
				}
				else if (to120 == board->ep) //If target is ep square
				{
					capture = bP;
					Add_Move(movelist, from120, to120, wP, capture, EP_CAPTURE, GET_MMVLVA_SCORE(capture, wP));
				}
			}

			//Check second target square
			to120 = from120 + 11;
			if (ON_BOARD_120(to120)) //If target square is on board
			{
				if (IS_BLACK_PIECE(board->board_array120[to120])) //If target has a piece that can be taken
				{
					capture = board->board_array120[to120];
					Add_Move(movelist, from120, to120, wP, capture, NOT_SPECIAL, GET_MMVLVA_SCORE(capture, wP));
				}
				else if (to120 == board->ep) //If target is ep square
				{
					capture = bP;
					Add_Move(movelist, from120, to120, wP, capture, EP_CAPTURE, GET_MMVLVA_SCORE(capture, wP));
				}
			}
		}
		/***** End Pawn Moves *****/

		/***************************************/
		/*************** KNIGHTS ***************/
		/***************************************/

		for (i = 0; i < board->piece_num[wN]; i++) //Loop through all available white knights
		{
			from120 = board->piece_list120[wN][i];
			ASSERT(ON_BOARD_120(from120));
			//Cycle through all directions, adding move if on board, and adding capture if applicable
			for (j = 0; j < NUM_KNIGHT_MOVES; j++)
			{
				//Calculate target square
				to120 = from120 + KNIGHT_MOVES[j]; 

				//See if target square is on the board
				if (ON_BOARD_120(to120))
				{
					//Move if square is empty, or capture if it contains a black piece
					if (board->board_array120[to120] == EMPTY)
					{
						//Add quiet move
						Add_Move(movelist, from120, to120, wN, 0, NOT_SPECIAL, 0);
					}
					else if (IS_BLACK_PIECE(board->board_array120[to120]))
					{
						//Add capture
						capture = board->board_array120[to120];
						Add_Move(movelist, from120, to120, wN, capture, NOT_SPECIAL, GET_MMVLVA_SCORE(capture, wN));
					}
				}
			}
		}
		/***** End Knight Moves *****/

		/***************************************/
		/*************** BISHOPS ***************/
		/***************************************/

		for (i = 0; i < board->piece_num[wB]; i++) //Loop through all available white bishops
		{
			from120 = board->piece_list120[wB][i];
			ASSERT(ON_BOARD_120(from120));
			//Cycle through all directions, adding move if on board, and adding capture if applicable
			for (j = 0; j < NUM_BISHOP_MOVES; j++)
			{
				//Increase slide distance, adding moves until off board or hitting another piece
				for (slide_num = 1; slide_num < 8; slide_num++)
				{
					//Calculate target square based on direction and slide number
					to120 = from120 + slide_num * BISHOP_MOVES[j];

					//See if target square is on the board, if not, break loop
					if (ON_BOARD_120(to120))
					{
						//Move if square is empty, capture and break if it contains a black piece, break if white piece
						if (board->board_array120[to120] == EMPTY)
						{
							//Add quiet move
							Add_Move(movelist, from120, to120, wB, 0, NOT_SPECIAL, 0);
						}
						else if (IS_BLACK_PIECE(board->board_array120[to120]))
						{
							//Add capture
							capture = board->board_array120[to120];
							Add_Move(movelist, from120, to120, wB, capture, NOT_SPECIAL, GET_MMVLVA_SCORE(capture, wB));
							break;
						}
						else if (IS_WHITE_PIECE(board->board_array120[to120]))
						{
							break; //Stop sliding
						}
					}
					else
					{
						break; //Stop sliding in this direction
					}
				}
			}
		}
		/***** End Bishop Moves *****/

		/***************************************/
		/**************** ROOKS ****************/
		/***************************************/

		for (i = 0; i < board->piece_num[wR]; i++) //Loop through all available white rooks
		{
			from120 = board->piece_list120[wR][i];
			ASSERT(ON_BOARD_120(from120));

			//Cycle through all directions, adding move if on board, and adding capture if applicable
			for (j = 0; j < NUM_ROOK_MOVES; j++)
			{
				//Increase slide distance, adding moves until off board or hitting another piece
				for (slide_num = 1; slide_num < 8; slide_num++)
				{
					//Calculate target square based on direction and slide number
					to120 = from120 + slide_num * ROOK_MOVES[j];

					//See if target square is on the board, if not, break loop
					if (ON_BOARD_120(to120))
					{
						//Move if square is empty, capture and break if it contains a black piece, break if white piece
						if (board->board_array120[to120] == EMPTY)
						{
							//Add quiet move
							Add_Move(movelist, from120, to120, wR, 0, NOT_SPECIAL, 0);
						}
						else if (IS_BLACK_PIECE(board->board_array120[to120]))
						{
							//Add capture
							capture = board->board_array120[to120];
							Add_Move(movelist, from120, to120, wR, capture, NOT_SPECIAL, GET_MMVLVA_SCORE(capture, wR));
							break;
						}
						else if (IS_WHITE_PIECE(board->board_array120[to120]))
						{
							break; //Stop sliding
						}
					}
					else
					{
						break; //Stop sliding in this direction
					}
				}
			}
		}
		/***** End Rook Moves *****/

		/***************************************/
		/*************** QUEENS ****************/
		/***************************************/

		for (i = 0; i < board->piece_num[wQ]; i++) //Loop through all available white queens
		{
			from120 = board->piece_list120[wQ][i];
			ASSERT(ON_BOARD_120(from120));

			//Cycle through all directions, adding move if on board, and adding capture if applicable
			for (j = 0; j < NUM_QUEEN_MOVES; j++)
			{
				//Increase slide distance, adding moves until off board or hitting another piece
				for (slide_num = 1; slide_num < 8; slide_num++)
				{
					//Calculate target square based on direction and slide number
					to120 = from120 + slide_num * QUEEN_MOVES[j];

					//See if target square is on the board, if not, break loop
					if (ON_BOARD_120(to120))
					{
						//Move if square is empty, capture and break if it contains a black piece, break if white piece
						if (board->board_array120[to120] == EMPTY)
						{
							//Add quiet move
							Add_Move(movelist, from120, to120, wQ, 0, NOT_SPECIAL, 0);
						}
						else if (IS_BLACK_PIECE(board->board_array120[to120]))
						{
							//Add capture
							capture = board->board_array120[to120];
							Add_Move(movelist, from120, to120, wQ, capture, NOT_SPECIAL, GET_MMVLVA_SCORE(capture, wQ));
							break;
						}
						else if (IS_WHITE_PIECE(board->board_array120[to120]))
						{
							break; //Stop sliding
						}
					}
					else
					{
						break; //Stop sliding in this direction
					}
				}
			}
		}
		/***** End Queen Moves *****/

		/***************************************/
		/*************** King *****************/
		/***************************************/

		from120 = board->piece_list120[wK][0];
		ASSERT(ON_BOARD_120(from120));

		/***** Normal Moves and Captures *****/
		//Cycle through all directions, adding move if on board, and adding capture if applicable
		for (j = 0; j < NUM_KING_MOVES; j++)
		{
			//Calculate target square based on direction and slide number
			to120 = from120 + KING_MOVES[j];

			//See if target square is on the board
			if (ON_BOARD_120(to120))
			{
				//Move if square is empty, capture if it contains a black piece
				if (board->board_array120[to120] == EMPTY)
				{
					//Add quiet move
					Add_Move(movelist, from120, to120, wK, 0, NOT_SPECIAL, 0);
				}
				else if (IS_BLACK_PIECE(board->board_array120[to120]))
				{
					//Add capture
					capture = board->board_array120[to120];
					Add_Move(movelist, from120, to120, wK, capture, NOT_SPECIAL, GET_MMVLVA_SCORE(capture, wK));
				}
			}
		}
		/***** Castling *****/
		//MAKE SURE MOVE DOES NOT CROSS SPACES UNDER ATTACK LATER
		//Kingside
		if ((board->castle_rights & WK_CASTLE) && (board->board_array120[F1] == EMPTY) && (board->board_array120[G1] == EMPTY)) //If spaces are empty
		{
			Add_Move(movelist, from120, G1, wK, 0, KING_CASTLE, 0); //Always moves to C1
		}
		//Queenside
		if ((board->castle_rights & WQ_CASTLE) && (board->board_array120[B1] == EMPTY) && (board->board_array120[C1] == EMPTY) && (board->board_array120[D1] == EMPTY))
		{
			Add_Move(movelist, from120, C1, wK, 0, QUEEN_CASTLE, 0); //Always moves to C1
		}
		/***** End King Moves *****/

	}
	else //side == BLACK
	{

	}
}