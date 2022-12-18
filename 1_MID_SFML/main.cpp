#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector> 
#include <algorithm>
#include <string>

#define WINDOW_X_LEN 1500
#define WINDOW_Y_LEN 1000
#define GOAL_SCORE 3

using namespace std;
using namespace sf;

unsigned char user_score = 0;
unsigned char enemy_score = 0;
int lets_play = 0; // 버튼 누르면 1 -> animation 실행

// 결과가 나면 이 함수 실행
void show_result(RenderWindow& _window, string _result) {

    RectangleShape game_result;
    game_result = RectangleShape(Vector2f(800, 500));
    game_result.setPosition(WINDOW_X_LEN/2 - game_result.getSize().x/2, WINDOW_Y_LEN/2 - game_result.getSize().y/2);
    Texture game_result_text;

    if(_result == "user") {
        game_result_text.loadFromFile("images/WIN.png");
    }
    else {
        game_result_text.loadFromFile("images/LOSE.png");
    }
    game_result.setTexture(&game_result_text);

    while(_window.isOpen()) {

        Event event;
        while(_window.pollEvent(event)) {
            switch(event.type) {
                case Event::Closed:
                    _window.close();
                    break;
            }
        }
        _window.clear();
        _window.draw(game_result);
        _window.display();
    }
}

class WALL {

public : 
    float y;
    RectangleShape rect;

    WALL();
    void draw(RenderWindow& _window);
};
WALL::WALL() {
    y= 10;
    rect = RectangleShape(Vector2f(WINDOW_X_LEN, y));
    rect.setFillColor(Color::Cyan);
    rect.setPosition(0, 200);
}
void WALL::draw(RenderWindow& _window) {
    _window.draw(rect);
}

class USER{

public :
    RectangleShape rect;
    float x,y;

    USER();
    void update(RenderWindow& _window);
    void draw(RenderWindow& _window);
};
USER::USER() {
    x = 10;
    y = 150;
    rect = RectangleShape(Vector2f(x, y));
    rect.setFillColor(Color::Blue);
    rect.setPosition(10, WINDOW_Y_LEN/2);
}
void USER::update(RenderWindow& _window) {
    
    // wall에 부딪힐 때 -> GAME의 update에서 구현
    Vector2i mouse_pos = Mouse::getPosition(_window);

    rect.setPosition(this->x, mouse_pos.y - (this->y)/2);
    // 화면 아래 침범
    if(mouse_pos.y + (this->y)/2 >= WINDOW_Y_LEN) {
        rect.setPosition(this->x , WINDOW_Y_LEN - this->y);
    }

}
void USER::draw(RenderWindow& _window) {
    _window.draw(rect);
}

class ENEMY {

public :
    RectangleShape rect;
    float x,y;
    float offsetY;

    ENEMY();
    void update(RenderWindow& _window);
    void draw(RenderWindow& _window);
};
ENEMY::ENEMY() {
    offsetY = 10.0f;
    x = 10;
    y = 250;
    rect = RectangleShape(Vector2f(x, y));
    rect.setFillColor(Color::Red);
    rect.setPosition(WINDOW_X_LEN - 10, WINDOW_Y_LEN/2);
}
void ENEMY::update(RenderWindow& _window) {
    
    // wall에 부딪힐 때 -> GAME의 update에서 구현

    Vector2f rect_pos = rect.getPosition();
    // 화면 아래 침범
    if(rect_pos.y + this->y >= WINDOW_Y_LEN) {
        if(offsetY > 0) {
            offsetY *= -1;
        }
    }
    rect.move(0,this->offsetY);

}
void ENEMY::draw(RenderWindow& _window) {
    _window.draw(rect);
}

class BALL{

public : 

    CircleShape circle;
    float radius, offsetX, offsetY;

    BALL();
    void update(RenderWindow& _window);
    void draw(RenderWindow& _window);
};
BALL::BALL() {
    radius = 15.0f;
    offsetX = -8.0f;
    offsetY = 8.0f;

    circle = CircleShape(radius);
    circle.setFillColor(Color::Green);
    circle.setPosition(WINDOW_X_LEN/2, WINDOW_Y_LEN/2);
}
void BALL::update(RenderWindow& _window) {
    
    // 아래 5가지 GAME에서 구형
    // 1 USER에 맞을 때 OR 2 왼쪽 벽을 침범
    // 3 ENEMY에 맞을 때 OR 4 오른쪽 벽을 침범
    // 5 WALL에 맞을 때

    this->circle.move(this->offsetX, this->offsetY);

    // 화면 아래 침범
    Vector2f circle_pos = circle.getPosition();
    if(circle_pos.y + this->radius >= WINDOW_Y_LEN) {
        if(this->offsetY > 0) {
            this->offsetY *= -1;
        }
    }

}
void BALL::draw(RenderWindow& _window) {
    _window.draw(circle);
}

class START {

public :
    RectangleShape startBtn;

    START();
    void draw(RenderWindow& _window);

};
START::START() {
    // startBtn setting
    startBtn = RectangleShape(Vector2f(300, 150));
    startBtn.setPosition(WINDOW_X_LEN/2 - 150, 20);
}
void START::draw(RenderWindow& _window) {
    Texture startBtnTexture;
    startBtnTexture.loadFromFile("images/START.png");
    startBtn.setTexture(&startBtnTexture);
    _window.draw(startBtn);
}

