#include <vector>
#include <iostream>
using namespace std;

typedef short state_t;

struct Move {
    short move;
    const char * desc;
};
typedef Move move_t;

struct Options {
    short moves[4];
    int size;
};

// name the bits
# define P1 1
# define P2 1 << 1
# define P3 1 << 2
# define P4 1 << 3
# define P5 1 << 4
# define P6 1 << 5
# define P7 1 << 6
# define P8 1 << 7
# define P9 1 << 8
# define P10 1 << 9
# define P11 1 << 10
# define P12 1 << 11
# define P13 1 << 12
# define P14 1 << 13 
# define P15 1 << 14

// not valid location
# define P16 1 << 15

// move triplets
Options options[15] = {
    {{P1|P2|P4, P1|P3|P6}, 2},
    {{P2|P4|P7, P2|P5|P9},2},
    {{P3|P5|P8, P3|P6|P10},2},
    {{P1|P2|P4, P4|P7|P11, P4|P5|P6, P4|P8|P13},4},
    {{P5|P8|P12, P5|P9|P14},2},
    {{P1|P3|P6, P4|P5|P6, P6|P9|P13, P6|P10|P15},4},
    {{P7|P4|P2, P7|P8|P9},2},
    {{P8|P5|P3,P8|P9|P10},2},
    {{P9|P8|P7,P9|P5|P2},2},
    {{P10|P6|P3,P10|P9|P8},2},
    {{P11|P7|P4,P11|P12|P13},2},
    {{P12|P8|P5,P12|P13|P14},2},
    {{P13|P12|P11,P13|P14|P15,P13|P8|P4,P13|P9|P6},4},
    {{P14|P9|P5,P14|P13|P12},2},
    {{P15|P10|P6,P15|P14|P13},2}
};

// legal moves
Options legal[15] = {
    {{P1|P2, P1|P3}, 2},
    {{P2|P4, P2|P5},2},
    {{P3|P5, P3|P6},2},
    {{P4|P2, P4|P7, P4|P5, P4|P8},4},
    {{P5|P8,P5|P9},2},
    {{P6|P3, P6|P5, P6|P9, P6|P10}, 4},
    {{P7|P4, P7|P8},2},
    {{P8|P5, P8|P9},2},
    {{P9|P8,P9|P5},2},
    {{P10|P6,P10|P9},2},
    {{P11|P7,P11|P12},2},
    {{P12|P8,P12|P13},2},
    {{P13|P12,P13|P14,P13|P8,P13|P9},4},
    {{P14|P9,P14|P13},2},
    {{P15|P10,P15|P14},2}
};

// for printing solution
struct OptionDesc {
    const char* name[4];
    int size;
};

OptionDesc desc[15] = {
    {{"p1 => p4", "p1 => p6"}, 2},
    {{"p2 => p7", "p2 => p9"}, 2},
    {{"p3 => p8", "p3 => p10"}, 2},
    {{"p4 => p1", "p4 => p11", "p4 => p6", "p4 => p13"}, 4},
    {{"p5 => p12", "p5 => p14"}, 2},
    {{"p6 => p1", "p6 => p4", "p6 => p13", "p6 => p15"}, 4},
    {{"p7 => p2", "p7 => p9"}, 2},
    {{"p8 => p3", "p8 => p10"}, 2},
    {{"p9 => p7", "p9 => p2"}, 2},
    {{"p10 => p3", "p10 => p8"}, 2},
    {{"p11 => p4", "p11 => p13"}, 2},
    {{"p12 => p5", "p12 => p14"}, 2},
    {{"p13 => p11", "p13 => p15", "p13 => p4", "p13 => p6"}, 4},
    {{"p14 => p5", "p14 => p12"}, 2},
    {{"p15 => p6", "p15 => p13"}, 2}
};

int LEGAL_COUNT = sizeof (legal) / sizeof (Options);

state_t START = P2|P3|P4|P5|P6|P7|P8|P9|P10|P11|P12|P13|P14|P15;

// make move: just xor
inline void make_move(state_t& s, move_t m) 
{
    s ^= m.move;
}

// undo move: just xor
inline void unmake_move (state_t& s, move_t m)
{
    s ^= m.move;
}

// define end state as peg in top position
inline bool end_state (state_t s)
{
    return (s ^ START) == (START|P1);
}

// generates moves from table of legal moves, and table of all possible move options
inline void generate_moves(state_t s, vector<move_t>& moves) 
{
    for (int i = 0; i < LEGAL_COUNT; i++) {
        for (int j = 0; j < legal[i].size; j++) {
            short L = legal[i].moves[j];
            short M = L ^ options[i].moves[j];
            if ((s & L) == L && (s & M) == 0) {
                move_t m;
                m.move = options[i].moves[j];
                m.desc = desc[i].name[j];
                moves.push_back(m);
            }
        }
    }
}

// basic depth first search:
bool dfs (state_t& s, int& count)
{
    bool found = false;

    if (end_state(s)) {
        count++;
        return true;
    }

    vector<move_t> moves;
    generate_moves(s, moves);

    for (vector<move_t>::iterator it = moves.begin();
            it != moves.end(); it++) {
        make_move (s, *it);
        found = dfs(s,count);
        unmake_move(s, *it);
        if (found && 0) {
            cout << it->desc << endl;
            return true;
        }
    }
    return false;
}

void init(state_t& s)
{
    s = START;
}

int main(int argc, char* argv[])
{
    state_t s;
    int count = 0;
    init(s);
    bool solved = dfs (s, count);
    cout << "solutions = " << count << endl;
    char c;
    cin >> c;
    return 0;
}
