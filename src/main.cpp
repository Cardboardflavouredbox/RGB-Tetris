#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <random>
#include <algorithm>

bool screenfocused=true;
unsigned char array[10][20]={},currentblock=0,blocklist[7]={0,1,2,3,4,5,6},currentblocknum=0,
    blocks[7][4][4][4]={
        {
        {
        {0,0,0,0},
        {11,11,11,11},
        {0,0,0,0},
        {0,0,0,0},
        },
        {
        {0,0,11,0},
        {0,0,11,0},
        {0,0,11,0},
        {0,0,11,0},
        },
        {
        {0,0,0,0},
        {0,0,0,0},
        {11,11,11,11},
        {0,0,0,0},
        },
        {
        {0,11,0,0},
        {0,11,0,0},
        {0,11,0,0},
        {0,11,0,0},
        }
        },
        {
        {
        {1,0,0},
        {1,1,1},
        {0,0,0},},
        {
        {0,1,1},
        {0,1,0},
        {0,1,0},},
        {
        {0,0,0},
        {1,1,1},
        {0,0,1},},
        {
        {0,1,0},
        {0,1,0},
        {1,1,0},}
        },
        {
        {
        {0,0,6},
        {6,6,6},
        {0,0,0},},
        {
        {0,6,0},
        {0,6,0},
        {0,6,6},},
        {
        {0,0,0},
        {6,6,6},
        {6,0,0},},
        {
        {6,6,0},
        {0,6,0},
        {0,6,0},}
        },
        {{
        {14,14},
        {14,14}},
        {
        {14,14},
        {14,14}},
        {
        {14,14},
        {14,14}},
        {
        {14,14},
        {14,14}},
        },
        {
        {{0,10,10},
        {10,10,0},
        {0,0,0},},

        {{0,10,0},
        {0,10,10},
        {0,0,10},},

        {{0,0,0},
        {0,10,10},
        {10,10,0},},

        {{10,0,0},
        {10,10,0},
        {0,10,0},}
        },
        {{
        {0,5,0},
        {5,5,5},
        {0,0,0}},
        {
        {0,5,0},
        {0,5,5},
        {0,5,0}},
        {
        {0,0,0},
        {5,5,5},
        {0,5,0}},
        {
        {0,5,0},
        {5,5,0},
        {0,5,0}},
        },
        {{
        {12,12,0},
        {0,12,12},
        {0,0,0}},
        {
        {0,0,12},
        {0,12,12},
        {0,12,0}},
        {
        {0,0,0},
        {12,12,0},
        {0,12,12}},
        {
        {0,12,0},
        {12,12,0},
        {12,0,0}},
        }
    };
short direction=0,blockx=0,blocky=0,blockfallwait=45,blockfallframes=45;

sf::Color CGAcolor(unsigned char color){
    return sf::Color(170*((color/4)%2) + 85*(color/8), 
    (1-(color==6)/3.0)*170*((color/2)%2) + 85*(color/8),
    170*(color%2) + 85*(color/8));
}

void keypresscheck(sf::Keyboard::Key keycode,char *key){
    if(screenfocused&&sf::Keyboard::isKeyPressed(keycode)){if(*key=='0')*key='2';else if(*key=='2')*key='1';}else *key='0';
}

bool checkthing(){
    for(unsigned char i=0;i<4;i++)for(unsigned char j=0;j<4;j++){
                if(blocks[currentblock][direction][j][i]!=0){
                if(j+blocky>18)return false;
                if(array[i+blockx][j+blocky+1]!=0)return false;}
            }
    return true;
}

bool overlapcheck(){
    for(unsigned char i=0;i<4;i++)for(unsigned char j=0;j<4;j++){
            if(blocks[currentblock][direction][j][i]!=0&&(array[i+blockx][j+blocky]!=0||i+blockx>9||i+blockx<0))return true;
            }
    return false;
}
bool turncheck(){
    bool left=false,right=false;
    for(unsigned char i=0;i<4;i++)for(unsigned char j=0;j<4;j++){
            if(blocks[currentblock][direction][j][i]!=0&&(array[i+blockx][j+blocky]!=0||i+blockx>9||i+blockx<0)){if(i<2)left=true;else right=true;}
            }
    while(left&&!right){
        left=false;right=false;
        blockx++;
        for(unsigned char i=0;i<4;i++)for(unsigned char j=0;j<4;j++){
            if(blocks[currentblock][direction][j][i]!=0&&(array[i+blockx][j+blocky]!=0||i+blockx>9||i+blockx<0)){if(i<2)left=true;else right=true;}
            }
    }
    while(!left&&right){
        left=false;right=false;
        blockx--;
        for(unsigned char i=0;i<4;i++)for(unsigned char j=0;j<4;j++){
            if(blocks[currentblock][direction][j][i]!=0&&(array[i+blockx][j+blocky]!=0||i+blockx>9||i+blockx<0)){if(i<2)left=true;else right=true;}
            }
    }
    return left&&right;
}

class TileMap : public sf::Drawable, public sf::Transformable
{
public:
    bool load(unsigned char array[][20])
    {
        // resize the vertex array to fit the level size
        m_vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
        m_vertices.resize(10 * 20 * 6);

        // populate the vertex array, with two triangles per tile
        for (unsigned char i = 0; i < 10; ++i)
        {
            for (unsigned char j = 0; j < 20; ++j)
            {
                // get a pointer to the triangles' vertices of the current tile
                sf::Vertex* triangles = &m_vertices[(i + j * 10) * 6];
                sf::Vector2u tileSize={54,54};

                // define the 6 corners of the two triangles
                triangles[0].position = sf::Vector2f(420+i * tileSize.x, j * tileSize.y);
                triangles[1].position = sf::Vector2f(420+(i + 1) * tileSize.x, j * tileSize.y);
                triangles[2].position = sf::Vector2f(420+i * tileSize.x, (j + 1) * tileSize.y);
                triangles[3].position = sf::Vector2f(420+i * tileSize.x, (j + 1) * tileSize.y);
                triangles[4].position = sf::Vector2f(420+(i + 1) * tileSize.x, j * tileSize.y);
                triangles[5].position = sf::Vector2f(420+(i + 1) * tileSize.x, (j + 1) * tileSize.y);

                for(unsigned char k=0;k<6;k++)triangles[k].color = CGAcolor(array[i][j]);
            }
        }

        return true;
    }

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        // apply the transform
        states.transform *= getTransform();

