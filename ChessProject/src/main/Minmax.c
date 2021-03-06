

#include "Minmax.h"


int minmax(Game* game,int depth, int alpha, int beta, int isMaximizing, int isCalledByWhite) {

	//halting condition.
	if ( depth == 0 || isCurrentPlayerStuck(game) ) { //change to isPlayerStuck().
		return scoringFunction(game, isCalledByWhite);
	}

	//Current player is maximizing.
	if ( isMaximizing ) {

		Moves* allMoves = getAllMoves(game);
		Move* move = allMoves->first;

		int val;
		int bestValue = INT_MIN;

		Game* gameCopy = NULL;

		while ( move != NULL ) {

			gameCopy = cloneGame(game); //doesn't clone move field
			doMove(gameCopy, move,0, EMPTY);

			// switch turns before recursive call
			switchTurns(gameCopy);

			//find score by rec call
			val = minmax(gameCopy,depth-1,alpha,beta,0, isCalledByWhite);

			Move* tmpMove = move;
			move = move->next;

			if ( val == bestValue ) {
				Move* tmpMoveCopy = cloneMove(tmpMove);
				addToMinmaxMoves(game,tmpMoveCopy);
			}
			else if ( val > bestValue ) {
				bestValue = val;
				game->minmaxScore = bestValue;
				if ( game->minmaxMove != NULL ) {
					freeMinmaxMove(game->minmaxMove); //frees only when not null
				}
				game->minmaxMove = cloneMove(tmpMove);
			}


			freeMinmaxMove(gameCopy->minmaxMove);
			free(gameCopy);
			gameCopy = NULL;

			alpha = max(alpha, bestValue); //check where this needs to be put.
			if (alpha >= beta){
				break;
			}

		}
		freeAllMoves(allMoves);
		return bestValue;
	}

	//Current player is minimizing
	else {
		Moves* allMoves = getAllMoves(game);
		Move* move = allMoves->first;

		int val;
		int bestValue = INT_MAX;

		while ( move != NULL ) {
			Game* gameCopy = cloneGame(game); //doesn't clone minmax fields
			gameCopy->minmaxScore = INT_MAX;
			doMove(gameCopy, move,0, EMPTY);
			// switch turns before recursive call

			switchTurns(gameCopy);

			//find score by rec call
			val = minmax(gameCopy,depth-1,alpha,beta,1,isCalledByWhite);

			Move* tmpMove = move;
			move = move->next;
			if ( val == bestValue ) {
				Move* tmpMoveCopy = cloneMove(tmpMove);
				addToMinmaxMoves(game,tmpMoveCopy);
			}
			else if ( val < bestValue ) {
				bestValue = val;
				game->minmaxScore = bestValue;
				if ( game->minmaxMove != NULL ) {
					freeMinmaxMove(game->minmaxMove); //frees only when not null
				}
				game->minmaxMove = cloneMove(tmpMove);
			}

			freeMinmaxMove(gameCopy->minmaxMove);
			gameCopy->minmaxMove = NULL;
			free(gameCopy);
			gameCopy = NULL;

			beta = min(beta, bestValue);//check where this needs to be put.
			if (alpha >= beta){
				break;
			}

		}
		freeAllMoves(allMoves);
		return bestValue;
	}
}

void addToMinmaxMoves(Game* game, Move* tempMoveCopy){
	/* Add a move to the linked list */
	if (game->minmaxMove == NULL){
		game->minmaxMove = tempMoveCopy;
		return;
	}

	Move* temp = game->minmaxMove;
	game->minmaxMove = tempMoveCopy;
	game->minmaxMove->next = temp;
}

void freeMinmaxMove(Move* move){
	if (move != NULL){
		Move* currMove = move;
		while ( currMove != NULL ) {
			Move* prevMove=currMove;
			currMove = currMove->next;
			freeMove(prevMove);
			prevMove = NULL;
		}
	}
}

void freeAllMoves(Moves* allMoves){

	if (allMoves != NULL){
		Move* currMove = allMoves->first;
		while ( currMove != NULL ) {
			Move* prevMove=currMove;
			currMove = currMove->next;
			freeMove(prevMove);
		}
		free(allMoves);
	}
	allMoves = NULL;
}

int min(int x, int y){
	if (x>y){
		return y;
	}
	return x;
}

int max (int x, int y){
	if (x>y){
		return x;
	}
	return y;
}

Moves* getAllMoves(Game* game){

	Moves* allMoves = NULL;
	allMoves = calloc(sizeof(Moves), 1);
	if (allMoves == NULL){
		quit();
	}
	allMoves->first = NULL;
	allMoves->maxEats = 0;

	for (int i=0; i<BOARD_SIZE;i++){
		for (int j=0; j<BOARD_SIZE; j++){
			getMoves(game, i, j, 1);
			addMovesToAllMoves(allMoves);
			freeMoves(0);
		}
	}
	return allMoves;
}

