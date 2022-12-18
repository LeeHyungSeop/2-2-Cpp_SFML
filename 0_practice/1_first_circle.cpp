#include <SFML/Graphics.hpp>
#include <iostream>


using namespace sf;
using namespace std;

int main(void){

    // Setup ==========================================================================
    RenderWindow window(VideoMode(600,600), "First Example");
        // 200 * 200 게임 창의 크기, 창 이름 : First Example
        // RenderWindow class의 window라는 contructor 호출

    CircleShape shape(200.0f);
        // CircleShape class에 원 반지름 100
    shape.setFillColor(Color::Green);
        // green색이 꽉 찬 원을 그려라
    
    // Infinity Loop ==================================================================
    while (window.isOpen()) {

        Event e; // 사용자가 보내는 event에 대해서 handle (닫기, 클릭 등)

        while(window.pollEvent(e)) { // 사용자가 어떤 event를 줬는지? 

            if(e.type == Event::Closed){ // 우상단 창닫기 버튼을 눌렀으면, 
                window.close();          // 창을 닫는다.
                cout << "closed" << endl;
            }

        }

        window.clear();     // 화면을 지운다
        window.draw(shape); // green색 꽉 찬 원을 그래픽 메모리에 로드
        window.display();   // 그래픽 메모리에 그려진 원을 창에 그려라
        
    }

    return 0;
}