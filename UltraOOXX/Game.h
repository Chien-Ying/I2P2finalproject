#pragma once

#include <UltraOOXX/UltraBoard.h>
#include <UltraOOXX/Wrapper/AI.h>
#include <GUI/GUIInterface.h>

#include <iostream>
#include <cassert>
#include <chrono>
#include <cstdarg>
#include <future>
#include <type_traits>

namespace TA
{
    class UltraOOXX
    {
    public:
        UltraOOXX(
            std::chrono::milliseconds runtime_limit = std::chrono::milliseconds(1000)
        ):
            m_runtime_limit(runtime_limit),
            m_P1(nullptr),
            m_P2(nullptr),
            MainBoard()
        {
            gui = new ASCII;
        }

        void setPlayer1(AIInterface *ptr) { assert(checkAI(ptr)); m_P1 = ptr; }
        void setPlayer2(AIInterface *ptr) { assert(checkAI(ptr)); m_P2 = ptr; }

        void run()
        {
            gui->title();
            int round = 0;
            if( !prepareState() ) return ;

            //Todo: Play Game
            putToGui("Hello world %d\n", 123);
            updateGuiGame();

            while (!checkGameover()) {          
                round++;
                //TODO
                
                //end
                AIInterface *first = nullptr;
                AIInterface *second = nullptr;
                BoardInterface::Tag tag = BoardInterface::Tag::O;

                //TODO
                if(round%2==1){
                    first = m_P1;
                    second = m_P2;
                    tag = BoardInterface::Tag::O;
                }
                else if(round%2==0){
                    first = m_P2;
                    second = m_P1;
                    tag = BoardInterface::Tag::X;
                }

                std::cout<<"Round:"<<round<<" Player:"<<(round%2? "O":"X")<<"\n";

                if (!playOneRound(first, tag, second)) {
                    std::cout<<"This is the end?\n";
                }
                updateGuiGame();

                system("read -p 'Press Enter to continue...' var");
                //end

                /*if (!playOneRound(first, tag, second)) {
                    
                }
                updateGuiGame();*/
            } 
        } 

   private:
        void updateGuiGame()
        {
            gui->updateGame(MainBoard);
        }

        bool playOneRound(AIInterface *user, BoardInterface::Tag tag, AIInterface *enemy)
        {
            auto pos = call(&AIInterface::queryWhereToPut, user, MainBoard);
            //TODO
            //std::cout<<pos.first<<"!!!"<<pos.second<<"!!!\n";
            
            enemy->callbackReportEnemy(pos.first,pos.second);
            MainBoard.set(pos.first, pos.second, tag);

            //Ido
            MainBoard.sub(pos.first/3, pos.second/3).determineWin(pos.first%3, pos.second%3);
            MainBoard.determineWin(pos.first/3, pos.second/3);
        //I_end
            //end
            return true;
        }

        bool checkGameover()
        {
            //TODO
            if(MainBoard.getWinTag() == TA::BoardInterface::Tag::X){
                std::cout<<"X WIN!GAMEOVER!!!\n";
                return true;
            }
            else if(MainBoard.getWinTag() == TA::BoardInterface::Tag::O){
                std::cout<<"O WIN!GAMEOVER!!!\n";
                return true;
            }
            else if(MainBoard.getWinTag() == TA::BoardInterface::Tag::Tie){
                std::cout<<"Tie!GAMEOVER!!!\n";
                return true;
            }
            else if(MainBoard.full()){
                std::cout<<"FULL!GAMEOVER!!!\n";
                return true;
            }
            else return false;
            //end
            /*return true; // Gameover!*/
        }

        bool prepareState()
        {
            call(&AIInterface::init, m_P1, true);
            call(&AIInterface::init, m_P2, false);
            return true;
        }

        template<typename Func ,typename... Args, 
            std::enable_if_t< std::is_void<
                    std::invoke_result_t<Func, AIInterface, Args...>
                >::value , int> = 0 >
        void call(Func func, AIInterface *ptr, Args... args)
        {
            std::future_status status;
            auto val = std::async(std::launch::async, func, ptr, args...);
            status = val.wait_for(std::chrono::milliseconds(m_runtime_limit));

            if( status != std::future_status::ready )
            {
                exit(-1);
            }
            val.get();
        }

        template<typename Func ,typename... Args, 
            std::enable_if_t< std::is_void<
                    std::invoke_result_t<Func, AIInterface, Args...>
                >::value == false, int> = 0 >
        auto call(Func func, AIInterface *ptr, Args... args)
            -> std::invoke_result_t<Func, AIInterface, Args...>
        {
            std::future_status status;
            auto val = std::async(std::launch::async, func, ptr, args...);
            status = val.wait_for(std::chrono::milliseconds(m_runtime_limit));

            if( status != std::future_status::ready )
            {
                exit(-1);
            }
            return val.get();
        }

        void putToGui(const char *fmt, ...)
        {
            va_list args1;
            va_start(args1, fmt);
            va_list args2;
            va_copy(args2, args1);
            std::vector<char> buf(1+std::vsnprintf(nullptr, 0, fmt, args1));
            va_end(args1);
            std::vsnprintf(buf.data(), buf.size(), fmt, args2);
            va_end(args2);

            if( buf.back() == 0 ) buf.pop_back();
            gui->appendText( std::string(buf.begin(), buf.end()) );
        }

        bool checkAI(AIInterface *ptr) 
        {
            return ptr->abi() == AI_ABI_VER;
        }

        int m_size;
        std::vector<int> m_ship_size;
        std::chrono::milliseconds m_runtime_limit;

        AIInterface *m_P1;
        AIInterface *m_P2;
        GUIInterface *gui;

        UltraBoard MainBoard;
    };
}