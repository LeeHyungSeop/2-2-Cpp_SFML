#include <iostream>
#include <SFML/Graphics.hpp>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <vector>

#define XLEN 1500
#define YLEN 1200
#define AI_BALL_SPEED 0.5

using namespace std;
using namespace sf;

float current_time = 0;
float save_time = 0; // pause할 때 시간 저장을 위해
float last_time = 0; // 공 생성을 위해
float final_time = 0; // show result할 때 최종 시간 보여주기

int user_save = 5;
int ai_save = 5;
bool isPause = true; // 

void showGameResult(RenderWindow& _window){

    Font font;
    Text finalText;

    font.loadFromFile("images/OpenSans-Bold.ttf");
    finalText.setFont(font);
    finalText.setCharacterSize(100);
    finalText.setFillColor(Color::White);
    finalText.setStyle(Text::Bold);
    finalText.setPosition(XLEN/2 - 450, 200);

    int timeSec = (int)(final_time * 100) / 100;
    int timeMili = (int)(final_time * 100) % 100;
    finalText.setString("play time : "+ to_string(timeSec) +"."+to_string(timeMili));

    RectangleShape gameResult;
    gameResult = RectangleShape(Vector2f(800, 500));
    gameResult.setPosition(XLEN/2 - 400, YLEN/2 - 200);
    Texture gameResultTexture;
    if(ai_save == 0){
        gameResultTexture.loadFromFile("images/win.png");
    }else {
        gameResultTexture.loadFromFile("images/lose.png");
    }
    gameResult.setTexture(&gameResultTexture);

    _window.clear();
    _window.draw(gameResult);
    _window.draw(finalText);
    _window.display();

    while(_window.isOpen()){
        Event e;
        while(_window.pollEvent(e)) {
            switch (e.type)
            {
            case Event::Closed:
                _window.close();
                break;
            }
        }
    }
}

class START{

public :
    RectangleShape start;
    Texture startTexture;
    RectangleShape pause;
    Texture pauseTexture;
    START();
    void draw(RenderWindow& _window);
};
START::START() {
    startTexture.loadFromFile("images/START.png");
    start = RectangleShape(Vector2f(400, 200));
    start.setTexture(&startTexture);
    start.setPosition(XLEN/2 - 200, 30);

    pauseTexture.loadFromFile("images/PAUSE.png");
    pause = RectangleShape(Vector2f(400, 200));
    pause.setTexture(&pauseTexture);
    pause.setPosition(XLEN/2 - 200, 30);
}
void START::draw(RenderWindow& _window) {
    if(isPause) {
        _window.draw(start);
    }
    else {
        _window.draw(pause);
    }
}

class BAR{

public :
    RectangleShape bar;

    BAR();
    void draw(RenderWindow& _window);

};
BAR::BAR() {
    bar = RectangleShape(Vector2f(XLEN, 20));
    bar.setPosition(0, 250);
    bar.setFillColor(Color::Cyan);
}
void BAR::draw(RenderWindow& _window){
    _window.draw(bar);
}

class BALL{

public :
    CircleShape ball;
    float offsetX, offsetY;

    BALL();
    void draw(RenderWindow& _window);
    void update(RenderWindow& _window);
};
BALL::BALL() {
    ball.setRadius(15);
    ball.setFillColor(Color:: Yellow);
}
void BALL::draw(RenderWindow& _window){
    _window.draw(ball);
}
void BALL::update(RenderWindow& _window){
    ball.move(offsetX, offsetY);
}

class USER{

public :

    Font font;
    Text userSave;

    Texture userTexture;
    RectangleShape user;

    USER();
    void draw(RenderWindow& _window);
    void update(float _dx, float _dy);
};
USER::USER() {

    // user_save 설정
    font.loadFromFile("images/OpenSans-Bold.ttf");
    userSave.setFont(font);
    userSave.setCharacterSize(100);
    userSave.setFillColor(Color::Red);
    userSave.setStyle(Text::Bold);
    userSave.setPosition(100, 50);

    // user 설정
    userTexture.loadFromFile("images/user_tank.png");
    user = RectangleShape(Vector2f(150,150));
    user.setTexture(&userTexture);
    user.setPosition(0, YLEN - user.getSize().y);
}
void USER::draw(RenderWindow& _window){

    userSave.setString(to_string(user_save));
    _window.draw(userSave);

    _window.draw(user);
}
void USER::update(float _dx, float _dy){
    Vector2f user_pos = user.getPosition();
    float finalX = user_pos.x + _dx;
    float finalY = user_pos.y + _dy;

    Vector2f user_size = user.getSize();

    if(user_pos.x <= 0 && _dx < 0){
        finalX = 0;
    }
    if(user_pos.x + user_size.x >= XLEN && _dx > 0){
        finalX = XLEN - user_size.x;
    }
    if(user_pos.y <= 270 && _dy < 0){
        finalY = 270;
    }
    if(user_pos.y + user_size.y >= YLEN && _dy > 0){
        finalY = YLEN - user_size.y;
    }
    user.setPosition(finalX, finalY);
}

