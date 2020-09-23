//traverse white from bottom

#include <bits/stdc++.h>
using namespace std;
int n_row = 15;
int n_col = 15;
float alpha = -1*FLT_MAX, beta = FLT_MAX;
float run_time;
string type, color;
vector <pair <int,int> > w_side_goals;
vector <pair <int,int> > b_side_goals;
vector <pair <int,int> > other_moves;
typedef pair<int,int> p;
map<p, p> parent;

int cou = 0;
ofstream myfile("output.txt");
ofstream mylog("log.txt",  std::ios_base::app);
int count_player_in_home = 0, count_player_in_opponent = 0, count_opponent_in_home = 0;




class Tile{
public:
	int row, col, tile, piece;
	pair<int,int> loc;

	Tile(){

	}

	Tile(int tile, int piece, int row, int col){
		this->tile = tile;
		this->piece = piece;
		this->row = row;
		this->col = col;
		loc = make_pair(row, col);
		//cout << loc.first << " " << loc.second << endl;
	}
};
struct ans{
	float best_val;
	pair<p,p> best_move;
	map<p, p> best_parent;
};
struct all_moves{
	Tile from;
	list<Tile> to;
	map<p,p> parent;
};
// All other functions:

bool new_find(map<p,p> parent, Tile t2){
	//cout << moves.size() <<endl;
	for(auto i = parent.begin(); i != parent.end(); i++){
		if(t2.loc == i->second)
			return true;
	}
	return false;
}

float eucd_distance(p p1, p p2){
    return sqrt(pow((p1.first - p2.first),2) + pow((p1.second - p2.second),2));

}
//check for terminal state
// bool home_full(int player, Tile **board){
// 	if(player == 1){
// 		for(int i = 0; i < w_side_goals.size(); i++){
// 			int r = w_side_goals[i].first;
// 			int c = w_side_goals[i].second;
// 			if(board[r][c].piece != player)
// 				return false;
// 		}
// 	}
// 	else{
// 		for(int i = 0; i < b_side_goals.size(); i++){
// 			int r = b_side_goals[i].first;
// 			int c = b_side_goals[i].second;
// 			if(board[r][c].piece != player)
// 				return false;
// 		}
// 	}
// 	return true;
// }


