#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>

#define WINDOW_X_LEN 1200
#define WINDOW_Y_LEN 1000

using namespace std;
using namespace sf;

int USER_HEART = 3;

class BALL {

public : 
    CircleShape ball;
    bool ball_alive;
    float ball_radius;
    float ball_offset_x, ball_offset_y;


    BALL(float ball_offset_x, float ball_offset_y, Vector2f ball_pos);
    void draw(RenderWindow& _window);
    void update(RenderWindow& _window);
};
BALL::BALL(float _ball_offset_x, float _ball_offset_y, Vector2f ball_pos) {

    ball_alive = true;
    ball_radius = 20;
    ball_offset_x = _ball_offset_x;
    ball_offset_y = _ball_offset_y;

    ball = CircleShape(ball_radius);
    ball.setFillColor(Color::White);
    ball.setPosition(ball_pos);

}
void BALL::draw(RenderWindow& _window){
    if(ball_alive == false){
        return;
    }
    _window.draw(ball);
}
void BALL::update(RenderWindow& _window){
    
    if(ball_alive == false) { // 목숨 끝났으면 끝
        return;
    }

    Vector2f ball_pos = ball.getPosition();

    // 왼쪽 벽에 맞았을 때
    if(ball_pos.x <= 0){
        if(ball_offset_x <= 0){
            ball_offset_x = -ball_offset_x;
        }
    }
    // 오른쪽 벽에 맞았을 때
    if(ball_pos.x + 2*ball_radius >= WINDOW_X_LEN){
        if(ball_offset_x >= 0){
            ball_offset_x = -ball_offset_x;
        }
    }
    // 위쪽 벽에 맞았을 때
    if(ball_pos.y <= 0){
        if(ball_offset_y <= 0){
            ball_offset_y = -ball_offset_y;
        }
    }
    // 아래 벽 맞았을 때, 목숨--
    if(ball_pos.y + 2*ball_radius >= WINDOW_Y_LEN){
        if(ball_offset_y >= 0){
            ball_offset_y = -ball_offset_y;
            USER_HEART--;
        }
    }
    ball.move(ball_offset_x, ball_offset_y);

}

class BALL_SET {

public: 
    vector<BALL> ball_vec;

    BALL_SET();
    void draw(RenderWindow& _window);
    void update(RenderWindow& _window);

};
BALL_SET::BALL_SET() {
    BALL temp(5, 5, Vector2f(WINDOW_X_LEN/2, WINDOW_Y_LEN/2));
    ball_vec.push_back(temp);
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
    }
}

class USER{

public :
    RectangleShape user;
    bool user_alive;
    
    USER();
    void draw(RenderWindow& _window);
    void update(RenderWindow& _window);
};
USER::USER() {
    user_alive = true;
    user = RectangleShape(Vector2f(200, 30));
    user.setFillColor(Color::Green);
    user.setPosition(WINDOW_X_LEN - user.getSize().x/2, WINDOW_Y_LEN - 40);
}
void USER::draw(RenderWindow& _window){
    if(user_alive == false){
        return;
    }
    _window.draw(user);
}
void USER::update(RenderWindow& _window){

    if(user_alive == false){
        return;
    }

    Vector2i mouse_pos = Mouse::getPosition(_window);
    Vector2f user_size = user.getSize();

    user.setPosition(mouse_pos.x - user_size.x/2 , WINDOW_Y_LEN - 40);

    if(mouse_pos.x - user_size.x/2 <= 0){
        user.setPosition(0, WINDOW_Y_LEN - 40);
    }
    if(mouse_pos.x + user_size.x/2 >= WINDOW_X_LEN) {
        user.setPosition(WINDOW_X_LEN - user_size.x , WINDOW_Y_LEN - 40);
    }

}

class RECT {

public :
    RectangleShape rect;
    bool rectAlive;         // 죽었는지?
    unsigned int rectHeart; // 목숨
    float rect_size_x, rect_size_y;

    RECT();
    void draw(RenderWindow& _window);
    void update(RenderWindow& _window);

};
RECT::RECT() {
    rectAlive = true;
    rectHeart = 3;   // 목숨 초기값 3
    rect_size_x = 120;
    rect_size_y = 40;

    rect = RectangleShape(Vector2f(rect_size_x, rect_size_y));
    rect.setFillColor(Color::Yellow);

}
void RECT::draw(RenderWindow& _window){
    if(rectAlive == false){
        return;
    }
    _window.draw(rect);
}
void RECT::update(RenderWindow& _window){
    if(rectAlive == false){
        return;
    }
}

