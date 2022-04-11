#include <iostream>
#include <filesystem>
#include "utils.h"
#include "logger.h"

int main() {

    debug("Writing payload to disk...");

    deploy::write_payload("C:\\windows\\system32\\cmd.exe");

    success("Payload successfully wrote to disk!");

    debug("Running payload...");

    if (deploy::run_payload()) {
        success("Payload ran successfully!");
    }
    else {
        error("Fatal error during the execution of the payload.");
    }

    debug("Cleaning up...");

    if (deploy::cleanup()) {
        success("Cleaned up successfully, closing...");
    }
    else {
        error("Fatal error during the cleanup of the payload.");
    }
} 