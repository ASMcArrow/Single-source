#include "DebugUISession.hh"

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

using namespace G4Threading;

G4int DebugUISession::ReceiveG4cout(const G4String& coutString)
{
    std::stringstream stream;
    stream << G4GetThreadId();

    std::ofstream file;
    std::string string = "/media/large2/SingleSource/"+stream.str();
    char* name = (char*)(string.c_str());
    file.open(name, std::ios_base::app | std::ios_base::out);
    file << coutString << "\n";
    file.close();
    return 0;
}
