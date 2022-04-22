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
#define r 114


#define NONE -1

class IMAGE{
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

class KEYBOARD{
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


int getcommand(){
	int ch = KEYBOARD::getkey();
	return ch;
}

int movement(){
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
		case enter:
		return 1;
		case r:
		return 2;
		break;
		default:
		return 0;
	}
	return 0;
}

void printcur(int i, int j, IMAGE::font f){
	if(visiualtable[i][j] == -3) {IMAGE::printc(i, j, "#", f); return;}
	if(visiualtable[i][j] > -1 && table[i][j] == 0) IMAGE::printc(i, j, "░", f);
	else if(visiualtable[i][j] > -1) IMAGE::printc(i, j, to_string(visiualtable[i][j]), f);
	else IMAGE::printc(i, j, ".", f);
}

void print(vector<vector<int>> &table){
	for(int i = 1; i <= m; i++) IMAGE::printc(0, i, "━", {NONE, NONE, NONE});
	for(int i = 1; i <= m; i++) IMAGE::printc(n+1, i, "━", {NONE, NONE, NONE});
	for(int i = 1; i <= n; i++) IMAGE::printc(i, m+1, "┃", {NONE, NONE, NONE});
	for(int i = 1; i <= n; i++) IMAGE::printc(i, 0, "┃", {NONE, NONE, NONE});
	IMAGE::printc(n+1, 0, "┗", {NONE, NONE, NONE});
	IMAGE::printc(0, m+1, "┓", {NONE, NONE, NONE});
	IMAGE::printc(n+1, m+1, "┛", {NONE, NONE, NONE});
	IMAGE::printc(0, 0, "┏", {NONE, NONE, NONE});
	for(int i = 1; i <= n; i++) {for(int j = 1; j <= m; j++) printcur(i, j, {NONE, BLACK, BG_WHITE}); cout << "\n";}
}

void bfs(pair<int, int> v){
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

bool ifwin(){
	for(int i = 1; i <= n; i++){
		for(int j = 1; j <= m; j++){
			if(table[i][j] != -1 && visiualtable[i][j] == -2) return false;
		}
	}
	return true;
}

pair<int, int> findfirstzero(){
	for(int i = 1; i <= n; i++){
		for(int j = 1; j <= m; j++){
			if(table[i][j] == 0) return {i, j};
		}
	}
	return {0, 0};
}

void printlose(){
	for(int i = 1; i <= n; i++){
		for(int j = 1; j <= m; j++){
			if(table[i][j] == -1){
			IMAGE::printc(i, j, "*", {BLINKING, RED, BG_WHITE});
			}else{
			printcur(i, j, {NONE, BLACK, BG_WHITE});
			}
		}
		cout << "\n";
	}
}

void game(){
	table.resize(n+2, vector<int> (m+2, -1)); for(int i = 1; i <= n; i++) for(int j = 1; j<=m; j++) table[i][j] = 0;

	visiualtable.resize(n+2, vector<int> (m+2, -2));
	used.resize(n+2, vector<int> (m+2, 0));

	set<pair<int, int>> st;

	for(int i = 0; (int)st.size() < c; i++){
		int x = rnd() % n + 1;
		int y = rnd() % m + 1;
		if(st.count({x, y}) != 0) continue;
		st.insert({x, y});
		table[x][y] = -1;
	}

	print(visiualtable);

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

	//int a, b;
	//KEYBOARD::getkey();
	int lastx = 0, lasty = 0;

	bfs(findfirstzero());
	print(visiualtable);

	printcur(1,1, {BLINKING, BLACK, BG_WHITE});
	// IMAGE::printc(1, 1, visiualtable[1][1], {BLINKING, WHITE, BG_BLACK});
	while(1){
		if(ifwin()){
			system("clear");
			IMAGE::prints("genius\n", {BOLD, RED, NONE});
			//auto ch = KEYBOARD::getkey();
			exit(0);
		}
		lastx = x;
		lasty = y;
		int bl = movement();
		if(bl == 2){
			if(visiualtable[x][y] == -3){
				if(used[x][y]){
					IMAGE::printc(x, y, to_string(table[x][y]), {BOLD, BLACK, BG_WHITE});
					visiualtable[x][y] = table[x][y];
				}else{
					IMAGE::printc(x, y, ".", {BOLD, BLACK, BG_WHITE});
					visiualtable[x][y] = -1;
				}
				continue;
			}
			IMAGE::printc(x, y, "#", {BOLD, BLACK, BG_WHITE});
			visiualtable[x][y] = -3;
			continue;
		}
		printcur(lastx, lasty, {NONE, BLACK, BG_WHITE});
		printcur(x, y, {BLINKING, BLACK, BG_WHITE});
		if(!bl) continue;
		int a = x;
		int b = y;
		if(table[a][b] == -1){
			system("clear");
			IMAGE::prints("LOSER\n", {BOLD, RED, NONE});
			printlose();
			exit(0);
		}
		bfs({a, b});
		print(visiualtable);
		printcur(x, y, {BLINKING, BLACK, BG_WHITE});
	}
}

int main(){
	IMAGE::prints("PLEASE ENTER N, M, \% OF MINES IN THE FIELD\n", {BOLD, RED, NONE});
	cin >> n >> m >> per;
	c = n * m * per / 100;
	system("clear");
	KEYBOARD::getkey();
	IMAGE::prints("i or ^ - to move up\nj or <- - to move left\nk or v - to move down\nl or -> - to move Right\nEnter - to dig\nr - to put a flag", {BOLD, RED, NONE});
	KEYBOARD::getkey();
	system("clear");
	game();
}
