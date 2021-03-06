#include <bits/stdc++.h>
#include <termios.h>
#include <unistd.h>

using namespace std;

vector<vector<int>> table;
vector<vector<int>> used;

vector<vector<int>> visiualtable;

mt19937 rnd(time(0));

int n = 20;
int m = 20;
int per = 0;
int c = n*m;

int x = 1; int y = 1;

//===================== font
#define BOLD 1
#define UNDERLINED 4
#define INVISIBLE 8
#define BLINKING 5
//===================== font color
#define BLACK 30
#define RED 31
#define WHITE 37
#define BLUE 34
#define GREEN 32
//===================== phone color
#define BG_BLACK 40
#define BG_RED 41
#define BG_WHITE 47
#define BG_BLUE 44

#define right 108
#define down 107
#define left 106
#define up 105

#define righta 3067
#define downa 3066
#define lefta 3068
#define upa 3065

#define enter 10
#define space 32
#define r 114

#define NONE -1
class Console{
public:
	class Image{
	public:
	    struct font {
	        int font_type, font_color, bg_color;

	        font(int _a = -1, int _b = -1, int _c = -1) {
	            font_type = _a;
	            font_color = _b;
	            bg_color = _c;
	        }

	        string get_string() {
	            string ans;
	            if (font_type != -1) {
	                ans.append(to_string(font_type));
	                if (font_color != -1 || bg_color != -1)ans.push_back(';');
	            }
	            if (font_color != -1) {
	                ans.append(to_string(font_color));
	                if (bg_color != -1) ans.push_back(';');
	            }
	            if (bg_color != -1) { ans.append(to_string(bg_color)); }
	            return ans;
	        }
	    };

	    static void printc(int x, int y, const std::string &s, font f) {
	        string cmd = f.get_string();
	        printf("\033[%d;%dH\x1b[%sm%s\x1b[0m", x+1, y+1, cmd.c_str(), s.c_str());
	        fflush(stdout);
	    }

	    static void prints(const std::string s, font f){
	    	string cmd = f.get_string();
	    	for(int i = 0; i < (int)s.size(); i++){
	    		string ch = s.substr(i, 1);
	    	 	printf("\x1b[%sm%s\x1b[0m", cmd.c_str(), ch.c_str());
	    	}
	    }
	};

	static void printcur(int i, int j, Image::font f){
		if(visiualtable[i][j] == -3) {Image::printc(i, j, "#", f); return;}
		if(visiualtable[i][j] > -1 && table[i][j] == 0) Image::printc(i, j, "???", f);
		else if(visiualtable[i][j] > -1) Image::printc(i, j, to_string(visiualtable[i][j]), f);
		else Image::printc(i, j, ".", f);
	}

	static void print(vector<vector<int>> &table){
		for(int i = 1; i <= m; i++) Image::printc(0, i, "???", {NONE, NONE, NONE});
		for(int i = 1; i <= m; i++) Image::printc(n+1, i, "???", {NONE, NONE, NONE});
		for(int i = 1; i <= n; i++) Image::printc(i, m+1, "???", {NONE, NONE, NONE});
		for(int i = 1; i <= n; i++) Image::printc(i, 0, "???", {NONE, NONE, NONE});
		Image::printc(n+1, 0, "???", {NONE, NONE, NONE});
		Image::printc(0, m+1, "???", {NONE, NONE, NONE});
		Image::printc(n+1, m+1, "???", {NONE, NONE, NONE});
		Image::printc(0, 0, "???", {NONE, NONE, NONE});
		for(int i = 1; i <= n; i++) {for(int j = 1; j <= m; j++) printcur(i, j, {NONE, BLACK, BG_WHITE}); cout << "\n";}
	}

	static void printstr(int x, int y, string s){
		for(int i = 0; i < (int)s.size(); i++){
			Image::printc(x, y+i, s.substr(i, 1), {NONE, NONE, NONE});
		}
	}
};

class Keyboard{
public:
	class GetKeys{
	public:
	    static int getkey(){
	     struct termios oldt,
	     newt;
	     int ch;
	     tcgetattr( STDIN_FILENO, &oldt );
	     newt = oldt;
	     newt.c_lflag &= ~( ICANON | ECHO );
	     tcsetattr( STDIN_FILENO, TCSANOW, &newt );
	     ch = getchar();
	     if(ch == 27){
	        ch = getchar();
	        if(ch != 91) {tcsetattr( STDIN_FILENO, TCSANOW, &oldt ); return ch;}
	        ch = getchar();
	        return ch + 3000;
	     }
	     tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
	     return ch;
	    }
	};
	
	static int getcommand(){
		int ch = Keyboard::GetKeys::getkey();
		return ch;
	}

	static int movement(){
		int s = getcommand();
		switch(s){
			case right:
			if(y < m) y++;
			break;
			case righta:
			if(y < m) y++;
			break;
			case down:
			if(x < n) x++;
			break;
			case downa:
			if(x < n) x++;
			break;
			case left:
			if(y > 1) y--;
			break;
			case lefta:
			if(y > 1) y--;
			break;
			case up:
			if(x > 1) x--;
			break;
			case upa:
			if(x > 1) x--;
			break;
			case space:
			return 1;
			case enter:
			return 2;
			break;
			default:
			return 0;
		}
		return 0;
	}
};

