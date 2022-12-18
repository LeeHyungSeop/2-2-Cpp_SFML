#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

#define X_LEN 800
#define Y_LEN 1200
#define TIME 10
#define FRAME_RATE 60

using namespace std;
using namespace sf;

int timer = TIME; 
int attackMode = 1;
int itemFlag = 0;
bool isStart = false;

class ENEMY{

    // 2p 만들어서 w, a, s, d로 입력 받고
    // 서로 쏜 총을 맞으면 목숨 -1
    // 목숨 0이 되면 그 사람 짐

};

class START{

public :
    Texture startTexture;
    RectangleShape start;

    START();
    void draw(RenderWindow& _window);
};
START::START() {
    startTexture.loadFromFile("images/START.png");
    start = RectangleShape(Vector2f(400,200));
    start.setTexture(&startTexture);
    start.setPosition(X_LEN/2 - 200, Y_LEN/2 - 100);
}
void START::draw(RenderWindow& _window) {
    _window.draw(start);
}

class ITEM{

    // item 먹으면 attackMode = 2 돼서 2발 발사 되도록..
public: 
    int isAlive;
    int firstAppear = 0; // 처음 등장한 시간
    Texture itemTexture;
    RectangleShape item;

    ITEM();
    void draw(RenderWindow& _window);

};
ITEM::ITEM() {
    isAlive = true;
    itemTexture.loadFromFile("images/item.png");
    item = RectangleShape(Vector2f(50,50));
    item.setTexture(&itemTexture);

    int randX = rand()%(X_LEN/2) + X_LEN/4;
    int randY = rand()%(Y_LEN/2) + Y_LEN/4;

    item.setPosition(randX, randY);
}
void ITEM::draw(RenderWindow& _window){
    if(isAlive == true){
        _window.draw(item);
    }
}
class BULLET {

public :
    bool isAlive;
    float offsetY;
    RectangleShape burst;  // 
    BULLET(Texture& _burstTexture);
    void draw(RenderWindow& _window);
    void update(RenderWindow& _window);
};
BULLET::BULLET(Texture& _burstTexture) {
    isAlive = true;
    offsetY = -15.0f;
    burst = RectangleShape(Vector2f(15,40)); // burst.getSize() method를 사용하기 위해서 Sprite가 아닌 Rectangle shape 사용
    burst.setTexture(&_burstTexture);
    // setPostion은 game에서 user 위치에다가 세팅    
}
void BULLET::draw(RenderWindow& _window) {
    if(!isAlive){
        return;
    }
    _window.draw(burst);
}
void BULLET::update(RenderWindow& _window) {

    burst.move(0, offsetY);
    Vector2f burst_pos = burst.getPosition();
    if(burst_pos.y <= 0) {
        isAlive = false;
    }
}

class USER {

public :

    Texture userTexture;
    RectangleShape user; // user.getSize() method를 사용하기 위해서 Sprite가 아닌 Rectangle shape 사용

    USER();
    void draw(RenderWindow& _window);
    void update(float dx, float dy);

};
USER::USER() {

    // user spaceship 세팅
    userTexture.loadFromFile("images/spaceship.png");
    user = RectangleShape(Vector2f(70,70));
    user.setTexture(&userTexture);
    user.setPosition(X_LEN/2, Y_LEN - 100);
}
void USER::draw(RenderWindow& _window) {
    _window.draw(user);
}  
void USER::update(float dx, float dy) {

    Vector2f user_pos = user.getPosition();
    float finalX = user_pos.x + dx;
    float finalY = user_pos.y + dy;
    Vector2f user_size = user.getSize();

    if(user_pos.x <= 0 && dx < 0){
        finalX = 0;
    }
    if(user_pos.x + user_size.x >= X_LEN && dx > 0){
        finalX = X_LEN - user_size.x;
    } 
    if(user_pos.y <= 0 && dy < 0){
        finalY = 0;
    }
    if(user_pos.y + user_size.y >= Y_LEN && dy > 0){
        finalY = Y_LEN - user_size.y;
    } 

    user.setPosition(finalX, finalY);
}

class GAME {

public :

    USER user;
    ITEM item;
    START start;

    Texture bgTexture;
    RectangleShape bg; 

    Texture burstTexture;
    vector<BULLET> bullets;

    Font font;
    Text _timerText; 
    char _timerX, _timerY;

    Texture userTexture;

