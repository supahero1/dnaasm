/**
 * \file dnaasm_main.cpp
 * \brief the C++ file with implementation of 'main' function for desktop application
 */

#include "common/Parameters.hpp"
#include "dbj/AssemblyParameters.hpp"
#include "dbj/AlignParameters.hpp"
#include "olc/OlcParameters.hpp"
#include "scfr/ScaffoldParameters.hpp"
#include "dnaasm/CommandManager.hpp"

#include <mt4cpp/OstreamCommandObserver.hpp>
#include <fstream>
#include <chrono>
#include <thread>

using namespace std;
using namespace mt4cpp;
using namespace dnaasm::common;
using namespace dnaasm::dnaasm;
using namespace dnaasm::dbj;
using namespace dnaasm::olc;
using namespace dnaasm::scfr;
using namespace dnaasm;

int main(int argc, const char* argv [])
{
    initCalcModule();

    Parameters::ParametersMap parameters = Parameters::convertArgvToMap(argc, argv);
    Parameters globalParameters(parameters);

    const char *mode = globalParameters.getMode();
    if (mode == NULL) {
        return -3;
    }

    if (globalParameters.getPrintHelp()) {
        if (string(mode) == "-assembly") {
            AssemblyParameters::printHelpForModule();
        } else if (string(mode) == "-align") {
            AlignParameters::printHelpForModule();
        } else if (string(mode) == "-scaffold") {
            ScaffoldParameters::printHelpForModule();
        } else if (string(mode) == "-olc") {
            OlcParameters::printHelpForModule();
        }
        return 0;
    }

    CommandManager &cmdmgr = CommandManager::getInstance();
    CommandID cmdId;
    int taskId = 1;

    ofstream ofile(globalParameters.getOutputFileName().c_str());

    if (string(mode) == "-assembly") {
        cmdId = cmdmgr.runAssemblyCommand(taskId, parameters);
    } else if (string(mode) == "-align") {
        cmdId = cmdmgr.runAlignCommand(taskId, parameters);
    } else if (string(mode) == "-scaffold") {
        cmdId = cmdmgr.runScaffoldCommand(taskId, parameters);
    } else if (string(mode) == "-olc") {
        cmdId = cmdmgr.runOlcCommand(taskId, parameters);
    } else {
        return -2;
    }

    cout << "PROGRESS: " << endl;

    stringstream progressStream;
    cmdmgr.setCommandObserver(cmdId, PCommandObserver(new OstreamCommandObserver(progressStream)));

    int lastPercentage = -1;
    while(true) {
        CommandDesc::State cmdState = cmdmgr.findCommandDesc(cmdId).state_;
        if (cmdState != CommandDesc::State::PENDING) {
            if (cmdState == CommandDesc::State::DONE) {
                ofile << cmdmgr.getResult(cmdId);
                ofile.close();
                cout << "\nDone. Result stored in file: "
                     << globalParameters.getOutputFileName() << endl;
                break;
            }
            if (cmdState == CommandDesc::State::EXCEPTION) {
                cout << "\nException!" << endl;
                break;
            }
        }

        string line;
        while (getline(progressStream, line)) {
            if (!line.empty()) {
                try {
                    int percentage = stoi(line);
                    if (percentage != lastPercentage) {
                        cout << "\r" << percentage << "%" << flush;
                        lastPercentage = percentage;
                    }
                } catch (...) {
                    // If not a number, just skip it
                }
            }
        }
        progressStream.clear();

        this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    return 0;
}


