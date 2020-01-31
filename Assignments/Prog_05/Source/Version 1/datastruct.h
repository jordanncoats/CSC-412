#pragma once
 /** This is the data type to store all relevant information read from each file
  */
using dataItem = struct
{
    /** The index of the process that will work with this file.
      */
    int process_index;
    /** The text's line position.
      */
    int line_number;
    /** The content text of each file.
      */
    std::string text;
    /** The original path and filename.
      */
    std::string file_path;
};