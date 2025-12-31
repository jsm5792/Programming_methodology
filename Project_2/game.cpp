#include "game.h"

using namespace std;

void Game::read_file(string filename)
{
    ifstream ifs(filename);
    if (!ifs)
    {
        cerr << "Error: file open failed" << endl;
        exit(1);
    }
    ifs >> problem_id;
    if(problem_id==0)
    {
        problem_specification = "";
    }
    else
    {
        ifs >> problem_specification;
    }

    cout << "problem_id: [" << problem_id << "]" << endl;
    cout << "problem_specification: [" << problem_specification << "]" << endl;

    // TODO: CREATE CHESSBOARD
    int N, M;
    ifs >> N;
    ifs.ignore();
    ifs >> M;
    board = new ChessBoard(M, N);
    ifs.ignore();
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<M;j++)
        {
            char c;
            ifs >> c;
            if(c!='.')
            {
                if(isupper(c))
                {
                    board->place_piece(create_piece("White",char_to_piece[c],i,j));
                }
                else
                {
                    board->place_piece(create_piece("Black",char_to_piece[toupper(c)],i,j));
                }
            }
        }
        ifs.ignore();
    }
    ifs.close();

}

void Game::open_output_file(string filename)
{
    ofs.open(filename);
    if (!ofs)
    {
        cerr << "Error: file open failed" << endl;
        exit(1);
    }
}

void Game::solve()
{
    switch (problem_id)
    {
    case 0:
        solve_print_board();
        break;
    case 1:
        solve_print_possible_moves();
        break;
    case 2:
        solve_check();
        break;
    case 3:
        solve_checkmate();
        break;
    case 4:
        solve_checkmate_in_one_move();
        break;
    default:
        cout << "Invalid problem_id" << endl;
        break;
    }
    return;
}

void Game::solve_print_board()
{
    ofs << *board;
}

void Game::solve_print_possible_moves()
{
    // TODO
    string color1 = (*board)[problem_specification]->get_color();
    vector<pair<int,int>> can_move;
    int height = (*board).get_height();
    int width = (*board).get_width();
    ChessPiece *piece =(*board)[problem_specification];
    vector<tuple<MoveType, Direction, int>> moves = piece->get_possible_moves();

    for(const auto &itr : moves)
    {
        
        Direction dir1 = get<1>(itr);
        int x = (*board)[problem_specification]->get_x();
        int y = (*board)[problem_specification]->get_y();
        if((*board)[problem_specification]->get_type()=="Queen"|| (*board)[problem_specification]->get_type() == "Rook" || (*board)[problem_specification]->get_type() == "Bishop")
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
                if((*board).exist_piece(x,y))
                {
                    if((*board).find_piece(x,y)->get_color()==color1)
                    {
                        break;
                    }
                    else
                    {
                        can_move.push_back(make_pair(x,y));
                        (*board).find_piece(x,y)->set_flag(1);
                        break;
                    }
                }
                else
                {
                    can_move.push_back(make_pair(x,y));               
                }
            }
        }
        else if((*board)[problem_specification]->get_type()=="Pawn")
        {
            if((*board)[problem_specification]->get_color()=="Black")
            {
                if(x==1)
                {
                    if(x+2<height&& !(*board).exist_piece(x+2,y) && !(*board).exist_piece(x+1,y))
                    {
                        can_move.push_back(make_pair(x+2,y));
                    }
                }
                if(true)
                {
                    if(x+1<height && y-1 >=0 && (*board).exist_piece(x+1,y-1))
                    {
                        if((*board).find_piece(x+1,y-1)->get_color()!="Black")
                        {
                            can_move.push_back(make_pair(x+1,y-1));
                            (*board).find_piece(x+1,y-1)->set_flag(1);
                        }
                    }
                    if(x+1<height && y+1 < width && (*board).exist_piece(x+1,y+1))
                    {
                        if((*board).find_piece(x+1,y+1)->get_color()!="Black")
                        {
                            can_move.push_back(make_pair(x+1,y+1));
                            (*board).find_piece(x+1,y+1)->set_flag(1);
                        }
                    }
                    if(x+1<height && !(*board).exist_piece(x+1,y))
                    {
                        can_move.push_back(make_pair(x+1,y));
                    }
                }
            }
            else
            {
                if(x==height-2)
                {
                    if(x-2>=0&& !(*board).exist_piece(x-2,y) && !(*board).exist_piece(x-1,y))
                    {
                        can_move.push_back(make_pair(x-2,y));
                    }
                }
                if(true)
                {
                    if(x-1>=0 && y-1 >=0 && (*board).exist_piece(x-1,y-1))
                    {
                        if((*board).find_piece(x-1,y-1)->get_color()=="Black")
                        {
                            can_move.push_back(make_pair(x-1,y-1));
                            (*board).find_piece(x-1,y-1)->set_flag(1);
                        }
                    }
                    if(x-1>=0 && y+1 < width && (*board).exist_piece(x-1,y+1))
                    {
                        if((*board).find_piece(x-1,y+1)->get_color()=="Black")
                        {
                            can_move.push_back(make_pair(x-1,y+1));
                            (*board).find_piece(x-1,y+1)->set_flag(1);
                        }

                    }
                    if(x-1>=height && !(*board).exist_piece(x-1,y))
                    {
                        can_move.push_back(make_pair(x-1,y));
                    }
                }
                
            }
        }

        else if((*board)[problem_specification]->get_type()=="Knight"||(*board)[problem_specification]->get_type()=="King")
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
            else if((*board).exist_piece(x,y))
            {
                if((*board).find_piece(x,y)->get_color()==color1)
                {
                    continue;
                }
                else
                {
                    can_move.push_back(make_pair(x,y));
                    (*board).find_piece(x,y)->set_flag(1);
                    continue;
                }

            }
            else
            {
                can_move.push_back(make_pair(x,y));               
            }

        }
        
    }
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            if(i ==(*board)[problem_specification]->get_x() && j == (*board)[problem_specification]->get_y())
            {
                if(color1=="Black")
                {
                    char s = piece_to_char[(*board).find_piece(i,j)->get_type()];
                    ofs << static_cast<char>(tolower(s));
                }
                else
                {
                    ofs << piece_to_char[(*board).find_piece(i,j)->get_type()];
                }

                
            }
            else if((*board).exist_piece(i,j))
            {
                if((*board).find_piece(i,j)->get_flag()==1)
                {
                    ofs << "x";
                }
                else
                {
                    ofs << ".";
                }
            }
            else
            {
                int check = 0;
                for(auto k : can_move)
                {
                    if(k.first==i && k.second==j)
                    {
                        ofs << "o";
                        check ++;
                    }
                }
                if(check==0)
                {
                    ofs << ".";
                }
            }
        }
        ofs << "\n";
    }

}

