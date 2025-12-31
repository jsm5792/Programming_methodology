#pragma once
/*
 * board.h
 */
#ifndef CHESSBOARD_H_
#define CHESSBOARD_H_

#include <iostream>
#include <fstream>
#include <string>
#include <tuple>
#include <vector>
#include <algorithm>
#include <cmath>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <cctype>

#include "utils.h"
#include "chesspiece.h"

using namespace std;


class ChessBoard
{
public:
    void initialize_board();
    
    ChessBoard(int width, int height): width(width), height(height) { initialize_board(); }

    ChessBoard& operator=(const ChessBoard& other);
    
    friend ostream& operator<<(ostream& os, const ChessBoard& board);
    ChessPiece* operator[](string s);
    int get_width()
    {
        return width;
    }
    int get_height()
    {
        return height;
    }

    ChessBoard(const ChessBoard& other);
    void place_piece(ChessPiece *piece)
    {
        tiles[piece->get_x()][piece->get_y()] = piece;
        if(piece->get_color()=="Black")
        {
            black_pieces.insert(piece);
        }
        else
        {
            white_pieces.insert(piece);
        }
    }
    bool exist_piece(int x,int y)
    {
        if (tiles[x][y]==nullptr)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    ChessPiece* find_piece(int x,int y)
    {
        return tiles[x][y];
    }
    set<ChessPiece*> whites()
    {
        return white_pieces;
    }
    set<ChessPiece*> blacks()
    {
        return black_pieces;
    }
    
    void swapTile(ChessPiece *piece, int x, int y)
    {
        ChessPiece *piece1;
        if(piece->get_color()=="White")
        {
            for(auto i = white_pieces.begin();i != white_pieces.end();)
            {
                if((*i)->get_x()==piece->get_x() && (*i)->get_y()==piece->get_y())
                {
                    white_pieces.erase(i++);
                }
                else
                {
                    i++;
                }
            }
            ChessPiece* target_piece = find_piece(x, y);
            tiles[piece->get_x()][piece->get_y()]=nullptr;
            for(auto i = black_pieces.begin();i != black_pieces.end();)
            {
                if((*i)->get_x()==x && (*i)->get_y()==y)
                {
                    black_pieces.erase(i++);
                }
                else
                {
                    i++;
                }
            }
            tiles[x][y] = create_piece(piece->get_color(),piece->get_type(),x,y);
            piece1 = create_piece("White",piece->get_type(),x,y);
            white_pieces.insert(piece1);
        }
        else
        {
            for(auto i = black_pieces.begin();i != black_pieces.end();)
            {
                if((*i)->get_x()==piece->get_x() && (*i)->get_y()==piece->get_y())
                {
                    black_pieces.erase(i++);
                }
                else
                {
                    i++;
                }
            }
            tiles[piece->get_x()][piece->get_y()]=nullptr;
            for(auto i = white_pieces.begin();i != white_pieces.end();)
            {
                if((*i)->get_x()==x && (*i)->get_y()==y)
                {
                    white_pieces.erase(i++);
                }
                else
                {
                    i++;
                }
            }
            tiles[x][y] = create_piece(piece->get_color(),piece->get_type(),x,y);
            piece1 = create_piece("Black",piece->get_type(),x,y);
            black_pieces.insert(piece1);
        }
    }


    // NOTE: YOU CAN ADD MORE MEMBER FUNCTIONS HERE

private: 
    int width, height;
    ChessPiece*** tiles;
    set<ChessPiece*> white_pieces;
    set<ChessPiece*> black_pieces;

    // NOTE: YOU CAN ADD MORE MEMBER VARIABLESS HERE

};

// NOTE: YOU CAN ADD MORE FUNCTIONS HERE
ChessBoard* swap_board(ChessBoard &board, ChessPiece *Piece, int x, int y);

vector<pair<int,int>> get_move(ChessBoard &board, ChessPiece *piece);

bool is_check(ChessBoard &board, string color);

bool is_checkmate(ChessBoard &board, string color);

#endif /* CHESSBOARD_H_ */