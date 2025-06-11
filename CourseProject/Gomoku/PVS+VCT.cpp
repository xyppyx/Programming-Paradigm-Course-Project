/**********************************************
# Project Name : Gomoku
# File Name    : Gomoku_PVS.cpp
# File Function: 实现基于PVS算法的五子棋ai
# Author       : YunPeng Xu(徐云鹏)
# Nickname     : 奕衍盯阵，鉴定为奕？悟！
# Update Date  : 2024/11/5
**********************************************/

/*调试开关*/
#define DEBUG_MODE      0                   // 调试模式，显示棋盘，时间等信息
#define AUTO_MODE       0                   // 自动模式，仅需输入玩家颜色，ai自动调用GetGreedyMove落子
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<random>
#include<chrono>
#include<cstring>
#include<algorithm>
#include<unordered_map>  

typedef unsigned long long ull;

/**********************************************************全局常量************************************************************************/
/*限制*/
const int kBoardSize       = 12;            // 棋盘大小
const int kCompensate      = 4;             // 棋盘坐标补偿值（内部坐标-输出坐标=4）
const int kBoardBegin      = 4;             // 棋盘起始位置
const int kBoardEnd        = 15;            // 棋盘结束位置
const int kMinDepth        = 3;             // 最小搜索深度
const int kMaxABDepth      = 9;             // 最大AB搜索深度
const int kMaxMoves        = 15;            // 每层最多候选落子数
const int kTimeLimit       = 1500;          // 限制时间(ms)
/*棋盘类型*/   
const int BLACK            = 0;             // 黑棋
const int WHITE            = 1;             // 白棋
const int EMPTY            = 2;             // 空位
const int OUT              = 3;             // 无效坐标
/*哈希表类型*/
const int HASH_EXACT       = 0;             // 精确值
const int HASH_ALPHA       = 1;             // Alpha值
const int HASH_BETA        = 2;             // Beta值
/*搜索类型*/
const int SEARCH_AB        = 0;             // Alpha-Beta搜索
const int SEARCH_VCF       = 1;             // VCF搜索
const int SEARCH_VCT       = 2;             // VCT搜索
/*棋型*/
const int T_FIVE_LINE      = 8;				// 成五
const int T_LIVE_FOUR      = 7;				// 活四
const int T_BLOCK_FOUR     = 6;				// 冲四
const int T_COMBINE        = 5;				// 组合（活三+活三，活三+冲四）
const int T_LIVE_THREE     = 4;			    // 活三
const int T_BLOCK_THREE    = 3;			    // 眠三
const int T_LIVE_TWO       = 2;				// 活二
const int T_BLOCK_TWO      = 1;				// 眠二
const int T_ONE            = 0;				// 单点
/*分数*/
const double S_FIVE_LINE   = 10000000;      // 五连分数
const double S_LIVE_FOUR   = 20000;         // 活四分数
const double S_COMBINE     = 10000;         // 组合分数（活三+活三，活三+冲四）
const double S_BLOCK_FOUR  = 1500;          // 冲四分数
const double S_LIVE_THREE  = 1500;          // 活三分数
const double S_BLOCK_THREE = 100;           // 冲三分数
const double S_LIVE_TWO    = 100;           // 活二分数
const double S_BLOCK_TWO   = 10;            // 冲二分数
const double S_ONE         = 1;             // 单点分数
const double g_value[9]    = {S_ONE, S_BLOCK_TWO, S_LIVE_TWO, S_BLOCK_THREE, S_LIVE_THREE, S_COMBINE, S_BLOCK_FOUR, S_LIVE_FOUR, S_FIVE_LINE};
const double ATK_RATIO     = 1;             // 进攻系数
const double DEF_RATIO     = 1;           // 防守系数
/*方向*/
const int Delta_X[4] = {1, 0, 1, 1};        // X 方向偏移量(四个方向：竖直，水平，左斜，右斜)
const int Delta_Y[4] = {0, 1, 1, -1};       // Y 方向偏移量
/*****************************************************************************************************************************************/



/**********************************************************全局数据************************************************************************/
//棋盘
int g_chess_count = 0;                                          // 棋子数
int g_player_count = 0;                                         // 我方棋子数
int g_enemy_count = 0;                                          // 敌方棋子数
struct Node { int type, num_in_2, pattern[2][4]; };             // 棋盘结构体(某点的棋子类型，两格内棋子数，四个方向上黑白棋能形成的棋型)
Node g_board[kBoardSize + 8][kBoardSize + 8];                   // 棋盘(向外拓展4格减少特判，即左上角为(4,4)，右下角为(15,15))
//Zobrist哈希表    
ull g_zobrist_board[kBoardSize + 8][kBoardSize + 8][2];         // Zobrist哈希表
ull g_cur_zobrist_key = 0;                                      // 当前局面哈希值
struct HashEntry { int depth, type; double score; };            // 哈希表结构体(深度，哈希类型，分数)
std::unordered_map<ull, HashEntry> g_zobrist_hashtable;         // zobrist置换表(当前局面哈希值 -> 深度和分数)
//棋型表   
int g_pattern_table[65536][2];                                  // 棋型表(65536=2^16，2位代表一个棋子的状态，储存一条某个点两侧长为4，总长为8的棋型的分数，用于快速更改Node.pattern)
double g_pattern_to_score[9][9][9][9];                          // 棋型分数表(表示棋盘上某一点的四个方向上的棋型分别为i,j,k,l时,该点的分数)
//ai
int g_player_color;                                             // 我方颜色
int g_enemy_color;                                              // 敌方颜色
bool g_is_aggressive;                                           // 是否先手侧重进攻，否则防守逼平
auto g_start_time = std::chrono::high_resolution_clock::now();  // 每一轮turn的开始时间 
bool g_is_timeout = false;                                      // alphabeta是否超时
int g_last_row = 7 + kCompensate, g_last_col = 4 + kCompensate; // 上一步落子位置，用于自动落子
struct Position                                                 // 落子位置与临时分数，用于GetOptionalMove启发式排序
{   
    int row, col; 
    double temp_score = 0;
    Position() : row(0), col(0), temp_score(0) {}
    Position(int r, int c) : row(r), col(c) {}
    Position(int r, int c, double s) : row(r), col(c), temp_score(s) {}
    bool operator == (const Position &pos) const { return row == pos.row && col == pos.col; }
};
/*****************************************************************************************************************************************/



