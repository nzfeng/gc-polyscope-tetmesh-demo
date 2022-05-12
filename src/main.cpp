#include "polyscope/polyscope.h"
#include "polyscope/volume_mesh.h"

#include "args/args.hxx"
#include "imgui.h"

#include <igl/readMESH.h>
#include <igl/volume.h>

// tetmesh stuff
Eigen::MatrixXd VERTICES; // a |V| x 3 matrix, where the ith row = 3D position of the ith vertex
Eigen::MatrixXi TETS;     // a |T| x 4 matrix, where the ith row = vertex indices of the ith tet
Eigen::MatrixXi FACES;    // a |F| x 3 matrix, where the ith row = vertex indices of the ith face

// Polyscope stuff
polyscope::VolumeMesh* psMesh;

/*
 * Load in the tet mesh.
 */
void readInTetmesh(const std::string& filepath) {

    // Read tetmesh from mesh file.
    igl::readMESH(filepath, VERTICES, TETS, FACES);
}

/*
 * Compute the dual volumes at each vertex, defined as sum of 1/4 * volume of each incident tet.
 */
Eigen::VectorXd computeVertexDualVolumes() {

    // We could also easily compute the volume of each tet by ourselves, but libigl has a function for it, and since
    // we're already using libigl...
    Eigen::VectorXd tetVolumes;
    igl::volume(VERTICES, TETS, tetVolumes);

    size_t nVertices = VERTICES.rows();
    size_t nTets = TETS.rows();
    Eigen::VectorXd vertexDualVolumes = Eigen::VectorXd::Zero(nVertices);

    for (size_t i = 0; i < nTets; i++) {
        double vol = tetVolumes(i);
        // Add this tet's contribution to vertex dual volumes.
        Eigen::Vector4i vIndices = TETS.row(i);
        for (size_t idx = 0; idx < 4; idx++) {
            vertexDualVolumes(vIndices[idx]) += vol / 4;
        }
    }
    return vertexDualVolumes;
}

void functionCallback() {

    if (ImGui::Button("Compute vertex dual volumes")) {
        Eigen::VectorXd vertexDualVolumes = computeVertexDualVolumes();
        psMesh->addVertexScalarQuantity("vertex dual volumes", vertexDualVolumes);
    }
}

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
    std::string meshFilepath = "../data/armadillo.mesh";
    if (meshFilename) {
        meshFilepath = args::get(meshFilename);
    }

    // Read in tetmesh.
    readInTetmesh(meshFilepath);

    // Initialize polyscope
    polyscope::init();

    // Display tetmesh
    psMesh = polyscope::registerTetMesh("tetmesh", VERTICES, TETS);

    // Set the callback function
    polyscope::state::userCallback = functionCallback;

    // Give control to the polyscope gui
    polyscope::show();

    return EXIT_SUCCESS;
}