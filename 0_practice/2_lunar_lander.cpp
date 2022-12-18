#include <SFML/Graphics.hpp>
#include <unistd.h> // windows.h 대신사용 Sleep(sec)
#include <string>
#include <iostream>

using namespace sf;
using namespace std;

class LunarLander {

private :

    double x, y;                // 착륙선의 위치
    double velocity;            // 착륙속도
    double fuel;                // 연료량

    string status;              // 현재 상태
    Texture t1,t2;              // 패턴, 무늬 (t1 : 우주선, t2 : 불꽃)
    Sprite burst;               // 실제로 움직이는 조각 (캐릭터, 우주선 등)
    Sprite* spaceship = nullptr;
        // unique_ptr<Sprite> spaceship;
    Font font;                  // 화면에 찍어줄 문자열 font
    Text text;                  // 화면에 찍어줄 문자열

public :   

    LunarLander(double h, double v, double f);
    ~LunarLander(); // spaceship 때문에 메모리 해제를 위해서 소멸자 필요
    void update(double dx, double dy);  // dx, dy는 방향키 누름에 따라서 변하는 x,y 위치
    void draw(RenderWindow& window);    // RenderWindow : 게임이 그래픽적으로 그려지는 창

    void setXY(double x, double y){ 
        this->x = x;    // 명확하게 member variable이라는 것을 명시할 수 있다.
        this->y = y;    // 따라서 this pointer 사용
    }
        // void setXY(double _x, double _y){
        //     x = _x;
        //     y = _y;   
        // }
};      

// constructor
LunarLander::LunarLander(double h, double v, double f) {

    // 달 착륙선 이미지를 texture t1에 로딩한다
    t1.loadFromFile("images/spaceship.png");
    spaceship = new Sprite();
    spaceship->setTexture(t1); // 달 착륙선 sprite에 t1을 가져다 붙인다.
        // spaceship.reset(new Sprite()); -> smart pointer는 실제로 포인터라 객체니까. reset(주소) method를 이용해서 공간 할당
    x = 500.0;
    this -> x = 500.0;  // 
    y = 200.0;
    spaceship->setPosition(x, y); // 초기에 500, 200에 위치시킬 것이다
}

LunarLander::~LunarLander() {
    delete spaceship;
}

// Functions
void LunarLander::update(double dx, double dy) {
    // 왼쪽 화살표를 누르면 dx = -10
    // 오른쪽 화살표를 누르면 dx = +10
    // 아래쪽 화살표를 누르면 dy = +10
    // 위쪽 화살표를 누르면 dy = -10
    x += dx;
    y += dy;
    spaceship->setPosition(x, y);
}

void LunarLander::draw(RenderWindow& window) {
    window.draw(*spaceship);
}

int main(void) {

    // 게임이 나타나는 창을 만든다.
    RenderWindow window(VideoMode(800, 500), "Lunar Lander");
        // 1000*800 크기의 창을 만들고, 제목은 "Lunar Lander"
    window.setFramerateLimit(60);
        // 60fps, 초당 60번 무한루프
    
    // Sprite는 실제로 화면에 그려지는 요소이고,
    // Texture는 그 Sprite 위에 그려지는 패턴
    Texture t;          
    Sprite background;  
    t.loadFromFile("images/background.png");
    background.setTexture(t);

    // 무한 루프 들어가기 전에 달착륙선을 만든다
    // 0 0 0은 사용하지 않기 때문에 아무거나 생성자에 입력
    LunarLander* lander1 = new LunarLander(0.0 ,0.0 ,0.0);  // 달착륙선1
    unique_ptr<LunarLander> lander2(new LunarLander(0.0, 0.0, 0.0)); // 달착륙선2

    // 윈도우가 열려 있으면 계속해서 반복 (초당 60번)
    while(window.isOpen()) {

        Event e;

        while (window.pollEvent(e)) {
            // 마우스로 window 닫힘버튼을 누르면 닫힘
            if(e.type == Event :: Closed) {
                window.close();
            }
            // 키보드로 esc 버튼 누르면 닫힘
            if(e.key.code == Keyboard::Escape) 
                window.close();
        }
        // lander1 키
        if (Keyboard::isKeyPressed(Keyboard::Up)) {
            lander1->update(0, -5);
        }  
        if (Keyboard::isKeyPressed(Keyboard::Down)) {
            lander1->update(0, 5);
        }
        if (Keyboard::isKeyPressed(Keyboard::Left)) {
            lander1->update(-5, 0);
        }
        if (Keyboard::isKeyPressed(Keyboard::Right)) {
            lander1->update(5, 0);
        }
        // lander2 키
        if (Keyboard::isKeyPressed(Keyboard::W)) {
            lander2->update(0, -5);
        }  
        if (Keyboard::isKeyPressed(Keyboard::S)) {
            lander2->update(0, 5);
        }
        if (Keyboard::isKeyPressed(Keyboard::A)) {
            lander2->update(-5, 0);
        }
        if (Keyboard::isKeyPressed(Keyboard::D)) {
            lander2->update(5, 0);
        }

        window.clear();             // RenderWindow를 싹 지움
        window.draw(background);    // RenderWindow에 sprite인 background를 그린다
        lander1->draw(window);      // spaceship을 그린다
        lander2->draw(window);      // spaceship2 그린다
        window.display();           // RenderWindow를 실제로 화면에 띄운다

    }

    delete lander1;

    return 0;
}