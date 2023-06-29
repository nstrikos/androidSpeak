rm -rf ~/projects/$1
cd ~/projects
git clone https://github.com/nstrikos/$1.git
rm -rf ~/builds/$1
mkdir ~/builds/$1
cd ~/builds/$1
~/Qt/5.15.2/gcc_64/bin/qmake ~/projects/$1/desktop/src/$1.pro
make
cp $1 ~/packages/$1_$2_amd64/usr/share/$1/$1
cd ~/packages
dpkg-deb --build --root-owner-group $1_$2_amd64