list<Tile> get_moves_at_tile(Tile curr_tile, int player, Tile **board, list<Tile> moves, int out_camp, bool adj = true){
	int r = curr_tile.loc.first;
	int c = curr_tile.loc.second;
	int new_row, new_col;
	Tile new_tile;

	vector<int>valid_tiles;
	valid_tiles.push_back(0);
	valid_tiles.push_back(1); 
	valid_tiles.push_back(2);

	if(curr_tile.tile != player){
		remove(valid_tiles.begin(), valid_tiles.end(), player), valid_tiles.end();
	}
	
	//cout << "tile: "<<curr_tile.tile << "player: " << player <<" loc: "<< curr_tile.loc.first << " " << curr_tile.loc.second << endl;
	if(curr_tile.tile != 0 && curr_tile.tile != player){
		remove(valid_tiles.begin(), valid_tiles.end(), 0), valid_tiles.end();
	}
		vector <pair<int,int> > order;
	// if(player == 2){
	// 	order.push_back(make_pair(-1,-1));order.push_back(make_pair(-1,0)); order.push_back(make_pair(0,-1));order.push_back(make_pair(1,-1));order.push_back(make_pair(-1,1));order.push_back(make_pair(0,1));order.push_back(make_pair(1,0));order.push_back(make_pair(1,1));
	// }
	// else if(player == 1){
	// 	order.push_back(make_pair(1,1));order.push_back(make_pair(0,1)); order.push_back(make_pair(1,0));order.push_back(make_pair(1,-1));order.push_back(make_pair(0,-1));order.push_back(make_pair(-1,1));order.push_back(make_pair(-1,0));order.push_back(make_pair(-1,-1));
	// }
	//for(int k = 0; k <= 7; k++){
	for(int i = -1; i < 2; i++){
		for(int j = -1; j < 2; j++){
		//cout << j << " " << i <<endl;
			new_row = r + j;
			new_col = c + i;

			if((new_row == r && new_col == c) || new_row < 0 || new_col < 0 || new_row > 15 || new_col > 15)
				continue;
			new_tile = board[new_row][new_col];
			if(find(valid_tiles.begin(), valid_tiles.end(), new_tile.tile) == valid_tiles.end())
				continue;
			if(new_tile.piece == 0){
				if(adj){
					//cout << " " << curr_tile.loc.first << curr_tile.loc.second << " " << new_tile.loc.first << new_tile.loc.second <<endl;
					//inside to inside
					if(new_tile.tile == player && out_camp == 1){
						continue;
					}
					if(new_tile.tile == player){
						if(player == 1){
							if((new_tile.loc.first - curr_tile.loc.first < 0) || (new_tile.loc.second - curr_tile.loc.second < 0))
								continue;
						}
						else if(player == 2){
							if((curr_tile.loc.first - new_tile.loc.first < 0) || (curr_tile.loc.second - new_tile.loc.second < 0))
								continue;
						}
					}  
					moves.push_back(new_tile);
				}
				//in jump dont consider dot-dots
				continue;
			}
			new_row = new_row + j;
			new_col = new_col + i;
			if(new_row < 0 || new_col < 0 || new_row > 15 || new_col > 15)
				continue;
			new_tile = board[new_row][new_col];
			//cout << " " << curr_tile.loc.first << curr_tile.loc.second << " " << new_tile.loc.first << new_tile.loc.second <<endl;

			if((parent.find(new_tile.loc) != parent.end())||find(valid_tiles.begin(), valid_tiles.end(), new_tile.tile) == valid_tiles.end()){
				continue;
			}
			if(new_tile.piece == 0){
				if(new_tile.tile == player){
						if(player == 1){
							if((new_tile.loc.first - curr_tile.loc.first < 0) || (new_tile.loc.second - curr_tile.loc.second < 0)){
								parent[new_tile.loc] = curr_tile.loc;
								moves = get_moves_at_tile(new_tile, player, board, moves, 1, false);
							}
							else{
								moves.insert(moves.begin(), new_tile);
								parent[new_tile.loc] = curr_tile.loc;
								moves = get_moves_at_tile(new_tile, player, board, moves, 1, false);
							}

						}
						else if(player == 2){
							if((curr_tile.loc.first - new_tile.loc.first < 0) || (curr_tile.loc.second - new_tile.loc.second < 0)){
								parent[new_tile.loc] = curr_tile.loc;
								moves = get_moves_at_tile(new_tile, player, board, moves, 1, false);
							}
							else{
								moves.insert(moves.begin(), new_tile);
								parent[new_tile.loc] = curr_tile.loc;
								moves = get_moves_at_tile(new_tile, player, board, moves, 1, false);
							}
						}
				} 
				else{
				moves.insert(moves.begin(), new_tile);
				parent[new_tile.loc] = curr_tile.loc;

				//cout << "new to tile for tile J: " << curr_tile.loc.first << curr_tile.loc.second << " " << new_tile.loc.first << new_tile.loc.second <<endl;  
				moves = get_moves_at_tile(new_tile, player, board, moves, 1, false);
					}
			}
		}
	}
return moves;
}


float maximum_dist(vector<float> dist){
	float m = -1*FLT_MAX;
	for(int i = 0; i < dist.size(); i++){
		m = max(m, dist[i]);
	}
	return m;
}

float eval_eucd(int player, Tile **board){
	float value = 0;
	if(player == 1){
		for (int i = 0 ; i < 16; i++){
        	for(int j = 0; j < 16; j++){
        		Tile tile = board[j][i];
        		if(tile.piece == 1){
        			value += eucd_distance(tile.loc, make_pair(15,15));
        		}
        	}
        }
 
        return value*-1.0;
	}
	else if(player == 2){
		for (int i = 0 ; i < 16; i++){
        	for(int j = 0; j < 16; j++){
        		Tile tile = board[j][i];
        		if(tile.piece == 2){
        			value += eucd_distance(tile.loc, make_pair(0,0));
        		}
        	}
        }
        return -1.0*value;
	}
}

// float eval_eucd(int player, Tile **board){
//         float value = 0;
//         for (int i = 0 ; i < 16; i++){
//         	for(int j = 0; j < 16; j++){
//         		Tile tile = board[j][i];
//         		//cout << tile.loc.first << " " <<tile.loc.second << " "<< tile.piece << endl;
        		
