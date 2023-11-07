# moonlib
The moonlib is an ECS game engine library that uses the raylib for rendering, user input and audio.

## Installation
You'll need cmake for the whole intallation process
You can download cmake [here](https://cmake.org/download/)

### Windows
First open a terminal **as administrator** and install/build the raylib for windows with [vcpkg](https://github.com/microsoft/vcpkg)

```bash
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
bootstrap-vcpkg.bat
vcpkg integrate install
vcpkg install raylib
```

Then clone the moonlib and build it with `cmake`

```bash
git clone git@github.com:hippolyte-aubert/moonlib.git
cd moonlib
mkdir build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake ..
cmake --build . --config Release
cmake --install .
```

### Linux

First install the raylib with `cmake`

```bash
git clone https://github.com/raysan5/raylib.git raylib
cd raylib
mkdir build && cd build
cmake -DBUILD_SHARED_LIBS=ON ..
make
sudo make install
```

Then clone the moonlib and build it with `cmake`

```bash
git clone git@github.com:hippolyte-aubert/moonlib.git
cd moonlib
mkdir build
cd build
cmake ..
make
sudo make install
```

## Example

```cpp
#include <moonlib.h>

class GameScene : public MLIB::DefaultScene {
    public:
        Game(MLIB::Engine &engine) : _engine(engine) {};
        ~Game() {};

        MLIB::Engine &engine;
        MLIB::Entity player;
        MLIB::Entity background;

        void Start()
        {
            player = engine.CreateEntity();
            background = engine.CreateEntity();

            engine.AddComponent(
                player,
                MLIB::Transform2D{
                    .position = {200, 150},
                    .rotation = 0,
                    .scale = {1, 1}
                }
            );

            engine.AddComponent(
                player,
                MLIB::Renderer2D{
                    .textureName = "player.png",
                    .tint = WHITE,
                    .zIndex = 0
                }
            );

            engine.AddComponent(
                background,
                MLIB::Transform2D{
                    .position = {0, 0},
                    .rotation = 0,
                    .scale = {1, 1}
                }
            );

            engine.AddComponent(
                background,
                MLIB::Renderer2D{
                    .textureName = "background.png",
                    .tint = WHITE,
                    .zIndex = 1
                }
            );
        }

        void Update()
        {
            MovePlayer();
        }

        void MovePlayer()
        {
            auto &transform = engine.GetComponent<MLIB::Transform2D>(player);

            transform.position.x += MLIB::InputSystem::GetAxis("Horizontal") * 400 * GetFrameTime();
            transform.position.y += MLIB::InputSystem::GetAxis("Vertical") * 400 * GetFrameTime();
        }
};

int main(void)
{
    MLIB::Engine engine("My Game", 800, 450, MLIB::Mode2D, "assets/");

    engine.Init();

    std::shared_ptr<MLIB::Scene> gameScene = std::make_shared<MLIB::Scene>(engine);

    unsigned int gameSceneId = engine.AddScene(gameScene);

    engine.SwitchScene(gameSceneId);

    engine.Run();
}
