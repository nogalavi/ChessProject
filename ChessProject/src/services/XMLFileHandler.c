#include "./XMLFileHandler.h"

int saveGameToFile(Game* game, char* path) {
	FILE* fp;
	if ( (fp = fopen(path, "w")) == NULL )
		return 0;

	char* nextTurn = (game->isWhiteTurn ? NEXT_IS_WHITE: NEXT_IS_BLACK);
	char* gameMode = (game->isTwoPlayersMode ? GAME_MODE_USER_USER : GAME_MODE_USER_COMPUTER);
	char* userColor = (game->isUserWhite ? USER_COLOR_WHITE : USER_COLOR_BLACK);
	char* minmaxDepth;
	if ( game->minmaxDepth == 1 ) minmaxDepth = "1";
	if ( game->minmaxDepth == 2 ) minmaxDepth = "2";
	if ( game->minmaxDepth == 3 ) minmaxDepth = "3";
	if ( game->minmaxDepth == 4 ) minmaxDepth = "4";
	char* difficulty = (game->isBest ? "Best" :  minmaxDepth );

	fprintf(fp,"%s\n", "<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
	fprintf(fp, "%s\n", TAG_GAME_S);
	fprintf(fp, "\t%s %s %s \n", TAG_NEXT_TURN_S, nextTurn, TAG_NEXT_TURN_E);
	fprintf(fp, "\t%s %s %s \n", TAG_GAME_MODE_S, gameMode, TAG_GAME_MODE_E);
	if ( !game->isTwoPlayersMode ) {
		fprintf(fp, "\t%s %s %s \n", TAG_DIFFICULTY_S, difficulty , TAG_DIFFICULTY_E);
		fprintf(fp, "\t%s %s %s \n", TAG_USER_COLOR_S, userColor , TAG_USER_COLOE_E);
	}
	else {
		fprintf(fp, "\t%s \n", EMPTY_DIFFICULTY_TAG);
		fprintf(fp, "\t%s \n", EMPTY_USER_COLOR_TAG);;
	}
	fprintf(fp, "\t%s\n", TAG_BOARD_S);

	for (int i = 0; i < BOARD_SIZE; i++){
		fprintf(fp, "\t\t<row_%d>", BOARD_SIZE - i);
			for (int j = 0; j < BOARD_SIZE; j++){
				if ( (game->board[j][BOARD_SIZE -1 - i]) == EMPTY ) {
					fprintf(fp, "_");
				} else {
					fprintf(fp,"%c", game->board[j][BOARD_SIZE -1 - i]);
				}
			}
			fprintf(fp, "</row_%d>\n", BOARD_SIZE - i);
		}
		fprintf(fp, "\t%s\n", TAG_BOARD_E);
		fprintf(fp, "%s\n", TAG_GAME_E);

		fclose(fp);

	return 1;
}

int loadGameFromFile(Game* game, char* path) {
	FILE* fp;
	if ((fp = fopen(path, "r")) == NULL) {
		return 0;
	}
	char row[50], *p;
	fgets(row, 50, fp);

	while ( !strstr(row, TAG_GAME_E) ) {
		if (strstr(row, TAG_NEXT_TURN_S)) {
			if (strstr(row, NEXT_IS_WHITE)) {
				game->isWhiteTurn = 1;
			} else {
				game->isWhiteTurn = 0;
			}
		} else if (strstr(row, TAG_GAME_MODE_S)) {
			if (strstr(row, GAME_MODE_USER_USER)) {
				game->isTwoPlayersMode = 1;
			} else {
				game->isTwoPlayersMode = 0;
			}
		}
		if (!game->isTwoPlayersMode) {
			if (strstr(row, TAG_DIFFICULTY_S)) {
				if (strstr(row, "1")) game->minmaxDepth = 1;
				if (strstr(row, "2")) game->minmaxDepth = 2;
				if (strstr(row, "3")) game->minmaxDepth = 3;
				if (strstr(row, "4")) game->minmaxDepth = 4;
				if (strstr(row, "best")) game->minmaxDepth = 4;
			}
			if (strstr(row, TAG_USER_COLOR_S)) {
				if (strstr(row, USER_COLOR_WHITE)) {
					game->isUserWhite = 1;
				} else {
					game->isUserWhite = 0;
				}
			}
		}
		else {
			game->minmaxDepth = 1;
			game->isUserWhite = 1;
		}

		if (strstr(row,"row")) {
			for (int k = 0; k < BOARD_SIZE; k++){
				p = row + 9; //skip the tag
				for (int j = 0; j < BOARD_SIZE; j++){

					if ( *p == '_') {
						game->board[j][BOARD_SIZE-1 - k] = EMPTY;
					} else {
						game->board[j][BOARD_SIZE-1 - k] = *p;
					}
					p++;
				}
				fgets(row, 50, fp);
			}
		fclose(fp);
		return 1;
		}
		fgets(row, 50, fp);
	}
	fclose(fp);
	return 1;
}

int doesSlotContainFile(int i) {
	 FILE* file;
	 char fileName[10];
	 sprintf(fileName, "%s%d.%s", SLOT_SEED, i,"xml");
	 if  ( (file = fopen(fileName, "r")) != NULL )   {
	        fclose(file);
	        return 1;
	  }
	  return 0;
}

int saveToSlot(Game* game, int i) {
	/** Saves an xml in a given slot, in the project directory **/

	 char fileName[10];
	 sprintf(fileName, "%s%d.%s", SLOT_SEED, i,"xml");

	 if ( saveGameToFile(game, fileName) == 0) // save failed
		 return 0;
	 return 1;
}

int loadFromAFullSlot(Game* game, int slotNumber) {

	 char fileName[10];
	 sprintf(fileName, "%s%d.%s", SLOT_SEED, slotNumber,"xml");
	 if ( loadGameFromFile(game, fileName) == 0) {
		 return 0;
	 }
	 return 1;
}