//         		if(tile.piece == 1) {
//         		//	if(!(home_full(player, board))){
//         			//value += (abs(j-15) + (abs(i-15)));
//         			value += eucd_distance(tile.loc, make_pair(15,15));
//         		}
//         		//else{
//         		//	value += -50;
//         		//}
//         		//}
//         		//cout << value <<endl;
//         		else if(tile.piece == 2){
//         		//	if(!(home_full(player, board))){
//         			//value += (abs(j-15) + (abs(i-15)));
//         			value -= eucd_distance(tile.loc, make_pair(0,0));
//         		//}
//         		//else{
//         		//	value -= -50;
//         		//}
//         		}
//         	}	
// 		}
// 		// if(maxim)
//   //       	value *= -1;
//         //cout << value << endl;
//     if(player == 1)
// 		return -1.0*value;
// 	else
// 		return value;
// }
	

float eval_misplaced(int player, Tile **board){
	        float value = 0;
        for (int i = 0 ; i < 16; i++){
        	for(int j = 0; j < 16; j++){
        		Tile tile = board[j][i];
        		if(tile.piece == 1){
        			vector<float> distances;
        			for(int k = 0; k < w_side_goals.size(); k++){
        				if(board[w_side_goals[k].first][w_side_goals[k].second].piece != 1){
        					distances.push_back(eucd_distance(tile.loc, w_side_goals[k]));
        				}
        		}
        		if(distances.size() > 0)
        			value += maximum_dist(distances);
        		else
        			value += -50; 
        		}
        		if(tile.piece == 2){
        			//cout << "green: " << board[j][i].loc.first << ", " << board[j][i].loc.second <<endl;
        			vector<float> distances;
        			for(int k = 0; k < b_side_goals.size(); k++){
        				if(board[b_side_goals[k].first][b_side_goals[k].second].piece != 2){
        					distances.push_back(eucd_distance(tile.loc, b_side_goals[k]));
        				}
        			}
        			//cout << maximum_dist(distances) <<endl;
        			if(distances.size() > 0)
        				value -= maximum_dist(distances);
        			else
        				value -= -50; 
        			//cout << value << endl;
        		}
        	}	
		}
		if(player == 1)
        	value *= -1;
        //cout << value << endl;
    
		return value;
}



list <all_moves> get_next_moves(int player, Tile **board, int count){
	list <all_moves> moves;
	struct all_moves move;
	list<Tile> m;
	int out_camp = 1;
	int f = 0, out_move = 0;
	
	if(count > 0){
	if(player == 1){
		for(int i = 0; i < b_side_goals.size(); i++){
			Tile curr_tile = board[b_side_goals[i].first][b_side_goals[i].second];
			if(curr_tile.piece != player)
				continue;
			parent.clear();
			move.from = curr_tile;
			move.to = get_moves_at_tile(curr_tile, player, board, m, 1);
			if(move.to.size() > 0){
				out_move = 1;
				move.parent = parent;
				moves.push_back(move);
			}
		}
		if(out_move == 1){
			return moves;
		}
		else{
			out_camp = 0;
			for(int i = 0; i < b_side_goals.size(); i++){
			Tile curr_tile = board[b_side_goals[i].first][b_side_goals[i].second];
			if(curr_tile.piece != player)
				continue;
			parent.clear();
			move.from = curr_tile;
			move.to = get_moves_at_tile(curr_tile, player, board, m, 0);
			if(move.to.size() >= 1)
				f = 1;
			move.parent = parent;
			moves.push_back(move);
			}
		}
	}

	if(player == 2){
		for(int i = 0; i < w_side_goals.size(); i++){
			Tile curr_tile = board[w_side_goals[i].first][w_side_goals[i].second];
			if(curr_tile.piece != player)
				continue;
			parent.clear();
			move.from = curr_tile;
			//cout << "from:"<<move.from.loc.first << move.from.loc.second <<endl; 
			move.to = get_moves_at_tile(curr_tile, player, board, m, 1);
		
			if(move.to.size() > 0){
				out_move = 1;
				move.parent = parent;
				moves.push_back(move);
			}
		}
		if(out_move == 1){
			return moves;
		}
			//cout << move.to.size() <<endl;
			else{
				out_camp = 0;
				for(int i = 0; i < w_side_goals.size(); i++){
				Tile curr_tile = board[w_side_goals[i].first][w_side_goals[i].second];
				if(curr_tile.piece != player)
					continue;
				parent.clear();
				move.from = curr_tile;
				move.to = get_moves_at_tile(curr_tile, player, board, m, 0);
				if(move.to.size() >= 1){
					f = 1;
					move.parent = parent;
					moves.push_back(move);
				}
				}
		}
	}
}

	if(f == 1){
		return moves;
	}
	else{
			if(player == 1){
			for(int i = 0; i <= 15; i++){
				for(int j = 0; j <= 15; j++){
					Tile curr_tile = board[j][i];
					if(curr_tile.tile == player || curr_tile.piece != player)
			 			continue;
			 		parent.clear();
			 		parent[curr_tile.loc] = (make_pair(-1,-1));
			 		move.from = curr_tile;
			 		move.to = get_moves_at_tile(curr_tile, player, board, m, 2);
			 		move.parent = parent;
					moves.push_back(move);
				}
			}
		}
		else if(player == 2){
			for(int i = 15; i >= 0; i--){
				for(int j = 15; j >= 0; j--){
					Tile curr_tile = board[j][i];
					if(curr_tile.tile == player || curr_tile.piece != player)
			 			continue;
			 		parent.clear();
			 		parent[curr_tile.loc] = (make_pair(-1,-1));
			 		move.from = curr_tile;
			 		move.to = get_moves_at_tile(curr_tile, player, board, m, 2);
			 		move.parent = parent;
					moves.push_back(move);
				}
			}
		}
	}
	return moves;
}