/**********************************************************全局函数************************************************************************/
inline bool CheckBound(int row, int col);                                   // 检查是否越界
inline int GetKeyOfLine(int row, int col, int dir);                         // 获取棋盘上某点dir方向上的长度为9的棋型的哈希值，用于获取棋型表中预处理的棋型
inline int GetNodeScore(int row, int col, int color);                       // 获取某点的分数
double GetBoardScore(int player_color);                                     // 获取当前局面分数
void UpdateBZ(int row, int col, int color, int pre_color);                  // 更新棋盘与哈希值
void InitZobrist();                                                         // 初始化Zobrist哈希表
void InitChessType();                                                       // 初始化棋型表
void Initialize();                                                          // 初始化（四个预置棋子，哈希表，棋型表）
void EnemyMove(int row, int col);                                           // 敌方走棋
void GetGreedyMove(int &row, int &col);                                     // AUTO_MODE下贪心搜索
void GetOptionalMove(std::vector<Position> &positions, int player_color);   // 启发式获取候选落子点
double AlphaBeta(int depth, double alpha, double beta, int player_color);   // alpha-beta剪枝搜索
void GetBestMove(int player_color, Position &best_move);                    // 获取最佳走法（ai算法调用入口）
void PrintBoard();                                                          // 打印棋盘
/*****************************************************************************************************************************************/


#if DEBUG_MODE//调试版本
int main()
{
    char cmd[20] = {'S', 'T', 'A', 'R', 'T'};
    Position best_move;
    while(true)
    {
        if(g_chess_count == kBoardSize * kBoardSize)
            break;
#if !AUTO_MODE
        scanf("%s", cmd);
#endif
        if(strcmp(cmd, "START") == 0)
        {  
            scanf("%d", &g_player_color);
            g_player_color -= 1;//0-黑，1-白
            g_enemy_color = g_player_color ^ 1;
            g_is_aggressive = (g_player_color == BLACK ? true : false);
            Initialize();
            printf("OK\n");
            fflush(stdout);
            PrintBoard();
#if AUTO_MODE
            if(g_player_color == BLACK)
                strcpy(cmd, "t");
            else
                strcpy(cmd, "p");
#endif
        }
        else if(strcmp(cmd, "p") == 0)
        {
            int enemy_row, enemy_col;
#if AUTO_MODE
            GetGreedyMove(enemy_row, enemy_col);
            strcpy(cmd, "t");
#else
            scanf("%d %d", &enemy_row, &enemy_col);
            enemy_row += kCompensate;
            enemy_col += kCompensate;
#endif
            EnemyMove(enemy_row, enemy_col);
            g_last_row = enemy_row;
            g_last_col = enemy_col;
            PrintBoard();
        }
        else if(strcmp(cmd, "t") == 0)
        {
            g_start_time = std::chrono::high_resolution_clock::now();
            GetBestMove(g_player_color, best_move);
            printf("%d %d\n", best_move.row - kCompensate, best_move.col - kCompensate);
            fflush(stdout);
            printf("DEBUG [Time = %ldms]\n", std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - g_start_time).count());
            fflush(stdout);
            g_last_row = best_move.row;
            g_last_col = best_move.col;
            PrintBoard();
#if AUTO_MODE
            strcpy(cmd, "p");
#endif
        }
        else if(strcmp(cmd, "pb") == 0)
        {
            int row, col;
            scanf("%d %d", &row, &col);
            UpdateBZ(row + kCompensate, col + kCompensate, BLACK, -1);
            PrintBoard();
        }
        else if(strcmp(cmd, "pw") == 0)
        {
            int row, col;
            scanf("%d %d", &row, &col);
            UpdateBZ(row + kCompensate, col + kCompensate, WHITE, -1);
            PrintBoard();
        }
        else if(strcmp(cmd, "pe") == 0)
        {
            int row, col;
            scanf("%d %d", &row, &col);
            UpdateBZ(row + kCompensate, col + kCompensate, EMPTY, g_board[row + kCompensate][col + kCompensate].type);
            PrintBoard();
        }
    }
    while(true);
    return 0;
}    
#else//提交版本
int main()
{
    char cmd[20];
    Position best_move;
    while(true)
    {
        scanf("%s", cmd);
        if(strcmp(cmd, "START") == 0)
        {  
            scanf("%d", &g_player_color);
            g_player_color -= 1;//0-黑，1-白
            g_enemy_color = g_player_color ^ 1;
            g_is_aggressive = (g_player_color == BLACK ? true : false);
            Initialize();
            printf("OK\n");
            fflush(stdout);
        }
        else if(strcmp(cmd, "PLACE") == 0)
        {
            int enemy_row, enemy_col;
            scanf("%d %d", &enemy_row, &enemy_col);
            EnemyMove(enemy_row + kCompensate, enemy_col + kCompensate);
        }
        else if(strcmp(cmd, "TURN") == 0)
        {
            g_start_time = std::chrono::high_resolution_clock::now();
            GetBestMove(g_player_color, best_move);
            printf("%d %d\n", best_move.row - kCompensate, best_move.col - kCompensate);
            fflush(stdout);
            printf("DEBUG TIME = %lld\n", std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - g_start_time).count());
            fflush(stdout);
        }
        else if(strcmp(cmd, "END") == 0)
        {
            int winner;
            scanf("%d", &winner);
            break;
        }
    }
    return 0;
}
#endif

