#include <iostream>
#include <string>
#include <vector>
int main(int argc, char *argv[])
{
  std::vector<std::string> rotate;
  std::vector<std::string> crop;
  std::vector<std::string> split;

  int child_counter;

  std::vector<std::string> args(argv, argv + argc);
  for (int i = 3; i < args.size(); ++i)
  {
    std::string str;
    if (args[i] == "rotate")
    {
      for (int r = 0; r < 2; r++)
      {
        str.append(args[i]);
        str.append(" ");
        i++;
        child_counter++;
      }
      rotate.push_back(str);
      str.clear();
    }
    if (args[i] == "crop")
    {
      for (int c = 0; c < 5; c++)
      {
        str.append(args[i]);
        str.append(" ");
        i++;
        child_counter++;
      }
      crop.push_back(str);
      str.clear();
      child_counter++;
    }
    if (args[i] == "split")
    {

      for (int s = 0; s < 1; s++)
      {
        str.append(args[i]);
        str.append(" ");
        i++;
        child_counter++;
      }
      split.push_back(str);
      str.clear();
      child_counter++;
    }
  }

  return 0;
}