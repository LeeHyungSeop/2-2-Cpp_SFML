// 22 18
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

#define windowX 1000
#define windowY 600
#define bulletALL 5

using namespace std;
using namespace sf;

int bulletNum = 0;

class BULLET {

public :
    bool bulletAlive;
    float bullet_radius = 15;
    CircleShape bullet;
    BULLET();

    void draw(RenderWindow& _window);
    void fire(RenderWindow& _window);
};
BULLET::BULLET(){
    this->bulletAlive = false;
    this->bullet = CircleShape(bullet_radius, 3);
    this->bullet.setFillColor(Color::Red);
}
void BULLET::draw(RenderWindow& _window){
    if(!this->bulletAlive){
        return;
    }
    _window.draw(this->bullet);
}
void BULLET::fire(RenderWindow& _window){
    this->bulletAlive = true;
}

class ENEMY{

public :
    RectangleShape rect;
    float offsetX = 6;
    bool enemyAlive;

    ENEMY();
    void draw(RenderWindow& _window);
    void update();
};
ENEMY::ENEMY() {
    enemyAlive = true;
    rect = RectangleShape(Vector2f(100, 20));
    rect.setPosition(20,20);
    rect.setFillColor(Color::Yellow);

}
void ENEMY::draw(RenderWindow& _window) {
    if(!enemyAlive){
        return;
    }
    _window.draw(rect);
}
void ENEMY::update() {
    if(!enemyAlive){
        return;
    }
    
    Vector2f rect_pos = rect.getPosition();

    if(rect_pos.x <= 0){ // 화면 왼쪽 가장자리 넘어가면
        offsetX *= -1;
    }
    if(rect_pos.x + rect.getSize().x >= windowX){ // 화면 왼쪽 가장자리 넘어가면
        offsetX *= -1;
    }
    rect.move(offsetX, 0);
}

class GUN {

public :
    
    CircleShape gun;
    BULLET* bullet = new BULLET[bulletALL];
    float gun_radius = 50;
    float bulletOffsetY = -5;

    GUN();
    void update(RenderWindow& _window);
    void draw(RenderWindow& _window);
    void fire(RenderWindow& _window);
};
GUN::GUN() {
    gun = CircleShape(gun_radius, 5);
    gun.setFillColor(Color::Green);
    gun.setPosition(windowX/2 - gun_radius, windowY - gun_radius);
}
void GUN::update(RenderWindow& _window){
    Vector2i mouse_pos = Mouse::getPosition(_window);

    gun.setPosition(mouse_pos.x - gun_radius, mouse_pos.y - gun_radius);
    if(mouse_pos.x - gun_radius <= 0){
        gun.setPosition(0, mouse_pos.y - gun_radius);
    }
    if(mouse_pos.x + gun_radius >= windowX){
        gun.setPosition(windowX - 2*gun_radius, mouse_pos.y - gun_radius);
    }
    if(mouse_pos.y + gun_radius >= windowY){
        gun.setPosition(mouse_pos.x - gun_radius, windowY - 2*gun_radius);
    }
    if(mouse_pos.y - gun_radius <= 0){
        gun.setPosition(mouse_pos.x - gun_radius, 0);
    }

    for (int i = 0; i < bulletALL; i++)
    {
        if(bullet[i].bulletAlive){
            bullet[i].bullet.move(0,bulletOffsetY);
            if(bullet[i].bullet.getPosition().y <= 0){
                bullet[i].bulletAlive = false;
                cout << "bullet " << i << " was died!!\n";
            }
        }
    }
    
}
void GUN::draw(RenderWindow& _window){
    _window.draw(gun);
    for (int i = 0; i < bulletALL; i++)
    {
        if(bullet[i].bulletAlive){
            bullet[i].draw(_window);
        }
    }
}
void GUN::fire(RenderWindow& _window){
    for (int i = 0; i < bulletALL; i++)
    {
        if(!bullet[i].bulletAlive){
            bullet[i].bullet.setPosition(gun.getPosition().x + gun_radius - bullet[i].bullet_radius, gun.getPosition().y);
            bullet[i].fire(_window);
            cout << "bullet " << i << " was clicked!\n";
            return;
        }
    }
    
}

class GAME{

public : 
    ENEMY enemy;
    GUN gun;

    void draw(RenderWindow& _window);
    void update(RenderWindow& _window);
    void fire(RenderWindow& _window);
};
void GAME:: draw(RenderWindow& _window){
    enemy.draw(_window);
    gun.draw(_window);
}
void GAME:: update(RenderWindow& _window){
    enemy.update();
    gun.update(_window);

    for (int i = 0; i < bulletALL; i++)
    {
        FloatRect enemy_area = enemy.rect.getGlobalBounds();
        FloatRect bullet_area = gun.bullet[i].bullet.getGlobalBounds();
        if(enemy_area.intersects(bullet_area) == true && enemy.enemyAlive == true && gun.bullet[i].bulletAlive == true) {
                // 겹쳤는지 반환해주는 method 
            gun.bullet[i].bulletAlive= false;
            enemy.enemyAlive = false;
            cout << "** enemy CRACKED bullet " << i <<" **\n";
        }
    }
    
}
void GAME:: fire(RenderWindow& _window){
    gun.fire(_window);
}

int main(void){

    RenderWindow window(VideoMode(1000, 600) , "Shooting Game");
    window.setFramerateLimit(120);

    GAME game;
    BULLET bullet;


    while(window.isOpen()){
        Event event;
        while(window.pollEvent(event)){
            switch(event.type){
                case Event::Closed:
                    window.close();
                    cout << "Closed Button Clicked, return 0\n";
                    break;
                case Event::KeyPressed:
                    if(event.key.code == Keyboard::Escape){
                        window.close();
                        cout << "Esc Button Clicked, return 0\n";
                    }
                    if(event.key.code == Keyboard::Space){
                        game.fire(window);
                    }
                    break;
            }
        }

        /* clear */
        window.clear();

        /* update */
        game.update(window);

        /* draw */
        game.draw(window);

        /* display */
        window.display();

    }

    return 0;
}