#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <algorithm> // count_if
#include <cstdlib> //rand(), srand()
#include <ctime> //time()

using namespace std;
using namespace sf;

float wall_pos_x = 0;

long wall_count=0;
Text wall_count_text; 
Font font; 

long score=0;
Text score_text; 

FloatRect war_area;
FloatRect rec_area;


class RECT{

public :

    RectangleShape rect;

    RECT();
    void draw(RenderWindow& _window);
    void update();

};

RECT::RECT() {
    rect = RectangleShape(Vector2f(20.0f, 10.0f));
    rect.setPosition(500, 30);
    rect.setFillColor(Color::Green);
    rec_area = rect.getGlobalBounds();
}
void RECT::draw(RenderWindow& _window){
    _window.draw(rect);
}
void RECT::update() {

}

class WALL {

public :
    RectangleShape wall;

    WALL();
    void draw(RenderWindow& _window);
    // update 필요 없

};
WALL::WALL() {
    wall = RectangleShape(Vector2f(3, 600));
    
    wall_pos_x = 100;
    wall.setPosition(wall_pos_x, 0);
    war_area = wall.getGlobalBounds();
};
void WALL::draw(RenderWindow& _window){
    _window.draw(wall);
}

class BALL {

public :
    float ball_radius=10;
    CircleShape ball;
    float offset_x, offset_y;
    bool touchY;

    BALL();
    void update();
    void draw(RenderWindow& _window);

};
BALL::BALL() {

    touchY = false;
    ball = CircleShape(ball_radius);
    ball.setFillColor(Color::Magenta);

    font.loadFromFile("images/OpenSans-Bold.ttf");      
    // wall_count_text.setFont(font); // 폰트 지정
    // wall_count_text.setCharacterSize(30); // 문자 크기 지정
    // wall_count_text.setFillColor(Color::Red); // 색 지정
    // wall_count_text.setStyle(Text::Bold); // 두꺼운 글씨 지정

    score_text.setFont(font); // 폰트 지정
    score_text.setCharacterSize(30); // 문자 크기 지정
    score_text.setFillColor(Color::Red); // 색 지정
    score_text.setStyle(Text::Bold); // 두꺼운 글씨 지정

    offset_x = (rand() % 10 - 5); // -6 ~ 4
    offset_y = (rand() % 10 - 5); // -6 ~ 4
    cout << "offset_x , offset_y : " << offset_x << " " << offset_y << endl;
}
void BALL::update(){
    
    if(touchY == true){
        return;
    }
    Vector2f ball_pos = ball.getPosition();

    // wall에 맞았는지 --------
    FloatRect ball_area = ball.getGlobalBounds();
    if (war_area.intersects(ball_area) == true) {
        if (offset_x <= 0) {
            offset_x = -offset_x;
            wall_count++;
        }
    }

    if (rec_area.intersects(ball_area) == true) {
        if (offset_y <= 0) {
            offset_y = -offset_y;
        }
    }
    
    // 위에 맞았는지?
    if(ball_pos.y <= 0){

        if(offset_y <= 0) {
            offset_y = -offset_y;
            wall_count++;
            score++;
            touchY = true;
        }

    }

    // 아래 맞았는지 ?
    if(ball_pos.y + 2*ball_radius >= 600){
        if(offset_y >= 0) {
            offset_y = -offset_y;
            wall_count++;
        }
    }

    // 왼쪽 맞았는지 ?
    if(ball_pos.x <= 0){
        if(offset_x <= 0) {
            offset_x = -offset_x;
            wall_count++;
        }
    }

    // 오른쪽 맞았는지 ?
    if(ball_pos.x + 2*ball_radius >= 1000){
        if(offset_x >= 0) {
            offset_x = -offset_x;
            wall_count++;
        }
    }

    ball.move(offset_x, offset_y);

    // wall_count_text.setString(to_string(wall_count));
    // wall_count_text.setPosition(500, 50);

    score_text.setString(to_string(score));
    score_text.setPosition(500, 50);
    

}
void BALL::draw(RenderWindow& _window) {
    _window.draw(score_text);
    if(touchY == true) {
        return;
    }
    _window.draw(ball);
    _window.draw(wall_count_text);
    _window.draw(score_text);
}

class BALL_SET {

public :

   vector<BALL> vec;

   BALL_SET(int num = 5);
   void update();
   void draw(RenderWindow& _window);

};
BALL_SET::BALL_SET(int num){ // default parameter

    for (int i = 0; i < num; i++)
    {
        BALL buf;
        buf.ball.setPosition(400, 500);
        vec.push_back(buf);
    }
}
void BALL_SET::update() {

    for (int i = 0; i <vec.size(); i++){
        vec[i].update();
    }  
}
void BALL_SET::draw(RenderWindow& _window) {

    for(int i = 0; i < vec.size() ; i++){

        vec[i].draw(_window);

    }

}

class GAME {

public :
    BALL_SET ball_set;
    WALL wall;
    RECT rect;

    GAME();
    void draw(RenderWindow& _window);
    void update();

};
GAME::GAME() {

}
void GAME:: draw(RenderWindow& _window) {
    ball_set.draw(_window);
    wall.draw(_window);
    rect.draw(_window);
}
void GAME:: update() {
    ball_set.update();
}

int main()
{
    RenderWindow window(VideoMode(1000, 600), "movingdot");
    window.setFramerateLimit(120);
    

    srand(time(NULL));  
    
    GAME game;
    

    while (window.isOpen())
    {
        // 마우스의 좌클릭 event를 감지.

        Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case Event::Closed:
                    window.close();
                break;
            }
        }


        /* update */
        game.update();
        
        /* clear */
        window.clear();

        /* draw */
        game.draw(window);

        /* display */
        window.display();

    }



    return 0;
}