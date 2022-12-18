#include <iostream>
#include <SFML/Graphics.hpp>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <vector>

#define XLEN 1500
#define YLEN 1200
#define BALL_CREATE_SPEED 2
#define LIFE_CREATE_SPEED 5
#define SLOW_CREATE_SPEED 10

using namespace std;
using namespace sf;

bool isSlow = false;

bool isPause = false;

bool isStart = false;
int ball_count = 5;
int user_save = 3;
int fivePeriod = 0;

float slow_start_time = 0;
float current_time = 0;
float save_time = 0;
float last_time = 0;
float last_save_time = 0;
float last_slow_time = 0;
float final_time = 0;



class SLOW{

public :
    CircleShape slow;
    // Texture slowTexture;

    SLOW();
    void draw(RenderWindow& _window);
};
SLOW::SLOW() {
    // slowTexture.loadFromFile("images/slow.png");
    slow = CircleShape(20);
    slow.setFillColor(Color::Cyan);
    // slow.setTexture(&slowTexture);
    float randX = rand()%XLEN;
    float randY = rand()%YLEN;
    cout << "slow randX, randY : " << randX << ", " << randY << endl; 
    slow.setPosition(randX, randY);
}
void SLOW::draw(RenderWindow& _window){
    _window.draw(slow);
}

class LIFE{

public :
    CircleShape life;
    // Texture lifeTexture;

    LIFE();
    void draw(RenderWindow& _window);
};
LIFE::LIFE() {
    // lifeTexture.loadFromFile("./images/life.png");
    life = CircleShape(15);
    life.setFillColor(Color:: Red);
    // life.setTexture(&lifeTexture);
    float randX = rand()%XLEN;
    float randY = rand()%YLEN;
    life.setPosition(randX, randY);
}
void LIFE::draw(RenderWindow& _window){
    _window.draw(life);
}

void showGameResult(RenderWindow& _window) {

    Font font;
    Text game_over;
    Text timer;
    int timeSec = (int)(current_time * 100) / 100;
    int timeMili = (int)(current_time * 100) % 100;
    Text ball;

    font.loadFromFile("images/OpenSans-Bold.ttf");

    game_over.setFont(font);
    game_over.setCharacterSize(100);
    game_over.setFillColor(Color:: Red);
    game_over.setStyle(Text::Bold);
    game_over.setPosition(XLEN/2 - 300, YLEN/2 - 100);

    timer.setFont(font);
    timer.setCharacterSize(50);
    timer.setFillColor(Color:: Yellow);
    timer.setStyle(Text::Bold);
    timer.setPosition(XLEN/2 - 300, YLEN/2 + 50);

    ball.setFont(font);
    ball.setCharacterSize(50);
    ball.setFillColor(Color:: Yellow);
    ball.setStyle(Text::Bold);
    ball.setPosition(XLEN/2 - 300, YLEN/2 + 120);

    game_over.setString("GAME OVER");
    timer.setString("record : "+to_string(timeSec)+ '.' +to_string(timeMili));
    ball.setString("ball : " + to_string(ball_count));

    _window.clear();
    _window.draw(game_over);
    _window.draw(timer);
    _window.draw(ball);
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

    START();
    void draw(RenderWindow& _window);
};
START::START() {
    startTexture.loadFromFile("images/START.png");
    start = RectangleShape(Vector2f(400, 200));
    start.setTexture(&startTexture);
    start. setPosition(XLEN/2 - 200, 50);
}
void START::draw(RenderWindow& _window){
    _window.draw(start);
}

class USER {

public :
    RectangleShape user;

    USER();
    void draw(RenderWindow& _window);
    void update(float _dx, float _dy);
};
USER::USER() {
    user = RectangleShape(Vector2f(30, 30));
    user.setFillColor(Color::Blue);
    user.setPosition(XLEN/2 - user.getSize().x/2, YLEN/2 - user.getSize().y/2);
}
void USER::draw(RenderWindow& _window) {
    _window.draw(user);
}
void USER::update(float _dx, float _dy) {
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
    if(user_pos.y <= 0 && _dy < 0){
        finalY = 0;
    }
    if(user_pos.y + user_size.y >= YLEN && _dy > 0){
        finalY = YLEN - user_size.y;
    }
    user.setPosition(finalX, finalY);
}

