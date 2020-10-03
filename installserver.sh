# open ports 1234
sudo yum groupinstall "Development Tools"

wget https://www.libsdl.org/release/SDL2-2.0.12.zip
unzip SDL2-2.0.12.zip 
cd SDL2-2.0.12/
./configure
make all
sudo make install

wget https://www.libsdl.org/projects/SDL_net/release/SDL2_net-2.0.1.zip
unzip SDL2_net-2.0.1.zip 
cd SDL2_net-2.0.1/
./configure
make all
sudo make install

sudo yum install sqlite-devel.x86_64

LD_LIBRARY_PATH=/usr/local/lib
export LD_LIBRARY_PATH

cd wizard-game/
make server
./output/server &
