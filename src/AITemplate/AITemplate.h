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
        std::cout<<"last step:"<<this->x<<" "<<this->y<<std::endl;
        int vaildx = x%3;
        int vaildy = y%3;

        std::cout<<"block vaild:"<<vaildx<<" "<<vaildy<<std::endl;
        
        int retx=0, rety=0;

        TA::Board& tgtBoard = MainBoard.sub(vaildx, vaildy);
        int points[3][3];

        for(int i=0; i<3; i++){
            for(int j=0; j<3; j++){
                points[i][j]=0;

                points[i][j]+=enemyAround(tgtBoard, i, j, this->mytag);
                points[i][j]+=allyAround(tgtBoard, i, j, this->mytag);
                if(canBlock(tgtBoard, i, j)){
                    points[i][j]=9;
                }
                if(canConqure(tgtBoard, i, j)){
                    points[i][j]=10;
                }
            }
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


        
        
        std::cout<<"choose:"<<retx<<","<<rety<<"\n";
        return std::make_pair(retx, rety);
        //end

        /*return std::make_pair(1,2);
        */
        std::cout<<"choose:"<<retx<<","<<rety<<"\n";
        return std::make_pair(retx, rety);
    }

    bool inRange(int x, int y){
        if(x>=0 && x<3 && y>=0 && y<3) return true;
        else return false;
    }
    int enemyAround(TA::Board tgtBoard, int x, int y, TA::BoardInterface::Tag t){
        int enemies=0;
        if(inRange(x, y+1)){
            
        }
        if(inRange(x+1, y+1)){
            
        }
        if(inRange(x+1, y)){
            
        }
        if(inRange(x+1, y-1)){
            
        }
        if(inRange(x, y-1)){
            
        }
        if(inRange(x-1, y-1)){
            
        }
        if(inRange(x-1, y)){
            
        }
        if(inRange(x-1, y+1)){
            
        }
        return enemies;
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
