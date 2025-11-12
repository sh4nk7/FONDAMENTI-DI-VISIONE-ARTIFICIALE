//OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

//std:
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <stdint.h>

struct ArgumentList {
  std::string      image;
  std::string      output;
  int              numspicchi;
  std::vector<int> edges_num;
};

// ==================================================================
// PARSE INPUTS 
// ==================================================================
bool ParseInputs(ArgumentList& args, int argc, char **argv) {
  int c;
  args.edges_num.clear();
  //numero di spicchi presente nell'immagine
  args.numspicchi=9;

  while ((c = getopt (argc, argv, "hi:s:o:n:")) != -1)

    switch (c)
    {
      case 'i':
        std::cout << "Immagine di input da opzioni " << optarg << std::endl;
        args.image = optarg;
        break;
      case 'o':
        std::cout << "Immagine di output da opzioni " << optarg << std::endl;
        args.output = optarg;
        break;
      case 'n':
        std::cout << "Numero spicchi immagine di input " << optarg << std::endl;
        args.numspicchi=atoi(optarg);
        break;
      case 's':
        std::cout << "Aggiungo agli edge da cercare il numero " << optarg << std::endl;
        args.edges_num.push_back(atoi(optarg));
        break;
      case 'h':
      default:
        std::cout<<"usage: " << argv[0] << "<options>"<<std::endl;
        std::cout<<"exit:  type q"<<std::endl<<std::endl;
        std::cout<<"Allowed options:"<<std::endl<<
          "   -h                       produce help message"<<std::endl<<
          "   -i filename              input image filename"<<std::endl<<
          "   -o filename              output image filename"<<std::endl<<
          "   -n number                number of edges in input image"<<std::endl<<
          "   -s edge                  edge index (can be used multiple times)"<<std::endl<<std::endl<<std::endl;
        return false;
    }
  sort(args.edges_num.begin(), args.edges_num.end());
  return true;
}

// ==================================================================
// FUNZIONE 
// ==================================================================
void drawEdge(const cv::Mat & image, const std::vector<int> & edges_num, cv::Mat & edges, int numspicchi)
{
  edges = cv::Mat::zeros(image.rows, image.cols, CV_8UC1);

  std::cout << "Edge richiesti: ";
  for (auto i : edges_num)
    std::cout << i << " ";
  std::cout << std::endl;

  // Calcolo centro dell'immagine
  cv::Point center(image.cols / 2, image.rows / 2);

  // Angolo tra gli spicchi
  double angleStep = 360.0 / numspicchi;

  // Disegna solo gli edge richiesti
  for (int idx : edges_num)
  {
    // Indici validi da 1 a numspicchi-1
    if (idx < 1 || idx > numspicchi - 1)
      continue;

    // Calcolo angolo
    double angle = idx * angleStep;
    double rad = angle * CV_PI / 180.0;

    // Punto finale del raggio
    cv::Point p(center.x + std::cos(rad) * image.cols / 2,
                center.y + std::sin(rad) * image.rows / 2);

    // Disegno linea bianca (spessore 2)
    cv::line(edges, center, p, cv::Scalar(255), 2);
  }
}

// ==================================================================
// MAIN 
// ==================================================================
int main(int argc, char **argv)
{
  ArgumentList args;
  if(!ParseInputs(args, argc, argv)) {
    return 1;
  }

  if(args.image.empty())
  {
    std::cerr << "Missing image filename,  please use -i option" << std::endl;
    return 1;
  }

  std::cout<<"Opening image " << args.image <<std::endl;
  cv::Mat image = cv::imread(args.image.c_str(), cv::IMREAD_GRAYSCALE);
  if(image.empty())
  {
    std::cerr << "Unable to open " << args.image << std::endl;
    return 1;
  }
  std::cout << "File " << args.image << " " << image.rows << "x" << image.cols << " succesfully loaded " << std::endl;

  // Output image
  cv::Mat edges;
  drawEdge(image, args.edges_num, edges, args.numspicchi);

  // Mostra immagini
  cv::namedWindow("image", cv::WINDOW_NORMAL);
  cv::imshow("image", image);

  cv::namedWindow("edges", cv::WINDOW_NORMAL);
  cv::imshow("edges", edges);

  // Attesa tasto
  cv::waitKey();

  // Salvataggio 
  if (!args.output.empty()) {
    cv::imwrite(args.output, edges);
    std::cout << "Immagine salvata come " << args.output << std::endl;
  }

  return 0;
}
