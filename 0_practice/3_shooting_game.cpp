#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <algorithm> // count_if

using namespace std;
using namespace sf;

class ENEMY
{
public:
   RectangleShape rect;
   float offset_x;
   bool enemy_alive;

   ENEMY();
   void draw(RenderWindow& _window);
   void update();
};
ENEMY::ENEMY()
{
   enemy_alive = true;

   rect = RectangleShape(Vector2f(80.0f, 20.0f));
   rect.setFillColor(Color::Magenta);
   rect.setPosition(30, 30);

   offset_x = 2.0f;


}
void ENEMY::draw(RenderWindow& _window)
{
   if (enemy_alive == false) {
      return;
   }
   _window.draw(rect);
}
void ENEMY::update()
{
   if (enemy_alive == false) {
      return;
   }
   // rect 객체가 화면의 좌/우측 가장자리에 도달하면 offset_x값의
   // 부호를 반대로 해준다.
   
   // 1. rect 객체의 위치정보를 얻기.
   Vector2f rect_pos = rect.getPosition();

   // 2. 화면 가장자리에 도달했는지 판단.
   if (rect_pos.x <= 0) { // 화면의 좌측 가장자리 도달.
      offset_x = 2.0f;
   }
   else if (rect_pos.x + rect.getSize().x >= 640) // 화면의 우측 가장자리 도달.
   {
      offset_x = -2.0f;
   }

   rect.move(offset_x, 0);
}


// ENEMY 객체들을 vector화 시켜서 다수의 ENEMY 객체를 생성
class ENEMY_SET{

public :

   vector<ENEMY> vec;

   ENEMY_SET(int num = 6);

   void draw(RenderWindow& _window);
   void update();
};
ENEMY_SET::ENEMY_SET(int num){ // default parameter
   for (int i = 0; i < num; i++)
   {
      ENEMY buf;
      buf.rect.setPosition(i*30, i*30); // 각 enemy별 위치를 다르게
      vec.push_back(buf);
   }
   
}
void ENEMY_SET::draw(RenderWindow& _window) {
   for(int i = 0; i<vec.size() ; i++){
      vec[i].draw(_window);
   }
}
void ENEMY_SET::update() {
   for (int i = 0; i <vec.size(); i++)
   {
      vec[i].update();
   }  
}


class GUN
{
public:
   CircleShape body;
   CircleShape bullet;
   bool bullet_alive;

   GUN();

   void update(RenderWindow& _window);
   void draw(RenderWindow& _window);
   void fire();
};
GUN::GUN()
{
   body = CircleShape(50);
   body.setFillColor(Color::Green);
   body.setPosition(640 / 2 - body.getRadius(), 480 - body.getRadius()); // 화면의 하단부 중심.

   bullet = CircleShape(10, 3);
   bullet.setFillColor(Color::White);
   bullet.setPosition(999, 999); // 화면 밖으로 빼놓기.

   bullet_alive = false;
}
void GUN::update(RenderWindow& _window)
{
   // 원 객체가 마우스 커서를 따라오도록 구현.
   Vector2i mouse_pos = Mouse::getPosition(_window);
   body.setPosition(mouse_pos.x - body.getRadius(), body.getPosition().y);

   // 원 객체가 화면 밖을 벗어나지 않도록 구현.
   // 
   // 1. 원 객체의 위치를 얻고
   Vector2f body_pos = body.getPosition();
   // 2. 그 위치정보를 기반으로 화면밖으로 나갔는지 판단.
   if (body_pos.x <= 0) { // 화면 좌측 침범.
      body.setPosition(0, body_pos.y);
   }
   else if (body_pos.x + 2 * body.getRadius() >= 640) // 화면 우측 침범.
   {
      body.setPosition(640 - 2 * body.getRadius(), body_pos.y);
   }
   
   // 총알을 위로 움직이기
   if (bullet_alive == true) {
      bullet.move(0, -5.0f);
   }

   // 총알의 window의 가장 상단부를 찍었지를 판단.
   // 총알 객체의 위치
   Vector2f bullet_pos = bullet.getPosition();
   if (bullet_pos.y <= 0) { // 총알이 화면의 상단부를 찍었을 때.
      bullet_alive = false;
   }
}
void GUN::draw(RenderWindow& _window)
{
   _window.draw(body);
   if (bullet_alive == true) {
      _window.draw(bullet);
   }
}
void GUN::fire()
{
   if (bullet_alive == true) {
      return;
   }

   bullet_alive = true;

   // 총알을 body의 중앙 상단부로 가져오기.
   Vector2f body_pos = body.getPosition();
   bullet.setPosition(body_pos.x + body.getRadius() - bullet.getRadius(), body_pos.y);
}