class GAME{

public : 

    WALL wall;
    USER user;
    ENEMY enemy;
    BALL ball;
    START start;

    Text user_score_text;
    Text enemy_score_text;

    Font font;

    GAME();
    void update(RenderWindow& _window);
    void draw(RenderWindow& _window);
};
GAME::GAME() {

    font.loadFromFile("images/OpenSans-Bold.ttf");

    user_score_text.setFont(font);
    user_score_text.setCharacterSize(100);
    user_score_text.setFillColor(Color::Blue);
    user_score_text.setStyle(Text::Bold);
    user_score_text.setPosition(50,30);

    enemy_score_text.setFont(font);
    enemy_score_text.setCharacterSize(100);
    enemy_score_text.setFillColor(Color::Red);
    enemy_score_text.setStyle(Text::Bold);
    enemy_score_text.setPosition(WINDOW_X_LEN - 150, 30);

}
void GAME::update(RenderWindow& _window) {

    user.update(_window);
    enemy.update(_window);
    ball.update(_window);

    Vector2f wall_pos = wall.rect.getPosition();
    Vector2f ball_pos = ball.circle.getPosition();

    FloatRect ball_area = ball.circle.getGlobalBounds();
    FloatRect user_area = user.rect.getGlobalBounds();
    FloatRect enemy_area = enemy.rect.getGlobalBounds();
    FloatRect wall_area = wall.rect.getGlobalBounds();

    // user가 wall을 침범할 때
    Vector2f user_pos = user.rect.getPosition();
    if(user_pos.y <= wall_pos.y + wall.y) {
        user.rect.setPosition(user.x, wall_pos.y + wall.y);
    }

    // enemy가 wall을 침범할 때
    Vector2f enemy_pos = enemy.rect.getPosition();
    if(enemy_pos.y <= wall_pos.y + wall.y) {
        if(enemy.offsetY < 0) {
            enemy.offsetY *= -1;
        }
    }

    // 1 ball이 USER에 맞을 때 
    if(ball_area.intersects(user_area) == true) {
        if(ball.offsetX < 0) {
            ball.offsetX *= -1;
        }
    }
    // 2 ball이 왼쪽 벽을 침범
    if(ball_pos.x <= 0) {
        if(ball.offsetX < 0) {
            ball.circle.setPosition(WINDOW_X_LEN/2, WINDOW_Y_LEN/2);
            user.rect.setPosition(10, WINDOW_Y_LEN/2);
            enemy_score++;
            lets_play = 0;
        }
    }
    // 3 ball이 ENEMY에 맞을 때
    if(ball_area.intersects(enemy_area) == true) {
        if(ball.offsetX > 0) {
            ball.offsetX *= -1;
        }
    }
    // 4 ball이 오른쪽 벽을 침범
    if(ball_pos.x + 2*ball.circle.getRadius() >= WINDOW_X_LEN) {
        if(ball.offsetX > 0) {
            ball.circle.setPosition(WINDOW_X_LEN/2, WINDOW_Y_LEN/2);
            enemy.rect.setPosition(WINDOW_X_LEN - 10, WINDOW_Y_LEN/2);
            user_score++;
            lets_play = 0;
        }
    }
    // 5 ball이 WALL에 맞을 때
    if(ball_area.intersects(wall_area) == true) {
        if(ball.offsetY < 0) {
            ball.offsetY *= -1;
        }
    }

    // GOAL_SCORE에 도달하면
    if(user_score == GOAL_SCORE) {
        show_result(_window, "user");
    }
    if(enemy_score == GOAL_SCORE) {
        show_result(_window, "enemy");
    }

}
void GAME::draw(RenderWindow& _window) {

    // 점수 update , draw
    user_score_text.setString(to_string(user_score));
    enemy_score_text.setString(to_string(enemy_score));
    _window.draw(user_score_text);
    _window.draw(enemy_score_text);

    
    start.draw(_window);
    wall.draw(_window);
    
    user.draw(_window);
    enemy.draw(_window);
    ball.draw(_window);
}

int main(void){

    RenderWindow window(VideoMode(WINDOW_X_LEN, WINDOW_Y_LEN), "shooting game, practice.");
    window.setFramerateLimit(120);

    GAME game;

    while(window.isOpen()) {
        Event event;

        while(window.pollEvent(event)) {
            switch (event.type) {
                case Event::Closed :
                    cout << "Closed event occured.\n";
                    window.close();
                    break;

                case Event::MouseButtonPressed :
                    if(Mouse::isButtonPressed(Mouse::Left) == true) {
                        Vector2i mouse_pos = Mouse::getPosition(window);
                        if(mouse_pos.x >= WINDOW_X_LEN/2 - 150 && mouse_pos.x <= WINDOW_X_LEN/2 + 150){
                            if(mouse_pos.y >=20 && mouse_pos.y <= 20 + 150) {
                                cout << "startBtn clicked.\n";
                                lets_play = 1;
                            }
                        }
                    }
                    break;
            }
        }

        /* update */
        if(lets_play == 1) {
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