class RECT_SET{

public :
    vector<RECT> rect_set;

    RECT_SET(int num = 6);

    void draw(RenderWindow& _window);
    void update(RenderWindow& _window);
};
RECT_SET::RECT_SET(int num) {

    for (int i = 0; i < num; i++)
    {
        RECT temp;
        temp.rect.setPosition(200*i + 50, 200);
        rect_set.push_back(temp);
    }

}
void RECT_SET::draw(RenderWindow& _window) {
    for (int i = 0; i < rect_set.size() ; i++)
    {
        rect_set[i].draw(_window);
    }
    
}
void RECT_SET::update(RenderWindow& _window) {
    for (int i = 0; i < rect_set.size() ; i++)
    {
        rect_set[i].update(_window);
    }
}

class GAME {

public: 
    
    RECT_SET rect_set;
    USER user;
    BALL_SET ball_set;

    Font font;
    Text user_heart_text;   // user 목숨
    Text rect_heart_text;   // rect 목숨 

    GAME();
    void draw(RenderWindow& _window);
    void update(RenderWindow& _window);
    void init_end(RenderWindow& _window, int status, unsigned char game_result);

};
GAME::GAME() {

    font.loadFromFile("images/OpenSans-Bold.ttf");
    
    user_heart_text.setFont(font);
    user_heart_text.setCharacterSize(50);
    user_heart_text.setFillColor(Color::Red);
    user_heart_text.setStyle(Text::Bold);

    rect_heart_text.setFont(font);
    rect_heart_text.setCharacterSize(50);
    rect_heart_text.setFillColor(Color::Red);
    rect_heart_text.setStyle(Text::Bold);

}
void GAME::draw(RenderWindow& _window){

    user.draw(_window);
    rect_set.draw(_window);
    ball_set.draw(_window);

    if(USER_HEART >= 1){
        _window.draw(user_heart_text);
    }

    for (int i = 0; i < rect_set.rect_set.size(); i++)
    {
        if(rect_set.rect_set[i].rectHeart == 0){
            continue;
        }
        rect_heart_text.setString(to_string(rect_set.rect_set[i].rectHeart));
        Vector2f rect_pos = rect_set.rect_set[i].rect.getPosition();
        Vector2f rect_size = rect_set.rect_set[i].rect.getSize();
        rect_heart_text.setPosition(rect_pos.x + rect_size.x/2 - 25 , rect_pos.y );
        _window.draw(rect_heart_text); // rect number draw
    }
    
    
}
void GAME::update(RenderWindow& _window){
    user.update(_window);
    ball_set.update(_window);

    // user 사망 선고
    if(USER_HEART == 0) {
        user.user_alive = false;
    }

    int sz = ball_set.ball_vec.size();
    cout << "sz : " << sz << endl;

    FloatRect user_area = user.user.getGlobalBounds();
    FloatRect ball_area;
    Vector2f user_pos = user.user.getPosition();

    // ball이 user에 맞았는지?
    for (int i = 0; i < sz; i++)
    {
        ball_area = ball_set.ball_vec[i].ball.getGlobalBounds();

        if(ball_area.intersects(user_area) == true) {
            if(ball_set.ball_vec[i].ball_offset_y >= 0){

                ball_set.ball_vec[i].ball_offset_y = -(ball_set.ball_vec[i].ball_offset_y-1);
                BALL temp(-ball_set.ball_vec[i].ball_offset_x, ball_set.ball_vec[i].ball_offset_y-1, ball_set.ball_vec[i].ball.getPosition());
                ball_set.ball_vec.push_back(temp);
                
            }
        }
    }

    // ball이 rect_set의 rect들에 맞았는지?

    FloatRect rect_area;
    Vector2f rect_pos;
    Vector2f ball_pos;
    for (int j = 0; j < ball_set.ball_vec.size(); j++)
    {
        ball_area = ball_set.ball_vec[j].ball.getGlobalBounds();
        ball_pos = ball_set.ball_vec[j].ball.getPosition();
        // if(ball_set.ball_vec[j].ball_alive == false){
        //     continue;
        // }

        for (int i = 0; i < rect_set.rect_set.size(); i++)
        {
            rect_area = rect_set.rect_set[i].rect.getGlobalBounds();
            rect_pos = rect_set.rect_set[i].rect.getPosition();

            if(rect_set.rect_set[i].rectHeart == false){ // 이미 죽은애들은 건너뜀
                continue;
            }

            if(ball_area.intersects(rect_area) == true){

                // ball_set.ball_vec[j].ball_alive = false;

                // ball이 rect_area의 위에 맞았는지?
                if(ball_pos.y < rect_pos.y) {
                    if(ball_set.ball_vec[j].ball_offset_y>=0){
                        ball_set.ball_vec[j].ball_offset_y = -ball_set.ball_vec[j].ball_offset_y;
                        rect_set.rect_set[i].rectHeart--;
                        if(rect_set.rect_set[i].rectHeart == 0){
                            rect_set.rect_set[i].rectAlive = false;
                        }
                    }
                }

                // ball이 rect_area의 아래에 맞았는지
                if(ball_pos.y > rect_pos.y) {
                    if(ball_set.ball_vec[j].ball_offset_y<=0){
                        ball_set.ball_vec[j].ball_offset_y = -ball_set.ball_vec[j].ball_offset_y;
                        rect_set.rect_set[i].rectHeart--;
                        if(rect_set.rect_set[i].rectHeart == 0){
                            rect_set.rect_set[i].rectAlive = false;
                        }                    
                    }
                }

            }
        }
    }   
    
    // user_heart_text update
    user_pos = user.user.getPosition();
    user_heart_text.setString(to_string(USER_HEART));
    user_heart_text.setPosition(user_pos.x + user.user.getSize().x/2 , user_pos.y - user.user.getSize().y);

    // game_result 판단
    if(user.user_alive == false){ // lose
        init_end(_window, 0, 0);
    }
    for (int i = 0; i < rect_set.rect_set.size(); i++)
    {
        if(rect_set.rect_set[i].rectAlive == true) { // 하나라도 살아있으면
            return;
        }
    }
    // 다 죽었다고 판단 --> win
    init_end(_window, 0, 1);
    
    
}
void GAME::init_end(RenderWindow& _window, int status, unsigned char game_result) {

    RectangleShape startBtn(Vector2f(500.0f, 300.0f));
    startBtn.setPosition(WINDOW_X_LEN/2 - 200, WINDOW_Y_LEN/2 -100);

    Texture startBtn_text;
    if(status){
        startBtn_text.loadFromFile("images/startBtn2.png");
    }
    else if(!status) {
        if(game_result == 0) {
            startBtn_text.loadFromFile("images/you_lose.png");
        }
        else {
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

            switch (event.type) {
                case Event::Closed:
                    _window.close();
                    break;
                case Event::MouseButtonPressed :
                    if(Mouse::isButtonPressed(Mouse::Left) == true) {
                        Vector2i mouse_pos = Mouse::getPosition(_window);
                        Vector2f btn_pos = startBtn.getPosition();
                        Vector2f btn_size = startBtn.getSize();

                        if(mouse_pos.x >= btn_pos.x && mouse_pos.x <= btn_pos.x + btn_size.x) {
                            if(mouse_pos.y >= btn_pos.y && mouse_pos.y <= btn_pos.y + btn_size.y) {
                                cout << "Start button clicked\n";
                                signal = true;
                            }
                        }
                    }
            }

        }
        if(signal){
            break;
        }
    }

}

int main(void){

    RenderWindow window(VideoMode(WINDOW_X_LEN, WINDOW_Y_LEN), "Swipe Block");
    window.setFramerateLimit(120);

    srand(time(NULL));

    GAME game;
    unsigned char game_result = -1;
    game.init_end(window, 1, game_result);

    while(window.isOpen()) {

        Event event;

        while(window.pollEvent(event)) {

            switch (event.type) {
                case Event::Closed:
                    window.close();
                    break;
            }

        }

        /* update */
        game.update(window);

        /* clear*/
        window.clear();

        /* draw*/
        game.draw(window);

        /* display*/
        window.display();

    }

    return 0;
}