int Game::solve_check()
{
    // TODO
    // EXAMPLE: "Black king is in check"
    //          "White king is not in check"
    if(problem_specification=="b")
    {
        if(is_check(*board,"Black"))
        {
            ofs << "Black king is in check";
        }
        else
        {
            ofs << "Black king is not in check";
        }
    }
    else
    {
        if(is_check(*board,"White"))
        {
            ofs << "White king is in check";
        }
        else
        {
            ofs << "White king is not in check";
        }

    }
    return 0;
}

int Game::solve_checkmate()
{
    // TODO 
    // EXAMPLE: "Black king is checkmated"
    //          "White king is not checkmated"
    

    if(problem_specification=="b")
    {
        if(is_checkmate(*board,"Black") && is_check(*board,"Black"))
        {
            ofs << "Black king is checkmated";
        }
        else
        {
            ofs << "Black king is not checkmated";
        }
    }
    else
    {
        if(is_checkmate(*board,"White") && is_check(*board,"White"))
        {
            ofs << "White king is checkmated";
        }
        else
        {
            ofs << "White king is not checkmated";
        }

    }
    return 0;

}

int Game::solve_checkmate_in_one_move()
{
    // TODO
    // EXAMPLE: "Black king is checkmated in one move"
    //          "White king is not checkmated in one move"
    set <ChessPiece *> piece;
    int check=0;
    vector<pair<int,int>> possible_moves;

    if(problem_specification=="b")
    {
        piece = (*board).whites();
        for(auto i : piece)
        {
            possible_moves = get_move(*board,i);
            for (auto j : possible_moves)
            {
                ChessBoard *temp = swap_board(*board,i,j.first,j.second);
                if((!is_check(*temp, "White")))
                {
                    if(is_checkmate(*temp,"Black") && is_check(*temp,"Black"))
                    {
                        check++;
                        break;
                    }
                }
                delete temp;
            }
        }
        if(check!=0)
        {
            ofs << "Black king is checkmated in one move";
        }
        else
        {
            ofs << "Black king is not checkmated in one move";
        }

    }
    else
    {
        piece = (*board).blacks();
        for(auto i : piece)
        {
            possible_moves = get_move(*board,i);
            for (auto j : possible_moves)
            {
                ChessBoard *temp = swap_board(*board,i,j.first,j.second);
                if((!is_check(*temp, "Black")))
                {
                    if(is_checkmate(*temp,"White") && is_check(*temp,"White"))
                    {
                        check++;
                        break;
                    }
                }
                delete temp;
            }
        }
        if(check!=0)
        {
            ofs << "White king is checkmated in one move";
        }
        else
        {
            ofs << "White king is not checkmated in one move";
        }
    }
    return 0;
}

