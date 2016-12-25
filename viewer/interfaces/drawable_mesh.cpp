#include "drawable_mesh.h"

#ifdef __APPLE__
#include <gl.h>
#else
#include <GL/gl.h>
#endif

void DrawableMesh::init() {
    drawMode          = DRAW_MESH | DRAW_SMOOTH | DRAW_VERTEXCOLOR;
    wireframeWidth    = 1;
    wireframeColor[0] = 0.1;
    wireframeColor[1] = 0.1;
    wireframeColor[2] = 0.1;
}

void DrawableMesh::draw() const {
    if (drawMode & DRAW_MESH) {
        if (drawMode & DRAW_WIREFRAME) {
            if (drawMode & DRAW_POINTS) {
                glDisable(GL_LIGHTING);
                glShadeModel(GL_FLAT);
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                glDepthRange(0.0, 1.0);
                renderPass();
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
            else if (drawMode & DRAW_FLAT) {
                glEnable(GL_LIGHTING);
                glShadeModel(GL_FLAT);
                glDepthRange(0.01, 1.0);
                renderPass();

                glDisable(GL_LIGHTING);
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                glDepthRange(0.0, 1.0);
                glDepthFunc(GL_LEQUAL);
                renderPass();
                glDepthFunc(GL_LESS);
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
            else if (drawMode & DRAW_SMOOTH) {
                glEnable(GL_LIGHTING);
                glShadeModel(GL_SMOOTH);
                glDepthRange(0.01, 1.0);
                renderPass();

                glDisable(GL_LIGHTING);
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                glDepthRange(0.0, 1.0);
                glDepthFunc(GL_LEQUAL);
                renderPass();
                glDepthFunc(GL_LESS);
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
        }
        else {
            if (drawMode & DRAW_POINTS) {
                glDisable(GL_LIGHTING);
                renderPass();
            }
            else if (drawMode & DRAW_FLAT) {
                glEnable(GL_LIGHTING);
                glShadeModel(GL_FLAT);
                renderPass();
            }
            else if (drawMode & DRAW_SMOOTH) {
                glEnable(GL_LIGHTING);
                glShadeModel(GL_SMOOTH);
                renderPass();
            }
        }
    }
}

bool DrawableMesh::isVisible() const {
    return (drawMode & DRAW_MESH);
}

void DrawableMesh::setWireframe(bool b) {
    if (b) drawMode |=  DRAW_WIREFRAME;
    else   drawMode &= ~DRAW_WIREFRAME;
}

void DrawableMesh::setFlatShading() {
    drawMode |=  DRAW_FLAT;
    drawMode &= ~DRAW_SMOOTH;
    drawMode &= ~DRAW_POINTS;
}

void DrawableMesh::setSmoothShading() {
    drawMode |=  DRAW_SMOOTH;
    drawMode &= ~DRAW_FLAT;
    drawMode &= ~DRAW_POINTS;
}

void DrawableMesh::setPointsShading() {
    drawMode |=  DRAW_POINTS;
    drawMode &= ~DRAW_FLAT;
    drawMode &= ~DRAW_SMOOTH;
}

void DrawableMesh::setWireframeColor(float r, float g, float b) {
    wireframeColor[0] = r;
    wireframeColor[1] = g;
    wireframeColor[2] = b;
}

void DrawableMesh::setWireframeWidth(float width) {
    wireframeWidth = width;
}

void DrawableMesh::setEnableVertexColor() {
    drawMode |=  DRAW_VERTEXCOLOR;
    drawMode &= ~DRAW_FACECOLOR;
}

void DrawableMesh::setEnableTriangleColor() {
    drawMode |=  DRAW_FACECOLOR;
    drawMode &= ~DRAW_VERTEXCOLOR;
}

void DrawableMesh::setVisible(bool b) {
    if (b) drawMode |=  DRAW_MESH;
    else   drawMode &= ~DRAW_MESH;
}

DrawableMesh::DrawableMesh(const std::vector<double>& coords, const std::vector<int>& triangles, const std::vector<double>& vertexNormals, const std::vector<float>& vertexColors, const std::vector<double>& triangleNormals, const std::vector<float>& triangleColors) :
    pCoords(&coords),
    pTriangles(&triangles),
    pVertexNormals(&vertexNormals),
    pVertexColors(&vertexColors),
    pTriangleNormals(&triangleNormals),
    pTriangleColors(&triangleColors)
  #ifdef COMMON_WITH_EIGEN
  ,
    V(nullptr),
    F(nullptr),
    NV(nullptr),
    CV(nullptr),
    NF(nullptr),
    CF(nullptr)
  #endif
{
    meshType = STD;
    init();
}

#ifdef COMMON_WITH_EIGEN
DrawableMesh::DrawableMesh(const Eigen::Matrix<double, Eigen::Dynamic, 3, Eigen::RowMajor>& V, const Eigen::Matrix<int, Eigen::Dynamic, 3, Eigen::RowMajor>& F, const Eigen::Matrix<double, Eigen::Dynamic, 3, Eigen::RowMajor>& NV, const Eigen::Matrix<float, Eigen::Dynamic, 3, Eigen::RowMajor>& CV, const Eigen::Matrix<double, Eigen::Dynamic, 3, Eigen::RowMajor>& NF, const Eigen::Matrix<float, Eigen::Dynamic, 3, Eigen::RowMajor>& CF) :
    pCoords(nullptr),
    pTriangles(nullptr),
    pVertexNormals(nullptr),
    pVertexColors(nullptr),
    pTriangleNormals(nullptr),
    pTriangleColors(nullptr),
    V(&V),
    F(&F),
    NV(&NV),
    CV(&CV),
    NF(&NF),
    CF(&CF)
{
    meshType = EIGEN;
    init();
}
#endif

void DrawableMesh::renderPass() const {
    if (meshType == STD)
        renderPass(pCoords->size()/3, pTriangles->size()/3, pCoords->data(), pTriangles->data(), pVertexNormals->data(), pVertexColors->data(), pTriangleNormals->data(), pTriangleColors->data());
    else if (meshType == EIGEN)
        renderPass(V->rows(), F->rows(), V->data(), F->data(), NV->data(), CV->data(), NF->data(), CF->data());
}

void DrawableMesh::renderPass(unsigned int nv, unsigned int nt, const double* coords, const int* triangles, const double* vertexNormals, const float* vertexColors, const double* triangleNormals, const float* triangleColors) const {
    if (drawMode & DRAW_POINTS) {
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_DOUBLE, 0, coords);

        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer (3, GL_FLOAT, 0, vertexColors);

        glDrawArrays(GL_POINTS, 0, nv);

        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
    }
    else if (drawMode & DRAW_SMOOTH || drawMode & DRAW_FLAT) {
        // Old fashioned, verbose and slow rendering.
        //
        if (drawMode & DRAW_FACECOLOR) {
            int n_tris = nt;
            for(int tid=0; tid<n_tris; ++tid) {
                int tid_ptr  = 3 * tid;
                int vid0     = triangles[tid_ptr + 0];
                int vid1     = triangles[tid_ptr + 1];
                int vid2     = triangles[tid_ptr + 2];
                int vid0_ptr = 3 * vid0;
                int vid1_ptr = 3 * vid1;
                int vid2_ptr = 3 * vid2;

                glBegin(GL_TRIANGLES);
                glColor3fv(&(triangleColors[tid_ptr]));
                glNormal3dv(&(vertexNormals[vid0_ptr]));
                glVertex3dv(&(coords[vid0_ptr]));
                glNormal3dv(&(vertexNormals[vid1_ptr]));
                glVertex3dv(&(coords[vid1_ptr]));
                glNormal3dv(&(vertexNormals[vid2_ptr]));
                glVertex3dv(&(coords[vid2_ptr]));
                glEnd();
            }
        }
        else if (drawMode & DRAW_VERTEXCOLOR){
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(3, GL_DOUBLE, 0, coords);

            glEnableClientState(GL_NORMAL_ARRAY);
            glNormalPointer(GL_DOUBLE, 0, vertexNormals);

            glEnableClientState(GL_COLOR_ARRAY);
            glColorPointer(3, GL_FLOAT, 0, vertexColors);

            glDrawElements(GL_TRIANGLES, nt*3, GL_UNSIGNED_INT, triangles);

            glDisableClientState(GL_COLOR_ARRAY);
            glDisableClientState(GL_NORMAL_ARRAY);
            glDisableClientState(GL_VERTEX_ARRAY);
        }
    }

    if (drawMode & DRAW_WIREFRAME) {
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_DOUBLE, 0, coords);

        glLineWidth(wireframeWidth);
        glColor4fv(wireframeColor);

        glDrawElements(GL_TRIANGLES, nt*3, GL_UNSIGNED_INT, triangles);

        glDisableClientState(GL_VERTEX_ARRAY);
    }
}
