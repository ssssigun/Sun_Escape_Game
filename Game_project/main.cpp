#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <fstream>
using namespace sf;
using namespace std;
class User
{
    double x,y, firstX, firstY;
    static int life;
    Texture s;
    Sprite SiGwon;

public:
    User(double gameX=0, double gameY=0)
    {
        x = firstX =gameX;
        y = firstY =gameY;
        s.loadFromFile("star.png");
        SiGwon.setTexture(s);
        SiGwon.setPosition(x, y);
        };

    double getX() { return x; }
    double getY() { return y; }
    int getL() { return life; }
    void setX(double gameX) { x = gameX; }
    void setY(double gameY) { y = gameY; }

    void draw(RenderWindow &window)
    {
        window.draw(SiGwon);
    }
    
    void downlife()
    {
        x = firstX;
        y = firstY;
        life--;

        SiGwon.setPosition(x, y);                                                                                       //////////////////////////////
    }

    void update(double gameX, double gameY)
    {
        x += gameX;
        y += gameY;
   
        SiGwon.setPosition(x, y);
    }

    User& operator=(const User& u1)                                         //대입 연산자 중복
    {
        this->x = u1.x;
        this->y = u1.y;
        this->firstX = u1.firstX;
        this->firstY = u1.firstY;
        this->life = u1.life;
        this->s = u1.s;
        this->SiGwon = u1.SiGwon;
        s.loadFromFile("star.png");
        SiGwon.setTexture(s);
        return *this;
    }
};

int User::life = 10;                                                    //정적 변수 사용

class Hurdle 
{
    double x, y;
    double dis = 0;
    Texture t;
    Sprite SungMin;

public:
    Hurdle(double gameX = 0, double gameY = 0)
    {
        x = gameX;
        y = gameY;
        t.loadFromFile("planet.png");
        SungMin.setTexture(t);
        SungMin.setPosition(x, y);
    };
  
    void draw(RenderWindow& window)
    {
        window.draw(SungMin);
    }

    void update(double gameX, double gameY)
    {
        x += gameX;
        y += gameY;
        SungMin.setPosition(x, y);
    }

    double getX() { return x; }
    double getY() { return y; }
    void setX(double gameX) { x = gameX; }
    void setY(double gameY) { y = gameY; }
    void crash(User& user)
    {
        dis = sqrt(pow((user.getX() - x), 2) + pow((user.getY() - y), 2));
 
        if (dis < 50)                ///////////////////////////
        {
            user.downlife();
        }
    }
};

class Point
{
    double x, y;
    double dis= 0;
    static int score;
    Texture t;
    Sprite hannam;
    bool collision = true;
    bool sc = false;

public:

    Point(double gameX = 0, double gameY = 0)
    {
        x = gameX;
        y = gameY;
        t.loadFromFile("meteor.png");
        hannam.setTexture(t);
        hannam.setPosition(x, y);
    };

    void draw(RenderWindow& window)
    {
        if (collision)
        {
            window.draw(hannam);
        }

    }
    double getX() { return x; }
    double getY() { return y; }
    int getS() { return score; }
    bool getC() { return collision; }
    void setX(double gameX) { x = gameX; }
    void setY(double gameY) { y = gameY; }
    void setS(int s) { score = s; }


    void crash(User& user)
    {
        dis = sqrt(pow((user.getX() - x), 2) + pow((user.getY() - y), 2));

        if (dis < 50)                
        {
            if (dis < 50 && collision == true)
            {
                score += 500;
            }
            collision = false;
        }
    }
};

int Point::score = 0;                                                   //정적 변수 사용

class Game
{
    Font f;
    Text stageT, scoreT,lifeT, stageN, scoreN, lifeN;
    String Life;
    const int Fontsize = 28;
    int score,life;
    char board[30];
    static int stage;

public:
    Game(User user1=0, Point user2 =0)
    {
        MakeText("Stage : ", stageT, 1168, 25);
        MakeText("Score : ", scoreT, 1168, 55);
        MakeText("Life : ", lifeT,1198, 85);

        MakeText(to_string(stage), stageN, 1300, 25);
        MakeText(to_string(user2.getS()), scoreN, 1300, 55);
        MakeText(to_string(user1.getL()), lifeN, 1300, 85);
        score = user2.getS();
        life = user1.getL();
    }   

