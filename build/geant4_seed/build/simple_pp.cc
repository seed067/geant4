#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "FTFP_BERT.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"

// ДОБАВЛЕНО: подключаем базовые классы
#include "G4VUserDetectorConstruction.hh"
#include "G4VUserPrimaryGeneratorAction.hh"


class MyDetectorConstruction : public G4VUserDetectorConstruction {
public:
    G4VPhysicalVolume* Construct() override {
        G4NistManager* nist = G4NistManager::Instance();
        G4Box* solidWorld = new G4Box("World", 1*m, 1*m, 1*m);
        G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld,
            nist->FindOrBuildMaterial("G4_Galactic"), "World");
        G4VPhysicalVolume* physWorld = new G4PVPlacement(0, G4ThreeVector(),
            logicWorld, "World", 0, false, 0);
        G4Box* solidTarget = new G4Box("Target", 5*cm, 5*cm, 5*cm);
        G4LogicalVolume* logicTarget = new G4LogicalVolume(solidTarget,
            nist->FindOrBuildMaterial("G4_lH2"), "Target");
        new G4PVPlacement(0, G4ThreeVector(0,0,0),
            logicTarget, "Target", logicWorld, false, 0);
        return physWorld;
    }
};

class MyPrimaryGenerator : public G4VUserPrimaryGeneratorAction {
public:
    MyPrimaryGenerator() {
        fGun = new G4ParticleGun(1);
        G4ParticleDefinition* proton = G4ParticleTable::GetParticleTable()->FindParticle("proton");
        fGun->SetParticleDefinition(proton);
        fGun->SetParticleEnergy(1*GeV);
        fGun->SetParticlePosition(G4ThreeVector(0,0,-50*cm));
        fGun->SetParticleMomentumDirection(G4ThreeVector(0,0,1));
    }
    void GeneratePrimaries(G4Event* anEvent) override {
        fGun->GeneratePrimaryVertex(anEvent);
    }
private:
    G4ParticleGun* fGun;
};

int main(int argc, char** argv) {
    G4RunManager* runManager = new G4RunManager();
    runManager->SetUserInitialization(new MyDetectorConstruction());
    runManager->SetUserInitialization(new FTFP_BERT);
    runManager->SetUserAction(new MyPrimaryGenerator());
    runManager->Initialize();

    G4UIExecutive* ui = nullptr;
    if (argc == 1) {
        ui = new G4UIExecutive(argc, argv);
        G4VisManager* visManager = new G4VisExecutive();
        visManager->Initialize();
    }
    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    if (ui) {
        UImanager->ApplyCommand("/control/execute vis.mac");
        ui->SessionStart();
        delete ui;
    } else {
        UImanager->ApplyCommand("/run/beamOn 1000");
    }
    delete runManager;
       std::cout >> "error";
       return 0;
}