class AI{

public :

    Font font;
    Text aiSave;

    Texture aiTexture;
    RectangleShape ai;

    AI();
    void draw(RenderWindow& _window);
    void update(RenderWindow& _window);
};
AI::AI() {

    // ai_save 설정
    font.loadFromFile("images/OpenSans-Bold.ttf");
    aiSave.setFont(font);
    aiSave.setCharacterSize(100);
    aiSave.setFillColor(Color::Blue);
    aiSave.setStyle(Text::Bold);
    aiSave.setPosition(XLEN - 200, 50);

    // ai 설정
    aiTexture.loadFromFile("images/AI_tank.png");
    ai = RectangleShape(Vector2f(150,150));
    ai.setTexture(&aiTexture);
    ai.setPosition(XLEN - ai.getSize().x, 270);
}
void AI::draw(RenderWindow& _window){
    _window.draw(ai);

    aiSave.setString(to_string(ai_save));
    _window.draw(aiSave);    
}

class GAME{

public:

    BAR bar;
    USER user;
    AI ai;

    vector <BALL> user_balls;
    vector <BALL> ai_balls;

    GAME();
    void draw(RenderWindow& _window);
    void update(RenderWindow& _window);
    void fire(RenderWindow& _window);

};
GAME::GAME() {

}
void GAME::draw(RenderWindow& _window){
    bar.draw(_window);
    user.draw(_window);
    ai.draw(_window);

    // user_balls draw
    for(int i = 0 ; i < user_balls.size() ; i++){
        user_balls[i].draw(_window);
    }
    // ai_balls draw
    for(int i = 0 ; i < ai_balls.size() ; i++){
        ai_balls[i].draw(_window);
    }
}
void GAME::update(RenderWindow& _window){
    
    // user_balls update
    for(int i = 0 ; i < user_balls.size() ; i++){
        user_balls[i].update(_window);

        Vector2f ball_pos = user_balls[i].ball.getPosition();
        Vector2f bar_pos = bar.bar.getPosition();
        Vector2f bar_size = bar.bar.getSize();
        FloatRect ai_area = ai.ai.getGlobalBounds();
        FloatRect ball_area = user_balls[i].ball.getGlobalBounds();
        // bar에 닿으면 삭제
        if(ball_pos.y <= bar_pos.y + bar_size.y){
            user_balls.erase(user_balls.begin() + i);
            i--;
        }
        // 화면을 넘어가면 삭제
        else if(ball_pos.x <= 0 || ball_pos.x >= XLEN || ball_pos.y >= YLEN) {
            user_balls.erase(user_balls.begin() + i);
            i--;
        }
        // AI에 부딪히면 삭제
        else if(ai_area.intersects(ball_area) == true) {
            user_balls.erase(user_balls.begin() + i);
            i--;
            ai_save -= 1;
        }

    }
    
    // ai_balls update
    for(int i = 0 ; i < ai_balls.size() ; i++){
        ai_balls[i].update(_window);

        Vector2f ball_pos = ai_balls[i].ball.getPosition();
        Vector2f bar_pos = bar.bar.getPosition();
        Vector2f bar_size = bar.bar.getSize();
        FloatRect user_area = user.user.getGlobalBounds();
        FloatRect ball_area = ai_balls[i].ball.getGlobalBounds();
        // bar에 닿으면 삭제
        if(ball_pos.y <= bar_pos.y + bar_size.y){
            ai_balls.erase(ai_balls.begin() + i);
            i--;
        }
        // 화면을 넘어가면 삭제
        else if(ball_pos.x <= 0 || ball_pos.x >= XLEN || ball_pos.y >= YLEN) {
            ai_balls.erase(ai_balls.begin() + i);
            i--;
        }
        // AI에 부딪히면 삭제
        else if(user_area.intersects(ball_area) == true) {
            ai_balls.erase(ai_balls.begin() + i);
            i--;
            user_save -= 1;
        }

    }

    // ai_balls create
    if(current_time - last_time >= AI_BALL_SPEED) {
        cout << "ai ball create time : " << current_time << endl;
        last_time = current_time;
        Vector2f user_pos = user.user.getPosition();
        Vector2f user_size = user.user.getSize();
        Vector2f ai_pos = ai.ai.getPosition();
        Vector2f ai_size = ai.ai.getSize();

        BALL temp;
        temp.ball.setPosition(Vector2f(ai_pos.x, ai_pos.y + ai_size.y));
        temp.ball.setFillColor(Color::Blue);
        float vecX = (user_pos.x + user_size.x) - (temp.ball.getPosition().x); 
        float vecY = (user_pos.y) - (temp.ball.getPosition().y);
        float pita = sqrt((vecX*vecX) + (vecY*vecY));
        temp.offsetX = (vecX / pita) * 10;
        temp.offsetY = (vecY / pita) * 10;
        cout << "created ball's (offsetX, offsetY) : " << temp.offsetX << ", " << temp.offsetY << endl;

        ai_balls.push_back(temp);
    }

    // game result update
    if(ai_save == 0 || user_save == 0){
        final_time = current_time;
        showGameResult(_window);
    }

}
void GAME::fire(RenderWindow& _window){
    Vector2i mouse_pos = Mouse::getPosition(_window);
    Vector2f user_pos = user.user.getPosition();
    Vector2f user_size = user.user.getSize();

    BALL temp;
    temp.ball.setPosition(user_pos.x + user_size.x , user_pos.y);
    temp.ball.setFillColor(Color::Red);
    float vecX = mouse_pos.x - (temp.ball.getPosition().x); 
    float vecY = mouse_pos.y - (temp.ball.getPosition().y);
    float pita = sqrt((vecX*vecX) + (vecY*vecY));
    temp.offsetX = (vecX / pita) * 10;
    temp.offsetY = (vecY / pita) * 10;
    cout << "created ball's (offsetX, offsetY) : " << temp.offsetX << ", " << temp.offsetY << endl;
    
    user_balls.push_back(temp);

}

