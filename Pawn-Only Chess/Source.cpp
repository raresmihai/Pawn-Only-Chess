#include <iostream>
#include <string>
#include<stdio.h>
#include<map>
#include<utility>
#include<vector>
#include <stdlib.h>     
#include <time.h>       

using namespace std;
class ChessBoard {
private:
	int nivel;
	map<int, pair<int, int>> white_pieces_map;
	map<int, pair<int, int>> black_pieces_map;
	bool game_done;
	bool opponent_moved_two_squares;
	int column_two_squares;
	int row_two_squares;
	char a[8][8];
	FILE *fd;
	void createInitialState() {
		fd = fopen("configuratie_initiala.txt", "r");
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				fscanf(fd, " %c", &a[i][j]);
				if (a[i][j] == 'N'){
					black_pieces_map[j] = make_pair(i, j);
				}
				if (a[i][j] == 'A'){
					white_pieces_map[j] = make_pair(i, j);
				}
			}
		}
		fclose(fd);
	}
	void delete_piece(int row, int col, char culoare){
		map<int, pair<int, int>>::iterator it;
		if (culoare == 'A'){
			for (int i = 0; i < 8; ++i){
				if (white_pieces_map[i].first == row && white_pieces_map[i].second == col){
					it = white_pieces_map.find(i);
					white_pieces_map.erase(it);
				}
			}
		}
		else{
			for (int i = 0; i < 8; ++i){
				if (black_pieces_map[i].first == row && black_pieces_map[i].second == col){
					it = black_pieces_map.find(i);
					black_pieces_map.erase(it);
				}
			}
		}
		a[row][col] = '0';
	}
	bool insideMap(int coordinate){
		return coordinate >= 0 && coordinate < 8;
	}
	void move_piece(int fromR, int fromC, int toR, int toC, char colour){
		if (colour == 'N'){
			cout << "\nNegru a mutat de la " << char(fromC + 'A') << char(fromR + '0' + 1) << " la " << char(toC + 'A') << char(toR + '0' + 1)<<"\n";
			for (int i = 0; i < 8; ++i){
				if (black_pieces_map[i].first == fromR && black_pieces_map[i].second == fromC){
					black_pieces_map[i] = make_pair(toR, toC);
					if (a[toR][toC] == 'A'){
						delete_piece(toR, toC, 'A');
					}
					a[toR][toC] = 'N';
					if (toR == 7){
						cout << "\n------\nNegru a castigat jocul!\n------\n";
						game_done = true;
					}
				}
			}
		}
		else{
			cout << "\nAlb a mutat de la " << char(fromC + 'A') << char(fromR + '0' + 1) << " la " << char(toC + 'A') << char(toR + '0' + 1)<<"\n";
			for (int i = 0; i < 8; ++i){
				if (white_pieces_map[i].first == fromR && white_pieces_map[i].second == fromC){
					white_pieces_map[i] = make_pair(toR, toC);
					if (a[toR][toC] == 'N'){
						delete_piece(toR, toC, 'N');
					}
					a[toR][toC] = 'A';
					if (toR == 0){
						cout << "\n------\nAlb a castigat jocul!\n------\n";
						game_done = true;
					}
				}
			}
		}
		a[fromR][fromC] = '0';
	}

	bool blackValidTransitionInsideMap(int fromC, int fromR, int toC, int toR){
		if (a[fromR][fromC] != 'N'){
			return false;
		}if ((fromR >= toR) || (toR - fromR > 2)){
			return false;
		}
		if (toR - fromR == 1){
			if (abs(toC - fromC) > 1){
				return false;
			}
			if (toC == fromC && a[toR][toC] != '0'){
				return false;
			}
			if (abs(toC - fromC) == 1 && a[toR][toC] != 'A'){
				return false;
			}
		}
		if (toR - fromR == 2){
			if (fromR != 1){
				return false;
			}
			if (fromC != toC){
				return false;
			}
			if (a[fromR + 1][toC] != '0' || a[toR][toC] != '0'){
				return false;
			}
		}
		return true;
	}
	bool whiteValidTransitionInsideMap(int fromC, int fromR, int toC, int toR){
		if (a[fromR][fromC] != 'A'){
			return false;
		}
		if ((fromR <= toR) || (fromR - toR > 2)) {
			return false;
		}
		if (fromR - toR == 1){
			if (abs(toC - fromC) > 1){
				return false;
			}
			if (toC == fromC && a[toR][toC] != '0'){
				return false;
			}
			if (abs(toC - fromC) == 1 && a[toR][toC] != 'N'){
				return false;
			}
		}
		if (fromR - toR == 2){
			if (fromR != 6){
				return false;
			}
			if (fromC != toC){
				return false;
			}
			if (a[fromR - 1][toC] != '0' || a[toR][toC] != '0'){
				return false;
			}
		}
		return true;
	}
	bool transitionIsValid(int fromC, int fromR, int toC, int toR, char colour) {
		if (!insideMap(fromC) || !insideMap(fromR) || !insideMap(toC) || !insideMap(toR)){
			return false;
		}
		if (colour == 'A'){
			return whiteValidTransitionInsideMap(fromC, fromR, toC, toR);
		}
		else{
			return blackValidTransitionInsideMap(fromC, fromR, toC, toR);
		}
	}
	int drum_liber(int linie, int coloana){
		int poate_avansa;
		if (transitionIsValid(coloana, linie, coloana, linie + 1, 'N')){
			if (transitionIsValid(coloana, linie, coloana, linie + 2, 'N')){
				poate_avansa = 2;
			}
			else{
				poate_avansa = 1;
			}
		}
		else{
			return 0;
		}
		for (int j = linie+1; j < 8; ++j){
			if (a[j][coloana] == 'A'){
				return 0;
			}
		}
		if (insideMap(coloana - 1)){
			for (int j = linie+1; j < 8; ++j){
				if (a[j][coloana - 1] == 'A'){
					return 0;
				}
			}
		}
		if (insideMap(coloana + 1)){
			for (int j = linie+1; j < 8; ++j){
				if (a[j][coloana + 1] == 'A')
					return 0;
			}
		}
		return poate_avansa;
	}
	bool Strategie_0(){
		cout << "\n----Strategie 0---\n";
		for (int i = 0; i < 8; ++i){
			int current_row = black_pieces_map[i].first;
			int current_column = black_pieces_map[i].second;
			int avansare_drum_liber = drum_liber(current_row, current_column);
			if (avansare_drum_liber == 2){
				move_piece(current_row, current_column, current_row + 2, current_column, 'N');
				return true;
			}
			else if (avansare_drum_liber == 1){
				move_piece(current_row, current_column, current_row + 1, current_column, 'N');
				return true;
			}
		}
		return false;
	}
	bool findEnPassant(int row, int col){
		map<int, pair<int, int>>::iterator it;
		for (int i = 0; i < 8; ++i){
			if (black_pieces_map[i].first == row && abs(col - black_pieces_map[i].second) == 1){
				move_piece(row, black_pieces_map[i].second, row + 1, col, 'N');
				cout << "\n------------En Passant!-------------\n";
				return true;
			}
		}
	}
	bool isSafeForBlack(int row, int col){
		if (insideMap(col + 1) && insideMap(row + 1)){
			if (a[row + 1][col + 1] == 'A'){
				return false;
			}
		}
		if (insideMap(col - 1) && insideMap(row + 1)){
			if (a[row + 1][col - 1] == 'A'){
				return false;
			}
		}
		return true;
	}
	void attack(vector<pair<pair<int, int>,pair<int, int>>> *attackers_list){
		int size = (*attackers_list).size();
		int random_position = rand() % size;
		int current_row = (*attackers_list)[random_position].first.first;
		int current_column = (*attackers_list)[random_position].first.second;
		int row_to_attack = (*attackers_list)[random_position].second.first;
		int column_to_attack = (*attackers_list)[random_position].second.second;
		move_piece(current_row, current_column, row_to_attack, column_to_attack, 'N');
	}
	bool Strategie_1(){
		cout << "\n----Strategie 1---\n";
		if (opponent_moved_two_squares){
			if (findEnPassant(row_two_squares, column_two_squares)){
				return true;
			}
		}
		srand(time(NULL));
		//first pair - who can attack, second pair - who can be attacked
		vector <pair<pair<int, int>, pair<int, int>>> possible_attackers;
		vector <pair<pair<int, int>, pair<int, int>>> possible_safe_attackers;
		for (int i = 0; i < 8; ++i){
			int current_row = black_pieces_map[i].first;
			int current_column = black_pieces_map[i].second;
			if (transitionIsValid(current_column, current_row, current_column + 1, current_row + 1, 'N')){
				if (isSafeForBlack(current_row + 1, current_column + 1)){
					possible_safe_attackers.push_back(make_pair(make_pair(current_row, current_column), make_pair(current_row + 1, current_column + 1)));
				}
				else{
					possible_attackers.push_back(make_pair(make_pair(current_row, current_column), make_pair(current_row + 1, current_column + 1)));
				}
			}
			if (transitionIsValid(current_column, current_row, current_column - 1, current_row + 1, 'N')){
				if (isSafeForBlack(current_row + 1, current_column - 1)){
					possible_safe_attackers.push_back(make_pair(make_pair(current_row, current_column), make_pair(current_row + 1, current_column - 1)));
				}
				else{
					possible_attackers.push_back(make_pair(make_pair(current_row, current_column), make_pair(current_row + 1, current_column - 1)));
				}
			}
		}
		if (possible_safe_attackers.size() != 0){
			attack(&possible_safe_attackers);
			return true;
		}
		else{
			if (possible_attackers.size() != 0){
				attack(&possible_attackers);
				return true;
			}
		}
		return false;
	}

	bool Strategie_2(){
		cout << "\n----Strategie 2---\n";
		int min_row=10;
		int col_for_min_row;
		for (int i = 0; i < 7; ++i){
			for (int j = i + 1; j < 8; ++j){
				int current_row_i = black_pieces_map[i].first;
				int current_col_i = black_pieces_map[i].second;
				int current_row_j = black_pieces_map[j].first;
				int current_col_j = black_pieces_map[j].second;
				if (current_row_i == current_row_j && abs(current_col_i - current_col_j) == 1){
					if (current_col_i % 2 ==0 ){
						if (transitionIsValid(current_col_i, current_row_i, current_col_i, current_row_i + 1, 'N')){
							if (current_row_i < min_row){
								min_row = current_row_i;
								col_for_min_row = current_col_i;
							}
						}
					}
					else{
						if (transitionIsValid(current_col_j, current_row_j, current_col_j, current_row_j + 1, 'N')){
							if (current_row_j < min_row){
								min_row = current_row_j;
								col_for_min_row = current_col_j;
							}
						}
					}
				}
			}
		}
		if (min_row != 10){
			move_piece(min_row, col_for_min_row, min_row + 1, col_for_min_row, 'N');
			return true;
		}
		return false;
	}
	void Strategie_3(){
		cout << "\n----Strategie 3---\n";
		int min_row = 10;
		int col_for_min_row;
		for (int i = 0; i < 8; ++i){
			int current_row = black_pieces_map[i].first;
			int current_col = black_pieces_map[i].second;
			if (current_row < min_row && transitionIsValid(current_col, current_row, current_col, current_row + 1, 'N')){
				min_row = current_row;
				col_for_min_row = current_col;
			}
		}
		if (transitionIsValid(col_for_min_row, min_row, col_for_min_row, min_row + 2, 'N')){
			move_piece(min_row, col_for_min_row, min_row + 2, col_for_min_row, 'N');
		}
		else{
			move_piece(min_row, col_for_min_row, min_row + 1, col_for_min_row, 'N');
		}
	}
	void applyStrategies() {
		if (!Strategie_0()){
			if (!Strategie_1()){
				if (!Strategie_2()){
					Strategie_3();
				}
			}
		}
	}
	void moveRandom(){
		vector<pair<pair<int, int>,pair<int,int>>> canMove;
		for (int i = 0; i < 8; ++i){
			int current_row = black_pieces_map[i].first;
			int current_column = black_pieces_map[i].second;
			if (transitionIsValid(current_column, current_row, current_column, current_row + 1,'N')){
				canMove.push_back(make_pair(make_pair(current_row, current_column), make_pair(current_row + 1, current_column)));
			}
			if (transitionIsValid(current_column, current_row, current_column, current_row + 2, 'N')){
				canMove.push_back(make_pair(make_pair(current_row, current_column), make_pair(current_row + 2, current_column)));
			}
			if (transitionIsValid(current_column, current_row, current_column+1, current_row + 1, 'N')){
				canMove.push_back(make_pair(make_pair(current_row, current_column), make_pair(current_row + 1, current_column + 1)));
			}
			if (transitionIsValid(current_column, current_row, current_column - 1, current_row + 1, 'N')){
				canMove.push_back(make_pair(make_pair(current_row, current_column), make_pair(current_row + 1, current_column - 1 )));
			}
		}
		int size = canMove.size();
		int random_move = rand() % size;
		move_piece(canMove[random_move].first.first, canMove[random_move].first.second, canMove[random_move].second.first, canMove[random_move].second.second, 'N');
	}