/*
* Function Name   : CheckBound
* Function        : 检查输入的行列是否在棋盘范围内
* Input Parameters: int - 行
*                   int - 列                   
* Return Value    : bool - 如果在棋盘范围(4,4)~(15,15)，返回true
*/
inline bool CheckBound(int row, int col)
{
    return row >= kBoardBegin && row <= kBoardEnd && col >= kBoardBegin && col <= kBoardEnd;
}

/*
* Function Name   : GetKeyOfLine
* Function        : 获取棋盘上某点dir方向上的长度为9的棋型的哈希值，用于获取棋型表中预处理的棋型
* Input Parameters: int - 行
*                   int - 列
*                   int - 方向
* Return Value    : int - 某点dir方向上的长度为9的棋型的哈希值
*/
inline int GetKeyOfLine(int row, int col, int dir)
{
    return ((g_board[row - Delta_X[dir] * 4][col - Delta_Y[dir] * 4].type)
        ^ (g_board[row - Delta_X[dir] * 3][col - Delta_Y[dir] * 3].type << 2)
        ^ (g_board[row - Delta_X[dir] * 2][col - Delta_Y[dir] * 2].type << 4)
        ^ (g_board[row - Delta_X[dir] * 1][col - Delta_Y[dir] * 1].type << 6)
        ^ (g_board[row + Delta_X[dir] * 1][col + Delta_Y[dir] * 1].type << 8)
        ^ (g_board[row + Delta_X[dir] * 2][col + Delta_Y[dir] * 2].type << 10)
        ^ (g_board[row + Delta_X[dir] * 3][col + Delta_Y[dir] * 3].type << 12)
        ^ (g_board[row + Delta_X[dir] * 4][col + Delta_Y[dir] * 4].type << 14));
}

/*
* Function Name   : GetNodeScore
* Function        : 获取某点的分数
* Input Parameters: int - 行
*                   int - 列
*                   int - 颜色
* Return Value    : int - 某点的分数
*/
inline int GetNodeScore(int row, int col, int color)
{
    return g_pattern_to_score[g_board[row][col].pattern[color][0]][g_board[row][col].pattern[color][1]][g_board[row][col].pattern[color][2]][g_board[row][col].pattern[color][3]];
}

/*
* Function Name   : GetBoardScore
* Function        : 获取当前局面分数，用于在AlphaBeta搜索中评估局面
* Input Parameters: int - 当前评估视角(这一层落子的颜色)
* Return Value    : double - 当前局面分数
*/
double GetBoardScore(int player_color)
{
    int player_type[9] = { 0 };               // 记录我方棋形数
	int enemy_type[9] = { 0 };                // 记录敌方棋形数
    int enemy_color = player_color ^ 1;
	int player_block_four_num = 0;			  	     // 缓存上次循环我方的冲四数量，用于判断活四，连续冲四
    int enemy_block_four_num = 0;			  	     // 缓存上次循环敌方的冲四数量，用于判断活四，连续冲四
    int player_live_three_num = 0;			  	     // 缓存上次循环我方的活三数量
    int enemy_live_three_num = 0;			  	     // 缓存上次循环敌方的活三数量

	// 统计两格以内的空位双方可以形成的棋形
    int i, j;
	for (i = kBoardBegin; i <= kBoardEnd; ++i)
    {
        for (j = kBoardBegin; j <= kBoardEnd; ++j)
        {
            if (g_board[i][j].num_in_2 && g_board[i][j].type == EMPTY)
            {
                //获取上次循环的4/3数量
                player_block_four_num = player_type[T_BLOCK_FOUR];
                enemy_block_four_num = enemy_type[T_BLOCK_FOUR];
                player_live_three_num = player_type[T_LIVE_THREE];
                enemy_live_three_num = enemy_type[T_LIVE_THREE];
                ++player_type[g_board[i][j].pattern[player_color][0]];
                ++player_type[g_board[i][j].pattern[player_color][1]];
                ++player_type[g_board[i][j].pattern[player_color][2]];
                ++player_type[g_board[i][j].pattern[player_color][3]];
                ++enemy_type[g_board[i][j].pattern[enemy_color][0]];
                ++enemy_type[g_board[i][j].pattern[enemy_color][1]];
                ++enemy_type[g_board[i][j].pattern[enemy_color][2]];
                ++enemy_type[g_board[i][j].pattern[enemy_color][3]];
                // 同一个点有两个冲四，那就相当于活四
                if (player_type[T_BLOCK_FOUR] - player_block_four_num >= 2)
                {
                    player_type[T_BLOCK_FOUR] -= 2;
                    player_type[T_LIVE_FOUR] += 1;
                }
                if(enemy_type[T_BLOCK_FOUR] - enemy_block_four_num >= 2)
                {
                    enemy_type[T_BLOCK_FOUR] -= 2;
                    enemy_type[T_LIVE_FOUR] += 1;
                }
                //同一个点有两个活三，那就相当于一个组合
                if(player_type[T_LIVE_THREE] - player_live_three_num >= 2)
                {
                    player_type[T_LIVE_THREE] -= 2;
                    player_type[T_COMBINE] += 1;
                }
                if(enemy_type[T_LIVE_THREE] - enemy_live_three_num >= 2)
                {
                    enemy_type[T_LIVE_THREE] -= 2;
                    enemy_type[T_COMBINE] += 1;
                }
                //同一个点有一个活三，一个冲四，那就相当于一个组合
                if(player_type[T_LIVE_THREE] - player_live_three_num >= 1 && player_type[T_BLOCK_FOUR] - player_block_four_num >= 1)
                {
                    player_type[T_LIVE_THREE] -= 1;
                    player_type[T_BLOCK_FOUR] -= 1;
                    player_type[T_COMBINE] += 1;
                }
                if(enemy_type[T_LIVE_THREE] - enemy_live_three_num >= 1 && enemy_type[T_BLOCK_FOUR] - enemy_block_four_num >= 1)
                {
                    enemy_type[T_LIVE_THREE] -= 1;
                    enemy_type[T_BLOCK_FOUR] -= 1;
                    enemy_type[T_COMBINE] += 1;
                }
            }
        }
    }
	// 当前局面轮到己方下棋
	// 1.己方存在成五点
	// 2.对方存在成五点
	// 3.己方存在活四
    // 4.对方存在活四
	// if(player_type[T_FIVE_LINE] >= 1) return S_FIVE_LINE;
	// if(enemy_type[T_FIVE_LINE] >= 1) return -S_FIVE_LINE;
	// if(player_type[T_LIVE_FOUR] >= 1) return S_LIVE_FOUR;
    // if(enemy_type[T_LIVE_FOUR] >= 1) return -S_LIVE_FOUR;

	double player_score = 0, enemy_score = 0;
    for(i = T_ONE; i <= T_FIVE_LINE; ++i)
    {
        player_score += player_type[i] * g_value[i] / 10;
        enemy_score += enemy_type[i] * g_value[i] / 10;
    }

#if DEBUG_MODE
    if(std::abs(player_score * 2 - enemy_score) > S_FIVE_LINE || std::abs(player_score * 5 - enemy_score) > S_FIVE_LINE)
        printf("DEBUG [Player Score = %.2f, Enemy Score = %.2f]\n", player_score, enemy_score);
#endif

    return player_score * 2 - enemy_score;
}

