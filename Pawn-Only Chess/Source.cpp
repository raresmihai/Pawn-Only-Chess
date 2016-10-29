#include <iostream>
#include <string>
using namespace std;
class ChessBoard {
private:
	char a[9][9];
	void createInitialState() {
		for (int i = 0; i <= 8; i++) {
			for (int j = 0; j <= 8; j++) {
				a[i][j] = '0';
			}
		}
		for (int i = 0; i <= 7; i++) {
			char c = char('A' + i);
			a[8][i] = c;
		}
		for (int i = 0; i <= 7; i++) {
			char c = char('0' + (8-i));
			a[i][8] = c;
		}
		for (int i = 0; i <= 7; i++) {
			a[1][i] = 'N';
		}
		for (int i = 0; i <= 7; i++) {
			a[6][i] = 'A';
		}
	}
	bool transitionIsValid(int fromC, int fromR, int toC, int toR) {
		//TO DO
		return true;
	}
	void move() {

	}
public:
	void outputTable() {
		for (int i = 0; i <= 8; i++) {
			for (int j = 0; j <= 8; j++) {
				if(i!=8||j!=8)
					cout << a[i][j];
			}
			cout << "\n";
		}
	}
	void play() {
		while (1) {
			cout << "\nIt's your turn!\n";
			char fromColumn, fromRow, toColumn, toRow;
			cin >> fromColumn >> fromRow >> toColumn >> toRow;
			int fromC = fromColumn - 'A';
			int fromR = 8 - (fromRow - '0');
			int toC = toColumn - 'A';
			int toR = 8 - (toRow - '0');
			if (transitionIsValid(fromC, fromR, toC, toR)) {
				a[fromR][fromC] = '0';
				a[toR][toC] = 'A';
			}
			outputTable();
			//sleep(2);
			move();
		}
	}
	ChessBoard() {
		createInitialState();
		outputTable();
		play();
	}
};

int main() {
	ChessBoard c = ChessBoard();
	int x;
	cin >> x;
}