class BALL {

public :

    CircleShape ball;
    int randX, randY;
    int offsetX, offsetY;

    BALL();
    void draw(RenderWindow& _window);
    void update(RenderWindow& _window);
};
BALL::BALL() {
    ball.setRadius(5);
    ball.setFillColor(Color::White);

    // randoom하게 처음 위치 세팅
    int xORy = rand()%2;
    int pORm = rand()%2;
    if(xORy == 0){ // 좌 우 화면에 공 처음 위
        if(pORm == 0){ //  0
            randX = 0;
            randY = rand() % YLEN;
        }
        else{ // XLEN
            randX = XLEN;
            randY = rand() % YLEN;
        }
    }
    else { // 상 하  화면에 공  처음 위
        if(pORm == 0){ // 0
            randX = rand()%XLEN;
            randY = 0;
        }
        else{ // YLEN
            randX = rand()%XLEN;
            randY = YLEN;
        }
    }
    ball.setPosition(randX, randY);

}
void BALL::draw(RenderWindow& _window){
    _window.draw(ball);
}
void BALL::update(RenderWindow& _window){
    if(isSlow) {
        // cout << "slow...\n";
        
        if(offsetX <= 0 && offsetY <= 0){ // 음 음
            ball.move(-1, -1);
        }
        if(offsetX <= 0 && offsetY >= 0){ // 음 양
            ball.move(-1, 1);
        }
        if(offsetX >= 0 && offsetY <= 0){ // 양 음
            ball.move(1, -1);
        }
        if(offsetX >= 0 && offsetY >=0){ // 양 양
            ball.move(1, 1);
        }
    }
    else {
        ball.move(offsetX, offsetY);
    }
}


class GAME{

public:

    USER user;

    Font font;
    Text lifeText;
    Text ballText;

    vector <BALL> balls;
    vector <LIFE> lifes;
    vector <SLOW> slows;