/*
* Function Name   : UpdateBZ
* Function        : 更新棋盘与哈希值(Borad and Zobrist)
* Input Parameters: int - 行
*                   int - 列
*                   int - 更新的棋子颜色，保证pre_color为-1
*                   int - 上一步棋子颜色，若不为-1，则表示撤销上一步棋子
*/
void UpdateBZ(int row, int col, int color, int pre_color = -1)
{
    int i, j;
    //更新当前位置的棋子
    g_board[row][col].type = color;

    int delta = 0;
    if(color == EMPTY)
        delta = -1;
    else if(color == BLACK || color == WHITE)
        delta = 1;

    //更新其两格内棋子，的两格内棋子的数量
    for(i = -2; i <= 2; ++i)
        for(j = -2; j <= 2; ++j)
            if(CheckBound(row + i, col + j) && (!(i == 0 && j == 0)))
                g_board[row + i][col + j].num_in_2 += delta;

    //更新以当前更改棋子为中心，9*9内的米字线上棋子的对应方向的棋型
    int new_row, new_col, new_key;
    for(i = 0; i < 4; ++i)
    {
        //一侧的四个延伸棋子
        new_row = row + Delta_X[i];
        new_col = col + Delta_Y[i];
        for(j = 0; j < 4 && g_board[new_row][new_col].type != OUT; ++j)
        {
            new_key = GetKeyOfLine(new_row, new_col, i);
            g_board[new_row][new_col].pattern[0][i] = g_pattern_table[new_key][0];
            g_board[new_row][new_col].pattern[1][i] = g_pattern_table[new_key][1];
            new_row += Delta_X[i];
            new_col += Delta_Y[i];
        }
        //另一侧的四个延伸棋子
        new_row = row - Delta_X[i];
        new_col = col - Delta_Y[i];
        for(j = 0; j < 4 && g_board[new_row][new_col].type != OUT; ++j)
        {
            new_key = GetKeyOfLine(new_row, new_col, i);
            g_board[new_row][new_col].pattern[0][i] = g_pattern_table[new_key][0];
            g_board[new_row][new_col].pattern[1][i] = g_pattern_table[new_key][1];
            new_row -= Delta_X[i];
            new_col -= Delta_Y[i];
        }
    } 
    
    //更新总棋子数
    g_chess_count += delta;
    if(color == g_player_color)
        ++g_player_count;
    else if(color == g_enemy_color)
        ++g_enemy_count;
    else if(pre_color == g_player_color)
        --g_player_count;
    else if(pre_color == g_enemy_color)
        --g_enemy_count;

    //更新当前局面哈希值
    if(color == BLACK || color == WHITE)
        g_cur_zobrist_key ^= g_zobrist_board[row][col][color];
    else if(color == EMPTY)
        g_cur_zobrist_key ^= g_zobrist_board[row][col][pre_color];
}

/*
* Function Name   : InitZobrist
* Function        : 初始化Zobrist哈希表
*/
void InitZobrist()
{
    std::random_device rand_device;
    std::mt19937_64 rng(rand_device());
    std::uniform_int_distribution<ull> uni_dist;
    for (int i = kBoardBegin; i <= kBoardEnd; ++i)
        for (int j = kBoardBegin; j <= kBoardEnd; ++j)
            for (int k = 0; k < 2; ++k) 
                g_zobrist_board[i][j][k] = uni_dist(rng);
}