int main(void){

    srand(time(NULL));
    RenderWindow window(VideoMode(XLEN, YLEN), "Fortless");
    window.setFramerateLimit(120);

    START start;
    GAME game;

    // timer setting (start)
    Font font;
    Text timer;
    font.loadFromFile("images/OpenSans-Bold.ttf");
    timer.setFont(font);
    timer.setCharacterSize(100);
    timer.setFillColor(Color::White);
    timer.setStyle(Text::Bold);
    timer.setPosition(XLEN/2 - 100, YLEN/2);

    sf::Clock clock;
    while(window.isOpen()){

        if(isPause == false){
            sf::Time elpased = clock.getElapsedTime();
            current_time = elpased.asSeconds() + save_time;
            int timeSec = (int)(current_time * 100) / 100;
            int timeMili = (int)(current_time * 100) % 100;
        }

        Event event;
        while(window.pollEvent(event)){
            switch (event.type)
            {
            case Event::Closed:
                cout<< "closed button clicked\n";
                window.close();
                break;
            case Event::MouseButtonPressed :
                if(isPause == true){
                    Vector2i mouse_pos = Mouse::getPosition(window);
                    Vector2f start_size = start.start.getSize();
                    Vector2f start_pos = start.start.getPosition();
                    if(mouse_pos.x >= start_pos.x && mouse_pos.x <= start_pos.x + start_size.x) {
                        if(mouse_pos.y >= start_pos.y && mouse_pos.y <= start_pos.y + start_size.y) {
                            cout << "start Button clicked\n";
                            clock.restart();
                            current_time = save_time;
                            isPause = false;
                        }
                    }
                }
                else {
                    Vector2i mouse_pos = Mouse::getPosition(window);
                    Vector2f start_size = start.pause.getSize();
                    Vector2f start_pos = start.pause.getPosition();
                    if(mouse_pos.x >= start_pos.x && mouse_pos.x <= start_pos.x + start_size.x) {
                        if(mouse_pos.y >= start_pos.y && mouse_pos.y <= start_pos.y + start_size.y) {
                            cout << "pause Button clicked\n";
                            save_time = current_time;
                            isPause = true;
                        }
                    }
                    if(isPause == false){
                        game.fire(window);
                    }
                }
            }
            
        }

        if(isPause == false){
            if(Keyboard::isKeyPressed(Keyboard::W)){
                game.user.update(0, -6);
            }
            if(Keyboard::isKeyPressed(Keyboard::S)){
                game.user.update(0, 6);
            }
            if(Keyboard::isKeyPressed(Keyboard::A)){
                game.user.update(-6, 0);
            }
            if(Keyboard::isKeyPressed(Keyboard::D)){
                game.user.update(6, 0);
            }
        }

        /* update */
        if(isPause == false){
            game.update(window);
        }

        /* clear */
        window.clear();

        /* draw */
        game.draw(window);

        /* start draw */
        start.draw(window);

        /* when paused, timer draw */
        if(isPause == true){
            int timeSec = (int)(save_time * 100) / 100;
            int timeMili = (int)(save_time * 100) % 100;
            timer.setString(to_string(timeSec)+ '.' +to_string(timeMili));
            window.draw(timer);
        }


        /* display */
        window.display();
    }


    return 0;
}