    int MakeText( String p, Text& t, double gameX = 0, double gameY = 0)
    {
        f.loadFromFile("hannamfree.ttf");
        t.setFont(f);              
        t.setCharacterSize(Fontsize);    
        t.setPosition(gameX, gameY);         
        t.setFillColor(Color::White);       
        t.setOutlineThickness(1.f);  
        t.setString(p); 
        return 0;
    }
    void update(User user1, Point user2)
    {
        MakeText(to_string(stage), stageN, 1300, 25);
        MakeText(to_string(user2.getS()), scoreN, 1300, 55);
        MakeText(to_string(user1.getL()), lifeN, 1300, 85);
        score = user2.getS();
    }
    void draw(RenderWindow& window,User user1, Point user2)
    {
        update(user1, user2);
        window.draw(stageT);
        window.draw(scoreT);
        window.draw(lifeT);
        window.draw(stageN);
        window.draw(scoreN);
        window.draw(lifeN);
    }
    int getS() { return stage; }
    void setS(int s) { stage = s; }
    
    void ShowScore() 
    {
        ofstream os{ "rank.txt",ios::app };
        if (!os)
        {
            cerr << "파일 오픈에 실패하였습니다" << endl;   exit(1);
        }
        os << "User               " << score << " 점" << endl;
        
        
        ifstream is{ "rank.txt",ios::in };
        if (is.fail())
        {
            cerr << "파일 오픈 실패" << endl;
            exit(1);
        }
        cout << " Name              Score" << endl;
        while (!is.eof())
        {
            int cnt = 0;
            is.getline(board, 100);
            cout << string(board) << endl;
            cnt++;
        }
    }

};

int Game::stage = 1;                                                    //정적 변수 사용

class Stage                                               //추상 클래스
{

protected:
    Texture t;
    Sprite background;                                    //배경
    int dx1,dy1 =0;                                     //장애물 속도
    int dx2,dy2 =0;
    User player;
    int next;
public: 
 
    Stage()
    {
    }

    virtual ~Stage()
    {
    }

    virtual void start(RenderWindow& window) {}

    void LifeZero(User u,Game& g)
    {
        if (u.getL() == 0)
        {
            g.ShowScore();
            exit(0);
        }
    }
};