/*
* Function Name   : InitChessType
* Function        : 初始化棋型表与棋型分数表
*/
void InitChessType()
{
    /*初始化棋型表*/
    //将key解码为棋型，后续程序中key将由函数GetKeyOfLine获得
    auto KeyToPattern = [](int key, int color) -> int
    {
        //将key解码到数组line
        int line[9];
		for(int i = 0; i < 9; ++i)
		{
			if(i == 4)
				line[i] = color;
			else
			{
				line[i] = key & 3;
				key >>= 2;
			}
		}

        //检查line是不是五连
		auto Check5 = [] (int *line, int color) -> bool
		{
			int length = 0;
			for(int i = 0; i < 9; ++i)
			{
				if(line[i] == color)
				{
					++length;
					if(length >= 5)
						return true;
				}
				else
					length = 0;
			}
			return false;
		};

        //检查line是不是活四或冲四
		auto Check4 = [Check5] (int *line, int color) -> int
		{
			int will_five_count = 0;
			for(int i = 0; i < 9; ++i)
			{
				if(line[i] != EMPTY)
					continue;
				line[i] = color;
				if(Check5(line, color))
					++will_five_count;
				line[i] = EMPTY;
			}

			if(will_five_count >= 2)
				return T_LIVE_FOUR;
			else if(will_five_count == 1)
				return T_BLOCK_FOUR;
			else
				return 0;	
		};

        //检查line是不是活三或冲三
		auto Check3 = [Check4] (int *line, int color) -> int
		{
			int will_live_four_count = 0;
			int will_block_four_count = 0;
			for(int i = 0; i < 9; ++i)
			{
				if(line[i] != EMPTY)
					continue;
				line[i] = color;
				int four_type = Check4(line, color);
				if(four_type == T_LIVE_FOUR)
					++will_live_four_count;
				else if(four_type == T_BLOCK_FOUR)
					++will_block_four_count;
				line[i] = EMPTY;
			}

			if(will_live_four_count > 0)
				return T_LIVE_THREE;
			else if(will_block_four_count > 0)
				return T_BLOCK_THREE;	
			else
				return 0;
		};

        //检查line是不是活二或冲二
		auto Check2 = [Check3] (int *line, int color) -> int
		{
			int will_live_three_count = 0;
			int will_block_three_count = 0;
			for(int i = 0; i < 9; ++i)
			{
				if(line[i] != EMPTY)
					continue;
				line[i] = color;
				int three_type = Check3(line, color);
				if(three_type == T_LIVE_THREE)
					++will_live_three_count;
				else if(three_type == T_BLOCK_THREE)
					++will_block_three_count;
				line[i] = EMPTY;
			}

			if(will_live_three_count > 0)
				return T_LIVE_TWO;
			else if(will_block_three_count > 0)
				return T_BLOCK_TWO;
			else 	
				return 0;
		};
        
		if(Check5(line, color))
			return T_FIVE_LINE;

		int four_type = Check4(line, color);
		if(four_type)
			return four_type;

		int three_type = Check3(line, color);
		if(three_type)
			return three_type;
		
		int two_type = Check2(line, color);
		if(two_type)
			return two_type;
		
		return T_ONE;
    };
    //遍历所有key，转为棋型并存入g_pattern_table
    for(int key = 0; key < 65536; ++key)//65536=2^16，8格棋子每格占2位
    {
        //如果中心点是0/1，长为8（不包括中心）的棋线的哈希值是key,则这条线的棋型是KeyToPattern(key, 0/1)
        g_pattern_table[key][BLACK] = KeyToPattern(key, BLACK);
        g_pattern_table[key][WHITE] = KeyToPattern(key, WHITE);
    }

    /*初始化棋型分数表*/
    //根据某个点四个方向上的棋型打分
    auto PatternToScore = [](int vertical, int parallel, int left_slant, int right_slant) -> double
    {
        double type[9] = { 0 };
        ++type[vertical];
        ++type[parallel];
        ++type[left_slant];
        ++type[right_slant];

        //如果有两个冲四，那就相当于有一个活四
        if(type[T_BLOCK_FOUR] >= 2)
        {
            type[T_BLOCK_FOUR] -= 2;
            type[T_LIVE_FOUR] += 1;
        }
        //如果有两个活三，那就相当于有一个组合
        if(type[T_LIVE_THREE] >= 2)
        {
            type[T_LIVE_THREE] -= 2;
            type[T_COMBINE] += 1;
        }
        //如果有一个活三和一个冲四，那就相当于有一个组合
        if(type[T_BLOCK_FOUR] >= 1 && type[T_LIVE_THREE] >= 1)
        {
            type[T_BLOCK_FOUR] -= 1;
            type[T_LIVE_THREE] -= 1;
            type[T_COMBINE] += 1;
        }
        double score = 0;
        for(int i = T_ONE; i <= T_FIVE_LINE; ++i)
            score += g_value[i] * type[i];
        return score;
    };
    //遍历一个点四个方向(- | \ /)上的棋型，为其打分  
    for(int vertical = 0; vertical < 9; ++vertical)
        for(int parallel = 0; parallel < 9; ++parallel)
            for(int left_slant = 0; left_slant < 9; ++left_slant)
                for(int right_slant = 0; right_slant < 9; ++right_slant)
                    g_pattern_to_score[vertical][parallel][left_slant][right_slant] = PatternToScore(vertical, parallel, left_slant, right_slant);
        
}

/*
* Function Name   : Initialize
* Function        : 初始化（四个预置棋子，棋盘边界内外类型，哈希表，棋型表）
*/
void Initialize()
{
    InitZobrist();
    InitChessType();
    
    for(int i = 0; i < kBoardSize + 8; ++i)
        for(int j = 0; j < kBoardSize + 8; ++j)
            if(CheckBound(i, j))
                g_board[i][j].type = EMPTY;
            else
                g_board[i][j].type = OUT;

    UpdateBZ(6 + kCompensate, 5 + kCompensate, BLACK, -1);
    UpdateBZ(5 + kCompensate, 6 + kCompensate, BLACK, -1);
    UpdateBZ(6 + kCompensate, 6 + kCompensate, WHITE, -1);
    UpdateBZ(5 + kCompensate, 5 + kCompensate, WHITE, -1);
}

