#include "polyscope/polyscope.h"
#include "polyscope/volume_mesh.h"

#include "args/args.hxx"
#include "imgui.h"

#include <igl/readMESH.h>

using namespace geometrycentral;
using namespace geometrycentral::surface;

// tetmesh stuff
Eigen::MatrixXd vertices; // a |V| x 3 matrix, where the ith row = 3D position of the ith vertex
Eigen::MatrixXi tets;     // a |T| x 4 matrix, where the ith row = vertex indices of the ith tet
Eigen::MatrixXi faces;    // a |F| x 3 matrix, where the ith row = vertex indices of the ith face

// Polyscope stuff
polyscope::VolumeMesh* psMesh;

/*
 * Load in the tet mesh.
 */
void readInTetmesh(const std::string& filepath) {

    // Read tetmesh from file.
    igl::readMESH(filepath, VERTICES, TETS, FACES);
}

void functionCallback() {}

int main(int argc, char** argv) {

    // Configure the argument parser
    args::ArgumentParser parser("gc-polyscope-tetmesh-demo");
    args::Positional<std::string> meshFilename(parser, "mesh", "A tetmesh file.");
    // Parse args
    try {
        parser.ParseCLI(argc, argv);
    } catch (args::Help) {
        std::cout << parser;
        return 0;
    } catch (args::ParseError e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }

    // If a mesh name was not given, use default mesh.
    std::string meshFilepath = "../data/hand.tet";
    if (meshFilename) {
        meshFilepath = args::get(meshFilename);
    }

    // Read in tetmesh.
    readInTetmesh(meshFilepath);

    // Initialize polyscope
    polyscope::init();

    // Display tetmesh
    psVolumeMesh = polyscope::registerTetMesh("tetmesh", VERTICES, TETS);

    // Set the callback function
    polyscope::state::userCallback = functionCallback;

    // Give control to the polyscope gui
    polyscope::show();

    return EXIT_SUCCESS;
}