// int isWinner(Tile **board){

// 	int flag = 0;
// 	for(int i = 0; i < b_side_goals.size(); i++){
// 		int r = b_side_goals[i].first;
// 		int c = b_side_goals[i].second;
// 		if(board[r][c].piece != 2)
// 			flag = 1;
// 	}
// 	if(flag == 0)
// 		return 1;
// 	flag = 0;
// 	for(int i = 0; i < w_side_goals.size(); i++){
// 		int r = w_side_goals[i].first;
// 		int c = w_side_goals[i].second;
// 		if(board[r][c].piece != 1)
// 			flag = 1;
// 	}
// 	if(flag == 0)
// 		return 1;
// 	else
// 		return 0;
// }
int isWinner(Tile **board){
	int flag = 1, flag_a_piece = 0;
	for(int i = 0; i < b_side_goals.size(); i++){
		int r = b_side_goals[i].first;
		int c = b_side_goals[i].second;
		if(board[r][c].piece == 2)
			flag_a_piece = 1;
		else if(board[r][c].piece != 2 || board[r][c].piece != 1)
			flag = 0;
	}
	if(flag && flag_a_piece)
	return 1;

	flag = 1, flag_a_piece = 0;
	for(int i = 0; i < w_side_goals.size(); i++){
		int r = w_side_goals[i].first;
		int c = w_side_goals[i].second;
		if(board[r][c].piece == 1)
			flag_a_piece = 1;
		else if(board[r][c].piece != 2 || board[r][c].piece != 1)
			flag = 0;
	}
	if(flag && flag_a_piece)
		return 1;
	return 0;
}


struct ans maximize(Tile **board, float alpha, float beta, int player, bool maxim, int depth, int eval_function_flag){
	list <all_moves> moves;
	float best_val;
	struct ans s;
	pair<p,p> best_move;
	int enemy = (player == 1)?2:1;
	int count;
	//cout << isWinner(board) << endl;

	if(depth == 0 || isWinner(board)){
		// if(eval_function_flag == 3)
		// 	s.best_val = eval_centroid(player, board);
		if(eval_function_flag == 1)
			s.best_val = eval_eucd(player, board);
		else 
			s.best_val = eval_misplaced(player, board);
		//cout << s.best_val <<endl;
		return s;
	}
	int max_size = INT_MIN;
	if(maxim){

		best_val = -1.0*FLT_MAX;
		count = count_player_in_home;
		moves = get_next_moves(player, board, count);
		//cout << moves.front().to.size();
		// for(auto it = moves.begin(); it != moves.end(); it++){
		// 	max_size += it->to.size();
		// }
		// if(max_size > 200 )
		// 	depth = 1;
		// for(auto i = it->to.begin(); i != it->to.end(); i++){
		// 	cout << "from : " << it->from.loc.first << ", " << it->from.loc.second << " to: " << (*i).loc.first << ", " <<(*i).loc.second<<endl;
		// cou++;		
		// }}
		// cout << cou << endl;
		//  cou = 0;
		// s.best_val = best_val;
		
	}
	else{
		best_val = FLT_MAX;
		enemy = (player == 1)?2:1;
		count = count_opponent_in_home;
		moves = get_next_moves(enemy, board, count);
		//s.best_val = best_val;
	}