        // draw the vertex array
        target.draw(m_vertices, states);
    }

    sf::VertexArray m_vertices;
};

void windowset(sf::RenderWindow& window,bool *gamequit){
    while (std::optional event = window.pollEvent()){
            if (event->is<sf::Event::Closed>()){window.close();*gamequit=true;}
            if(event->is<sf::Event::FocusGained>())screenfocused=true;
            else if(event->is<sf::Event::FocusLost>())screenfocused=false;

            if (auto resized = event->getIf<sf::Event::Resized>()){
                float x,y;
                if((resized->size.x)>(resized->size.y)){
                    y=1080;
                    x=(float(resized->size.x)/float(resized->size.y)*1080.f);
                }
                else{
                    x=1920;
                    y=(float(resized->size.y)/float(resized->size.x)*1920.f);
                }
                sf::FloatRect visibleArea({(-x+1920.f)/2,(-y+1080.f)/2},{x,y});

                window.setView(sf::View(visibleArea));
            }
    }
}

int main()
{
    char upkey='0',leftkey='0',downkey='0',rightkey='0',rightturn='0',leftturn='0';
    bool gamequit=false;
    auto window = sf::RenderWindow(sf::VideoMode({1470, 956}), "Tetris wow");
    window.setFramerateLimit(60);
    TileMap tiles;

    auto resized = window.getSize();
    float tempx,tempy;
    if((resized.x)>(resized.y)){
        tempy=1080;
        tempx=(float(resized.x)/float(resized.y)*1080.f);
    }
    else{
        tempx=1920;
        tempy=(float(resized.y)/float(resized.x)*1920.f);
    }
    sf::FloatRect visibleArea({(-tempx+1920.f)/2,(-tempy+1080.f)/2},{tempx,tempy});

    window.setView(sf::View(visibleArea));

    std::shuffle(std::begin(blocklist), std::end(blocklist),std::mt19937(std::random_device()()));
    currentblock=blocklist[currentblocknum];
            

    while (window.isOpen())
    {
        windowset(window,&gamequit);


        for(unsigned char i=0;i<20;i++){
            unsigned char linecheck=0;
            for(unsigned char j=0;j<10;j++){if(array[j][i]!=0)linecheck++;}
            if(linecheck==10){
                for(unsigned char j=i;j>0;j--){
                    for(unsigned char k=0;k<10;k++)array[k][j]=array[k][j-1];
                }
            }
        }
        blockfallwait--;
        keypresscheck(sf::Keyboard::Key::Q,&leftturn);
        keypresscheck(sf::Keyboard::Key::E,&rightturn);
        keypresscheck(sf::Keyboard::Key::A,&leftkey);
        keypresscheck(sf::Keyboard::Key::D,&rightkey);
        keypresscheck(sf::Keyboard::Key::W,&upkey);
        keypresscheck(sf::Keyboard::Key::S,&downkey);

        if(leftturn=='2'){
            direction--;if(direction<0)direction=3;
            if(turncheck()){direction++;if(direction>3)direction=0;}
            }
        else if(rightturn=='2'){
            direction++;if(direction>3)direction=0;
            if(turncheck()){direction--;if(direction<0)direction=3;}
            }
        
        if(leftkey=='2'){blockx--;if(overlapcheck())blockx++;}
        if(rightkey=='2'){blockx++;if(overlapcheck())blockx--;}
        bool check=true;
        if(upkey=='2'){
            while(check){
                check=checkthing();
                if(check)blocky++;
                }
            }
        if(downkey=='1'||blockfallwait==0){check=checkthing();if(check)blocky++;if(blockfallwait==0)blockfallwait=blockfallframes;}

        for(unsigned char i=0;i<4;i++)for(unsigned char j=0;j<4;j++){
            if(blocks[currentblock][direction][j][i]!=0){
                while(i+blockx>9)blockx--;
                while(i+blockx<0)blockx++;
                while(j+blocky>19)blocky--;
                while(i+blockx>9)blockx--;
                }
            }
        for(unsigned char i=0;i<4;i++)for(unsigned char j=0;j<4;j++){
            if(blocks[currentblock][direction][j][i]!=0)array[i+blockx][j+blocky]=blocks[currentblock][direction][j][i];
            }
            

        tiles.load(array);

        sf::RectangleShape rect({1920.f, 1080.f});
        rect.setFillColor(CGAcolor(8));

        window.clear();
        window.draw(rect);
        window.draw(tiles);
        window.display();

        if(check){for(unsigned char i=0;i<4;i++)for(unsigned char j=0;j<4;j++)if(blocks[currentblock][direction][j][i]!=0)array[i+blockx][j+blocky]=0;}
        else {
            blocky=0;
            blockfallwait=blockfallframes;
            if(currentblocknum>5){currentblocknum=0;std::shuffle(std::begin(blocklist), std::end(blocklist),std::mt19937(std::random_device()()));}
            else currentblocknum++;
            currentblock=blocklist[currentblocknum];
            }
    }
}
