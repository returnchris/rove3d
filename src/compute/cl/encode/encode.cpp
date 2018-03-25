#include <cstdint>
#include <iostream>
#include <string>
#include <cstring>
#include "../../../core/encoder.h"
///
int32_t main()
{
  std::string prefix = "std::string program = \"";
  std::string postfix = "\";";
  // ---- DRAFT
  FILE* draft_file = fopen("obsf_draft.cl", "r");
  if (!draft_file) {
    printf("\nFailed to open obsfucated draft file.\n");
  } else {
    fseek(draft_file, 0L, SEEK_END);
    size_t draft_size = ftell(draft_file);
    rewind(draft_file);
    char* draft_data = (char*)malloc(sizeof(char) * draft_size);
    if (fread(draft_data, 1, draft_size, draft_file) == 0) {
      printf("\nFailed to read obsfucated draft file.\n");
    } else {
      std::string draft(draft_data);
      std::string encoded;
      Encoder::Encode(&draft, &encoded);
      FILE* file = fopen("../kernels/program.cl.h", "w+");
      if (file) {
        fwrite(prefix.c_str(), sizeof(char), prefix.length(), file);
        fwrite(encoded.c_str(), sizeof(char), 
               encoded.size(), file);
        fwrite(postfix.c_str(), sizeof(char), postfix.length(), file);
        fclose(file);
      } else {
        printf("\nFailed to open non-image kernel for writing.\n");
      }
    }
    fclose(draft_file);
    free(draft_data);
  }
  return 0; 
}
