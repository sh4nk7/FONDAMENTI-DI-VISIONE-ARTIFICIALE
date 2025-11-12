//OpneCV
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

//std:
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <unistd.h>
#include <stdint.h>

struct ArgumentList {
  std::string      image;
  std::string      output;
  int              numspicchi;
  std::vector<int> edges_num;
};


bool ParseInputs(ArgumentList& args, int argc, char **argv) {
  int c;
  args.edges_num.clear();
  //numero di spicchi presente nell'immagine
  //
  //il default e' 9, ma se pensate di utilizzare altre immagini potete cambiarlo
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


/*
 * La funzione riceve in ingresso l'immagine di spicchi e un vettore di indici di edge
 *
 * Come posso selezionare gli edge richiesti in modo veloce? Cosa distingue principalmente un edge dall'altro, in questa particolare immagine?
 */
void drawEdge(const cv::Mat & image, const std::vector<int> & edges_num, cv::Mat & edges, int numspicchi)
{
  edges=cv::Mat::zeros(image.rows, image.cols, CV_8UC1);

  std::cout<<"Edge richiesti: ";
  for(auto i : edges_num)
    std::cout<<i<<" ";
  std::cout<<std::endl;

  /**
   * YOUR CODE HERE
   */
}


int main(int argc, char **argv)
{

  //////////////////////////////////////////////////////////////////
  // Parse argument list:
  //
  // DO NOT TOUCH

  ArgumentList args;
  if(!ParseInputs(args, argc, argv)) {
    return 1;
  }

  if(args.image.empty())
  {
    std::cerr << "Missing image filename,  please use -i option" << std::endl;
    return 1;
  }

  //opening file
  std::cout<<"Opening image" << args.image <<std::endl;
  cv::Mat image = cv::imread(args.image.c_str(), CV_8UC1);
  if(image.empty())
  {
    std::cerr << "Unable to open " << args.image << std::endl;
    return 1;
  }
  std::cout << "File " << args.image << " " << image.rows << "x" << image.cols << " succesfully loaded " << std::endl;


  /////////////////////////////////////////////////////////////////////
  //	EX1
  //
  // In input avete un'immagine divisa in "spicchi" di toni di grigio, in particolare 9
  //
  // Scrivere una funzione che, dato un vettore di numeri compresi tra 1 e 8 disegni una nuova immagine con una
  // riga bianca in corrispondenza degli edge selezionati (si veda il testo)
  //
  // L'immagine di uscita sara' quindi binaria, con soli due livelli: 255 sugli edgi richiesti, 0 dalle altre parti
  //


  //immagine di output
  cv::Mat edges;

  //indici degli edge che voglio visualizzare nel range [1,(numspicchi-1)]

  drawEdge(image, args.edges_num, edges, args.numspicchi);
  /////////////////////////////////////////////////////////////////////


  ///////////////////////////////////////////////////////////////////////////////////////////////////////
  //display images
  //
  // DO NOT TOUCH
  cv::namedWindow("image", cv::WINDOW_NORMAL);
  cv::imshow("image", image);

  cv::namedWindow("edges", cv::WINDOW_NORMAL);
  cv::imshow("edges", edges);

  //wait for key
  cv::waitKey();
  ///////////////////////////////////////////////////////////////////////////////////////////////////////

  return 0;
}