	for(auto it = moves.begin(); it != moves.end(); it++){
		for(auto i = it->to.begin(); i != it->to.end(); i++){
			//cout << "from: "<<it->from.loc.first << " " <<it->from.loc.second << " to: " <<(*i).loc.first << " " <<(*i).loc.second<<endl;
			Tile curr_move_from = it->from;
			Tile curr_move_to = *i;
			//cout << "from : " << curr_move_from.loc.first << ", " << curr_move_from.loc.second << " to: " << curr_move_to.loc.first << ", " <<curr_move_to.loc.second<<endl;
			//cout << s.best_val << " " << s.best_move.first.first << ", " << s.best_move.first.second << " " << s.best_move.second.first << ", " << s.best_move.second.second << endl;

			int piece = curr_move_from.piece;
			board[curr_move_from.loc.first][curr_move_from.loc.second].piece = 0;
			board[curr_move_to.loc.first][curr_move_to.loc.second].piece = piece;

			int flagmax = 0, flagmin = 0;
			if(maxim && curr_move_from.tile == player && curr_move_to.tile == 0){
				count_player_in_home--;
				flagmax = 1;
			}
			if(!maxim && curr_move_from.tile == enemy && curr_move_to.tile == 0){
				count_opponent_in_home--;
				flagmin = 1;
			}

			float val = maximize(board, alpha, beta, player, !maxim, depth - 1, eval_function_flag).best_val;


			if(flagmax == 1){
				count_player_in_home++;
			}
			else if(flagmin){
				count_opponent_in_home++;
			}

			//cout << "for d: " << depth << " "<< val <<" "<< best_val <<endl;
			if(maxim && (val > best_val)){
				best_val = val;
				s.best_val = best_val;
				s.best_move = make_pair(curr_move_from.loc, curr_move_to.loc);
				s.best_parent = it->parent; 
				alpha = max(alpha, val);
			}
			if(!(maxim) && (val < best_val)){
				best_val = val;
				s.best_val = best_val;
				beta = min(beta, val);
			}
			board[curr_move_from.loc.first][curr_move_from.loc.second].piece = piece;
			board[curr_move_to.loc.first][curr_move_to.loc.second].piece = 0;

			if(beta <= alpha){
				//cout << "prunes="<<cou++<<endl;
				return s;
			}
			//cout  << "from : " << best_move.first.first << ", " << best_move.first.second << " to: " << best_move.second.first << ", " <<best_move.second.second<<endl;
			//cout << s.best_val <<endl;
		}
		//cerr << "Time for 1 from elapsed: " << (1.0 * clock()-start)/ CLOCKS_PER_SEC<< "s\n";

	}
	return s;
}

void display_board(Tile **board){
	for(int i = 0; i < 16; i++){
		for(int j = 0; j < 16; j++){
			cout << board[j][i].piece << " ";
		}
		cout << endl;
	}
}