    GAME();
    void initFiveBall();
    void draw(RenderWindow& _window);
    void update(RenderWindow& _window);
};
GAME::GAME() {
    // 남은 생명
    font.loadFromFile("images/OpenSans-Bold.ttf");
    lifeText.setFont(font);
    lifeText.setCharacterSize(50);
    lifeText.setFillColor(Color:: Yellow);
    lifeText.setStyle(Text::Bold);
    lifeText.setPosition(30, YLEN - 60);

    // 남은 볼
    ballText.setFont(font);
    ballText.setCharacterSize(50);
    ballText.setFillColor(Color:: Yellow);
    ballText.setStyle(Text::Bold);
    ballText.setPosition(XLEN/2 - 50, YLEN - 60);
}
void GAME::draw(RenderWindow& _window){
    user.draw(_window);

    lifeText.setString("life : " +to_string(user_save));
    _window.draw(lifeText);

    ballText.setString("ball : " +to_string(ball_count));
    _window.draw(ballText);

    // ball draw
    for (int i = 0; i < balls.size(); i++)
    {
        balls[i].draw(_window);
    }
    // life draw
    for (int i = 0; i < lifes.size(); i++)
    {
        lifes[i].draw(_window);
    }
    // slow draw
    for (int i = 0; i < slows.size(); i++)
    {
        slows[i].draw(_window);
    }
    
}
void GAME::update(RenderWindow& _window){
    
    // ball update
    for (int i = 0; i < balls.size(); i++)
    {
        balls[i].update(_window);

        Vector2f ball_pos = balls[i].ball.getPosition();
        FloatRect ball_area = balls[i].ball.getGlobalBounds();
        FloatRect user_area = user.user.getGlobalBounds();

        // 공이 화면 밖으로 나가면 지우고 다시 그려준다 (숫자 유지)
        if(ball_pos.x <= 0 || ball_pos.x >= XLEN || ball_pos.y <= 0 || ball_pos.y >= YLEN){
            balls.erase(balls.begin() + i);
            i--;

            BALL temp;
            Vector2f temp_pos = temp.ball.getPosition();
            float randX = rand()%(XLEN-500) + 250;
            float randY = rand()%(YLEN-300) + 150;
            float vecX = randX - temp_pos.x;
            float vecY = randY - temp_pos.y;
            float pita = sqrt((vecX*vecX) + (vecY*vecY));
            int randSpeed = (int)rand()%4 + 4;
            temp.offsetX = (vecX / pita) * randSpeed;
            temp.offsetY = (vecY / pita) * randSpeed;
            // cout << "new ball's offset : " << temp.offsetX << " , " << temp.offsetY << endl;
            balls.push_back(temp);
        }
        else if(ball_area.intersects(user_area) == true) {
            user_save -= 1;
            balls.erase(balls.begin() + i);
            i--;
            BALL temp;
            Vector2f temp_pos = temp.ball.getPosition();
            float randX = rand()%(XLEN-500) + 250;
            float randY = rand()%(YLEN-300) + 150;
            float vecX = randX - temp_pos.x;
            float vecY = randY - temp_pos.y;
            float pita = sqrt((vecX*vecX) + (vecY*vecY));
            int randSpeed = (int)rand()%4 + 4;
            temp.offsetX = (vecX / pita) * randSpeed;
            temp.offsetY = (vecY / pita) * randSpeed;
            cout << "new ball's offset : " << temp.offsetX << " , " << temp.offsetY << endl;
            balls.push_back(temp);
        }

    }

    // ball create
    if(current_time - last_time >= BALL_CREATE_SPEED){ // 2초 경과하면
        ball_count += 1;
        last_time = current_time;
        fivePeriod += 1;
        if(fivePeriod == 5){ // sniper
            fivePeriod = 0;
            cout << "sniper\n";
            BALL temp;
            Vector2f temp_pos = temp.ball.getPosition();
            Vector2f user_pos = user.user.getPosition();
            float vecX = user_pos.x - temp_pos.x;
            float vecY = user_pos.y - temp_pos.y;
            float pita = sqrt((vecX*vecX) + (vecY*vecY));
            // int randSpeed = (int)rand()%4 + 4;
            temp.offsetX = (vecX / pita) * 6;
            temp.offsetY = (vecY / pita) * 6;
            // cout << "new ball's offset : " << temp.offsetX << " , " << temp.offsetY << endl;
            temp.ball.setFillColor(Color::Red);
            balls.push_back(temp);
        }

        else {
            BALL temp;
            Vector2f temp_pos = temp.ball.getPosition();
            float randX = rand()%(XLEN-500) + 250;
            float randY = rand()%(YLEN-300) + 150;
            float vecX = randX - temp_pos.x;
            float vecY = randY - temp_pos.y;
            float pita = sqrt((vecX*vecX) + (vecY*vecY));
            int randSpeed = (int)rand()%4 + 4;
            temp.offsetX = (vecX / pita) * randSpeed;
            temp.offsetY = (vecY / pita) * randSpeed;
            // cout << "new ball's offset : " << temp.offsetX << " , " << temp.offsetY << endl;
            balls.push_back(temp);
        }
    }

    // life update
    for (int i = 0; i < lifes.size(); i++)
    {
        FloatRect life_pos = lifes[i].life.getGlobalBounds();
        FloatRect user_area = user.user.getGlobalBounds();
        if(life_pos.intersects(user_area) == true){
            if(user_save <= 2){
                user_save += 1;
            }
            lifes.erase(lifes.begin() + i);
            i--;
        }

    }
    
    // slow update
    for (int i = 0; i < slows.size(); i++)
    {
        FloatRect slow_pos = slows[i].slow.getGlobalBounds();
        FloatRect user_area = user.user.getGlobalBounds();
        if(slow_pos.intersects(user_area) == true){
            slows.erase(slows.begin() + i);
            i--;
            isSlow = true;
            slow_start_time = current_time;
        }
    }
    if(current_time - slow_start_time >= 3) {
        isSlow = false;
    }

    // life create
    if(current_time - last_save_time >= LIFE_CREATE_SPEED ){
        if(lifes.size() == 3){ // 최대 3개
            last_save_time = current_time;
            return;
        }
        cout << "life item\n";
        last_save_time = current_time;
        LIFE temp;
        lifes.push_back(temp);
    }

    // slow create
    if(current_time - last_slow_time >= SLOW_CREATE_SPEED ){
        if(slows.size() == 1){ // 최대 1개
            last_slow_time = current_time;
            return;
        }
        cout << "slow item\n";
        last_slow_time = current_time;
        SLOW temp;
        slows.push_back(temp);
    }

    // game result
    if(user_save <= 0) {
        showGameResult(_window);
    }

}
void GAME::initFiveBall() {
    for (int i = 0; i < 5; i++)
    {
        BALL temp;
        Vector2f temp_pos = temp.ball.getPosition();
        float randX = rand()%(XLEN-500) + 250;
        float randY = rand()%(YLEN-300) + 150;
        float vecX = randX - temp_pos.x;
        float vecY = randY - temp_pos.y;
        float pita = sqrt((vecX*vecX) + (vecY*vecY));
        int randSpeed = (int)rand()%4 + 4;
        temp.offsetX = (vecX / pita) * randSpeed;
        temp.offsetY = (vecY / pita) * randSpeed;
        cout << "new ball's offset : " << temp.offsetX << " , " << temp.offsetY << endl;

        balls.push_back(temp);
    }
    
}


