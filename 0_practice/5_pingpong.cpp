#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <algorithm> // count_if
#include <ctime> // time()
#include <cstdlib> //rand(), srand()
#include <unistd.h> // sleep()

#define WINDOW_X_LEN 1500
#define WINDOW_Y_LEN 1000

using namespace std;
using namespace sf;

int up_score = 0;
int down_score = 0;

class USER {

public : 
    RectangleShape up_user;
    RectangleShape down_user;
    float up_user_offsetx;
    float down_user_offsetx;

    USER();
    void draw(RenderWindow& _window);
    void up_update(RenderWindow& _window);  // AI
    void down_update(RenderWindow& _window); // 1 user

};
USER::USER() {
    up_user_offsetx = 20.0f;
    up_user = RectangleShape(Vector2f(200, 10));
    up_user.setFillColor(Color::Green);
    up_user.setPosition(WINDOW_X_LEN / 2 - 75, 30);

    down_user_offsetx = 20.0f;
    down_user = RectangleShape(Vector2f(200, 10));
    down_user.setFillColor(Color::Green);
    down_user.setPosition(WINDOW_X_LEN / 2 - 75, WINDOW_Y_LEN - 50);

}
void USER::draw(RenderWindow& _window){
    _window.draw(up_user);
    _window.draw(down_user);
}
void USER::down_update(RenderWindow& _window){
    
    Vector2i mouse_pos = Mouse::getPosition(_window);
    down_user.setPosition(mouse_pos.x - down_user.getSize().x / 2, WINDOW_Y_LEN - 40);

    Vector2f down_user_pos = down_user.getPosition();
    // 2. 그 위치정보를 기반으로 화면밖으로 나갔는지 판단.
    if (down_user_pos.x <= 0) { // 화면 좌측 침범.
        down_user.setPosition(0, down_user_pos.y);
    }
    else if (down_user_pos.x + down_user.getSize().x >= WINDOW_X_LEN) // 화면 우측 침범.
    {
        down_user.setPosition(WINDOW_X_LEN - down_user.getSize().x, down_user_pos.y);
    }
}
void USER::up_update (RenderWindow& _window) {
    
    Vector2f up_user_pos = up_user.getPosition();
    if(up_user_pos.x <= 0) {
        if(up_user_offsetx <= 0) {
            up_user_offsetx = -up_user_offsetx;
        }
    }
    if(up_user_pos.x + up_user.getSize().x >= WINDOW_X_LEN) {
        if(up_user_offsetx >= 0) {
            up_user_offsetx = -up_user_offsetx;
        } 
    }
    up_user.move(up_user_offsetx, 0);

}

class BALL {

public : 

    CircleShape ball;
    bool ball_alive;
    float ball_radius;
    float offset_x, offset_y;
    
    BALL();
    void draw(RenderWindow& _window);
    void update(RenderWindow& _window);

};
BALL::BALL() {

    ball_alive = true;
    ball_radius = 15;
    ball = CircleShape(ball_radius);
    ball.setFillColor(Color::White);
    ball.setPosition(WINDOW_X_LEN / 2 , WINDOW_Y_LEN / 2);
    
    offset_x = rand() % 16 - 8;
    if(offset_x >= -3 && offset_x<= 3 ) {
        offset_x *= 2;
        if(offset_x == 0) {
            offset_x = 5;
        }
    }
    offset_y = rand() % 16 - 8;
     if(offset_y >= -3 && offset_y<= 3 ) {
        offset_y *= 2;
        if(offset_y == 0) {
            offset_y = 5;
        }
    }

}
void BALL::update(RenderWindow& _window) {
    // if(ball_alive == false) {
    //     return; 
    // }

    Vector2f ball_pos = ball.getPosition();

    // 화면 왼쪽에 부딪
    if(ball_pos.x <= 0) {
        if(offset_x <= 0) {
            offset_x = -offset_x;
        }
    }
    // 화면 오른쪽에 부딪 s
    if(ball_pos.x + 2*ball_radius >= WINDOW_X_LEN) {
        if(offset_x >= 0) {
            offset_x = -offset_x;
        }
    }
    // 화면 위에 부딪 (down user score)
    if(ball_pos.y <= 0) {
        if(offset_y <= 0) {
            ball_alive = false;
            down_score++;
        }
    }
    // 화면 아래에 부딪 (up user score)
    if(ball_pos.y + 2*ball_radius >= WINDOW_Y_LEN) {
        if(offset_y >= 0) {
            ball_alive = false;
            up_score++;
        }
    }
    ball.move(offset_x, offset_y);
}
void BALL::draw(RenderWindow& _window) {
    // if(ball_alive == false) {
    //     return;
    // }
    _window.draw(ball);
}


