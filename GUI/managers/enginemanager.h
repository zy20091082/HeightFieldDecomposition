#ifndef ENGINEMANAGER_H
#define ENGINEMANAGER_H

#include <QFrame>
#include <QLabel>
#include "dcel/gui/dcelmanager.h"
#include "lib/grid/drawablegrid.h"
#include "lib/grid/drawableirregulargrid.h"
#include "common.h"
#include "engine/box.h"
#include "engine/energy.h"
#include "engine/engine.h"
#include "common/timer.h"
#include "eigenmesh/gui/drawableeigenmesh.h"
#include "engine/heightfieldslist.h"
#include "engine/splitting.h"


namespace Ui {
    class EngineManager;
}

class EngineManager : public QFrame, public SerializableObject {
        Q_OBJECT

    public:
        explicit EngineManager(QWidget *parent = 0);

        void deleteDrawableObject(DrawableObject* d);
        ~EngineManager();

        void updateLabel(double value, QLabel* label);
        void updateBoxValues();
        void updateColors(double angleThreshold, double areaThreshold);
        Pointd getLimits();

        void saveMSCFile(const std::string &filename, const Dcel &d, const BoxList &bl);

        void serializeBC(const std::string& filename);
        void deserializeBC(const std::string& filename);

        // SerializableObject interface
        void serialize(std::ofstream& binaryFile) const;
        bool deserialize(std::ifstream& binaryFile);

    private slots:
        void on_generateGridPushButton_clicked();

        void on_distanceSpinBox_valueChanged(double arg1);

        void on_targetComboBox_currentIndexChanged(int index);

        void on_kernelRadioButton_toggled(bool checked);

        void on_weigthsRadioButton_toggled(bool checked);

        void on_freezeKernelPushButton_clicked();

        void on_sliceCheckBox_stateChanged(int arg1);

        void on_sliceSlider_valueChanged(int value);

        void on_sliceComboBox_currentIndexChanged(int index);

        void on_serializePushButton_clicked();

        void on_deserializePushButton_clicked();

        void on_wSpinBox_valueChanged(double arg1);

        void on_hSpinBox_valueChanged(double arg1);

        void on_dSpinBox_valueChanged(double arg1);

        void on_plusXButton_clicked();

        void on_minusXButton_clicked();

        void on_plusYButton_clicked();

        void on_minusYButton_clicked();

        void on_plusZButton_clicked();

        void on_minusZButton_clicked();

        void on_energyBoxPushButton_clicked();

        void on_minimizePushButton_clicked();

        void on_BFGSButton_clicked();

        void on_serializeBoxPushButton_clicked();

        void on_deserializeBoxPushButton_clicked();

        void on_iterationsSlider_sliderMoved(int position);

        void on_energyIterationsButton_clicked();

        void on_showAllSolutionsCheckBox_stateChanged(int arg1);

        void on_solutionsSlider_valueChanged(int value);

        void on_setFromSolutionButton_clicked();

        void on_wireframeDcelCheckBox_stateChanged(int arg1);

        void on_pointsDcelRadioButton_toggled(bool checked);

        void on_flatDcelRadioButton_toggled(bool checked);

        void on_smoothDcelRadioButton_toggled(bool checked);

        void on_trianglesCoveredPushButton_clicked();

        void on_stepDrawGridSpinBox_valueChanged(double arg1);

        void on_subtractPushButton_clicked();

        void on_stickPushButton_clicked();

        void on_serializeBCPushButton_clicked();

        void on_deserializeBCPushButton_clicked();

        void on_createAndMinimizeAllPushButton_clicked();

        void on_allHeightfieldsCheckBox_stateChanged(int arg1);

        void on_heightfieldsSlider_valueChanged(int value);

        void on_minXSpinBox_valueChanged(double arg1);

        void on_minYSpinBox_valueChanged(double arg1);

        void on_minZSpinBox_valueChanged(double arg1);

        void on_maxXSpinBox_valueChanged(double arg1);

        void on_maxYSpinBox_valueChanged(double arg1);

        void on_maxZSpinBox_valueChanged(double arg1);

        void on_toleranceSlider_valueChanged(int value);

        void on_areaToleranceSpinBox_valueChanged(double arg1);

        void on_saveObjsButton_clicked();

        void on_cleanAllPushButton_clicked();

        void on_reorderBoxes_clicked();

        void on_loadOriginalPushButton_clicked();

        void on_loadSmoothedPushButton_clicked();

        void on_taubinPushButton_clicked();

        void on_packPushButton_clicked();

        void on_reconstructionPushButton_clicked();

        void on_putBoxesAfterPushButton_clicked();

        void on_snappingPushButton_clicked();

        void on_colorPiecesPushButton_clicked();

        void on_deleteBoxesPushButton_clicked();

        void on_pushButton_clicked();

        void on_limitsConstraintCheckBox_stateChanged(int arg1);

        void on_smartPackingPushButton_clicked();

        void on_explodePushButton_clicked();

        void on_createBoxPushButton_clicked();

        void on_coveredTrianglesPushButton_clicked();

        void on_drawBoxMeshCheckBox_stateChanged(int arg1);

        void on_markerMeshPushButton_clicked();

        void on_splitConnectedComponentsPushButton_clicked();

        void on_globalOptimalOrientationPushButton_clicked();

        void on_experimentButton_clicked();

        void on_createBox2PushButton_clicked();

        void on_restoreBoxesPushButton_clicked();

        void on_pushPriorityBoxButton_clicked();

        void on_clearPriorityPushButton_clicked();

    signals:
        void finished();

    private:
        Ui::EngineManager *ui;
        MainWindow& mainWindow; //puntatore alla mainWindow
        DrawableGrid* g;
        DrawableDcel* d;
        Box3D* b;
        Box3D* b2;
        BoxList* iterations;
        BoxList* solutions;
        DrawableEigenMesh* baseComplex;
        Energy e;
        HeightfieldsList *he;
        DrawableEigenMesh originalMesh;
        DrawableEigenMesh markerMesh;

        BoxList originalSolutions;
        bool alreadySplitted;

        std::map<unsigned int, unsigned int> splittedBoxesToOriginals;
        std::list<unsigned int> priorityBoxes;
};

#endif // ENGINEMANAGER_H