class Stage1 : public Stage
{

public:
    Stage1()
    {
        User player(520, 280);                                //플레이어 
        this->player = player;
        t.loadFromFile("stage1.png");
        background.setTexture(t);
        dx1 = dy1 = 8;                                     //장애물 속도 재정의
        dx2 = dy2= -8;

    }
    void start(RenderWindow& window)
    {
        Hurdle ballArray[5]                               //장애물들 위
        {
            Hurdle(660, 240),
            Hurdle(720, 600),
            Hurdle(770, 240),
            Hurdle(480, 660),                                //아래
            Hurdle(950, 720)
        };
       
        Point pointArray[2]                                  //포인트들
        {
            Point(475, 740),
            Point(960, 740)
        };

        Game a(player, pointArray[0]);                                 //게임 인터페이스
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        while (window.isOpen())                                 //여기서부터 게임 시작
        {
            Event e;
            while (window.pollEvent(e));
            {
                if (e.type == Event::Closed)
                {
                    window.close();
                }

            }
            if (Keyboard::isKeyPressed(Keyboard::Left))         //키보드 입력 부분  
                player.update(-5.0, 0.0);
            if (Keyboard::isKeyPressed(Keyboard::Right))
                player.update(5.0, 0.0);
            if (Keyboard::isKeyPressed(Keyboard::Down))
                player.update(0.0, 5.0);
            if (Keyboard::isKeyPressed(Keyboard::Up))
                player.update(0.0, -5.0);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (player.getX() <= 480)                           //맵 위치 제한            
            {
                player.setX(480);
            }
            if (player.getX() >= 950)
            {
                player.setX(950);
            }
            if (player.getY() <= 243)
            {
                player.setY(243);
            }
            if (player.getY() >= 730)
            {
                player.setY(730);
            }

            if (player.getX() <= 650 && player.getY() >= 325 && player.getY() <= 330)       //위쪽(왼쪽) 굴절 구간 
            {
                player.setY(325);
            }

            if (player.getX() >= 785 && player.getY() >= 325 && player.getY() <= 330)        //위쪽(오른쪽) 굴절 구간 
            {
                player.setY(325);
            }

            if (player.getY() >= 335 && player.getY() <= 640)       //가운데 굴절
            {
                if (player.getX() <= 665)
                {
                    player.setX(665);
                }
                if (player.getX() >= 770)
                {
                    player.setX(770);
                }
            }

            if (player.getX() <= 650 && player.getY() >= 645 && player.getY() <= 650)        //아래쪽(왼쪽) 굴절
            {
                player.setY(650);
            }

            if (player.getX() >= 785 && player.getY() >= 645 && player.getY() <= 650)        //아래쪽(오른쪽) 굴절
            {
                player.setY(650);
            }

            try 
            {
                if (player.getY() <= 335 && player.getX() >= 850)                               //다음 라운드 이동
                {
                    throw next;
                }
            }
            catch (int e)
            {
                pointArray[0].setS(pointArray[0].getS() + 100);
                a.setS(a.getS() + 1);
                break;
            }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            for (int i = 0; i < 5; i++)                                   //장애물 충돌판정
            {
                ballArray[i].crash(player);
            }

            for (int i = 0; i < 2; i++)                                   //포인트 충돌판정
            {
                pointArray[i].crash(player);
            }
            LifeZero(player, a);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (ballArray[0].getY() <= 240 || ballArray[0].getY() >= 600)
            {
                dy1 = -dy1;
            }
            if (ballArray[1].getY() <= 240 || ballArray[1].getY() >= 600)
            {
                dy2 = -dy2;
            }

            if (ballArray[3].getX() < 480 || ballArray[3].getX() > 950)
            {
                dx1 = -dx1;
            }
            if (ballArray[4].getX() < 480 || ballArray[4].getX() > 950)
            {
                dx2 = -dx2;
            }

            for (int i = 0; i < 3; i++)                                   //장애물 이동 위
            {
                if (i % 2)
                {
                    ballArray[i].update(0, dy1);
                }
                else
                {
                    ballArray[i].update(0, dy2);
                }
            }

            for (int i = 3; i < 5; i++)                                   //장애물 이동 아래
            {
                if (i % 2)
                {
                    ballArray[i].update(dx1, 0);
                }
                else
                {
                    ballArray[i].update(dx2, 0);
                }
            }
            
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            window.clear();                                         //그리기
            window.draw(background);
            a.draw(window, player, pointArray[0]);
            player.draw(window);

            for (int i = 0; i < 5; i++)
            {
                ballArray[i].draw(window);
            }

            for (int i = 0; i < 2; i++)
            {
                pointArray[i].draw(window);
            }

            window.display();
        }
    }
};
    