class GAME
{
public:
   // 적 객체
   ENEMY_SET enemy_set;
   // 원 객체
   GUN gun;

   Text alive_enemy_num_text; // enemy가 몇 명 살아있는지 gun에 그려줄 Texture 변수
   Font font;

   GAME();

   void fire();
   void update(RenderWindow& _window);
   void draw(RenderWindow& _window);
   void init_end(RenderWindow& _window, int status = 1); // status : init? end?
};
GAME::GAME() {

   font.loadFromFile("images/OpenSans-Bold.ttf");

   alive_enemy_num_text.setFont(font); // 폰트 지정
   alive_enemy_num_text.setCharacterSize(30); // 문자 크기 지정
   alive_enemy_num_text.setFillColor(Color::Magenta); // 색 지정
   alive_enemy_num_text.setStyle(Text::Bold); // 두꺼운 글씨 지정

}
void GAME::fire()
{
   gun.fire();
}
void GAME::update(RenderWindow& _window)
{
   enemy_set.update();
   gun.update(_window);
   
   // 위 두 객체가 서로 충돌 했는지 검사.
   FloatRect bullet_area = gun.bullet.getGlobalBounds();
   for (int i = 0; i < enemy_set.vec.size(); i++)
   {
      FloatRect enemy_area = enemy_set.vec[i].rect.getGlobalBounds();
      if (enemy_area.intersects(bullet_area) == true && enemy_set.vec[i].enemy_alive == true && gun.bullet_alive == true) { // 충돌
         gun.bullet_alive = false;
         enemy_set.vec[i].enemy_alive = false;
      }
   }

   // 살아있는 ENEMY 수를 세고 그 수를 Text 변수로 써서 body 위에 display
   int alive_enemy_num = 0;
   alive_enemy_num = count_if(enemy_set.vec.begin(), enemy_set.vec.end(), 
      [] (ENEMY& e) {
         return (e.enemy_alive == true); // true라면 return 
      }
   );
   
   alive_enemy_num_text.setString(to_string(alive_enemy_num));
   Vector2f body_pos = gun.body.getPosition();
   alive_enemy_num_text.setPosition(body_pos.x + 40, body_pos.y);

   // 게임이 끝났는지 검사
   // ENEMEY가 다 죽으면 끝. (enemy_alive가 전부 false면 끝)
   for (int i = 0; i < enemy_set.vec.size(); i++)
   {
      if(enemy_set.vec[i].enemy_alive) { // enemy가 하나라도 살아있다면
         return;                         // 돌려보낸다
      }
   }

   init_end(_window, 0); // end 화면 실행

}
void GAME::draw(RenderWindow& _window)
{
   enemy_set.draw(_window);
   gun.draw(_window);

   _window.draw(alive_enemy_num_text);
}
void GAME::init_end(RenderWindow& _window, int status){
   RectangleShape startBtn(Vector2f(300.0f, 150.0f));
   startBtn.setPosition(640/2 - 150, 480/2 - 75);

   Texture startBtnTexture;
   if(status){
      startBtnTexture.loadFromFile("images/startBtn2.png");
   }
   else if(!status){
      startBtnTexture.loadFromFile("images/end.png");
   }
   startBtn.setTexture(&startBtnTexture); // 주소를 넘겨야 함

   // 시작, 끝 화면은 정적인 화면이므로 , clear, update, draw, display를 한 번만 하면 된다
   _window.clear();
   _window.draw(startBtn);
   _window.display();
   bool signal = false;

   // 화면을 display
   while (_window.isOpen())
   {
      // 마우스의 좌클릭 event를 감지.

      Event event;
      while (_window.pollEvent(event))
      {
         switch (event.type)
         {
         case Event::Closed:
            _window.close();
            break;
         case Event::MouseButtonPressed:
            // 마우스의 좌클릭으로 시작 버튼 눌렀는지 검사.
            if (Mouse::isButtonPressed(Mouse::Left) == true) {
               // 마우스 커서의 위치가 startBtn 위에 있는지 검사
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

         default:
            break; 
         }
      }

      if(signal){
         break;
      }

   }

}

int main()
{
   RenderWindow window(VideoMode(640, 480), "shoot");
   window.setFramerateLimit(120);

   GAME game;

   game.init_end(window);

   while (window.isOpen())
   {
      // 마우스의 좌클릭 event를 감지.

      Event event;
      while (window.pollEvent(event))
      {
         switch (event.type)
         {
         case Event::Closed:
            window.close();
            break;
         case Event::MouseButtonPressed:
            // 마우스의 좌클릭으로 총알 발사.
            if (Mouse::isButtonPressed(Mouse::Left) == true) {
               // 총알을 발사.
               game.fire();
            }

            break;

         default:
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