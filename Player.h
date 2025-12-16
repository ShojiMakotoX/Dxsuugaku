#pragma once
#include "Base.h"

class Player :
    public Base
{
public:
    //コンストラクタ
    Player();
    Player(const Vector2D& pos, const Vector2D& vel, unsigned int color);
    ~Player();

    //override類
    virtual void Update(); //オーバーライドしそう
    virtual void Draw();

private:
    //独自のメンバ
    
    //自分の向き（ベクトル）
    int muki_;
    //3角形の3点の座標
    int baseWidth_;//底辺の幅
    int height_;//高さ

};