int scoringFunction(Game* game, int isCalledByWhite) {
	/* Scoring function for the minimax. */
	int result;

	//player lost
	if ( isCurrentPlayerLose(game) ) {
		if ((isCalledByWhite && game->isWhiteTurn) || (!isCalledByWhite && !game->isWhiteTurn)){
			return INT_MIN;
		}
		else {
			return INT_MAX;
		}
	}
	//player in tie.
	if (isTie(game)){
		if ( (isCalledByWhite && game->isWhiteTurn) || (!isCalledByWhite && !game->isWhiteTurn)){
			return INT_MIN+1;
		}
		else {
			return INT_MAX-1;
		}
	}

	// no one lost- we got to depth 0
	int numOfWhitePawns = 0;
	int numOfWhiteKnights = 0;
	int numOfWhiteBishops = 0;
	int numOfWhiteRooks = 0;
	int numOfWhiteQueens = 0;
	int numOfWhiteKings = 0;

	int numOfBlackPawns = 0;
	int numOfBlackKnights = 0;
	int numOfBlackBishops = 0;
	int numOfBlackRooks = 0;
	int numOfBlackQueens = 0;
	int numOfBlackKings = 0;
	int i,j;
	for ( i=0; i<BOARD_SIZE; i ++) {
		for ( j=0; j<BOARD_SIZE; j++ ) {
			 if ( game->board[i][j] == WHITE_P ) {
				 numOfWhitePawns ++;
			 }
			 if ( game->board[i][j] == WHITE_N ) {
				 numOfWhiteKnights ++;
			 }
			 if ( game->board[i][j] == WHITE_B ) {
				 numOfWhiteBishops ++;
			 }
			 if ( game->board[i][j] == WHITE_R ) {
				 numOfWhiteRooks ++;
			 }
			 if ( game->board[i][j] == WHITE_Q ) {
				 numOfWhiteQueens ++;
			 }
			 if ( game->board[i][j] == WHITE_K ) {
				 numOfWhiteKings ++;
			 }

			 if ( game->board[i][j] == BLACK_P ) {
				 numOfBlackPawns ++;
			 }
			 if ( game->board[i][j] == BLACK_N ) {
				 numOfBlackKnights ++;
			 }
			 if ( game->board[i][j] == BLACK_B ) {
				 numOfBlackBishops ++;
			 }
			 if ( game->board[i][j] == BLACK_R ) {
				 numOfBlackRooks ++;
			 }
			 if ( game->board[i][j] == BLACK_Q ) {
				 numOfBlackQueens ++;
			 }
			 if ( game->board[i][j] == BLACK_K ) {
				 numOfBlackKings ++;
			 }
		}
	}
	int whitePlayerScore = numOfWhitePawns + 3*(numOfWhiteKnights+numOfWhiteBishops) + 5*numOfWhiteRooks + 9*numOfWhiteQueens + 400*numOfWhiteKings;
	int blackPlayerScore = numOfBlackPawns + 3*(numOfBlackKnights+numOfBlackBishops) + 5*numOfBlackRooks + 9*numOfBlackQueens + 400*numOfBlackKings;

	if (isCalledByWhite) {
		result = whitePlayerScore - blackPlayerScore;
	}
	else {
		result = blackPlayerScore - whitePlayerScore;
	}
	return result;
}

void addMovesToAllMoves(Moves* allMoves){

	if(moves->first == NULL){ // There are no moves in moves.
		return;
	}

	if (allMoves->first == NULL){
		Move* movesCurrMove = moves->first;
		allMoves->first = cloneMove(movesCurrMove); //doesn't clone 'next'.
		Move* allMovesCurrMove = allMoves->first;;
		while (movesCurrMove != NULL){
			allMovesCurrMove->next = cloneMove(movesCurrMove->next);
			movesCurrMove = movesCurrMove->next;
			allMovesCurrMove = allMovesCurrMove->next;
		}
	}

	else{
		Move* movesCurrMove = moves->first;
		while (movesCurrMove != NULL){
			Move* temp = allMoves->first;
			Move* newMove = cloneMove(movesCurrMove);
			allMoves->first = newMove;
			newMove->next = temp;
			movesCurrMove = movesCurrMove->next;
		}
	}
}

Move* getBestMoveForUser(Game* game){

	minmax(game,game->minmaxDepth, INT_MIN, INT_MAX, 1, game->isWhiteTurn); //updates game->move
	Move* currMove = game->minmaxMove;
	while (currMove != NULL){
		printMove(currMove);
		currMove = currMove->next;
	}
	Move* tempMove = cloneMove(game->minmaxMove);
	freeMinmaxMove(game->minmaxMove);
	game->minmaxMove = NULL; //just in case
	game->minmaxScore = INT_MIN;
	return tempMove;

}