/*
* Function Name   : EnemyMove
* Function        : 敌方走棋
* Input Parameters: int - 行
*                   int - 列
*/
void EnemyMove(int row, int col)
{
    UpdateBZ(row, col, g_enemy_color, -1);
}

/*
* Function Name   : GetGreedyMove
* Function        : AUTO_MODE下贪心搜索下一步落子位置
* Input Parameters: int - 行
*                   int - 列
*/
void GetGreedyMove(int &row, int &col)
{
    int i, j;
    double max_score = -10 * S_FIVE_LINE;
    for(i = kBoardBegin; i <= kBoardEnd; ++i)
    {
        for(j = kBoardBegin; j <= kBoardEnd; ++j)
        {
            if(g_board[i][j].type != EMPTY || g_board[i][j].num_in_2 <= 1)
                continue;
            double score = 0;
            score += ATK_RATIO * GetNodeScore(i, j, g_player_color);
            score += DEF_RATIO * GetNodeScore(i, j, g_enemy_color);
            if(score > max_score)
            {
                max_score = score;
                row = i;
                col = j;
            }
            else if(score == max_score && g_board[i][j].num_in_2 > g_board[row][col].num_in_2)
            {
                row = i;
                col = j;
            }
        }
    }
}

/*
* Function Name   : GetOptionalMove
* Function        : 启发式获取候选落子点
* Input Parameters: std::vector<Position> - 候选落子点
*                   int - 当前落子视角
*/
void GetOptionalMove(std::vector<Position> &positions, int player_color)
{
    int i, j;
    int enemy_color = player_color ^ 1;
    positions.clear();
    //先判断我方是否有五连点
    for(i = kBoardBegin; i <= kBoardEnd; ++i)
    {
        for(j = kBoardBegin; j <= kBoardEnd; ++j)
        {
            if(g_board[i][j].type != EMPTY || g_board[i][j].num_in_2 == 0)
                continue;
            if(GetNodeScore(i, j, player_color) >= S_FIVE_LINE)
            {
                //有一个立刻返回，漏了也无所谓，因为必赢
                positions.push_back(Position{i, j, 0});
                return;
            }
        }
    }
    //再判断敌方是否有五连点
    for(i = kBoardBegin; i <= kBoardEnd; ++i)
    {
        for(j = kBoardBegin; j <= kBoardEnd; ++j)
        {
            if(g_board[i][j].type != EMPTY || g_board[i][j].num_in_2 == 0)
                continue;
            if(GetNodeScore(i, j, enemy_color) >= S_FIVE_LINE)
            {
                //有一个立刻返回，漏了也无所谓，因为>=1必输
                positions.push_back(Position{i, j, 0});
                return;
            }
        }
    }
    //判断我方是否有活四
    for(i = kBoardBegin; i <= kBoardEnd; ++i)
    {
        for(j = kBoardBegin; j <= kBoardEnd; ++j)
        {
            if(g_board[i][j].type != EMPTY || g_board[i][j].num_in_2 == 0)
                continue;
            if(GetNodeScore(i, j, player_color) >= S_LIVE_FOUR)
            {
                positions.push_back(Position{i, j, 0});
                return;
            }
        }
    }
    //判断敌方是否有活四
    for(i = kBoardBegin; i <= kBoardEnd; ++i)
    {
        for(j = kBoardBegin; j <= kBoardEnd; ++j)
        {
            if(g_board[i][j].type != EMPTY || g_board[i][j].num_in_2 == 0)
                continue;
            if(GetNodeScore(i, j, enemy_color) >= S_LIVE_FOUR)
            {
                positions.push_back(Position{i, j, 0});
                return;
            }
        }
    }
    //判断我方是否有组合
    for(i = kBoardBegin; i <= kBoardEnd; ++i)
    {
        for(j = kBoardBegin; j <= kBoardEnd; ++j)
        {
            if(g_board[i][j].type != EMPTY || g_board[i][j].num_in_2 == 0)
                continue;
            if(GetNodeScore(i, j, player_color) >= S_COMBINE)
            {
                positions.push_back(Position{i, j, 0});
                return;
            }
        }
    }
    //判断敌方是否有组合
    for(i = kBoardBegin; i <= kBoardEnd; ++i)
    {
        for(j = kBoardBegin; j <= kBoardEnd; ++j)
        {
            if(g_board[i][j].type != EMPTY || g_board[i][j].num_in_2 == 0)
                continue;
            if(GetNodeScore(i, j, enemy_color) >= S_COMBINE)
            {
                positions.push_back(Position{i, j, 0});
                return;
            }
        }
    }

    //如果双方都没有五连四连组合点
    double cur_score = 0;
    for(i = kBoardBegin; i <= kBoardEnd; ++i)
    {
        for(j = kBoardBegin; j <= kBoardEnd; ++j)
        {
            if(g_board[i][j].type != EMPTY || g_board[i][j].num_in_2 <= 0)
                continue;
            cur_score = 0;
            cur_score += ATK_RATIO * GetNodeScore(i, j, player_color);
            cur_score += DEF_RATIO * GetNodeScore(i, j, enemy_color);
            positions.push_back(Position{i, j, cur_score});
        }
    }

    //排序，取前kMaxMoves个
    std::sort(positions.begin(), positions.end(), [](const Position &a, const Position &b) {
        if(a.temp_score != b.temp_score)
            return a.temp_score > b.temp_score;
        return g_board[a.row][a.col].num_in_2 > g_board[b.row][b.col].num_in_2;
    });
    if(positions.size() > kMaxMoves)
        positions.resize(kMaxMoves);
}

