#pragma once

#include <UltraOOXX/Wrapper/AI.h>
#include <UltraOOXX/UltraBoard.h>
#include <algorithm>
#include <random>
#include <ctime>

class AI : public AIInterface
{
    //Ido
    int x;
    int y;
    TA::BoardInterface::Tag mytag;
    //I_end
public:
    //TODO
    //int x;
    //int y;
    //end
    void init(bool order) override
    {
        // any way
        if(order)this->mytag = TA::BoardInterface::Tag::O;
        else this->mytag = TA::BoardInterface::Tag::X;
    }

    void callbackReportEnemy(int x, int y) override
    {
        //TODO
        this->x = x;
        this->y = y;
        //end

        /*(void) x;
        (void) y;
        // give last step
        */
    }
    std::pair<int,int> queryWhereToPut(TA::UltraBoard MainBoard) override
    {
        //TODO
        std::cout<<"last step:("<<this->x<<" "<<this->y<<")\n";
        int vaildx = x%3;
        int vaildy = y%3;

        std::cout<<"block vaild:("<<vaildx<<" "<<vaildy<<")\n";
        
        int retx=0, rety=0;

        TA::Board& tgtBoard = MainBoard.sub(vaildx, vaildy);
        int points[3][3];

        for(int i=0; i<3; i++){
            for(int j=0; j<3; j++){
                points[i][j]=0;
                if(tgtBoard.state(i, j)==TA::BoardInterface::Tag::None){
                    points[i][j]+=enemyAround(tgtBoard, i, j, this->mytag);
                    //points[i][j]+=allyAround(tgtBoard, i, j, this->mytag);
                    /*if(canBlock(tgtBoard, i, j)){
                        points[i][j]=9;
                    }*///merge with enemyAround();
                    /*if(canConqure(tgtBoard, i, j)){
                        points[i][j]=10;
                    }*/
                }
            }
        }

        //print point for debug
        //some flush problem
        std::cout<<"Points analysis:\n";
        for(int i=0; i<3; i++){
            for(int j=0; j<3; j++){
                std::cout<<points[i][j]<<" ";
            }
            std::cout<<"\n";
        }


        if(!MainBoard.sub(vaildx, vaildy).full()){
            retx = vaildx*3 + rand()%3;
            rety = vaildy*3 + rand()%3;
            while(!MainBoard.isVaild(retx, rety)){
                retx = vaildx*3 + rand()%3;
                rety = vaildy*3 + rand()%3;
            }
            
        }
        else{
            retx = rand()%9;
            rety = rand()%9;
            while(!MainBoard.isVaild(retx, rety)){
                retx = rand()%9;
                rety = rand()%9;
            }
        }


        
        
        std::cout<<"choose:("<<retx<<","<<rety<<")\n";
        return std::make_pair(retx, rety);
        //end
    }

    bool inRange(int x, int y){
        if(x>=0 && x<3 && y>=0 && y<3) return true;
        else return false;
    }
    bool isEnemy(TA::Board tgtBoard, int x, int y, TA::BoardInterface::Tag allytag){
        TA::BoardInterface::Tag enemytag;
        if(allytag == TA::BoardInterface::Tag::O) enemytag = TA::BoardInterface::Tag::X;
        else if(allytag == TA::BoardInterface::Tag::X) enemytag = TA::BoardInterface::Tag::O;
        if(inRange(x,y)){
            if(tgtBoard.state(x,y)==enemytag){
                return true;
            }
            else return false;
        }
        else return false;
    }
    bool isNone(TA::Board tgtBoard, int x, int y){
        if(inRange(x,y)){
            if(tgtBoard.state(x,y)==TA::BoardInterface::Tag::None){
                return true;
            }
            else return false;
        }
        else return false;
    }
    