class Stage2 : public Stage
{

public:
    Stage2()
    {
        User player(280, 230);                                //플레이어
        this->player = player;
        t.loadFromFile("stage2.png");
        background.setTexture(t);

        dx1 = 15;                                           //장애물 재정의
        dx2 = -15;
    }
    void start(RenderWindow& window)
    {
        Hurdle ballArray[8]                               //장애물들
        {
            Hurdle(225, 330),
            Hurdle(1215, 390),
            Hurdle(225, 450),
            Hurdle(1215, 510),
            Hurdle(225, 570),
            Hurdle(1215, 630),
            Hurdle(225, 690),
            Hurdle(1215, 750)
        };

        Point pointArray[2]                                 //포인트들
        {
            Point(225, 765),
            Point(1213, 765)
        };

        Game a(player, pointArray[0]);                                 //게임 인터페이스
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        while (window.isOpen())                                 //여기서부터 게임 시작
        {
            Event e;
            while (window.pollEvent(e));
            {
                if (e.type == Event::Closed)
                {
                    window.close();
                }

            }

            if (Keyboard::isKeyPressed(Keyboard::Left))         //키보드 입력 부분  
                player.update(-5.0, 0.0);
            if (Keyboard::isKeyPressed(Keyboard::Right))
                player.update(5.0, 0.0);
            if (Keyboard::isKeyPressed(Keyboard::Down))
                player.update(0.0, 5.0);
            if (Keyboard::isKeyPressed(Keyboard::Up))
                player.update(0.0, -5.0);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            if (player.getX() <= 230)                           //맵 위치 제한            
            {
                player.setX(230);
            }
            if (player.getX() >= 1213)
            {
                player.setX(1213);
            }
            if (player.getY() <= 188)
            {
                player.setY(188);
            }
            if (player.getY() >= 765)
            {
                player.setY(765);
            }

            if (player.getX() >= 340 && player.getX() <= 1095 && player.getY() <= 338)          //윗 부분
            {
                player.setY(338);
            }

            if (player.getY() <= 332 && player.getX() >= 325 && player.getX() <= 330)           //왼쪽 모서리
            {
                player.setX(325);
            }
            try 
            {
                if (player.getX() >= 1080 && player.getY() <= 290)                                 //다음 라운드 이동
                {
                    throw next;
                }
            }
            catch (int e)
            {
                pointArray[0].setS(pointArray[0].getS() + 200);
                a.setS(a.getS() + 1);
                break;
            }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            for (int i = 0; i < 8; i++)                                   //장애물 충돌판정
            {
                ballArray[i].crash(player);
            }

            for (int i = 0; i < 2; i++)                                   //포인트 충돌판정
            {
                pointArray[i].crash(player);
            }
            LifeZero(player, a);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (ballArray[0].getX() == 225 || ballArray[0].getX() == 1215)                           //장애물 이동
            {
                dx1 = -dx1;
            }
            if (ballArray[1].getX() == 225 || ballArray[1].getX() == 1215)
            {
                dx2 = -dx2;
            }

            for (int i = 0; i < 8; i++)
            {
                if (i % 2)
                {
                    ballArray[i].update(dx1, 0);
                }
                else
                {
                    ballArray[i].update(dx2, 0);
                }
            }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            window.clear();                                         //그리기
            window.draw(background);
            a.draw(window, player, pointArray[0]);
            player.draw(window);

            for (int i = 0; i < 8; i++)
            {
                ballArray[i].draw(window);
            }

            for (int i = 0; i < 2; i++)
            {
                pointArray[i].draw(window);
            }

            window.display();
        }
    }

};

class Stage3 : public Stage
{
public:

    Stage3()
    {
        User player(705, 720);                                //플레이어
        this->player = player;
        t.loadFromFile("stage3.png");
        background.setTexture(t);

        dx1 = 15;                                           //장애물 재정의
        dx2 = -15;
        dy1 = 5;
        dy2 = -5;
    }

