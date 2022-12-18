#include <iostream>
#include <SFML/Graphics.hpp>
#include <ctime>
#include <cstdlib> 
#include <cmath>
#include <vector>

using namespace std;
using namespace sf;

#define XLEN 1500
#define YLEN 1200
#define AI_BALL_PERIOD 0.5

// user areawd
// 0 ~ XLEN/2 , YLEN/2 ~ YLEN

float current_time = 0; // 현재 흘러가는 시간
float final_time = 0; // 게임 결과가 날 때의 시간
float last_time = 0; 
int user_save = 3;
int ai_save = 3;
bool isStart = false;

void showGameResult(RenderWindow& _window) {


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
    finalText.setString("playing time : " + to_string(timeSec) + "."+to_string(timeMili));

    RectangleShape gameResult;
    gameResult = RectangleShape(Vector2f(800.0f, 500.0f));
    gameResult.setPosition(XLEN/2 - 400, YLEN/2 - 200);
    Texture gameResultTexture;
    if(ai_save == 0){ // 내가 이김
        gameResultTexture.loadFromFile("./images/win.png");
    }
    else {
        gameResultTexture.loadFromFile("./images/lose.png");
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

class START {

public :
    RectangleShape startBtn;
    Texture startTexture;
    START();
    void draw(RenderWindow& _window);
};
START::START() {
    startTexture.loadFromFile("./images/START.png");
    startBtn = RectangleShape(Vector2f(400, 200));
    startBtn.setTexture(&startTexture);
    startBtn.setPosition(XLEN/2 - 200, 30);
}
void START::draw(RenderWindow& _window) {
    _window.draw(startBtn);
}


class BAR{

public:
    RectangleShape bar;

    BAR();
    void draw(RenderWindow& _window);
};
BAR::BAR(){
    bar = RectangleShape(Vector2f(XLEN, 20));
    bar.setPosition(0, 250);
    bar.setFillColor(Color::Cyan);
}
void BAR::draw(RenderWindow& _window) {
    _window.draw(bar);
}

class BALL{

public :
    CircleShape ball;
    int randX, randY;
    float offsetX, offsetY;

    BALL();
    void draw(RenderWindow& _window);
    void update(RenderWindow& _window);
};
BALL::BALL() {
    ball.setRadius(15);
    ball.setFillColor(Color::Yellow);
    // setPosition은 game.fire에서

}
void BALL::draw(RenderWindow& _window){
    _window.draw(ball);
}
void BALL::update(RenderWindow& _window){
    ball.move(offsetX, offsetY);
}

class AI{

public :
    RectangleShape ai_area;

    Font font;
    Text aiSave;
    
    Texture aiTexture;
    CircleShape ai;

    float offsetX, offsetY;

    AI();
    void draw(RenderWindow& _window);
};
AI::AI() {
    ai_area = RectangleShape(Vector2f(XLEN/2 - 200, YLEN/2 - 100));
    ai_area.setFillColor(Color::Blue);
    ai_area.setPosition(XLEN - ai_area.getSize().x, 250 + 20);

    // ai_tank 불러오기
    aiTexture.loadFromFile("./images/AI_tank.png");
    ai = CircleShape(80);
    ai.setTexture(&aiTexture);
    ai.setPosition(XLEN - ai.getRadius()*2, 250 + 20);

    // ai_save 출력하기
    font.loadFromFile("images/OpenSans-Bold.ttf");
    aiSave.setFont(font);
    aiSave.setCharacterSize(100);
    aiSave.setFillColor(Color::Blue);
    aiSave.setStyle(Text::Bold);
    aiSave.setPosition(XLEN-200,50);
    aiSave.setString(to_string(ai_save));
}
void AI::draw(RenderWindow& _window){
    _window.draw(ai_area);
    _window.draw(ai);
    aiSave.setString(to_string(ai_save));
    _window.draw(aiSave);
}

class USER{

public:
    RectangleShape user_area;

    Font font;
    Text userSave;
    
    Texture userTexture;
    CircleShape user;

    float offsetX, offsetY;

    USER();
    void draw(RenderWindow& _window);
    void update(float _dx, float _dy);
};
USER::USER() {
    user_area = RectangleShape(Vector2f(XLEN/2 - 200, YLEN/2 - 100));
    user_area.setFillColor(Color::Red);
    user_area.setPosition(0, YLEN/2 + 100);

    // user_tank 불러오기
    userTexture.loadFromFile("./images/user_tank.png");
    user = CircleShape(80);
    user.setTexture(&userTexture);
    user.setPosition(0, YLEN - (user.getRadius()*2));

    // user_save 출력하기
    font.loadFromFile("images/OpenSans-Bold.ttf");
    userSave.setFont(font);
    userSave.setCharacterSize(100);
    userSave.setFillColor(Color::Red);
    userSave.setStyle(Text::Bold);
    userSave.setPosition(100,50);
    userSave.setString(to_string(user_save));
}
void USER::draw(RenderWindow& _window) {
    _window.draw(user_area);
    _window.draw(user);
    userSave.setString(to_string(user_save));
    _window.draw(userSave);
}
void USER::update(float _dx, float _dy) {
    Vector2f user_pos = user.getPosition();
    float finalX = user_pos.x + _dx;
    float finalY = user_pos.y + _dy;
    int user_2r = user.getRadius() * 2;
    Vector2f user_area_pos = user_area.getPosition();


    if(user_pos.x <= 0 && _dx < 0){
        finalX = 0;
    }
    if(user_pos.x + user_2r >= user_area_pos.x + user_area.getSize().x  && _dx > 0){
        finalX = user_area_pos.x + user_area.getSize().x - user_2r;
    } 
    if(user_pos.y <= user_area_pos.y && _dy < 0){
        finalY = user_area_pos.y;
    }
    if(user_pos.y + user_2r >= YLEN && _dy > 0){
        finalY = YLEN - user_2r;
    }
    user.setPosition(finalX, finalY);
    
}

class GAME{

public :

    BAR bar;

    USER user;
    vector <BALL> user_balls;

    AI ai;
    vector <BALL> ai_balls;

    GAME();
    void draw(RenderWindow& _window);
    void update(RenderWindow& _window);
    void fire(RenderWindow& _window);

};
GAME::GAME(){

}
void GAME::draw(RenderWindow& _window){

    bar.draw(_window);
    ai.draw(_window);
    user.draw(_window);
    
    // user_balls draw
    for(auto i : user_balls){
        i.draw(_window);
    }
    // ai_balls draw
    for(auto i : ai_balls){
        i.draw(_window);
    }

}
void GAME::update(RenderWindow& _window){
    
    // user_balls update
    for(int i = 0 ; i < user_balls.size() ; i++){
        user_balls[i].update(_window);
        Vector2f user_balls_pos = user_balls[i].ball.getPosition();
        FloatRect ai_area = ai.ai.getGlobalBounds();
        FloatRect ball_area = user_balls[i].ball.getGlobalBounds();

        // _window 밖으로 나가면 지우기
        if(user_balls_pos.x >= XLEN || user_balls_pos.x <= 0 || user_balls_pos.y >= YLEN || user_balls_pos.y <= 0) {
            user_balls.erase(user_balls.begin() + i);
            i--;
        }
        // bar 넘어가면 지우기
        else if(user_balls_pos.y <= bar.bar.getPosition().y + bar.bar.getSize().y){
            user_balls.erase(user_balls.begin() + i);
            i--;
        }
        // ai 맞으면 지우기, aiSave--;
        else if(ai_area.intersects(ball_area) == true){
            user_balls.erase(user_balls.begin() + i);
            i--;
            ai_save -= 1;
        }
    }

    // ai_balls update
    for (int i = 0; i < ai_balls.size(); i++)
    {
        ai_balls[i].update(_window);
        Vector2f ai_balls_pos = ai_balls[i].ball.getPosition();
        // _window 밖으로 나가면 지우기
        if(ai_balls_pos.x >= XLEN || ai_balls_pos.x <= 0 || ai_balls_pos.y >= YLEN || ai_balls_pos.y <= 0) {
            ai_balls.erase(ai_balls.begin() + i);
            i--;
        }
        FloatRect user_area = user.user.getGlobalBounds();
        FloatRect ball_area = ai_balls[i].ball.getGlobalBounds();
        if(user_area.intersects(ball_area) == true){
            ai_balls.erase(ai_balls.begin() + i);
            i--;
            user_save -= 1;
        }
    }
    

    // ai_balls create
    if(current_time - last_time >= AI_BALL_PERIOD) {
        cout << "ai ball 생성 시각 : " << current_time << endl;
        last_time = current_time;
        BALL temp_b;
        cout << "ball info : " << temp_b.randX << " " << temp_b.randY << endl;

        int randX = rand() % (int)user.user_area.getSize().x + user.user_area.getPosition().x;
        int randY = rand() % (int)user.user_area.getSize().y + user.user_area.getPosition().y;
        temp_b.ball.setPosition(Vector2f(ai.ai.getPosition().x , ai.ai.getPosition().y + ai.ai.getRadius()*2*(3/4)));

        Vector2f temp_b_pos = temp_b.ball.getPosition();
        int vecX = randX - temp_b_pos.x; 
        int vecY = randY - temp_b_pos.y; 
        float pita = sqrt((vecX*vecX) + (vecY*vecY));
        temp_b.offsetX = (vecX / pita) * 10;
        temp_b.offsetY = (vecY / pita) * 10;
        cout << "temp_b offsetX, offsetY : " << temp_b.offsetX << ", " << temp_b.offsetY << endl; 

        ai_balls.push_back(temp_b);
    }

    if(ai_save == 0 || user_save == 0){
        final_time = current_time;
        showGameResult(_window);
    }

}
void GAME::fire(RenderWindow& _window){
    Vector2i mouse_pos = Mouse::getPosition(_window);
    Vector2f user_pos = user.user.getPosition();

    BALL temp_b; // ball을 user 자리에 위치시킨다
    temp_b.ball.setPosition(Vector2f(user_pos.x + user.user.getRadius()/2 , user_pos.y));   

    Vector2f temp_b_pos = temp_b.ball.getPosition();    
    float vecX = mouse_pos.x - temp_b_pos.x;
    float vecY = mouse_pos.y - temp_b_pos.y;
    float pita = sqrt((vecX*vecX) + (vecY*vecY));
    temp_b.offsetX = (vecX / pita) * 10;
    temp_b.offsetY = (vecY / pita) * 10;
    cout << "temp_b offsetX, offsetY : " << temp_b.offsetX << ", " << temp_b.offsetY << endl; 

    user_balls.push_back(temp_b);
}

int main(void){

    srand(time(NULL));
    RenderWindow window(VideoMode(XLEN, YLEN), "practice");
    window.setFramerateLimit(120);

    GAME game;
    START start;

    Font font;
    Text timerText;
    font.loadFromFile("images/OpenSans-Bold.ttf");
    timerText.setFont(font);
    timerText.setCharacterSize(100);
    timerText.setFillColor(Color::White);
    timerText.setStyle(Text::Bold);
    timerText.setPosition(XLEN/2 - 100, 50);

    sf::Clock clock; // starts the clock
    while(window.isOpen()) {

        if(isStart) {
            sf::Time elapsed1 = clock.getElapsedTime();
            current_time = elapsed1.asSeconds(); // 1.52
            int timeSec = (int)(current_time * 100) / 100;
            int timeMili = (int)(current_time * 100) % 100;
            timerText.setString(to_string(timeSec) + '.'+to_string(timeMili));
        }

        Event event;

        while(window.pollEvent(event)) {
            
            switch (event.type) {
                case Event :: Closed :
                    cout << "Closed Button clicked\n";
                    window.close();
                    break;
                 case Event::KeyPressed :
                    if(Keyboard::isKeyPressed(Keyboard::P) == true) {
                        cout << "Display Time!\n";
                    }
                    break;
                case Event::MouseButtonPressed :
                    Vector2i mouse_pos = Mouse::getPosition(window);
                    Vector2f start_size = start.startBtn.getSize();
                    Vector2f start_pos = start.startBtn.getPosition();
                    if(isStart == false){
                        if(mouse_pos.x >= start_pos.x  && mouse_pos.x <= start_pos.x + start_size.x){
                            if(mouse_pos.y >= start_pos.y && mouse_pos.y <= start_pos.y + start_size.y){
                                cout << "start Button clicked\n";
                                isStart = true;
                                clock.restart(); // restarts the clock
                            }
                        }
                    }
                    else {
                        game.fire(window);
                    }
                    break;
                    
            }

        }

        if(isStart){
            if(Keyboard::isKeyPressed(Keyboard::W)) {
                game.user.update(0, -6);
            }
            if(Keyboard::isKeyPressed(Keyboard::S)) {
                game.user.update(0, 6);   
            }
            if(Keyboard::isKeyPressed(Keyboard::A)) {
                game.user.update(-6, 0);
            }
            if(Keyboard::isKeyPressed(Keyboard::D)) {
                game.user.update(6, 0);
            }
        }

        if(isStart){
            /* update */
            game.update(window);
        }
        /* clear */
        window.clear();

        /* draw */
        game.draw(window);

        /* timer */
        window.draw(timerText);

        if(!isStart){
            start.draw(window);
        }

        /* display */
        window.display();


    }

    return 0;
}