    int enemyAround(TA::Board tgtBoard, int x, int y, TA::BoardInterface::Tag t){
        //X None X is not considered!
        int enemypnt=1;
        int blockpnt=9;
        int totalpnt=0;
        if(x+y==1 || x+y==3){
            //at cross, check ignore tilt
            if(y==0 && ((isEnemy(tgtBoard, x, y+1, t)&&isNone(tgtBoard, x, y+2)) || 
                (isNone(tgtBoard, x, y+1)&&isEnemy(tgtBoard, x, y+2, t)))){
                totalpnt+=enemypnt;
            }
            else if(y==0 && isEnemy(tgtBoard, x, y+1, t) && isEnemy(tgtBoard, x, y+2, t)){
                totalpnt+=blockpnt;
            }

            if(y==1 && ((isEnemy(tgtBoard, x, y-1, t)&&isNone(tgtBoard, x, y+1)) || 
                (isNone(tgtBoard, x, y-1)&&isEnemy(tgtBoard, x, y+1, t)))){
                totalpnt+=enemypnt;
            }
            else if(y==1 && isEnemy(tgtBoard, x, y-1, t) && isEnemy(tgtBoard, x, y+1, t)){
                totalpnt+=blockpnt;
            }

            if(y==2 && ((isEnemy(tgtBoard, x, y-2, t)&&isNone(tgtBoard, x, y-1)) || 
                (isNone(tgtBoard, x, y-2)&&isEnemy(tgtBoard, x, y-1, t)))){
                totalpnt+=enemypnt;
            }
            else if(y==2 && isEnemy(tgtBoard, x, y-2, t) && isEnemy(tgtBoard, x, y-1, t)){
                totalpnt+=blockpnt;
            }

            if(x==0 && ((isEnemy(tgtBoard, x+1, y, t)&&isNone(tgtBoard, x+2, y)) || 
                (isNone(tgtBoard, x+1, y)&&isEnemy(tgtBoard, x+2, y, t)))){
                totalpnt+=enemypnt;
            }
            else if(x==0 && isEnemy(tgtBoard, x+1, y, t) && isEnemy(tgtBoard, x+2, y, t)){
                totalpnt+=blockpnt;
            }

            if(x==1 && ((isEnemy(tgtBoard, x-1, y, t)&&isNone(tgtBoard, x+1, y)) || 
                (isNone(tgtBoard, x-1, y)&&isEnemy(tgtBoard, x+1, y, t)))){
                totalpnt+=enemypnt;
            }
            else if(x==1 && isEnemy(tgtBoard, x-1, y, t) && isEnemy(tgtBoard, x+1, y, t)){
                totalpnt+=blockpnt;
            }

            if(x==2 && ((isEnemy(tgtBoard, x-2, y, t)&&isNone(tgtBoard, x-1, y)) || 
                (isNone(tgtBoard, x-2, y)&&isEnemy(tgtBoard, x-1, y, t)))){
                totalpnt+=enemypnt;
            }
            else if(x==2 && isEnemy(tgtBoard, x-2, y, t) && isEnemy(tgtBoard, x-1, y, t)){
                totalpnt+=blockpnt;
            }

            /*if(isEnemy(tgtBoard, x, y+1, t)){
                totalpnt+=enemypnt;
                if(isEnemy(tgtBoard, x, y+2, t))totalpnt+=blockpnt;
            }
            if(isEnemy(tgtBoard, x+1, y, t)){
                totalpnt+=enemypnt;
                if(isEnemy(tgtBoard, x+2, y, t))totalpnt+=blockpnt;
            }
            if(isEnemy(tgtBoard, x, y-1, t)){
                totalpnt+=enemypnt;
                if(isEnemy(tgtBoard, x, y-2, t))totalpnt+=blockpnt;
            }
            if(isEnemy(tgtBoard, x-1, y, t)){
                totalpnt+=enemypnt;
                if(isEnemy(tgtBoard, x-2, y, t))totalpnt+=blockpnt;
            }*/
        }
        else if(x+y==0 || x+y==2 || x+y==4){
            //corner and middle, check all
            //cross
            if(y==0 && ((isEnemy(tgtBoard, x, y+1, t)&&isNone(tgtBoard, x, y+2)) || 
                (isNone(tgtBoard, x, y+1)&&isEnemy(tgtBoard, x, y+2, t)))){
                totalpnt+=enemypnt;
            }
            else if(y==0 && isEnemy(tgtBoard, x, y+1, t) && isEnemy(tgtBoard, x, y+2, t)){
                totalpnt+=blockpnt;
            }

            if(y==1 && ((isEnemy(tgtBoard, x, y-1, t)&&isNone(tgtBoard, x, y+1)) || 
                (isNone(tgtBoard, x, y-1)&&isEnemy(tgtBoard, x, y+1, t)))){
                totalpnt+=enemypnt;
            }
            else if(y==1 && isEnemy(tgtBoard, x, y-1, t) && isEnemy(tgtBoard, x, y+1, t)){
                totalpnt+=blockpnt;
            }

            if(y==2 && ((isEnemy(tgtBoard, x, y-2, t)&&isNone(tgtBoard, x, y-1)) || 
                (isNone(tgtBoard, x, y-2)&&isEnemy(tgtBoard, x, y-1, t)))){
                totalpnt+=enemypnt;
            }
            else if(y==2 && isEnemy(tgtBoard, x, y-2, t) && isEnemy(tgtBoard, x, y-1, t)){
                totalpnt+=blockpnt;
            }

            if(x==0 && ((isEnemy(tgtBoard, x+1, y, t)&&isNone(tgtBoard, x+2, y)) || 
                (isNone(tgtBoard, x+1, y)&&isEnemy(tgtBoard, x+2, y, t)))){
                totalpnt+=enemypnt;
            }
            else if(x==0 && isEnemy(tgtBoard, x+1, y, t) && isEnemy(tgtBoard, x+2, y, t)){
                totalpnt+=blockpnt;
            }

            if(x==1 && ((isEnemy(tgtBoard, x-1, y, t)&&isNone(tgtBoard, x+1, y)) || 
                (isNone(tgtBoard, x-1, y)&&isEnemy(tgtBoard, x+1, y, t)))){
                totalpnt+=enemypnt;
            }
            else if(x==1 && isEnemy(tgtBoard, x-1, y, t) && isEnemy(tgtBoard, x+1, y, t)){
                totalpnt+=blockpnt;
            }

            if(x==2 && ((isEnemy(tgtBoard, x-2, y, t)&&isNone(tgtBoard, x-1, y)) || 
                (isNone(tgtBoard, x-2, y)&&isEnemy(tgtBoard, x-1, y, t)))){
                totalpnt+=enemypnt;
            }
            else if(x==2 && isEnemy(tgtBoard, x-2, y, t) && isEnemy(tgtBoard, x-1, y, t)){
                totalpnt+=blockpnt;
            }
            
            //tilt
            if((isEnemy(tgtBoard, x+1, y+1, t)&&isNone(tgtBoard, x+2, y+2))||
                (isNone(tgtBoard, x+1, y+1)&&isEnemy(tgtBoard, x+2, y+2, t))){
                totalpnt+=enemypnt;
            }
            else if(isEnemy(tgtBoard, x+1, y+1, t)&&isEnemy(tgtBoard, x+2, y+2, t)){
                totalpnt+=blockpnt;
            }

            if((isEnemy(tgtBoard, x+1, y-1, t)&&isNone(tgtBoard, x+2, y-2))||
                (isNone(tgtBoard, x+1, y-1)&&isEnemy(tgtBoard, x+2, y-2, t))){
                totalpnt+=enemypnt;
            }
            else if(isEnemy(tgtBoard, x+1, y-1, t)&&isEnemy(tgtBoard, x+2, y-2, t)){
                totalpnt+=blockpnt;
            }

            if((isEnemy(tgtBoard, x-1, y+1, t)&&isNone(tgtBoard, x-2, y+2))||
                (isNone(tgtBoard, x-1, y+1)&&isEnemy(tgtBoard, x-2, y+2, t))){
                totalpnt+=enemypnt;
            }
            else if(isEnemy(tgtBoard, x-1, y+1, t)&&isEnemy(tgtBoard, x-2, y+2, t)){
                totalpnt+=blockpnt;
            }

            if((isEnemy(tgtBoard, x-1, y-1, t)&&isNone(tgtBoard, x-2, y-2))||
                (isNone(tgtBoard, x-1, y-1)&&isEnemy(tgtBoard, x-2, y-2, t))){
                totalpnt+=enemypnt;
            }
            else if(isEnemy(tgtBoard, x-1, y-1, t)&&isEnemy(tgtBoard, x-2, y-2, t)){
                totalpnt+=blockpnt;
            }

            if((isEnemy(tgtBoard, x+1, y+1, t)&&isNone(tgtBoard, x-1, y-1))||
                (isNone(tgtBoard, x+1, y+1)&&isEnemy(tgtBoard, x-1, y-1, t))){
                totalpnt+=enemypnt;
            }
            else if(isEnemy(tgtBoard, x+1, y+1, t)&&isEnemy(tgtBoard, x-1, y-1, t)){
                totalpnt+=blockpnt;
            }

            if((isEnemy(tgtBoard, x+1, y-1, t)&&isNone(tgtBoard, x-1, y+1))||
                (isNone(tgtBoard, x+1, y-1)&&isEnemy(tgtBoard, x-1, y+1, t))){
                totalpnt+=enemypnt;
            }
            else if(isEnemy(tgtBoard, x+1, y-1, t)&&isEnemy(tgtBoard, x-1, y+1, t)){
                totalpnt+=blockpnt;
            }

            /*if(isEnemy(tgtBoard, x, y+1, t)){
                totalpnt+=enemypnt;
                if(isEnemy(tgtBoard, x, y+2, t))totalpnt+=blockpnt;
            }
            if(isEnemy(tgtBoard, x+1, y+1, t)){
                totalpnt+=enemypnt;
                if(isEnemy(tgtBoard, x+2, y+2, t))totalpnt+=blockpnt;
            }
            if(isEnemy(tgtBoard, x+1, y, t)){
                totalpnt+=enemypnt;
                if(isEnemy(tgtBoard, x+2, y, t))totalpnt+=blockpnt;
            }
            if(isEnemy(tgtBoard, x+1, y-1, t)){
                totalpnt+=enemypnt;
                if(isEnemy(tgtBoard, x+2, y-2, t))totalpnt+=blockpnt;
            }
            if(isEnemy(tgtBoard, x, y-1, t)){
                totalpnt+=enemypnt;
                if(isEnemy(tgtBoard, x, y-2, t))totalpnt+=blockpnt;
            }
            if(isEnemy(tgtBoard, x-1, y-1, t)){
                totalpnt+=enemypnt;
                if(isEnemy(tgtBoard, x-2, y-2, t))totalpnt+=blockpnt;
            }
            if(isEnemy(tgtBoard, x-1, y, t)){
                totalpnt+=enemypnt;
                if(isEnemy(tgtBoard, x-2, y, t))totalpnt+=blockpnt;
            }
            if(isEnemy(tgtBoard, x-1, y+1, t)){
                totalpnt+=enemypnt;
                if(isEnemy(tgtBoard, x-2, y+2, t))totalpnt+=blockpnt;
            }*/
        }
        return totalpnt;
    }
    int allyAround(TA::Board tgtBoard, int x, int y, TA::BoardInterface::Tag t){
        int ally=0;

        return ally;
    }
    bool canBlock(TA::Board tgtBoard, int x, int y){
        bool ans = false;

        return ans;
    }
    bool canConqure(TA::Board tgtBoard, int x, int y){
        bool ans = false;

        return ans;
    }
};
