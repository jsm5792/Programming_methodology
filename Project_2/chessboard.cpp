#include "chessboard.h"

using namespace std;


void ChessBoard::initialize_board()
{
    // TODO: implement initialize_board
    tiles = new ChessPiece**[height];
    for(int i=0; i<height; i++)
    {
        tiles[i] = new ChessPiece*[width];
        for(int j=0; j<width; j++)
        {
            tiles[i][j] = nullptr;
        }
    }
}

ostream& operator<<(ostream& os, const ChessBoard& board)
{
    // TODO: implement operator<<
    for(int i=0; i<board.height; i++)
    {
        for(int j=0; j<board.width; j++)
        {
            if(board.tiles[i][j] != nullptr)
            {
                os << *(board.tiles[i][j]);
            }
            else
            {
                os << ".";
            }
        }
        os << "\n";
    }
    return os;
}

ChessPiece* ChessBoard::operator[](string s)
{
    // TODO: implement operator[]
    int x,y;
    if(s.length()==3)
    {
        y = s[1] - 'a';
        x = height - s[2] + '0';
    }
    else
    {
        y = s[1] - 'a';
        x = height - (s[2] - '0')* 10 - (s[3] - '0'); 

    }
    return tiles[x][y];
}

ChessBoard& ChessBoard::operator=(const ChessBoard& other)
{
    // TODO: implement operator=
    if(this == &other)
    {
        return * this;
    }
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            delete tiles[i][j];
        }
        delete[] tiles[i];
    }
    delete[] tiles;

    width = other.width;
    height = other.height;

    tiles = new ChessPiece**[height];
    for(int i=0;i<height; i++)
    {
        tiles[i]= new ChessPiece*[width];
        for(int j=0;j<width; j++)
        {
            if(other.tiles[i][j]!=nullptr)
            {
                tiles[i][j] = create_piece(other.tiles[i][j]);
            }
            else
            {
                tiles[i][j] = nullptr;
            }
            
        }
    }
    for(auto i: black_pieces)
    {
        delete i;
    }

    black_pieces.clear();

    for(auto i: white_pieces)
    {
        delete i;
    }
    
    white_pieces.clear();

    for(auto i: other.black_pieces)
    {
        ChessPiece *p;
        p = create_piece(i);
        black_pieces.insert(p);
    }
    for(auto i: other.white_pieces)
    {
        ChessPiece *p;
        p = create_piece(i);
        white_pieces.insert(p);
    }

    return *this;
}

ChessBoard::ChessBoard(const ChessBoard& other)
{
    // TODO: implement copy constructor
    width = other.width;
    height = other.height;
    tiles = new ChessPiece**[height];
    for(int i=0;i<height; i++)
    {
        tiles[i]= new ChessPiece*[width];
        for(int j=0;j<width; j++)
        {
            for(int j=0;j<width; j++)
            {
                if(other.tiles[i][j]!=nullptr)
                {
                    tiles[i][j] = create_piece(other.tiles[i][j]);
                }
                else
                {
                    tiles[i][j] = nullptr;
                }
            
            }
        }
    }
    for(auto i: other.black_pieces)
    {
        ChessPiece *p;
        p = create_piece(i);
        black_pieces.insert(p);
    }
    for(auto i: other.white_pieces)
    {
        ChessPiece *p;
        p = create_piece(i);
        white_pieces.insert(p);
    }
}

// NOTE: YOU CAN ADD MORE FUNCTIONS HERE

ChessBoard* swap_board(ChessBoard &board, ChessPiece *Piece, int x, int y)
{
    ChessBoard* new_board;
    new_board = new ChessBoard(board);
    new_board->swapTile(Piece,x,y);
    return new_board;
}