void display_move(p p2, map<p, p>& parent){
	if(parent[p2] == make_pair(-1,-1)){
		return;
	}
	display_move(parent[p2], parent);
	myfile << "J " << parent[p2].first << "," << parent[p2].second << " " << p2.first << "," << p2.second << endl;
	cout << "J " << parent[p2].first << "," << parent[p2].second << " " << p2.first << "," << p2.second << endl;
}
int main()
{
	char c;
	ifstream fin("input.txt");
	fin >> type >> color >> run_time;
	//fin.get();

	int player;
	if(!(color.compare("BLACK")))
		player = 1;
	else 
		player = 2;
	
	Tile **board = new Tile*[16];
	for (int i = 0; i <= 15; ++i)
 		board[i] = new Tile[16];

     int ip[16][16] = { { 1, 1, 1, 1, 1, 0,0,0,0,0,0,0,0,0,0,0 }, { 1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0 }, { 1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0 }, {1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0 },{1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0, 2,2}, {0,0,0,0,0,0,0,0,0,0,0,0,0, 2,2,2 }, { 0,0,0,0,0,0,0,0,0,0,0,0, 2,2,2,2}, { 0,0,0,0,0,0,0,0,0,0,0, 2, 2, 2, 2, 2 }, { 0,0,0,0,0,0,0,0,0,0,0, 2,2,2,2,2 }}; 

	for(int i = 0; i < 16; i++){
		for(int j = 0; j < 16; j++){
			Tile element;
			if(ip[j][i] == 1){
				element = Tile(1, 0, j, i);
			}
			else if(ip[j][i] == 2){
				element = Tile(2, 0, j, i);
			}
			else
				element = Tile(0, 0, j, i);
			board[j][i] = element;
		}
	}
	
	int enemy = (player == 1)?2:1;
	for (int i = 0; i <= n_row; i++) {
		for (int j = 0; j <= n_col; j++) {
			fin >> c;
			Tile element;
			if(c == 'B'){
				board[j][i].piece = 1;
				if(player == 1 && ip[j][i] == 2)
					count_player_in_opponent++;
				if(player == 1 && ip[j][i] == 1)
					count_player_in_home++;
				if(enemy == 1 && ip[j][i] == 1)
					count_opponent_in_home++;
			}
			else if(c == 'W'){
				board[j][i].piece = 2;
				if(player == 2 && ip[j][i] == 1)
					count_player_in_opponent++;
				if(player == 2 && ip[j][i] == 2)
					count_player_in_home++;
				if(enemy == 2 && ip[j][i] == 2)
					count_opponent_in_home++;
			}
			else
				board[j][i].piece = 0;
		}
			//cout << element.tile << " " << element.row << " " << element.col << endl;
			//board[j][i] = element;
		//fin.get();
	}
	fin.close();
	int eval_function_flag, depth;
	
	// cout << count_player_in_home << endl;
	// cout << count_player_in_opponent << endl;

	//EVAL 1: EUCD.. EVAL 2: MISPLACED.. EVAL 3: CENTROID 
	if(count_player_in_home > 0){
		eval_function_flag = 1;
	}
	else if(count_player_in_opponent >= 16){
		eval_function_flag = 2;
	}
	else{
		eval_function_flag = 1;
	}
	

	if(run_time < 2){
		depth = 1;
	}
	else{
		depth = 3;
	}

cout << depth <<endl;
	for(int j = 0; j <= 4; j++){
		b_side_goals.push_back(make_pair(j,0));
	}
	for(int j = 0; j <= 4; j++){
		b_side_goals.push_back(make_pair(j,1));
	}
	for(int j = 0; j <= 3; j++){
		b_side_goals.push_back(make_pair(j,2));
	}
	for(int j = 0; j <= 2; j++){
		b_side_goals.push_back(make_pair(j,3));
	}
	for(int j = 0; j <= 1; j++){
		b_side_goals.push_back(make_pair(j,4));
	}

	for(int j = 14; j <= 15; j++){
		w_side_goals.push_back(make_pair(j,11));
	}
	for(int j = 13; j <= 15; j++){
		w_side_goals.push_back(make_pair(j,12));
	}
	for(int j = 12; j <= 15; j++){
		w_side_goals.push_back(make_pair(j,13));
	}
	for(int j = 11; j <= 15; j++){
		w_side_goals.push_back(make_pair(j,14));
	}
	for(int j = 11; j <= 15; j++){
		w_side_goals.push_back(make_pair(j,15));
	}


	
	struct ans action = maximize(board, alpha, beta, player, true, depth, eval_function_flag);
	if((abs(action.best_move.first.first - action.best_move.second.first) < 2) && (abs(action.best_move.first.second - action.best_move.second.second) < 2)){
		myfile << "E " << action.best_move.first.first << "," << action.best_move.first.second << " " << action.best_move.second.first << "," << action.best_move.second.second <<endl; 
		cout << "E " << action.best_move.first.first << "," << action.best_move.first.second << " " << action.best_move.second.first << "," << action.best_move.second.second <<endl; 

	}
	else{
	action.best_parent[action.best_move.first] = make_pair(-1,-1);
	display_move(action.best_move.second, action.best_parent);
	}

	myfile.close();

mylog << "Time elapsed: " << 1.0 * clock() / CLOCKS_PER_SEC << "s\n";
	return 0;
}