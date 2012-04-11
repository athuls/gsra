#include "facenet.h"

#include <QtGui>
#include <QApplication>


int main2()
{
  facenet mynet;
  mynet.train();
  return 0;
}

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  Idx<ubyte> img(1,1);
  load_matrix<ubyte>(img, "data/sample.mat");
  img = image_resize(img, 300, 300, 1);

  facenet mynet("data/weights_9283.mat");
  //Idx<int> size = mynet.calc_sizes(300, 300);
  //size.printElems();
  Idx<int> size(1);
  size.set(65, 0);

  Idx<double> res = mynet.detect(&img, 300, 300, size, 1.0, 6.8, 30);

  ebwindow mywindow(300, 300);
  mywindow.gray_draw_matrix((void*)&img, UBYTE, 0, 0, 0, 255);
  mywindow.show();

  mynet.show_net( true, false);
  return a.exec();
}

int main3(){
  extract_faces(42, 42, "/home/cyril/net/datasets/test", "coordinates.txt", 0, 
		' ', "", "data/coordonnées.txt");
  return 0;
}