class BALL_SET {

public :

    vector<BALL> ball_vec;

    BALL_SET(int num = 10);
    
    void draw(RenderWindow& _window);
    void update(RenderWindow& _window);

};
BALL_SET::BALL_SET(int num) {

    for (int i = 0; i < num; i++)
    {
        BALL temp;
        ball_vec.push_back(temp);
    }

}
void BALL_SET::draw(RenderWindow& _window) {

    for (int i = 0; i < ball_vec.size(); i++)
    {
        ball_vec[i].draw(_window);
    }
    
}
void BALL_SET::update(RenderWindow& _window) {

    for (int i = 0; i < ball_vec.size(); i++)
    {
        ball_vec[i].update(_window);

        // erase() 함수 사용하여 ball 객체 하나를 그리지 않는 법
        if(ball_vec[i].ball_alive == false) {
            ball_vec.erase(ball_vec.begin() + i);
            i--;
        }
    }

}


class WALL {

public :

    RectangleShape left_wall;
    RectangleShape right_wall;
    float left_wall_offset;
    float right_wall_offset;

    WALL();
    void draw(RenderWindow& _window);
    void update(RenderWindow& _window);

};
WALL::WALL() {

    left_wall_offset = 4;
    left_wall = RectangleShape(Vector2f(10, WINDOW_Y_LEN));
    left_wall.setPosition(0,0);
    left_wall.setFillColor(Color::Yellow);

    right_wall_offset = -4;
    right_wall = RectangleShape(Vector2f(10, WINDOW_Y_LEN));
    right_wall.setPosition(WINDOW_X_LEN - right_wall.getSize().x, 0);
    right_wall.setFillColor(Color::Yellow);
}
void WALL::draw(RenderWindow& _window) {
    _window.draw(left_wall);
    _window.draw(right_wall);
}
void WALL::update(RenderWindow& _window) {
    
    Vector2f left_wall_pos = left_wall.getPosition();
    Vector2f right_wall_pos = right_wall.getPosition();

    // 가운데 300픽셀 정도 공간까지 최대로 좁혀짐

    // left_wall
    if(left_wall_pos.x + left_wall.getSize().x >= WINDOW_X_LEN/2 - 250) {
        if(left_wall_offset >= 0) {
            left_wall_offset = -left_wall_offset;
        }
    }
    if(left_wall_pos.x <= 0) {
        if(left_wall_offset <= 0) {
            left_wall_offset = -left_wall_offset;
        }
    }

    // right_wall
    if(right_wall_pos.x <= WINDOW_X_LEN/2 + 250) {
        if(right_wall_offset <= 0) {
            right_wall_offset = -right_wall_offset;
        }
    }
    if(right_wall_pos.x + right_wall.getSize().x >= WINDOW_X_LEN) {
        if(right_wall_offset >= 0) {
            right_wall_offset = -right_wall_offset;
        }
    }

    left_wall.move(left_wall_offset, 0);
    right_wall.move(right_wall_offset, 0);

}


class GAME {

public : 

    BALL_SET ball_set;
    WALL wall;
    USER user;

    Text up_score_text; 
    Text down_score_text; 
    Font font;