vector<pair<int,int>> get_move(ChessBoard &board, ChessPiece *piece)
{
    vector<pair<int,int>> can_move;
    vector<tuple<MoveType, Direction, int>> moves = piece->get_possible_moves();
    int height = board.get_height();
    int width = board.get_width();
    string color = piece->get_color();
    for(const auto &itr : moves)
    {
        
        Direction dir1 = get<1>(itr);
        int x = piece->get_x();
        int y = piece->get_y();

        if(piece->get_type()=="Queen"|| piece->get_type() == "Rook" || piece->get_type() == "Bishop")
        {
            while(1)
            {
                x+=direction_to_pair[dir1].first;
                y+=direction_to_pair[dir1].second;
                if(x>=height||x<0)
                {
                    break;
                }
                if(y>=width||y<0)
                {
                    break;
                }
                if(board.exist_piece(x,y))
                {
                    if(board.find_piece(x,y)->get_color()==color)
                    {
                        break;
                    }
                    else
                    {
                        can_move.push_back(make_pair(x,y));
                        break;
                    }
                }
                else
                {
                    can_move.push_back(make_pair(x,y));               
                }
            }
        }

        else if(piece->get_type()=="Pawn")
        {
            if(piece->get_color()=="Black")
            {
                if(x==1)
                {
                    if(x+2<height&& !board.exist_piece(x+2,y) && !board.exist_piece(x+1,y))
                    {
                        can_move.push_back(make_pair(x+2,y));
                    }
                }
                if(true)
                {
                    if(x+1<height && y-1 >=0 && board.exist_piece(x+1,y-1))
                    {
                        if(board.find_piece(x+1,y-1)->get_color()!="Black")
                        {
                            can_move.push_back(make_pair(x+1,y-1));
                        }
                    }
                    if(x+1<height && y+1 < width && board.exist_piece(x+1,y+1))
                    {
                        if(board.find_piece(x+1,y+1)->get_color()!="Black")
                        {
                            can_move.push_back(make_pair(x+1,y+1));
                        }
                    }
                    if(x+1<height && !board.exist_piece(x+1,y))
                    {
                        can_move.push_back(make_pair(x+1,y));
                    }
                }
            }
            else
            {
                if(x==height-2)
                {
                    if(x-2>=0&& !board.exist_piece(x-2,y) && !board.exist_piece(x-1,y))
                    {
                        can_move.push_back(make_pair(x-2,y));
                    }
                }
                if(true)
                {
                    if(x-1>=0 && y-1 >=0 && board.exist_piece(x-1,y-1))
                    {
                        if(board.find_piece(x-1,y-1)->get_color()=="Black")
                        {
                            can_move.push_back(make_pair(x-1,y-1));
                        }
                    }
                    if(x-1>=0 && y+1 < width && board.exist_piece(x-1,y+1))
                    {
                        if(board.find_piece(x-1,y+1)->get_color()=="Black")
                        {
                            can_move.push_back(make_pair(x-1,y+1));
                        }

                    }
                    if(x-1>=height && !board.exist_piece(x-1,y))
                    {
                        can_move.push_back(make_pair(x-1,y));
                    }
                }
                
            }
        }

        else if(piece->get_type()=="Knight")
        {
            x+=direction_to_pair[dir1].first;
            y+=direction_to_pair[dir1].second;
            if(x>=height||x<0)
            {
                continue;
            }
            else if(y>=width||y<0)
            {
                continue;
            }
            else if(board.exist_piece(x,y))
            {
                if(board.find_piece(x,y)->get_color()==color)
                {
                    continue;
                }
                else
                {
                    can_move.push_back(make_pair(x,y));
                    continue;
                }

            }
            else
            {
                can_move.push_back(make_pair(x,y));               
            }

        }
        
        else if(piece->get_type()=="King")
        {
            x+=direction_to_pair[dir1].first;
            y+=direction_to_pair[dir1].second;
            if(x>=height||x<0)
            {
                continue;
            }
            else if(y>=width||y<0)
            {
                continue;
            }
            else if(board.exist_piece(x,y))
            {
                if(board.find_piece(x,y)->get_color()==color)
                {
                    continue;
                }
                else
                {
                    if(x-1>=0 && y-1 >=0 &&board.exist_piece(x-1,y-1) && board.find_piece(x-1,y-1)->get_type()=="King" && board.find_piece(x-1,y-1)->get_color()!=color)
                    {
                        continue;
                    }
                    else if(x-1>=0 && board.exist_piece(x-1,y) && board.find_piece(x-1,y)->get_type()=="King"&& board.find_piece(x-1,y)->get_color()!=color)
                    {
                        continue;
                    }
                    else if(x-1>=0 && y+1<width && board.exist_piece(x-1,y+1) && board.find_piece(x-1,y+1)->get_type()=="King"&& board.find_piece(x-1,y+1)->get_color()!=color)
                    {
                        continue;
                    }
                    else if(y-1 >=0 && board.exist_piece(x,y-1) && board.find_piece(x,y-1)->get_type()=="King"&& board.find_piece(x,y-1)->get_color()!=color)
                    {
                        continue;
                    }
                    else if(y+1 < width && board.exist_piece(x,y+1) && board.find_piece(x,y+1)->get_type()=="King"&& board.find_piece(x,y+1)->get_color()!=color)
                    {
                        continue;
                    }
                    else if(x+1<height && y-1 >=0 &&board.exist_piece(x+1,y-1) && board.find_piece(x+1,y-1)->get_type()=="King"&& board.find_piece(x+1,y-1)->get_color()!=color)
                    {
                        continue;
                    }
                    else if(x+1<height && board.exist_piece(x+1,y) && board.find_piece(x+1,y)->get_type()=="King"&& board.find_piece(x+1,y)->get_color()!=color)
                    {
                        continue;
                    }
                    else if(x+1 <height && y+1 < width && board.exist_piece(x+1,y+1) && board.find_piece(x+1,y+1)->get_type()=="King"&& board.find_piece(x+1,y+1)->get_color()!=color)
                    {
                        continue;
                    }
                    else 
                    {
                        can_move.push_back(make_pair(x,y));
                    }
                }

            }
            else
            {
                if(x-1>=0 && y-1 >=0 &&board.exist_piece(x-1,y-1) && board.find_piece(x-1,y-1)->get_type()=="King"&& board.find_piece(x-1,y-1)->get_color()!=color)
                {

                    continue;
                }
                else if(x-1>=0 && board.exist_piece(x-1,y) && board.find_piece(x-1,y)->get_type()=="King"&& board.find_piece(x-1,y)->get_color()!=color)
                {
                    continue;
                }
                else if(x-1>=0 && y+1<width && board.exist_piece(x-1,y+1) && board.find_piece(x-1,y+1)->get_type()=="King"&& board.find_piece(x-1,y+1)->get_color()!=color)
                {
                    continue;
                }
                else if(y-1 >=0 && board.exist_piece(x,y-1) && board.find_piece(x,y-1)->get_type()=="King"&& board.find_piece(x,y-1)->get_color()!=color)
                {
                    continue;
                }
                else if(y+1 < width && board.exist_piece(x,y+1) && board.find_piece(x,y+1)->get_type()=="King"&& board.find_piece(x,y+1)->get_color()!=color)
                {
                    continue;
                }
                else if(x+1<height && y-1 >=0 &&board.exist_piece(x+1,y-1) && board.find_piece(x+1,y-1)->get_type()=="King"&& board.find_piece(x+1,y-1)->get_color()!=color)
                {
                    continue;
                }
                else if(x+1<height && board.exist_piece(x+1,y) && board.find_piece(x+1,y)->get_type()=="King"&& board.find_piece(x+1,y)->get_color()!=color)
                {
                    continue;
                }
                else if(x+1 <height && y+1 < width && board.exist_piece(x+1,y+1) && board.find_piece(x+1,y+1)->get_type()=="King"&& board.find_piece(x+1,y+1)->get_color()!=color)
                {
                    continue;
                }
                can_move.push_back(make_pair(x,y));       
            }
        }
        
    }
    
    return can_move;

}

