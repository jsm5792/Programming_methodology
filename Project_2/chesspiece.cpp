#include "chesspiece.h"

using namespace std;


ChessPiece::ChessPiece(string color, string type, int x, int y, int flag)
{
    this->color = color;
    this->type = type;
    this->x = x;
    this->y = y;
    this->flag = flag;
}

ostream& operator<<(ostream& os, const ChessPiece& piece)
{
    char c;
    c = piece_to_char[piece.type];
    if (piece.color == "Black")
    {
        c = tolower(c);
    }
    os << c;
    return os;
}

ChessPiece::ChessPiece(const ChessPiece& other)
{
    this->color = other.color;
    this->type = other.type;
    this->x = other.x;
    this->y = other.y;
    this->flag = other.flag;
}

ChessPiece* create_piece(string color, string type, int x, int y, int flag)
{
    // TODO : implement create_piece
    ChessPiece *piece = NULL;
    if(type == "King")
    {
        piece = new King(color,x,y,flag);
    }
    else if(type == "Queen")
    {
        piece = new Queen(color,x,y,flag);
    }
    else if(type == "Rook")
    {
        piece = new Rook(color,x,y,flag);
    }
    else if(type == "Bishop")
    {
        piece = new Bishop(color,x,y,flag);
    }
    else if(type == "Knight")
    {
        piece = new Knight(color,x,y,flag);
    }
    else if(type == "Pawn")
    {
        piece = new Pawn(color,x,y,flag);
    }
    else
    {
        piece = new Other(color,type,x,y,flag);
    }
    return piece;
    
}

// NOTE: YOU CAN ADD OTHER FUNCTIONS HERE

ChessPiece* create_piece(ChessPiece *piece){
    ChessPiece *piece1 = NULL;

    if(piece->get_type() == "King")
    {
        piece1 = new King(piece->get_color(), piece->get_x(), piece->get_y(), piece->get_flag());
    }
    else if(piece->get_type() == "Queen")
    {
        piece1 = new Queen(piece->get_color(), piece->get_x(), piece->get_y(), piece->get_flag());
    }
    else if(piece->get_type() == "Rook")
    {
        piece1 = new Rook(piece->get_color(), piece->get_x(), piece->get_y(), piece->get_flag());
    }
    else if(piece->get_type() == "Bishop")
    {
        piece1 = new Bishop(piece->get_color(), piece->get_x(), piece->get_y(), piece->get_flag());
    }
    else if(piece->get_type() == "Knight")
    {
        piece1 = new Knight(piece->get_color(), piece->get_x(), piece->get_y(), piece->get_flag());
    }
    else if(piece->get_type() == "Pawn")
    {
        piece1 = new Pawn(piece->get_color(), piece->get_x(), piece->get_y(), piece->get_flag());
    }
    else
    {
        piece1 = new Other(piece->get_color(), piece->get_type(), piece->get_x(), piece->get_y(), piece->get_flag());
    }
    return piece1;
}