/*
* Function Name   : AlphaBeta
* Function        : AlphaBeta -> negamax -> pvs搜索
* Input Parameters: int - 当前搜索深度
*                   double - alpha值
*                   double - beta值
*                   int - 当前落子视角
* Return Value    : double - 当前落子后的min/max局面分数
*/
double AlphaBeta(int depth, double alpha, double beta, int player_color, Position& last_move)
{
    //如果当前局面已经搜索过了
    if(g_zobrist_hashtable.find(g_cur_zobrist_key) != g_zobrist_hashtable.end())
    {
        HashEntry entry = g_zobrist_hashtable[g_cur_zobrist_key];
        if(entry.depth >= depth)
        {
            if(entry.type == HASH_EXACT)
                return entry.score;
            else if(entry.type == HASH_ALPHA && entry.score <= alpha)
                return alpha;
            else if(entry.type == HASH_BETA && entry.score >= beta)
                return beta;
        }
    }

    int hash_type = HASH_ALPHA;
    int enemy_color = player_color ^ 1;
    bool find_pv = false;
    double score;

    //如果深度为0 或 棋盘满了 或 上一步敌方已经成五
    if(depth == 0 || g_chess_count == kBoardSize * kBoardSize || GetNodeScore(last_move.row, last_move.col, enemy_color) >= S_FIVE_LINE)
    {
        score = GetBoardScore(player_color);
        g_zobrist_hashtable[g_cur_zobrist_key] = HashEntry{depth, HASH_EXACT, score};
        return score;
    }

    //如果当前局面已经超时
    if(g_is_timeout || std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - g_start_time).count() > kTimeLimit - 100)
    {
        g_is_timeout = true;
        return GetBoardScore(player_color);
    }

    // 获取当前局面的最佳落子点
    std::vector<Position> optional_moves;
    GetOptionalMove(optional_moves, player_color);

    for(auto &move : optional_moves)
    {
        UpdateBZ(move.row, move.col, player_color, -1);
        if(find_pv)
        {
            //pvs搜索
            score = -AlphaBeta(depth - 1, -alpha - 1, -alpha, enemy_color, move);
            if(alpha < score && score < beta)//pvs失败，重新ab搜索
                score = -AlphaBeta(depth - 1, -beta, -alpha, enemy_color, move);
        }
        else
            score = -AlphaBeta(depth - 1, -beta, -alpha, enemy_color, move);
        UpdateBZ(move.row, move.col, EMPTY, player_color);
        if(score >= beta)
        {
            g_zobrist_hashtable[g_cur_zobrist_key] = HashEntry{depth, HASH_BETA, beta};
            return beta;
        }
        if(score > alpha)
        {
            alpha = score;
            hash_type = HASH_EXACT;
            find_pv = true;
        }
    }
    g_zobrist_hashtable[g_cur_zobrist_key] = HashEntry{depth, hash_type, alpha};
    return alpha;
}

/*
* Function Name   : IDDFS
* Function        : 开始迭代加深搜索
* Input Parameters: int - 当前落子视角
*                   Position - 最佳落子点
*/
void IDDFS(int player_color, Position &best_move, std::vector<Position> &optional_moves)
{
    double max_score = -10 * S_FIVE_LINE, last_score, alpha, beta, score;
    int enemy_color = player_color ^ 1;
    Position last_best_move;
    GetOptionalMove(optional_moves, player_color);
    for(int iterative_depth = kMinDepth; iterative_depth <= kMaxABDepth; iterative_depth += 1)
    {
        //如果当前迭代或上一次迭代的alphabeta已经超时，直接返回最佳走法
        if(g_is_timeout || std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - g_start_time).count() > kTimeLimit)
            break;
        alpha = -10 * S_FIVE_LINE, beta = 10 * S_FIVE_LINE; 
        last_score = max_score; 
        last_best_move = best_move;
        for(auto &move : optional_moves)
        {
            if(g_is_timeout || std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - g_start_time).count() > kTimeLimit)
                break;
            UpdateBZ(move.row, move.col, player_color, -1);
            score = -AlphaBeta(iterative_depth - 1, -beta, -alpha, enemy_color, move);
            UpdateBZ(move.row, move.col, EMPTY, player_color);
            if(g_is_timeout || std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - g_start_time).count() > kTimeLimit)
                break;
            if(score > max_score)
            {
                max_score = score;
                best_move = move;
            }//如果分数相同，选择两格内棋子数多的
            else if(score == max_score && g_board[move.row][move.col].num_in_2 > g_board[best_move.row][best_move.col].num_in_2)
                best_move = move;
        }
#if DEBUG_MODE
        printf("DEBUG [Iterative Depth = %d, Time = %ldms]\n", iterative_depth, std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - g_start_time).count());
        fflush(stdout);
        printf("DEBUG [Best Move = (%d, %d), Score = %.2f]\n", best_move.row, best_move.col, max_score);
        fflush(stdout);
#endif
        if(std::abs(max_score - last_score) < 1e-6 && best_move == last_best_move && iterative_depth >= 8)
            break;
    }
}