    void start(RenderWindow& window)
    {
        Hurdle ballArray[10]                               //장애물들 가운데
        {
            Hurdle(337, 320),
            Hurdle(1127, 380),
            Hurdle(337, 440),
            Hurdle(1127, 500),
            Hurdle(337, 560),
            Hurdle(1127, 620),
            Hurdle(325, 155),                              //여기서 부터 위 장애물
            Hurdle(385, 275),
            Hurdle(513, 155),
            Hurdle(573, 275)
        };

        Point pointArray[4]                                 //포인트들
        {
            Point(325, 150),
            Point(325, 635),
            Point(1142, 305),
            Point(583, 150)
        };

        Game a(player, pointArray[0]);                                 //게임 인터페이스
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        while (window.isOpen())                                 //여기서부터 게임 시작
        {
            Event e;
            while (window.pollEvent(e));
            {
                if (e.type == Event::Closed)
                {
                    window.close();
                }

            }

            if (Keyboard::isKeyPressed(Keyboard::Left))         //키보드 입력 부분  
                player.update(-5.0, 0.0);
            if (Keyboard::isKeyPressed(Keyboard::Right))
                player.update(5.0, 0.0);
            if (Keyboard::isKeyPressed(Keyboard::Down))
                player.update(0.0, 5.0);
            if (Keyboard::isKeyPressed(Keyboard::Up))
                player.update(0.0, -5.0);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (player.getX() <= 337)                           //맵 위치 제한            
            {
                player.setX(337);
            }
            if (player.getX() >= 1127)
            {
                player.setX(1127);
            }
            if (player.getY() <= 155)
            {
                player.setY(155);
            }
            

            if (player.getY() <= 305 && player.getX() >= 575)          //위쪽 굴절 구간
            {
                    player.setX(575);
            }
            if (player.getX() >= 580 && player.getY() <= 315)
            {
                    player.setY(315);
            }
            
            if (player.getY() >= 625)                                               //아래쪽 굴절 구간
            {
                if (player.getX() >= 1065 && player.getX() <= 1127)
                {
                    
                }
                else if (player.getX() >= 670 && player.getX() <= 750)
                {
                    if (player.getX() <= 675)
                    {
                        player.setX(675);
                    }
                    if (player.getX() >= 730)
                    {
                        player.setX(730);
                    }
                    if (player.getY() >= 760)
                    {
                        player.setY(760);
                    }

                }
                else 
                {
                    player.setY(625);
                }
            }
            try 
            {
                if (player.getX() >= 1065 && player.getY() >= 680)     //다음 라운드 이동
                {
                    throw next;
                }
            }
            catch (int e)
            {
                pointArray[0].setS(pointArray[0].getS() + 300);
                a.setS(a.getS() + 1);
                break;
            }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            for (int i = 0; i < 10; i++)                                   //장애물 충돌판정
            {
                ballArray[i].crash(player);
            }

            for (int i = 0; i < 4; i++)                                   //포인트 충돌판정
            {
                pointArray[i].crash(player);
            }
            LifeZero(player, a);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            if (ballArray[0].getX() <= 337 || ballArray[0].getX() >= 1127)                  //장애물 이동 범위 제한 가운데
            {
                dx1 = -dx1;
            }
            if (ballArray[1].getX() <= 337 || ballArray[1].getX() >= 1127)
            {
                dx2 = -dx2;
            }

            if (ballArray[6].getY() <= 155 || ballArray[6].getY() >= 275)                  //장애물 이동 범위 제한 위
            {
                dy1 = -dy1;
            }
            if (ballArray[7].getY() <= 155 || ballArray[7].getY() >= 275)
            {
                dy2 = -dy2;
            }

            for (int i = 0; i < 6; i++)                                                     //장애물 이동 가운데
            {
                if (i % 2)
                {
                    ballArray[i].update(dx1, 0);
                }
                else
                {
                    ballArray[i].update(dx2, 0);
                }
            }

            for (int i = 6; i < 10; i++)                                                     //장애물 이동 위
            {
                if (i % 2)
                {
                    ballArray[i].update(0, dy1);
                }
                else
                {
                    ballArray[i].update(0, dy2);
                }
            }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            window.clear();                                         //그리기
            window.draw(background);
            a.draw(window, player, pointArray[0]);
            player.draw(window);

            for (int i = 0; i < 10; i++)
            {
                ballArray[i].draw(window);
            }

            for (int i = 0; i < 4; i++)
            {
                pointArray[i].draw(window);
            }

            window.display();
        }
    }

};

class Stage4 : public Stage
{
public:

    Stage4()
    {
        User player(235, 135);                                //플레이어
        this->player = player;
        t.loadFromFile("stage4.png");
        background.setTexture(t);

        dx1 = 8;                                           //장애물 재정의
        dx2 = -8;
        dy1 = 8;
        dy2 = -8;
    }
    void start(RenderWindow& window)
    {
        Hurdle ballArray[19]                                //장애물들
        {
            Hurdle(185, 240),
            Hurdle(245, 655),
            Hurdle(305, 240),
            Hurdle(365, 655),
            Hurdle(425, 240),
            Hurdle(485, 655),
            Hurdle(545, 240),
            Hurdle(605, 655),
            Hurdle(665, 240),
            Hurdle(725, 655),
            Hurdle(785, 240),
            Hurdle(845, 655),
            Hurdle(905, 240),
            Hurdle(965, 655),
            Hurdle(1025, 240),
            Hurdle(1085, 655),
            Hurdle(1145, 240),
            Hurdle(1205, 655),
            Hurdle(1265, 240)
        };

        Point pointArray[6]                                   //포인트들
        {
            Point(178, 685),
            Point(1280, 233),
            Point(845, 333),
            Point(845, 580),
            Point(592, 433),
            Point(592, 483)
        };
        

        Game a(player, pointArray[0]);                                 //게임 인터페이스
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        while (window.isOpen())                                 //여기서부터 게임 시작
        {
            Event e;
            while (window.pollEvent(e));
            {
                if (e.type == Event::Closed)
                {
                    window.close();
                }

            }

            if (Keyboard::isKeyPressed(Keyboard::Left))         //키보드 입력 부분  
                player.update(-5.0, 0.0);
            if (Keyboard::isKeyPressed(Keyboard::Right))
                player.update(5.0, 0.0);
            if (Keyboard::isKeyPressed(Keyboard::Down))
                player.update(0.0, 5.0);
            if (Keyboard::isKeyPressed(Keyboard::Up))
                player.update(0.0, -5.0);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (player.getX() <= 190)                           //맵 위치 제한            
            {
                player.setX(190);
            }
            if (player.getX() >= 1265)
            {
                player.setX(1265);
            }
            if (player.getY() <= 80)
            {
                player.setY(80);
            }

            if (player.getY() <= 235 && player.getX() >= 280 )                                   //위쪽 구간
            {
                player.setX(280);
            }
            if (player.getY() >= 240 && player.getY() <= 245 && player.getX() >= 280)
            {
                player.setY(245);
            }
                

            if (player.getY() >= 674 && player.getX() <= 1160)                                   //아래쪽 구간
            {
                    player.setY(674);
            }

            if (player.getX() >= 540 && player.getX() <= 895)                                   //ㄹ자 구간
            {                                                                                   //수평 구간
                if (player.getY() >= 340 && player.getY() <= 345)                               //1아래           
                {
                    player.setY(345);
                }
                else if (player.getY() >= 270 && player.getY() <= 275)                          //1위
                {
                    player.setY(270);
                }
                else if (player.getY() >= 490 && player.getY() <= 495)                          //2아래
                {
                    player.setY(495);
                }
                else if (player.getY() >= 423 && player.getY() <= 428)                          //2위
                {
                    player.setY(423);
                }
                else if (player.getY() >= 630 && player.getY() <= 635)                          //3아래
                {
                    player.setY(635);
                }
                else if (player.getY() >= 565 && player.getY() <= 570)                          //3위
                {   
                    player.setY(565);                                                           
                }
            }

            if (player.getY() >= 425 && player.getY() <= 625)                                   //ㄹ자 구간
            {                                                                                   //수직 구간 
                if (player.getX() >= 530 && player.getX() <= 535)                               //왼왼
                {
                    player.setX(530);
                }
                else if (player.getX() >= 600 && player.getX() <= 605)                          //왼오
                {
                    player.setX(605);
                }
            }
            if (player.getY() >= 270 && player.getY() <= 335)                              //왼위
            {
                if (player.getX() >= 530 && player.getX() <= 535)
                {
                    player.setX(530);
                }
            }
            if (player.getY() >= 280 && player.getY() <= 490)                                                                  
            {
                if (player.getX() >= 830 && player.getX() <= 835)                               //오왼
                {
                    player.setX(830);
                }
                else if (player.getX() >= 900 && player.getX() <= 905)                          //오오
                {
                    player.setX(905);
                }
            }

            if (player.getY() >= 570 && player.getY() <= 630)
            {
                if (player.getX() >= 900 && player.getX() <= 905)                          //오아
                {
                     player.setX(905);
                }
            }
            try 
            {
                if (player.getY() >= 710 && player.getX() >= 1160)                                   //다음 라운드 이동
                {
                    throw next;
                }
            }
            catch (int e)
            {
                pointArray[0].setS(pointArray[0].getS() + 400);
                a.setS(a.getS() + 1);
                break;
            }
            
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            for(int i=0; i<19;i++)                                                       //장애물 충돌판정
            { 
                ballArray[i].crash(player);
            }
                                              
            for (int i = 0; i < 6; i++)                                                  //포인트 충돌판정
            {
                pointArray[i].crash(player);
            }
            LifeZero(player, a);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (ballArray[0].getY() < 240 || ballArray[0].getY() > 655)                 //장애물 이동 위
            {
                dy1 = -dy1;
            }

            if (ballArray[1].getY() < 240 || ballArray[1].getY() > 655)
            {
                dy2 = -dy2;
            }

            for (int i = 0; i < 19; i++)
            {
                if (i % 2)
                {
                    ballArray[i].update(0, dy2);
                }
                else
                {
                    ballArray[i].update(0, dy1);
                }
            }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            window.clear();                                         //그리기
            window.draw(background);
            a.draw(window, player, pointArray[0]);
            player.draw(window);

            for (int i = 0; i < 19; i++)
            {
                ballArray[i].draw(window);
            }

            for (int i = 0; i < 6; i++)
            {
                pointArray[i].draw(window);
            }

            window.display();
        }
    }

};

