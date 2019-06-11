#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctype.h>
#include <vector>
#include <random>
#include <map>

using namespace std;

// Takes a file input and reads each line sequentially
string read_lyric_file(ifstream& file_load) {
  string text = "";

  while (file_load.peek() != EOF) {
    string new_line;
    file_load >> new_line;
    text += new_line + " ";
  }

  return text;
}

// Converts each char to it's lowercase counterpart
string to_lowercase_string(string s) {
  int i = 0;
  char str[s.length() + 1];
  strcpy(str, s.c_str());
  string lowercase = "";
  while (str[i]) {
    if(s[i] >= 'A' && s[i] <= 'Z') {
      lowercase += str[i] + 32;
    }
    else {
      lowercase += str[i];
    }
    i++;
  }
  return lowercase;
}

// Removes all non-alphanumeric, apostrophe, space or newline characters
string remove_special_characters(string s) {
    for (int i = 0; i < s.size(); i++) {
        if ((s[i] < 'A' || s[i] > 'Z') &&
        (s[i] < 'a' || s[i] > 'z') &&
        s[i] != 39 && s[i] != 32)
        {
          s.erase(i, 1);
          i--;
        }
    }
    return s;
}

int main() {
  // Prompt user
  cout << "Please input the location of the lyric file: \n";

  // Load file name
  ifstream in_file;
  string text_file_source;
  cin >> text_file_source;
  in_file.open(text_file_source);

  // Check for file error
  if (in_file.fail()) {
    cerr << "Error opening file" << endl;
    exit(1);
  }

  // Notify of processing File
  cout << "\nProcessing File... (" << text_file_source << ")\n";

  // Grab lyrics from the file and remove non-alphanumeric characters
  // Then make the entire string lowercase
  string lyrics = read_lyric_file(in_file);
  lyrics = remove_special_characters(lyrics);
  lyrics = to_lowercase_string(lyrics);

  // Now we tokenize the entire string in order to cut each word out
  stringstream ssin(lyrics);
  vector<string> all_lyrics;
  string temp;
  while(ssin.good())
  {
    ssin >> temp;
    all_lyrics.push_back(temp);
  }
  all_lyrics.pop_back();

  // Now all words are in this one vector, correctly formatted
  // We make a map of colors (randomized)
  int i = 0;
  srand(time(0));
  map<string,vector<int> > lyric_colors;
  for(i = 0; i < all_lyrics.size(); i++)
  {
    lyric_colors[all_lyrics.at(i)].push_back((rand() % 255));
    lyric_colors[all_lyrics.at(i)].push_back((rand() % 255));
    lyric_colors[all_lyrics.at(i)].push_back((rand() % 255));
  }

  // The size of our square (side^2 pixels)
  const int side = all_lyrics.size() - 1;

  // Set the beginning information for the .ppm file (Magic number, size, color limit)
  ofstream img ("output_pic.ppm");
  img << "P3" << endl;
  img << side << " " << side << endl;
  img << "255" << endl;

  // Set each pixel by adding to the .ppm file
  for(i = 0; i < side; i++)
  {
    for(int j = 0; j < side; j++)
    {
        if(all_lyrics.at(i) == all_lyrics.at(j) || j == i)
        {
          vector<int> img_num = lyric_colors[all_lyrics.at(i)];
          img << img_num.at(0) << " " << img_num.at(1) << " " << img_num.at(2) << " ";
        }
        else
        {
          img << 0 << " " << 0 << " " << 0 << " ";
        }
    }
    img << endl;

  }

  // Open and view the image
  cout << "\nProcessing Complete!\n\nOpening File...\n";
  system("open output_pic.ppm");
}