void VCF(int player_color, Position &best_move, std::vector<Position> &optional_moves)
{
    int i, j;
    for(i = kBoardBegin; i <= kBoardEnd; i++)
    {
        for(j = kBoardBegin; j <= kBoardEnd; j++)
        {
            if(g_board[i][j].type != EMPTY || g_board[i][j].num_in_2 == 0)
                continue;
            if(g_board[i][j].pattern[player_color][0] >= T_BLOCK_FOUR 
            || g_board[i][j].pattern[player_color][1] >= T_BLOCK_FOUR 
            || g_board[i][j].pattern[player_color][2] >= T_BLOCK_FOUR 
            || g_board[i][j].pattern[player_color][3] >= T_BLOCK_FOUR)
            {
                optional_moves.push_back(Position{i, j, ATK_RATIO * GetNodeScore(i, j, player_color) + DEF_RATIO * GetNodeScore(i, j, player_color)});
            }
        }
    }
    //不存在VCF点
    if(optional_moves.size() == 0)
        return;
}

/*
* Function Name   : GetBestMove
* Function        : 获取最佳落子点
* Input Parameters: int - 当前落子视角
* Return Value    : Position - 最佳落子点
*/
void GetBestMove(int player_color, Position &best_move)
{
    int i, j;
    int enemy_color = player_color ^ 1;
    g_is_timeout = false;
    //第一手背谱
    if(g_chess_count == 4)//执黑
    {
        UpdateBZ(7 + kCompensate, 7 + kCompensate, BLACK, -1);
        best_move = Position{7 + kCompensate, 7 + kCompensate};
        return;
    }
    else if(g_chess_count == 5)//执白
    {
        //如果对面花月阵，防守一波
        if(g_board[7 + kCompensate][7 + kCompensate].type == BLACK)
        {
            UpdateBZ(4 + kCompensate, 6 + kCompensate, WHITE, -1);
            best_move = Position{4 + kCompensate, 6 + kCompensate};
            return;
        }
        else if(g_board[4 + kCompensate][4 + kCompensate].type == BLACK)
        {
            UpdateBZ(7 + kCompensate, 5 + kCompensate, WHITE, -1);
            best_move = Position{7 + kCompensate, 5 + kCompensate};
            return;
        }
        //如果对面活三，八卦阵起
        if(g_board[4 + kCompensate][7 + kCompensate].type == BLACK)
        {
            UpdateBZ(7 + kCompensate, 4 + kCompensate, WHITE, -1);
            best_move = Position{7 + kCompensate, 4 + kCompensate};
            return;
        }
        else if(g_board[7 + kCompensate][4 + kCompensate].type == BLACK)
        {
            UpdateBZ(4 + kCompensate, 7 + kCompensate, WHITE, -1);
            best_move = Position{4 + kCompensate, 7 + kCompensate};
            return;
        }
    }
    
    //第二手或对面不背谱，开始正常搜索
    //如果我方已经有五连点，直接落子，返回
    for(i = kBoardBegin; i <= kBoardEnd; ++i)
    {
        for(j = kBoardBegin; j <= kBoardEnd; ++j)
        {
            if(g_board[i][j].type != EMPTY || g_board[i][j].num_in_2 == 0)
                continue;
            if(GetNodeScore(i, j, player_color) >= S_FIVE_LINE)
            {    
                UpdateBZ(i, j, player_color, -1);
                best_move = Position{i, j};
                return;
            }
        }
    }
    //如果敌方已经有五连点，直接返回
    for(i = kBoardBegin; i <= kBoardEnd; ++i)
    {
        for(j = kBoardBegin; j <= kBoardEnd; ++j)
        {
            if(g_board[i][j].type != EMPTY || g_board[i][j].num_in_2 == 0)
                continue;
            if(GetNodeScore(i, j, enemy_color) >= S_FIVE_LINE)
            {
                UpdateBZ(i, j, player_color, -1);
                best_move = Position{i, j};
                return;
            }
        }
    }
    //如果双方都没有五连点，开始搜索
    std::vector<Position> positions;
    // if(!g_is_aggressive)
    // {
    //     for(i = kBoardBegin; i <= kBoardEnd; ++i)
    //     {
    //         for(j = kBoardBegin; j <= kBoardEnd; ++j)
    //         {
    //             if(g_board[i][j].type != EMPTY || g_board[i][j].num_in_2 == 0)
    //                 continue;
    //             double cur = 0;
    //             cur += GetNodeScore(i, j, player_color);
    //             cur += 1.5 * GetNodeScore(i, j, enemy_color);
    //             if(cur > max_score)
    //             {
    //                 max_score = cur;
    //                 best_move = Position{i, j};
    //             }
    //             else if(cur == max_score && g_board[i][j].num_in_2 > g_board[best_move.row][best_move.col].num_in_2)
    //                 best_move = Position{i, j};
    //         }
    //     }
    //     UpdateBZ(best_move.row, best_move.col, player_color, -1);
    //     return;
    // }

    //先算杀
    VCF(player_color, best_move, positions);
    //再迭代加深搜索
    IDDFS(player_color, best_move, positions);
    UpdateBZ(best_move.row, best_move.col, player_color, -1);
}

/*
* Function Name   : PrintBoard
* Function        : DEBUG_MODE下打印棋盘
*/
void PrintBoard()
{
    printf("  ");
    for(int i = 0; i < 12; ++i)
        printf("%d ", i % 10);
    printf("\n");
    for(int i = kBoardBegin; i <= kBoardEnd; ++i)
    {
        printf("%d ", (i - kCompensate) % 10);
        for(int j = kBoardBegin; j <= kBoardEnd; ++j)
        {
            if(g_board[i][j].type == BLACK && i == g_last_row && j == g_last_col)
                printf("# ");
            else if(g_board[i][j].type == WHITE && i == g_last_row && j == g_last_col)
                printf("@ ");
            else if(g_board[i][j].type == BLACK)
                printf("X ");
            else if(g_board[i][j].type == WHITE)
                printf("O ");
            else
                printf(". ");
        }
        printf("\n");
    }
    printf("\n");
}