bool is_check(ChessBoard &board, string color) //Check면 True 아니면 False
{
    set<ChessPiece *> piece;
    int check = 0;
    if(color == "Black")
    {
        piece = board.whites();
    }
    else
    {
        piece = board.blacks();
    }
    vector<pair<int,int>> can_move;
    for(auto i : piece)
    {
        /*
        if(i->get_type()=="King")
        {
            continue;
        }
        */
        can_move = get_move(board,i);
        for(auto j : can_move)
        {
            if(board.exist_piece(j.first,j.second))
            {
                if(board.find_piece(j.first,j.second)->get_type()=="King" && board.find_piece(j.first,j.second)->get_color()==color)
                {   
                    check ++;
                    break;
                }
            }
        }
        }
    if(check==0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool is_checkmate(ChessBoard &board, string color) // Checkmate면 True 아니면 False
{
    vector<pair<int,int>> can_move;
    set<ChessPiece *> piece;
    int check = 0;
    if(color == "Black")
    {
        piece = (board).blacks();
    }
    else
    {
        piece = (board).whites();
    }
    for(auto i : piece)
    {
        can_move = get_move(board,i);
        for (auto j : can_move)
        {
            ChessBoard *temp = swap_board(board,i,j.first,j.second);
            if(!is_check(*temp,color))
            {
                check++;
                break;
            }
        }
    }
    if(check==0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