void gameInit(RenderWindow& _window) {

    START start;
    USER user;

    while(_window.isOpen()) {

        Event event;

        while(_window.pollEvent(event)){

            switch (event.type)
            {
            case Event:: Closed:
                _window.close();
                break;
            case Event::MouseButtonPressed :
                Vector2i mouse_pos = Mouse::getPosition(_window);
                Vector2f start_size = start.start.getSize();
                Vector2f start_pos = start.start.getPosition();
                if(mouse_pos.x >= start_pos.x && mouse_pos.x <= start_pos.x + start_size.x) {
                    if(mouse_pos.y >= start_pos.y && mouse_pos.y <= start_pos.y + start_size.y){
                        cout << "start  button clicked\n";
                        isStart = true;
                    }
                }

            }

        }
        _window.clear();
        start.draw(_window);
        user.draw(_window);
        _window.display();

        if(isStart){
            return;
        }

    }
}


int main(void){

    srand(time(NULL));
    RenderWindow window(VideoMode(XLEN, YLEN), "final");

    window.setFramerateLimit(120);

    GAME game;

    Text timer;
    Font font;
    font.loadFromFile("images/OpenSans-Bold.ttf");
    timer.setFont(font);
    timer.setCharacterSize(50);
    timer.setFillColor(Color:: Yellow);
    timer.setStyle(Text::Bold);
    timer.setPosition(XLEN - 150, YLEN - 60);

    gameInit(window);
    game.initFiveBall();

    sf::Clock clock;
    while(window.isOpen()) {

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
            case Event:: Closed:
                window.close();
                break;
            case Event:: KeyPressed: 
                if(isPause == true){
                    if(Keyboard::isKeyPressed(Keyboard::Space)) {
                        cout << "pause\n";
                        clock.restart();
                        current_time = save_time;
                        isPause = false;
                    }
                }
                else {
                    if(Keyboard::isKeyPressed(Keyboard::Space)) {
                        cout << "pause\n";
                        save_time = current_time;
                        isPause = true;
                    }
                }

            }

        }

        if(isPause == false){
            if(Keyboard::isKeyPressed(Keyboard::Up)){
                game.user.update(0, -8);
            }
            if(Keyboard::isKeyPressed(Keyboard::Down)){
                game.user.update(0, 8);
            }
            if(Keyboard::isKeyPressed(Keyboard::Left)){
                game.user.update(-8, 0);
            }
            if(Keyboard::isKeyPressed(Keyboard::Right)){
                game.user.update(8, 0);
            }
        }

        /* update */
        if(isPause == false){
            game.update(window);
        }

        window.clear();

        game.draw(window);

        // timer draw
        if(isPause == true){
            int timeSec = (int)(current_time * 100) / 100;
            int timeMili = (int)(current_time * 100) % 100;
            timer.setString(to_string(timeSec)+ '.' +to_string(timeMili));
            window.draw(timer);
        }
        else {
            int timeSec = (int)(current_time * 100) / 100;
            int timeMili = (int)(current_time * 100) % 100;
            timer.setString(to_string(timeSec)+ '.' +to_string(timeMili));
            window.draw(timer);
        }

        window.display();



    }

    return 0;
}