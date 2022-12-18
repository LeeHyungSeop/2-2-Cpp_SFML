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
#define BALL_PERIOD 0.5

bool displayTime = false;
float last_time = 0;
float current_time = 0;

class DEST{

public :
    CircleShape dest;
    int randX, randY;
    int radius = 16;
    
    DEST();
    void draw(RenderWindow& _window);
};
DEST::DEST() {
    randX = (rand() % 300) + (XLEN-500); // XLEN-300 ~ XLEN-200
    randY = (rand() % 300) + (YLEN-500); // YLEN-300 ~ YLEN-200

    dest = CircleShape(radius);
    dest.setFillColor(Color::Blue);
    dest.setPosition(randX, randY);
}
void DEST::draw(RenderWindow& _window) {
    _window.draw(dest);
}

class BALL {

public :
    bool isAlive;
    CircleShape ball;
    float radius;
    int randX, randY;
    float offsetX, offsetY;

    BALL();
    void draw(RenderWindow& _window);
    void update(RenderWindow& _window);
};
BALL::BALL() {
    isAlive = true;
    radius = 15;
    randX = (rand() % 100) + 200; // 100 ~ 300
    randY = (rand() % 100) + 200; // 100 ~ 300

    ball = CircleShape(radius);
    ball.setFillColor(Color::Yellow);
    ball.setPosition(randX, randY);
}
void BALL::draw(RenderWindow& _window) {
    if(isAlive){
        _window.draw(ball);
    }
}
void BALL::update(RenderWindow& _window) {
    if(isAlive){
        ball.move(offsetX, offsetY);
    }
}


class GAME {

public :
    vector <BALL> ball;
    vector <DEST> dest;
    float vecX, vecY, pita;


    GAME();
    void draw(RenderWindow& _window);
    void update(RenderWindow& _window);
};
GAME::GAME() {
    BALL temp_b;
    DEST temp_d;

    cout << "ball info : " << temp_b.randX << " " << temp_b.randY << endl;
    cout << "dest info : " << temp_d.randX << " " << temp_d.randY << endl;
    vecX = temp_d.randX - temp_b.randX; 
    vecY = temp_d.randY - temp_b.randY; 
    pita = sqrt((vecX*vecX) + (vecY*vecY));
    temp_b.offsetX = (vecX / pita) * 7;
    temp_b.offsetY = (vecY / pita) * 7;
    cout << "temp_b offsetX, offsetY : " << temp_b.offsetX << ", " << temp_b.offsetY << endl; 

    ball.push_back(temp_b);
    dest.push_back(temp_d);
}
void GAME::draw(RenderWindow& _window) {
    for(auto& i : ball){
        i.draw(_window);
    }
    for(auto& i : dest){
        i.draw(_window);
    }
}
void GAME::update(RenderWindow& _window) {

    if(current_time - last_time >= BALL_PERIOD) {
        cout << "ball 생성 시각 : " << current_time << endl;
        last_time = current_time;
        BALL temp_b;
        DEST temp_d;
        cout << "ball info : " << temp_b.randX << " " << temp_b.randY << endl;
        cout << "dest info : " << temp_d.randX << " " << temp_d.randY << endl;
        vecX = temp_d.randX - temp_b.randX; 
        vecY = temp_d.randY - temp_b.randY; 
        pita = sqrt((vecX*vecX) + (vecY*vecY));
        temp_b.offsetX = (vecX / pita) * 7;
        temp_b.offsetY = (vecY / pita) * 7;
        cout << "temp_b offsetX, offsetY : " << temp_b.offsetX << ", " << temp_b.offsetY << endl; 

        ball.push_back(temp_b);
        dest.push_back(temp_d);
    }

    for(int i = 0 ; i < ball.size() ; i++){
        ball[i].update(_window);

        Vector2f ball_pos = ball[i].ball.getPosition();
        if(ball_pos.x >= XLEN || ball_pos.y >= YLEN) {
            ball[i].isAlive = false;
            ball.erase(ball.begin() + i);
            dest.erase(dest.begin() + i);
            i--;
            cout << "ball이 destination 도달\n\n";
        }
    }

}

int main(void){

    srand(time(NULL));
    RenderWindow window(VideoMode(XLEN, YLEN), "practice");
    window.setFramerateLimit(120);

    GAME game;

    sf::Clock clock; // starts the clock
    while(window.isOpen()) {

        sf::Time elapsed1 = clock.getElapsedTime();
        current_time = elapsed1.asSeconds();
        // cout << "current_time : " << round(current_time*100) / 100 << endl;

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
                        // ---
                        displayTime = true;
                    }
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

        if(displayTime){
            cout << "Play 시간 : " << round(current_time*100)/100 << endl;
            break;
        }

    }

    return 0;
}