class Stage5 : public Stage
{
public:

    Stage5()
    {
        User player(258, 200);                                //플레이어
        this->player = player;
        t.loadFromFile("stage5.png");
        background.setTexture(t);

        dx1 = 8;                                           //장애물 재정의
        dx2 = 5;
        dy1 = 5;
        dy2 = -6;
    }

    void start(RenderWindow& window)
    {
        bool b1 = true;

        Hurdle ballArray[56]
        {
            Hurdle(300,640),                         //네모 아래
            Hurdle(350, 640),
            Hurdle(480, 640),
            Hurdle(530, 640),
            Hurdle(660, 640),
            Hurdle(710, 640),

            Hurdle(230, 575),                         //move

            Hurdle(220, 280),                         //왼쪽
            Hurdle(220, 330),
            Hurdle(220, 460),
            Hurdle(220, 510),

            Hurdle(285, 280),                        //move

            Hurdle(350, 170),                        //맨위
            Hurdle(460, 235),
            Hurdle(570, 170),
            Hurdle(680, 235),
            Hurdle(795, 280),                       //중간
            Hurdle(795, 330),
            Hurdle(730, 445),
            Hurdle(730, 495),

            Hurdle(725, 180),                        //오른쪽 위 move
            Hurdle(725, 220),

            Hurdle(1185, 340),                       //오른쪽 사이드
            Hurdle(1125, 340),
            Hurdle(1185, 400),
            Hurdle(1125, 400),
            Hurdle(1185, 460),
            Hurdle(1125, 460),
            Hurdle(1185, 520),
            Hurdle(1125, 520),
            Hurdle(1185, 640),
            Hurdle(1125, 640),
            Hurdle(1185, 700),
            Hurdle(1125, 700),
            Hurdle(1185, 760),
            Hurdle(1125, 760),
            Hurdle(1185, 820),
            Hurdle(1125, 820),

            Hurdle(350, 790),                           //맨 아래
            Hurdle(350, 850),
            Hurdle(470, 790),
            Hurdle(470, 850),
            Hurdle(590, 790),
            Hurdle(590, 850),
            Hurdle(710, 790),
            Hurdle(710, 850),
            Hurdle(830, 790),
            Hurdle(830, 850),
            Hurdle(950, 790),
            Hurdle(950, 850),
            Hurdle(1010, 790),
            Hurdle(1010, 850),
            Hurdle(410, 790),
            Hurdle(530, 850),
            Hurdle(770, 790),
            Hurdle(890, 790)
        };
       
        Point pointArray[3]                                 //포인트들
        {
            Point(220, 645),
            Point(1265, 175),
            Point(805, 645)
        };

        Game a(player, pointArray[0]);                                 //게임 인터페이스
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        while (window.isOpen())                                 //여기서부터 게임 시작
        {
            Event e;
            while (window.pollEvent(e));
            {
                if (e.type == Event::Closed)
                {
                    window.close();
                }

            }

            if (Keyboard::isKeyPressed(Keyboard::Left))             //키보드 입력 부분  
                player.update(-5.0, 0.0);
            if (Keyboard::isKeyPressed(Keyboard::Right))
                player.update(5.0, 0.0);
            if (Keyboard::isKeyPressed(Keyboard::Down))
                player.update(0.0, 5.0);
            if (Keyboard::isKeyPressed(Keyboard::Up))
                player.update(0.0, -5.0);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (player.getX() <= 230)                           //맵 위치 제한            
            {
                player.setX(230);
            }
            if (player.getX() >= 1190 && player.getX() <= 1195 && player.getY() >= 230)
            {
                player.setX(1190);
            }
            if (player.getY() <= 182)
            {
                player.setY(182);
            }
            if (player.getY() >= 843)
            {
                player.setY(843);
            }
            

            if (player.getX() >= 305 && player.getX() <= 710)                       //가운데 네모
            {
                if (player.getY() >= 225 && player.getY() <= 230)
                {
                    player.setY(225);
                }
                if (player.getY() >= 565 && player.getY() <= 570)
                {
                    player.setY(570);
                }
            }
            if (player.getY() >= 230 && player.getY() <= 570)
            {
                if (player.getX() >= 300 && player.getX() <= 305)
                {
                    player.setX(300);
                }
                if (player.getX() >= 720 && player.getX() <= 725)
                {
                    player.setX(725);
                }
            }

            if (player.getX() <= 850 && player.getY() >= 635 && player.getY() <= 640)
            {
                player.setY(635);
            }
            if (player.getY() <= 635 && player.getY() >= 225 && player.getX() >= 800 && player.getX() <= 805)
            {
                player.setX(800);
            }

            if (player.getX() >= 800 && player.getX() <= 1105 && player.getY() >= 225 && player.getY() <= 230)
            {
                player.setY(225);
            }

            if (player.getY() >= 225 && player.getY() <= 800 && player.getX() >= 1105 && player.getX() <= 1110)
            {
                player.setX(1110);
            }

            if (player.getX() >= 230 && player.getX() <= 1110 && player.getY() >= 795 && player.getY() <= 800)
            {
                player.setY(800);
            }

            if (player.getX() >= 1190 && player.getY() >= 225 && player.getY() <= 230)
            {
                player.setY(225);
            }
            if (player.getX() >= 1265)
            {
                player.setX(1265);
            }
            try
            {
                if (player.getX() <= 275 && player.getY() >= 795)                   //다음 라운드 이동
                {
                    throw next;
                }
            }
            catch(int e)
            {
                pointArray[0].setS(pointArray[0].getS() + 1000);
                a.setS(a.getS() + 1);
                break;
            }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            for (int i = 0; i < 22; i++)
            {
                ballArray[i].crash(player);
            }
            if (b1)
            {
                for (int i = 22; i < 56; i++)
                {
                    ballArray[i].crash(player);
                }
            }
            for(int i = 0; i < 3; i++)                                   //포인트 충돌판정
            {
                pointArray[i].crash(player);
            }
            LifeZero(player, a);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            if (ballArray[6].getX() < 230 || ballArray[6].getX() > 800)               //네모 밑
            {
                dx1 = -dx1;
                b1 = !b1;
            }
            ballArray[6].update(dx1, 0);

            if (ballArray[11].getY() < 280 || ballArray[11].getY() > 520)             //왼쪽
            {
                dy1 = -dy1;
            }
            ballArray[11].update(0, dy1);

            if (ballArray[20].getX() < 720 || ballArray[20].getX() > 1260)               //오른쪽 위
            {
                dx2 = -dx2;
            }
            ballArray[20].update(dx2, 0);
            ballArray[21].update(dx2, 0);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            window.clear();                                         //그리기
            window.draw(background);
            a.draw(window, player, pointArray[0]);
            player.draw(window);

            for (int i = 0; i < 22; i++)
            {
                ballArray[i].draw(window);
            }

            if (b1)
            {
                for (int i = 22; i < 56; i++)
                {
                    ballArray[i].draw(window);
                }
            }

            for (int i = 0; i < 3; i++)
            {
                pointArray[i].draw(window);
            }

            window.display();
        }
    }

};



int main()
{   

    RenderWindow window(VideoMode(1500, 1000), "Game");
    window.setFramerateLimit(60);

    
    Stage* S1 = new Stage1();                                    //상향 형변환
    Stage* S2 = new Stage2();
    Stage* S3 = new Stage3();
    Stage* S4 = new Stage4();
    Stage* S5 = new Stage5();
   
  
        S1->start(window);
        S2->start(window);
        S3->start(window);
        S4->start(window);
        S5->start(window);
       


    delete S1;
    delete S2;
    delete S3;
    delete S4;
    delete S5;

    return 0;
}   