    GAME();
    void update(RenderWindow& _window);
    void draw(RenderWindow& _window);
    void fire();
};
GAME::GAME() { 
    // 배경 세팅
    bgTexture.loadFromFile("images/space.png");
    bg = RectangleShape(Vector2f(X_LEN, Y_LEN));
    bg.setTexture(&bgTexture);
    // 총알 세팅
    burstTexture.loadFromFile("images/burst.png");

    font.loadFromFile("images/OpenSans-Bold.ttf");

    // 남은 시간 출력할 text 세팅
    _timerText.setFont(font);
    _timerText.setCharacterSize(50);
    _timerText.setFillColor(Color::Green);
    _timerText.setStyle(Text::Bold);
    _timerX = 80;
    _timerY = 50;
    _timerText.setPosition(_timerX, _timerY);
    _timerText.setString(to_string(timer));
    
}
void GAME::update(RenderWindow& _window){
    _timerText.setString(to_string(timer)); // timer값에 따라 update
    if(timer <= 5) {
        _timerText.setFillColor(Color::Red);
        _timerText.setCharacterSize(50 + (5-timer)*25); // 점점 크게
    }

    // bullets update
    for (int i = 0; i < bullets.size(); i++)
    {
        bullets[i].update(_window);
        if(bullets[i].isAlive == false){ // 총알이 죽었다면? vector에서 지운다
            bullets.erase(bullets.begin() + i);
            i--;
        }
    }
    
    // item 먹었는지 판단
    if(itemFlag) {  
        FloatRect item_area = item.item.getGlobalBounds();
        FloatRect user_area = user.user.getGlobalBounds();
        if(user_area.intersects(item_area) == true) {
            itemFlag = 0;
            item.isAlive = false;
            attackMode++;
            item.firstAppear = timer;
        }
        if(timer - item.firstAppear == 3) { // 3초 경과했으면 없앤다
            cout << "item 등장한지 3초 경과\n";
            itemFlag = 0;
            item.isAlive = false;
        }
    }
    
    
}
void GAME::draw(RenderWindow& _window){
    if(isStart == false){
        _window.draw(bg);
        _window.draw(_timerText); // bg를 가장 먼저 그려줘야 한다.
        start.draw(_window);
        user.draw(_window);
        return;
    }
    _window.draw(bg);
    _window.draw(_timerText);

    // user draw
    user.draw(_window);

    // bullets draw
    for (int i = 0; i < bullets.size(); i++)
    {
        bullets[i].draw(_window);
    }
    // item draw
    if(itemFlag) {
        item.draw(_window);
    }
}
void GAME::fire() {

    if(attackMode == 1){
        BULLET temp(burstTexture);
        Vector2f user_pos = user.user.getPosition();
        Vector2f user_size = user.user.getSize();
        Vector2f temp_size = temp.burst.getSize();
        
        temp.burst.setPosition(user_pos.x + user_size.x/2 - temp_size.x/2, user_pos.y);
        bullets.push_back(temp);
    }
    else if(attackMode == 2){
        BULLET temp1(burstTexture);
        BULLET temp2(burstTexture);
        Vector2f user_pos = user.user.getPosition();
        Vector2f user_size = user.user.getSize();
        Vector2f temp_size = temp1.burst.getSize();
        
        temp1.burst.setPosition(user_pos.x + user_size.x/2 - temp_size.x/2 + 15, user_pos.y);
        bullets.push_back(temp1);
        temp2.burst.setPosition(user_pos.x + user_size.x/2 - temp_size.x/2 - 15, user_pos.y);
        bullets.push_back(temp2);
    }
    

}

int main(void) {

    srand(time(NULL));

    RenderWindow window(VideoMode(X_LEN, Y_LEN), "SHOOTING GAME");
    window.setFramerateLimit(FRAME_RATE);
    GAME game;
    int timer_temp = 1;

    while(window.isOpen()) {

        Event e;
        while(window.pollEvent(e)) {

                switch(e.type) {
                    case Event::Closed :
                        cout << "Closed button clicked\n";
                        window.close();
                        break;
                    case Event::KeyPressed :
                        if(Keyboard::isKeyPressed(Keyboard::Space)) {
                            cout << "spacebar clicked\n";
                            if(isStart){
                                game.fire();
                            }
                        }
                    case Event::MouseButtonPressed :
                        if(Mouse::isButtonPressed(Mouse::Left) == true) {
                            Vector2i mouse_pos = Mouse::getPosition(window);
                            if(mouse_pos.x >= X_LEN/2 - 200 && mouse_pos.x <= X_LEN/2 + 200) {
                                if(mouse_pos.y >= Y_LEN/2 - 100 && mouse_pos.y <= Y_LEN/2 +100){
                                    isStart = true; 
                                }
                            }
                        }
                }
        }

        if(isStart){
            if(Keyboard::isKeyPressed(Keyboard::W)) {
                game.user.update(0, -12);
            }
            if(Keyboard::isKeyPressed(Keyboard::S)) {
                game.user.update(0, 12);   
            }
            if(Keyboard::isKeyPressed(Keyboard::A)) {
                game.user.update(-12, 0);
            }
            if(Keyboard::isKeyPressed(Keyboard::D)) {
                game.user.update(12, 0);
            }
            // item 출현 (먹으면 총알 2개로 나가기)
            if(timer == 6 && timer_temp == 1){
                cout << "item appeared\n";
                itemFlag = 1;
            }
            // timer update
            if(timer_temp % FRAME_RATE == 0) {
                timer--;
                timer_temp = 0;
                cout << timer << endl;
            }
            if(timer == 0){
                break;
            }
            timer_temp++;
        }


        /* update */
        if(isStart) {
            game.update(window);
        }

        /* clear */
        window.clear();

        /* draw */
        game.draw(window);

        /* display */
        window.display();

    }

    return 0;
}