class Game{
public:
	static void bfs(pair<int, int> v){
		if(used[v.first][v.second] == 1) return;

		used[v.first][v.second] = 1;

		if(table[v.first][v.second] != -1) visiualtable[v.first][v.second] = table[v.first][v.second];
		
		if(table[v.first][v.second] != 0) return;

		int x = v.first;
		int y = v.second;

		bfs({x-1, y});
		bfs({x+1, y});
		bfs({x, y+1});
		bfs({x, y-1});
		bfs({x-1, y+1});
		bfs({x-1, y-1});
		bfs({x+1, y+1});
		bfs({x+1, y-1});
	}

	static bool ifwin(){
		for(int i = 1; i <= n; i++){
			for(int j = 1; j <= m; j++){
				if(table[i][j] != -1 && visiualtable[i][j] == -2) return false;
			}
		}
		return true;
	}

	static void printlose(){
		for(int i = 1; i <= n; i++){
			for(int j = 1; j <= m; j++){
				if(table[i][j] == -1){
				Console::Image::printc(i, j, "*", {BOLD, ((visiualtable[i][j] != -3) ? RED : GREEN), BG_WHITE});
				}else{
				Console::printcur(i, j, {NONE, BLACK, BG_WHITE});
				}
			}
			cout << "\n";
		}
	}

	static void buildfield(int xx, int yy){
		set<pair<int, int>> st;

		for(int i = 0; (int)st.size() < c; i++){
			int x = rnd() % n + 1;
			int y = rnd() % m + 1;
			if(st.count({x, y}) != 0 || (abs(x-xx) <= 1 && abs(y-yy) <= 1)) continue;
			st.insert({x, y});
			table[x][y] = -1;
		}
	}

	static void buildtable(){
		for(int i = 1; i <= n; i++){
			for(int j = 1; j <= m; j++){
				if(table[i][j] == -1) continue;
				for(int k = max(i-1, 1); k <= min(i+1, n); k++){
					for(int l = max(j-1, 1); l <= min(j+1, m); l++){
						if(table[k][l] == -1) table[i][j]++;
					}
				}
			}
		}
	}
};

void game(){
	table.resize(n+2, vector<int> (m+2, -1)); for(int i = 1; i <= n; i++) for(int j = 1; j<=m; j++) table[i][j] = 0;

	visiualtable.resize(n+2, vector<int> (m+2, -2));
	used.resize(n+2, vector<int> (m+2, 0));

	Console::print(visiualtable);

	Console::printstr(n+3, 0, "press i, j, k, l to move");
	Console::printstr(n+3, 29, "press Enter to put/delete a flag");
	Console::printstr(n+3, 63, "press Space to open some space");

	//int a, b;
	//Keyboard::getkey();
	int lastx = 0, lasty = 0;

	//bfs(findfirstzero());
	Console::print(visiualtable);

	Console::printcur(1,1, {NONE, WHITE, BG_BLACK});
	int count = 0;
	
	while(1){
		//Image::printc(n+5, 0, " ", {INVISIBLE, NONE, NONE});
		if(Game::ifwin() && count > 0){
			system("clear");
			Console::Image::prints("You WIN\n", {BOLD, RED, NONE});
			//cout << "Your time: " <<  (double)clock() / CLOCKS_PER_SEC << "\n";
			//auto ch = Keyboard::getkey();
			exit(0);
		}
		lastx = x;
		lasty = y;
		int bl = Keyboard::movement();
		if(bl == 2){
			if(visiualtable[x][y] == -3){
				if(used[x][y]){
					if(table[x][y] != 0){
					Console::Image::printc(x, y, to_string(table[x][y]), {BOLD, BLACK, BG_WHITE});
					}else{
					Console::Image::printc(x, y, "???", {BOLD, BLACK, BG_WHITE});
					}
					visiualtable[x][y] = table[x][y];
				}else{
					Console::Image::printc(x, y, ".", {BOLD, BLACK, BG_WHITE});
					visiualtable[x][y] = -1;
				}
				continue;
			}
			Console::Image::printc(x, y, "#", {BOLD, BLACK, BG_WHITE});
			visiualtable[x][y] = -3;
			continue;
		}
		Console::printcur(lastx, lasty, {NONE, BLACK, BG_WHITE});
		Console::printcur(x, y, {NONE, WHITE, BG_BLACK});
		if(!bl) continue;
		int a = x;
		int b = y;
		if(table[a][b] == -1){
			system("clear");
			Console::Image::prints("You LOSE\n", {BOLD, RED, NONE});
			Game::printlose();
			exit(0);
		}
		if(count == 0) { Game::buildfield(a, b); Game::buildtable();}
		Game::bfs({a, b});
		count++;
		Console::print(visiualtable);
		Console::printcur(x, y, {NONE, WHITE, BG_BLACK});
	}
}

int main(){
	cout << " _   __  __ _              \n| |_|  \\/  (_)_ _  ___ _ _ \n|  _| |\\/| | | ' \\/ -_) '_|\n \\__|_|  |_|_|_||_\\___|_|  \n\n\n\n\nPress any button to start\n";
    Keyboard::GetKeys::getkey();
    system("clear");
	Console::Image::prints("ENTER N, M, \% OF MINES IN THE FIELD\n", {BOLD, RED, NONE});
	cin >> n >> m >> per;
	c = n * m * per / 100;
	system("clear");
	//Keyboard::getkey();
	//Ima::prints("i or ^ - to move up\nj or <- - to move left\nk or v - to move down\nl or -> - to move Right\nEnter - to dig\nr - to put a flag", {BOLD, RED, NONE});
	Keyboard::GetKeys::getkey();
	system("clear");
	cout << /*(show ? "\033[?25h" : */"\033[?25l"/*)*/;
	game();
}