    GAME();
    void init_end(RenderWindow& _window, int status, unsigned char game_result);
    void draw(RenderWindow& _window);
    void update(RenderWindow& _window);
    void user_input(RenderWindow& _window);

};
GAME::GAME() {
    font.loadFromFile("images/OpenSans-Bold.ttf");

    up_score_text.setFont(font);
    up_score_text.setCharacterSize(50);
    up_score_text.setFillColor(Color::Magenta);
    up_score_text.setStyle(Text::Bold);

    down_score_text.setFont(font);
    down_score_text.setCharacterSize(50);
    down_score_text.setFillColor(Color::Magenta);
    down_score_text.setStyle(Text::Bold);
}
void GAME::draw(RenderWindow& _window)  {
    ball_set.draw(_window);
    wall.draw(_window);
    _window.draw(up_score_text);
    _window.draw(down_score_text);
    user.draw(_window);
}
void GAME::update(RenderWindow& _window) {

    ball_set.update(_window);
    wall.update(_window);
    user.up_update(_window);
    user.down_update(_window);

    // ball이 user 또는 wall에 충돌했는지? --------------------
    FloatRect left_wall_area = wall.left_wall.getGlobalBounds();
    FloatRect right_wall_area = wall.right_wall.getGlobalBounds();
    FloatRect up_user_area = user.up_user.getGlobalBounds();
    FloatRect down_user_area = user.down_user.getGlobalBounds();

    for (int i = 0; i < ball_set.ball_vec.size(); i++)
    {
        FloatRect ball_area = ball_set.ball_vec[i].ball.getGlobalBounds();
        // left_wall에 부딪
        if(ball_area.intersects(left_wall_area) == true){
            if(ball_set.ball_vec[i].offset_x <= 0){
                ball_set.ball_vec[i].offset_x *= -1; 
            }
        }
        // right_wall에 부딪
        if(ball_area.intersects(right_wall_area) == true){
            if(ball_set.ball_vec[i].offset_x >= 0){
                ball_set.ball_vec[i].offset_x *= -1; 
            }
        }
        // up_user와 부딪혔는지?   
        if(ball_area.intersects(up_user_area) == true){
            if(ball_set.ball_vec[i].offset_y <= 0){
                ball_set.ball_vec[i].offset_y *= -1; 
            }
        }  
        // down_user와 부딪혔는지?   
        if(ball_area.intersects(down_user_area) == true){
            if(ball_set.ball_vec[i].offset_y >= 0){
                ball_set.ball_vec[i].offset_y *= -1; 
            }
        }    
    }

    // score update ------------------------------
    up_score_text.setString(to_string(up_score));
    up_score_text.setPosition(50, 50);
    down_score_text.setString(to_string(down_score));
    down_score_text.setPosition(WINDOW_X_LEN - 100, WINDOW_Y_LEN - 100);
    
    // init_end
    unsigned char game_result;
    if(up_score + down_score == 10) {
        if(up_score > down_score) { // LOSE
            game_result = 0;
        }
        else if(down_score > up_score) { // WIN
            game_result = 2;
        }
        else { // DRAW
            game_result = 1;
        }
        sleep(1);
        init_end(_window, 0, game_result);
    }

}
void GAME::user_input(RenderWindow& _window) {
    user.down_update(_window);
}
void GAME::init_end(RenderWindow& _window, int status, unsigned char game_result) {

    RectangleShape startBtn(Vector2f(500.0f, 300.0f));
    startBtn.setPosition(WINDOW_X_LEN/2 - 300, WINDOW_Y_LEN/2 - 150);

    Texture startBtn_text;
    if(status) {
        startBtn_text.loadFromFile("images/startBtn2.png");
    }
    else if(!status) {
        if(game_result == 0) { // lose
            startBtn_text.loadFromFile("images/you_lose.png");
        }
        else if(game_result == 1) { // tie
            startBtn_text.loadFromFile("images/you_tie.png");
        }
        else { // win
            startBtn_text.loadFromFile("images/you_win.png");
        }
    }
    startBtn.setTexture(&startBtn_text);

    _window.clear();
    _window.draw(startBtn);
    _window.display();
    bool signal = false;

    while(_window.isOpen()) {

        Event event;
        
        while(_window.pollEvent(event)) {
            switch (event.type)
            {
                case Event::Closed:
                    cout << "Closed Button Clicked\n";
                    _window.close();
                    break;
                case Event::MouseButtonPressed :
                    if(Mouse::isButtonPressed(Mouse::Left) == true) {
                        Vector2i mouse_pos = Mouse::getPosition(_window);
                        Vector2f btn_pos = startBtn.getPosition();
                        Vector2f btn_size = startBtn.getSize();

                        if(mouse_pos.x >= btn_pos.x && mouse_pos.x <= btn_pos.x + btn_size.x){
                            if(mouse_pos.y >= btn_pos.y && mouse_pos.y <= btn_pos.y + btn_size.y){
                                cout << "Start Button Clicked \n";
                                signal = true;
                            }
                        }
                    }
                    break;
            }
            
        }
        if(signal) {
            break;
        }
    }

}

int main(void){

    RenderWindow window(VideoMode(WINDOW_X_LEN, WINDOW_Y_LEN), "PING PONG GAME!!");
    window.setFramerateLimit(120);

    srand(time(NULL));
    
    GAME game;
    unsigned char game_result = -1;
    game.init_end(window, 1, game_result);
    
    while(window.isOpen()) {

        Event event;
        
        while(window.pollEvent(event)) {
            switch (event.type)
            {
                case Event::Closed:
                    cout << "Closed Button Clicked\n";
                    window.close();
                    break;
            }
            
        }
        /* update */
        game.update(window);

        /* clear */
        window.clear();

        /* draw */
        game.draw(window);

        /* display */
        window.display();
    }

    return 0;
}