#include <cmath>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <netinet/in.h>

/// This should process the binary SPIR-V file and put it into a std::array in a .cpp or .hpp file that can be
/// included in the main program.
void processFile(const std::string& inputFilename, const std::string& sourceFilename, const std::string& headerFilename,
                 const std::string& variableName)
{
  std::ifstream input(inputFilename, std::ios::ate | std::ios::binary);

  if (!input.is_open())
  {
    throw std::runtime_error("Failed to open file");
  }

  const auto fileSize = input.tellg();
  std::vector<uint32_t> buffer(static_cast<uint32_t>(std::ceil(static_cast<float>(fileSize)/4.0f)));
  input.seekg(0);
  input.read(reinterpret_cast<char*>(buffer.data()), fileSize);
  input.close();

  std::ofstream output(sourceFilename);
  std::ofstream header(headerFilename);

  if (!output.is_open() || !header.is_open())
  {
    throw std::runtime_error("Failed to open file");
  }

  const std::string arrayType = [&buffer]() {
    std::stringstream ss;
    ss << "std::array<uint32_t, " << buffer.size() << "> ";
    return ss.str();
  }();

  header << "#include <array>" << std::endl
         << arrayType  << variableName << "();" << std::endl;
  header.close();

  output << "#include <array>" << std::endl
         << arrayType << variableName << "() { " << std::endl
         << arrayType << "shader = {" << std::endl;

  int n = 0;
  for (const auto& value : buffer)
  {
    output << "0x" << std::hex << std::setfill('0') << std::setw(8) << static_cast<uint32_t>(value) << ',';
    if ((++n % 8) == 0)
    {
      output << std::endl;
    }
    else
    {
      output << ' ';
    }
  }
  output << "};" << std::endl
         << "return shader;" << std::endl << "}";
  output.close();
}

int main(int argc, char** argv)
{
  if (argc != 9)
  {
    return 1;
  }

  std::string inputFilename;
  std::string sourceFilename;
  std::string variableName;
  std::string headerFilename;

  for (int idx = 1; idx < argc -1; idx += 2)
  {
    const auto arg = std::string{argv[idx]};
    if (arg == "-i")
    {
      inputFilename = argv[idx+1];
    }
    else if (arg == "-o")
    {
      sourceFilename = argv[idx+1];
    }
    else if (arg == "-n")
    {
      variableName = argv[idx+1];
    }
    else if (arg == "-h")
    {
      headerFilename = argv[idx+1];
    }
    else
    {
      throw std::runtime_error("Invalid argument found");
    }
  }

  processFile(inputFilename, sourceFilename, headerFilename, variableName);

  return 0;
}