public:
	void outputTable() {
		cout << "\n";
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
					cout << a[i][j];
			}
			cout << i + 1;
			cout << "\n";
		}
		for (char x = 'A'; x <= 'H'; ++x){
			cout << x;
		}
		cout << "\n";
	}
	void play() {
		start_game:
			cout << "\nAlegeti nivelul de joc, 0 - easy, 1- hard!\n";
			cin >> nivel;
			if (nivel != 0 && nivel != 1){
				goto start_game;
			}
		while (!game_done) {
			opponent_turn:
				cout << "\nIt's your turn!\n";
				char fromColumn, fromRow, toColumn, toRow;
				cin >> fromColumn >> fromRow >> toColumn >> toRow;
				int fromC = fromColumn - 'A';
				int fromR = (fromRow - '0' - 1);
				int toC = toColumn - 'A';
				int toR = (toRow - '0' - 1);
				if (fromR - toR == 2){
					opponent_moved_two_squares = true;
					column_two_squares = toC;
					row_two_squares = toR;
				}
				else{
					opponent_moved_two_squares = false;
				}
				if (transitionIsValid(fromC, fromR, toC, toR, 'A')) {
					move_piece(fromR, fromC, toR, toC, 'A');
	    		}
				else{
					cout << "Mutare invalida!";
					goto opponent_turn;
				}
				if (game_done){
					continue;
				}
				outputTable();
				if (nivel == 1){
					applyStrategies();
				}
				else{
					moveRandom();
				}
				outputTable();
		}
	}
	ChessBoard() {
		createInitialState();
		game_done = false;
		outputTable();
		play();
	}
};

int main() {
	ChessBoard c = ChessBoard();
	int x;
	cin >> x;
}